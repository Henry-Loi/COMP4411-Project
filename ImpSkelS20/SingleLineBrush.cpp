//
// SingleLineBrush.cpp
//
// The implementation of SingleLine Brush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

#include "singlelinebrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>
#include <iostream>

SingleLineBrush::SingleLineBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

void SingleLineBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  int width = pDoc->getLineWidth();

  glLineWidth((float)width);

  BrushMove(source, target);
}
void SingleLineBrush::RightClickBrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    int width = pDoc->getLineWidth();

    glLineWidth((float)width);
}


void SingleLineBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("SingleLineBrush::BrushMove  document is NULL\n");
    return;
  }
  glBegin(GL_LINES);
  SetColorAlpha(source, pDoc->getAlpha());


  float size = pDoc->getSize();
  float angle = pDoc->getLineAngle();
  std::cout << pDoc->m_pUI->isEdgeClipping << std::endl;

  if ((pDoc->m_pUI->isEdgeClipping)) {
      pDoc->setGetPixel(ORIGINAL_IMAGE);

      EdgeClipMove(source, target, size, angle);
  }
  else{
      //no edge clip

      glVertex2f(target.x - (cos(DEG2RAD(angle)) * size / 2),
          target.y - (sin(DEG2RAD(angle)) * size / 2));
      glVertex2f(target.x + (cos(DEG2RAD(angle)) * size / 2),
          target.y + (sin(DEG2RAD(angle)) * size / 2));
  }

  glEnd();
}

void SingleLineBrush::PointerMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("SingleLineBrush::BrushMove  document is NULL\n");
        return;
    }

    glColor3f(1, 0, 0);
    glBegin(GL_LINES);

    float size = pDoc->getSize();
    float angle = pDoc->getLineAngle();
    glVertex2f(target.x - (cos(DEG2RAD(angle)) * size / 2),
        target.y - (sin(DEG2RAD(angle)) * size / 2));
    glVertex2f(target.x + (cos(DEG2RAD(angle)) * size / 2),
        target.y + (sin(DEG2RAD(angle)) * size / 2));


    glEnd();
}


void SingleLineBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
