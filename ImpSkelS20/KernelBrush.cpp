//
// KernelBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "Kernelbrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();

KernelBrush::KernelBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void KernelBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int size = pDoc->getSize();

  glPointSize((float)size);

  BrushMove(source, target);
}

void KernelBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("KernelBrush::BrushMove  document is NULL\n");
    return;
  }

  SetColorAlpha(source, pDoc->getAlpha());
  glBegin(GL_POINTS);

  glVertex2d(target.x, target.y);

  glEnd();
}

void KernelBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
