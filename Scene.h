#ifndef __SCENE_H__
#define __SCENE_H__

#include <list>
#include <vector>
#include "Raytracer.h"
#include "Camera.h"
#include "Surface.h"
#include "Triangle.h"
#include "Sphere.h"

using std::list;
using std::vector;

struct Sample;
class Light;

class Scene {

public:
	vector<Surface*> objects;
	list<Light> lights;
	Camera camera;
	RayTracer rayTracer;

public:
	Scene();
	~Scene();
	void TraceScene(Color *image, int maxDepth);
	void AddObjects(Surface *obj);
	void AddTriangles(list<Triangle> &triangles);
	void AddSpheres(list<Sphere> &spheres);
	void AddLight(Light &light);
	void SetupLights(const list<Light> &lights);
	void Clear_ObjectsLights();
};

#endif