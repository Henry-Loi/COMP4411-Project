//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

//  Paint_coordinates
//
//  Y-
//  |
//  |
//  |
//  |_______X+

#include "paintview.h"
#include "ImpBrush.h"
#include "fastmath.h"
#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#include "ScatteredLinesBrush.h"
#include "SingleLineBrush.h"

#define LEFT_MOUSE_DOWN 1
#define LEFT_MOUSE_DRAG 2
#define LEFT_MOUSE_UP 3
#define RIGHT_MOUSE_DOWN 4
#define RIGHT_MOUSE_DRAG 5
#define RIGHT_MOUSE_UP 6

#ifndef WIN32
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

static int eventToDo;
static int isAnEvent = 0;
static Point coord;
Point right_start(0, 0); // start of right click
bool rightCLick = false;
float right_size = 0;
float right_angle = 0;

int irand(int);

PaintView::PaintView(int x, int y, int w, int h, const char *l,
                     ImpressionistDoc *pDoc)
    : Fl_Gl_Window(x, y, w, h, l) {
  m_pDoc = pDoc;
  m_nWindowWidth = w;
  m_nWindowHeight = h;
  m_pUndoBitstart = new unsigned char[m_nWindowWidth * m_nWindowHeight * 3];
};

void PaintView::draw() {
#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

  if (!valid()) {

    glClearColor(0.7f, 0.7f, 0.7f, 1.0);

    // We're only using 2-D, so turn off depth
    glDisable(GL_DEPTH_TEST);

    ortho();

    glClear(GL_COLOR_BUFFER_BIT);
  }

  if (isUndo) {
    isUndo = false;
    return;
  }

  Point scrollpos; // = GetScrollPosition();
  scrollpos.x = 0;
  scrollpos.y = 0;

  m_nWindowWidth = w();
  m_nWindowHeight = h();

  int drawWidth, drawHeight;
  drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
  drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

  int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
  if (startrow < 0)
    startrow = 0;

  m_pPaintBitstart = m_pDoc->m_ucPainting +
                     3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

  m_nDrawWidth = drawWidth;
  m_nDrawHeight = drawHeight;

  m_nStartRow = startrow;
  m_nEndRow = startrow + drawHeight;
  m_nStartCol = scrollpos.x;
  m_nEndCol = m_nStartCol + drawWidth;

  if (m_pDoc->m_ucPainting && !isAnEvent) {
    RestoreContent();
  }

  if (m_pDoc->m_ucPainting && isAnEvent) {

    // Clear it after processing.
    isAnEvent = 0;

    Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
    Point target(coord.x, m_nWindowHeight - coord.y);

    Point cur; // current position of cursor after rite click

    // if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
    //   if (target.x - target.y != 0) {
    //     m_pDoc->m_pUI->setLineAngle(
    //         atan((target.y - last_target.x) / (target.x - target.y)));
    //   } else {
    //     m_pDoc->m_pUI->setLineAngle(0);
    //   }
    // }

    last_target = target;
    int angle = 0;
    int size = 0;
    // This is the event handler
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      if (coord.x > m_pDoc->m_nPaintWidth || coord.y > m_pDoc->m_nPaintHeight) {
        break;
      }
      if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
        // coordinate of start point
        right_start.x = coord.x;
        right_start.y = coord.y;
      }
      m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
      break;
    case LEFT_MOUSE_DRAG:
      if (coord.x > m_pDoc->m_nPaintWidth || coord.y > m_pDoc->m_nPaintHeight) {
        break;
      }
      if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
        // coordinate of endpoint
        cur.x = coord.x;
        cur.y = coord.y;
        angle = RAD2DEG(atan(((double)right_start.y - (double)cur.y) /
                             ((double)cur.x - (double)right_start.x)));
        if (angle < 0)
          angle += 360;
        m_pDoc->m_pUI->setLineAngle(angle);
        // coordiante for start point of next iteration
        right_start.x = cur.x;
        right_start.y = cur.y;
      } else if (m_pDoc->m_pCurrentDirection == GRADIENT) {
        angle = RAD2DEG(GradientDirection(source, target));
        if (angle < 0)
          angle += 360;
        m_pDoc->m_pUI->setLineAngle(angle);
      }
      m_pDoc->m_pCurrentBrush->BrushMove(source, target);
      std::cout << m_pDoc->m_pUI->get_m_R() << " " << m_pDoc->m_pUI->get_m_B()
                << " " << m_pDoc->m_pUI->get_m_G() << " " << std::endl;

      break;
    case LEFT_MOUSE_UP:
      m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
      m_pDoc->saveUndo();
      SaveCurrentContent();
      RestoreContent();
      break;
    case RIGHT_MOUSE_DOWN:
      RightClickBrushBegin(source, target);
      break;
    case RIGHT_MOUSE_DRAG:

      PointerMove(source, target, right_start);
      RestoreContent();
      break;
    case RIGHT_MOUSE_UP:
      RightClickBrushEnd(source, target);

      break;

    default:
      printf("Unknown event!!\n");
      break;
    }
  }

  glFlush();

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA
}

int PaintView::handle(int event) {
  switch (event) {
  case FL_ENTER:
    redraw();
    break;
  case FL_PUSH:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    if (Fl::event_button() > 1)
      eventToDo = RIGHT_MOUSE_DOWN;
    else
      eventToDo = LEFT_MOUSE_DOWN;
    isAnEvent = 1;
    redraw();
    break;
  case FL_DRAG:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    if (Fl::event_button() > 1)
      eventToDo = RIGHT_MOUSE_DRAG;
    else
      eventToDo = LEFT_MOUSE_DRAG;
    isAnEvent = 1;
    redraw();
    m_pDoc->m_pUI->m_origView->updateCursor(coord);
    break;
  case FL_RELEASE:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    if (Fl::event_button() > 1)
      eventToDo = RIGHT_MOUSE_UP;
    else
      eventToDo = LEFT_MOUSE_UP;
    isAnEvent = 1;
    redraw();
    break;
  case FL_MOVE:
    coord.x = Fl::event_x();
    coord.y = Fl::event_y();
    m_pDoc->m_pUI->m_origView->updateCursor(coord);
    break;
  default:
    return 0;
    break;
  }

  return 1;
}

void PaintView::refresh() { redraw(); }

void PaintView::resizeWindow(int width, int height) {
  resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent() {
  // Tell openGL to read from the front buffer when capturing
  // out paint strokes
  glReadBuffer(GL_FRONT);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

  glReadPixels(0, m_nWindowHeight - m_nDrawHeight, m_nDrawWidth, m_nDrawHeight,
               GL_RGB, GL_UNSIGNED_BYTE, m_pPaintBitstart);
}

void PaintView::RestoreContent() {
  glDrawBuffer(GL_BACK);

  glClear(GL_COLOR_BUFFER_BIT);

  glRasterPos2i(0, m_nWindowHeight - m_nDrawHeight);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
  glDrawPixels(m_nDrawWidth, m_nDrawHeight, GL_RGB, GL_UNSIGNED_BYTE,
               m_pPaintBitstart);

  //	glDrawBuffer(GL_FRONT);
}

int irand(int);
int PaintView::autoPaint(void) {
#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA

  m_pDoc->saveUndo();

  Point scrollpos; // = GetScrollPosition();
  scrollpos.x = 0;
  scrollpos.y = 0;

  m_nWindowWidth = w();
  m_nWindowHeight = h();

  int drawWidth, drawHeight;
  drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
  drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

  int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
  if (startrow < 0)
    startrow = 0;

  m_pPaintBitstart = m_pDoc->m_ucPainting +
                     3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

  m_nDrawWidth = drawWidth;
  m_nDrawHeight = drawHeight;

  m_nStartRow = startrow;
  m_nEndRow = startrow + drawHeight;
  m_nStartCol = scrollpos.x;
  m_nEndCol = m_nStartCol + drawWidth;

  float original_size = m_pDoc->m_pUI->getSize();
  float spacing = m_pDoc->m_pUI->getSpacing();

  // Create a vector of pairs for all possible (i, j) values
  std::vector<std::pair<float, float>> ij_pairs;
  for (float i = original_size / 4; i < m_nDrawWidth + spacing; i += spacing) {
    for (float j = original_size / 4; j < m_nDrawHeight + spacing;
         j += spacing) {
      ij_pairs.push_back(std::make_pair(i, j));
    }
  }

  // Shuffle the vector to get a random order
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(ij_pairs.begin(), ij_pairs.end(), g);

  // Iterate over the shuffled vector
  for (const auto &ij : ij_pairs) {
    float i = ij.first;
    float j = ij.second;

    if (m_pDoc->m_pUI->isSizeRand) {
      m_pDoc->m_pUI->setSize(original_size * (0.8 + irand(20) / 100.0));
    }

    Point source((i + m_nStartCol), ((float)m_nEndRow - j));
    Point target(i, (float)m_nWindowHeight - j);

    m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
    m_pDoc->m_pCurrentBrush->BrushMove(source, target);
    m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
  }

  SaveCurrentContent();
  RestoreContent();

  // finish auto painting
  m_pDoc->m_pUI->setSize(original_size);

  glFlush();

  refresh();
  m_pDoc->m_pUI->m_origView->updateCursor(coord);

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA

  return 0;
}

void PaintView::RightClickBrushBegin(const Point source, const Point target) {

  ImpressionistUI *dlg = m_pDoc->m_pUI;

  int width = m_pDoc->getLineWidth();

  glLineWidth((float)width);
  if (!rightCLick) {
    right_start.x = coord.x;
    right_start.y = coord.y;
    rightCLick = true;
  }
}

void PaintView::PointerMove(const Point source, const Point target,
                            const Point start) {

  ImpressionistUI *dlg = m_pDoc->m_pUI;

  if (m_pDoc == NULL) {
    printf(":PaintView  document is NULL\n");
    return;
  }
  Point cur(coord.x, coord.y);

  Point new_source((cur.x + start.x) / 2 + m_nStartCol,
                   m_nEndRow - (cur.y + start.y) / 2);
  Point new_target((cur.x + start.x) / 2,
                   m_nWindowHeight - (cur.y + start.y) / 2);

  glColor3f(1, 0, 0);
  glBegin(GL_LINES);

  right_size = sqrt(pow(cur.x - start.x, 2) + pow(cur.y - start.y, 2));
  right_angle = atan(((double)start.y - (double)cur.y) /
                     ((double)cur.x - (double)start.x));
  std::cout << "angle:" << right_angle << std::endl;
  std::cout << "start.x:" << start.x << "start.y" << start.y << std::endl;
  std::cout << "cur.x:" << cur.x << "cur.y" << cur.y << std::endl;
  glVertex2f(new_target.x - (cos(right_angle) * right_size / 2),
             new_target.y - (sin(right_angle) * right_size / 2));
  glVertex2f(new_target.x + (cos(right_angle) * right_size / 2),
             new_target.y + (sin(right_angle) * right_size / 2));

  glEnd();
}

void PaintView::RightClickBrushEnd(const Point source, const Point target) {

  ImpressionistUI *dlg = m_pDoc->m_pUI;
  if (typeid(*(m_pDoc->m_pCurrentBrush)) == typeid(SingleLineBrush) ||
      typeid(*(m_pDoc->m_pCurrentBrush)) == typeid(ScatteredLinesBrush)) {
    dlg->setLineAngle(RAD2DEG(right_angle));
    dlg->setSize(right_size);
  }
  rightCLick = false;
}

// find gradient angle(degree)
// (x,y) color_origin[0]->(0,0); color_origin[1]->(0,-1);
// color_origin[2]->(1,0);
float PaintView::GradientDirection(const Point source, const Point target) {

  Point temp;
  GLubyte color_origin[3][3];
  float intensity[3] = {0};
  for (int i = 0; i < 3; i++) {
    temp.y = source.y - i % 2;
    temp.x = source.x + i / 2;
    memcpy(color_origin[i], m_pDoc->GetOriginalPixel(temp), 3);
    intensity[i] = 0.299 * (float)color_origin[i][0] +
                   0.587 * (float)color_origin[i][2] +
                   0.114 * (float)color_origin[i][1];
  }

  return atan((intensity[1] - intensity[0]) / (intensity[2] - intensity[0]));
}

void PaintView::applyKernel() {
  ImpressionistUI *dlg = m_pDoc->m_pUI;
  if (dlg->matrix_kernel.size() == 0) {
    return;
  }
  ImpBrush::c_pBrushes[BRUSH_CUSTOM_KERNEL]->BrushInit(nullptr);
}

void PaintView::setPainterlyStyle(PainterlyStyle style) {
  // m_pDoc->m_pUI->m_painterlyStyle = style;
}

void PaintView::setPainterlyStroke(PainterlyStroke stroke) {
  // m_pDoc->m_pUI->m_painterlyStroke = stroke;
}

void PaintView::apply_painterly(void) {
#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA
  m_pDoc->clearCanvas();

  /* pseduo code
    function paint(sourceImage,R1 ... Rn)
  { canvas := a new constant color image
  // paint the canvas
  for each brush radius Ri,
  from largest to smallest do
  { // apply Gaussian blur
  referenceImage = sourceImage * G(fσ Ri)
  // paint a layer
  paintLayer(canvas, referenceImage, Ri)
  }
  return canvas
  }
  */

  ImpressionistUI *dlg = m_pDoc->m_pUI;
  PainterlyParam *param = get_painterly_param();
  PainterlyBrush *m_pPainterlyBrush = m_pDoc->m_pUI->m_pPainterlyBrush;

  if (m_pDoc->m_ucPainting == NULL) {
    return;
  }
  if (m_pDoc->m_ucOriginal == NULL) {
    return;
  }

  int width = m_pDoc->m_nWidth;
  int height = m_pDoc->m_nHeight;

  int paint_width = m_pDoc->m_nPaintWidth;
  int paint_height = m_pDoc->m_nPaintHeight;

  // FIXME: is this a hack to make the painterly brush work?
  // glColor3d(1, 1, 1);
  // glBegin(GL_POLYGON);
  // glVertex2d(0, 0);
  // glVertex2d(0, height);
  // glVertex2d(width, height);
  // glVertex2d(width, 0);
  // glEnd();

  unsigned char *canvas = new unsigned char[width * height * 3];
  memset(canvas, 255, width * height * 3);

  // paint the canvas
  for (int i = 0; i < param->Layer; i++) {
    unsigned char *tmp = new unsigned char[width * height * 3];
    // apply Gaussian blur
    std::vector<std::vector<float>> GKernel =
        gaussKernel(m_pPainterlyBrush->R[i], param->Blur * param->Blur *
                                                 m_pPainterlyBrush->R[i] *
                                                 m_pPainterlyBrush->R[i]);
    if (param->Blur != 0) {
      m_pDoc->applyKernel(tmp, GKernel, m_pPainterlyBrush->R[i],
                          m_pPainterlyBrush->R[i]);
    } else {
      memcpy(tmp, m_pDoc->m_ucOriginal, width * height * 3);
    }

    // paint a layer
    paintLayer(canvas, tmp, m_pPainterlyBrush->R[i]);
    delete[] tmp;
  }

  SaveCurrentContent();
  RestoreContent();

  glFlush();

  refresh();
  m_pDoc->m_pUI->m_origView->updateCursor(coord);

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA
}

void PaintView::paintLayer(unsigned char *canvas, unsigned char *referenceImage,
                           int radius) {
  /* pesudo code
  procedure paintLayer(canvas,referenceImage, R)
    { S := a new set of strokes, initially empty
    // create a pointwise difference image
    D := difference(canvas,referenceImage)
    grid := fg R
    for x=0 to imageWidth stepsize grid do
    for y=0 to imageHeight stepsize grid do
    { // sum the error near (x,y)
    M := the region (x-grid/2..x+grid/2,
    y-grid/2..y+grid/2)
    areaError := i j M, ∈∑ Di,j / grid2
    if (areaError > T) then
    { // find the largest error point
    (x1,y1) := arg max i j M, ∈ Di,j
    s :=makeStroke(R,x1,y1,referenceImage)
    add s to S
    }
    }
    paint all strokes in S on the canvas,
    in random order
  }*/
  PainterlyParam *param = get_painterly_param();

  int height = m_pDoc->m_nPaintHeight, width = m_pDoc->m_nPaintWidth;
  float *zBuffer = new float[height * width];

  std::vector<PainterlyBrush::Stroke *> strokes;

  for (int i = 0; i < height * width; ++i)
    zBuffer[i] = 1.f;

  // create a pointwise difference image
  unsigned char *D = new unsigned char[m_nDrawWidth * m_nDrawHeight];

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int index = (i * width + j) * 3;
      D[i * width + j] = calDifference(canvas + index, referenceImage + index);
    }
  }

  m_pDoc->m_pUI->setSize(radius);

  int grid = param->GridSize * radius;

  for (int x = 0; x < height; x += grid) {
    for (int y = 0; y < width; y += grid) {
      // sum the error near (x,y)
      float areaError = 0;
      Point maxPoint;

      int startRow = max(0, y - grid / 2), startCol = max(0, x - grid / 2);
      int endRow = min(startRow + grid / 2, height),
          endCol = min(startCol + grid / 2, width);

      float maxError = -1;
      int gridSum = 0;
      for (int i = startRow; i < endRow; ++i)
        for (int j = startCol; j < endCol; ++j) {
          float curError = D[i * width + j];
          areaError += curError;
          gridSum++;
          if (curError > maxError) {
            maxPoint.x = i;
            maxPoint.y = j;
            maxError = curError;
          }
        }
      areaError /= gridSum;

      if (areaError > param->Threshold) {
        strokes.push_back(
            makeSplineStroke(canvas, maxPoint.y, maxPoint.x, radius));
      }
    }
  }

  // painterly brush
  m_pDoc->m_pUI->m_pPainterlyBrush->StartPaint(strokes, canvas);

  delete[] zBuffer;
  delete[] D;
  for (auto *stroke : strokes)
    delete stroke;
}

PainterlyBrush::Stroke *PaintView::makeSplineStroke(unsigned char *ref, int x,
                                                    int y, int size) {
  PainterlyBrush::Stroke *stroke = new PainterlyBrush::Stroke;
  stroke->depth = frand();
  stroke->size = size;

  Point cur(x, y);
  double prevGradX = 0.0, prevGradY = 0.0;
  stroke->controlPoints.push_back(cur);

  int width = m_pDoc->m_nPaintWidth, height = m_pDoc->m_nPaintHeight;
  int index = (y * width + x) * 3;
  memcpy(stroke->color, ref + index, 3);

  PainterlyParam *param = get_painterly_param();

  for (int i = 0; i < param->MaxStrokeLength; ++i) {
    index = (cur.y * width + cur.x) * 3;
    if (calDifference(ref + index, m_pDoc->m_ucPainting + index) <
        calDifference(ref + index, stroke->color)) {
      if (i >= param->MinStrokeLength)
        return stroke;
    }

    auto grad = calGradient(cur.x, cur.y, ref);
    float magnitude =
        std::sqrt(grad.first * grad.first + grad.second * grad.second);
    if (magnitude < 1e-4)
      return stroke; // vanishing gradient

    float gradX = -grad.second / magnitude,
          gradY = grad.first / magnitude; // find perpendicular and unit vector

    if (gradX * prevGradX + gradY * prevGradY <
        0) // find direction with smaller angle
    {
      gradX = -gradX;
      gradY = -gradY;
    }

    gradX = param->Curvature * gradX + (1 - param->Curvature) * prevGradX;
    gradY = param->Curvature * gradY + (1 - param->Curvature) * prevGradY;

    magnitude = std::sqrt(gradX * gradX + gradY * gradY);
    prevGradX = gradX / magnitude;
    prevGradY = gradY / magnitude;

    cur.x += prevGradX * size;
    cur.y += prevGradY * size;
    cur.x = max(cur.x, 0);
    cur.x = min(width - 1, cur.x);
    cur.y = max(cur.y, 0);
    cur.y = min(height - 1, cur.y);
    stroke->controlPoints.push_back(cur);
  }

  return stroke;
}

unsigned char *PaintView::getColor(int x, int y, unsigned char *src) {
  x = max(x, 0);
  x = min(x, m_pDoc->m_nPaintWidth - 1);
  y = max(y, 0);
  y = min(y, m_pDoc->m_nPaintHeight - 1);

  return src + (y * m_pDoc->m_nPaintWidth + x) * 3;
}

int sobelOpDirY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
int sobelOpDirX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

std::pair<float, float> PaintView::calGradient(int x, int y,
                                               unsigned char *src) {
  float gradX = 0.f, gradY = 0.f;
  for (int i = 0; i < 3; ++i)   // row
    for (int j = 0; j < 3; ++j) // col
    {
      float greyScale = rgb2Grey(getColor(x + j - 1, y + 2 - i, src));
      gradX += greyScale * sobelOpDirX[i][j];
      gradY += greyScale * sobelOpDirY[i][j];
    }
  return std::pair<float, float>(gradX, gradY);
}

PainterlyParam *PaintView::get_painterly_param(void) {
  return &m_pDoc->m_pUI->m_pPainterlyBrush
              ->param[m_pDoc->m_pUI->m_pPainterlyBrush->m_painterlyStyle];
}
