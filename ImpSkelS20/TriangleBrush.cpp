//
// TriangleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "trianglebrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

extern float frand();

TriangleBrush::TriangleBrush(ImpressionistDoc* pDoc, char* name)
    : ImpBrush(pDoc, name) {}

void TriangleBrush::BrushBegin(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    int size = pDoc->getSize();

    glPointSize((float)size);

    BrushMove(source, target);
}

void TriangleBrush::BrushMove(const Point source, const Point target) {
    ImpressionistDoc* pDoc = GetDocument();
    ImpressionistUI* dlg = pDoc->m_pUI;

    if (pDoc == NULL) {
        printf("PointBrush::BrushMove  document is NULL\n");
        return;
    }

    SetColorAlpha(source, pDoc->getAlpha());
    glBegin(GL_TRIANGLES);
    float size = pDoc->getSize();
    glVertex2d(target.x+size/sqrt(3), target.y-size/3);
    glVertex2d(target.x - size / sqrt(3), target.y - size / 3);
    glVertex2d(target.x , target.y +2* size / 3);
    glEnd();
}

void TriangleBrush::BrushEnd(const Point source, const Point target) {
    // do nothing so far
}
