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
  HEIGHT,
  HEAD_ROTATE,
  FRONTLEG_ROTATE,
  LEFTSIDELEG_ROTATE,
  RIGHTSIDELEG_ROTATE,
  LEFTSIDEFEET_ROTATE,
  RIGHTSIDEFEET_ROTATE,
  BODY_ROTATE,
  XSCALE,
  YSCALE,
  ZSCALE,
  LIGHT_X,
  LIGHT_Y,
  LIGHT_Z,
  LIGHT_INTENSITY,
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

#endif