#include "Scene.h"
#include <iostream>

using std::cout;

Scene::Scene() : camera() { }

Scene::~Scene() { }

void Scene::TraceScene(Color* image, int maxDepth) 
{
	cout << "Tracing Scene ... \n";

	Ray viewRay;
	Sample samp;
	float lastMinT;
	HitRecord rec;
	Color color;
	int count = 0;

	lastMinT = kRayMaxT;
	int width = camera.GetWidth(), height = camera.GetHeight();

	for(int i = 0; i < height; i++)		// row
	{
		for(int j = 0; j < width; j++)	// column
		{
			count++;
			// For each sample point, generate viewing ray
			samp = Sample(i, j);	// ith row, jth column
			camera.GetViewRay(samp, viewRay);
			
			// Trace this viewRay
			color = rayTracer.Trace(viewRay, objects, lights, maxDepth+1);
			image[i*width+j] = color;		// Note that our image is generated left to right
		}
		cout << count << " / " << width*height << " Completed\n";
	}
}

void Scene::AddObjects(Surface *obj)
{
	objects.push_back(obj);
	cout << "Added an Object to Scene\n";
}

void Scene::AddTriangles(list<Triangle> &triangles)
{
	for (list<Triangle>::iterator iter = triangles.begin();
		 iter != triangles.end();
		 iter++)
	{
		Surface *surf = (Surface *)&(*iter);
		objects.push_back(surf);
	}
	cout << "Added a bunch of Triangles to Scene\n";
}

void Scene::AddSpheres(list<Sphere> &spheres)
{
	for (list<Sphere>::iterator iter = spheres.begin();
		iter != spheres.end();
		iter++)
	{
		Surface *surf = (Surface *)&(*iter);
		objects.push_back(surf);
	}
	cout << "Added a bunch of Spheres to Scene\n";
}

void Scene::AddLight(Light &light)
{
	lights.push_back(light);
	cout << "Added a Light to Scene\n";
}

void Scene::SetupLights(const list<Light> &lights)
{
	this->lights.clear();
	this->lights = lights;
	cout << "Added Lights to Scene\n";
}

void Scene::Clear_ObjectsLights()
{
	objects.clear();
	lights.clear();
	cout << "Clean up all Objects and Lights\n";
}