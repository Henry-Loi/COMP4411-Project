//
// PainterlyBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "painterlybrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();

PainterlyBrush::PainterlyBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void PainterlyBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int size = pDoc->getSize();
  glPointSize((float)size);

  BrushMove(source, target);
}

void PainterlyBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  PainterlyParam *arr = pDoc->m_pUI->m_paintView->m_painterlyParam;
  PainterlyStroke type =
      static_cast<PainterlyStroke>(pDoc->m_pUI->getPainterlyStroke());
  PainterlyParam *param = &arr[type];

  if (pDoc == NULL) {
    printf("PainterlyBrush::BrushMove  document is NULL\n");
    return;
  }

  // add color jitter
  float Jr = param->Jr;
  float Jg = param->Jg;
  float Jb = param->Jb;
  float Jh = param->Jh;
  float Js = param->Js;
  float Jv = param->Jv;

  float r = frand() * Jr;
  float g = frand() * Jg;
  float b = frand() * Jb;
  float h = frand() * Jh;
  float s = frand() * Js;
  float v = frand() * Jv;

  // apply color jitter to source color and paint
  unsigned char color[3];
  getSourceRGB(source, color);
  float hsv[3];
  RGBtoHSV(color, hsv);
  hsv[0] += h;
  hsv[1] += s;
  hsv[2] += v;
  hsv[0] = fmod(hsv[0], 360);
  hsv[1] = min(1.0f, max(0.0f, hsv[1]));
  hsv[2] = min(1.0f, max(0.0f, hsv[2]));

  float rgb[3];
  HSVtoRGB(hsv, rgb);
  color[0] = (unsigned char)min(255.0f, max(0.0f, color[0] + r));
  color[1] = (unsigned char)min(255.0f, max(0.0f, color[1] + g));
  color[2] = (unsigned char)min(255.0f, max(0.0f, color[2] + b));

  if (pDoc->m_pUI->getPainterlyStyle() == PAINTERLY_EXPRESSIONIST ||
      pDoc->m_pUI->getPainterlyStyle() == PAINTERLY_POINTILLIST) {
    // TODO: use stork length parameter
  }

  if (type == PAINTERLY_BSPLINE_BRUSH) {
    // TODO: use curve parameter
  }

  glBegin(GL_POINTS);
  glColor4f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0, param->Alpha);

  glVertex2d(target.x, target.y);

  glEnd();
}

void PainterlyBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
