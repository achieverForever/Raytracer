#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Surface.h"
#include <glm/glm.hpp>
#include "Util.h"

class Sphere : public Surface {

public:
	vec3 center;
	float radius;
	Material material;

public:
	Sphere();
	Sphere(const vec3 &centr, float rad, const Material &matrl, mat4 *transf=NULL, mat4 *invTransf=NULL);
	virtual ~Sphere();
	bool Hit(const Ray &viewRay, float baseT, float maxT, HitRecord &rec);
};

#endif