#pragma once

#include <FL/gl.h>

typedef enum { TEXTURE_TEST, NUM_OF_TERXTURES } TextureType;

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

static char *texture_list[NUM_OF_TERXTURES] = {
    "texture/test.bmp",
};

void drawTextureBox(GLuint, float height, float width, float length);

void drawTextureCylinder(GLuint, float radius, float height, int sides);
