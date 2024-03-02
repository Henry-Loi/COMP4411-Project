//
// BluringBrush.cpp
//
// The implementation of BluringLine Brush. It is a kind of ImpBrush. All your
// brush implementations will look like the file with the different GL primitive
// calls.
//

#include "bluringbrush.h"
#include "fastmath.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <gl/gl.h>
#include <iostream>

BluringBrush::BluringBrush(ImpressionistDoc* pDoc, char* name)
    : ImpBrush(pDoc, name) {}

void BluringBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    int size = pDoc->getSize();

        glPointSize((float)size);
    BrushMove(source, target);
}



void BluringBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("PointBrush::BrushMove  document is NULL\n");
        return;
    }


    int blurMatrix[3][3] = { {1,1,1},
                            {1,1,1},
                            {1,1,1} };
    float denominator = 9.0*255.0;

    for (int i = 0; i < 9; i++) {
        glBegin(GL_POINTS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        float* rawColor = kernelOperation(source, blurMatrix, 3);
        glColor4f((GLfloat)(rawColor[0] / (denominator)),
            (GLfloat)(rawColor[1] / (denominator)), (GLfloat)(rawColor[2] / denominator), pDoc->getAlpha());
        delete[]rawColor;
        glVertex2d(target.x, target.y);
        glEnd();

    }


}


void BluringBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}