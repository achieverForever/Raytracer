#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <glm/glm.hpp>

typedef glm::mat4 mat4;

// Forward declarations
struct Ray;
struct HitRecord;

class Surface{
public:
	mat4 *transf;
	mat4 *invTransf;

	Surface();
	// Not yet implemented
	virtual bool Hit(const Ray &viewRay, float baseT, float lastMinT, HitRecord &rec) = 0;
};

#endif