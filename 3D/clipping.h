#pragma once
#include "Vector.h"
#include "Math3D.h"

#define NUM_PLANES 6
#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIS 10

enum
{
	LEFT_FRUSTUM_PLANE,
	RIGHT_FRUSTUM_PLANE,
	TOP_FRUSTUM_PLANE,
	BOTTOM_FRUSTUM_PLANE,
	NEAR_FRUSTUM_PLANE,
	FAR_FRUSTUM_PLANE
};

typedef struct
{
	vec3_t point;
	vec3_t normal;
} plane_t;

typedef struct
{
	vec3_t vertices[MAX_NUM_POLY_VERTICES];
	tex2_t texcrds[MAX_NUM_POLY_VERTICES];
	int numVertices;
} polygon_t;

void C_InitFrustumPlanes(float fovx, float fovy, float zNear, float zFar);
polygon_t C_CreatePolyFromVertices(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t auv, tex2_t buv, tex2_t cuv);
void C_ClipPolygon(polygon_t* poly);
void C_TrianglesFromPoly(polygon_t* poly, triangle_t tris[], int* numTris);