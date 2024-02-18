//
// ScatteredCirclesBrush.cpp
//
// The implementation of SingleLine Brush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

#include "ScatteredCirclesbrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>

extern int irand(int max);
extern float frand();

void drawFilledCircle(GLfloat x, GLfloat y, GLuint radius);

ScatteredCirclesBrush::ScatteredCirclesBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void ScatteredCirclesBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int size = pDoc->getSize();

  glPointSize((float)size);

  BrushMove(source, target);
}

void ScatteredCirclesBrush::BrushMove(const Point source, Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("ScatteredCirclesBrush::BrushMove  document is NULL\n");
    return;
  }

  SetColorAlpha(source, pDoc->getAlpha());

  int num_of_circles = irand(2) + 3;
  float spacing = 20.0f;

  for (int i = 0; i < num_of_circles; i++) {

    Point ran_target = target.scattered(spacing);

    drawFilledCircle(ran_target.x, ran_target.y, pDoc->getSize() / 2);
  }
}

void ScatteredCirclesBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
