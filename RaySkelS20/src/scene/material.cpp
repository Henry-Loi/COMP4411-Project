#include "material.h"
#include "light.h"
#include "ray.h"

#include "../ui/TraceUI.h"
extern TraceUI *traceUI;
// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade(Scene *scene, const ray &r, const isect &i) const {
  // YOUR CODE HERE

  // For now, this method just returns the diffuse color of the object.
  // This gives a single matte color for every distinct surface in the
  // scene, and that's it.  Simple, but enough to get you started.
  // (It's also inconsistent with the phong model...)

  // Your mission is to fill in this method with the rest of the phong
  // shading model, including the contributions of all the light sources.
  // You will need to call both distanceAttenuation() and shadowAttenuation()
  // somewhere in your code in order to compute shadows and light falloff.

  vec3f I =
      ke + prod(ka, vec3f(traceUI->m_nAmbientLight, traceUI->m_nAmbientLight,
                          traceUI->m_nAmbientLight));
  vec3f P = r.at(i.t);
  vec3f N = i.N;
  vec3f V = -r.getDirection();
  for (list<Light *>::const_iterator light = scene->beginLights();
       light != scene->endLights(); light++) {
    auto l = *light;
    vec3f L = l->getDirection(P);
    vec3f diffuse = kd * max(0.0, L.dot(N));
    vec3f R = 2 * (L.dot(N)) * N - L;
    vec3f specular = ks * pow(max(0.0, R.dot(V)), shininess * 128.0);
    I += prod(prod(l->getColor(P), (diffuse + specular)) *
                  l->distanceAttenuation(P),
              l->shadowAttenuation(P));
  }

  return I.clamp();
}
