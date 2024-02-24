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

typedef enum {
  PAINTERLY_IMPRESSIONIST = 0,
  PAINTERLY_EXPRESSIONIST,
  PAINTERLY_COLOR_WASH,
  PAINTERLY_POINTILLIST,
  PAINTERLY_CUSTOMIZED,
  NUM_OF_PAINTERLY_STYLE
} PainterlyStyle;

typedef enum {
  PAINTERLY_CURVE_BRUSH = 0,
  PAINTERLY_BSPLINE_BRUSH,
  PAINTERLY_CIRCLE_BRUSH,
  PAINTERLY_CLIP_LINE_BRUSH,
  PAINTERLY_LINE_BRUSH,
  NUM_OF_PAINTERLY_STROKE
} PainterlyStroke;

class PainterlyParam {
public:
  PainterlyParam()
      : Threshold(0), Curvature(0), Blur(0), GridSize(0), MinStrokeLength(0),
        MaxStrokeLength(0), Alpha(0), Layer(0), R0Level(0), Jr(0), Jg(0), Jb(0),
        Jh(0), Js(0), Jv(0){};
  PainterlyParam(int threshold, float curvature, float blur, float gridSize,
                 int minStrokeLength, int maxStrokeLength, float alpha,
                 int layer, int r0Level, float jr, float jg, float jb, float jh,
                 float js, float jv)
      : Threshold(threshold), Curvature(curvature), Blur(blur),
        GridSize(gridSize), MinStrokeLength(minStrokeLength),
        MaxStrokeLength(maxStrokeLength), Alpha(alpha), Layer(layer),
        R0Level(r0Level), Jr(jr), Jg(jg), Jb(jb), Jh(jh), Js(js), Jv(jv){};

  int Threshold;
  float Curvature;
  float Blur;
  float GridSize;
  int MinStrokeLength;
  int MaxStrokeLength;
  float Alpha;
  int Layer;
  int R0Level;
  float Jr;
  float Jg;
  float Jb;
  float Jh;
  float Js;
  float Jv;
};

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

  void setPainterlyStyle(PainterlyStyle style);
  void setPainterlyStroke(PainterlyStroke stroke);

  Point last_target;

  // auto painting
  int autoPaint(void);

  // custom kernel
  void applyKernel(void);

  // painterly
  PainterlyParam m_painterlyParam[NUM_OF_PAINTERLY_STYLE];
  void apply_painterly(void);
  void paintLayer(unsigned char *canvas, unsigned char *referenceImage,
                  int radius);

  ImpressionistDoc *m_pDoc;

private:
  GLvoid *m_pPaintBitstart;
  int m_nDrawWidth, m_nDrawHeight, m_nStartRow, m_nEndRow, m_nStartCol,
      m_nEndCol, m_nWindowWidth, m_nWindowHeight;

  // painterly
  PainterlyStyle m_painterlyStyle;
  PainterlyStroke m_painterlyStroke;
  const int R[3] = {2, 4, 8};
};

#endif