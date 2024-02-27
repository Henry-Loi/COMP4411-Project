//
// BSplines.h
// Reference to https://zhuanlan.zhihu.com/p/50450278
//

#ifndef BSPLINES_H
#define BSPLINES_H

#include "ImpBrush.h"
#include <vector>

class BSplines {
public:
  BSplines(const std::vector<Point> &controlPoints, int degree, int nSamples);

  void createKnots();

  Point deBoor(int i, int j, float t);
  int getKnotIndex(float t);

  std::vector<Point> samples;
  const std::vector<Point> &controlPoints;
  float *knots;
  int nSamples;
  int nControls;
  int degree;
};

#endif
