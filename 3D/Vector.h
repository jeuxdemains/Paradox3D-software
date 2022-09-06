#pragma once
#include <math.h>

/// TYPEDEFS
typedef struct
{
	float x, y;
} Vec2_t;

typedef struct
{
	int x, y;
} Vec2i_t;


typedef struct
{
	Vec2_t points[3];
} ScreenTriangle_t;

typedef struct
{
	float x, y, z;
} Vec3_t;

typedef struct
{
	float x, y, z, w;
} Vec4_t;


/// VECTOR 2
Vec2_t M_Vec2Add(Vec2_t v1, Vec2_t v2);
Vec2_t M_Vec2Sub(Vec2_t vec1, Vec2_t vec2);
void M_Vec2ScaleFace(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3, float scaleFactor);
void M_Vec2TranslateFace(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3, float x, float y);
//conversions
Vec2_t M_Vec2FromVec3(Vec3_t v);
Vec2_t M_Vec2FromVec4(Vec4_t v);

/// VECTOR 3
Vec3_t M_MulVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_DivVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_AddVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_SubVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_NormalizeVec3(Vec3_t v);
float M_DotVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_CrossVec3(Vec3_t p1, Vec3_t p2);
Vec3_t M_NormalVec3(Vec3_t p1, Vec3_t p2, Vec3_t p3);
Vec3_t M_MaxZVec3(Vec3_t p1, Vec3_t p2, Vec3_t p3);
//conversions
Vec3_t M_Vec3FromVec4(Vec4_t vec);


/// VECTOR 4
Vec4_t M_MulVec4(Vec4_t p1, Vec4_t p2);
Vec4_t M_AddVec4(Vec4_t p1, Vec4_t p2);
//conversions
Vec4_t M_Vec4FromVec3(Vec3_t vec);
Vec4_t M_Vec4FromVec2(Vec2_t vec);