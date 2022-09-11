#include "OBJModelLoader.h"

void OBJ_LoadModel(char* fileName, Model_t* modelData)
{
	FILE* file;
	if (fopen_s(&file, fileName, "r") != 0)
	{
		return;
	}

	char line[1024];
	vec3_t vertices[MAX_OBJ_VTX_CNT];
	FaceTex_t faces[MAX_OBJ_FACE_CNT];

	int vertCnt = 0;
	int faceCnt = 0;

	while (fgets(line, 1024, file))
	{
		//read vertex
		if (strncmp(line, "v ", 2) == 0)
		{
			vec3_t vertex;
			sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			vertices[vertCnt++] = vertex;
		}

		//read face
		if (strncmp(line, "f ", 2) == 0)
		{
			FaceTex_t face;
			sscanf_s(line, "f %d %d %d", &face.a, &face.b, &face.c);
			faces[faceCnt++] = face;
		}
	}

	fclose(file);

	//copy to model
	modelData->vertices = (vec3_t*)malloc(sizeof(vec3_t) * vertCnt);
	for (int i = 0; i < vertCnt; i++)
	{
		modelData->vertices[i].x = vertices[i].x;
		modelData->vertices[i].y = vertices[i].y;
		modelData->vertices[i].z = vertices[i].z;
	}

	modelData->faces = (FaceTex_t*)malloc(sizeof(FaceTex_t) * faceCnt);
	for (int i = 0; i < faceCnt; i++)
	{
		modelData->faces[i].a = faces[i].a;
		modelData->faces[i].b = faces[i].b;
		modelData->faces[i].c = faces[i].c;
	}

	modelData->vecCnt = vertCnt;
	modelData->facesCnt = faceCnt;
}