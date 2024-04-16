#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/ray.h"
#include "scene/scene.h"

class RayTracer {
public:
  RayTracer();
  ~RayTracer();

  vec3f trace(Scene *scene, double x, double y);
  vec3f traceRay(Scene *scene, const ray &r, const vec3f &thresh, int depth);

  void getBuffer(unsigned char *&buf, int &w, int &h);
  double aspectRatio();
  void traceSetup(int w, int h);
  void traceLines(int start = 0, int stop = 10000000);
  void tracePixel(int i, int j);

  bool loadScene(char *fn);

  bool sceneLoaded();

  int loadBackground(char *iname);

private:
  unsigned char *buffer;
  int buffer_width, buffer_height;
  int bufferSize;
  Scene *scene;

  unsigned char *background;
  int bg_width, bg_height;

  bool m_bSceneLoaded;

  vec3f reflectionDirection(const ray &r, const isect &i);
  vec3f refractionDirection(const ray &r, const isect &i, double n, bool &);

  vec3f getBackground(Scene *scene, const ray &r);
};

#endif // __RAYTRACER_H__
