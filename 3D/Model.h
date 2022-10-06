#ifndef MODEL_H
#define MODEL_H

#include "Math3D.h"
#include "Matrix.h"
#include "Texture.h"
#include <stdlib.h>

#define FACE_POINTS_CNT 3
#define MODEL_MAX_CNT 20

typedef struct
{
	vec3_t face[3];
} Face_t;

typedef struct
{
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	//uint32_t color;
} FaceTex_t;

typedef struct
{
	vec3_t modelPosition;
	vec3_t* vertices;
	vec3_t rotation;
	vec3_t translation;
	vec3_t scale;

	FaceTex_t* faces;
	uint32_t vecCnt;
	uint32_t facesCnt;
	uint32_t* texture;
	uint32_t textureW;
	uint32_t textureH;
} Model_t;

typedef struct
{
	vec3_t vertices[3];
	tex2_t texCrds[3];
	float depth;
} TransformedModelFace_t;

static Model_t allModels[MODEL_MAX_CNT];
static uint32_t allModelsCnt = 0;

void ModelInit(Model_t* model);
void ModelLoadPngTexture(char* fileName, Model_t* model);

#endif