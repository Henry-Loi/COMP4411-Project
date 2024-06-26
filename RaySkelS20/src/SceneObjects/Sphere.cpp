#include <cmath>

#include "Sphere.h"
#include <iostream>
bool Sphere::intersectLocal( const ray& r, isect& i ) const
{
	vec3f v = -r.getPosition();
	double b = v.dot(r.getDirection());
	double discriminant = b*b - v.dot(v) + 1;

	if( discriminant < 0.0 ) {
		return false;
	}

	discriminant = sqrt( discriminant );
	double t2 = b + discriminant;

	if( t2 <= RAY_EPSILON ) {
		return false;
	}

	i.obj = this;

	double t1 = b - discriminant;

	if( t1 > RAY_EPSILON ) {
		i.t = t1;
		i.N = r.at( t1 ).normalize();
	} else {
		i.t = t2;
		i.N = r.at( t2 ).normalize();
	}

	return true;
}

vec3f Sphere::MapToTexture(TextureMap* textMap, vec3f pos) const {

	return textMap->getSphereColor(transform->globalToLocalCoords(pos));
}

vec3f Sphere::MapToNormal(TextureMap* textMap, vec3f pos, isect& i) const{
	vec3f result(0.0, 0.0, 0.0);
	mat4f mat(vec4f(0.0,0.0,1.0,0.0), vec4f(0.0, 1.0, 0.0, 0.0), vec4f(1.0, 0.0, 0.0, 0.0), vec4f(0.0, 0.0, 0.0, 1.0));


	TransformNode* curTransform = transform->createChild(mat);
	result = textMap->getSphereNormal(transform->globalToLocalCoords(pos));
	return  result.normalize();
		
}

