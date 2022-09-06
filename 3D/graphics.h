#pragma once
#include "SDLSystem.h"
#include "Math3D.h"
#include "Model.h"
#include "Matrix.h"

#define FPS_TARGET 60

typedef struct
{
	Vec3_t direction;
} Light_t;

extern int G_debugEnableBackfaceCulling;
extern int G_debugInvertBackFaceCulling;
extern int G_debugStopRotation;
extern int G_debugRasterize;
extern int G_debugDrawVertices;
extern int G_debugDrawWireframe;
extern int G_debugRenderTextured;

extern Position_t worldPosition;

void G_RunRenderLoop();
void G_DrawPoint(Vec2_t v);
void G_DrawPointI(uint32_t x, uint32_t y);
void G_DrawPointColor(Vec2_t v, uint32_t color);
void G_DrawTexel(int x, int y, uint32_t* texture,
	Vec4_t a, Vec4_t b, Vec4_t c,
	Tex2_t uv_a, Tex2_t uv_b, Tex2_t uv_c,
	float lightPercFactor);
void G_DrawLine(Vec2_t p1, Vec2_t p2);
void G_DrawLineI(Vec2i_t p1, Vec2i_t p2);
void G_RasterTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3);
void G_RenderTexturedTriangle(
	Vec4_t p1, Vec4_t p2, Vec4_t p3,
	Tex2_t tp1, Tex2_t tp2, Tex2_t tp3,
	uint32_t* texture, float lightPrecFactor);

void G_ClearBuffer();
void G_SetDrawColor(uint32_t colorHex);
void G_DrawXYColor(int x, int y, uint32_t color);
uint32_t G_ColorFromZ(float depth, uint32_t color);
void G_CapFrameRate(uint32_t deltaTime);
void G_SortFacesByZ(TransformedModelFace_t* model, uint32_t cnt);
void G_DrawVertex(Vec2_t v, uint32_t size);
void G_ClipFaceZ(Vec3_t* v1, Vec3_t* v2, Vec3_t* v3);
uint32_t G_LightIntensity(uint32_t originalColor, float percentageFactor);
float G_CalcFaceIllumination(Vec3_t face[3], Vec3_t lightDir);
void G_InitZBuffer(int w, int h);
void G_ClearZBuffer();