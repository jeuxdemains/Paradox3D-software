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

void G_RunRenderLoop();
void G_DrawPoint(Vec2_t v);
void G_DrawPointColor(Vec2_t v, uint32_t color);
void G_DrawLine(Vec2_t p1, Vec2_t p2);
void G_ClearBuffer();
void G_SetDrawColor(uint32_t colorHex);
uint32_t G_ColorFromZ(float depth, uint32_t color);
void G_CapFrameRate(uint32_t deltaTime);
void G_SortFacesByZ(TransformedModelFace_t* model, uint32_t cnt);
void G_DrawVertex(Vec2_t v, uint32_t size);
void G_ClipFaceZ(Vec3_t* v1, Vec3_t* v2, Vec3_t* v3);
uint32_t G_LightIntensity(uint32_t originalColor, float percentageFactor);