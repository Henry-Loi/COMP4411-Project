#include "material.h"
#include "light.h"
#include "ray.h"

#include "../ui/TraceUI.h"
#include <iostream>
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

  vec3f I = ke + prod(ka, scene->ambientLightColor *
                              traceUI->m_nAmbientLightIntensity);
  vec3f P = r.at(i.t);
  vec3f N = i.N.normalize();
  vec3f V = (r.getDirection()).normalize();

  for (list<Light *>::const_iterator light = scene->beginLights();
       light != scene->endLights(); light++) {
    auto l = *light;
    vec3f L = ((l->getDirection(P))).normalize();
    vec3f diffuse = kd * max(0.0, N.dot(L));
    vec3f R = -(2 * (N.dot(L)) * N - L);
    double verify = R.dot(V);
    vec3f specular = ks * pow(max(0.0, R.dot(V)), shininess * 128.0);

    I += prod(prod(l->getColor(P),
                   l->distanceAttenuation(P) * l->shadowAttenuation(P)),
              (diffuse + specular));
    // std::cout << "  L = " << L << "   V =" << V << " N = "<<N <<" R = "<<R <<
    // std::endl;
    std::cout << "DIstance atten: " << l->distanceAttenuation(P) << std::endl;
  }

  return I.clamp();
}
