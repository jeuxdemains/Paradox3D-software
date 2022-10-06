#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

/// TYPEDEFS
typedef struct
{
	float x, y;
} vec2_t;

typedef struct
{
	int x, y;
} vec2i_t;


typedef struct
{
	vec2_t points[3];
} ScreenTriangle_t;

typedef struct
{
	float x, y, z;
} vec3_t;

typedef struct
{
	float x, y, z, w;
} vec4_t;

//SCALAR OPERATIONS
vec3_t M_MulVec3Scalar(vec3_t p1, float scalar);

/// VECTOR 2
vec2_t M_Vec2Add(vec2_t v1, vec2_t v2);
vec2_t M_Vec2Sub(vec2_t vec1, vec2_t vec2);
void M_Vec2ScaleFace(vec2_t* p1, vec2_t* p2, vec2_t* p3, float scaleFactor);
void M_Vec2TranslateFace(vec2_t* p1, vec2_t* p2, vec2_t* p3, float x, float y);
//conversions
vec2_t M_Vec2FromVec3(vec3_t v);
vec2_t M_Vec2FromVec4(vec4_t v);

/// VECTOR 3
vec3_t M_NewVec3(float a, float b, float c);
vec3_t M_MulVec3(vec3_t p1, vec3_t p2);
vec3_t M_DivVec3(vec3_t p1, vec3_t p2);
vec3_t M_AddVec3(vec3_t p1, vec3_t p2);
vec3_t M_SubVec3(vec3_t p1, vec3_t p2);
vec3_t M_NormalizeVec3(vec3_t v);
float M_DotVec3(vec3_t p1, vec3_t p2);
vec3_t M_CrossVec3(vec3_t p1, vec3_t p2);
vec3_t M_NormalVec3(vec3_t p1, vec3_t p2, vec3_t p3);
vec3_t M_MaxZVec3(vec3_t p1, vec3_t p2, vec3_t p3);
vec3_t M_Vec3Clone(vec3_t* vertex);
//conversions
vec3_t M_Vec3FromVec4(vec4_t vec);


/// VECTOR 4
vec4_t M_MulVec4(vec4_t p1, vec4_t p2);
vec4_t M_AddVec4(vec4_t p1, vec4_t p2);
//conversions
vec4_t M_Vec4FromVec3(vec3_t vec);
vec4_t M_Vec4FromVec2(vec2_t vec);

#endif