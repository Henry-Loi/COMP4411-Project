// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "main.h"
#include "modelerdraw.h"
#include "texturedraw.h"

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView *createRobotModel(int x, int y, int w, int h, char *label) {
  return new RobotModel(x, y, w, h, label);
}

void RobotModel::set_model_lighting(void) {
  // set light2 for the model
  float light2_position[] = {VAL(LIGHT2_X), VAL(LIGHT2_Y), VAL(LIGHT2_Z), 1.0};
  float light2_diffuse[] = {VAL(LIGHT2_INTENSITY), VAL(LIGHT2_INTENSITY),
                            VAL(LIGHT2_INTENSITY), 1.0};
  float light2_specular[] = {1.0, 1.0, 1.0, 1.0};
  float light2_ambient[] = {0.0, 0.0, 0.0, 1.0};

  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
}

void RobotModel::initTextureMap() {
  for (int i = 0; i < NUM_OF_TEXTURES; i++) {
    textureMaps[i] = new TextureMap(texture_list[i]);
  }
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotModel
void RobotModel::draw() {
  // This call takes care of a lot of the nasty projection
  // matrix stuff.  Unless you want to fudge directly with the
  // projection matrix, don't bother with this ...
  ModelerView::draw();

  set_model_lighting();

  // draw the floor
  setAmbientColor(.1f, .1f, .1f);
  setDiffuseColor(COLOR_RED);
  glPushMatrix();
  glTranslated(-5, -4, -5);
  drawBox(10, 0.01f, 10);
  glPopMatrix();

  // draw a robot model
  setAmbientColor(.1f, .1f, .1f);


  glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

  glScaled(VAL(XSCALE), VAL(YSCALE), VAL(ZSCALE));

  // glTranslated(0.0, 0.0, 0.5);
  // glRotated(90, 1.0, 0.0, 0.0);
  // drawCylinder(4, 0.1, 0.2);
  // glPopMatrix();

  // draw eyes
  glPushMatrix();
  setDiffuseColor(COLOR_RED);
  glTranslated(0.5, 1, 0);
  drawSphere(0.2);


  //currentCoordinates
  //    y
  //    |
  //    |
  //    z------x
 
  //draw torso and waist (root)
  setDiffuseColor(COLOR_YELLOW);
  glRotated(90, 0.0, 1.0, 0.0);
  glTranslated(-2, 2, -0.7);
  drawBox(4, 1.5, 1.4);
  glTranslated(0.5, -1.5, 0);
  drawBox(3, 1.5, 1.4);
  // draw waist
  glPushMatrix();
  setDiffuseColor(COLOR_BLUE);
  glTranslated(0, -0.5, 0);
  drawBox(3, 0.5, 1.4);
  glTranslated(1, -1.5, 0);
  drawBox(1, 1.5, 1.4);
  glTranslated(0.5, 3.5, 0.7);


  // draw head
  glPushMatrix();
  setDiffuseColor(COLOR_BLUE);
  glRotated(VAL(HEAD_ROTATE), 0.0, 1.0, 0.0);
  glTranslated(-1, 1.5, -0.7);
  drawBox(2, 2, 1.4);

  // draw eyes
  setDiffuseColor(COLOR_RED);
  glTranslated(0.5, 1, 1.4);
  drawSphere(0.2);
  glTranslated(1, 0, 0);
  drawSphere(0.2);
  glPopMatrix();

  ////draw leg
  //glPushMatrix();
  //setDiffuseColor(COLOR_GREY);
  //glTranslated(-0.5, 0.75, 0.7);
  //drawSphere(1.5 / 2);

  //glPushMatrix();
  //setDiffuseColor(COLOR_RED);
  //glRotated(90, 1.0, 0.0, 0.0);
  //glTranslated(0, 0, 0.7);
  //drawCylinder(2, 1.0 / 2, 1.5 / 2);

  //glPopMatrix();
  //glPopMatrix();
  ////end draw leg

  ////draw 2nd leg
  //glPushMatrix();
  //setDiffuseColor(COLOR_GREY);
  //glTranslated(1.5, 0.75, 0.7);
  //drawSphere(1.5 / 2);

  //glPushMatrix();
  //setDiffuseColor(COLOR_RED);
  //glRotated(90, 1.0, 0.0, 0.0);
  //glTranslated(0, 0, 0.7);
  //drawCylinder(2, 1.0 / 2, 1.5 / 2);

  //glPopMatrix();
  //glPopMatrix();
  ////end draw 2nd leg


  //glPopMatrix();
  ////draw arm
  //glPushMatrix();
  //setDiffuseColor(COLOR_GREY);
  //glTranslated(-1.2, 2.25, 0.7);
  //drawSphere(1.5/2);

  //glPushMatrix();
  //setDiffuseColor(COLOR_RED);
  //glRotated(90, 1.0, 0.0, 0.0);
  //glTranslated(0, 0, 0.7);
  //drawCylinder(3,1.0/2,1.5/2);
  //
  //glPushMatrix();
  //setDiffuseColor(COLOR_YELLOW);
  //glTranslated(-0.25, -0.5, 3);
  //drawBox(0.5, 1, 1);

  //glPopMatrix();
  //glPopMatrix();
  //glPopMatrix();
  ////end draw arm

  ////draw 2nd arm
  //glPushMatrix();
  //setDiffuseColor(COLOR_GREY);
  //glTranslated(4.2, 2.25, 0.7);
  //drawSphere(1.5 / 2);

  //glPushMatrix();
  //setDiffuseColor(COLOR_RED);
  //glRotated(90, 1.0, 0.0, 0.0);
  //glTranslated(0, 0, 0.7);
  //drawCylinder(3, 1.0 / 2, 1.5 / 2);
 
  //glPushMatrix();
  //setDiffuseColor(COLOR_YELLOW);
  //glTranslated(-0.25, -0.5, 3);
  //drawBox(0.5, 1, 1);
  //glPopMatrix();
  //glPopMatrix();
  //glPopMatrix();
  //end draw 2nd arm

  glPopMatrix();

  glPopMatrix();

  // draw mouth
  //   setDiffuseColor(COLOR_GREEN);
  //   glPushMatrix();
  //   glTranslated(0, -0.5, 0.5);
  //   glScaled(1, 0.5, 0.5);
  //   drawBox(0.5, 0.5, 0.5);
  //   glPopMatrix();

  //   // draw body
  //   glPushMatrix();
  //   glTranslated(0.0, 0.0, -1.5);
  //   glScaled(1, 1, 2);
  //   drawSphere(1);
  //   glPopMatrix();

  //   // draw arms
  //   glPushMatrix();
  //   glTranslated(0.0, 0.0, -1.5);
  //   glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
  //   glTranslated(0.0, 0.0, -1.5);
  //   glScaled(0.5, 0.5, 3);
  //   drawBox(1, 1, 1);

  //   glTranslated(0.0, 0.0, 3);
  //   glRotated(-VAL(ROTATE), 0.0, 1.0, 0.0);
  //   glTranslated(0.0, 0.0, -1.5);
  //   glScaled(0.5, 0.5, 3);
  //   drawBox(1, 1, 1);

  glPopMatrix();
}

int main() {
  // Initialize the controls
  // Constructor is ModelerControl(name, minimumvalue, maximumvalue,
  // stepsize, defaultvalue)
  ModelerControl controls[NUMCONTROLS];
  controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
  controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
  controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
  controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
  controls[HEAD_ROTATE] = ModelerControl("Head Rotate", -135, 135, 1, 0);

  controls[XSCALE] = ModelerControl("X Scale", 0.1, 2, 0.1f, 1);
  controls[YSCALE] = ModelerControl("Y Scale", 0.1, 2, 0.1f, 1);
  controls[ZSCALE] = ModelerControl("Z Scale", 0.1, 2, 0.1f, 1);

  // texture mapping control
  controls[TEXTURE_MAPPING] = ModelerControl("Texture Mapping", 0, 1, 1, 0);

  // L System control
  controls[L_SYSTEM_DISPLAY] = ModelerControl("L System Display", 0, 1, 1, 1);
  controls[L_SYSTEM_GENERATION] =
      ModelerControl("L System Generation", 1, 5, 1, 5);

  // light controls
  controls[LIGHT0_X] = ModelerControl("Light0 X", -5, 5, 0.1f, 4);
  controls[LIGHT0_Y] = ModelerControl("Light0 Y", -5, 5, 0.1f, 2);
  controls[LIGHT0_Z] = ModelerControl("Light0 Z", -5, 5, 0.1f, -4);
  controls[LIGHT0_INTENSITY] =
      ModelerControl("Light 0 Intensity", 0, 1, 0.1f, 1);

  controls[LIGHT1_X] = ModelerControl("Light1 X", -5, 5, 0.1f, -2);
  controls[LIGHT1_Y] = ModelerControl("Light1 Y", -5, 5, 0.1f, 1);
  controls[LIGHT1_Z] = ModelerControl("Light1 Z", -5, 5, 0.1f, 5);
  controls[LIGHT1_INTENSITY] =
      ModelerControl("Light 1 Intensity", 0, 1, 0.1f, 1);

  controls[LIGHT2_X] = ModelerControl("Light2 X", -5, 5, 0.1f, 0);
  controls[LIGHT2_Y] = ModelerControl("Light2 Y", -5, 5, 0.1f, 0);
  controls[LIGHT2_Z] = ModelerControl("Light2 Z", -5, 5, 0.1f, 0);
  controls[LIGHT2_INTENSITY] =
      ModelerControl("Light 2 Intensity", 0, 1, 0.1f, 0);

  ModelerApplication::Instance()->Init(&createRobotModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}
