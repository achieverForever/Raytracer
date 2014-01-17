#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Surface.h"
#include "Util.h"

class Triangle : public Surface {

public:
	Vertex vert0, vert1, vert2;
	vec3 normal;
	Material *material;

public:
	Triangle(int i0, int i1, int i2, Material *matrl, vector<Vertex> *vertices, mat4 *transf=NULL, mat4 *invTransf=NULL);
	~Triangle();
	bool Hit(const Ray &viewRay, float baseT, float lastMinT, HitRecord &rec);

private:
	Triangle();
};


#endif

