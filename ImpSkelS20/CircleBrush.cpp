//
// CircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "CircleBrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void drawFilledCircle(GLfloat x, GLfloat y, GLuint radius) {
  int i;
  int triangleAmount = 20; // # of triangles used to draw circle

  // GLfloat radius = 0.8f; //radius
  GLfloat twicePi = 2.0f * PI;

  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y); // center of circle
  for (i = 0; i <= triangleAmount; i++) {
    glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
               y + (radius * sin(i * twicePi / triangleAmount)));
  }
  glEnd();
}

// Further Development: we may need it for a new brush
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
  int i;
  int lineAmount = 100; // # of triangles used to draw circle

  // GLfloat radius = 0.8f; //radius
  GLfloat twicePi = 2.0f * PI;

  glBegin(GL_LINE_LOOP);
  for (i = 0; i <= lineAmount; i++) {
    glVertex2f(x + (radius * cos(i * twicePi / lineAmount)),
               y + (radius * sin(i * twicePi / lineAmount)));
  }
  glEnd();
}

void CircleBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int size = pDoc->getSize();

  glPointSize((float)size);

  BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("CircleBrush::BrushMove  document is NULL\n");
    return;
  }

  SetAlpha(source, pDoc->getAlpha());

  drawFilledCircle(target.x, target.y, pDoc->getSize() / 2);
}

void CircleBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
