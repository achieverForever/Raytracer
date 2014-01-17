#include "Triangle.h"

Triangle::Triangle() { }

Triangle::Triangle(int i0, int i1, int i2, Material *matrl, vector<Vertex> *vertices, mat4 *transf/*=NULL*/, mat4 *invTransf/*=NULL*/)
{
	vert0 = (*vertices)[i0];
	vert1 = (*vertices)[i1];
	vert2 = (*vertices)[i2];
	material = matrl;
	this->transf = transf;
	this->invTransf = invTransf;

	// Transform the triangle vertices to World Space
	if(transf)
	{
		TransformPosition(vert0, (*transf));
		TransformPosition(vert1, (*transf));
		TransformPosition(vert2, (*transf));
	}
	normal = glm::normalize(glm::cross((vert1-vert0), (vert2-vert1)));
}

Triangle::~Triangle() { }

bool Triangle::Hit(const Ray &viewRay, float baseT, float lastMinT, HitRecord &rec)
{
	float t, determinant, beta, gamma, invDet, dDotN;
	
	const vec3 &A = vert0, &B = vert1, &C = vert2, &E = viewRay.origin, &D = viewRay.direction;

	determinant =  (A.x-B.x) * ((A.y-C.y)*D.z - (A.z-C.z)*D.y)
				 + (A.y-B.y) * ((A.z-C.z)*D.x - (A.x-C.x)*D.z)
				 + (A.z-B.z) * ((A.x-C.x)*D.y - (A.y-C.y)*D.x);
	invDet = 1.0f / determinant;
	
	// Compute t
	dDotN = glm::dot(D, normal);
	if(dDotN <= 0.0001f && dDotN >= -0.0001f)	// Ray is parallel to the plane
		return false;
	t = (glm::dot(A, normal) - glm::dot(E, normal)) / dDotN;

	if(t < baseT || t > lastMinT)	// Not intersect with plane that the triangle lies on
		return false;

	float a, b, c, d, e, f, g, h, i, j, k, l;
	a = A.x-B.x;
	b = A.y-B.y;
	c = A.z-B.z;
	d = A.x-C.x;
	e = A.y-C.y;
	f = A.z-C.z;
	g = D.x;
	h = D.y;
	i = D.z;
	j = A.x-E.x;
	k = A.y-E.y;
	l = A.z-E.z;

	// Compute gamma
	gamma =(i*(a*k-j*b)
			+ h*(j*c-a*l)
			+ g*(b*l-k*c)) * invDet;
	if(gamma < 0.0f || gamma > 1.0f)	// Intersects with this plane but miss this triangle
		return false;

	// Compute beta
	beta = (j*(e*i-h*f)
			+ k*(g*f-d*i)
			+ l*(d*h-e*g)) * invDet;
	if(beta < 0.0f || beta > (1.0f-gamma))	// Intersects with this plane but missed the triangle
		return false;

	// If we reach here, we do hit the triangle, return hitRecord
	rec.t = t;
	rec.hitPoint = viewRay.GetIntersectPoint(t);
	rec.normal = this->normal;
	rec.surf = this;
	rec.material = this->material;

	return true;
}

