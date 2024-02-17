//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "paintview.h"
#include "ImpBrush.h"
#include "fastmath.h"
#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <stdexcept>

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

    if (m_pDoc->m_pCurrentDirection == BRUSH_DIRECTION) {
      if (target.x - target.y != 0) {
        m_pDoc->m_pUI->setLineAngle(
            atan((target.y - last_target.x) / (target.x - target.y)));
      } else {
        m_pDoc->m_pUI->setLineAngle(0);
      }
    }

    last_target = target;

    // This is the event handler
    switch (eventToDo) {
    case LEFT_MOUSE_DOWN:
      // m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
      autoPaint();
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
      // MY TODO: Implement mouse down line creation
      break;
    case RIGHT_MOUSE_DRAG:
      // MY TODO: Implement line real time update
      break;
    case RIGHT_MOUSE_UP:
      // MY TODO: Calculate the line angle and set it in the document
      m_pDoc->m_pUI->setLineAngle(m_pDoc->m_pUI->m_tarLineAngle);
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
  for (float i = original_size / 4; i < m_nDrawWidth + spacing; i += spacing) {
    for (float j = original_size / 4; j < m_nDrawHeight + spacing;
         j += spacing) {
      if (m_pDoc->m_pUI->isSizeRand) {
        m_pDoc->m_pUI->setSize(irand(original_size));
      }

      Point source((i + m_nStartCol), ((float)m_nEndRow - j));
      Point target(i, (float)m_nWindowHeight - j);

      m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
      m_pDoc->m_pCurrentBrush->BrushMove(source, target);
      m_pDoc->m_pCurrentBrush->BrushEnd(source, target);
    }
  }

  SaveCurrentContent();
  RestoreContent();

  // finish auto painting
  m_pDoc->m_pUI->setSize(original_size);

  glFlush();

  return 1;
}
