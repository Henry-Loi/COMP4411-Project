//
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the
// brush. It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>
#include <vcruntime_string.h>
#include <vector>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "CircleBrush.h"
#include "PointBrush.h"
#include "SingleLineBrush.h"
// scattered brushes
#include "ScatteredCirclesBrush.h"
#include "ScatteredLinesBrush.h"
#include "ScatteredPointsBrush.h"

// alpha mapped brush
#include "AlphaMappedBrush.h"

// kernel brush
#include "KernelBrush.h"

// painterly brush
#include "PainterlyBrush.h"

// MY TODO: add other brushes
#include "CurvedBrush.h"

#define DESTROY(p)                                                             \
  {                                                                            \
    if ((p) != NULL) {                                                         \
      delete[] p;                                                              \
      p = NULL;                                                                \
    }                                                                          \
  }

ImpressionistDoc::ImpressionistDoc() {
  // Set NULL image name as init.
  m_imageName[0] = '\0';

  m_nWidth = -1;
  m_ucBitmap = NULL;
  m_ucOriginal = NULL;
  m_ucPainting = NULL;

  // create one instance of each brush
  ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
  ImpBrush::c_pBrushes = new ImpBrush *[ImpBrush::c_nBrushCount];

  ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush(this, "Points");

  // Note: You should implement these 5 brushes.  They are set the same
  // (PointBrush) for now
  ImpBrush::c_pBrushes[BRUSH_LINES] = new SingleLineBrush(this, "Lines");
  ImpBrush::c_pBrushes[BRUSH_CIRCLES] = new CircleBrush(this, "Circles");
  ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] =
      new ScatteredPointsBrush(this, "Scattered Points");
  ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES] =
      new ScatteredLinesBrush(this, "Scattered Lines");
  ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] =
      new ScatteredCirclesBrush(this, "Scattered Circles");
  ImpBrush::c_pBrushes[BRUSH_ALPHA_MAPPED] =
      new AlphaMappedBrush(this, "Alpha Mapped");
  ImpBrush::c_pBrushes[BRUSH_CUSTOM_KERNEL] = new KernelBrush(this, "Kernel");
  ImpBrush::c_pBrushes[BRUSH_CURVED] = new CurvedBrush(this, "Curved");

  // make one of the brushes current
  m_pCurrentBrush = ImpBrush::c_pBrushes[0];
}

//---------------------------------------------------------
// Set the current UI
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI *ui) { m_pUI = ui; }

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char *ImpressionistDoc::getImageName() { return m_imageName; }

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type) {
  m_pCurrentBrush = ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setStokeDirection(int type) {
  m_pCurrentDirection = (StrokeDirection)type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize() { return m_pUI->getSize(); }
int ImpressionistDoc::getLineWidth() { return m_pUI->getLineWidth(); }
int ImpressionistDoc::getLineAngle() { return m_pUI->getLineAngle(); }
float ImpressionistDoc::getAlpha() { return m_pUI->getAlpha(); }
int ImpressionistDoc::getSpacing() { return m_pUI->getSpacing(); }
int ImpressionistDoc::getEdgeThreshold() { return m_pUI->getEdgeThreshold(); }

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) {
  // try to open the image to read
  unsigned char *data;
  int width, height;

  if ((data = readBMP(iname, width, height)) == NULL) {
    fl_alert("Can't load bitmap file");
    return 0;
  }

  // reflect the fact of loading the new image
  m_nWidth = width;
  m_nPaintWidth = width;
  m_nHeight = height;
  m_nPaintHeight = height;

  // release old storage
  if (m_ucOriginal != nullptr) {
    delete[] m_ucOriginal;
    m_ucOriginal = nullptr;
    m_ucBitmap = nullptr;
  }
  if (m_ucPainting != nullptr) {
    delete[] m_ucPainting;
    m_ucPainting = nullptr;
  }

  m_ucOriginal = data;
  m_ucBitmap = m_ucOriginal;

  // allocate space for draw view
  m_ucPainting = new unsigned char[width * height * 3];
  memset(m_ucPainting, 0, width * height * 3);

  // init the history record
  m_ucLastPainting = new unsigned char[width * height * 3];
  memset(m_ucLastPainting, 0, width * height * 3);

  m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
                              m_pUI->m_mainWindow->y(), width * 2, height + 25);

  // display it on origView
  m_pUI->m_origView->resizeWindow(width, height);
  m_pUI->m_origView->refresh();

  // refresh paint view as well
  m_pUI->m_paintView->resizeWindow(width, height);
  m_pUI->m_paintView->refresh();

  return 1;
}

//---------------------------------------------------------
// Load the mural image
// This is called by the UI when the load image button is
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadMuralImage(char *iname) {
  // empty image prevention
  if (!m_ucOriginal) {
    return loadImage(iname);
  }

  // try to open the image to read
  unsigned char *data;
  int width, height;

  if ((data = readBMP(iname, width, height)) == NULL) {
    fl_alert("Can't load bitmap file");
    return 0;
  }

  // in theory no need update window size
  if (width != m_nWidth || height != m_nHeight || m_nPaintHeight != m_nHeight ||
      m_nPaintWidth != m_nWidth) {
    fl_alert("The mural image should have the same size as the original image");
    return 0;
  }

  // release old storage
  delete[] m_ucOriginal;

  m_ucOriginal = data;
  m_ucBitmap = m_ucOriginal;

  return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) {

  writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

  return 1;
}

void ImpressionistDoc::Undo(void) {
  memcpy(m_ucPainting, m_ucLastPainting,
         m_nWidth * m_nHeight * 3 * sizeof(unsigned char));
  m_pUI->m_paintView->refresh();
}

void ImpressionistDoc::saveUndo(void) {
  memcpy(m_ucLastPainting, m_ucPainting,
         m_nWidth * m_nHeight * 3 * sizeof(unsigned char));
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() {

  // Release old storage
  if (m_ucPainting) {
    delete[] m_ucPainting;

    // allocate space for draw view
    m_ucPainting = new unsigned char[m_nPaintWidth * m_nPaintHeight * 3];
    memset(m_ucPainting, 0, m_nPaintWidth * m_nPaintHeight * 3);

    // refresh paint view as well
    m_pUI->m_paintView->refresh();
  }

  return 0;
}

int ImpressionistDoc::swapCanvas() {
  unsigned char *temp = m_ucOriginal;
  m_ucOriginal = m_ucPainting;
  m_ucBitmap = m_ucOriginal;
  m_ucPainting = temp;

  m_pUI->m_origView->refresh();
  m_pUI->m_paintView->refresh();

  return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte *ImpressionistDoc::GetOriginalPixel(int x, int y) {
  if (x < 0)
    x = 0;
  else if (x >= m_nWidth)
    x = m_nWidth - 1;

  if (y < 0)
    y = 0;
  else if (y >= m_nHeight)
    y = m_nHeight - 1;

  return (GLubyte *)(m_ucOriginal + 3 * (y * m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte *ImpressionistDoc::GetOriginalPixel(const Point p) {
  return GetOriginalPixel(p.x, p.y);
}

void ImpressionistDoc::applyKernel(GLubyte *target,
                                   std::vector<std::vector<float>> kernel,
                                   int width, int height) {
  // apply kernel to the original image
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      unsigned char *source, sum[3];

      for (int x = 0; x < kernel.size(); x++) {
        for (int y = 0; y < kernel.size(); y++) {
          int curX = i - kernel.size() / 2 + x,
              curY = j - kernel.size() / 2 + y;
          source = GetOriginalPixel(curX + j, curY + i);
          for (int k = 0; k < 3; ++k) {
            sum[k] += source[k] * kernel[i][j];
          }
        }
      }
      memcpy(target + (j * width + i) * 3, sum, 3);
    }
  }
}
