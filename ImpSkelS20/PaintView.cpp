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
#include <random>
#include <stdexcept>
#include <vector>

#include "ScatteredLinesBrush.h"
#include "SingleLineBrush.h"
#include <iostream>

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
boolean rightCLick = false;
float right_size = 0;
float right_angle = 0;

int irand(int);

PaintView::PaintView(int x, int y, int w, int h, const char *l)
    : Fl_Gl_Window(x, y, w, h, l) {
  m_nWindowWidth = w;
  m_nWindowHeight = h;
}

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
    float angle = 0;
    int size = 0;
    // This is the event handler
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
        right_start.x = coord.x;
        right_start.y = coord.y;
      }
      m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
      break;
    case LEFT_MOUSE_DRAG:
      if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
        cur.x = coord.x;
        cur.y = coord.y;
        angle = atan(((double)right_start.y - (double)cur.y) /
                     ((double)cur.x - (double)right_start.x));
        m_pDoc->m_pUI->setLineAngle(RAD2DEG(angle));
        right_start.x = cur.x;
        right_start.y = cur.y;
      }
      m_pDoc->m_pCurrentBrush->BrushMove(source, target);
     
      std::cout << m_pDoc->m_pUI->getR() << " " << m_pDoc->m_pUI->getG() << " " << m_pDoc->m_pUI->getB() << std::endl;
      std::cout << m_pDoc->m_pUI->Color_Selection->r() << " " << m_pDoc->m_pUI->Color_Selection->g() << " " << m_pDoc->m_pUI->Color_Selection->b() << " " << std::endl;
      break;
    case LEFT_MOUSE_UP:
      m_pDoc->m_pCurrentBrush->BrushEnd(source, target);

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

void PaintView::applyKernel() {
  ImpressionistUI *dlg = m_pDoc->m_pUI;
  if (dlg->matrix_kernel.size() == 0) {
    return;
  }
  ImpBrush::c_pBrushes[BRUSH_CUSTOM_KERNEL]->BrushInit(nullptr);
}
