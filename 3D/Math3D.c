#include "Math3D.h"

void M3D_SwapInt(int* n1, int* n2)
{
	int tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

vec3_t M3D_TranslateVec3(vec3_t vector, vec3_t pos)
{
	return M_AddVec3(vector, pos);
}

vec4_t M_TranslateVec4(vec4_t vector, vec4_t pos)
{
	return M_AddVec4(vector, pos); //M_AddVec3(vector, pos);
}

vec3_t M3D_RotVectorX(vec3_t vector, float angle)
{
	vec3_t v;
	v.y = vector.y * cosf(angle) - vector.z * sinf(angle);
	v.z = vector.y * sinf(angle) + vector.z * cosf(angle);

	v.x = vector.x;

	return v;
}

vec3_t M3D_RotVectorY(vec3_t vector, float angle)
{
	vec3_t v;
	v.x = vector.x * cosf(angle) - vector.z * sinf(angle);
	v.z = vector.x * sinf(angle) + vector.z * cosf(angle);

	v.y = vector.y;

	return v;
}

vec3_t M3D_RotVectorZ(vec3_t vector, float angle)
{
	vec3_t v;
	v.x = vector.x * cosf(angle) - vector.y * sinf(angle);
	v.y = vector.x * sinf(angle) + vector.y * cosf(angle);

	v.z = vector.z;

	return v;
}

vec2_t M3D_ProjectVec3(vec3_t* vector, int scrnW, int scrnH)
{
	vec2_t v;

	//vector->z += 8.5f; //M_SCALE / (M_SCALE / 2);

	v.x = (scrnW / 2) + ( vector->x / vector->z) * M_SCALE;
	v.y = (scrnH / 2) + ( vector->y / vector->z) * M_SCALE;

	return v;
}

int M3D_Vec3HasNegZ(vec3_t v)
{
	if (v.z < 0.1)
		return 1;

	return 0;
}

vec3_t M3D_MaxZOfFace(vec3_t p1, vec3_t p2, vec3_t p3)
{
	vec3_t v;
	if (p1.z > p2.z)
		v = p1;
	else
		v = p2;

	if (v.z < p3.z)
		v = p3;

	return v;
}

int M3D_IsFrontFace(vec3_t p1, vec3_t p2, vec3_t p3, vec3_t camera)
{
	vec3_t normal = M_NormalVec3(p1, p2, p3);
	vec3_t camRay = M_SubVec3(camera, p1);
	float dot = M_DotVec3(normal, camRay);
	
	if (dot < 0)
		return 0;

	return 1;
}

void M_Vec2Swap(vec2_t* p1, vec2_t* p2)
{
	vec2_t tmp = *p1;
	p1->x = p2->x;
	p1->y = p2->y;

	p2->x = tmp.x;
	p2->y = tmp.y;
}

void M_Vec2UVSwap(vec2_t* p1, vec2_t* p2, tex2_t* p1uv, tex2_t* p2uv)
{
	vec2_t tmp = *p1;
	tex2_t tmpTex = *p1uv;

	p1->x = p2->x;
	p1->y = p2->y;
	p1uv->u = p2uv->u;
	p1uv->v = p2uv->v;

	p2->x = tmp.x;
	p2->y = tmp.y;
	p2uv->u = tmpTex.u;
	p2uv->v = tmpTex.v;
}

void M_Vec4UVSwap(vec4_t* p1, vec4_t* p2, tex2_t* p1uv, tex2_t* p2uv)
{
	vec4_t tmp = *p1;
	tex2_t tmpTex = *p1uv;

	p1->x = p2->x;
	p1->y = p2->y;
	p1->z = p2->z;
	p1->w = p2->w;

	p1uv->u = p2uv->u;
	p1uv->v = p2uv->v;

	p2->x = tmp.x;
	p2->y = tmp.y;
	p2->z = tmp.z;
	p2->w = tmp.w;

	p2uv->u = tmpTex.u;
	p2uv->v = tmpTex.v;
}

//sort triangle vertices by Y coordinate
void M3D_SortTrianglePointsY(vec2_t* p1, vec2_t* p2, vec2_t* p3)
{
	if ((int)p1->y > (int)p2->y)
	{
		M_Vec2Swap(p1, p2);
	}

	if ((int)p2->y > (int)p3->y)
	{
		M_Vec2Swap(p2, p3);
	}

	if ((int)p1->y > (int)p2->y)
	{
		M_Vec2Swap(p1, p2);
	}
}

void M3D_SortTexturedTrianglePointsY(
	vec2_t* p1, vec2_t* p2, vec2_t* p3, 
	tex2_t* p1uv, tex2_t* p2uv, tex2_t* p3uv)
{
	if ((int)p1->y > (int)p2->y)
	{
		M_Vec2UVSwap(p1, p2, p1uv, p2uv);
	}

	if ((int)p2->y > (int)p3->y)
	{
		M_Vec2UVSwap(p2, p3, p2uv, p3uv);
	}

	if ((int)p1->y > (int)p2->y)
	{
		M_Vec2UVSwap(p1, p2, p1uv, p2uv);
	}
}

void M3D_Vec4SortTexturedTrianglePointsY(
	vec4_t* p1, vec4_t* p2, vec4_t* p3,
	tex2_t* p1uv, tex2_t* p2uv, tex2_t* p3uv)
{
	if ((int)p1->y > (int)p2->y)
	{
		M_Vec4UVSwap(p1, p2, p1uv, p2uv);
	}

	if ((int)p2->y > (int)p3->y)
	{
		M_Vec4UVSwap(p2, p3, p2uv, p3uv);
	}

	if ((int)p1->y > (int)p2->y)
	{
		M_Vec4UVSwap(p1, p2, p1uv, p2uv);
	}
}

//calculate triangle flat-bottom / flat-top
//required for rasterization
vec2_t M3D_CalcTriangleMidPoint(vec2_t p1, vec2_t p2, vec2_t p3)
{
	vec2_t pMiddle;
	pMiddle.y = p2.y;
	pMiddle.x = ((p3.x - p1.x) * (p2.y - p1.y)) / (p3.y - p1.y) + p1.x;

	return pMiddle;
}


//calculate the barycentric weights of point P
//required by the affine texture mapping
vec3_t M3D_BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	vec2_t ab = M_Vec2Sub(b, a);
	vec2_t bc = M_Vec2Sub(c, b);
	vec2_t ac = M_Vec2Sub(c, a);
	vec2_t ap = M_Vec2Sub(p, a);
	vec2_t bp = M_Vec2Sub(p, b);

	float triangle_area_abc = (ab.x * ac.y - ab.y * ac.x);
	float alpha = (bc.x * bp.y - bp.x * bc.y) / triangle_area_abc;
	float beta = (ap.x * ac.y - ac.x * ap.y) / triangle_area_abc;
	float gamma = 1.0f - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };

	return weights;
}

void M3D_Transform(matrices_t* matrices, vec3_t triangle[3])
{
	//TRANSFORM
	for (int i = 0; i < 3; i++)
	{
		matrices->worldMatrix = Mat4_MakeIdentity();
		matrices->rotMat = Mat4_Mul4Mat4(
			matrices->rotMatX,
			matrices->rotMatY,
			matrices->rotMatZ,
			Mat4_MakeIdentity());

		matrices->worldMatrix = Mat4_Mul4Mat4(
			matrices->scaleMat,
			matrices->rotMat,
			matrices->transMat,
			matrices->worldMatrix);

		//multiply the original vector by the world matrix
		vec4_t original_vector = M_Vec4FromVec3(triangle[i]);
		vec4_t transformed_vector = Mat4_MulVec4(matrices->worldMatrix, original_vector);

		//multiply the transformed vector by the view matrix to update it according to the camera
		transformed_vector = Mat4_MulVec4(matrices->viewMat, transformed_vector);

		//save to transformed vertices array
		triangle[i] = M_Vec3FromVec4(transformed_vector);
	}
}