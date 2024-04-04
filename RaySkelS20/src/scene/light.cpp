#include <cmath>

#include "../ui/TraceUI.h"
#include "light.h"

double DirectionalLight::distanceAttenuation(const vec3f &P) const {
  // distance to light is infinite, so f(di) goes to 0.  Return 1.
  return 1.0;
}

vec3f DirectionalLight::shadowAttenuation(const vec3f &P) const {
  // YOUR CODE HERE:
  // You should implement shadow-handling code here.
  vec3f d = getDirection(P).normalize();
  ray R(P, d);
  isect i;

  vec3f atten = getColor(P);
  while (scene->intersect(R, i)) {
    R = ray(R.at(i.t), d);
    atten = prod(atten, i.getMaterial().kt);
  }
  
  return atten;
    //return vec3f(1, 1, 1);
}

vec3f DirectionalLight::getColor(const vec3f &P) const {
  // Color doesn't depend on P
  return color;
}
vec3f DirectionalLight::getDirection(const vec3f &P) const {
  return -orientation;
}
extern TraceUI *traceUI;
double PointLight::distanceAttenuation(const vec3f &P) const {
  // YOUR CODE HERE

  // You'll need to modify this method to attenuate the intensity
  // of the light based on the distance between the source and the
  // point P.  For now, I assume no attenuation and just return 1.0

  double constant_atten_coeff = traceUI->m_nConstant_att;
  double linear_atten_coeff = traceUI->m_nLinear_att;
  double quad_atten_coeff = traceUI->m_nQuad_att;

  double distance = (position - P).length();
  double coeff = (constant_atten_coeff + linear_atten_coeff * distance +
                  quad_atten_coeff * distance * distance);

  return min(1.0, 1.0 / coeff);
}

vec3f PointLight::getColor(const vec3f &P) const {
  // Color doesn't depend on P
  return color;
}

vec3f PointLight::getDirection(const vec3f &P) const {
  return (position - P).normalize();
}

vec3f PointLight::shadowAttenuation(const vec3f &P) const {
  // YOUR CODE HERE:
  // You should implement shadow-handling code here.
  return vec3f(1, 1, 1);
}
