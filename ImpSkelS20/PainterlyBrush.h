//
// PainterlyBrush.h
//
// The header file for Painterly Brush.
//

#ifndef PAINTERLY_BRUSH_H
#define PAINTERLY_BRUSH_H

#include "ImpBrush.h"
#include <vector>

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

class PainterlyBrush : public ImpBrush {
  friend class CurvedBrush;

public:
  PainterlyBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

  struct Stroke {
    int size;
    float depth;
    unsigned char color[3];
    std::vector<Point> controlPoints;
  };

  // painterly
  PainterlyStyle m_painterlyStyle;
  PainterlyStroke m_painterlyStroke;

  const int R[3] = {2, 4, 8};
  const float JitterIndex = 0.2;

  PainterlyParam param[NUM_OF_PAINTERLY_STYLE];

  void BrushBegin(const Point source, const Point target){};
  void BrushMove(const Point source, const Point target){};
  void BrushEnd(const Point source, const Point target){};

  int BrushInit(void *) { return 1; }

  ImpressionistDoc *m_pDoc;

  void StartPaint(std::vector<Stroke *>, float *);
  void renderCircles(int x, int y, Stroke &stroke, float *zBuffer);
};

#endif