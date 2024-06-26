#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDLSystem.h"
#include "Math3D.h"
#include "Model.h"
#include "Matrix.h"
#include "camera.h"
#include "clipping.h"
#include "OBJModelLoader.h"
#include "debug.h"

#define SLOW_REND_DELAY 10
#define FPS_TARGET 60
#define LOGICAL_SCRN_W 1024
#define LOGICAL_SCRN_H 768
#define SCRN_W LOGICAL_SCRN_W / 3;
#define SCRN_H LOGICAL_SCRN_H / 3;

typedef struct
{
	vec3_t direction;
} Light_t;

typedef struct
{
	vec3_t pos;
	float dirAngle;
} Position_t;

typedef struct
{
	char* textureName;
	char* modelName;
} model_list_t;

void G_RunRenderLoop();
void G_DrawPoint(vec2_t v);
void G_DrawPointI(uint32_t x, uint32_t y);
void G_DrawPointColor(vec2_t v, uint32_t color);
void G_DrawTexel(int x, int y, Model_t* model,
	vec4_t a, vec4_t b, vec4_t c,
	tex2_t uv_a, tex2_t uv_b, tex2_t uv_c,
	float lightPercFactor);
void G_DrawPixel(int x, int y, uint32_t color,
	vec4_t point_a, vec4_t point_b, vec4_t point_c,
	tex2_t uv_a, tex2_t uv_b, tex2_t uv_c,
	float lightPercFactor);

void G_DrawLine(vec2_t p1, vec2_t p2);
void G_DrawLineI(vec2i_t p1, vec2i_t p2);

void G_RasterTriangle(vec2_t p1, vec2_t p2, vec2_t p3);
void G_RenderTexturedTriangle(
	vec4_t p1, vec4_t p2, vec4_t p3,
	tex2_t tp1, tex2_t tp2, tex2_t tp3,
	Model_t* model, float lightPrecFactor, int isSolidColor, uint32_t color);

void G_ClearBuffer();
void G_SetDrawColor(uint32_t colorHex);
void G_DrawXYColor(uint32_t x, uint32_t y, uint32_t color);
uint32_t G_ColorFromZ(float depth, uint32_t color);
void G_CapFrameRate(uint32_t deltaTime);
void G_SortFacesByZ(TransformedModelFace_t* model, uint32_t cnt);
void G_DrawVertex(vec2_t v, uint32_t size);
void G_ClipFaceZ(vec3_t* v1, vec3_t* v2, vec3_t* v3);
uint32_t G_LightIntensity(uint32_t originalColor, float percentageFactor);
float G_CalcFaceIllumination(vec3_t face[3], vec3_t lightDir);
void G_InitZBuffer(int w, int h);
void G_ClearZBuffer();
void G_RenderZBuffer();
void G_Shutdown(void);
void G_Init(void);
void G_LoadModels(void);
void G_InitMatrices(matrices_t* matrices, Model_t* modelData);
int G_IsBackface(vec3_t triangle[3]); //0 = no, 1 = yes
void G_ClipFrustum(vec3_t triangleVerts[3], FaceTex_t* faceTexture, triangle_t* clippedTriangles, int* numClipped);
void G_CullFrustum(const triangle_t* clippedTriangles, const int numClippedTriangles, TransformedModelFace_t* frustumVisibleFaces, int* culledVecCnt);

#endif