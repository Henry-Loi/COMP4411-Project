 // SAMPLE_SOLUTION
#include "particle.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <FL/glut.h>
#include <GL/glu.h>
#include <cstdio>
#include <math.h>

void Particle::add_force(Force* f) {

	//if (Gravity* g = dynamic_cast<Gravity*>(f)) {
	//	g->force = prod((g->g),Vec3d(0.0, mass, 0.0));
	//}
	forces.push_back(f);


}

void Particle::nextPos(float deltaT) {
	netForce = Vec3d(0.0, 0.0, 0.0);
	for (std::vector<Force*>::iterator it = forces.begin(); it != forces.end(); it++)
	{	
		//if (Viscous* g = dynamic_cast<Viscous*>(*it)) {
		//	g->force = -(g->K)*speed;
		//}
		netForce += (*it)->force;
	}
	speed += netForce / mass * deltaT;
	position += speed * deltaT;
}

void Particle::draw() {
	setDiffuseColor(0, 0, 1);
	glPushMatrix();
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3f(position[0], position[1], position[2]);
	glEnd();
	glPopMatrix();
	glPointSize(1);
}