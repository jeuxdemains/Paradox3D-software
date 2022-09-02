#include "Matrix.h"

Mat4_t Mat4_MakeIdentity(void)
{
	Mat4_t mat = 
	{ {
		{1.0f, 0, 0 ,0},
		{0, 1.0f, 0 ,0},
		{0,0,1.0f, 0},
		{0,0,0,1.0f}
	} };

	return mat;
};

Mat4_t Mat4_MakePerspective(float aspect, float fov, float zNear, float zFar)
{
	Mat4_t m = { {{0}} };

	m.m[0][0] = aspect * (1 / tanf(fov / 2.0f));		//aspect * half FOV
	m.m[1][1] = 1 / tanf(fov / 2.0f);				//half FOV
	m.m[2][2] = zFar / (zFar - zNear);					//normalized frustum Z len
	m.m[3][3] = (-zFar * zNear) / (zFar * zNear);		//offset the Z projection plane
	m.m[3][2] = 1.0f;									//save the original Z value
														//when mul with Vec3 in the W component

	return m;
}

Mat4_t Mat4_MakeScale(float sx, float sy, float sz)
{
	Mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][0] = sx;
	mat4.m[1][1] = sy;
	mat4.m[2][2] = sz;

	return mat4;
}

Mat4_t Mat4_MakeTranslation(float tx, float ty, float tz)
{
	Mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][3] = tx;
	mat4.m[1][3] = ty;
	mat4.m[2][3] = tz;

	return mat4;
}

Mat4_t Mat4_MakeRotationX(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	Mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[1][1] = c;
	mat4.m[1][2] = -s;
	mat4.m[2][1] = s;
	mat4.m[2][2] = c;

	return mat4;
}

Mat4_t Mat4_MakeRotationY(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	Mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][0] = c;
	mat4.m[0][2] = s;
	mat4.m[2][0] = -s;
	mat4.m[2][2] = c;

	return mat4;
}

Mat4_t Mat4_MakeRotationZ(float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	Mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][0] = c;
	mat4.m[0][1] = -s;
	mat4.m[1][0] = s;
	mat4.m[1][1] = c;

	return mat4;
}

Vec4_t Mat4_MulVec4(Mat4_t m, Vec4_t v)
{
	Vec4_t result;

	result.x = 
		m.m[0][0] * v.x + 
		m.m[0][1] * v.y + 
		m.m[0][2] * v.z + 
		m.m[0][3] * v.w;

	result.y = 
		m.m[1][0] * v.x + 
		m.m[1][1] * v.y + 
		m.m[1][2] * v.z + 
		m.m[1][3] * v.w;

	result.z = 
		m.m[2][0] * v.x + 
		m.m[2][1] * v.y + 
		m.m[2][2] * v.z + 
		m.m[2][3] * v.w;

	result.w = 
		m.m[3][0] * v.x + 
		m.m[3][1] * v.y + 
		m.m[3][2] * v.z + 
		m.m[3][3] * v.w;

	return result;
}

Mat4_t Mat4_MulMat4(Mat4_t m1, Mat4_t m2)
{
	Mat4_t m3;

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			m3.m[row][col] =
				m1.m[row][0] * m2.m[0][col] +
				m1.m[row][1] * m2.m[1][col] +
				m1.m[row][2] * m2.m[2][col] +
				m1.m[row][3] * m2.m[3][col];
		}
	}

	return m3;
}
/// 
/// NOTE: working with CW world coordinates
/// 

Mat4_t Mat4_Mul4Mat4(Mat4_t m1, Mat4_t m2, Mat4_t m3, Mat4_t world)
{
	Mat4_t result;
	result = Mat4_MulMat4(m1, world);
	result = Mat4_MulMat4(m2, result);
	result = Mat4_MulMat4(m3, result);

	return result;
}

//Converting to NDC (normalized device context)
Vec4_t Mat4_MulVec4ProjectionMat4(Vec4_t v, Mat4_t projMat)
{
	Vec4_t result = Mat4_MulVec4(projMat, v);

	if (result.w != 0.0)
	{
		//divide all by the original Z saved in W
		//at the projection matrix mul phase
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}

	return result;
}