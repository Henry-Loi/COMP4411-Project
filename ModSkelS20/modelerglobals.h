#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values
// of the controls from the user interface.
enum SampleModelControls {
  XPOS,
  YPOS,
  ZPOS,
  HEAD_ROTATE,
  FRONTLEG_ROTATE,
  LEFTSIDELEG_ROTATE,
  RIGHTSIDELEG_ROTATE,
  LEFTSIDELEG_LENGTH,
  RIGHTSIDELEG_LENGTH,
  LEFTSIDEFEET_ROTATE,
  RIGHTSIDEFEET_ROTATE,
  LEFTSIDELEG_YAWROTATE,
  RIGHTSIDELEG_YAWROTATE,
  BODY_ROTATE,
  FULL_MOVEMENT,
  XSCALE,
  YSCALE,
  ZSCALE,
  
  
  
  
  _MAPPING,
  L_SYSTEM_DISPLAY,
  L_SYSTEM_GENERATION,
  METABALLS_THRESHOLD,
  METABALLS_STEP,
  DISPLAY_GOAL,
  GOAL_X,
  GOAL_Y,
  GOAL_Z,
  LIGHT0_X,
  LIGHT0_Y,
  LIGHT0_Z,
  LIGHT0_INTENSITY,
  LIGHT1_X,
  LIGHT1_Y,
  LIGHT1_Z,
  LIGHT1_INTENSITY,
  LIGHT2_X,
  LIGHT2_Y,
  LIGHT2_Z,
  LIGHT2_INTENSITY,
  LEVELOF_DETAILS,
  BODY_PITCH,
  SHOCKWAVE,
  CHARACTER,
  MOOD,
  ANIMATION,
  NUMCONTROLS,
};

// Colors
#define COLOR_RED 1.0f, 0.0f, 0.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f
#define COLOR_YELLOW 1.0f, 1.0f, 0.0f
#define COLOR_GREY 0.5f, 0.5f, 0.5f

// We'll be getting the instance of the application a lot;
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))
#define SETVAL(x, y) (ModelerApplication::Instance()->SetControlValue(x, y))

#endif