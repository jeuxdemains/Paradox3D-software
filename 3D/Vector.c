#include "Vector.h"

// SCALAR OPERATIONS
vec3_t M_MulVec3Scalar(vec3_t p1, float scalar)
{
	vec3_t v;
	v.x = p1.x * scalar;
	v.y = p1.y * scalar;
	v.z = p1.z * scalar;

	return v;
}

///
/// VECTOR 2
///

vec2_t M_Vec2Add(vec2_t v1, vec2_t v2)
{
	vec2_t v3;
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;

	return v3;
}

vec2_t M_Vec2Sub(vec2_t vec1, vec2_t vec2)
{
	vec2_t vec;
	vec.x = vec1.x - vec2.x;
	vec.y = vec1.y - vec2.y;

	return vec;
}

void M_Vec2ScaleFace(vec2_t* p1, vec2_t* p2, vec2_t* p3, float scaleFactor)
{
	p1->x *= scaleFactor;
	p1->y *= scaleFactor;
	p2->x *= scaleFactor;
	p2->y *= scaleFactor;
	p3->x *= scaleFactor;
	p3->y *= scaleFactor;
}

void M_Vec2TranslateFace(vec2_t* p1, vec2_t* p2, vec2_t* p3, float x, float y)
{
	p1->x += x;
	p1->y += y;
	p2->x += x;
	p2->y += y;
	p3->x += x;
	p3->y += y;
}

/// 
/// VECTOR 3
///

vec3_t M_NewVec3(float a, float b, float c)
{
	vec3_t v = { a, b, c };
	return v;
}

vec3_t M_CrossVec3(vec3_t p1, vec3_t p2)
{
	vec3_t normal;
	normal.x = p1.y * p2.z - p1.z * p2.y;
	normal.y = p1.z * p2.x - p1.x * p2.z;
	normal.z = p1.x * p2.y - p1.y * p2.x;

	return normal;
}

vec3_t M_MulVec3(vec3_t p1, vec3_t p2)
{
	vec3_t v;
	v.x = p1.x * p2.x;
	v.y = p1.y * p2.y;
	v.z = p1.z * p2.z;

	return v;
}

vec3_t M_DivVec3(vec3_t p1, vec3_t p2)
{
	vec3_t v;
	v.x = p1.x / p2.x;
	v.y = p1.y / p2.y;
	v.z = p1.z / p2.z;

	return v;
}

vec3_t M_AddVec3(vec3_t p1, vec3_t p2)
{
	vec3_t v;
	v.x = p1.x + p2.x;
	v.y = p1.y + p2.y;
	v.z = p1.z + p2.z;

	return v;
}

vec3_t M_SubVec3(vec3_t p1, vec3_t p2)
{
	vec3_t v;
	v.x = p1.x - p2.x;
	v.y = p1.y - p2.y;
	v.z = p1.z - p2.z;

	return v;
}

float M_DotVec3(vec3_t p1, vec3_t p2)
{
	return
		(p1.x * p2.x) +
		(p1.y * p2.y) +
		(p1.z * p2.z);
}

vec3_t M_NormalVec3(vec3_t p1, vec3_t p2, vec3_t p3)
{
	vec3_t d1 = M_SubVec3(p2, p1);
	vec3_t d2 = M_SubVec3(p3, p1);

	vec3_t normal = M_CrossVec3(d1, d2);

	return normal;
}

vec3_t M_NormalizeVec3(vec3_t v)
{
	vec3_t vec;
	float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	vec.x = v.x / len;
	vec.y = v.y / len;
	vec.z = v.z / len;

	return vec;
}

vec3_t M_MaxZVec3(vec3_t p1, vec3_t p2, vec3_t p3)
{
	vec3_t p;
	if (p1.z > p2.z)
		p = p1;
	else
		p = p2;

	if (p.z < p3.z)
		p = p3;

	return p;
}

vec3_t M_Vec3Clone(vec3_t* vertex)
{
	vec3_t v =
	{
		.x = vertex->x,
		.y = vertex->y,
		.z = vertex->z
	};

	return v;
}

///
/// VECTOR 4
///

vec4_t M_Vec4FromVec3(vec3_t vec)
{
	vec4_t vec4 = { vec.x, vec.y, vec.z, 1.0f };
	return vec4;
}

vec4_t M_Vec4FromVec2(vec2_t vec)
{
	vec4_t v = { .x = vec.x, .y = vec.y, .z = 0.0f, .w = 1.0f };
	return v;
}

vec3_t M_Vec3FromVec4(vec4_t vec)
{
	vec3_t vec3 = { vec.x, vec.y, vec.z };
	return vec3;
}

vec2_t M_Vec2FromVec3(vec3_t v)
{
	vec2_t v2;
	v2.x = v.x;
	v2.y = v.y;

	return v2;
}

vec2_t M_Vec2FromVec4(vec4_t v)
{
	vec2_t v2;
	v2.x = v.x;
	v2.y = v.y;

	return v2;
}

vec4_t M_MulVec4(vec4_t p1, vec4_t p2)
{
	vec4_t v;
	v.x = p1.x * p2.x;
	v.y = p1.y * p2.y;
	v.z = p1.z * p2.z;
	v.w = p1.w * p2.w;

	return v;
}

vec4_t M_AddVec4(vec4_t p1, vec4_t p2)
{
	vec4_t v;
	v.x = p1.x + p2.x;
	v.y = p1.y + p2.y;
	v.z = p1.z + p2.z;
	v.w = p1.w;

	return v;
}