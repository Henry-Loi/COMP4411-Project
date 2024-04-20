#include "particle_system.h"

double drand(double v1, double v2) {
  return (double)rand() / RAND_MAX * (v2 - v1) + v1;
}

int irand(int v1, int v2) { return rand() % (v2 - v1) + v1; }

ParticleSystem::ParticleSystem(Scene *scene, Material *mat)
    : MaterialSceneObject(scene, mat) {
  origin = {0, 0, 0};
  u = {1, 0, 0};
  v = {0, 1, 0};
}

Particle ParticleSystem::generateRandomParticle() {
  vec3f position = origin + drand(-0.1, 0.1) * u + drand(-0.1, 0.1) * v;
  vec3f velocity =
      (dir + drand(-2, 2) * u + drand(-2, 2) * v) * drand(speedMin, speedMax);
  double decay = drand(lifeMin, lifeMax);
  vec3f color;
  Particle particle = {position, velocity, color, decay};
  return particle;
}

void ParticleSystem::init() {
  particles.clear();
  for (int i = 0; i < num_of_particle; i++) {
    Particle particle = generateRandomParticle();
    float t = irand(0, numEmit);
    particle.position += particle.velocity * t + dir * force * t * t / 2;
    if (particle.position.length() > vec3f(1.3, 1.3, 0).length()) {
      particle.color =
          vec3f((endColorMax[0] - (endColorMax[0] - endColorMin[0]) *
                                      particle.position.length()),
                (endColorMax[1] - (endColorMax[1] - endColorMin[1]) *
                                      particle.position.length()),
                (endColorMax[2] - (endColorMax[2] - endColorMin[2]) *
                                      particle.position.length()));
    } else if (particle.position.length() > vec3f(0.7, 0.7, 0).length()) {
      particle.color =
          vec3f(((endColorMax[0] - (endColorMax[0] - endColorMin[0]) *
                                       particle.position.length()) +
                 (initColorMax[0] - (initColorMax[0] - initColorMin[0]) *
                                        particle.position.length())) /
                    2,
                ((endColorMax[1] - (endColorMax[1] - endColorMin[1]) *
                                       particle.position.length()) +
                 (initColorMax[1] - (initColorMax[1] - initColorMin[1]) *
                                        particle.position.length())) /
                    2,
                ((endColorMax[2] - (endColorMax[2] - endColorMin[2]) *
                                       particle.position.length()) +
                 (initColorMax[2] - (initColorMax[2] - initColorMin[2]) *
                                        particle.position.length())) /
                    2);
    } else {
      particle.color =
          vec3f((initColorMax[0] - (initColorMax[0] - initColorMin[0]) *
                                       particle.position.length()),
                (initColorMax[1] - (initColorMax[1] - initColorMin[1]) *
                                       particle.position.length()),
                (initColorMax[2] - (initColorMax[2] - initColorMin[2]) *
                                       particle.position.length()));
    }
    particles.push_back(particle);
  }
}

bool ParticleSystem::intersectLocal(const ray &r, isect &i) const {
  vec3f color(0, 0, 0);
  bool isIntersect = false;
  double minT = 999999;
  vec3f p = r.getPosition();
  vec3f d = r.getDirection();
  for (auto &&particle : particles) {
    vec3f q = particle.position;
    if (intersectParticle(q, p, d)) {
      // if((r.at(t)-q).length() - 5< RAY_EPSILON){
      // if(abs(abs(dd.dot(d))-1)<RAY_EPSILON){
      isIntersect = true;
      double t = (q - p).length();
      minT = min(minT, t);
      for (int j = 0; j < 3; j++)
        color[j] = max(color[j], particle.color[j]);
    }
  }
  if (isIntersect) {
    Material *m = new Material;
    m->ke = color;
    i.t = minT;
    i.N = -d;
    i.material = m;
  }
  return isIntersect;
}
