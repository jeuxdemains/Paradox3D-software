#ifndef SDLSYSTEM_H
#define SDLSYSTEM_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef _WIN32
    #include <SDL.h>
#else
    #include <SDL2/SDL.h>
#endif

#include "Vector.h"
#include "camera.h"
#include "debug.h"
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

static bool movingForward;
static bool movingBack;
static bool rotLeft;
static bool rotRight;
static bool movingUp;
static bool movingDown;

void SDLSystemInit();
void SDLSystemShutdown();

SDL_Window *SDLSystemCreateWindow(uint32_t w, uint32_t h);
SDL_Renderer *SDLSystemCreateRenderer(SDL_Window *sdlWindow);
void SDLSystemRender();
int SDLHandleEvents(float deltaTime);
void DebugMessage(char *string);
uint32_t GetScreenH();
uint32_t GetScreenW();
void SetScreenSize(uint32_t width, uint32_t height);
uint32_t* GetScreenBuffer();
SDL_Renderer* GetSDLRenderer();
SDL_Window* GetSDLWindow();
SDL_Texture* GetSDLTexture();

#endif