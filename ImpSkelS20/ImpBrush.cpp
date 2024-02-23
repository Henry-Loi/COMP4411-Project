//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "ImpBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
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