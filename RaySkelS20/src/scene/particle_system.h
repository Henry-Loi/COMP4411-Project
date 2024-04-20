#pragma once

#include "../scene/scene.h"
#include <vector>

typedef struct {
  vec3f position;
  vec3f velocity;
  vec3f color;
  double life;
} Particle;

class ParticleSystem : public MaterialSceneObject {
public:
  ParticleSystem(Scene *scene, Material *mat);
  ~ParticleSystem(){};
  std::vector<Particle> particles;
  void set(vec3f origin, vec3f dir, double force, vec3f initColorMin,
           vec3f initColorMax, double speedMin, double speedMax, double lifeMin,
           double lifeMax, int numEmit, int num_of_particle);
  void init();

  Particle generateRandomParticle();

  bool intersectLocal(const ray &r, isect &i) const override;
  vec3f origin, dir, initColorMin, initColorMax, endColorMin, endColorMax, u, v;
  double speedMin, speedMax, lifeMin, lifeMax, force, numEmit, num_of_particle;
  // q: particle pos, p: ray pos, d: ray dir
  bool intersectParticle(vec3f &q, vec3f &p, vec3f &d) const {
    vec3f pq = (q - p).normalize();
    return (abs(abs(pq * d) - 1) < RAY_EPSILON);
  }
};
