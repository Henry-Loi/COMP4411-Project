#include "texturedraw.h"

#include "bitmap.h"

void TextureMap::loadTexture(char *filename) {
  // Load the texture file

  // Seek to the beginning of the file and read the header (assuming BMP format)
  texImage = readBMP(filename, texImageWidth, texImageHeight);

  printf("TextureMap::loadTexture: %s %d %d\n", filename, texImageWidth,
         texImageHeight);

  // Generate a new texture ID
  glGenTextures(1, &texImageId);
}

void drawTextureBox(GLuint texImageId, float height, float width,
                    float length) {
  // Enable texture mapping
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texImageId);

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

  // Disable texture mapping
  glDisable(GL_TEXTURE_2D);
}

void drawTextureCylinder(GLuint texImageId, float radius, float height,
                         int sides) {
  // Enable texture mapping
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texImageId);

  // Draw a textured cylinder
  glBegin(GL_QUAD_STRIP);

  // Calculate the angle between each segment
  float angleStep = 2.0f * 3.14159f / sides;

  for (int i = 0; i <= sides; ++i) {
    float angle = i * angleStep;
    float x = radius * cos(angle);
    float y = radius * sin(angle);

    // Calculate the texture coordinates
    float u = static_cast<float>(i) / sides;
    float v = 0.0f;

    // Draw the vertices and apply texture coordinates
    glTexCoord2f(u, v);
    glVertex3f(x, y, 0.0f);
    glTexCoord2f(u, 1.0f);
    glVertex3f(x, y, height);
  }

  glEnd();

  // Disable texture mapping
  glDisable(GL_TEXTURE_2D);
}
