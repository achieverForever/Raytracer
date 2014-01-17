#include "Util.h"
#include "Variables.h"


Light::Light() : type(POINT), position(0.0f), color(0.0f), attenuation(1.0f, 0.0f, 0.0f) { }

Light::Light(LightType t, float x, float y, float z, float r, float g, float b, float konst/* =1.0f */, float linear/* =0.0f */, float quad/* =0.0f */)
{
	type = t;
	position = vec3(x, y, z);
	if(t == DIRECTIONAL)
		TransformDirection(position, currMatrix);
	else
		TransformPosition(position, currMatrix);
	color = Color(r, g, b);
	attenuation = vec3(konst, linear, quad);
	if(type == DIRECTIONAL)
		lightDir = glm::normalize(position);
}

Light::Light(LightType t, const vec3 &pos, const Color &col, const vec3 &atten/* =vec3 */)
{
	type = t;
	position = pos;
	color = col;
	attenuation = atten;
	if(type == DIRECTIONAL)
		lightDir = glm::normalize(position);
}

vec3 & Light::GetLightDirection(const vec3 &target)
{
	if(type == DIRECTIONAL)
	{
		return lightDir;
	}
	else
	{
		lightDir = glm::normalize(position - target);
		return lightDir;
	}
}

void RightMultiply(const mat4 & mat, stack<mat4> & matrixStack)
{
	mat4 & T = matrixStack.top();
	T = T * mat;
}

void LoadMatrix(const mat4 & mat, stack<mat4> & matrixStack)
{	
	mat4 & T = matrixStack.top();
	T = mat;
}

vec3 & TransformDirection(vec3 & direction, const mat4 & matrix)
{
	vec4 tmpDir(direction, 0.0f);
	direction = vec3(matrix * tmpDir);
	return direction;
}

vec3 & TransformNormal(vec3 & normal, const mat4 & matrix)
{
	vec4 tmpN(normal, 0.0f);
	normal = glm::normalize(vec3(glm::transpose(matrix) * tmpN));
	return normal;
}

vec3 & TransformPosition(vec3 & position, const mat4 & matrix)
{
	vec4 tmpPos(position, 1.0f);
	tmpPos = matrix * tmpPos;
	position = vec3(tmpPos / tmpPos.w);
	//position = vec3(tmpPos);
	return position;
}

void TransformRay(Ray &ray, const mat4 &matrix)
{
	TransformPosition(ray.origin, matrix);
	TransformDirection(ray.direction, matrix);
	ray.direction = glm::normalize(ray.direction);
}

void Clamp01(Color &c)
{
	c = glm::clamp(c, Color(0.0f), Color(1.0f));
}

ostream & operator<< (ostream &stream, const vec3 &v)
{
	return stream << v.x << " " << v.y << " " << v.z << " ";
}