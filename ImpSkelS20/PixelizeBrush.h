//
// Pixelize Brush.h
//
// The header file for Pixelize Brush.
//

#ifndef PIXELIZE_BRUSH_H
#define PIXELIZE_BRUSH_H

#include "ImpBrush.h"

class PixelizeBrush : public ImpBrush {
public:
  PixelizeBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target);
  void BrushMove(const Point source, const Point target);
  void BrushEnd(const Point source, const Point target);
  char *BrushName(void);

  int BrushInit(void *) { return 1; }
};

#endif