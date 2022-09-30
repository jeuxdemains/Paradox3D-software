#include "Matrix.h"

mat4_t Mat4_MakeIdentity(void)
{
	mat4_t mat = 
	{ {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	} };

	return mat;
};

mat4_t Mat4_MakePerspective(float aspect, float fov, float zNear, float zFar)
{
	// | (h/w)*1/tan(fov/2)             0              0                 0 |
	// |                  0  1/tan(fov/2)              0                 0 |
	// |                  0             0     zf/(zf-zn)  (-zf*zn)/(zf-zn) |
	// |                  0             0              1                 0 |

	mat4_t m = { {{0}} };

	m.m[0][0] = aspect * (1 / tanf(fov / 2.0f));		//aspect * half FOV
	m.m[1][1] = 1 / tanf(fov / 2.0f);				//half FOV
	m.m[2][2] = zFar / (zFar - zNear);					//normalized frustum Z len
	m.m[2][3] = (-zFar * zNear) / (zFar - zNear);		//offset the Z projection plane
	m.m[3][2] = 1.0f;									//save the original Z value
														//when mul with Vec3 in the W component

	return m;
}

mat4_t Mat4_MakeScale(vec3_t scale)
{
	mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][0] = scale.x;
	mat4.m[1][1] = scale.y;
	mat4.m[2][2] = scale.z;

	return mat4;
}

mat4_t Mat4_MakeTranslation(vec3_t translation)
{
	mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][3] = translation.x;
	mat4.m[1][3] = translation.y;
	mat4.m[2][3] = translation.z;

	return mat4;
}

mat4_t Mat4_MakeRotationX(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[1][1] = c;
	mat4.m[1][2] = -s;
	mat4.m[2][1] = s;
	mat4.m[2][2] = c;

	return mat4;
}

mat4_t Mat4_MakeRotationY(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][0] = c;
	mat4.m[0][2] = s;
	mat4.m[2][0] = -s;
	mat4.m[2][2] = c;

	return mat4;
}

mat4_t Mat4_MakeRotationZ(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	mat4_t mat4 = Mat4_MakeIdentity();
	mat4.m[0][0] = c;
	mat4.m[0][1] = -s;
	mat4.m[1][0] = s;
	mat4.m[1][1] = c;

	return mat4;
}

vec4_t Mat4_MulVec4(mat4_t m, vec4_t v)
{
	vec4_t result;

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

mat4_t Mat4_MulMat4(mat4_t m1, mat4_t m2)
{
	mat4_t m3;

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

mat4_t Mat4_Mul4Mat4(mat4_t m1, mat4_t m2, mat4_t m3, mat4_t world)
{
	mat4_t result;
	result = Mat4_MulMat4(m1, world);
	result = Mat4_MulMat4(m2, result);
	result = Mat4_MulMat4(m3, result);

	return result;
}

//Converting to NDC (normalized device context)
vec4_t Mat4_MulVec4ProjectionMat4(vec4_t v, mat4_t projMat)
{
	vec4_t result = Mat4_MulVec4(projMat, v);

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

mat4_t Mat4_LookAt(vec3_t eye, vec3_t target, vec3_t up)
{
	//Compute the right (x), forward (z) and up (y)
	vec3_t z = M_SubVec3(target, eye);
	z = M_NormalizeVec3(z);
	vec3_t x = M_CrossVec3(up, z);
	vec3_t y = M_CrossVec3(z, x);

	mat4_t viewMatrix =
	{ {
		{x.x, x.y, x.z, -M_DotVec3(x, eye)},
		{y.x, y.y, y.z, -M_DotVec3(y, eye)},
		{z.x, z.y, z.z, -M_DotVec3(z, eye)},
		{0, 0, 0, 1},
	} };

	return viewMatrix;
}