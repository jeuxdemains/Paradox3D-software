#pragma once
#include <stdint.h>
#include "upng.h"

typedef struct
{
	float u;
	float v;
} tex2_t;

typedef struct
{
	uint32_t* textureData;
	uint32_t width;
	uint32_t height;
} texture_t;

//extern int T_texWidth;
//extern int T_texHeight;

//extern uint32_t* T_meshTexture;

tex2_t T_Texture2Clone(tex2_t* tex);
texture_t* T_LoadPngTexture(char* fileName);
void T_FreeResources();