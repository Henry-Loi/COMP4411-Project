#pragma once

#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include <FL/gl.h>

#include "modelerglobals.h"

// To make a RobotModel, we inherit off of ModelerView
class RobotModel : public ModelerView {
public:
  RobotModel(int x, int y, int w, int h, char *label)
      : ModelerView(x, y, w, h, label) {}

  void set_model_lighting(void);

  virtual void draw();
};
