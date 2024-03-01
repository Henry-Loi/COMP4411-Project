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

//void SingleLineBrush::EdgeClipMove(const Point source, const Point target,const int size,const int angle) {
//    ImpressionistDoc* pDoc = GetDocument();
//    float start_x = target.x - (cos(DEG2RAD(angle)) * size / 2);
//    float start_y = target.y - (sin(DEG2RAD(angle)) * size / 2);
//    float end_x = target.x + (cos(DEG2RAD(angle)) * size / 2);
//    float end_y = target.y + (sin(DEG2RAD(angle)) * size / 2);
//
//    int iterate = 0;
//    bool end_found = FALSE;
//    bool start_found = FALSE;
//
//    while (iterate < size / 2) {
//        if (!end_found) {
//            GLubyte* color_e = pDoc->GetEdgeImagePixel(source.x + sin(DEG2RAD(angle)) * iterate, source.y + cos(DEG2RAD(angle)) * iterate);
//            std::cout << "end:" << 0.299 * (float)color_e[0] + 0.587 * (float)color_e[1] + 0.114 * (float)color_e[2] << std::endl;
//            if ((0.299 * (float)color_e[0] + 0.587 * (float)color_e[1] + 0.114 * (float)color_e[2])/255 == 1) {
//                end_x = source.x + cos(DEG2RAD(angle)) * iterate;
//                end_y = source.y + sin(DEG2RAD(angle)) * iterate;
//                end_found = TRUE;
//            }
//        }
//        if (!start_found) {
//            GLubyte* color_s = pDoc->GetEdgeImagePixel(source.x - sin(DEG2RAD(angle)) * iterate, source.y - cos(DEG2RAD(angle)) * iterate);
//            std::cout << "start:" << 0.299 * (float)color_s[0] + 0.587 * (float)color_s[1] + 0.114 * (float)color_s[2] << std::endl;
//            if ((0.299 * (float)color_s[0] + 0.587 * (float)color_s[1] + 0.114 * (float)color_s[2])/255 == 1) {
//                start_x = source.x - cos(DEG2RAD(angle)) * iterate;
//                start_y = source.y - sin(DEG2RAD(angle)) * iterate;
//                start_found = TRUE;
//            }
//        }
//        if (start_found && end_found)
//            break;
//        iterate++;
//    }
//    glVertex2f(start_x,start_y);
//    glVertex2f(end_x,end_y);    
//}

void SingleLineBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
