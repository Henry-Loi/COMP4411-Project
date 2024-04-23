#pragma once

#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"

#include "texturedraw.h"

#include "modelerglobals.h"
#include "vec.h"

#include <vector>

// To make a RobotModel, we inherit off of ModelerView
class RobotModel : public ModelerView {
public:
  RobotModel(int x, int y, int w, int h, char *label)
      : ModelerView(x, y, w, h, label) {
    initTextureMap();
  }

  void set_model_lighting(int);
  void set_mood(int);

  enum MoodType { NONE, NORMAL, HAPPY, SAD, ANGRY, NUM_OF_MOODS };

  void initTextureMap(void);

  TextureMap *textureMaps[NUM_OF_TEXTURES];

  virtual void draw();
};

float frand(void);
