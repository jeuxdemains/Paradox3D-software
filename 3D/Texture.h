#pragma once
#include <stdint.h>

typedef struct
{
	float u;
	float v;
} tex2_t;

extern int T_texWidth;
extern int T_texHeight;

extern const uint8_t REDBRICK_TEXTURE[];
extern uint32_t* T_meshTexture;

tex2_t T_Texture2Clone(tex2_t* tex);