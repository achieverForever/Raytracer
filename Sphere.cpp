#include <cstdio>
#include "Sphere.h"

Sphere::Sphere() : center(0.0f), radius(0.0f), material() { }

Sphere::Sphere(const vec3 &centr, float rad, const Material &matrl, mat4 *transf/* =NULL */, mat4 *invTransf/* =NULL */)
{
	center = centr; radius = rad; material = matrl;
	this->transf = transf;
	this->invTransf = invTransf;
}

Sphere::~Sphere() { }

bool Sphere::Hit(const Ray &viewRay, float baseT, float maxT, HitRecord &rec)
{
	float A, B, C, discmnt, t0, t1, r;
	vec3 E_C, d, e, c;
	Ray localRay = viewRay;

	if(invTransf)
	{
		// NEW	
		// For simplicity, transform the ray to sphere's Model Space for intersection test
		TransformRay(localRay, (*invTransf));
	}
	d = localRay.direction;
	e = localRay.origin;
	
	c = center;
	r = radius;
	E_C = e - c;

	B = 2.0f * glm::dot(d, E_C);
	A = 1.0f;
	C = glm::dot(E_C, E_C) - r * r;
	discmnt = B * B - 4.0f * A * C;
	if(discmnt <= 0.0f)	// No hits
	{
		return false;
	}
	else
	{
		t0 = (-B + glm::sqrt(B*B - 4*C)) * 0.5f;
		t1 = (-B - glm::sqrt(B*B - 4*C)) * 0.5f;
		if(!(t0 > 0.0f && t1 > 0.0f))	
		{								
			return false;
		}
		else
		{
			float tMin = glm::min(t0, t1);
			if(tMin <= baseT || tMin >= maxT)	// Hit occurred "behind" us or further 
			{								 	// from our last closest hit, return false
				return false;
			}
			else
			{
				vec3 localHitPoint = localRay.GetIntersectPoint(tMin);
				vec3 localNormal = (localHitPoint- c) / radius;
				// NEW
				if(transf && invTransf)
				{	
					// Transform local hit point and normal back to world space
					TransformPosition(localHitPoint, (*transf));
					TransformNormal(localNormal, (*invTransf));
				}
				rec.t = tMin;	// POSSIBLE ERROR
				rec.hitPoint = localHitPoint;
				rec.normal = localNormal;
				rec.surf = this;
				rec.material = &this->material;

				return true;
			}

		}

	}
}


