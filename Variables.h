#include "Util.h"
#include <list>
#include <string>

using std::list;
using std::string;

class Triangle;
class Sphere;

// Declarations of shared variables

extern int width, height;	// Specify the image size
extern int maxDepth;		// Maximum number of bounces
extern string outFileName;

// Camera
extern vec3 eye, center, worldUp;
extern float fovy;

// Geometry
extern int maxVerts;
extern vector<Vertex> vertices;
extern list<Triangle> triangles;
extern list<Sphere> spheres;

extern list<Material> materials;

// Lights
extern list<Light> lights;

// Transform
extern stack<mat4> matrixStack;	
extern list<mat4> transforms;
extern mat4 currMatrix;
extern mat4 invCurrMatrix;
