//
// PainterlyBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "painterlybrush.h"
#include "BSplines.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

#include <cmath>
#include <vector>

extern float frand();

PainterlyBrush::PainterlyBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {
  m_pDoc = pDoc;
  m_painterlyStyle = PAINTERLY_IMPRESSIONIST;
  m_painterlyStroke = PAINTERLY_CURVE_BRUSH;

  param[PAINTERLY_IMPRESSIONIST] = {PainterlyParam(100, 1.0f, 0.5f, 1.0f, 4, 16,
                                                   1.0f, 3, 3, 0.0f, 0.0f, 0.0f,
                                                   0.0f, 0.0f, 0.0f)};
  param[PAINTERLY_EXPRESSIONIST] = {PainterlyParam(550, 0.25f, 0.5f, 1.0f, 10,
                                                   16, 0.7f, 3, 3, 0.0f, 0.0f,
                                                   0.0f, 0.0f, 0.0f, 0.5f)};
  param[PAINTERLY_COLOR_WASH] = {PainterlyParam(200, 1.0f, 0.5f, 1.0f, 4, 16,
                                                0.5f, 3, 3, 0.3f, 0.3f, 0.3f,
                                                0.0f, 0.0f, 0.0f)};
  param[PAINTERLY_POINTILLIST] = {PainterlyParam(100, 1.0f, 0.5f, 1.0f, 0, 0,
                                                 1.0f, 2, 2, 0.0f, 0.0f, 0.0f,
                                                 0.3f, 0.0f, 1.0f)};
  param[PAINTERLY_CUSTOMIZED] = {PainterlyParam(100, 1.0f, 0.5f, 1.0f, 4, 16,
                                                1.0f, 3, 3, 0.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 0.0f)};
};

void PainterlyBrush::StartPaint(std::vector<Stroke *> strokes, float *zBuffer) {
  PainterlyParam *p = &param[m_painterlyStyle];

  for (auto *stroke : strokes) {
    BSplines bSplines(stroke->controlPoints, 3, stroke->size * 4);
    auto &samples = bSplines.samples;

    if (p->Jh > 0.0 || p->Js > 0.0 || p->Jv > 0.0) {
      float hsv[3];
      RGBtoHSV(stroke->color, hsv);
      hsv[0] = (1 - p->Jh * JitterIndex) * hsv[0] +
               p->Jh * JitterIndex * frand() * 360;
      hsv[1] = (1 - p->Js * JitterIndex) * hsv[1] +
               p->Js * JitterIndex * frand() * 1;
      hsv[2] = (1 - p->Jv * JitterIndex) * hsv[2] +
               p->Jv * JitterIndex * frand() * 1;
      HSVtoRGB(hsv, stroke->color);
    }

    stroke->color[0] = (1 - p->Jr * JitterIndex) * stroke->color[0] +
                       p->Jr * JitterIndex * frand() * 255;
    stroke->color[1] = (1 - p->Jg * JitterIndex) * stroke->color[1] +
                       p->Jg * JitterIndex * frand() * 255;
    stroke->color[2] = (1 - p->Jb * JitterIndex) * stroke->color[2] +
                       p->Jb * JitterIndex * frand() * 255;

    for (auto &sample : samples) {
      renderCircles(sample.x, sample.y, *stroke, zBuffer);
    }
  }
}

void PainterlyBrush::renderCircles(int x, int y, Stroke &stroke,
                                   float *zBuffer) {
  ImpressionistDoc *m_pDoc = GetDocument();

  int height = m_pDoc->m_nPaintHeight, width = m_pDoc->m_nPaintWidth,
      radius = stroke.size;
  unsigned char *canvas = m_pDoc->m_ucPainting;

  PainterlyParam *param = &param[m_painterlyStyle];

  for (int i = y - radius; i < y + radius; ++i)
    for (int j = x - radius; j < x + radius; ++j) {
      if (std::sqrt((i - y) * (i - y) + (j - x) * (j - x)) > radius)
        continue;
      if (i < 0 || j < 0 || i >= height || j >= width)
        continue;

      int index = i * width + j;
      if (zBuffer[index] < stroke.depth)
        continue;
      zBuffer[index] = stroke.depth;

      (canvas + index * 3)[0] = stroke.color[0] * param->Alpha;
      (canvas + index * 3)[1] = stroke.color[1] * param->Alpha;
      (canvas + index * 3)[2] = stroke.color[2] * param->Alpha;
    }
}
