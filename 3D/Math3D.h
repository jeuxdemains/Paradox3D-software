#pragma once
#include <math.h>
#include "Vector.h"
#include "Texture.h"

#define M_SCALE 200.0f

typedef unsigned __int32 uint32_t;

typedef struct
{
	Vec3_t points[3];
} Triangle_t;

Vec3_t M_RotVectorX(Vec3_t vector, float angle);
Vec3_t M_RotVectorY(Vec3_t vector, float angle);
Vec3_t M_RotVectorZ(Vec3_t vector, float angle);
Vec3_t M_TranslateVec3(Vec3_t vector, Vec3_t pos);

Vec2_t M_ProjectVec3(Vec3_t* vector, int scrnW, int scrnH);
int M_Vec3HasNegZ(Vec3_t v);
Vec3_t M_MaxZOfFace(Vec3_t p1, Vec3_t p2, Vec3_t p3);
int M_IsFrontFace(Vec3_t p1, Vec3_t p2, Vec3_t p3, Vec3_t camera);
void M_SortTrianglePointsY(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3);
void M_SortTexturedTrianglePointsY(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3, Tex2_t* p1uv, Tex2_t* p2uv, Tex2_t* p3uv);
void M_Vec4SortTexturedTrianglePointsY(
	Vec4_t* p1, Vec4_t* p2, Vec4_t* p3,
	Tex2_t* p1uv, Tex2_t* p2uv, Tex2_t* p3uv);
Vec2_t M_CalcTriangleMidPoint(Vec2_t p1, Vec2_t p2, Vec2_t p3);

void M_SwapInt(int* n1, int* n2);
Vec3_t M_BarycentricWeights(Vec2_t a, Vec2_t b, Vec2_t c, Vec2_t p);