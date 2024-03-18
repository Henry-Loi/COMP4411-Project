#pragma once

#include <FL/gl.h>

typedef enum { TEXTURE_DOT, TEXTURE_BRICK, NUM_OF_TEXTURES } TextureType;

class TextureMap {
public:
  TextureMap(char *path) { loadTexture(path); }
  ~TextureMap() {}

  void loadTexture(char *);

  int texImageWidth;
  int texImageHeight;

  GLubyte *texImage;
  GLuint texImageId;
};

static char *texture_list[NUM_OF_TEXTURES] = {"dot.bmp", "brick.bmp"};

void drawTextureBox(TextureMap *, float height, float width, float length);

void drawTextureCylinder(TextureMap *, float, float, float);
