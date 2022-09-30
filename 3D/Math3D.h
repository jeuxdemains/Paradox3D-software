#pragma once
#include <math.h>
#include "Vector.h"
#include "Texture.h"
#include "Matrix.h"

#define M_SCALE 200.0f

typedef unsigned __int32 uint32_t;

typedef struct
{
	vec3_t points[3];
	tex2_t texcoord[3];
} triangle_t;

vec3_t M3D_RotVectorX(vec3_t vector, float angle);
vec3_t M3D_RotVectorY(vec3_t vector, float angle);
vec3_t M3D_RotVectorZ(vec3_t vector, float angle);
vec3_t M3D_TranslateVec3(vec3_t vector, vec3_t pos);

vec2_t M3D_ProjectVec3(vec3_t* vector, int scrnW, int scrnH);
int M3D_Vec3HasNegZ(vec3_t v);
vec3_t M3D_MaxZOfFace(vec3_t p1, vec3_t p2, vec3_t p3);
int M3D_IsFrontFace(vec3_t p1, vec3_t p2, vec3_t p3, vec3_t camera);
void M3D_SortTrianglePointsY(vec2_t* p1, vec2_t* p2, vec2_t* p3);
void M3D_SortTexturedTrianglePointsY(vec2_t* p1, vec2_t* p2, vec2_t* p3, tex2_t* p1uv, tex2_t* p2uv, tex2_t* p3uv);
void M3D_Vec4SortTexturedTrianglePointsY(
	vec4_t* p1, vec4_t* p2, vec4_t* p3,
	tex2_t* p1uv, tex2_t* p2uv, tex2_t* p3uv);
vec2_t M3D_CalcTriangleMidPoint(vec2_t p1, vec2_t p2, vec2_t p3);

void M3D_SwapInt(int* n1, int* n2);
vec3_t M3D_BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);
void M3D_Transform(matrices_t* matrices, vec3_t triangle[3]);