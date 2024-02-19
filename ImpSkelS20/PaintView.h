//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include "ImpBrush.h"
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

class ImpressionistDoc;

class PaintView : public Fl_Gl_Window {
public:
  PaintView(int x, int y, int w, int h, const char *l);
  void draw();
  int handle(int event);

  void refresh();

  void resizeWindow(int width, int height);

  void SaveCurrentContent();

  void RestoreContent();
  void RightClickBrushBegin(const Point source, const Point target);
  void PointerMove(const Point source, const Point target, const Point start);
  void RightClickBrushEnd(const Point source, const Point target);

  Point last_target;

  // auto painting
  int autoPaint(void);

  // custom kernal
  void applyKernel(void);

  ImpressionistDoc *m_pDoc;

private:
  GLvoid *m_pPaintBitstart;
  int m_nDrawWidth, m_nDrawHeight, m_nStartRow, m_nEndRow, m_nStartCol,
      m_nEndCol, m_nWindowWidth, m_nWindowHeight;
};

#endif