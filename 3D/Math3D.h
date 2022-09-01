#pragma once
#include <math.h>
#include "Vector.h"

#define M_SCALE 200.0f

typedef unsigned __int32 uint32_t;

Vec3_t M_RotVectorX(Vec3_t vector, float angle);
Vec3_t M_RotVectorY(Vec3_t vector, float angle);
Vec3_t M_RotVectorZ(Vec3_t vector, float angle);
Vec3_t M_TranslateVec3(Vec3_t vector, Vec3_t pos);

Vec2_t M_ProjectVec3(Vec3_t* vector, int scrnW, int scrnH);
int M_Vec3HasNegZ(Vec3_t v);
Vec3_t M_MaxZOfFace(Vec3_t p1, Vec3_t p2, Vec3_t p3);
int M_IsFrontFace(Vec3_t p1, Vec3_t p2, Vec3_t p3, Vec3_t camera);
void M_SortTrianglePointsY(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3);
Vec2_t M_CalcTriangleMidPoint(Vec2_t p1, Vec2_t p2, Vec2_t p3);
void G_RasterTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3);