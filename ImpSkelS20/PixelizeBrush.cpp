//
// Pixelize Brush.cpp
//
// The implementation of Pixelize Brush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

// Idea inspired from: https://github.com/dipsywong98/COMP4411-impressionist

#include "PixelizeBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();

PixelizeBrush::PixelizeBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void PixelizeBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int size = pDoc->getSize();

  BrushMove(source, target);
}

void PixelizeBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *pUI = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("PixelizeBrush::BrushMove  document is NULL\n");
    return;
  }

  if (source.x > pDoc->m_nPaintWidth || source.y < 0) {
    return;
  }

  int size = pUI->getSize();
  int lineWith = pUI->getLineWidth();
  int w = pDoc->m_nPaintWidth, h = pDoc->m_nPaintHeight;

  Point new_source = Point(0, 0);
  Point new_target = Point(0, 0);

  glPointSize(1.0);
  glBegin(GL_POINTS);

  // print part of the original image
  for (int i = -size / 2; i < size / 2; i++) {
    for (int j = -size / 2; j < size / 2; j++) {
      new_source.x = source.x + i - (source.x + i) % lineWith;
      new_source.y = source.y + j - (source.y + j) % lineWith;
      new_target.x = target.x + i;
      new_target.y = target.y + j;

      SetColor(new_source);
      glVertex2d(new_target.x, new_target.y);
    }
  }
  glEnd();
}

void PixelizeBrush::BrushEnd(const Point source, const Point target) {}
