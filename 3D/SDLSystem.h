#pragma once
#include <SDL.h>
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>
#include "Vector.h"
#include "camera.h"

int G_debugEnableBackfaceCulling;
int G_debugInvertBackFaceCulling;
int G_debugStopRotation;
int G_debugRasterize;
int G_debugDrawVertices;
int G_debugDrawWireframe;
int G_debugRenderTextured;
int G_debugRenderZBuffer;

bool movingForward;
bool movingBack;
bool rotLeft;
bool rotRight;
bool movingUp;
bool movingDown;

uint32_t _ScreenW, _ScreenH;
uint32_t* screenBuffer;
SDL_Renderer* sdlSystemRenderer;
SDL_Window* sdlSystemWindow;
SDL_Texture* screenTexture;

void SDLSystemInit();
void SDLSystemShutdown();

SDL_Window* SDLSystemCreateWindow(uint32_t w, uint32_t h);
SDL_Renderer* SDLSystemCreateRenderer(SDL_Window* sdlWindow);
void SDLSystemRender();
int SDLHandleEvents(float deltaTime);

void DebugMessage(char* string);