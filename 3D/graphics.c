#include "graphics.h"

model_list_t models[2] =
{ 
	{
		.modelName = "doom2m1.obj",
		.textureName = "doom2m1.png"
	},
	{
		.modelName = "model2.obj",
		.textureName = "texture2.png"
	}
};
int modelsCnt = 1;

matrices_t matrices =
{
	.rotMat = {0,0,0,0},
	.rotMatX = {0,0,0,0},
	.rotMatY = {0,0,0,0},
	.rotMatZ = {0,0,0,0},
	.scaleMat = {0,0,0,0},
	.transMat = {0,0,0,0},
	.viewMat = {0,0,0,0},
	.worldMatrix = {0,0,0,0}
};

Position_t worldPosition =
{
	.pos = {0,0,0}
};

Light_t light = { { 0.0f, 1.0f, 1.0f } };
uint32_t drawColor = 0xFFFFFFFF;
float vertDirCW = -1.0f; //1 = CW; -1 = CCW
float lightTheta = 0.0f;
float* zBuffer = NULL;
static uint32_t* screenBuffer = NULL;

float aspectx;
float aspecty;
float fovy;
float fovx;
float zNear;
float zFar;

mat4_t projMat;
TransformedModelFace_t frustumVisibleVerts[1804];

void G_InitZBuffer(int w, int h)
{
	zBuffer = (float*)malloc(sizeof(float) * w * h);
	G_ClearZBuffer();
}

void G_ClearZBuffer()
{
	for (uint32_t y = 0; y < GetScreenH(); y++)
	{
		for (uint32_t x = 0; x < GetScreenW(); x++)
		{
			zBuffer[(GetScreenW() * y) + x] = 1.0f;
		}
	}
}

void G_LoadModels()
{
	for (int i = 0; i < modelsCnt; i++)
	{
		//Open model 
		Model_t model;
		ModelInit(&model);
		OBJ_LoadModel(models[i].modelName, &model);
		ModelLoadPngTexture(models[i].textureName, &model);

		allModels[i] = model;
		allModelsCnt++;
	}
}

void G_Init(void)
{
	SDLSystemInit();
	uint32_t w = SCRN_W;
	uint32_t h = SCRN_H;
	SetScreenSize(w, h);
	int logicalScreenW = LOGICAL_SCRN_W;
	int logicalScreenH = LOGICAL_SCRN_H;
	SDL_Window* window = SDLSystemCreateWindow(logicalScreenW, logicalScreenH);
	SDL_Renderer* rend = SDLSystemCreateRenderer(window);

	G_InitZBuffer(w, h);
 	screenBuffer = GetScreenBuffer();

	//init aspect, fov, projection matrix
	aspectx = (float)w / (float)h;
	aspecty = (float)w / (float)h;
	fovy = 3.14159f / 3.0f; //PI / 3.0f = 1.047f (60 degrees in radians)
	fovx = atanf(tanf(fovy / 2) * aspectx) * 4.0f;
	zNear = 1.0f;
	zFar = 1000.0f;
	projMat = Mat4_MakePerspective(aspecty, fovy, zNear, zFar);

	//init frustum planes
	C_InitFrustumPlanes(fovx, fovy, zNear, zFar);

	//Load all models
	G_LoadModels();

	printf("Models loaded.\n");

}

void G_InitMatrices(matrices_t* matrices, Model_t* modelData)
{
	matrices->viewMat = C_GetCameraViewMat();
	matrices->scaleMat = Mat4_MakeScale(modelData->scale);
	matrices->transMat = Mat4_MakeTranslation(modelData->translation);
	matrices->rotMatX = Mat4_MakeRotationX(modelData->rotation.x);
	matrices->rotMatY = Mat4_MakeRotationX(modelData->rotation.y);
	matrices->rotMatZ = Mat4_MakeRotationZ(modelData->rotation.z);
}

int G_IsBackface(vec3_t triangle[3])
{
	if (G_debugEnableBackfaceCulling == 1)
	{
		vec3_t origin = { 0,0,0 };

		int isFrontFace = M3D_IsFrontFace(
			triangle[0],
			triangle[1],
			triangle[2],
			origin);

		if (isFrontFace == G_debugInvertBackFaceCulling)
		{
			return 1;
		}
	}

	return 0;
}

void G_ClipFrustum(vec3_t triangleVerts[3], FaceTex_t* faceTexture, triangle_t* clippedTriangles, int* numClipped)
{
	//CLIPPING
	//create polygon from transformed, clipped vertices
	polygon_t polygon = C_CreatePolyFromVertices(
		triangleVerts[0], triangleVerts[1], triangleVerts[2],
		faceTexture->a_uv, faceTexture->b_uv, faceTexture->c_uv
	);

	C_ClipPolygon(&polygon);

	//after clipping break poly to tris
	C_TrianglesFromPoly(&polygon, clippedTriangles, numClipped);
}

void G_CullFrustum(const triangle_t* clippedTriangles, const int numClippedTriangles, TransformedModelFace_t* frustumVisibleFaces, int* culledVecCnt)
{
	for (int idxVec = 0; idxVec < numClippedTriangles; idxVec++)
	{
		triangle_t tri = clippedTriangles[idxVec];

		//copy to transformed vertices array
		frustumVisibleVerts[*culledVecCnt].vertices[0] = tri.points[0];
		frustumVisibleVerts[*culledVecCnt].vertices[1] = tri.points[1];
		frustumVisibleVerts[*culledVecCnt].vertices[2] = tri.points[2];

		//copy texture coordinates
		frustumVisibleVerts[*culledVecCnt].texCrds[0] = tri.texcoord[0];
		frustumVisibleVerts[*culledVecCnt].texCrds[1] = tri.texcoord[1];
		frustumVisibleVerts[*culledVecCnt].texCrds[2] = tri.texcoord[2];

		//save average Z
		/*frustumVisibleVerts[*culledVecCnt].depth =
			(faceVertecies[0].z + faceVertecies[1].z + faceVertecies[2].z) / 3.0f;*/

		*culledVecCnt += 1;
	}
}

void G_RunRenderLoop()
{
	G_Init();

	/*printf("Rendering %d vertices with %d faces\n",
		modelData->vecCnt, modelData->facesCnt);*/

	float deltaTime = 0;
	uint32_t prevFrameTime = 0;

	while (SDLHandleEvents(deltaTime) == 0)
	{
		deltaTime = (SDL_GetTicks() - prevFrameTime) / 1000.0f;
		prevFrameTime = SDL_GetTicks();
		
		for (int i = 0; i < allModelsCnt; i++)
		{
			Model_t* modelData = &allModels[i];

			if (i == 1)
			{
				//modelData->rotation.x += 0.15f * deltaTime;
				//modelData->rotation.y += 0.15f * deltaTime;
				//modelData->rotation.z = -20.0f;

				//modelData->translation.y += 2.5f * sinf(lightTheta) * deltaTime;

				//camera.position.x = 0; //+= 0.008f * deltaTime;
				//camera.position.y = 0; //+= 0.008f * deltaTime;
				//lightTheta += 2.0f * deltaTime;
				//light.direction.x += 3.14f*2 * cosf(lightTheta) * deltaTime;
				//light.direction.y += 2.0f * deltaTime * cosf(lightTheta);
				//light.direction.z += 3.0f * cosf(lightTheta) * deltaTime;
			}

			modelData->scale.x = 2;
			modelData->scale.y = 2;
			modelData->scale.z = 2;
			
			//init rotation, translation, scale and view matrices
			G_InitMatrices(&matrices, modelData);

			vec3_t faceVertecies[3];
			int transformedVtxCnt = 0;

			//pseudo-update subroutine
			for (int i = 0; i < modelData->facesCnt; i += 1)
			{
				FaceTex_t faceTexture = modelData->faces[i];
				int a = modelData->faces[i].a;
				int b = modelData->faces[i].b;
				int c = modelData->faces[i].c;

				faceVertecies[0] = modelData->vertices[a];
				faceVertecies[1] = modelData->vertices[b];
				faceVertecies[2] = modelData->vertices[c];

				//3D TRANSFORM (scale, rotate, translate)
				M3D_Transform(&matrices, faceVertecies);

				//BACK-FACE CULLING
				if (G_IsBackface(faceVertecies) == 1) continue;

				//CLIPPING
				triangle_t tris_after_clipping[MAX_NUM_POLY_TRIS];
				int num_tris_after_clipping = 0;
				G_ClipFrustum(faceVertecies, &faceTexture, tris_after_clipping, &num_tris_after_clipping);

				//CULLING (collect only visible vertices after clipping)
				G_CullFrustum(tris_after_clipping, num_tris_after_clipping, frustumVisibleVerts, &transformedVtxCnt);
			}

			//Not needed if Z-Buffer is implemented
			//G_SortFacesByZ(transformed, transformedVtxCnt);

			//PROJECT
			for (int i = 0; i < transformedVtxCnt; i++)
			{
				//convert to quaternions to mul with matrices
				vec4_t v4d1 = M_Vec4FromVec3(frustumVisibleVerts[i].vertices[0]);
				vec4_t v4d2 = M_Vec4FromVec3(frustumVisibleVerts[i].vertices[1]);
				vec4_t v4d3 = M_Vec4FromVec3(frustumVisibleVerts[i].vertices[2]);

				//multiply by the projection matrix
				v4d1 = Mat4_MulVec4ProjectionMat4(v4d1, projMat);
				v4d2 = Mat4_MulVec4ProjectionMat4(v4d2, projMat);
				v4d3 = Mat4_MulVec4ProjectionMat4(v4d3, projMat);

				//convert to screen-space 2d vectors
				vec2_t v2d1 = M_Vec2FromVec4(v4d1);
				vec2_t v2d2 = M_Vec2FromVec4(v4d2);
				vec2_t v2d3 = M_Vec2FromVec4(v4d3);

				//screen-space scale
				M_Vec2ScaleFace(
					&v2d1, &v2d2, &v2d3,
					GetScreenH() / 2.0f);

				//screen-space translate
				M_Vec2TranslateFace(
					&v2d1, &v2d2, &v2d3,
					GetScreenW() / 2.0f, GetScreenH() / 2.0f);

				//calc light intensity
				//float lightPerc = G_CalcFaceIllumination(transformed[i].vertices, light.direction);
				float lightPerc = G_CalcFaceIllumination(frustumVisibleVerts[i].vertices, light.direction);

				if (G_debugSlowRendering)
				{
					//wireframe
					if (G_debugDrawWireframe)
					{
						uint32_t color = 0x0055FFFF; // G_LightIntensity(0x0055FFFF, lightPerc);
						G_SetDrawColor(color);
						G_DrawLine(v2d1, v2d2);
						SDLSystemRender();
						SDL_Delay(SLOW_REND_DELAY);
						G_DrawLine(v2d2, v2d3);
						SDLSystemRender();
						SDL_Delay(SLOW_REND_DELAY);
						G_DrawLine(v2d3, v2d1);
						SDLSystemRender();
						SDL_Delay(SLOW_REND_DELAY);
					}

					//vertices
					if (G_debugDrawVertices)
					{
						//color = G_ColorFromZ(maxZVert, 0xFF, 0x00, 0x00);
						G_SetDrawColor(0xFF0000FF);
						G_DrawVertex(v2d1, 2);
						G_DrawVertex(v2d2, 2);
						G_DrawVertex(v2d3, 2);
					}
				}
				

				//rasterize
				if (G_debugRasterize && !G_debugRenderTextured)
				{
					vec4_t v1 = M_Vec4FromVec2(v2d1);
					vec4_t v2 = M_Vec4FromVec2(v2d2);
					vec4_t v3 = M_Vec4FromVec2(v2d3);

					v1.z = v4d1.z;
					v1.w = v4d1.w;
					v2.z = v4d2.z;
					v2.w = v4d2.w;
					v3.z = v4d3.z;
					v3.w = v4d3.w;

					G_RenderTexturedTriangle(
						v1, v2, v3,
						frustumVisibleVerts[i].texCrds[0],
						frustumVisibleVerts[i].texCrds[1],
						frustumVisibleVerts[i].texCrds[2],
						0, lightPerc,
						1, 0xFFFFFFFF);
				}

				//texture map
				if (G_debugRenderTextured)
				{
					vec4_t v1 = M_Vec4FromVec2(v2d1);
					vec4_t v2 = M_Vec4FromVec2(v2d2);
					vec4_t v3 = M_Vec4FromVec2(v2d3);

					v1.z = v4d1.z;
					v1.w = v4d1.w;
					v2.z = v4d2.z;
					v2.w = v4d2.w;
					v3.z = v4d3.z;
					v3.w = v4d3.w;

					G_RenderTexturedTriangle(
						v1, v2, v3,
						frustumVisibleVerts[i].texCrds[0],
						frustumVisibleVerts[i].texCrds[1],
						frustumVisibleVerts[i].texCrds[2],
						modelData, lightPerc,
						0, 0);
				}


				//wireframe
				if (G_debugDrawWireframe)
				{
					uint32_t color = 0x0055FFFF; // G_LightIntensity(0x0055FFFF, lightPerc);
					G_SetDrawColor(color);
					G_DrawLine(v2d1, v2d2);
					G_DrawLine(v2d2, v2d3);
					G_DrawLine(v2d3, v2d1);
				}

				//vertices
				if (G_debugDrawVertices)
				{
					//color = G_ColorFromZ(maxZVert, 0xFF, 0x00, 0x00);
					G_SetDrawColor(0xFF0000FF);
					G_DrawVertex(v2d1, 2);
					G_DrawVertex(v2d2, 2);
					G_DrawVertex(v2d3, 2);
				}
			}
		}
		if (G_debugRenderZBuffer)
			G_RenderZBuffer();

		SDLSystemRender();
		G_ClearBuffer();
		G_ClearZBuffer();

		G_debugSlowRendering = 0;

		//G_CapFrameRate(deltaTime);
	}

	G_Shutdown();
}

void G_Shutdown(void)
{
	T_FreeResources();
	free(zBuffer);
	SDLSystemShutdown();
}

void G_RenderZBuffer()
{
	
	for (uint32_t y = 0; y < GetScreenH(); y++)
	{
		for (uint32_t x = 0; x < GetScreenW(); x++)
		{
			float zBuff = zBuffer[(GetScreenW() * y) + x];
			uint32_t r = (0xFFFFFFFF & 0xFF000000) * (-zBuff);
			uint32_t g = (0xFFFFFFFF & 0x00FF0000) * (-zBuff * 5);
			uint32_t b = (0xFFFFFFFF & 0x0000FF00) * (-zBuff * 3);
			uint32_t a = 255;

			uint32_t clr = (r & 0xFF000000) | (g & 0x00FF0000) | (b & 0x0000FF00) | a;

			screenBuffer[(GetScreenW() * y) + x] = clr;
		}
	}
}

float G_CalcFaceIllumination(vec3_t face[3], vec3_t lightDir)
{
	vec3_t normal = M_NormalVec3(face[0], face[1], face[2]);
	normal = M_NormalizeVec3(normal);

	float lightPerc = vertDirCW * M_DotVec3(normal, light.direction);
	if (lightPerc < 0.3f)
		lightPerc = 0.3f;
	if (lightPerc > 0.9f)
		lightPerc = 0.9f;

	return lightPerc;
}

uint32_t G_LightIntensity(uint32_t originalColor, float percentageFactor)
{
	uint32_t r = (originalColor & 0xFF000000) * percentageFactor;
	uint32_t g = (originalColor & 0x00FF0000) * percentageFactor;
	uint32_t b = (originalColor & 0x0000FF00) * percentageFactor;
	uint32_t a = (originalColor & 0x000000FF) * percentageFactor;

	uint32_t newColor = (r & 0xFF000000) | (g & 0x00FF0000) | (b & 0x0000FF00) | (a & 0x000000FF);

	return newColor;
}

void G_ClipFaceZ(vec3_t* v1, vec3_t* v2, vec3_t* v3)
{
	if (v1->z < 0.01f)
		v1->z = 0.01f;
	if (v2->z < 0.01f)
		v2->z = 0.01f;
	if (v3->z < 0.01f)
		v3->z = 0.01f;
}

void G_DrawVertex(vec2_t v, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		for (uint32_t j = 0; j < size; j++)
		{
			vec2_t v2 = v;
			v2.x += i;
			v2.y += j;
			G_DrawPoint(v2);
		}
	}
}

void G_CapFrameRate(uint32_t deltaTime)
{
	double frameTime = 1000.0 / FPS_TARGET;
	if (deltaTime < frameTime)
		SDL_Delay((uint32_t)frameTime - deltaTime);
}

void G_SortFacesByZ(TransformedModelFace_t* model, uint32_t cnt)
{
	for (uint32_t i = 0; i < cnt; i++)
	{
		for (uint32_t j = i; j < cnt; j++)
		{
			if (model[i].depth < model[j].depth)
			{
				TransformedModelFace_t tmp;
				tmp.vertices[0] = model[i].vertices[0];
				tmp.vertices[1] = model[i].vertices[1];
				tmp.vertices[2] = model[i].vertices[2];
				tmp.texCrds[0] = model[i].texCrds[0];
				tmp.texCrds[1] = model[i].texCrds[1];
				tmp.texCrds[2] = model[i].texCrds[2];
				float tmpDepth = model[i].depth;

				model[i].vertices[0] = model[j].vertices[0];
				model[i].vertices[1] = model[j].vertices[1];
				model[i].vertices[2] = model[j].vertices[2];
				model[i].texCrds[0] = model[j].texCrds[0];
				model[i].texCrds[1] = model[j].texCrds[1];
				model[i].texCrds[2] = model[j].texCrds[2];

				model[i].depth = model[j].depth;

				model[j].vertices[0] = tmp.vertices[0];
				model[j].vertices[1] = tmp.vertices[1];
				model[j].vertices[2] = tmp.vertices[2];
				model[j].texCrds[0] = tmp.texCrds[0];
				model[j].texCrds[1] = tmp.texCrds[1];
				model[j].texCrds[2] = tmp.texCrds[2];
				model[j].depth = tmpDepth;
			}
		}
	}
}

uint32_t G_ColorFromZ(float depth, uint32_t color)
{
	uint8_t* c = (uint8_t*)&color;
	uint8_t r = c[3];
	uint8_t g = c[2];
	uint8_t b = c[1];
	uint8_t a = c[0];

	float factor = depth * 13.0f;
	if (r > 0)
		r -= factor;

	if (b > 0)
		b -= factor;

	if (g > 0)
		g -= factor;

	uint32_t clr = r << 24 | g << 16 | b << 8 | 0xFF; //(uint32_t)((float)color - (v.z*50));

	return clr;
}

void G_SetDrawColor(uint32_t colorHex)
{
	drawColor = colorHex;
}

void G_DrawPoint(vec2_t v)
{
	if (v.x < 0.0 || v.x >= GetScreenW() || v.y < 0.0 || v.y >= GetScreenH())
		return;

	if (GetScreenW() * (uint32_t)v.y + (uint32_t)v.x > GetScreenW() * GetScreenH())
		return;

	screenBuffer[GetScreenW() * (uint32_t)v.y + (uint32_t)v.x] = drawColor;
}

void G_DrawPointI(uint32_t x, uint32_t y)
{
	if (x < 0 || x >= GetScreenW() || y < 0 || y >= GetScreenH())
		return;

	if (GetScreenW() * (uint32_t)y + (uint32_t)x > GetScreenW() * GetScreenH())
		return;

	screenBuffer[GetScreenW() * (uint32_t)y + (uint32_t)x] = drawColor;
}

void G_DrawPointColor(vec2_t v, uint32_t color)
{
	if (v.x < 0 || v.x > GetScreenW() || v.y < 0 || v.y > GetScreenH())
		return;

	if (GetScreenW() * (uint32_t)v.y + (uint32_t)v.x > GetScreenW() * GetScreenH())
		return;

	screenBuffer[GetScreenW() * (uint32_t)v.y + (uint32_t)v.x] = color;
}

void G_DrawXYColor(uint32_t x, uint32_t y, uint32_t color)
{
	if (x < 0 || y > GetScreenW() || x < 0 || y > GetScreenH())
		return;

	if (GetScreenW() * (uint32_t)y + (uint32_t)x > GetScreenW() * GetScreenH())
		return;

	screenBuffer[GetScreenW() * (uint32_t)y + (uint32_t)x] = color;
}

void G_DrawLineXY2(int x0, int y0, int x1, int y1)
{
	int dx = (x1 - x0);
	int dy = (y1 - y0);

	int longestSideLen = (abs(dx) >= abs(dy)) ? abs(dx) : abs(dy);

	float xInc = dx / (float)longestSideLen;
	float yInc = dy / (float)longestSideLen;

	float curX = (float)x0;
	float curY = (float)y0;

	for (int i = 0; i <= longestSideLen; i++)
	{
		vec2_t point;
		point.x = roundf(curX);
		point.y = roundf(curY);
		G_DrawPoint(point);

		curX += xInc;
		curY += yInc;
	}
}

void G_DrawLine(vec2_t p1, vec2_t p2)
{
	vec2i_t pi1 = { (int)p1.x, (int)p1.y };
	vec2i_t pi2 = { (int)p2.x, (int)p2.y };
	G_DrawLineI(pi1, pi2);
}

void G_DrawLineI(vec2i_t p1, vec2i_t p2)
{
	int x0 = p1.x, 
		y0 = p1.y, 
		x1 = p2.x, 
		y1 = p2.y;

	int dx;
	if (x1 > x0)
		dx = x1 - x0;
	else
		dx = x0 - x1;

	int dy;
	if (y1 > y0)
		dy = y1 - y0;
	else
		dy = y0 - y1;

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	for (;;)
	{
		G_DrawPointI(x0, y0);
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void G_ClearBuffer()
{
	memset(screenBuffer, 0, sizeof(uint32_t) * GetScreenH() * GetScreenW());
}

//p3 = midPoint
void G_RasterTopHalfTriangle(vec2_t p1, vec2_t p2, vec2_t p3)
{
	float inv_slope1 = (float)((int)p2.x - (int)p1.x) / ((int)p2.y - (int)p1.y);
	float inv_slope2 = (float)((int)p3.x - (int)p1.x) / ((int)p3.y - (int)p1.y);
	float x_start = (int)p1.x;
	float x_end = (int)p1.x;

	//TOP to BOTTOM
	for (int y = (int)p1.y; y <= (int)p3.y; y++)
	{
		vec2i_t a, b;
		a.x = (int)x_start;
		a.y = y;
		b.x = (int)x_end;
		b.y = y;

		G_DrawLineI(a, b);

		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}

//p2 = midPoint
void G_RasterBottomHalfTriangle(vec2_t p1, vec2_t p2, vec2_t p3)
{
	//Draw bottom to top
	float inv_slope1 = (float)((int)p3.x - (int)p1.x) / ((int)p3.y - (int)p1.y);
	float inv_slope2 = (float)((int)p3.x - (int)p2.x) / ((int)p3.y - (int)p2.y);
	float x_start = (int)p3.x;
	float x_end = (int)p3.x;

	//BOTTOM to TOP
	for (int y = (int)p3.y; y >= (int)p1.y; y--)
	{
		vec2i_t a, b;
		a.x = (int)x_start;
		a.y = y;
		b.x = (int)x_end;
		b.y = y;

		G_DrawLineI(a, b);

		x_start -= inv_slope1;
		x_end -= inv_slope2;
	}
}

/* 
                   p1
				  (x,y)
			       /|
			      / |
			     /  |
                /   |
               /    |
              /     |      <---- flat bottom triangle
             /      |
            /       |
           /        |
          /         |
	     /          |
		/  FLAT BOT |
   p2  /------------+ midPoint
 (x,y) \_  FLAT TOP | (Mx, My)
	     \_         |
		   \_       |
		     \_     |      <---- flat top triangle
			   \_   |
			     \_ |
				   \|
				    p3
				   (x,y)
*/

void G_RasterTriangle(vec2_t p1, vec2_t p2, vec2_t p3)
{
	M3D_SortTrianglePointsY(&p1, &p2, &p3);

	int y1 = (int)p1.y;
	int y2 = (int)p2.y;
	int y3 = (int)p3.y;

	if (y2 == y3)
		G_RasterTopHalfTriangle(p1, p2, p3);
	else if (y1 == y2)
		G_RasterBottomHalfTriangle(p1, p2, p3);
	else
	{
		vec2_t midPoint = M3D_CalcTriangleMidPoint(p1, p2, p3);
		G_RasterTopHalfTriangle(p1, p2, midPoint);
		G_RasterBottomHalfTriangle(p2, midPoint, p3);
	}
}

void G_RenderTexturedTriangle(
	vec4_t p1, vec4_t p2, vec4_t p3,
	tex2_t tp1, tex2_t tp2, tex2_t tp3, 
	Model_t* model, float lightPrecFactor,
	int isSolidColor, uint32_t color)
{
	M3D_Vec4SortTexturedTrianglePointsY(&p1, &p2, &p3, &tp1, &tp2, &tp3);

	// Flip the V component to account for inverted UV-coordinates (V grows downwards)
	tp1.v = 1.0f - tp1.v;
	tp2.v = 1.0f - tp2.v;
	tp3.v = 1.0f - tp3.v;

	int x0 = (int)p1.x;
	int y0 = (int)p1.y;
	int x1 = (int)p2.x;
	int y1 = (int)p2.y;
	int x2 = (int)p3.x;
	int y2 = (int)p3.y;

	//render top triangle
	float inv_slope1 = 0;
	float inv_slope2 = 0;

	if (y1 - y0 != 0) 
		inv_slope1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) 
		inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0)
	{
		for (int y = y0; y < y1; y++)
		{
			int x_start = x1 + (y - y1) * inv_slope1;
			int x_end = x0 + (y - y0) * inv_slope2;

			if (x_end < x_start)
				M3D_SwapInt(&x_start, &x_end);

			for (int x = x_start; x < x_end; x++)
			{
				if (isSolidColor)
				{
					G_DrawPixel(
						x, y, color,
						p1, p2, p3,
						tp1, tp2, tp3,
						lightPrecFactor
					);
				}
				else
				{
					G_DrawTexel(
						x, y, model,
						p1, p2, p3,
						tp1, tp2, tp3,
						lightPrecFactor
					);
				}
			}

			if (G_debugSlowRendering)
			{
				SDLSystemRender();
				SDL_Delay(SLOW_REND_DELAY);
			}
		}
	}
	
	//render bottom triangle
	inv_slope1 = 0;
	inv_slope2 = 0;

	if (y2 - y1 != 0)
		inv_slope1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0)
		inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0)
	{
		for (int y = y1; y < y2; y++)
		{
			int x_start = x1 + (y - y1) * inv_slope1;
			int x_end = x0 + (y - y0) * inv_slope2;

			if (x_end < x_start)
				M3D_SwapInt(&x_start, &x_end);

			for (int x = x_start; x < x_end; x++)
			{
				if (isSolidColor)
				{
					G_DrawPixel(
						x, y, color,
						p1, p2, p3,
						tp1, tp2, tp3,
						lightPrecFactor
					);
				}
				else
				{
					G_DrawTexel(
						x, y, model,
						p1, p2, p3,
						tp1, tp2, tp3,
						lightPrecFactor
					);
				}
			}

			if (G_debugSlowRendering)
			{
				SDLSystemRender();
				SDL_Delay(SLOW_REND_DELAY);
			}
		}
	}
}

void G_DrawTexel(int x, int y, Model_t* model,
	vec4_t point_a, vec4_t point_b, vec4_t point_c,
	tex2_t uv_a, tex2_t uv_b, tex2_t uv_c, 
	float lightPercFactor)
{
	vec2_t p = { x, y };
	vec2_t a = M_Vec2FromVec4(point_a);
	vec2_t b = M_Vec2FromVec4(point_b);
	vec2_t c = M_Vec2FromVec4(point_c);

	vec3_t bWeights = M3D_BarycentricWeights(a, b, c, p);

	float alpha = bWeights.x;
	float beta = bWeights.y;
	float gamma = bWeights.z;

	//perform barycentric weights U V interpolation
	float interp_u = (uv_a.u / point_a.w) * alpha + (uv_b.u / point_b.w) * beta + (uv_c.u / point_c.w) * gamma;
	float interp_v = (uv_a.v / point_a.w) * alpha + (uv_b.v / point_b.w) * beta + (uv_c.v / point_c.w) * gamma;

	float interpolated_reciprocal_w = (1.0f / point_a.w) * alpha + (1.0f / point_b.w) * beta + (1.0f / point_c.w) * gamma;
	interp_u /= interpolated_reciprocal_w;
	interp_v /= interpolated_reciprocal_w;

	//get pixel offset within the texture
	int tex_x = abs((int)(interp_u * model->textureW)) % model->textureW;
	int tex_y = abs((int)(interp_v * model->textureH)) % model->textureH;

	interpolated_reciprocal_w = 1.0f - interpolated_reciprocal_w;

	if (y < 0 || y >= (int)GetScreenH() || x < 0 || x >= (int)GetScreenW())
		return;

	if (interpolated_reciprocal_w < zBuffer[(GetScreenW() * y) + x])
	{
		uint32_t color = G_LightIntensity(
			model->texture[(model->textureW * tex_y) + tex_x], 
			lightPercFactor);

		G_DrawXYColor(x, y, color);

		//update the Z-Buffer
		zBuffer[(GetScreenW() * y) + x] = interpolated_reciprocal_w;
	}
}

void G_DrawPixel(int x, int y, uint32_t color,
	vec4_t point_a, vec4_t point_b, vec4_t point_c,
	tex2_t uv_a, tex2_t uv_b, tex2_t uv_c,
	float lightPercFactor)
{
	vec2_t p = { (float)x, (float)y };
	vec2_t a = M_Vec2FromVec4(point_a);
	vec2_t b = M_Vec2FromVec4(point_b);
	vec2_t c = M_Vec2FromVec4(point_c);

	vec3_t bWeights = M3D_BarycentricWeights(a, b, c, p);

	float alpha = bWeights.x;
	float beta = bWeights.y;
	float gamma = bWeights.z;

	//perform barycentric weights U V interpolation
	float interp_u = (uv_a.u / point_a.w) * alpha + (uv_b.u / point_b.w) * beta + (uv_c.u / point_c.w) * gamma;
	float interp_v = (uv_a.v / point_a.w) * alpha + (uv_b.v / point_b.w) * beta + (uv_c.v / point_c.w) * gamma;

	float interpolated_reciprocal_w = (1.0f / point_a.w) * alpha + (1.0f / point_b.w) * beta + (1.0f / point_c.w) * gamma;
	interp_u /= interpolated_reciprocal_w;
	interp_v /= interpolated_reciprocal_w;

	//get pixel offset within the texture
	int tex_x = abs((int)(interp_u * 64)) % 64;
	int tex_y = abs((int)(interp_v * 64)) % 64;

	interpolated_reciprocal_w = 1.0f - interpolated_reciprocal_w;

	if (y < 0 || y >= (int)GetScreenH() || x < 0 || x >= (int)GetScreenW())
		return;

	if (interpolated_reciprocal_w < zBuffer[(GetScreenW() * y) + x])
	{
		color = G_LightIntensity(color, lightPercFactor);
		G_DrawXYColor(x, y, color);

		//update the Z-Buffer
		zBuffer[(GetScreenW() * y) + x] = interpolated_reciprocal_w;
	}
}
