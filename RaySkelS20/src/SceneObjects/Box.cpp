#include <assert.h>
#include <cmath>

#include "Box.h"

bool Box::intersectLocal(const ray &r, isect &i) const {

  BoundingBox bb = ComputeLocalBoundingBox();
  double Tnear, Tfar;

  if (bb.intersect(r, Tnear, Tfar)) {
    if (Tnear < RAY_EPSILON || Tfar < RAY_EPSILON) {
      return false;
    }

    i.obj = this;
    i.setT(Tnear);
    i.setN(r.getDirection());
    i.N.normalize();

    return true;
  } else {
    return false;
  }

  return false;
}