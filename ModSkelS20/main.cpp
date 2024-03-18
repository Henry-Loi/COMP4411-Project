// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include <FL/gl.h>

#include "modelerglobals.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView {
public:
  SampleModel(int x, int y, int w, int h, char *label)
      : ModelerView(x, y, w, h, label) {}

  virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView *createSampleModel(int x, int y, int w, int h, char *label) {
  return new SampleModel(x, y, w, h, label);
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw() {
  // This call takes care of a lot of the nasty projection
  // matrix stuff.  Unless you want to fudge directly with the
  // projection matrix, don't bother with this ...
  ModelerView::draw();

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

  ModelerApplication::Instance()->Init(&createSampleModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}
