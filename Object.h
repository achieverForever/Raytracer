#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include <list>
#include "Surface.h"
#include "Util.h"
#include "Triangle.h"

using std::vector;
using std::list;

class Object : public Surface {
private:
	BoundingBox box;
	vector<Surface*> primitives;

public:
	Object();
	Object(const list<Triangle> &tris);
	void CalcBoundingBox();
	bool Hit(const Ray &viewRay, float baseT, float lastMinT, HitRecord &rec)

};

#endif