#include "material.h"
#include "light.h"
#include "ray.h"

#include "../ui/TraceUI.h"
#include <cmath>
#include <iostream>
#include <math.h>

extern TraceUI *traceUI;

double WarnModel(SpotLight *l, const vec3f &P, const int specular_exp) {

  if ((acos(l->getDirection(P).normalize().dot(
           l->getSpotDirection().normalize())) /
       3.142) *
          180.0 >=
      l->getCutoffAngle()[0]) {
    return -1.0;
  } // larger than cutoff angle(Warn Model)
  else
    return pow(
        l->getDirection(P).normalize().dot(l->getSpotDirection().normalize()),
        specular_exp);
}
// Apply the phong model to this point on the surface of the object, returning
// the color of that point.

////textureTrigger = 1 
//texture
////textureTrigger = 2 
// normal
////textureTrigger = 3 
//texture & normal

vec3f Material::shade(Scene* scene, const ray& r, const isect& i, int textureTrigger,vec3f textDiffuse, vec3f bumpNormal) const {
  // YOUR CODE HERE

  // For now, this method just returns the diffuse color of the object.
  // This gives a single matte color for every distinct surface in the
  // scene, and that's it.  Simple, but enough to get you started.
  // (It's also inconsistent with the phong model...)

  // Your mission is to fill in this method with the rest of the phong
  // shading model, including the contributions of all the light sources.
  // You will need to call both distanceAttenuation() and shadowAttenuation()
  // somewhere in your code in order to compute shadows and light falloff.

  vec3f I = ke;
  vec3f P = r.at(i.t);
  vec3f N = i.N.normalize();
  //Bump
  if (textureTrigger > 1) {
      N = bumpNormal;
  }
  vec3f V = (r.getDirection()).normalize();
  bool default_amb = true; // if no ambient light, use the one from the Trace UI
  for (list<Light *>::const_iterator light = scene->beginLights();
       light != scene->endLights(); light++) {
    auto l = *light;
    auto u = *light;
    if (dynamic_cast<AmbientLight *>(u)) {
      I += prod(ka, l->getColor(P) * traceUI->m_nAmbientLightIntensity);
      default_amb = false;
      continue;
    }
    auto v = *light;
    vec3f m_diffuse = vec3f(0.0, 0.0, 0.0);
    //Texture Mapping
    if (textureTrigger==0 ||textureTrigger ==2)
        m_diffuse = kd;
    else
        m_diffuse = textDiffuse;
    vec3f L = l->getDirection(P).normalize();
    vec3f diffuse = m_diffuse * max(0.0, N.dot(L));
    vec3f R = -(2 * (N.dot(L)) * N - L);
    double verify = R.dot(V);

    vec3f specular = ks * pow(max(0.0, R.dot(V)), shininess * 128.0);

    // apply warn model
    double warn = 1.0;
    if (dynamic_cast<SpotLight*>(u)) {

        SpotLight* spotL = dynamic_cast<SpotLight*>(u);
        warn = WarnModel(spotL, P, traceUI->m_nWarnExponent);
        if (warn < 0)
            continue;
    }

    I += prod(prod(l->getColor(P),
                   l->distanceAttenuation(P) * l->shadowAttenuation(P)),
              (diffuse + specular)) *
         warn;
    if (default_amb)
      I += prod(ka,
                scene->ambientLightColor * traceUI->m_nAmbientLightIntensity);
    //// std::cout << "  L = " << L << "   V =" << V << " N = "<<N <<" R = "<<R
    ///<< / std::endl;
    // std::cout << "DIstance atten: " << l->distanceAttenuation(P) <<
    // std::endl;
  }

  return I.clamp();
}
