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
    float denominator = 9.0f;
    float* rawColor = kernelOperation(source, blurMatrix, 3);
    std::cout << "rawColor: " << rawColor[0] << " " << rawColor[1] << " " << rawColor[2] << std::endl;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f((GLfloat)(rawColor[0] / (255.0*denominator)), (GLfloat)(rawColor[1] / (255.0 * denominator)), (GLfloat)(rawColor[2] / (255.0 * denominator)), pDoc->getAlpha());
    std::cout << "rawColor: " << (GLfloat)(rawColor[0] / (255.0 * denominator)) << " " << (GLfloat)(rawColor[1] / (255.0 * denominator)) << " " << (GLfloat)(rawColor[2] / (255.0 * denominator)) << std::endl;
    //SetColorAlpha(source, pDoc->getAlpha());
    glBegin(GL_POINTS);

    glVertex2d(target.x, target.y);

    glEnd();
}


void BluringBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}