#include "graphics.h"

Vec3_t camera = { 0, 0, 0 };
float thetaX = 0.0f, thetaY = 0.0f, thetaZ = 0.0f;
uint32_t drawColor = 0xFFFFFFFF;
G_debugEnableBackfaceCulling = 1;
G_debugInvertBackFaceCulling = 0;
G_debugDrawWireframe = 1;

Model_t modelData =
{
	.faces = NULL,
	.facesCnt = 0,
	.modelPosition = 0.0f,
	.vecCnt = 0,
	.vertices = NULL,
	.rotation = {0,0,0},
	.translation = {0,0,0},
	.scale = {1.0f, 1.0f, 1.0f}
};

void G_RunRenderLoop()
{
	SDLSystemInit();
	SDL_Window* window = SDLSystemCreateWindow(800, 600);
	SDL_Renderer* rend = SDLSystemCreateRenderer(window);

	//modelData.faces = &modelCubeFaces; //&faces; // &carFaces;
	//modelData.vertices = &modelCube; //&model; // &carModel;
	//modelData.facesCnt = sizeof(modelCubeFaces) / sizeof(*modelCubeFaces);
	//modelData.vecCnt = sizeof(modelCube) / sizeof(*modelCube);

	modelData.faces = faces; // &carFaces;
	modelData.vertices = model; // &carModel;
	modelData.facesCnt = sizeof(faces) / sizeof(*faces);
	modelData.vecCnt = sizeof(model) / sizeof(*model);

	TransformedModelFace_t transformed[1804];

	printf("Rendering %d vertices with %d faces\n",
		modelData.vecCnt, modelData.facesCnt);

	uint32_t deltaTime;
	
	while (SDLSystemShouldQuit() == 0)
	{
		deltaTime = SDL_GetTicks();
		int transformedVtxCnt = 0;

		if (G_debugStopRotation == 0)
		{
			modelData.rotation.x += 0.01f;
			modelData.rotation.y += 0.005f;
			modelData.rotation.z += 0.015f;
		}

		//modelData.translation.x += 0.01f;
		modelData.translation.z = 8.0f;

		Mat4_t scaleMat = Mat4_MakeScale(
			modelData.scale.x,
			modelData.scale.y,
			modelData.scale.z
		);

		Mat4_t transMat = Mat4_MakeTranslation(
			modelData.translation.x,
			modelData.translation.y,
			modelData.translation.z
		);

		Mat4_t rotMatX = Mat4_MakeRotationX(modelData.rotation.x);
		Mat4_t rotMatY = Mat4_MakeRotationX(modelData.rotation.y);
		Mat4_t rotMatZ = Mat4_MakeRotationZ(modelData.rotation.z);

		Vec3_t faceVertecies[3];

		for (uint32_t i = 0; i < modelData.facesCnt; i += 1)
		{
			int a = (modelData.faces + FACE_POINTS_CNT * i)[0];
			int b = (modelData.faces + FACE_POINTS_CNT * i)[1];
			int c = (modelData.faces + FACE_POINTS_CNT * i)[2];

			faceVertecies[0] = modelData.vertices[a];
			faceVertecies[1] = modelData.vertices[b];
			faceVertecies[2] = modelData.vertices[c];

			for (int j = 0; j < 3; j++)
			{
				Vec4_t v = M_Vec4FromVec3(faceVertecies[j]);
				Mat4_t worldMatrix = Mat4_MakeIdentity();
				Mat4_t rotMat = Mat4_Mul4Mat4(rotMatX, rotMatY, rotMatZ, Mat4_MakeIdentity());
				worldMatrix = Mat4_Mul4Mat4(scaleMat, rotMat, transMat, worldMatrix);
				v = Mat4_MulVec4(worldMatrix, v);

				faceVertecies[j] = M_Vec3FromVec4(v);
			}

			faceVertecies[0] = M_TranslateVec3(faceVertecies[0], camera);
			faceVertecies[1] = M_TranslateVec3(faceVertecies[1], camera);
			faceVertecies[2] = M_TranslateVec3(faceVertecies[2], camera);

			if (G_debugEnableBackfaceCulling == 1)
			{
				if (M_IsFrontFace(
					faceVertecies[0],
					faceVertecies[1],
					faceVertecies[2],
					camera) == G_debugInvertBackFaceCulling)
				{
					continue;
				}
			}

			//copy to transformed vertices array
			transformed[transformedVtxCnt].vertices[0] = faceVertecies[0];
			transformed[transformedVtxCnt].vertices[1] = faceVertecies[1];
			transformed[transformedVtxCnt].vertices[2] = faceVertecies[2];

			//sort by Z
			transformed[transformedVtxCnt].depth = (
				faceVertecies[0].z + faceVertecies[1].z + faceVertecies[2].z) / 3.0f;

			transformedVtxCnt++;
		}

		G_SortFacesByZ(transformed, transformedVtxCnt);

		for (int i = 0; i < transformedVtxCnt; i++)
		{
			Vec2_t v2d1 = M_ProjectVec3(&transformed[i].vertices[0], _ScreenW, _ScreenH);
			Vec2_t v2d2 = M_ProjectVec3(&transformed[i].vertices[1], _ScreenW, _ScreenH);
			Vec2_t v2d3 = M_ProjectVec3(&transformed[i].vertices[2], _ScreenW, _ScreenH);

			//Vec3_t maxZVert = M_MaxZVec3(v1, v2, v3);
			//uint32_t color = G_ColorFromZ(maxZVert, 0xFF, 0xFF, 0xFF);

			//rasterize
			if (G_debugRasterize)
			{
				uint32_t color = G_ColorFromZ(transformed[i].depth, 0x00FFFFFF);
				G_SetDrawColor(color);
				G_RasterTriangle(v2d1, v2d2, v2d3);
			}
			
			//wireframe
			if (G_debugDrawWireframe)
			{
				G_SetDrawColor(0x0055FFFF);
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
		
		SDLSystemRender();
		G_ClearBuffer();

		deltaTime = SDL_GetTicks() - deltaTime;
		G_CapFrameRate(deltaTime);
	}

	SDLSystemShutdown();
}

void G_DrawVertex(Vec2_t v, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		for (uint32_t j = 0; j < size; j++)
		{
			Vec2_t v2 = v;
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
				Face_t tmp;
				tmp.face[0] = model[i].vertices[0];
				tmp.face[1] = model[i].vertices[1];
				tmp.face[2] = model[i].vertices[2];
				float tmpDepth = model[i].depth;

				model[i].vertices[0] = model[j].vertices[0];
				model[i].vertices[1] = model[j].vertices[1];
				model[i].vertices[2] = model[j].vertices[2];
				model[i].depth = model[j].depth;

				model[j].vertices[0] = tmp.face[0];
				model[j].vertices[1] = tmp.face[1];
				model[j].vertices[2] = tmp.face[2];
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

	if (r > 0)
		r -= depth * 17.0f;

	if (b > 0)
		b -= depth * 17.0f;

	if (g > 0)
		g -= depth * 17.0f;

	uint32_t clr = r << 24 | g << 16 | b << 8 | 0xFF; //(uint32_t)((float)color - (v.z*50));

	return clr;
}

void G_SetDrawColor(uint32_t colorHex)
{
	drawColor = colorHex;
}

void G_DrawPoint(Vec2_t v)
{
	if (v.x < 0.0 || v.x >= _ScreenW || v.y < 0.0 || v.y >= _ScreenH)
		return;

	if (_ScreenW * (uint32_t)v.y + (uint32_t)v.x > _ScreenW * _ScreenH)
		return;

	screenBuffer[_ScreenW * (uint32_t)v.y + (uint32_t)v.x] = drawColor;
}

void G_DrawPointColor(Vec2_t v, uint32_t color)
{
	if (v.x < 0 || v.x > _ScreenW || v.y < 0 || v.y > _ScreenH)
		return;

	if (_ScreenW * (uint32_t)v.y + (uint32_t)v.x > _ScreenW * _ScreenH)
		return;

	screenBuffer[_ScreenW * (uint32_t)v.y + (uint32_t)v.x] = color;
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
		Vec2_t point;
		point.x = roundf(curX);
		point.y = roundf(curY);
		G_DrawPoint(point);

		curX += xInc;
		curY += yInc;
	}
}

void G_DrawLine(Vec2_t p1, Vec2_t p2)
{
	int x0 = (int)p1.x, 
		y0 = (int)p1.y, 
		x1 = (int)p2.x, 
		y1 = (int)p2.y;

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
		Vec2_t p = { .x = (float)x0, .y = (float)y0 };
		G_DrawPoint(p);
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void G_ClearBuffer()
{
	memset(screenBuffer, 0, sizeof(uint32_t) * _ScreenH * _ScreenW);
}

//p3 = midPoint
void G_RasterTopHalfTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3)
{
	float inv_slope1 = (p2.x - p1.x) / (p2.y - p1.y);
	float inv_slope2 = (p3.x - p1.x) / (p3.y - p1.y);
	float x_start = p1.x;
	float x_end = p1.x;

	//TOP to BOTTOM
	for (int y = (int)ceil(p1.y); y <= (int)floor(p3.y); y++)
	{
		Vec2_t a, b;
		a.x = x_start;
		a.y = (float)y;
		b.x = x_end;
		b.y = (float)y;

		G_DrawLine(a, b);

		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}

//p2 = midPoint
void G_RasterBottomHalfTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3)
{
	//Draw bottom to top
	float inv_slope1 = (p3.x - p1.x) / (p3.y - p1.y);
	float inv_slope2 = (p3.x - p2.x) / (p3.y - p2.y);
	float x_start = p3.x;
	float x_end = p3.x;

	//BOTTOM to TOP
	for (int y = p3.y; y >= p1.y; y--)
	{
		Vec2_t a, b;
		a.x = x_start;
		a.y = (float)y;
		b.x = x_end;
		b.y = (float)y;

		G_DrawLine(a, b);

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

void G_RasterTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3)
{
	M_SortTrianglePointsY(&p1, &p2, &p3);

	int y1 = (int)p1.y;
	int y2 = (int)p2.y;
	int y3 = (int)p3.y;

	if (y2 == y3)
		G_RasterTopHalfTriangle(p1, p2, p3);
	else if (y1 == y2)
		G_RasterBottomHalfTriangle(p1, p2, p3);
	else
	{
		Vec2_t midPoint = M_CalcTriangleMidPoint(p1, p2, p3);
		G_RasterTopHalfTriangle(p1, p2, midPoint);
		G_RasterBottomHalfTriangle(p2, midPoint, p3);
	}
}

