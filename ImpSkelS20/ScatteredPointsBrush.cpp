//
// ScatteredPointsBrush.cpp
//
// The implementation of ScatteredPoints Brush. It is a kind of ImpBrush. All
// your brush implementations will look like the file with the different GL
// primitive calls.
//

#include "ScatteredPointsbrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>

extern int irand(int max);
extern float frand();

ScatteredPointsBrush::ScatteredPointsBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void ScatteredPointsBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  glPointSize((float)0.1); // hard coded the point size to 0.1

  BrushMove(source, target);
}

void ScatteredPointsBrush::BrushMove(const Point source, Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("ScatteredPointsBrush::BrushMove  document is NULL\n");
    return;
  }

  SetAlpha(source, pDoc->getAlpha());

  int num_of_points = irand(50) + 50;
  // float spacing = 20.0f;

  SetAlpha(source, pDoc->getAlpha());

  for (int i = 0; i < num_of_points; i++) {
    glBegin(GL_POINTS);

    Point ran_target = target.scattered(pDoc->getSize());

    glVertex2d(ran_target.x, ran_target.y);

    glEnd();
  }
}

void ScatteredPointsBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
