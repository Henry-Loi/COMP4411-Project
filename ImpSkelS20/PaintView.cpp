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
Point start(0,0);//start of right click
boolean rightCLick = false;

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
  
    Point cur;//current position of cursor after rite click

    //if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
    //  if (target.x - target.y != 0) {
    //    m_pDoc->m_pUI->setLineAngle(
    //        atan((target.y - last_target.x) / (target.x - target.y)));
    //  } else {
    //    m_pDoc->m_pUI->setLineAngle(0);
    //  }
    //}


    last_target = target;
    int angle = 0;
    int size = 0;
    // This is the event handler
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
      break;
    case LEFT_MOUSE_DRAG:
      m_pDoc->m_pCurrentBrush->BrushMove(source, target);
      break;
    case LEFT_MOUSE_UP:
      m_pDoc->m_pCurrentBrush->BrushEnd(source, target);

      SaveCurrentContent();
      RestoreContent();
      break;
    case RIGHT_MOUSE_DOWN:
        if (!rightCLick) {
            start.x =coord.x;
            start.y = coord.y;
            rightCLick = true;
        }
        m_pDoc->setBrushType(BRUSH_LINES);
        ((SingleLineBrush*)(m_pDoc->m_pCurrentBrush))->RightClickBrushBegin(source, target);
      // MY TODO: Implement mouse down line creation
      break;
    case RIGHT_MOUSE_DRAG:

            cur.x = coord.x;
            cur.y = coord.y;
            angle = RAD2DEG(atan((double)(((double)cur.y - (double)start.y)*(-1) / ((double)cur.x - (double)start.x))));
            std::cout << "enter: angle" << angle << std::endl;
            std::cout << "x:" << cur.x << "y:" << cur.y << std::endl;
            std::cout << "st_x:" << start.x << "st_y:" << start.y << std::endl;
            if (rightCLick)
                std::cout << "rightCLicked" << std::endl;
            if (angle < 0)
                angle = 360 + angle;

            ////set angle
            m_pDoc->m_pUI->setLineAngle(angle);
            ////set size
            size = sqrt(pow((cur.y - start.y), 2) + pow((cur.x - start.x), 2));
            m_pDoc->m_pUI->setSize(size);
            std::cout << "size:" <<size <<std::endl;
            //m_pDoc->m_pUI->setSize(15);
            ////set midpoint
            cur.x = (cur.x + start.x) / 2;
            cur.y = (cur.y + start.y) / 2;
            source.x = cur.x + m_nStartCol;
            source.y = m_nEndRow - cur.y;
            target.x = cur.x;
            target.y = m_nWindowHeight - cur.y;
            ((SingleLineBrush*)(m_pDoc->m_pCurrentBrush))->PointerMove(source, target);
            //glColor3f(0, 0, 0);
            //m_pDoc->m_pCurrentBrush->BrushMove(source, target);
            glFlush();

      // MY TODO: Implement line real time update
      break;
    case RIGHT_MOUSE_UP:
      // MY TODO: Calculate the line angle and set it in the document
        m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
        rightCLick = false;
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

#ifndef MESA
  // To avoid flicker on some machines.
  glDrawBuffer(GL_BACK);
#endif // !MESA

  return 0;
}
