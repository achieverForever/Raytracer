#ifndef __UTIL_H__
#define __UTIL_H__

#include <stack>
#include <vector>
#include <iostream>
#include <limits>
#include <glm\glm.hpp>
#include "Surface.h"

using namespace std;

const float PI = 3.14159265359f;
#define DEG2RAD(degrees) ((degrees) * PI / 180.0f)

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 Color;
typedef vec3 Vertex;

enum LightType{ POINT, DIRECTIONAL };

const float kRayMinT = 0.0001f;	// To avoid precision problem
const float kRayMaxT = numeric_limits<float>::max();

class Light {
	// Not consider spotlight and area light, may be extended.
public:
	LightType type;
	vec3 position;
	vec3 color;
	vec3 attenuation;
	vec3 lightDir;		// Direction to light, to reduce computation

public:
	Light();
	Light(LightType t, float x, float y, float z, float r, float g, float b, float konst=1.0f, float linear=0.0f, float quad=0.0f);
	Light(LightType t, const vec3 &pos, const Color &col, const vec3 &atten=vec3(1.0f, 0.0f, 0.0f));
	vec3 & GetLightDirection(const vec3 &target);
};

struct Material{
	// Not consider translucent, may be extended.
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	float shininess;

	Material() : ambient(0.0f), diffuse(0.0f), specular(0.0f), emission(0.0f), shininess(0.0f) { }
	Material(const vec3 &a, const vec3 &d, const vec3 &s, const vec3 &e, float shi)
	{
		ambient = a; diffuse = d; specular = s; emission = e; shininess = shi;  
	}
};

struct Ray{

	vec3 origin;
	vec3 direction;
	float minT, maxT;

	Ray() : origin(0.0f), direction(0.0f), minT(kRayMinT), maxT(kRayMaxT) { }
	Ray(const vec3 &org, const vec3 &dir, float minTT=kRayMinT, float maxTT=kRayMaxT) : 
		origin(org), direction(dir), minT(minTT), maxT(maxTT) { }
	Ray(const Ray &ray)
	{
		origin = ray.origin;
		direction = ray.direction;
		minT = ray.minT;
		maxT = ray.maxT;
	}
	vec3 GetIntersectPoint(float t) const
	{
		return (origin + direction * t);
	}
};

struct HitRecord{
	// Not consider texture coordinate, may be extended
	float t;			// The "t" parameter of the intersection point
	vec3 hitPoint;
	Material *material;
	vec3 normal;
	Surface *surf;

	HitRecord() : t(kRayMaxT), hitPoint(0.0f), material(NULL), normal(0.0f), surf(NULL) { }
};

struct BoundingBox{
	// BoundingBox structure
	vec3 min;
	vec3 max;

	BoundingBox() : min(0.0f), max(0.0f) { }
	BoundingBox(const vec3 &minn, const vec3 &maxx) : min(minn), max(maxx) { }
};

struct Sample{
	float i, j;		// i:height(row), j:width(column), Note that (0, 0) is at 
					// top-left, +I goes from top to bottom, +J goes from left to right
	Sample() : i(0.5f), j(0.5f) { }
	Sample(int ii, int jj)
	{
		i = (float)ii + 0.5f;	// Sample point is located at the center of each pixel
		j = (float)jj + 0.5f;
	}
};

void RightMultiply(const mat4 & mat, stack<mat4> & matrixStack);
void LoadMatrix(const mat4 & mat, stack<mat4> & matrixStack);
vec3 & TransformDirection(vec3 & direction, const mat4 & matrix);
vec3 & TransformNormal(vec3 & normal, const mat4 & matrix);
vec3 & TransformPosition(vec3 & position, const mat4 & matrix);
void TransformRay(Ray &ray, const mat4 &matrix);
void Clamp01(Color &c);
ostream & operator<< (ostream &stream, const vec3 &v);

#endif

