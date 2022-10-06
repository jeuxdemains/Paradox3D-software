#include "OBJModelLoader.h"

void OBJ_LoadModel(char* fileName, Model_t* inModelPointer)
{
	FILE* file;

	printf("opening object file: %s\n", fileName);

#ifdef _win32
	if (fopen_s(&file, fileName, "r") != 0) return;
#else
	file = fopen(fileName, "rb");
	if (file == NULL) 
	{
		printf("failed to open model file: %s\n", fileName);
		return;
	}
#endif


	char line[1024];
	vec3_t* vertices = (vec3_t*)malloc(sizeof(vec3_t) * MAX_OBJ_VTX_CNT); //[MAX_OBJ_VTX_CNT];
	tex2_t* texUV = (tex2_t*)malloc(sizeof(tex2_t) * MAX_OBJ_VTX_CNT); //[MAX_OBJ_VTX_CNT];
	FaceTex_t* faces = (FaceTex_t*)malloc(sizeof(FaceTex_t) * MAX_OBJ_FACE_CNT); // [MAX_OBJ_FACE_CNT] ;

	int vertCnt = 0;
	int texCnt = 0;
	int faceCnt = 0;

	while (fgets(line, 1023, file))
	{
		//read vertex
		if (strncmp(line, "v ", 2) == 0)
		{
			vec3_t vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			vertices[vertCnt++] = vertex;
		}

		//read texture UV
		if (strncmp(line, "vt ", 3) == 0)
		{
			tex2_t tex;
			sscanf(line, "vt %f %f", &tex.u, &tex.v);
			texUV[texCnt++] = tex;
		}

		//read face
		if (strncmp(line, "f ", 2) == 0)
		{
			int vertIndecies[3];
			int texIndecied[3];
			int normalIndecies[3];

			sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertIndecies[0], &texIndecied[0], &normalIndecies[0],
				&vertIndecies[1], &texIndecied[1], &normalIndecies[1],
				&vertIndecies[2], &texIndecied[2], &normalIndecies[2]);

			FaceTex_t face =
			{
				.a = vertIndecies[0] - 1,
				.b = vertIndecies[1] - 1,
				.c = vertIndecies[2] - 1,
				.a_uv = texUV[texIndecied[0] - 1],
				.b_uv = texUV[texIndecied[1] - 1],
				.c_uv = texUV[texIndecied[2] - 1],
			};

			faces[faceCnt++] = face;
		}
	}

	fclose(file);

	//copy to model
	inModelPointer->vertices = (vec3_t*)malloc(sizeof(vec3_t) * vertCnt);
	for (int i = 0; i < vertCnt; i++)
	{
		inModelPointer->vertices[i].x = vertices[i].x;
		inModelPointer->vertices[i].y = vertices[i].y;
		inModelPointer->vertices[i].z = vertices[i].z;
	}

	inModelPointer->faces = (FaceTex_t*)malloc(sizeof(FaceTex_t) * faceCnt);
	for (int i = 0; i < faceCnt; i++)
	{
		inModelPointer->faces[i].a = faces[i].a;
		inModelPointer->faces[i].b = faces[i].b;
		inModelPointer->faces[i].c = faces[i].c;
		inModelPointer->faces[i].a_uv = faces[i].a_uv;
		inModelPointer->faces[i].b_uv = faces[i].b_uv;
		inModelPointer->faces[i].c_uv = faces[i].c_uv;
	}

	inModelPointer->vecCnt = vertCnt;
	inModelPointer->facesCnt = faceCnt;

	free(texUV);
	free(vertices);
	free(faces);
}
