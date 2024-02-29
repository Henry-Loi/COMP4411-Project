//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "ImpBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <iostream>
#include <cmath>
#include "fastmath.h"

// Static class member initializations
int ImpBrush::c_nBrushCount = 0;
ImpBrush **ImpBrush::c_pBrushes = NULL;

ImpBrush::ImpBrush(ImpressionistDoc *pDoc, char *name)
    : m_pDoc(pDoc), m_pBrushName(name) {}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc *ImpBrush::GetDocument(void) { return m_pDoc; }

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char *ImpBrush::BrushName(void) { return m_pBrushName; }

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample
// the color from
//----------------------------------------------------
void ImpBrush::SetColor(const Point source) {
  ImpressionistDoc *pDoc = GetDocument();

  GLubyte color[3];

  memcpy(color, pDoc->GetOriginalPixel(source), 3);

  glColor3ubv(color);
}

void ImpBrush::SetColorAlpha(const Point source, float alpha) {
  ImpressionistDoc *pDoc = GetDocument();

  GLubyte color[3];


  memcpy(color, pDoc->GetOriginalPixel(source), 3);
  std::cout << "checkAlpha" << std::endl;
  std::cout << (float)color[0] <<" "<< (float)color[1]<<" " << (float)color[2] << std::endl;

  //For Manual_Color_Selection 
  color[0] = ((float)color[0] + (float)(pDoc->m_pUI->Color_Selection->r())*255.0-255.0 >0)? ((float)color[0] + (float)(pDoc->m_pUI->Color_Selection->r()) * 255.0) - 255.0:0;
  color[1] = ((float)color[1] + (float)(pDoc->m_pUI->Color_Selection->g())*255.0 - 255.0 > 0) ? ((float)color[1] + (float)(pDoc->m_pUI->Color_Selection->g()) * 255.0) - 255.0 : 0;
  color[2] = ((float)color[2] + (float)(pDoc->m_pUI->Color_Selection->b())*255.0 - 255.0 > 0) ? ((float)color[2] + (float)(pDoc->m_pUI->Color_Selection->b()) * 255.0) - 255.0 : 0;
  //
  
  std::cout << "checkAlpha2" << std::endl;
  std::cout << (float)color[0] << " " << (float)color[1] << " " << (float)color[2] << std::endl;
  std::cout << "checkRGB" << std::endl;
  std::cout << (float)pDoc->m_pUI->get_m_R() << " " << (float)pDoc->m_pUI->get_m_G() << " " << (float)pDoc->m_pUI->get_m_B() << std::endl;

  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0, alpha);
}

void ImpBrush::getSourceRGB(const Point source, GLubyte color[3]) {
  ImpressionistDoc *pDoc = GetDocument();

  memcpy(color, pDoc->GetOriginalPixel(source), 3);
}

// input a int matrix as kernel
float ImpBrush::kernelOperation(const Point Source, int arr[3][3], int kernel_size, const int border[2]) {
    float sum = 0;
    float intensity = 0;
    GLubyte color_origin[3];
    int column = 0;
    int row = 0;

    for (int i = Source.x - (kernel_size - 1) / 2; i <= Source.x + (kernel_size - 1) / 2; i++) {//column
        row = 0;
        for (int j = Source.y - (kernel_size - 1) / 2; j <= Source.y + (kernel_size - 1) / 2; j++) {//row
            //if (i > border[0]) {
            //    if (j > border[1])
            //        memcpy(color_origin, (m_pDoc->*(m_pDoc->GetPixel))(Point(abs(2 * border[0] - i), abs(2 * border[1] - j))), 3);
            //    else
            //        memcpy(color_origin, (m_pDoc->*(m_pDoc->GetPixel))(Point(abs(2 * border[0] - i), abs(j))), 3);
            //}
            //else if(j>border[1])
            //    memcpy(color_origin, (m_pDoc->*(m_pDoc->GetPixel))(Point(abs(i), abs(2 * border[1] - j))), 3);
            //else
                memcpy(color_origin, (m_pDoc->*(m_pDoc->GetPixel))(Point(abs(i),abs(j))), 3);
            intensity = 0.299 * (float)color_origin[0] + 0.587 * (float)color_origin[1] + 0.114 * (float)color_origin[2];
            std::cout << "row:" << row << "Column:" << column <<"arr"<<arr[row][column] << std::endl;
            sum += intensity * (float) arr[row][column];
            row++;
        }
        column++;
    }
    return sum;
}


// return gradient direction and magnitude, [1] ->direction,[2]->magnitude
float* ImpBrush::getGradient(const Point Source,const int border[2]) {

    int o_sobelX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
    int o_sobelY[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };



    //int* sobelX[3];
    //int* sobelY[3];
    //for (int i = 0; i < 3; i++) {
    //    sobelX[i] = o_sobelX[i];
    //}
    //for (int i = 0; i < 3; i++) {
    //    sobelY[i] = o_sobelY[i];
    //}

    int kernel_size = 3;
    float gradientX = kernelOperation(Source, o_sobelX, kernel_size, border);
    float gradientY = kernelOperation(Source, o_sobelY, kernel_size, border);

    float gradient[2];
    if (gradientX != 0)
        gradient[0] = atan(-gradientY / gradientX);
    else if (gradientY >= 0)
        gradient[0] = DEG2RAD(90);
    else
        gradient[0] = DEG2RAD(-90);
    gradient[1] = sqrt(pow(gradientX, 2) + pow(gradientY, 2));

    return gradient;
}