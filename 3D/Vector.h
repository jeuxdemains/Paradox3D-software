#pragma once

///
/// VECTOR 2
///

typedef struct
{
	float x, y;
} Vec2_t;

typedef struct
{
	Vec2_t points[3];
} ScreenTriangle_t;

Vec2_t M_Vec2Add(Vec2_t v1, Vec2_t v2);


/// 
/// VECTOR 3
///

typedef struct
{
	float x, y, z;
} Vec3_t;

Vec3_t M_MulVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_DivVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_AddVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_SubVec3(Vec3_t p1, Vec3_t p2);

float M_DotVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_CrossVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_NormalVec3(Vec3_t p1, Vec3_t p2, Vec3_t p3);
Vec3_t M_MaxZVec3(Vec3_t p1, Vec3_t p2, Vec3_t p3);

///
/// VECTOR 4
///

typedef struct
{
	float x, y, z, w;
} Vec4_t;

Vec4_t M_Vec4FromVec3(Vec3_t vec);
Vec3_t M_Vec3FromVec4(Vec4_t vec);