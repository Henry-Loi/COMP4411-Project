//
// SingleLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "singlelinebrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>

extern float frand();

SingleLineBrush::SingleLineBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void SingleLineBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  // int size = pDoc->getWidth();

  glLineWidth((float)1);

  BrushMove(source, target);
}

void SingleLineBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("SingleLineBrush::BrushMove  document is NULL\n");
    return;
  }

  glBegin(GL_LINE);
  SetColor(source);

  glVertex2f(target.x, target.y);

  glEnd();
}

void SingleLineBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
