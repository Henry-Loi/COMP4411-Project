//
// SharpeningBrush.cpp
//
// The implementation of SharpeningBrush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

#include "sharpeningbrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>
#include <iostream>

SharpeningBrush::SharpeningBrush(ImpressionistDoc* pDoc, char* name)
    : ImpBrush(pDoc, name) {}

void SharpeningBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;
    int size = pDoc->getSize();
    //if (size)
    //    glPointSize((float)size / 3);
    //else
    //    glPointSize((float)1);
    glPointSize((float)size);
    BrushMove(source, target);
}



void SharpeningBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("PointBrush::BrushMove  document is NULL\n");
        return;
    }


    int sharpMatrix[3][3] = { {-1,-1,-1},
                            {-1,9,-1},
                            {-1,-1,-1} };
    float denominator =  255.0;

    float size = pDoc->getSize();
    //for (int i = 0; i < 9; i++) {
        glBegin(GL_POINTS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //    Point ran_target(target.x + (float)(i / 3 - 1.0) * size/3, target.y + (float)(i % 3 - 1) * size/3);
    //    Point ran_source(source.x + (float)(i / 3 - 1.0) * size/3, source.y + (float)(i % 3 - 1) * size/3);
        float* rawColor = kernelOperation(source, sharpMatrix, 3);
        glColor4f((GLfloat)(rawColor[0] / (denominator)),
            (GLfloat)(rawColor[1] / (denominator)), (GLfloat)(rawColor[2] / denominator), pDoc->getAlpha());
        delete[]rawColor;
        glVertex2d(target.x, target.y);
        glEnd();

    //}



}


void SharpeningBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}