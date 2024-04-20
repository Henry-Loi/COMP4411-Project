#include <cmath>

#include "Hyperbolic.h"
#include <iostream>

bool Hyperbolic::intersectLocal(const ray& r, isect& i) const
{
	vec3f P = r.getPosition();
	vec3f d = r.getDirection();
	//Hyperboloid formula => z^2-x^2-y^2 = 1
	double a = d[2] * d[2] - d[1] * d[1] - d[0] * d[0];
	double b = 2 * (P[2] * d[2] - P[1] * d[1] - P[0] * d[0]);
	double c = P[2] * P[2] - P[1] * P[1] - P[0] * P[0]-1;
	double discriminant = b * b - 4.0 * a * c;
	if (discriminant < RAY_EPSILON)//Ray_epsilon
		return false;



	discriminant = sqrt(discriminant);

	double t1 = (-b - discriminant) / (2.0 * a);
	double t2 = (-b + discriminant) / (2.0 * a);

	if (t2 < RAY_EPSILON) {
		return false;
	}

	if (t1 > RAY_EPSILON) {
		// Two intersections.
		vec3f P_n = r.at(t1);
		double z = P_n[2];
		double y = P_n[1];
		double x = P_n[0];
		if (z >= -3.0 && z <= 3.0&&((y<0.2 &&y>-0.2&&z>0.0)|| (x<0.2 && x>-0.2&& z<0.0))) {
			// It's okay.
			i.obj = this;
			i.t = t1;
			i.N = vec3f(-2*P_n[0],-2*P_n[1],2*P[2]).normalize();

			return true;
		}
	}
	vec3f P_n = r.at(t2);
	double z = P_n[2];
	double y = P_n[1];
	double x = P_n[0];
	if (z >= -3.0 && z <= 3.0 && ((y<0.2 && y>-0.2 && z > 0.0) || (x<0.2 && x>-0.2 && z < 0.0))) {
		i.obj = this;
		i.t = t2;
		i.N = vec3f(-2 * P_n[0], -2 * P_n[1], 2 * P[2]).normalize();
		
		return true;
	}


	return false;
}

