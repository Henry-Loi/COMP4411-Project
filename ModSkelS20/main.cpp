// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "main.h"

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView *createRobotModel(int x, int y, int w, int h, char *label) {
  return new RobotModel(x, y, w, h, label);
}

void RobotModel::set_model_lighting(void) {
  // set light2 for the model
  float light2_position[] = {VAL(LIGHT_X), VAL(LIGHT_Y), VAL(LIGHT_Z), 1.0};
  float light2_diffuse[] = {VAL(LIGHT_INTENSITY), VAL(LIGHT_INTENSITY),
                            VAL(LIGHT_INTENSITY), 1.0};
  float light2_specular[] = {1.0, 1.0, 1.0, 1.0};
  float light2_ambient[] = {0.0, 0.0, 0.0, 1.0};

  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
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
  setDiffuseColor(COLOR_BLUE);
  glPushMatrix();
  glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

  // // draw cannon
  // glPushMatrix();
  // glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
  // glRotated(-90, 1.0, 0.0, 0.0);
  // drawCylinder(VAL(HEIGHT), 0.1, 0.1);

  // glTranslated(0.0, 0.0, VAL(HEIGHT));
  // drawCylinder(1, 1.0, 0.9);

  // glTranslated(0.0, 0.0, 0.5);
  // glRotated(90, 1.0, 0.0, 0.0);
  // drawCylinder(4, 0.1, 0.2);
  // glPopMatrix();

  // draw head
  glPushMatrix();
  glRotated(VAL(HEAD_ROTATE), 0.0, 1.0, 0.0);
  glTranslated(-1, 3, -0.7);
  drawBox(2, 2, 1.4);

  // draw eyes
  glPushMatrix();
  setDiffuseColor(COLOR_RED);
  glTranslated(0.5, 1, 0);
  drawSphere(0.2);

  glTranslated(1, 0, 0);
  drawSphere(0.2);
  glPopMatrix();

  // draw body
  glPushMatrix();
  setDiffuseColor(COLOR_YELLOW);
  glTranslated(-1, -1.5, 0);
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
  // draw leg
  glPushMatrix();
  setDiffuseColor(COLOR_GREY);
  glTranslated(-0.5, 0.75, 0.7);
  drawSphere(1.5 / 2);

  glPushMatrix();
  setDiffuseColor(COLOR_RED);
  glRotated(90, 1.0, 0.0, 0.0);
  glTranslated(0, 0, 0.7);
  drawCylinder(2, 1.0 / 2, 1.5 / 2);

  glPopMatrix();
  glPopMatrix();
  // end draw leg

  // draw 2nd leg
  glPushMatrix();
  setDiffuseColor(COLOR_GREY);
  glTranslated(1.5, 0.75, 0.7);
  drawSphere(1.5 / 2);

  glPushMatrix();
  setDiffuseColor(COLOR_RED);
  glRotated(90, 1.0, 0.0, 0.0);
  glTranslated(0, 0, 0.7);
  drawCylinder(2, 1.0 / 2, 1.5 / 2);

  glPopMatrix();
  glPopMatrix();
  // end draw 2nd leg

  glPopMatrix();
  // draw arm
  glPushMatrix();
  setDiffuseColor(COLOR_GREY);
  glTranslated(-1.2, 2.25, 0.7);
  drawSphere(1.5 / 2);

  glPushMatrix();
  setDiffuseColor(COLOR_RED);
  glRotated(90, 1.0, 0.0, 0.0);
  glTranslated(0, 0, 0.7);
  drawCylinder(3, 1.0 / 2, 1.5 / 2);

  glPushMatrix();
  setDiffuseColor(COLOR_YELLOW);
  glTranslated(-0.25, -0.5, 3);
  drawBox(0.5, 1, 1);

  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  // end draw arm

  // draw 2nd arm
  glPushMatrix();
  setDiffuseColor(COLOR_GREY);
  glTranslated(4.2, 2.25, 0.7);
  drawSphere(1.5 / 2);

  glPushMatrix();
  setDiffuseColor(COLOR_RED);
  glRotated(90, 1.0, 0.0, 0.0);
  glTranslated(0, 0, 0.7);
  drawCylinder(3, 1.0 / 2, 1.5 / 2);

  glPushMatrix();
  setDiffuseColor(COLOR_YELLOW);
  glTranslated(-0.25, -0.5, 3);
  drawBox(0.5, 1, 1);
  glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  // end draw 2nd arm

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

  // light controls
  controls[LIGHT_X] = ModelerControl("Light X", -5, 5, 0.1f, 0);
  controls[LIGHT_Y] = ModelerControl("Light Y", -5, 5, 0.1f, 0);
  controls[LIGHT_Z] = ModelerControl("Light Z", -5, 5, 0.1f, 0);
  controls[LIGHT_INTENSITY] = ModelerControl("Light Intensity", 0, 1, 0.1f, 0);

  ModelerApplication::Instance()->Init(&createRobotModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}
