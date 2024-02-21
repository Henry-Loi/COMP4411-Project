//
// KernelBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush
// implementations will look like the file with the different GL primitive
// calls.
//

#include "Kernelbrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <exception>
#include <string>
#include <vector>

extern float frand();

KernelBrush::KernelBrush(ImpressionistDoc *pDoc, char *name)
    : ImpBrush(pDoc, name) {}

int KernelBrush::BrushInit(void *params) {
  ImpressionistDoc *pDoc = GetDocument();
  kernel = pDoc->m_pUI->matrix_kernel;

  // print kernel in alert
  std::string kernel_str = "";
  for (int i = 0; i < kernel.size(); i++) {
    for (int j = 0; j < kernel[i].size(); j++) {
      kernel_str += std::to_string(kernel[i][j]) + " ";
    }
    kernel_str += "\n";
  }

  fl_alert("Kernel:\n%s", kernel_str.c_str());

  return 1;
}

void KernelBrush::BrushBegin(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (kernel.size() == 0) {
    fl_alert("Please set the kernel first");
    return;
  }

  glPointSize(1.0f);

  BrushMove(source, target);
}

void KernelBrush::BrushMove(const Point source, const Point target) {
  ImpressionistDoc *pDoc = GetDocument();
  ImpressionistUI *dlg = pDoc->m_pUI;

  if (pDoc == NULL) {
    printf("KernelBrush::BrushMove  document is NULL\n");
    return;
  }

  SetColorAlpha(source, pDoc->getAlpha());
  glBegin(GL_POINTS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int ksize = sqrt(kernel.size());
  int bsize = pDoc->getSize();
  for (int w = 0; w < bsize; w++) {
    for (int h = 0; h < bsize; h++) {
      float sum[3] = {0};
      for (int i = 0; i < ksize; i++) {
        for (int j = 0; j < ksize; j++) {
          unsigned char color[3];
          Point new_target = Point(w + i - ksize / 2, h + j - ksize / 2);
          if (new_target.x < 0 || new_target.x >= pDoc->m_nWidth ||
              new_target.y < 0 || new_target.y >= pDoc->m_nHeight) {
            continue;
          }
          getSourceRGB(source, color);
          sum[0] += kernel[i][j] * color[0];
          sum[1] += kernel[i][j] * color[1];
          sum[2] += kernel[i][j] * color[2];
        }
      }
      glColor4f(sum[0] / 255.0, sum[1] / 255.0, sum[2] / 255.0,
                pDoc->getAlpha());
      glVertex2d(target.x - ksize / 2 + w, target.y - ksize / 2 + h);
    }
  }

  glEnd();
}

void KernelBrush::BrushEnd(const Point source, const Point target) {
  // do nothing so far
}
