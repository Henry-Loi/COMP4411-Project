#include "CurvedBrush.h"
#include "BSplines.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"

#include <cmath>
#include <complex>
#include <math.h>
#include <iostream>

CurvedBrush::CurvedBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {
  // painterly = new PainterlyBrush(pDoc, name);
}

void CurvedBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();

  BrushMove(source, target);
}

void CurvedBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("Curved Brush::BrushMove  document is NULL\n");
    return;
  }

  int size = pDoc->getSize(), height = pDoc->m_nPaintHeight,
      width = pDoc->m_nPaintWidth;

  if (size > 5)
    size = 5 + (size - 5) / 10;

  auto *stroke = pDoc->m_pUI->m_paintView->makeSplineStroke(
      pDoc->m_ucOriginal, source.x, source.y, size);
  renderStrokes(stroke);
}

void CurvedBrush::PainterlyBrushBegin(
    const Point source, const Point target, unsigned char *ref,
    PainterlyBrush::Stroke *stroke = nullptr) {
  ImpressionistDoc *pDoc = GetDocument();

  PainterlyBrushMove(source, target, ref, stroke);
}

void CurvedBrush::PainterlyBrushMove(const Point source, const Point target,
                                     unsigned char *ref = nullptr,
                                     PainterlyBrush::Stroke *stroke = nullptr) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("Curved Brush::BrushMove  document is NULL\n");
    return;
  }

  if (ref == nullptr)
    ref = pDoc->m_ucBitmap;

  int size = pDoc->getSize(), height = pDoc->m_nPaintHeight,
      width = pDoc->m_nPaintWidth;

  if (size > 5)
    size = 5 + (size - 5) / 10;

  if (stroke == nullptr) {
      std::cout << " makesplinePaintly" << std::endl;
    stroke = pDoc->m_pUI->m_paintView->makeSplineStroke(ref, source.x, source.y,
                                                        size);
  }
  renderStrokes(stroke);
}

void CurvedBrush::BrushEnd(const Point source, const Point target) {}

void CurvedBrush::renderStrokes(PainterlyBrush::Stroke *stroke) {
  int sampleRate = 4;
  BSplines bSplines(stroke->controlPoints, 3, stroke->size * sampleRate);
  auto &samples = bSplines.samples;

  for (auto &sample : samples) {
    renderCircles(sample.x, sample.y, *stroke);
  }
}

void CurvedBrush::renderCircles(int x, int y, PainterlyBrush::Stroke &stroke) {
  auto *m_Doc = GetDocument();
  int height = m_Doc->m_nPaintHeight, width = m_Doc->m_nPaintWidth,
      radius = m_Doc->getSize();
  unsigned char *orig = m_Doc->m_ucPainting;

  if (radius > 3)
    radius = 3;

  glPointSize(1.f);
  glBegin(GL_POINTS);

  for (int i = y - radius; i < y + radius; ++i)
    for (int j = x - radius; j < x + radius; ++j) {
      if (std::sqrt((i - y) * (i - y) + (j - x) * (j - x)) >
          radius) // Use std::hypot instead of std::norm
        continue;
      if (i < 0 || j < 0 || i >= height || j >= width)
        continue;

      int index = i * width + j;

      unsigned char color[3];
      memcpy(color, orig + index * 3, 3);
      alphaBlending(stroke.color, color);
      glColor3ubv(color);
      glVertex2d(j, i);
    }

  glEnd();
  glPointSize(m_Doc->getSize());
}

void CurvedBrush::alphaBlending(unsigned char *source, unsigned char *target) {
  auto alpha = GetDocument()->getAlpha();
  target[0] = source[0] * alpha + target[0] * (1 - alpha);
  target[1] = source[1] * alpha + target[1] * (1 - alpha);
  target[2] = source[2] * alpha + target[2] * (1 - alpha);
}
