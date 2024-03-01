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
#include <iostream>


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
  //std::cout << "checkAlpha" << std::endl;
  //std::cout << (float)color[0] <<" "<< (float)color[1]<<" " << (float)color[2] << std::endl;

  //For Manual_Color_Selection 
  color[0] = ((float)color[0] + (float)(pDoc->m_pUI->Color_Selection->r())*255.0-255.0 >0)? ((float)color[0] + (float)(pDoc->m_pUI->Color_Selection->r()) * 255.0) - 255.0:0;
  color[1] = ((float)color[1] + (float)(pDoc->m_pUI->Color_Selection->g())*255.0 - 255.0 > 0) ? ((float)color[1] + (float)(pDoc->m_pUI->Color_Selection->g()) * 255.0) - 255.0 : 0;
  color[2] = ((float)color[2] + (float)(pDoc->m_pUI->Color_Selection->b())*255.0 - 255.0 > 0) ? ((float)color[2] + (float)(pDoc->m_pUI->Color_Selection->b()) * 255.0) - 255.0 : 0;
  //
  /*std::cout << "checkAlpha2" << std::endl;
  std::cout << (float)color[0] << " " << (float)color[1] << " " << (float)color[2] << std::endl;
  std::cout << "checkRGB" << std::endl;
  std::cout << (float)pDoc->m_pUI->get_m_R() << " " << (float)pDoc->m_pUI->get_m_G() << " " << (float)pDoc->m_pUI->get_m_B() << std::endl;

  */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0, alpha);
}

void ImpBrush::getSourceRGB(const Point source, GLubyte color[3]) {
  ImpressionistDoc *pDoc = GetDocument();

  memcpy(color, pDoc->GetOriginalPixel(source), 3);
}

// input a int matrix as kernel, return array [0]->R, [1] ->G, [2] ->B
double* ImpBrush::kernelOperation(const Point Source, int arr[3][3], int kernel_size) {
    double sum[4] = { 0,0,0,0 };
    double intensity = 0;
    GLubyte color_origin[3];
    int column = 0;
    int row = 0;

    for (int i = Source.x - (kernel_size - 1) / 2; i <= Source.x + (kernel_size - 1) / 2; i++) {//column
        row = 0;
        for (int j = Source.y - (kernel_size - 1) / 2; j <= Source.y + (kernel_size - 1) / 2; j++) {//row
            memcpy(color_origin, (m_pDoc->*(m_pDoc->GetPixel))(Point(abs(i),abs(j))), 3);
            sum[0] += ((double)color_origin[0]* (double)arr[row][column]);
            sum[1] += ((double)color_origin[1]* (double)arr[row][column]);
            sum[2] += ((double)color_origin[2]* (double)arr[row][column]);
            intensity = 0.299 * (double)color_origin[0] + 0.587 * (double)color_origin[1] + 0.114 * (double)color_origin[2];
            sum[3] += intensity * (double)arr[row][column];
            row++;
        }
        column++;
    }
    return sum;
}


// return gradient direction and magnitude, [1] ->direction,[2]->magnitude
float* ImpBrush::getGradient(const Point Source) {

    int o_sobelX[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
    int o_sobelY[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };
    int kernel_size = 3;
    double* raw_gradientX = kernelOperation(Source, o_sobelX, kernel_size);
    double* raw_gradientY = kernelOperation(Source, o_sobelY, kernel_size);
    float gradientX = raw_gradientX[3];
    float gradientY = raw_gradientY[3];

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

void ImpBrush::EdgeClipMove(const Point source, const Point target, const int size, const int angle) {

    std::cout << "edgerun" << std::endl;
    ImpressionistDoc* pDoc = GetDocument();
    if (pDoc->m_ucEdgeImage == NULL) {
     std::cout << "edgeAuto" << std::endl;
        EdgeAutoPaint();
    }
    float start_x = target.x - (cos(DEG2RAD(angle)) * size / 2);
    float start_y = target.y - (sin(DEG2RAD(angle)) * size / 2);
    float end_x = target.x + (cos(DEG2RAD(angle)) * size / 2);
    float end_y = target.y + (sin(DEG2RAD(angle)) * size / 2);

    int iterate = 0;
    bool end_found = FALSE;
    bool start_found = FALSE;

    while (iterate < size / 2) {
        if (!end_found) {
            GLubyte* color_e = pDoc->GetEdgeImagePixel(source.x + cos(DEG2RAD(angle)) * iterate, source.y + sin(DEG2RAD(angle)) * iterate);
            std::cout << "end:" << 0.299 * (float)color_e[0] + 0.587 * (float)color_e[1] + 0.114 * (float)color_e[2] << std::endl;
            if ((0.299 * (float)color_e[0] + 0.587 * (float)color_e[1] + 0.114 * (float)color_e[2]) / 255 >0.9) {
                end_x = target.x + cos(DEG2RAD(angle)) * iterate;
                end_y = target.y + sin(DEG2RAD(angle)) * iterate;
                end_found = TRUE;
            }
        }
        if (!start_found) {
            GLubyte* color_s = pDoc->GetEdgeImagePixel(source.x - cos(DEG2RAD(angle)) * iterate, source.y - sin(DEG2RAD(angle)) * iterate);
            std::cout << "start:" << 0.299 * (float)color_s[0] + 0.587 * (float)color_s[1] + 0.114 * (float)color_s[2] << std::endl;
            if ((0.299 * (float)color_s[0] + 0.587 * (float)color_s[1] + 0.114 * (float)color_s[2]) / 255 >0.9) {
                start_x = target.x - cos(DEG2RAD(angle)) * iterate;
                start_y = target.y - sin(DEG2RAD(angle)) * iterate;
                start_found = TRUE;
            }
        }
        if (start_found && end_found)
            break;
        iterate++;
    }
    glVertex2f(start_x, start_y);
    glVertex2f(end_x, end_y);
}
void ImpBrush::EdgeAutoPaint() {
    ImpressionistDoc* pDoc = GetDocument();
    GLubyte* pixelData = new GLubyte[pDoc->m_nWidth * pDoc->m_nHeight * 3];
    for (int j = 0; j <  pDoc->m_nHeight; j++) {
        for (int i = 0; i < pDoc->m_nWidth; i++) {
            Point temp(i, j);
            float* gradient = getGradient(temp); 
            if (gradient[1] > pDoc->m_pUI->getEdgeThreshold()) {//threshold
                int index = (j) * pDoc->m_nWidth + i;
                pixelData[index * 3] = 255; // Red component
                pixelData[index * 3 + 1] = 255; // Green component
                pixelData[index * 3 + 2] = 255; // Blue component
            }
            else {
                int index = (j) * pDoc->m_nWidth + i;
                pixelData[index * 3] = 0; // Red component
                pixelData[index * 3 + 1] = 0; // Green component
                pixelData[index * 3 + 2] = 0; // Blue component
            }
        }
    }
    size_t bufferSize = pDoc->m_nWidth * pDoc->m_nHeight * 3;
    if (pDoc->m_ucEdgeImage != nullptr) {
        delete[]pDoc->m_ucEdgeImage;
        pDoc->m_ucEdgeImage = nullptr;
    }
    pDoc->m_ucEdgeImage = new unsigned char[bufferSize];
    memset(pDoc->m_ucEdgeImage, 0, bufferSize);
    std::memcpy(pDoc->m_ucEdgeImage, pixelData, bufferSize);

}