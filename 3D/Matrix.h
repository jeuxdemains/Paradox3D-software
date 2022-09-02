#pragma once
#include "Vector.h"
#include <math.h>

typedef struct
{
	float m[4][4];
} Mat4_t;

Mat4_t Mat4_MakeIdentity(void);
Mat4_t Mat4_MakeScale(float sx, float sy, float sz);
Vec4_t Mat4_MulVec4(Mat4_t m, Vec4_t v);
Mat4_t Mat4_MakeTranslation(float tx, float ty, float tz);
Mat4_t Mat4_MakeRotationX(float angle);
Mat4_t Mat4_MakeRotationY(float angle);
Mat4_t Mat4_MakeRotationZ(float angle);
Mat4_t Mat4_MakePerspective(float aspect, float fov, float zNear, float zFar);

Mat4_t Mat4_MulMat4(Mat4_t m1, Mat4_t m2);
Mat4_t Mat4_Mul4Mat4(Mat4_t m1, Mat4_t m2, Mat4_t m3, Mat4_t world);
Vec4_t Mat4_MulVec4ProjectionMat4(Vec4_t v, Mat4_t projMat);