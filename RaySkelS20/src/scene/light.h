#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;

protected:
	Light(Scene* scene, const vec3f& col, const vec3f& distAttenConst)
		: SceneElement(scene), color(col), distAttenConst(distAttenConst){
	}

	vec3f 		color;
	vec3f		distAttenConst; // (constant_attenuation_coeff,linear_attenuation_coeff,quadratic_attenuation_coeff)
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color, const vec3f& distAttenConst)
		: Light( scene, color,distAttenConst ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	PointLight( Scene *scene, const vec3f& pos, const vec3f& color, const vec3f& distAttenConst )
		: Light( scene, color, distAttenConst), position( pos ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f position;
};


class AmbientLight
	: public Light
{
public:
	AmbientLight(Scene* scene, const vec3f& color, const vec3f& distAttenConst)
		: Light(scene, color, distAttenConst){}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation(const vec3f& P) const;
	virtual vec3f getColor(const vec3f& P) const;
	virtual vec3f getDirection(const vec3f& P) const;

protected:
	vec3f position;
};

#endif // __LIGHT_H__
