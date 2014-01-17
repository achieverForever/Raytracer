#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera() : eye(0.0f), forward(0.0f), right(0.0f), up(0.0f), width(0), height(0),
				   fovy(0.0f), nearPlane(1.0f), viewMatrix(1.0f) { }

void Camera::LookAt(const vec3 &lookfrom, const vec3 &lookat, const vec3 &worldUp, float fovy)
{
	eye = lookfrom;
	forward = glm::normalize((lookat - lookfrom));
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
	fovy = DEG2RAD(fovy);

	tanFovYBy2 = glm::tan(fovy * 0.5f);
	tanFovXBy2 = tanFovYBy2 * (float) width / height;
	invWidthBy2 = 1.0f / (width*0.5f);
	invHeightBy2 = 1.0f / (height*0.5f);
	nearPlane = 1.0f;

}

int Camera::GetWidth()
{
	return width;
}

int Camera::GetHeight()
{
	return height;
}

void Camera::Width_Height(int w, int h)
{
	width = w;	height = h;
}

void Camera::GetViewRay(const Sample &samp, Ray &ray)
{
	ray.origin = eye;
	ray.direction = tanFovXBy2 * (samp.j-width*0.5f) * invWidthBy2 * right
					+ tanFovYBy2 * (height*0.5f-samp.i) * invHeightBy2 * up
					+ forward;
	ray.direction = glm::normalize(ray.direction);
}

const mat4 & Camera::GetViewMatrix()
{
	vec3 u, v, w;
	u = right;
	v = up;
	w = -forward;
	viewMatrix[0] = vec4(u.x, v.x, w.x, 0.0f);
	viewMatrix[1] = vec4(u.y, v.y, w.y, 0.0f);
	viewMatrix[2] = vec4(u.z, v.z, w.z, 0.0f);
	viewMatrix[3] = vec4(
		-u.x * eye.x - u.y * eye.y - u.z * eye.z,
		-v.x * eye.x - v.y * eye.y - v.z * eye.z, 
		-w.x * eye.x - w.y * eye.y - w.z * eye.z,
		1.0f);

	return viewMatrix;
}

void Camera::PrintImage(string fileName, Color *pColors)
{
	int pixelNum = width * height;
	BYTE *pixels = new BYTE[3*pixelNum];

	// Store the pixel data to an output file
	for(int i = 0; i < pixelNum; i++)
	{
		BYTE r, g, b;
		Clamp01((*pColors));
		r = static_cast<unsigned char>(pColors->x * 255.0f);
		g = static_cast<unsigned char>(pColors->y * 255.0f);
		b = static_cast<unsigned char>(pColors->z * 255.0f);	
		pixels[i*3] = b;	// I don't know why we need to flip the rgb order
		pixels[i*3+1] = g;
		pixels[i*3+2] = r;
		pColors++;
	}

	// Pay attention to the last param!
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, height, width * 3, 24, 
												 0xFF0000, 0x00FF00, 0x0000FF, true);	
	cout << "Printing Image: " << fileName << endl;
	cout << "Done!\n";

	FreeImage_Save(FIF_PNG, img, fileName.c_str(), 0);
	delete[] pixels;
}