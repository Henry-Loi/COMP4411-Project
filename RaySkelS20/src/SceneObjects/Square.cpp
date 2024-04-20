#include <cmath>

#include "Square.h"
#include <iostream>


bool Square::intersectLocal( const ray& r, isect& i ) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if( d[2] == 0.0 ) {
		return false;
	}

	double t = -p[2]/d[2];

	if( t <= RAY_EPSILON ) {
		return false;
	}

	vec3f P = r.at( t );

	if( P[0] < -0.5 || P[0] > 0.5 ) {	
		return false;
	}

	if( P[1] < -0.5 || P[1] > 0.5 ) {	
		return false;
	}

	i.obj = this;
	i.t = t;
	if( d[2] > 0.0 ) {
		i.N = vec3f( 0.0, 0.0, -1.0 );
	} else {
		i.N = vec3f( 0.0, 0.0, 1.0 );
	}

	return true;
}

vec3f Square::MapToTexture(TextureMap* textMap, vec3f pos) const {
	return textMap->getSquareColor(transform->globalToLocalCoords(pos));
}

vec3f Square::MapToNormal(TextureMap* textMap, vec3f pos, isect& i) const {

	vec3f result(0.0, 0.0, 0.0);
	mat4f mat ;
	mat[0][3] = -0.5;
	mat[1][3] = -0.5;

	TransformNode* curTransform = transform->createChild(mat);
	result = textMap->getSquareNormal(curTransform->globalToLocalCoords(pos));
	return result.normalize();
}
