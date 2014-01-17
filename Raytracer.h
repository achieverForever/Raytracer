#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

#include <list>
#include <vector>
#include "Util.h"
#include "Surface.h"

using std::list;
using std::vector;

class RayTracer {
public:
	Color Trace(const Ray &ray, vector<Surface*> &objects, list<Light> &lights, int depth);
};

#endif