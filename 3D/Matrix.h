#pragma once
#include "Vector.h"
#include <math.h>

typedef struct
{
	float m[4][4];
} mat4_t;

mat4_t Mat4_MakeIdentity(void);
mat4_t Mat4_MakeScale(float sx, float sy, float sz);
vec4_t Mat4_MulVec4(mat4_t m, vec4_t v);
mat4_t Mat4_MakeTranslation(float tx, float ty, float tz);
mat4_t Mat4_MakeRotationX(float angle);
mat4_t Mat4_MakeRotationY(float angle);
mat4_t Mat4_MakeRotationZ(float angle);
mat4_t Mat4_MakePerspective(float aspect, float fov, float zNear, float zFar);
mat4_t Mat4_LookAt(vec3_t eye, vec3_t target, vec3_t up);

mat4_t Mat4_MulMat4(mat4_t m1, mat4_t m2);
mat4_t Mat4_Mul4Mat4(mat4_t m1, mat4_t m2, mat4_t m3, mat4_t world);
vec4_t Mat4_MulVec4ProjectionMat4(vec4_t v, mat4_t projMat);
