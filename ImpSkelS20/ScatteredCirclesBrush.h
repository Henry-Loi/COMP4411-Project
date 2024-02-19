//
// ScatteredCirclesBrush.h
//
// The header file for Scattered Circles Brush.
//

#ifndef SCATTEREDCIRCLESBRUSH_H
#define SCATTEREDCIRCLESBRUSH_H

#include "ImpBrush.h"

class ScatteredCirclesBrush : public ImpBrush {
public:
  ScatteredCirclesBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target);
  void BrushMove(const Point source, const Point target);
  void BrushEnd(const Point source, const Point target);
  char *BrushName(void);

  int BrushInit(void) { return 1; }
};

#endif