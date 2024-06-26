#include "modelerdraw.h"
#include <FL/gl.h>
#include <FL/glut.h>
#include <GL/glu.h>
#include <cstdio>
#include <math.h>
#include "vec.h"
#include "mat.h"
#include "modelerapp.h"
#include "modelerglobals.h"

// ********************************************************
// Support functions from previous version of modeler
// ********************************************************
void _dump_current_modelview(void) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  if (mds->m_rayFile == NULL) {
    fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
    exit(-1);
  }

  GLdouble mv[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, mv);
  fprintf(mds->m_rayFile,
          "transform(\n    (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n     "
          "(%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n",
          mv[0], mv[4], mv[8], mv[12], mv[1], mv[5], mv[9], mv[13], mv[2],
          mv[6], mv[10], mv[14], mv[3], mv[7], mv[11], mv[15]);
}

void _dump_current_material(void) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  if (mds->m_rayFile == NULL) {
    fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
    exit(-1);
  }

  fprintf(mds->m_rayFile,
          "material={\n    diffuse=(%f,%f,%f);\n    ambient=(%f,%f,%f);\n}\n",
          mds->m_diffuseColor[0], mds->m_diffuseColor[1],
          mds->m_diffuseColor[2], mds->m_diffuseColor[0],
          mds->m_diffuseColor[1], mds->m_diffuseColor[2]);
}

// ****************************************************************************

// Initially assign singleton instance to NULL
ModelerDrawState *ModelerDrawState::m_instance = NULL;

ModelerDrawState::ModelerDrawState() : m_drawMode(NORMAL), m_quality(MEDIUM) {
  float grey[] = {.5f, .5f, .5f, 1};
  float white[] = {1, 1, 1, 1};
  float black[] = {0, 0, 0, 1};

  memcpy(m_ambientColor, black, 4 * sizeof(float));
  memcpy(m_diffuseColor, grey, 4 * sizeof(float));
  memcpy(m_specularColor, white, 4 * sizeof(float));

  m_shininess = 0.5;

  m_rayFile = NULL;
}

// CLASS ModelerDrawState METHODS
ModelerDrawState *ModelerDrawState::Instance() {
  // Return the singleton if it exists, otherwise, create it
  return (m_instance) ? (m_instance) : m_instance = new ModelerDrawState();
}

// ****************************************************************************
// Modeler functions for your use
// ****************************************************************************
// Set the current material properties

void setAmbientColor(float r, float g, float b) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  mds->m_ambientColor[0] = (GLfloat)r;
  mds->m_ambientColor[1] = (GLfloat)g;
  mds->m_ambientColor[2] = (GLfloat)b;
  mds->m_ambientColor[3] = (GLfloat)1.0;

  if (mds->m_drawMode == NORMAL)
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mds->m_ambientColor);
}

void setDiffuseColor(float r, float g, float b) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  mds->m_diffuseColor[0] = (GLfloat)r;
  mds->m_diffuseColor[1] = (GLfloat)g;
  mds->m_diffuseColor[2] = (GLfloat)b;
  mds->m_diffuseColor[3] = (GLfloat)1.0;

  if (mds->m_drawMode == NORMAL)
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mds->m_diffuseColor);
  else
    glColor3f(r, g, b);
}

void setSpecularColor(float r, float g, float b) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  mds->m_specularColor[0] = (GLfloat)r;
  mds->m_specularColor[1] = (GLfloat)g;
  mds->m_specularColor[2] = (GLfloat)b;
  mds->m_specularColor[3] = (GLfloat)1.0;

  if (mds->m_drawMode == NORMAL)
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mds->m_specularColor);
}

void setShininess(float s) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  mds->m_shininess = (GLfloat)s;

  if (mds->m_drawMode == NORMAL)
    glMaterialf(GL_FRONT, GL_SHININESS, mds->m_shininess);
}

void setDrawMode(DrawModeSetting_t drawMode) {
  ModelerDrawState::Instance()->m_drawMode = drawMode;
}

void setQuality(QualitySetting_t quality) {
  ModelerDrawState::Instance()->m_quality = quality;
}

bool openRayFile(const char rayFileName[]) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  fprintf(stderr, "Ray file format output is buggy (ehsu)\n");

  if (!rayFileName)
    return false;

  if (mds->m_rayFile)
    closeRayFile();

  mds->m_rayFile = fopen(rayFileName, "w");

  if (mds->m_rayFile != NULL) {
    fprintf(mds->m_rayFile, "SBT-raytracer 1.0\n\n");
    fprintf(mds->m_rayFile, "camera { fov=30; }\n\n");
    fprintf(
        mds->m_rayFile,
        "directional_light { direction=(-1,-1,-1); color=(0.7,0.7,0.7); }\n\n");
    return true;
  } else
    return false;
}

void _setupOpenGl() {
  ModelerDrawState *mds = ModelerDrawState::Instance();
  switch (mds->m_drawMode) {
  case NORMAL:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    break;
  case FLATSHADE:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    break;
  case WIREFRAME:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glShadeModel(GL_FLAT);
  default:
    break;
  }
}

void closeRayFile() {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  if (mds->m_rayFile)
    fclose(mds->m_rayFile);

  mds->m_rayFile = NULL;
}

void drawSphere(double r) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  _setupOpenGl();

  if (mds->m_rayFile) {
    _dump_current_modelview();
    fprintf(mds->m_rayFile, "scale(%f,%f,%f,sphere {\n", r, r, r);
    _dump_current_material();
    fprintf(mds->m_rayFile, "}))\n");
  } else {
    int divisions;
    GLUquadricObj *gluq;

    switch (mds->m_quality) {
    case HIGH:
      divisions = 32;
      break;
    case MEDIUM:
      divisions = 20;
      break;
    case LOW:
      divisions = 12;
      break;
    case POOR:
      divisions = 8;
      break;
    }

    gluq = gluNewQuadric();
    gluQuadricDrawStyle(gluq, GLU_FILL);
    gluQuadricTexture(gluq, GL_TRUE);
    gluSphere(gluq, r, divisions, divisions);
    gluDeleteQuadric(gluq);
  }
}

void drawBox(double x, double y, double z) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  _setupOpenGl();

  if (mds->m_rayFile) {
    _dump_current_modelview();
    fprintf(mds->m_rayFile, "scale(%f,%f,%f,translate(0.5,0.5,0.5,box {\n", x,
            y, z);
    _dump_current_material();
    fprintf(mds->m_rayFile, "})))\n");
  } else {
    /* remember which matrix mode OpenGL was in. */
    int savemode;
    glGetIntegerv(GL_MATRIX_MODE, &savemode);

    /* switch to the model matrix and scale by x,y,z. */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScaled(x, y, z);

    glBegin(GL_QUADS);

    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);

    glNormal3d(0.0, -1.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 1.0);

    glNormal3d(-1.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 1.0, 1.0);
    glVertex3d(0.0, 1.0, 0.0);

    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 1.0);
    glVertex3d(1.0, 0.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(0.0, 1.0, 1.0);

    glNormal3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, 1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(1.0, 1.0, 0.0);

    glNormal3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);
    glVertex3d(1.0, 1.0, 1.0);
    glVertex3d(1.0, 0.0, 1.0);

    glEnd();

    /* restore the model matrix stack, and switch back to the matrix
    mode we were in. */
    glPopMatrix();
    glMatrixMode(savemode);
  }
}

void drawTextureBox(double x, double y, double z) {
  // NOT IMPLEMENTED, SORRY (ehsu)
}

void drawCylinder(double h, double r1, double r2) {
  ModelerDrawState *mds = ModelerDrawState::Instance();
  int divisions;

  _setupOpenGl();

  switch (mds->m_quality) {
  case HIGH:
    divisions = 32;
    break;
  case MEDIUM:
    divisions = 20;
    break;
  case LOW:
    divisions = 12;
    break;
  case POOR:
    divisions = 8;
    break;
  }

  if (mds->m_rayFile) {
    _dump_current_modelview();
    fprintf(mds->m_rayFile,
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2);
    _dump_current_material();
    fprintf(mds->m_rayFile, "})\n");
  } else {
    GLUquadricObj *gluq;

    /* GLU will again do the work.  draw the sides of the cylinder. */
    gluq = gluNewQuadric();
    gluQuadricDrawStyle(gluq, GLU_FILL);
    gluQuadricTexture(gluq, GL_TRUE);
    gluCylinder(gluq, r1, r2, h, divisions, divisions);
    gluDeleteQuadric(gluq);

    if (r1 > 0.0) {
      /* if the r1 end does not come to a point, draw a flat disk to
          cover it up. */

      gluq = gluNewQuadric();
      gluQuadricDrawStyle(gluq, GLU_FILL);
      gluQuadricTexture(gluq, GL_TRUE);
      gluQuadricOrientation(gluq, GLU_INSIDE);
      gluDisk(gluq, 0.0, r1, divisions, divisions);
      gluDeleteQuadric(gluq);
    }

    if (r2 > 0.0) {
      /* if the r2 end does not come to a point, draw a flat disk to
          cover it up. */

      /* save the current matrix mode. */
      int savemode;
      glGetIntegerv(GL_MATRIX_MODE, &savemode);

      /* translate the origin to the other end of the cylinder. */
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslated(0.0, 0.0, h);

      /* draw a disk centered at the new origin. */
      gluq = gluNewQuadric();
      gluQuadricDrawStyle(gluq, GLU_FILL);
      gluQuadricTexture(gluq, GL_TRUE);
      gluQuadricOrientation(gluq, GLU_OUTSIDE);
      gluDisk(gluq, 0.0, r2, divisions, divisions);
      gluDeleteQuadric(gluq);

      /* restore the matrix stack and mode. */
      glPopMatrix();
      glMatrixMode(savemode);
    }
  }
}
void drawTriangle(double x1, double y1, double z1, double x2, double y2,
                  double z2, double x3, double y3, double z3) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  _setupOpenGl();

  if (mds->m_rayFile) {
    _dump_current_modelview();
    fprintf(mds->m_rayFile,
            "polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); "
            "faces=((0,1,2));\n",
            x1, y1, z1, x2, y2, z2, x3, y3, z3);
    _dump_current_material();
    fprintf(mds->m_rayFile, "})\n");
  } else {
    double a, b, c, d, e, f;

    /* the normal to the triangle is the cross product of two of its edges. */
    a = x2 - x1;
    b = y2 - y1;
    c = z2 - z1;

    d = x3 - x1;
    e = y3 - y1;
    f = z3 - z1;

    glBegin(GL_TRIANGLES);
    glNormal3d(b * f - c * e, c * d - a * f, a * e - b * d);
    glVertex3d(x1, y1, z1);
    glVertex3d(x2, y2, z2);
    glVertex3d(x3, y3, z3);
    glEnd();
  }
}

void drawTorus(double innerRadius, double outerRadius) {

  ModelerDrawState *mds = ModelerDrawState::Instance();
  int divisions;

  _setupOpenGl();

  switch (mds->m_quality) {
  case HIGH:
    divisions = 32;
    break;
  case MEDIUM:
    divisions = 20;
    break;
  case LOW:
    divisions = 12;
    break;
  case POOR:
    divisions = 8;
    break;
  }
  glutSolidTorus(innerRadius, outerRadius, divisions, divisions);
} // modify

void LSystem::draw_system(int generations) {
  if (generations == 1) {
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, distance, 0);
    glEnd();

    glTranslated(0, distance, 0);
    drawSphere(radius);
    glTranslated(0, -distance, 0);

    glBegin(GL_LINES);
    glVertex3f(0, distance, 0);
    glVertex3f(distance, distance * 2, 0);
    glEnd();

    glTranslated(0, distance, 0);
    drawSphere(radius);
    glTranslated(0, -distance, 0);

    glBegin(GL_LINES);
    glVertex3f(0, distance, 0);
    glVertex3f(-distance, distance * 2, 0);
    glEnd();

    glTranslated(0, distance, 0);
    drawSphere(radius);
    glTranslated(0, -distance, 0);
    glPopMatrix();
  } else {
    glPushMatrix();

    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, distance, 0);
    glEnd();

    glTranslated(0, distance, 0);
    drawSphere(radius);
    glRotated(-angle / generations, 0.0, 0.0, 1.0);
    this->draw_system(generations - 1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, distance, 0);
    drawSphere(radius);
    glRotated(angle / generations, 0.0, 0.0, 1.0);
    this->draw_system(generations - 1);
    glPopMatrix();
  }
}

void drawLSystemTree(float angle, float distance, float radius) {
  LSystem tree(angle, distance, radius);
  // Draw the L-system tree
  glLineWidth((GLfloat)4);
  tree.draw_system(VAL(L_SYSTEM_GENERATION));
} // modify

#include "metaballs.h"
void drawMetaBall(void) {
  glScaled(0.3, 0.3, 0.3);
  MetaBalls mb;
  mb.setUpGrid();
  mb.setUpMetaballs();
  mb.evalScalarField();
  mb.draw();
}

void drawComplexShape() {
  // int numTriangles = 100;
  // float height = 3.0;
  // float radius = 1.0;
  // const float angleIncrement = 2 * M_PI / numTriangles;
  // const Vector3 apex = { 0.0f, 0.0f, height };

  // float currentAngle = 0.0f;

  // for (int i = 0; i < numTriangles; i++) {
  //     const float x1 = radius * cos(currentAngle);
  //     const float y1 = radius * sin(currentAngle);
  //     const float x2 = radius * cos(currentAngle + angleIncrement);
  //     const float y2 = radius * sin(currentAngle + angleIncrement);

  //    // Define the vertices of the base
  //    const Vector3 baseVertex1 = { x1, y1, 0.0f }; // Current vertex
  //    const Vector3 baseVertex2 = { x2, y2, 0.0f }; // Next vertex

  //    drawTriangle(baseVertex1.x, baseVertex1.y, baseVertex1.z, baseVertex2.x,
  //    baseVertex2.y, baseVertex2.z,apex.x,apex.y,apex.z);

  //    currentAngle += angleIncrement;
  //}

  const float radius = 1.0;     // Radius of the base
  const float height = 10.0;    // Height of the cone
  const int numSegments = 30;   // Number of segments in the cone base
  const int numGridPoints = 50; // Number of points on the grid
  const float gridSize = 20.0;  // Size of the grid in both x and y directions
  const float increment = gridSize / numGridPoints;
  const float halfGridSize = gridSize / 2.0;

  for (int i = 0; i < numGridPoints - 1; i++) {
    for (int j = 0; j < numGridPoints - 1; j++) {
      // Calculate the (x, y) coordinates of the four corners of the current
      // grid cell
      float x1 = -halfGridSize + i * increment;
      float y1 = -halfGridSize + j * increment;
      float x2 = x1 + increment;
      float y2 = y1 + increment;

      // Calculate the z-values for the function z = cos(sqrt(x^2 + y^2)) -
      // (1/5)sqrt(x^2 + y^2)
      float z1 = cos(2 * sqrt(x1 * x1 + y1 * y1)) -
                 (2.0 / 5.0) * sqrt(x1 * x1 + y1 * y1);
      float z2 = cos(2 * sqrt(x2 * x2 + y1 * y1)) -
                 (2.0 / 5.0) * sqrt(x2 * x2 + y1 * y1);
      float z3 = cos(2 * sqrt(x1 * x1 + y2 * y2)) -
                 (2.0 / 5.0) * sqrt(x1 * x1 + y2 * y2);
      float z4 = cos(2 * sqrt(x2 * x2 + y2 * y2)) -
                 (2.0 / 5.0) * sqrt(x2 * x2 + y2 * y2);

      // Render the two triangles of the current grid cell
      drawTriangle(x1, y1, z1, x2, y1, z2, x1, y2, z3);

      drawTriangle(x1, y2, z3, x2, y1, z2, x2, y2, z4);
    }
  }
}

void setDiffuseColorwithAlpha(float r, float g, float b, float opacity) {
  ModelerDrawState *mds = ModelerDrawState::Instance();

  mds->m_diffuseColor[0] = (GLfloat)r;
  mds->m_diffuseColor[1] = (GLfloat)g;
  mds->m_diffuseColor[2] = (GLfloat)b;
  mds->m_diffuseColor[3] = (GLfloat)opacity; // Set the opacity

  if (mds->m_drawMode == NORMAL) {
    glEnable(GL_BLEND); // Enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mds->m_diffuseColor);

  } else {
    glColor4f(r, g, b, opacity); // Use glColor4f for setting color with opacity
  }
}

