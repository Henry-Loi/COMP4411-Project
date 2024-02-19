//
// AlphaMappedBrush.h
//
// The header file for Scattered Circles Brush.
//

#ifndef ALPHA_MAPPING_BRUSH_H
#define ALPHA_MAPPING_BRUSH_H

#include "ImpBrush.h"

class AlphaMappedBrush : public ImpBrush {
public:
  AlphaMappedBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target);
  void BrushMove(const Point source, const Point target);
  void BrushEnd(const Point source, const Point target);
  char *BrushName(void);

  int BrushInit();

private:
  unsigned char *m_alphaMap; // imported alpha map
  int mapWidth, mapHeight;

  unsigned char *GetMapPixel(int x, int y);
  unsigned char *GetMapPixel(const Point p);

  void convertToAlphaMap(unsigned char *data);
  void mapRescale(int width, int height);
};

#endif