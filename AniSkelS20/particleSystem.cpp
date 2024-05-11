#pragma warning(disable : 4786)

#include "particleSystem.h"

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(double gravity, double viscous) {
  // TODO
  srand(time(0));
  // curT = 0;
  forces.push_back(new Gravity(Vec3d(0.0, -gravity, 0.0)));
  forces.push_back(new Viscous(viscous));
}

/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() {
  // TODO
  particles.clear();
  forces.clear();
}

/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t) {

  // TODO

  // These values are used by the UI ...
  // -ve bake_end_time indicates that simulation
  // is still progressing, and allows the
  // indicator window above the time slider
  // to correctly show the "baked" region
  // in grey.
  bake_end_time = -1;
  simulate = true;
  dirty = true;
}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t) {

  // TODO

  // These values are used by the UI
  simulate = false;
  dirty = true;
}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t) {

  // TODO

  // These values are used by the UI
  simulate = false;
  dirty = true;
}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t) {

  // TODO
  float deltaT = t - curT;
  curT = t;
  if (isSimulate()) {
    if (!isBakedAt(t)) {
      for (int i = 0; i < particles.size(); ++i) {
        particles[i]->nextPos(deltaT);
      }
      bakeParticles(t);
      printf("not baked\n");
    } else {
      particles = bakeInfo[t];
      printf("baked\n");
    }
  } else {
    printf("not simulate\n");
  }
}

/** Render particles */
void ParticleSystem::drawParticles(float t) {
  // TODO
  if (isSimulate()) {
    for (int i = 0; i < particles.size(); ++i) {
      particles[i]->draw();
    }
  }
}

/** Adds the current configuration of particles to
 * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) {

  // TODO
  bakeInfo.insert(std::pair<float, std::vector<Particle *>>(t, particles));
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked() {

  // TODO
  bakeInfo.clear();
}

void ParticleSystem::createParticles(Vec3d pos, int num) {
  if (simulate) {
    for (int i = 0; i < num; ++i) {
      double mass = rand() % 10 + 0.2;
      Particle p = Particle(pos, mass);
      double F = rand() % 10 / 10.0 + 0.2;
      double theta = rand() % 360 / 57.3;

      double zSpeed = -(rand() % 10 / 10.0 + 5);

      double ySpeed = 0;
      double xSpeed = -(rand() % 10 / 10.0) + 0.5;
      p.setSpeed(Vec3d(xSpeed, ySpeed, zSpeed));
      for (std::vector<Force *>::iterator it = forces.begin();
           it != forces.end(); it++) {
        p.add_force(*it);
      }
      particles.push_back(&p);
    }
  }
}

bool ParticleSystem::isBakedAt(float t) {
  map<float, std::vector<Particle *>>::iterator it = bakeInfo.find(t);
  return (it != bakeInfo.end());
}
