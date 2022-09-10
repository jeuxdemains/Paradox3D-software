#include "clipping.h"

plane_t frustumPlanes[NUM_PLANES];

void C_InitFrustumPlanes(float fovx, float fovy, float zNear, float zFar)
{
	float cos_half_fov_x = cosf(fovx / 2);
	float sin_half_fov_x = sinf(fovx / 2);

	float cos_half_fov_y = cosf(fovy / 2);
	float sin_half_fov_y = sinf(fovy / 2);

	vec3_t origin = { 0,0,0 };

	frustumPlanes[LEFT_FRUSTUM_PLANE].point = origin;
	frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov_x;
	frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

	frustumPlanes[RIGHT_FRUSTUM_PLANE].point = origin;
	frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov_x;
	frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

	frustumPlanes[TOP_FRUSTUM_PLANE].point = origin;
	frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov_y;
	frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

	frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = origin;
	frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov_y;
	frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

	frustumPlanes[NEAR_FRUSTUM_PLANE].point.x = 0;
	frustumPlanes[NEAR_FRUSTUM_PLANE].point.y = 0;
	frustumPlanes[NEAR_FRUSTUM_PLANE].point.z = zNear;
	frustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustumPlanes[FAR_FRUSTUM_PLANE].point.x = 0;
	frustumPlanes[FAR_FRUSTUM_PLANE].point.y = 0;
	frustumPlanes[FAR_FRUSTUM_PLANE].point.z = zFar;
	frustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

polygon_t C_CreatePolyFromVertices(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t auv, tex2_t buv, tex2_t cuv)
{
	polygon_t poly =
	{
		.numVertices = 3,
		.vertices = { v0, v1, v2 },
		.texcrds = { auv, buv, cuv}
	};

	return poly;
}

float float_learp(float a, float b, float p)
{
	return a + p * (b - a);
}

void C_ClipPolyAgainstPlane(polygon_t* poly, int plane)
{
	vec3_t plane_point = frustumPlanes[plane].point;
	vec3_t plane_normal = frustumPlanes[plane].normal;

	vec3_t vertices_inside[MAX_NUM_POLY_VERTICES];
	tex2_t texcoords_inside[MAX_NUM_POLY_VERTICES];

	int num_vertices_inside = 0;

	vec3_t* current_vertex = &poly->vertices[0];
	tex2_t* current_tex = &poly->texcrds[0];
	vec3_t* prev_vertex = &poly->vertices[poly->numVertices - 1]; 
	tex2_t* prev_tex = &poly->texcrds[poly->numVertices - 1];

	//dot product of each vertex against the clipping plane
	//if dot > 0 => vertex is inside the frustum plane
	//if dot < 0 => vertex is outside the frustum plane
	float current_dot = 0; // M_DotVec3(M_SubVec3(*current_vertex, plane_point), plane_normal);
	float prev_dot = M_DotVec3(M_SubVec3(*prev_vertex, plane_point), plane_normal);

	//while current_vertex is not the last vertex of the poly
	while (current_vertex != &poly->vertices[poly->numVertices])
	{
		current_dot = M_DotVec3(M_SubVec3(*current_vertex, plane_point), plane_normal);

		//if one of the vertices is otside and the other inside 
		//dot product should be negative
		if (current_dot * prev_dot < 0)
		{
			//find interpolation factor
			float interp_factor = prev_dot / (prev_dot - current_dot);

			//calc the intersection point
			vec3_t intersection_point =
			{
				.x = float_learp(prev_vertex->x, current_vertex->x, interp_factor),
				.y = float_learp(prev_vertex->y, current_vertex->y, interp_factor),
				.z = float_learp(prev_vertex->z, current_vertex->z, interp_factor),
			};
			/*vec3_t intersection_point = M_Vec3Clone(current_vertex);
			intersection_point = M_SubVec3(intersection_point, *prev_vertex);
			intersection_point = M_MulVec3Scalar(intersection_point, interp_factor);
			intersection_point = M_AddVec3(intersection_point, *prev_vertex);*/

			//calc texture point
			tex2_t interpolated_tex =
			{
				.u = float_learp(prev_tex->u, current_tex->u, interp_factor),
				.v = float_learp(prev_tex->v, current_tex->v, interp_factor)
			};

			vertices_inside[num_vertices_inside] = M_Vec3Clone(&intersection_point);
			texcoords_inside[num_vertices_inside] = T_Texture2Clone(&interpolated_tex);
			num_vertices_inside++;
		}

		//if vertex inside the frustum
		if (current_dot > 0)
		{
			vertices_inside[num_vertices_inside] = M_Vec3Clone(current_vertex);
			texcoords_inside[num_vertices_inside] = T_Texture2Clone(current_tex);
			num_vertices_inside++;
		}

		//move to the next vertex
		prev_dot = current_dot;
		prev_vertex = current_vertex;
		prev_tex = current_tex;
		current_vertex++;
		current_tex++;
	}

	//Copy back the new vertices to the polygon
	for (int i = 0; i < num_vertices_inside; i++)
	{
		poly->vertices[i] = M_Vec3Clone(&vertices_inside[i]);
		poly->texcrds[i] = T_Texture2Clone(&texcoords_inside[i]);
	}
	poly->numVertices = num_vertices_inside;
}

void C_ClipPolygon(polygon_t* poly)
{
	C_ClipPolyAgainstPlane(poly, LEFT_FRUSTUM_PLANE);
	C_ClipPolyAgainstPlane(poly, RIGHT_FRUSTUM_PLANE);
	C_ClipPolyAgainstPlane(poly, TOP_FRUSTUM_PLANE);
	C_ClipPolyAgainstPlane(poly, BOTTOM_FRUSTUM_PLANE);
	C_ClipPolyAgainstPlane(poly, NEAR_FRUSTUM_PLANE);
	C_ClipPolyAgainstPlane(poly, FAR_FRUSTUM_PLANE);
}

void C_TrianglesFromPoly(polygon_t* poly, triangle_t tris[], int* numTris)
{
	for (int i = 0; i < poly->numVertices - 2; i++)
	{
		int idx0 = 0;
		int idx1 = i + 1;
		int idx2 = i + 2;

		tris[i].points[0] = poly->vertices[idx0];
		tris[i].points[1] = poly->vertices[idx1];
		tris[i].points[2] = poly->vertices[idx2];

		tris[i].texcoord[0] = poly->texcrds[idx0];
		tris[i].texcoord[1] = poly->texcrds[idx1];
		tris[i].texcoord[2] = poly->texcrds[idx2];
	}

	*numTris = poly->numVertices - 2;
}