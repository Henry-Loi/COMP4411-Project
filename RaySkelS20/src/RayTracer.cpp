// The main ray tracer.

#include <Fl/fl_ask.h>
#include <stdlib.h>

#include "RayTracer.h"
#include "fileio/parse.h"
#include "fileio/read.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "ui/TraceUI.h"
extern TraceUI *traceUI;
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of
// 0.
vec3f RayTracer::trace(Scene *scene, double x, double y) {

  // antialiasing by supersampling and averaging the color
  if (traceUI->m_nSubsamplePixelSize > 1) {
    int n_subpixels = traceUI->m_nSubsamplePixelSize;
    double subpixel_height = 1.0 / (buffer_height * n_subpixels);
    double subpixel_width = 1.0 / (buffer_width * n_subpixels);
    double start_x = x - subpixel_width * (float(n_subpixels - 1) / 2);
    double start_y = y + subpixel_height * (float(n_subpixels - 1) / 2);
    vec3f result(0, 0, 0);
    double jitter_x = 0;
    double jitter_y = 0;

    for (int y = 0; y < n_subpixels; y++) {
      // jitter the subpixel
      if (traceUI->m_nSubsampleJitter) {
        jitter_x = static_cast<float>(rand()) / RAND_MAX - subpixel_width / 2;
        jitter_x = min(jitter_x, subpixel_width / 2);
        jitter_y = static_cast<float>(rand()) / RAND_MAX - subpixel_height / 2;
        jitter_y = min(jitter_y, subpixel_height / 2);
      }

      for (int x = 0; x < n_subpixels; x++) {
        double new_y = start_y - y * subpixel_height + jitter_y;
        double new_x = start_x + x * subpixel_width + jitter_x;

        ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
        scene->getCamera()->rayThrough(new_x, new_y, r);
        result += traceRay(scene, r, vec3f(1.0, 1.0, 1.0), traceUI->getDepth())
                      .clamp();
      }
    }
    result /= (n_subpixels * n_subpixels);
    return result;
  }

  ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
  scene->getCamera()->rayThrough(x, y, r);
  return traceRay(scene, r, vec3f(1.0, 1.0, 1.0), traceUI->getDepth()).clamp();
}

vec3f RayTracer::reflectionDirection(const ray &r, const isect &i) {
  // equation: R = D - 2(D dot N)N
  vec3f R = r.getDirection() - 2 * (r.getDirection().dot(i.N) * i.N);
  return R.normalize();
}

vec3f RayTracer::refractionDirection(const ray &r, const isect &i, double n,
                                     bool &internal_refraction) {
  // equation: T = nD - (n(D dot N) + sqrt(1 - n^2(1 - (D dot N)^2)))N
  double cos_I = r.getDirection().dot(i.N);
  double sin_theta = n * n * (1.0 - cos_I * cos_I);
  if (sin_theta > 1.0) {
    internal_refraction = true;
  }
  double cos_theta2 = sqrt(1.0 - sin_theta);
  double cos_theta = sqrt(1.0 - sin_theta);
  vec3f refraction = n * r.getDirection() + (n * cos_I - cos_theta) * i.N;

  return refraction.normalize();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay(Scene *scene, const ray &r, const vec3f &thresh,
                          int depth) {
  isect i;

  // if (depth > traceUI->getDepth()) {
  //   return vec3f(0.0, 0.0, 0.0);
  // }

  if (scene->intersect(r, i)) {
    // YOUR CODE HERE

    // An intersection occured!  We've got work to do.  For now,
    // this code gets the material for the surface that was intersected,
    // and asks that material to provide a color for the ray.

    // This is a great place to insert code for recursive ray tracing.
    // Instead of just returning the result of shade(), add some
    // more steps: add in the contributions from reflected and refracted
    // rays.

    const Material &m = i.getMaterial();
    vec3f I = m.shade(scene, r, i);

    // adaptive termination
    if (I.length() < traceUI->m_nAdaptiveThresh) {
      return I;
    }

    // reflection
    if (depth > 0) {

      vec3f R = reflectionDirection(r, i);
      ray reflect_ray = ray(r.at(i.t), R);
      vec3f I_r = prod(traceRay(scene, reflect_ray, thresh, depth - 1), m.kr);

      // if there are not intersect object with the reflection ray
      if (!scene->intersect(reflect_ray, i)) {
        I_r = getBackground(scene, reflect_ray);
      }

      I = I + I_r;

      // if there are no refraction return the phong model + reflect color
      if (!m.kt.iszero()) {

        // refraction
        bool internal_refraction = false;

        double n = 0.0;
        double refract_index = 1.0;

        if (i.N * r.getDirection() < -RAY_EPSILON) {
          n = refract_index / m.index;
        } else {
          n = m.index / refract_index;
          i.N = -i.N; // reverse the normal
        }

        vec3f T = refractionDirection(r, i, m.index, internal_refraction);

        if (!internal_refraction) {
          ray r_refract = ray(r.at(i.t), T);
          vec3f I_t;

          if (!scene->intersect(r_refract, i)) {
            I_t = getBackground(scene, r_refract);
          } else {
            I_t = prod(traceRay(scene, r_refract, thresh, depth - 1), m.kt);
          }

          I = I - prod(I, m.kt) + I_t;
        }
      }
    }

    return I;

  } else {
    // No intersection.  This ray travels to infinity, so we color
    // it according to the background color, which in this (simple) case
    // is just black.

    if (background && traceUI->m_nEnableBackground) {
      return getBackground(scene, r);
    }

    return vec3f(0.0, 0.0, 0.0);
  }
}

RayTracer::RayTracer() {
  buffer = NULL;
  buffer_width = buffer_height = 256;
  scene = NULL;

  m_bSceneLoaded = false;
}

RayTracer::~RayTracer() {
  delete[] buffer;
  delete scene;
}

void RayTracer::getBuffer(unsigned char *&buf, int &w, int &h) {
  buf = buffer;
  w = buffer_width;
  h = buffer_height;
}

double RayTracer::aspectRatio() {
  return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded() { return m_bSceneLoaded; }

bool RayTracer::loadScene(char *fn) {
  try {
    scene = readScene(fn);
  } catch (ParseError pe) {
    fl_alert("ParseError: %s\n", pe);
    return false;
  }

  if (!scene)
    return false;

  buffer_width = 256;
  buffer_height =
      (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

  bufferSize = buffer_width * buffer_height * 3;
  buffer = new unsigned char[bufferSize];

  // separate objects into bounded and unbounded
  scene->initScene();

  // Add any specialized scene loading code here

  m_bSceneLoaded = true;

  return true;
}

bool RayTracer::loadHeightMap(char *iname) {
  try {
    scene = loadHeightField(iname);
  } catch (ParseError pe) {
    fl_alert("ParseError: %s\n", pe);
    return false;
  }

  if (!scene)
    return false;

  buffer_width = 256;
  buffer_height =
      (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

  bufferSize = buffer_width * buffer_height * 3;
  buffer = new unsigned char[bufferSize];

  // separate objects into bounded and unbounded
  scene->initScene();

  // Add any specialized scene loading code here

  m_bSceneLoaded = true;

  return true;
}

void RayTracer::traceSetup(int w, int h) {
  if (buffer_width != w || buffer_height != h) {
    buffer_width = w;
    buffer_height = h;

    bufferSize = buffer_width * buffer_height * 3;
    delete[] buffer;
    buffer = new unsigned char[bufferSize];
  }
  memset(buffer, 0, w * h * 3);
}

void RayTracer::traceLines(int start, int stop) {
  vec3f col;
  if (!scene)
    return;

  if (stop > buffer_height)
    stop = buffer_height;

  for (int j = start; j < stop; ++j)
    for (int i = 0; i < buffer_width; ++i)
      tracePixel(i, j);
}

void RayTracer::tracePixel(int i, int j) {
  vec3f col;

  if (!scene)
    return;

  double x = double(i) / double(buffer_width);
  double y = double(j) / double(buffer_height);

  col = trace(scene, x, y);

  unsigned char *pixel = buffer + (i + j * buffer_width) * 3;

  pixel[0] = (int)(255.0 * col[0]);
  pixel[1] = (int)(255.0 * col[1]);
  pixel[2] = (int)(255.0 * col[2]);
}

#include "fileio/bitmap.h"
int RayTracer::loadBackground(char *iname) {
  // try to open the image to read
  unsigned char *data;
  int width, height;

  if ((data = readBMP(iname, width, height)) == NULL) {
    fl_alert("Can't load bitmap file");
    return 0;
  }

  // reflect the fact of loading the new image
  bg_width = width;
  bg_height = height;

  background = data;
}

vec3f RayTracer::getBackground(Scene *scene, const ray &r) {
  vec3f dir = r.getDirection();

  double x = dir * scene->getCamera()->u;
  double y = dir * scene->getCamera()->v;
  double z = dir * scene->getCamera()->look;

  x = x / z + 0.5;
  y = y / z + 0.5;

  int xGrid = int(x * bg_width);
  int yGrid = int(y * bg_height);

  if (xGrid < 0 || xGrid >= bg_width || yGrid < 0 || yGrid >= bg_height) {
    return vec3f(0, 0, 0);
  }

  return vec3f(background[(yGrid * bg_width + xGrid) * 3] / 255.0,
               background[(yGrid * bg_width + xGrid) * 3 + 1] / 255.0,
               background[(yGrid * bg_width + xGrid) * 3 + 2] / 255.0);
}
