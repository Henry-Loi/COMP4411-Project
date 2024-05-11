// SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec.h"
#include <map>
#include <vector>

class Particle;

class Force {
public:
  Vec3d force;
  Force(Vec3d input) : force(input) {}
  ~Force() { cout << "force deleted" << endl; }
  virtual void addForce(Particle* p) {
  
  };
};
class Gravity : public Force {
public:
  Gravity(Vec3d v) : Force(Vec3d(0.0, 0.0, 0.0)), g(v) {}
  Vec3d g; // gravity
  void addForce(Particle* p); 


};

class Viscous : public Force {
public:
  Viscous(double m) : K(m), Force(Vec3d(0.0, 0.0, 0.0)) {}
  double K; // k of the force
  void addForce(Particle* p); 

};

class Particle {
public:
	Particle(Vec3d p, double m) : position(p), mass(m) { p_forces.clear(); }

  inline void setPos(Vec3d p) { position = p; }
  inline void setSpeed(Vec3d s) { speed = s; }
  inline void setNetForce(Vec3d f) { netForce = f; }
  inline Vec3d getPos() const { return position; }
  inline Vec3d getSpeed() const { return speed; }
  inline Vec3d getNetForce() const { return netForce; }
  inline double getMass() const { return mass; }

  void add_force(Force *f);
  void nextPos(float deltaT);
  void draw();

private:
  double mass;
  Vec3d position;
  Vec3d speed;
  Vec3d netForce;
  std::vector<Force *> p_forces;
};

#endif