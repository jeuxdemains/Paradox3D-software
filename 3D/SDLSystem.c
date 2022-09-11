#include "SDLSystem.h"

void SDLSystemInit()
{
	SDL_Init(SDL_INIT_VIDEO);
}

void SDLSystemShutdown()
{
	SDL_DestroyTexture(screenTexture);
	SDL_DestroyRenderer(sdlSystemRenderer);
	SDL_DestroyWindow(sdlSystemWindow);
	SDL_Quit();

	free(screenBuffer);
}

void SDLSystemInitScreenBuffer(uint32_t w, uint32_t h)
{
	uint32_t buffSize = sizeof(uint32_t) * (w * h);

	screenBuffer = (uint32_t*)malloc(buffSize);
	if (screenBuffer != NULL)
		memset(screenBuffer, 0, buffSize);
}

void SDLSystemCreateScreenTexture(uint32_t w, uint32_t h)
{
	screenTexture = SDL_CreateTexture(
		sdlSystemRenderer, SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_STREAMING, w, h);
}

SDL_Window* SDLSystemCreateWindow(uint32_t w, uint32_t h)
{
	_ScreenW = w;
	_ScreenH = h;
	SDLSystemInitScreenBuffer(w, h);
	sdlSystemWindow = SDL_CreateWindow(
		"Paradox 3D", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);

	return sdlSystemWindow;
}

SDL_Renderer* SDLSystemCreateRenderer(SDL_Window* sdlWindow)
{
	sdlSystemRenderer = SDL_CreateRenderer(
		sdlWindow, 0,
		SDL_RENDERER_SOFTWARE);
	SDLSystemCreateScreenTexture(_ScreenW, _ScreenH);

	return sdlSystemRenderer;
}

void HandleDebug(SDL_Event event)
{
	if (event.key.keysym.sym == SDLK_b)
	{
		G_debugEnableBackfaceCulling = !G_debugEnableBackfaceCulling;

		if (G_debugEnableBackfaceCulling == 0)
			DebugMessage("Back-face culling disabled");
		else
			DebugMessage("Back-face culling enabled");
	}

	if (event.key.keysym.sym == SDLK_r)
	{
		G_debugInvertBackFaceCulling = !G_debugInvertBackFaceCulling;

		if (G_debugInvertBackFaceCulling == 0)
			DebugMessage("Back-face culling set to CCW");
		else
			DebugMessage("Back-face culling set to CW");
	}

	if (event.key.keysym.sym == SDLK_SPACE)
		G_debugStopRotation = !G_debugStopRotation;

	if (event.key.keysym.sym == SDLK_1)
		G_debugRasterize = !G_debugRasterize;

	if (event.key.keysym.sym == SDLK_2)
		G_debugDrawVertices = !G_debugDrawVertices;

	if (event.key.keysym.sym == SDLK_3)
		G_debugDrawWireframe = !G_debugDrawWireframe;

	if (event.key.keysym.sym == SDLK_4)
		G_debugRenderTextured = !G_debugRenderTextured;

	if (event.key.keysym.sym == SDLK_5)
		G_debugRenderZBuffer = !G_debugRenderZBuffer;
}


void HandleCamera(SDL_Event event, float deltaTime)
{
	const float walkSpeed = 30.0f;
	const float rotSpeed = 8.0f;

	if (event.key.keysym.sym == SDLK_w)
	{
		camera.forwardVelocity = M_MulVec3Scalar(camera.direction, 20.0f * deltaTime);
		camera.position = M_AddVec3(camera.position, camera.forwardVelocity);
	}

	if (event.key.keysym.sym == SDLK_s)
	{
		camera.forwardVelocity = M_MulVec3Scalar(camera.direction, 20.0f * deltaTime);
		camera.position = M_SubVec3(camera.position, camera.forwardVelocity);
	}

	if (event.key.keysym.sym == SDLK_e)
	{
		camera.position.y -= walkSpeed * deltaTime;
	}

	if (event.key.keysym.sym == SDLK_c)
	{
		camera.position.y += walkSpeed * deltaTime;
	}

	if (event.key.keysym.sym == SDLK_a)
	{
		camera.yawAngle -= rotSpeed * deltaTime;
	}

	if (event.key.keysym.sym == SDLK_d)
	{
		camera.yawAngle += rotSpeed * deltaTime;
	}
}

int SDLHandleEvents(float deltaTime)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
		return 1;

	if (event.type == SDL_KEYDOWN)
	{
		HandleDebug(event);
		HandleCamera(event, deltaTime);
	}

	return 0;
}



void SDLSystemRender()
{
	SDL_UpdateTexture(
		screenTexture, NULL, screenBuffer,
		_ScreenW * sizeof(uint32_t));
	SDL_RenderCopy(
		sdlSystemRenderer, screenTexture,
		NULL, NULL);
	SDL_RenderPresent(sdlSystemRenderer);
}

void DebugMessage(char* string)
{
	printf("Debug: %s\n", string);
}