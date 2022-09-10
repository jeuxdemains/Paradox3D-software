#pragma once
#include <math.h>
#include "Vector.h"
#include "Texture.h"

#define M_SCALE 200.0f

typedef unsigned __int32 uint32_t;

typedef struct
{
	vec3_t points[3];
} Triangle_t;

vec3_t M_RotVectorX(vec3_t vector, float angle);
vec3_t M_RotVectorY(vec3_t vector, float angle);
vec3_t M_RotVectorZ(vec3_t vector, float angle);
vec3_t M_TranslateVec3(vec3_t vector, vec3_t pos);

vec2_t M_ProjectVec3(vec3_t* vector, int scrnW, int scrnH);
int M_Vec3HasNegZ(vec3_t v);
vec3_t M_MaxZOfFace(vec3_t p1, vec3_t p2, vec3_t p3);
int M_IsFrontFace(vec3_t p1, vec3_t p2, vec3_t p3, vec3_t camera);
void M_SortTrianglePointsY(vec2_t* p1, vec2_t* p2, vec2_t* p3);
void M_SortTexturedTrianglePointsY(vec2_t* p1, vec2_t* p2, vec2_t* p3, Tex2_t* p1uv, Tex2_t* p2uv, Tex2_t* p3uv);
void M_Vec4SortTexturedTrianglePointsY(
	vec4_t* p1, vec4_t* p2, vec4_t* p3,
	Tex2_t* p1uv, Tex2_t* p2uv, Tex2_t* p3uv);
vec2_t M_CalcTriangleMidPoint(vec2_t p1, vec2_t p2, vec2_t p3);

void M_SwapInt(int* n1, int* n2);
vec3_t M_BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);