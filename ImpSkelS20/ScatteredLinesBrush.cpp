//
// ScatteredLinesBrush.cpp
//
// The implementation of SingleLine Brush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

#include "Scatteredlinesbrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>

extern int irand(int max);
extern float frand();

ScatteredLinesBrush::ScatteredLinesBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void ScatteredLinesBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int width = pDoc->getLineWidth();

  glLineWidth((float)width);

  BrushMove(source, target);
}

void ScatteredLinesBrush::BrushMove(const Point source, Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("ScatteredLinesBrush::BrushMove  document is NULL\n");
    return;
  }

  SetColorAlpha(source, pDoc->getAlpha());

  int num_of_lines = irand(2) + 3;
  float size = pDoc->getSize();
  float angle = pDoc->getLineAngle();
  float spacing = 20.0f;

  for (int i = 0; i < num_of_lines; i++) {
    glBegin(GL_LINES);
    Point ran_target = target.scattered(spacing);
    Point ran_source = Point(ran_target.x+ pDoc->m_pUI->m_paintView->getStartCol(),
        ran_target.y - pDoc->m_pUI->m_paintView->getWindowHeight() + pDoc->m_pUI->m_paintView->getEndRow());
    SetColorAlpha(ran_source, pDoc->getAlpha());
    if ((pDoc->m_pUI->isEdgeClipping)) {
        pDoc->setGetPixel(ORIGINAL_IMAGE);
        EdgeClipMove(ran_source, ran_target, size, angle);
    }
    else {
        glVertex2f(ran_target.x - (cos(DEG2RAD(angle)) * size / 2),
            ran_target.y - (sin(DEG2RAD(angle)) * size / 2));
        glVertex2f(ran_target.x + (cos(DEG2RAD(angle)) * size / 2),
            ran_target.y + (sin(DEG2RAD(angle)) * size / 2));
    }

    glEnd();
  }
}

void ScatteredLinesBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
