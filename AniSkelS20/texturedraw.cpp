#include "texturedraw.h"

#include "bitmap.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerglobals.h"

#include <FL/fl_ask.H>

void TextureMap::loadTexture(char *filename) {
  // Load the texture file

  // Seek to the beginning of the file and read the header (assuming BMP format)
  texImage = readBMP(filename, texImageWidth, texImageHeight);

  if (texImage == NULL) {
    // pop alert
    fl_alert("Error reading texture file");
    return;
  }
  // Generate a new texture ID
  glGenTextures(3, &texImageId);
}

void drawTextureBox(TextureMap *map, float height, float width, float length) {

  if (!VAL(TEXTURE_MAPPING)) {
    drawBox(height, width, length);
    return;
  }

  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&map->texImageId);
  glBindTexture(GL_TEXTURE_2D, map->texImageId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map->texImageWidth,
               map->texImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, map->texImage);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
            GL_MODULATE); // default is GL_MODULATE for interacting with light
  glBindTexture(GL_TEXTURE_2D, map->texImageId); // repeat?

  glPushMatrix();

  glTranslated(width / 2, height / 2, length / 2);
  // Draw a textured box
  glBegin(GL_QUADS);

  // Front face
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-width / 2, -height / 2, length / 2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width / 2, -height / 2, length / 2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width / 2, height / 2, length / 2);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-width / 2, height / 2, length / 2);

  // Back face
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-width / 2, -height / 2, -length / 2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width / 2, -height / 2, -length / 2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width / 2, height / 2, -length / 2);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-width / 2, height / 2, -length / 2);

  // Top face
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-width / 2, height / 2, length / 2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width / 2, height / 2, length / 2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width / 2, height / 2, -length / 2);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-width / 2, height / 2, -length / 2);

  // Bottom face
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-width / 2, -height / 2, length / 2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width / 2, -height / 2, length / 2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width / 2, -height / 2, -length / 2);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-width / 2, -height / 2, -length / 2);

  // Right face
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(width / 2, -height / 2, length / 2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(width / 2, -height / 2, -length / 2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(width / 2, height / 2, -length / 2);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(width / 2, height / 2, length / 2);

  // Left face
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-width / 2, -height / 2, length / 2);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-width / 2, -height / 2, -length / 2);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-width / 2, height / 2, -length / 2);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-width / 2, height / 2, length / 2);

  glEnd();

  glPopMatrix();

  // Disable texture mapping
  glDisable(GL_TEXTURE_2D);
}

void drawTextureCylinder(TextureMap *map, float height, float radius1,
                         float radius2) {
  if (!VAL(TEXTURE_MAPPING)) {
    drawCylinder(height, radius1, radius2);
    return;
  }

  glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&map->texImageId);
  glBindTexture(GL_TEXTURE_2D, map->texImageId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map->texImageWidth,
               map->texImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, map->texImage);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
            GL_MODULATE); // default is GL_MODULATE for interacting with light
  glBindTexture(GL_TEXTURE_2D, map->texImageId); // repeat?

  drawCylinder(height, radius1, radius2);

  glDisable(GL_TEXTURE_2D);
}
