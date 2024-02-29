//
// CurvedBrush.h
//
// The header file for Curved Brush.
//

#ifndef CURVED_BRUSH_H
#define CURVED_BRUSH_H

#include "ImpBrush.h"
#include "PainterlyBrush.h"

class CurvedBrush : public ImpBrush {
  friend class PainterlyBrush;

public:
  CurvedBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  void BrushBegin(const Point source, const Point target);
  void BrushMove(const Point source, const Point target);
  void BrushEnd(const Point source, const Point target);

  void PainterlyBrushBegin(const Point source, const Point target,
                           unsigned char *, PainterlyBrush::Stroke *);
  void PainterlyBrushMove(const Point source, const Point target,
                          unsigned char *, PainterlyBrush::Stroke *);
  char *BrushName(void);

  int BrushInit(void *params) { return 1; };

private:
  void renderStrokes(PainterlyBrush::Stroke *stroke);
  void renderCircles(int x, int y, PainterlyBrush::Stroke &stroke);
  void alphaBlending(unsigned char *source, unsigned char *target);
  PainterlyBrush *painterly;
};

#endif