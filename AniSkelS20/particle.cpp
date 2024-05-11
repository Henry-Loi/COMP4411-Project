// SAMPLE_SOLUTION
#include "particle.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <FL/glut.h>
#include <GL/glu.h>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <stdio.h>

void Particle::add_force(Force *f) {
    Vec3d temp;
    temp = f->force;
    if (Gravity* g = dynamic_cast<Gravity*>(f)) {
        temp = prod((g->g), Vec3d(0.0, mass, 0.0));
    }

        
   Force* force = new Force(temp);
  p_forces.push_back(force);
  printf("Added force %f %f %f\n", force->force[0], force->force[1], force->force[2]);
  printf("Force %f %f %f\n", p_forces[0]->force[0], p_forces[0]->force[1],
         p_forces[0]->force[2]);
  std::cout << "Size: " << p_forces.size() << std::endl;
}

void Particle::nextPos(float deltaT) {
  //   netForce = Vec3d(0.0, 0.0, 0.0);

  if (p_forces.size() == 0) {
    printf("No forces\n");
    return;
  }
  std::cout << "Size: " << p_forces.size() << std::endl;
  if(p_forces.size()>0)
    for (Force* force : p_forces)
      if (force != nullptr)
          netForce += force->force;
  //for (std::vector<Force *>::iterator it = forces.begin(); it != forces.end();
  //     it++) {
  //  printf("Force %f %f %f\n", (*it)->force[0], (*it)->force[1],
  //         (*it)->force[2]);
  //  if (*it != nullptr) {
  //    (*it)->addForce(this);
  //  }
  //}
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

void Gravity::addForce(Particle *p) {
  p->setNetForce(p->getNetForce() + g * p->getMass());
  printf("Gravity %f %f %f\n", p->getNetForce()[0], p->getNetForce()[1],
         p->getNetForce()[2]);
}

void Viscous::addForce(Particle *p) {
  p->setNetForce(p->getNetForce() - K * p->getSpeed());
  printf("Viscous %f %f %f\n", p->getNetForce()[0], p->getNetForce()[1],
         p->getNetForce()[2]);
}
