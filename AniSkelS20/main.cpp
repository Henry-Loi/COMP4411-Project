// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "main.h"
#include "modelerdraw.h"
#include "texturedraw.h"
#include <GL/gl.h>
#include <array>
#include <cmath>
#include <iostream>

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView *createRobotModel(int x, int y, int w, int h, char *label) {
  return new RobotModel(x, y, w, h, label);
}

void RobotModel::set_model_lighting(int mood) {
  // set light2 for the model
  float light2_position[] = {VAL(LIGHT2_X), VAL(LIGHT2_Y), VAL(LIGHT2_Z), 1.0};
  float light2_diffuse[] = {VAL(LIGHT2_INTENSITY), VAL(LIGHT2_INTENSITY),
                            VAL(LIGHT2_INTENSITY), 1.0};
  float light2_specular[] = {1.0, 1.0, 1.0, 1.0};
  float light2_ambient[] = {0.0, 0.0, 0.0, 1.0};

  switch (mood) {
  case HAPPY:
    light2_diffuse[0] = 1.0;
    light2_diffuse[1] = 1.0;
    light2_diffuse[2] = 0.3;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
    ModelerApplication::Instance()->SetControlValue(LIGHT2_INTENSITY, 1.0);
    break;
  case SAD:
    light2_diffuse[0] = 0.3;
    light2_diffuse[1] = 0.3;
    light2_diffuse[2] = 1.0;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
    ModelerApplication::Instance()->SetControlValue(LIGHT2_INTENSITY, 1.0);
    break;
  case ANGRY:
    light2_diffuse[0] = 1.0;
    light2_diffuse[1] = 0.3;
    light2_diffuse[2] = 0.3;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse);
    ModelerApplication::Instance()->SetControlValue(LIGHT2_INTENSITY, 1.0);
    break;
  case NORMAL:
    ModelerApplication::Instance()->SetControlValue(LIGHT2_INTENSITY, 0);
    break;
  default:
    break;
  }

  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
  glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
}

void RobotModel::set_mood(int state) {

  switch (state) {
  case ANGRY:
    ModelerApplication::Instance()->SetControlValue(FRONTLEG_ROTATE, -60);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDEFEET_ROTATE, -60);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDEFEET_ROTATE, -60);

    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_YAWROTATE, 37);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_YAWROTATE, 37);
    ModelerApplication::Instance()->SetControlValue(L_SYSTEM_GENERATION, 5);
    break;
  case SAD:
    ModelerApplication::Instance()->SetControlValue(FRONTLEG_ROTATE, 13);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_LENGTH, 1);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_LENGTH, 1);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDEFEET_ROTATE, -60);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDEFEET_ROTATE, -60);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_YAWROTATE, 125);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_YAWROTATE,
                                                    125);
    ModelerApplication::Instance()->SetControlValue(L_SYSTEM_GENERATION, 1);
    break;
  case HAPPY:
    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_YAWROTATE, 125);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_YAWROTATE,
                                                    125);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_ROTATE, -45);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_ROTATE, -45);
    ModelerApplication::Instance()->SetControlValue(L_SYSTEM_GENERATION, 5);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_LENGTH, 3.5);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_LENGTH, 3.5);
    break;
  case NORMAL:
    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_ROTATE, 0);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_ROTATE, 0);
    ModelerApplication::Instance()->SetControlValue(FRONTLEG_ROTATE, 0);
    ModelerApplication::Instance()->SetControlValue(LEFTSIDEFEET_ROTATE, 0);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDEFEET_ROTATE, 0);

    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_YAWROTATE, 0);
    ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_YAWROTATE, 0);
    ModelerApplication::Instance()->SetControlValue(L_SYSTEM_GENERATION, 5);
    break;
  default:
    break;
  }
}

void RobotModel::initTextureMap() {
  for (int i = 0; i < NUM_OF_TEXTURES; i++) {
    textureMaps[i] = new TextureMap(texture_list[i]);
  }
}

int forest_flag = 0;
// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotModel
void RobotModel::draw() {
  // This call takes care of a lot of the nasty projection
  // matrix stuff.  Unless you want to fudge directly with the
  // projection matrix, don't bother with this ...
  ModelerView::draw();

  std::array<float, 3> COLOR1 = {0.0, 0.0, 0.0};
  if (VAL(CHARACTER) == 1)
    COLOR1 = {0.0, 0.0, 1.0};
  else
    COLOR1 = {223.0 / 255.0, 89.0 / 255.0, 0.0};
  set_model_lighting(VAL(MOOD));

  set_mood(VAL(MOOD));

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

  // currentCoordinates
  //     y
  //     |
  //     |
  //     z------x

  // currentCoordinates
  //     y
  //     |
  //     |
  //     z------x

  if (VAL(FULL_MOVEMENT) != 0) {
    SETVAL(LEFTSIDEFEET_ROTATE, VAL(BODY_PITCH) / 2);
    SETVAL(RIGHTSIDEFEET_ROTATE, VAL(BODY_PITCH) / 2);
    SETVAL(FRONTLEG_ROTATE, -VAL(BODY_PITCH));
    SETVAL(LEFTSIDELEG_ROTATE, VAL(BODY_PITCH) * -3 / 2);
    SETVAL(RIGHTSIDELEG_ROTATE, VAL(BODY_PITCH) * -3 / 2);
  }
  if (VAL(DISPLAY_GOAL)) {
    glPushMatrix();
    setDiffuseColor(COLOR_RED);
    glTranslated(VAL(GOAL_X), VAL(GOAL_Y), VAL(GOAL_Z));
    drawSphere(0.2);
    glPopMatrix();

    // offset from the leg joint
    float off_x = VAL(XPOS);
    float off_y = VAL(YPOS);
    float off_z = VAL(ZPOS);
    // calculate the direction vector
    float direction[3] = {(VAL(GOAL_Z) - VAL(ZPOS)) * -1,
                          (VAL(GOAL_Y) - VAL(YPOS) - 1),
                          VAL(GOAL_X) - VAL(XPOS) - 2};

    std::cout << direction[1] << direction[2] << std::endl;

    float angle3 = 0;
    float angle4 = M_PI / 2;
    float right_leg_length = 3.5;
    float angle5 = 0;

    // inverse kinematics
    float angle1 = acos((-direction[1] / (sqrt(direction[1] * direction[1] +
                                               direction[2] * direction[2]))));
    float angle2 = acos((direction[0] / (sqrt(direction[2] * direction[2] +
                                              direction[1] * direction[1] +
                                              direction[0] * direction[0]))));
    ;
    float left_leg_length =
        sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
             direction[0] * direction[0]) -
        1.5;
    // float angle3 = atan2(direction[2], direction[1]

    if (sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
             direction[0] * direction[0]) -
            1.5 >
        3.5) {
      left_leg_length = 3.5;

    } else if (left_leg_length < 0.5) {
      left_leg_length = 0.5;
    }
    if (direction[2] < 0) {
      angle1 = 0;
      angle2 = M_PI / 2;
      left_leg_length = 3.5;
      direction[0] = (VAL(GOAL_Z) - VAL(ZPOS)) * -1;
      direction[1] = (VAL(GOAL_Y) - VAL(YPOS) - 1);
      direction[2] = VAL(GOAL_X) - VAL(XPOS) + 2;
      if (direction[2] < 0) {
        angle3 = acos((-direction[1] / (sqrt(direction[1] * direction[1] +
                                             direction[2] * direction[2]))));
        angle4 = acos((direction[0] / (sqrt(direction[2] * direction[2] +
                                            direction[1] * direction[1] +
                                            direction[0] * direction[0]))));
        ;
        right_leg_length =
            sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
                 direction[0] * direction[0]) -
            1.5;
        if (right_leg_length > 3.5)
          right_leg_length = 3.5;
        if (right_leg_length < 0.5)
          right_leg_length = 0.5;
      } else {
        direction[0] = (VAL(GOAL_Z) - VAL(ZPOS) - 1) * -1;
        direction[1] = (VAL(GOAL_Y) - VAL(YPOS) + 2.75);
        angle5 = acos((-direction[0] / (sqrt(direction[0] * direction[0] +
                                             direction[1] * direction[1])))) -
                 M_PI / 2;
        if (angle5 > M_PI / 3)
          angle5 = M_PI / 3;
        else if (angle5 < -M_PI / 6)
          angle5 = -M_PI / 6;
      }
    }

    std::cout << ((angle1)*180.0) / M_PI << "ANGLE1" << std::endl;

    SETVAL(LEFTSIDELEG_YAWROTATE, (((angle1)*180.0)) / M_PI);

    SETVAL(LEFTSIDELEG_ROTATE, 90 - 1 * (angle2 * 180) / M_PI);
    SETVAL(LEFTSIDELEG_LENGTH, left_leg_length);
    SETVAL(RIGHTSIDELEG_YAWROTATE, (((angle3)*180.0)) / M_PI);

    SETVAL(RIGHTSIDELEG_ROTATE, 90 - 1 * (angle4 * 180) / M_PI);
    SETVAL(RIGHTSIDELEG_LENGTH, right_leg_length);
    SETVAL(FRONTLEG_ROTATE, (((angle5)*180.0)) / M_PI);
  }

  float sideLeftLegangle = VAL(LEFTSIDELEG_ROTATE);
  float sideRightLegangle = -VAL(RIGHTSIDELEG_ROTATE);
  float sideLeftFeetangle = VAL(LEFTSIDEFEET_ROTATE);
  float sideRightFeetangle = -VAL(RIGHTSIDEFEET_ROTATE);
  float frontLegangle = VAL(FRONTLEG_ROTATE);
  float torsoAngle = VAL(BODY_PITCH);
  // draw torso and waist (root)
  //------------------------------------------------//
  setAmbientColor(0.8f, 0.8f, 0.8f);

  setDiffuseColor(1.0f, 1.0f, 1.0f);
  glRotated(VAL(BODY_ROTATE), 0.0, 1.0, 0.0);
  glRotated(90 + torsoAngle, 1.0, 0.0, 0.0);
  glTranslated(0, 0, -2);
  // drawCylinder(4, 2, 2);
  drawTextureCylinder(textureMaps[TEXTURE_BRICK], 4, 2.05, 2.05);
  setDiffuseColor(0.3f, 0.3f, 0.3f);
  if (VAL(LEVELOF_DETAILS) > 0)
    drawCylinder(0.5, 2.1, 2.1);
  glTranslated(0, 0, 4);
  setDiffuseColor(1.0f, 1.0f, 1.0f);
  if (VAL(LEVELOF_DETAILS) > 0)
    drawCylinder(0.5, 1.9, 1.5);
  glTranslated(0, 0, -2);
  glRotated(-90, 1.0, 0.0, 0.0);
  //------------------------------------------------//

  setDiffuseColor(COLOR_GREEN);
  glPushMatrix();
  glTranslated(0, -0.5, 2.0);
  drawTorus(0.2, 1.0);
  glPopMatrix();
  // draw head
  //------------------------------------------------//
  glPushMatrix();

  glTranslated(0, 2, 0);
  setDiffuseColor(COLOR1[0], COLOR1[1], COLOR1[2]);
  glRotated(VAL(HEAD_ROTATE), 0.0, 1.0, 0.0);
  if (VAL(LEVELOF_DETAILS) > 1)
    if (VAL(CHARACTER) == 1)
      drawSphere(2);
    else {
      glRotated(-90.0, 1.0, 0.0, 0.0);
      drawCylinder(2, 2, 1.5);
      glRotated(90.0, 1.0, 0.0, 0.0);
    }
  // draw eyes
  glTranslated(0, 0.3, 0);
  glRotated(5, 0.0, 1.0, 0.0);
  glTranslated(0, 0, 1.5);
  setDiffuseColor(COLOR_RED);
  if (VAL(LEVELOF_DETAILS) > 2)
    drawCylinder(0.5, 0.2, 0.2);
  glTranslated(0, 0, -1.5);

  glRotated(15, 0.0, 1.0, 0.0);
  setDiffuseColor(0.2f, 0.2f, 0.2f);
  glTranslated(0, 0.1, 1.5);
  if (VAL(LEVELOF_DETAILS) > 2)
    drawCylinder(0.8, 0.2, 0.2);
  glPopMatrix();
  //------------------------------------------------//

  // draw front leg
  //------------------------------------------------//
  glPushMatrix();
  glRotated(180, 1.0, 0.0, 0.0);
  glRotated(90, 0.0, 1.0, 0.0);
  glTranslated(1.0, 2.75, -0.5);
  // glRotated(VAL(FRONTLEG_ROTATE), 0.0, 0.0, 1.0);
  glRotated(frontLegangle, 0.0, 0.0, 1.0);
  setDiffuseColor(COLOR_BLUE);
  if (VAL(LEVELOF_DETAILS) > 1)
    drawCylinder(1.0, 0.5, 0.5);
  glTranslated(-0.75, 0.25, -0.25);
  setDiffuseColor(1.0f, 1.0f, 1.0f);
  if (VAL(LEVELOF_DETAILS) > 2)
    drawBox(1.5, 0.9, 1.5);

  glPopMatrix();
  //------------------------------------------------//

  // draw left side leg
  //------------------------------------------------//
  glPushMatrix();
  glRotated(90, 0.0, 1.0, 0.0);

  glTranslated(0.0, 1.0, 2);
  glRotated(-VAL(LEFTSIDELEG_YAWROTATE), 1.0, 0.0, 0.0);
  setDiffuseColor(1.0f, 1.0f, 1.0f);
  if (VAL(LEVELOF_DETAILS) > 1)
    if (VAL(CHARACTER) == 1)
      drawCylinder(1, 1.0, 1.0);
    else {
      glTranslated(0.0, 0.0, 0.5);
      drawSphere(1);
      glTranslated(0.0, 0.0, -0.5);
    }
  glRotated(180 + sideLeftLegangle, 0.0, 0.0, 1.0);
  glTranslated(-1.0, 0.0, 0.25);
  if (VAL(LEVELOF_DETAILS) > 2)
    if (VAL(CHARACTER) == 1)
      drawBox(2, 1.5, 0.75);
  setDiffuseColor(COLOR1[0], COLOR1[1], COLOR1[2]);
  glTranslated(0.25, 0.5, 0.15);
  if (VAL(LEVELOF_DETAILS) > 3)
    drawBox(1.5, VAL(LEFTSIDELEG_LENGTH), 0.5);
  glTranslated(0.75, VAL(LEFTSIDELEG_LENGTH), 0.0);
  if (VAL(LEVELOF_DETAILS) > 3)
    drawCylinder(0.5, 0.75, 0.75);
  glRotated(sideLeftFeetangle, 0.0, 0.0, 1.0);
  glTranslated(-0.75, 0.0, -0.5);
  setDiffuseColor(1.0f, 1.0f, 1.0f);
  if (VAL(LEVELOF_DETAILS) > 4)
    if (VAL(CHARACTER) == 1)
      drawBox(1.5, 0.9, 1.5);
    else {
      setDiffuseColor(0.2f, 0.2f, 0.2f);
      glTranslated(0.75, 0.25, 0.25);
      drawCylinder(0.9, 0.75, 0.75);
    }
  glPopMatrix();
  //------------------------------------------------//

  // draw right side leg
  //------------------------------------------------//
  glPushMatrix();
  glRotated(-90, 0.0, 1.0, 0.0);

  glTranslated(0.0, 1.0, 2);
  glRotated(-VAL(RIGHTSIDELEG_YAWROTATE), 1.0, 0.0, 0.0);
  setDiffuseColor(1.0f, 1.0f, 1.0f);
  if (VAL(LEVELOF_DETAILS) > 1)
    if (VAL(CHARACTER) == 1)
      drawCylinder(1, 1.0, 1.0);
    else {
      glTranslated(0.0, 0.0, 0.5);
      drawSphere(1);
      glTranslated(0.0, 0.0, -0.5);
    }
  glRotated(180 + sideRightLegangle, 0.0, 0.0, 1.0);
  glTranslated(-1.0, 0.0, 0.25);
  if (VAL(LEVELOF_DETAILS) > 2)
    if (VAL(CHARACTER) == 1)
      drawBox(2, 1.5, 0.75);
  setDiffuseColor(COLOR1[0], COLOR1[1], COLOR1[2]);
  glTranslated(0.25, 0.5, 0.15);
  if (VAL(LEVELOF_DETAILS) > 3)
    drawBox(1.5, VAL(RIGHTSIDELEG_LENGTH), 0.5);
  glTranslated(0.75, VAL(RIGHTSIDELEG_LENGTH), 0.0);
  if (VAL(LEVELOF_DETAILS) > 3)
    drawCylinder(0.5, 0.75, 0.75);
  glRotated(sideRightFeetangle, 0.0, 0.0, 1.0);
  glTranslated(-0.75, 0.0, -0.5);
  setDiffuseColor(1.0f, 1.0f, 1.0f);
  if (VAL(LEVELOF_DETAILS) > 4)
    if (VAL(CHARACTER) == 1)
      drawBox(1.5, 0.9, 1.5);
    else {
      setDiffuseColor(0.2f, 0.2f, 0.2f);
      glTranslated(0.75, 0.25, 0.25);
      drawCylinder(0.9, 0.75, 0.75);
    }
  glPopMatrix();
  //------------------------------------------------//
  if (VAL(LEVELOF_DETAILS) > 5 && VAL(L_SYSTEM_DISPLAY) == 1) {
    glPushMatrix();
    setDiffuseColor(COLOR_GREEN);
    glTranslated(0, -0.5, 2.0);
    drawTorus(0.2, 1.0);
    glTranslated(0, -0.5, 0.1);
    drawLSystemTree(30, 0.2, 0.01);
    glPopMatrix();
  }
  glPushMatrix();
  glTranslated(4, 4, 0);
  drawMetaBall();
  glPopMatrix();

  if (VAL(SHOCKWAVE) == 1) {
    glPushMatrix();
    setDiffuseColorwithAlpha(0.3f, 0.8f, 1.0f, 0.5);
    glRotated(180, 0.0, 1.0, 0.0);
    glTranslated(0.0, 2.25, -4.0);
    drawComplexShape();
    glDisable(GL_BLEND); // Disable blending after rendering
    glPopMatrix();
  }
  // goal display

  if (forest_flag) {
    // set yellow color
    setDiffuseColor(1.0f, 1.0f, 0.0f);
    // random display of trees
    glScaled(2, 2, 2);
    glTranslated(0, -5, 0);
    for (int i = 0; i < 10; i++) {
      glPushMatrix();
      glTranslated(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
      drawLSystemTree(120, 0.2, 0.01);
      glPopMatrix();
    }
  }
}

int main() {
  // Initialize the controls
  // Constructor is ModelerControl(name, minimumvalue, maximumvalue,
  // stepsize, defaultvalue)
  ModelerControl controls[NUMCONTROLS];
  controls[LEVELOF_DETAILS] =
      ModelerControl("Change Level of Detail", 0, 6, 1, 6);

  controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
  controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
  controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
  controls[HEAD_ROTATE] = ModelerControl("Head Rotate", -135, 135, 1, 0);
  controls[FRONTLEG_ROTATE] = ModelerControl("Front Leg Rotate", -60, 30, 1, 0);
  controls[LEFTSIDELEG_ROTATE] =
      ModelerControl("Left Side Leg Rotate", -180.0, 180.0, 0.1, 0);
  controls[RIGHTSIDELEG_ROTATE] =
      ModelerControl("Right Side Leg Rotate", -180.0, 180.0, 0.1, 0);
  controls[LEFTSIDELEG_LENGTH] =
      ModelerControl("Left Side Leg Length", 1, 3.5, 0.1f, 3.5);
  controls[RIGHTSIDELEG_LENGTH] =
      ModelerControl("Right Side Leg Length", 1, 3.5, 0.1f, 3.5);
  controls[LEFTSIDEFEET_ROTATE] =
      ModelerControl("Left Side Feet Rotate", -180, 180, 0.1, 0);
  controls[RIGHTSIDEFEET_ROTATE] =
      ModelerControl("Right Side Feet Rotate", -180, 180, 1, 0);
  controls[RIGHTSIDELEG_YAWROTATE] =
      ModelerControl("Right Side Yaw Rotate", -180, 180, 1, 0);
  controls[LEFTSIDELEG_YAWROTATE] =
      ModelerControl("Left Side Yaw Rotate", -180, 180, 1, 0);
  controls[FULL_MOVEMENT] = ModelerControl("Full Movement", -30, 45, 1, 0);

  controls[BODY_PITCH] = ModelerControl("Body Pitch Angle", -30, 45, 1, 0);
  controls[FULL_MOVEMENT] = ModelerControl("Full Movement", 0, 1, 1, 0);

  controls[BODY_ROTATE] = ModelerControl("Body Rotate", -180, 180, 1, 0);
  controls[XSCALE] = ModelerControl("X Scale", 0.1, 2, 0.1f, 1);
  controls[YSCALE] = ModelerControl("Y Scale", 0.1, 2, 0.1f, 1);
  controls[ZSCALE] = ModelerControl("Z Scale", 0.1, 2, 0.1f, 1);

  // texture mapping control
  controls[TEXTURE_MAPPING] = ModelerControl("Texture Mapping", 0, 1, 1, 0);

  // L System control
  controls[L_SYSTEM_DISPLAY] = ModelerControl("L System Display", 0, 1, 1, 1);
  controls[L_SYSTEM_GENERATION] =
      ModelerControl("L System Generation", 1, 5, 1, 5);

  // Metaballs control
  controls[METABALLS_THRESHOLD] =
      ModelerControl("Metaballs Threshold", 0.1, 10, 0.1f, 2.0);
  controls[METABALLS_STEP] = ModelerControl("Metaballs Step", -10, 10, 0.5f, 0);

  // mood
  controls[MOOD] =
      ModelerControl("Mood (Normal, HAPPY, SAD, ANGRY)", 0, 4, 1, 0);

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
  controls[SHOCKWAVE] = ModelerControl("SHOCKWAVE", 0, 1, 1, 0);
  controls[CHARACTER] = ModelerControl("Character", 1, 2, 1, 1);

  // goal-oriented control
  controls[DISPLAY_GOAL] = ModelerControl("Display Goal", 0, 1, 1, 0);
  controls[GOAL_X] = ModelerControl("Goal X", -8, 8, 0.1f, 4);
  controls[GOAL_Y] = ModelerControl("Goal Y", -8, 8, 0.1f, 0);
  controls[GOAL_Z] = ModelerControl("Goal Z", -8, 8, 0.1f, 0);

  ModelerApplication::Instance()->Init(&createRobotModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}
