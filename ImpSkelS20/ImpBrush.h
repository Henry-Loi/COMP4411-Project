#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum {
  BRUSH_POINTS = 0,
  BRUSH_LINES,
  BRUSH_CIRCLES,
  BRUSH_SCATTERED_POINTS,
  BRUSH_SCATTERED_LINES,
  BRUSH_SCATTERED_CIRCLES,
  BRUSH_ALPHA_MAPPED,
  BRUSH_CUSTOM_KERNEL,
  BRUSH_CURVED,
  BRUSH_BLUR,
  BRUSH_SHARPENING,
  BRUSH_PIXELIZE,
  NUM_BRUSH_TYPE, // Make sure this stays at the end!
  /* implicit brush here */
  BRUSH_PAINTERLY,
};
// Decide to get pixel from Original Image/Another Image /Edge Image

class ImpressionistDoc; // Pre-declaring class
extern float frand();

class Point {
public:
  Point(){};
  Point(int xx, int yy) {
    x = xx;
    y = yy;
  };

  int x, y;

  Point scattered(float range) {
    return Point(this->x + frand() * range - range / 2,
                 this->y + frand() * range - range / 2);
  }
};

class ImpBrush {
protected:
  ImpBrush::ImpBrush(ImpressionistDoc *pDoc = NULL, char *name = NULL);

public:
  // The implementation of your brush should realize these virtual functions
  virtual void BrushBegin(const Point source, const Point target) = 0;
  virtual void BrushMove(const Point source, const Point target) = 0;
  virtual void BrushEnd(const Point source, const Point target) = 0;

  virtual int BrushInit(void *) = 0;

  // according to the source image and the position, determine the draw color
  void SetColor(const Point source);
  void SetColorAlpha(const Point source, float alpha);

  void getSourceRGB(const Point source, unsigned char color[3]);

  // get Doc to communicate with it
  ImpressionistDoc *GetDocument(void);

  // Return the name of the brush (not used in this version).
  char *BrushName(void);

  static int c_nBrushCount;     // How many brushes we have,
  static ImpBrush **c_pBrushes; // and what they are.
  float *kernelOperation(const Point Source, int arr[3][3],
                         const int kernel_size); // Use kernel/mask on the image
                                                 // and get value from local
  float *getGradient(const Point Source);
  void EdgeClipMove(const Point source, const Point target, const int size,
                    const int angle);
  void EdgeAutoPaint();

protected:
  ImpressionistDoc *m_pDoc;

private:
  // Brush's name (not used in this version).
  char *m_pBrushName;
};

#endif