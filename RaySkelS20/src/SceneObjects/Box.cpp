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
    vec3f P =r.at(Tnear);
    vec3f N(0.0, 0.0, 0.0);

    if (abs(bb.max[0] - P[0]) < RAY_EPSILON)
        N[0] = 1.0;
    else if (abs(bb.min[0] - P[0]) < RAY_EPSILON)
        N[0] = -1.0;
    if (abs(bb.max[1] - P[1]) < RAY_EPSILON)
        N[1] = 1.0;
    else if (abs(bb.min[1] - P[1]) < RAY_EPSILON)
        N[1] = -1.0;
    if (abs(bb.max[2] - P[2]) < RAY_EPSILON)
        N[2] = 1.0;
    else if (abs(bb.min[2] - P[2]) < RAY_EPSILON)
        N[2] = -1.0;
    //i.setN(r.getDirection());
    i.setN(N);

    i.N.normalize();

    return true;
  } else {
    return false;
  }

  return false;
}