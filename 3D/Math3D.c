#include "Math3D.h"

void M_SwapInt(int* n1, int* n2)
{
	int tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

Vec3_t M_TranslateVec3(Vec3_t vector, Vec3_t pos)
{
	return M_AddVec3(vector, pos);
}

Vec4_t M_TranslateVec4(Vec4_t vector, Vec4_t pos)
{
	return M_AddVec4(vector, pos); //M_AddVec3(vector, pos);
}

Vec3_t M_RotVectorX(Vec3_t vector, float angle)
{
	Vec3_t v;
	v.y = vector.y * cos(angle) - vector.z * sin(angle);
	v.z = vector.y * sin(angle) + vector.z * cos(angle);

	v.x = vector.x;

	return v;
}

Vec3_t M_RotVectorY(Vec3_t vector, float angle)
{
	Vec3_t v;
	v.x = vector.x * cos(angle) - vector.z * sin(angle);
	v.z = vector.x * sin(angle) + vector.z * cos(angle);

	v.y = vector.y;

	return v;
}

Vec3_t M_RotVectorZ(Vec3_t vector, float angle)
{
	Vec3_t v;
	v.x = vector.x * cos(angle) - vector.y * sin(angle);
	v.y = vector.x * sin(angle) + vector.y * cos(angle);

	v.z = vector.z;

	return v;
}

Vec2_t M_ProjectVec3(Vec3_t* vector, int scrnW, int scrnH)
{
	Vec2_t v;

	//vector->z += 8.5f; //M_SCALE / (M_SCALE / 2);

	v.x = (scrnW / 2) + ( vector->x / vector->z) * M_SCALE;
	v.y = (scrnH / 2) + ( vector->y / vector->z) * M_SCALE;

	return v;
}

int M_Vec3HasNegZ(Vec3_t v)
{
	if (v.z < 0.1)
		return 1;

	return 0;
}

Vec3_t M_MaxZOfFace(Vec3_t p1, Vec3_t p2, Vec3_t p3)
{
	Vec3_t v;
	if (p1.z > p2.z)
		v = p1;
	else
		v = p2;

	if (v.z < p3.z)
		v = p3;

	return v;
}

int M_IsFrontFace(Vec3_t p1, Vec3_t p2, Vec3_t p3, Vec3_t camera)
{
	Vec3_t normal = M_NormalVec3(p1, p2, p3);
	Vec3_t camRay = M_SubVec3(camera, p1);
	float dot = M_DotVec3(normal, camRay);
	
	if (dot < 0)
		return 0;

	return 1;
}

void M_Vec2Swap(Vec2_t* p1, Vec2_t* p2)
{
	Vec2_t tmp = *p1;
	p1->x = p2->x;
	p1->y = p2->y;

	p2->x = tmp.x;
	p2->y = tmp.y;
}

void M_Vec2UVSwap(Vec2_t* p1, Vec2_t* p2, Tex2_t* p1uv, Tex2_t* p2uv)
{
	Vec2_t tmp = *p1;
	Tex2_t tmpTex = *p1uv;

	p1->x = p2->x;
	p1->y = p2->y;
	p1uv->u = p2uv->u;
	p1uv->v = p2uv->v;

	p2->x = tmp.x;
	p2->y = tmp.y;
	p2uv->u = tmpTex.u;
	p2uv->v = tmpTex.v;
}

void M_Vec4UVSwap(Vec4_t* p1, Vec4_t* p2, Tex2_t* p1uv, Tex2_t* p2uv)
{
	Vec4_t tmp = *p1;
	Tex2_t tmpTex = *p1uv;

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
void M_SortTrianglePointsY(Vec2_t* p1, Vec2_t* p2, Vec2_t* p3)
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

void M_SortTexturedTrianglePointsY(
	Vec2_t* p1, Vec2_t* p2, Vec2_t* p3, 
	Tex2_t* p1uv, Tex2_t* p2uv, Tex2_t* p3uv)
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
Vec2_t M_CalcTriangleMidPoint(Vec2_t p1, Vec2_t p2, Vec2_t p3)
{
	Vec2_t pMiddle;
	pMiddle.y = p2.y;
	pMiddle.x = ((p3.x - p1.x) * (p2.y - p1.y)) / (p3.y - p1.y) + p1.x;

	return pMiddle;
}


//calculate the barycentric weights of point P
//required by the affine texture mapping
Vec3_t M_BarycentricWeights(Vec2_t a, Vec2_t b, Vec2_t c, Vec2_t p)
{
	Vec2_t ab = M_Vec2Sub(b, a);
	Vec2_t bc = M_Vec2Sub(c, b);
	Vec2_t ac = M_Vec2Sub(c, a);
	Vec2_t ap = M_Vec2Sub(p, a);
	Vec2_t bp = M_Vec2Sub(p, b);

	float triangle_area_abc = (ab.x * ac.y - ab.y * ac.x);
	float alpha = (bc.x * bp.y - bp.x * bc.y) / triangle_area_abc;
	float beta = (ap.x * ac.y - ac.x * ap.y) / triangle_area_abc;
	float gamma = 1.0f - alpha - beta;

	Vec3_t weights = { alpha, beta, gamma };

	return weights;
}