//
// impressionistDoc.h
//
// header file for Doc
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "bitmap.h"
#include "impressionist.h"

#include <vector>

// Each stroke direction type has an associated constant.
typedef enum {
  SLIDER_RIGHT_MOUSE = 0,
  GRADIENT,
  BRUSH_DIRECTION,
  NUM_STROKE_DIRECTION
} StrokeDirection;

class ImpressionistUI;

class ImpressionistDoc {
public:
  ImpressionistDoc();

  void setUI(ImpressionistUI *ui); // Assign the UI to use

  int loadImage(char *iname);      // called by the UI to load image
  int loadMuralImage(char *iname); // called by the UI to load mural image

  int saveImage(char *iname); // called by the UI to save image

  int clearCanvas();           // called by the UI to clear the drawing canvas
  int swapCanvas();            // called by the UI to swap the canvas
  void setBrushType(int type); // called by the UI to set the brushType

  // called by the UI to set the Stroke Direction
  void setStokeDirection(int type);
  int getSize(); // get the UI size
  int getLineWidth();
  int getLineAngle();
  float getAlpha();
  int getSpacing();
  int getEdgeThreshold();

  void setLineAngle(int angle);
  void setSize(int size); // set the UI size
  char *getImageName();   // get the current image name

  // Attributes
public:
  // Dimensions of original window.
  int m_nWidth, m_nHeight;
  // Dimensions of the paint window.
  int m_nPaintWidth, m_nPaintHeight;
  // Bitmaps for original image and painting.
  unsigned char *m_ucOriginal;
  unsigned char *m_ucBitmap;
  unsigned char *m_ucPainting;

  // The current active brush.
  ImpBrush *m_pCurrentBrush;
  StrokeDirection m_pCurrentDirection;

  bool isEdgeClipping;
  bool isAnotherGradient;
  bool isSizeRand;

  ImpressionistUI *m_pUI;

  // Operations
public:
  // Get the color of the original picture at the specified coord
  GLubyte *GetOriginalPixel(int x, int y);
  // Get the color of the original picture at the specified point
  GLubyte *GetOriginalPixel(const Point p);

  void applyKernel(GLubyte *target, std::vector<std::vector<float>> kernel,
                   int width, int height);

private:
  char m_imageName[256];
};

extern void MessageBox(char *message);

#endif
