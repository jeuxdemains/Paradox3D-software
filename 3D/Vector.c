#include "Vector.h"

///
/// VECTOR 2
///

Vec2_t M_Vec2Add(Vec2_t v1, Vec2_t v2)
{
	Vec2_t v3;
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;

	return v3;
}

/// 
/// VECTOR 3
///

Vec3_t M_CrossVec3(Vec3_t p1, Vec3_t p2)
{
	Vec3_t normal;
	normal.x = p1.y * p2.z - p1.z * p2.y;
	normal.y = p1.z * p2.x - p1.x * p2.z;
	normal.z = p1.x * p2.y - p1.y * p2.x;

	return normal;
}

Vec3_t M_MulVec3(Vec3_t p1, Vec3_t p2)
{
	Vec3_t v;
	v.x = p1.x * p2.x;
	v.y = p1.y * p2.y;
	v.z = p1.z * p2.z;

	return v;
}

Vec3_t M_DivVec3(Vec3_t p1, Vec3_t p2)
{
	Vec3_t v;
	v.x = p1.x / p2.x;
	v.y = p1.y / p2.y;
	v.z = p1.z / p2.z;

	return v;
}

Vec3_t M_AddVec3(Vec3_t p1, Vec3_t p2)
{
	Vec3_t v;
	v.x = p1.x + p2.x;
	v.y = p1.y + p2.y;
	v.z = p1.z + p2.z;

	return v;
}

Vec3_t M_SubVec3(Vec3_t p1, Vec3_t p2)
{
	Vec3_t v;
	v.x = p1.x - p2.x;
	v.y = p1.y - p2.y;
	v.z = p1.z - p2.z;

	return v;
}

float M_DotVec3(Vec3_t p1, Vec3_t p2)
{
	return
		(p1.x * p2.x) +
		(p1.y * p2.y) +
		(p1.z * p2.z);
}

Vec3_t M_NormalVec3(Vec3_t p1, Vec3_t p2, Vec3_t p3)
{
	Vec3_t d1 = M_SubVec3(p2, p1);
	Vec3_t d2 = M_SubVec3(p3, p1);

	Vec3_t normal = M_CrossVec3(d1, d2);

	return normal;
}

Vec3_t M_MaxZVec3(Vec3_t p1, Vec3_t p2, Vec3_t p3)
{
	Vec3_t p;
	if (p1.z > p2.z)
		p = p1;
	else
		p = p2;

	if (p.z < p3.z)
		p = p3;

	return p;
}

///
/// VECTOR 4
///

Vec4_t M_Vec4FromVec3(Vec3_t vec)
{
	Vec4_t vec4 = { vec.x, vec.y, vec.z, 1.0f };
	return vec4;
}

Vec3_t M_Vec3FromVec4(Vec4_t vec)
{
	Vec3_t vec3 = { vec.x, vec.y, vec.z };
	return vec3;
}

Vec2_t M_Vec2FromVec3(Vec3_t v)
{
	Vec2_t v2;
	v2.x = v.x;
	v2.y = v.y;

	return v2;
}

Vec2_t M_Vec2FromVec4(Vec4_t v)
{
	Vec2_t v2;
	v2.x = v.x;
	v2.y = v.y;

	return v2;
}

Vec2_t M_Vec2ScaleFace(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3, float scaleFactor)
{
	p1->x *= scaleFactor;
	p1->y *= scaleFactor;
	p2->x *= scaleFactor;
	p2->y *= scaleFactor;
	p3->x *= scaleFactor;
	p3->y *= scaleFactor;
}

Vec2_t M_Vec2TranslateFace(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3, float x, float y)
{
	p1->x += x;
	p1->y += y;
	p2->x += x;
	p2->y += y;
	p3->x += x;
	p3->y += y;
}