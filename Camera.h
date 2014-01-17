#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Util.h"
#include "FreeImage.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class Camera{

private:
	vec3 eye;
	vec3 forward, right, up;
	int width, height;
	float nearPlane;
	float fovy;
	mat4 viewMatrix;

	float tanFovYBy2, tanFovXBy2;	// Precompute these 2 terms
	float invWidthBy2, invHeightBy2;

public:
	Camera();
	int GetWidth();
	int GetHeight();
	void LookAt(const vec3 &lookfrom, const vec3 &lookat, const vec3 &worldUp, float fovy);
	void Width_Height(int w, int h);
	void GetViewRay(const Sample &samp, Ray &ray);
	const mat4 & GetViewMatrix();
	void PrintImage(string fileName, Color *pColors);
};

#endif