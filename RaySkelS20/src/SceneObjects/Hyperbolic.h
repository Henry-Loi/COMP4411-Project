#pragma once
#ifndef __HYPERBOLIC_H__
#define __HYPERBOLIC_H__

#include "../scene/scene.h"
# include <iostream>
class Hyperbolic
	: public MaterialSceneObject
{
public:
	Hyperbolic(Scene* scene, Material* mat)
		: MaterialSceneObject(scene, mat)
	{
		
		std::cout << "Hyperbolic Constructed" << std::endl;
		std::cout << material->kd << std::endl;
	}

	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return true; }

	virtual BoundingBox ComputeLocalBoundingBox()
	{
		BoundingBox localbounds;
		localbounds.min = vec3f(-1.0f, -1.0f, -1.0f);
		localbounds.max = vec3f(1.0f, 1.0f, 1.0f);
		return localbounds;
	}
};


#endif // __SPHERE_H__