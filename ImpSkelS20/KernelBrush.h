//
// KernelBrush.h
//
// The header file for Point Brush.
//

#ifndef KERNAL_BRUSH_H
#define KERNAL_BRUSH_H

#include "ImpBrush.h"

class KernelBrush : public ImpBrush {
public:
  KernelBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target);
  void BrushMove(const Point source, const Point target);
  void BrushEnd(const Point source, const Point target);
  char *BrushName(void);

  int BrushInit(void) { return 1; }
};

#endif