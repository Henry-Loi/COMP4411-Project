//
// AlphaMappedBrush.cpp
//
// The implementation of SingleLine Brush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

#include "AlphaMappedBrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>

extern int irand(int max);
extern float frand();

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {
  m_alphaMap = NULL;
}

int AlphaMappedBrush::BrushInit(void) {
  ImpressionistDoc *pDoc = GetDocument();

  // load alpha map
  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());

  unsigned char *data;
  int width, height;

  if ((data = readBMP(newfile, width, height)) == NULL) {
    fl_alert("Can't load bitmap file");
    return 0;
  }

  // reflect the fact of loading the new image
  mapWidth = width;
  mapHeight = height;

  // release old storage
  if (m_alphaMap)
    delete[] m_alphaMap;

  convertToAlphaMap(data);
}

void AlphaMappedBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  // int size = pDoc->getSize();

  // mapRescale(size, size);

  BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const Point source, Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("AlphaMappedBrush::BrushMove  document is NULL\n");
    return;
  }

  // print alpha map with color in source at target
  glPointSize(1);
  glBegin(GL_POINTS);
  for (int i = 0; i < mapWidth; i++) {
    for (int j = 0; j < mapHeight; j++) {
      SetColorAlpha(source, int(m_alphaMap[i * mapWidth + j]) / 255.0);
      glVertex2d(target.x + i, target.y + j);
    }
  }
}

void AlphaMappedBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}

// helper private function
void AlphaMappedBrush::convertToAlphaMap(unsigned char *data) {
  m_alphaMap = new unsigned char[mapWidth * mapHeight];
  // convert to alpha map (grayscale)
  for (int i = 0; i < mapWidth * mapHeight; i++) {
    m_alphaMap[i] = (data[i * 3] + data[i * 3 + 1] + data[i * 3 + 2]) / 3;
  }
}

void AlphaMappedBrush::mapRescale(int width, int height) {
  // rescale the alpha map
  unsigned char *rescaledMap = new unsigned char[width * height];
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      rescaledMap[i * width + j] =
          m_alphaMap[i * mapWidth / width * mapHeight / height +
                     j * mapHeight / height];
    }
  }
  delete[] m_alphaMap;
  m_alphaMap = rescaledMap;
}
