#include <iostream>
#include <vector>
#include <string>
#include "ReadFile.h"
#include "FreeImage.h"
#include "Util.h"
#include "Triangle.h"
#include "Scene.h"
#include "Sphere.h"

using namespace std;

// Global variables

int width, height;	// Specify the image size
int maxDepth;		// Maximum number of bounces
string outFileName;

// Camera
vec3 eye, center, worldUp;
float fovy;

// Geometry
int maxVerts;
vector<Vertex> vertices;
list<Triangle> triangles;
list<Sphere> spheres;

list<Material> materials;

// Lights
list<Light> lights;

// Transform
stack<mat4> matrixStack;	
list<mat4> transforms;
mat4 currMatrix, invCurrMatrix;

Scene scene;

void InitSharedMem();
void ClearSharedMem();
int main(int argc,char* argv[]);

void InitSharedMem()
{
	currMatrix = invCurrMatrix = mat4(1.0f);
	eye = center = worldUp = vec3(0.0f);
	fovy = 0.0f;
	width = height = 0;
	maxDepth = 0;
	maxVerts = 0;
	outFileName = "testing.png";
}

int main(int argc,char* argv[])
{
	InitSharedMem();
	FreeImage_Initialise();

	string sceneFile;
	//sceneFile = "testscenes/scene3.test";
	cout << "Enter the Scene File: ";
	cin >> sceneFile;
	ReadFile(sceneFile.c_str());

	Color *image = new Color[width * height];

	// Set up the camera
	scene.camera.Width_Height(width, height);
	scene.camera.LookAt(eye, center, worldUp, fovy);

	scene.SetupLights(lights);
	scene.AddSpheres(spheres);
	scene.AddTriangles(triangles);
	
	scene.TraceScene(image, maxDepth);

	scene.camera.PrintImage(outFileName, image);

	FreeImage_DeInitialise();
	scene.Clear_ObjectsLights();	
	ClearSharedMem();
	delete[] image;

	system("pause");
	return 0;
}

void ClearSharedMem()
{
	vertices.clear();
	triangles.clear();
	spheres.clear();
	materials.clear();
	lights.clear();
	transforms.clear();
}

