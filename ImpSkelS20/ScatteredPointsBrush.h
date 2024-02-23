//
// ScatteredPointsBrush.h
//
// The header file for Scattered Lines Brush.
//

#ifndef SCATTEREDPOINTSBRUSH_H
#define SCATTEREDPOINTSBRUSH_H

#include "ImpBrush.h"

class ScatteredPointsBrush : public ImpBrush {
public:
  ScatteredPointsBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target);
  void BrushMove(const Point source, const Point target);
  void BrushEnd(const Point source, const Point target);
  char *BrushName(void);

  int BrushInit(void *) { return 1; }
};

#endif