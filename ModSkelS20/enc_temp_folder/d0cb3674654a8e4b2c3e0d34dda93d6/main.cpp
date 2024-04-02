// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "main.h"
#include "modelerdraw.h"
#include "texturedraw.h"
#include <cmath>

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

  int sideLeftLegangle = VAL(LEFTSIDELEG_ROTATE);
  int sideRightLegangle = -VAL(RIGHTSIDELEG_ROTATE);
  int sideLeftFeetangle = VAL(LEFTSIDEFEET_ROTATE);
  int sideRightFeetangle = -VAL(RIGHTSIDEFEET_ROTATE);
  int frontLegangle = VAL(FRONTLEG_ROTATE);
  int torsoAngle = 0;
  if (VAL(FULL_MOVEMENT) != 0) {
    sideLeftFeetangle = VAL(FULL_MOVEMENT) / 2;
    sideRightFeetangle = -VAL(FULL_MOVEMENT) / 2;
    frontLegangle = -VAL(FULL_MOVEMENT);
    torsoAngle = VAL(FULL_MOVEMENT);
    sideLeftLegangle = (sideLeftFeetangle + torsoAngle) * -1;
    sideRightLegangle = (sideRightFeetangle - torsoAngle) * -1;
  }

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
  setDiffuseColor(COLOR_BLUE);
  glRotated(VAL(HEAD_ROTATE), 0.0, 1.0, 0.0);
  if (VAL(LEVELOF_DETAILS) > 1)
    drawSphere(2);
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
    drawCylinder(1, 1.0, 1.0);
  glRotated(180 + sideLeftLegangle, 0.0, 0.0, 1.0);
  glTranslated(-1.0, 0.0, 0.25);
  if (VAL(LEVELOF_DETAILS) > 2)
    drawBox(2, 1.5, 0.75);
  setDiffuseColor(COLOR_BLUE);
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
    drawBox(1.5, 0.9, 1.5);
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
    drawCylinder(1, 1.0, 1.0);
  glRotated(180 + sideRightLegangle, 0.0, 0.0, 1.0);
  glTranslated(-1.0, 0.0, 0.25);
  if (VAL(LEVELOF_DETAILS) > 2)
    drawBox(2, 1.5, 0.75);
  setDiffuseColor(COLOR_BLUE);
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
    drawBox(1.5, 0.9, 1.5);
  glPopMatrix();
  //------------------------------------------------//
  if (VAL(LEVELOF_DETAILS) > 5) {
    glPushMatrix();
    setDiffuseColor(COLOR_GREEN);
    glTranslated(0, -0.5, 2.0);
    drawTorus(0.2, 1.0);
    glTranslated(0, -0.5, 0.1);
    drawLSystemTree(30, 0.2, 0.01);
    glPopMatrix();
  }

  // goal display
  if (VAL(DISPLAY_GOAL)) {
    glPushMatrix();
    setDiffuseColor(COLOR_RED);
    glTranslated(VAL(GOAL_X), VAL(GOAL_Y), VAL(GOAL_Z));
    drawSphere(0.2);
    glPopMatrix();

    //offset from the leg joint
    float off_x = VAL(XPOS);
    float off_y = VAL(YPOS);
    float off_z = VAL(ZPOS);
    // calculate the direction vector
    float direction[3] = {(VAL(GOAL_Z) - VAL(ZPOS))*-1, (VAL(GOAL_Y) - VAL(YPOS)-1),
                          VAL(GOAL_X) - VAL(XPOS)-2};



    float angle3 = 0;
        float angle4 = M_PI / 2; 
        float right_leg_length = 3.5;
    // inverse kinematics
    float angle1 = acos((-direction[1] /
        (sqrt(direction[1] * direction[1] +
            direction[2] * direction[2]))));
    float angle2 = acos((direction[0] /
        (sqrt(direction[2] * direction[2] + direction[1] * direction[1]+
            direction[0] * direction[0]))));;
    float left_leg_length = sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
        direction[0] * direction[0])-1.5;
    // float angle3 = atan2(direction[2], direction[1]);

    if (sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
        direction[0] * direction[0]) - 1.5 > 3.5) {
        left_leg_length = 3.5;

    }
    else if (left_leg_length < 0.5) {
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
            angle3 = acos((-direction[1] /
                (sqrt(direction[1] * direction[1] +
                    direction[2] * direction[2]))));
            angle4 = acos((direction[0] /
                (sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
                    direction[0] * direction[0]))));;
            right_leg_length = sqrt(direction[2] * direction[2] + direction[1] * direction[1] +
                direction[0] * direction[0]) - 1.5;
            if (right_leg_length > 3.5)
                right_leg_length = 3.5;
            if (right_leg_length < 0.5)
                right_leg_length = 0.5;
        }
    }

    

    ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_YAWROTATE,
        (((angle1)* 180.0)) / M_PI);

     ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_ROTATE,
                                                     90-1*( angle2 * 180)/ M_PI);
     ModelerApplication::Instance()->SetControlValue(LEFTSIDELEG_LENGTH,left_leg_length
         );
     ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_YAWROTATE,
         (((angle3) * 180.0)) / M_PI);

     ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_ROTATE,
         90 - 1 * (angle4 * 180) / M_PI);
     ModelerApplication::Instance()->SetControlValue(RIGHTSIDELEG_LENGTH, right_leg_length
     );
  }

  glTranslated(4, 4, 0);
  drawMetaBall();
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
  controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
  controls[HEAD_ROTATE] = ModelerControl("Head Rotate", -135, 135, 1, 0);
  controls[FRONTLEG_ROTATE] = ModelerControl("Front Leg Rotate", -60, 30, 1, 0);
  controls[LEFTSIDELEG_ROTATE] =
      ModelerControl("Left Side Leg Rotate", -180, 180, 1, 0);
  controls[RIGHTSIDELEG_ROTATE] =
      ModelerControl("Right Side Leg Rotate", -180, 180, 1, 0);
  controls[LEFTSIDELEG_LENGTH] =
      ModelerControl("Left Side Leg Length", 1, 3.5, 0.1f, 3.5);
  controls[RIGHTSIDELEG_LENGTH] =
      ModelerControl("Right Side Leg Length", 1, 3.5, 0.1f, 3.5);
  controls[LEFTSIDEFEET_ROTATE] =
      ModelerControl("Left Side Feet Rotate", -180, 180, 1, 0);
  controls[RIGHTSIDEFEET_ROTATE] =
      ModelerControl("Right Side Feet Rotate", -180, 180, 1, 0);
  controls[RIGHTSIDELEG_YAWROTATE] =
      ModelerControl("Right Side Yaw Rotate", -180, 180, 1, 0);
  controls[LEFTSIDELEG_YAWROTATE] =
      ModelerControl("Left Side Yaw Rotate", -180, 180, 1, 0);
  controls[FULL_MOVEMENT] = ModelerControl("Full Movement", -30, 45, 1, 0);

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

  // goal-oriented control
  controls[DISPLAY_GOAL] = ModelerControl("Display Goal", 0, 1, 1, 0);
  controls[GOAL_X] = ModelerControl("Goal X", -8, 8, 0.1f, 4);
  controls[GOAL_Y] = ModelerControl("Goal Y", -8, 8, 0.1f, 0);
  controls[GOAL_Z] = ModelerControl("Goal Z", -8, 8, 0.1f, 0);

  ModelerApplication::Instance()->Init(&createRobotModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}
