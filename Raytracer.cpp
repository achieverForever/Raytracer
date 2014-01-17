#include "Raytracer.h"
#include "glm/glm.hpp"
#include <cstdio>

Color RayTracer::Trace(const Ray &ray, vector<Surface*> &objects, list<Light> &lights, int depth)
{
	Color color(0.0f);

	if(depth == 0)
	{
		//cout << "Recursion Ends - Depth became 0\n";
		return color;
	}

	HitRecord rec;
	bool isHitAny = false;
	float lastMinT = kRayMaxT;			// Keep track of the closest intersection
	vec3 hitPoint, L, H, V;
	vec3 shadowRayDir, reflDir;
	Ray shadowRay, reflRay;
	float shi;
	float distance, atten;
	float nDotL, nDotH;

	// For each object in the scene, check for intersection
	for(vector<Surface*>::iterator iter = objects.begin();
		iter != objects.end();
		iter++)
	{
		Surface *surf = (*iter);
		if (surf->Hit(ray, 1.0f, lastMinT, rec))
		{
			// If we get a closer hit, update lastMinT
			isHitAny = true;
			// NEW
			//lastMinT = rec.t;
			lastMinT = glm::length(ray.origin - rec.hitPoint);
		}
	}

	if(isHitAny)
	{
		// Closest intersection
		const Color &diff = rec.material->diffuse;
		const Color &spec = rec.material->specular;
		const vec3 &N = rec.normal;
		//hitPoint = ray.GetIntersectPoint(rec.t);
		// NEW
		hitPoint = rec.hitPoint;

		V = glm::normalize(ray.origin - hitPoint);
		shi = rec.material->shininess;

		color += rec.material->ambient + rec.material->emission;

		// Calculate the Reflection Ray, then trace it
		// NEW
		//reflDir = ray.direction - 2.0f * glm::dot(ray.direction, N) * N;
		reflDir = glm::reflect(ray.direction, N);
		reflRay = Ray(hitPoint, reflDir);
		color += Trace(reflRay, objects, lights, depth-1) * spec;

		// For each light in the scene, cast a Shadow Ray to check if this point
		// is in shadow, if not, accumulate diffuse and specular contributions for this light
		for (list<Light>::iterator iter2 = lights.begin();
			iter2 != lights.end();
			iter2++)
		{
			Light &light = (*iter2);

			// Cast a Shadow Ray from hitPoint to light
			// NEW
/*
			vec3 offset = N * 0.001f;
			if(light.type != DIRECTIONAL)
			{
				shadowRayDir = light.GetLightDirection(hitPoint);
				distance = glm::length((light.position - hitPoint));
				shadowRay = Ray(hitPoint+offset, shadowRayDir, kRayMinT, distance);
			}
			else
			{
				shadowRayDir = light.GetLightDirection(hitPoint);
				shadowRay = Ray(hitPoint+offset, shadowRayDir, kRayMinT, kRayMaxT);
			}*/
			if(light.type != DIRECTIONAL)
			{
				shadowRayDir = light.GetLightDirection(hitPoint);
				distance = glm::length((light.position - hitPoint));
				shadowRay = Ray(hitPoint, shadowRayDir, kRayMinT, distance);
			}
			else
			{
				shadowRayDir = light.GetLightDirection(hitPoint);
				shadowRay = Ray(hitPoint, shadowRayDir, kRayMinT, kRayMaxT);
			}

			// Check if Shadow Ray hit any object, if so, do nothing.
			bool isBlocked = false; 
			for(vector<Surface*>::iterator iter3 = objects.begin();
				iter3 != objects.end();
				iter3++)
			{
				Surface *surf = (*iter3);
				if (surf->Hit(shadowRay, kRayMinT, shadowRay.maxT, rec))
				{
					isBlocked = true;
					break;
				}
			}

			if(isBlocked){	
				continue;	// Blocked, do nothing and continue to check next light
			} else
			{
				// Not blocked, accumulate this light's contribution
				if(light.type != DIRECTIONAL)
				{
					atten = 1.0f / (light.attenuation.x + light.attenuation.y * distance
						+ light.attenuation.z * distance * distance);
				}
				else
				{
					atten = 1.0f;
				}
				L = light.GetLightDirection(hitPoint);
				H = glm::normalize(L + V);
				nDotL = glm::dot(N, L);
				nDotH = glm::dot(N, H);
				color += diff * light.color * atten * glm::max(nDotL, 0.0f);	// Diffuse term
				color += spec * light.color * atten * glm::pow( glm::max(nDotH, 0.0f), shi ); // Specular term
			}
		}	// End of for each light

		return color;
	}
	else
	{
		//printf("Recursion Ends - No Intersection\n");
		return color;	// No intersection, just return black
	}
}
