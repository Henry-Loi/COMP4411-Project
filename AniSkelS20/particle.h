 // SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec.h"
#include <vector>
#include <map>

class Force {
public:
	Vec3d force;
	Force(Vec3d input):force(input) {
	}
	virtual ~Force() {}
};
class Gravity : public Force {
public:
	Gravity(Vec3d v) : Force(Vec3d(0.0,0.0,0.0)),g(v) {}
	Vec3d g;   //gravity
};

class Viscous : public Force {
public:
	Viscous(double m) : K(m), Force(Vec3d(0.0, 0.0, 0.0)) {}
	double K;  // k of the force
};

class Particle {
public:
	Particle(Vec3d p, double m) : position(p), mass(m) {}

	inline void setPos(Vec3d p) { position = p; }
	inline void setSpeed(Vec3d s) { speed = s; }
	inline void setNetForce(Vec3d f) { netForce = f; }
	inline Vec3d getPos() const { return position; }
	inline Vec3d getSpeed() const { return speed; }
	inline Vec3d getNetForce() const { return netForce; }
	inline double getMass() const { return mass; }

	void add_force(Force* f);
	void nextPos(float deltaT);
	void draw();

private:
	double mass;
	Vec3d position;
	Vec3d speed;
	Vec3d netForce;
	std::vector<Force*> forces;
};


#endif