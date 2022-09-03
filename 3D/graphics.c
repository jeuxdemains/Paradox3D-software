#include "graphics.h"

G_debugEnableBackfaceCulling = 1;
G_debugInvertBackFaceCulling = 0; //1 = normal; -1 = inverted
G_debugDrawWireframe = 1;
G_debugRenderTextured = 1;

Vec3_t camera = { 0, 0, 0 };
Light_t light = { { 0.0, 0.0, 1 } };
uint32_t drawColor = 0xFFFFFFFF;
int vertDirCW = -1; //1 = CW; -1 = CCW

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

Mat4_t projMat;
TransformedModelFace_t transformed[1804];

void G_RunRenderLoop()
{
	SDLSystemInit();
	SDL_Window* window = SDLSystemCreateWindow(800, 600);
	SDL_Renderer* rend = SDLSystemCreateRenderer(window);

	//init aspect, fov, projection matrix
	float fov = 3.14159f / 3.0f; //PI / 3.0f = 1.047f (60 degrees in radians)
	float aspect = (float)_ScreenH / (float)_ScreenW;
	float zNear = 0.1f;
	float zFar = 100.0f;
	projMat = Mat4_MakePerspective(aspect, fov, zNear, zFar);

	//init texture
	T_meshTexture = (uint32_t*)REDBRICK_TEXTURE;
	T_texWidth = 64;
	T_texHeight = 64;

	//init model
	modelData.faces = modelCubeFaces; //&faces; // &carFaces;
	modelData.vertices = modelCube; //&model; // &carModel;
	modelData.facesCnt = (sizeof(modelCubeFaces)) / (sizeof(*modelCubeFaces));
	modelData.vecCnt = sizeof(modelCube) / sizeof(*modelCube);

	//init model
	//modelData.faces = faces; // &carFaces;
	//modelData.vertices = model; // &carModel;
	//modelData.facesCnt = sizeof(faces) / sizeof(*faces);
	//modelData.vecCnt = sizeof(model) / sizeof(*model);


	printf("Rendering %d vertices with %d faces\n",
		modelData.vecCnt, modelData.facesCnt);

	uint32_t deltaTime;

	while (SDLSystemShouldQuit() == 0)
	{
		deltaTime = SDL_GetTicks();

		if (G_debugStopRotation == 0)
		{
			modelData.rotation.x += 0.01f;
			modelData.rotation.y += 0.02f;
			modelData.rotation.z += 0.015f;
		}

		//zoom out the model
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
		int transformedVtxCnt = 0;

		for (uint32_t i = 0; i < modelData.facesCnt; i += 1)
		{
			FaceTex_t face = modelData.faces[i];
			int a = modelData.faces[i].a;
			int b = modelData.faces[i].b;
			int c = modelData.faces[i].c;

			faceVertecies[0] = modelData.vertices[a - 1];
			faceVertecies[1] = modelData.vertices[b - 1];
			faceVertecies[2] = modelData.vertices[c - 1];
			
			for (int j = 0; j < 3; j++)
			{
				Vec4_t v = M_Vec4FromVec3(faceVertecies[j]);

				Mat4_t worldMatrix = Mat4_MakeIdentity();
				Mat4_t rotMat = Mat4_Mul4Mat4(rotMatX, rotMatY, rotMatZ, Mat4_MakeIdentity());
				worldMatrix = Mat4_Mul4Mat4(scaleMat, rotMat, transMat, worldMatrix);
				v = Mat4_MulVec4(worldMatrix, v);

				faceVertecies[j] = M_Vec3FromVec4(v);
			}

			G_ClipFaceZ(&faceVertecies[0], &faceVertecies[1], &faceVertecies[2]);

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

			//copy texture coordinates
			transformed[transformedVtxCnt].texCrds[0] = face.a_uv;
			transformed[transformedVtxCnt].texCrds[1] = face.b_uv;
			transformed[transformedVtxCnt].texCrds[2] = face.c_uv;

			//save average Z
			transformed[transformedVtxCnt].depth = (
				faceVertecies[0].z + faceVertecies[1].z + faceVertecies[2].z) / 3.0f;

			transformedVtxCnt++;
		}

		G_SortFacesByZ(transformed, transformedVtxCnt);

		for (int i = 0; i < transformedVtxCnt; i++)
		{
			//convert to quaternions to mul with matrices
			Vec4_t v4d1 = M_Vec4FromVec3(transformed[i].vertices[0]);
			Vec4_t v4d2 = M_Vec4FromVec3(transformed[i].vertices[1]);
			Vec4_t v4d3 = M_Vec4FromVec3(transformed[i].vertices[2]);

			//multiply by the projection matrix
			v4d1 = Mat4_MulVec4ProjectionMat4(v4d1, projMat);
			v4d2 = Mat4_MulVec4ProjectionMat4(v4d2, projMat);
			v4d3 = Mat4_MulVec4ProjectionMat4(v4d3, projMat);

			//convert to screen-space 2d vectors
			Vec2_t v2d1 = M_Vec2FromVec4(v4d1);
			Vec2_t v2d2 = M_Vec2FromVec4(v4d2);
			Vec2_t v2d3 = M_Vec2FromVec4(v4d3);

			//screen-space scale
			M_Vec2ScaleFace(
				&v2d1, &v2d2, &v2d3,
				_ScreenH / 2.0f);

			//screen-space translate
			M_Vec2TranslateFace(
				&v2d1, &v2d2, &v2d3,
				_ScreenW / 2.0f, _ScreenH / 2.0f);

			//calc light intensity
			Vec3_t normal = M_NormalVec3(
				transformed[i].vertices[0],
				transformed[i].vertices[1],
				transformed[i].vertices[2]);

			float lightPerc = vertDirCW * M_DotVec3(normal, light.direction);
			if (lightPerc < 0.1f)
				lightPerc = 0.1f;
			if (lightPerc > 0.9f)
				lightPerc = 0.9f;

			//texture map
			if (G_debugRenderTextured)
			{
				G_RenderTexturedTriangle(v2d1, v2d2, v2d3, 
					transformed[i].texCrds[0],
					transformed[i].texCrds[1],
					transformed[i].texCrds[2], 
					T_meshTexture);
			}

			//rasterize
			if (G_debugRasterize)
			{
				uint32_t color = G_LightIntensity(0xFFFF00FF, lightPerc);
				G_SetDrawColor(color);
				G_RasterTriangle(v2d1, v2d2, v2d3);
			}
			
			//wireframe
			if (G_debugDrawWireframe)
			{
				uint32_t color = G_LightIntensity(0x0055FFFF, lightPerc);
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
		
		SDLSystemRender();
		G_ClearBuffer();

		deltaTime = SDL_GetTicks() - deltaTime;
		G_CapFrameRate(deltaTime);
	}

	SDLSystemShutdown();
}

uint32_t G_LightIntensity(uint32_t originalColor, float percentageFactor)
{
	uint32_t r = (originalColor & 0xFF000000) * percentageFactor;
	uint32_t g = (originalColor & 0x00FF0000) * percentageFactor;
	uint32_t b = (originalColor & 0x0000FF00) * percentageFactor;
	uint32_t a = (originalColor & 0x000000FF);

	uint32_t newColor = (r & 0xFF000000) | (g & 0x00FF0000) | (b & 0x0000FF00) | a;

	return newColor;
}

void G_ClipFaceZ(Vec3_t* v1, Vec3_t* v2, Vec3_t* v3)
{
	if (v1->z < 0.01f)
		v1->z = 0.01f;
	if (v2->z < 0.01f)
		v2->z = 0.01f;
	if (v3->z < 0.01f)
		v3->z = 0.01f;
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

void G_DrawPoint(Vec2_t v)
{
	if (v.x < 0.0 || v.x >= _ScreenW || v.y < 0.0 || v.y >= _ScreenH)
		return;

	if (_ScreenW * (uint32_t)v.y + (uint32_t)v.x > _ScreenW * _ScreenH)
		return;

	screenBuffer[_ScreenW * (uint32_t)v.y + (uint32_t)v.x] = drawColor;
}

void G_DrawPointI(int x, int y)
{
	if (x < 0 || x >= _ScreenW || y < 0 || y >= _ScreenH)
		return;

	if (_ScreenW * (uint32_t)y + (uint32_t)x > _ScreenW * _ScreenH)
		return;

	screenBuffer[_ScreenW * (uint32_t)y + (uint32_t)x] = drawColor;
}

void G_DrawPointColor(Vec2_t v, uint32_t color)
{
	if (v.x < 0 || v.x > _ScreenW || v.y < 0 || v.y > _ScreenH)
		return;

	if (_ScreenW * (uint32_t)v.y + (uint32_t)v.x > _ScreenW * _ScreenH)
		return;

	screenBuffer[_ScreenW * (uint32_t)v.y + (uint32_t)v.x] = color;
}

void G_DrawXYColor(int x, int y, uint32_t color)
{
	if (x < 0 || y > _ScreenW || x < 0 || y > _ScreenH)
		return;

	if (_ScreenW * (uint32_t)y + (uint32_t)x > _ScreenW * _ScreenH)
		return;

	screenBuffer[_ScreenW * (uint32_t)y + (uint32_t)x] = color;
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
	Vec2i_t pi1 = { (int)p1.x, (int)p1.y };
	Vec2i_t pi2 = { (int)p2.x, (int)p2.y };
	G_DrawLineI(pi1, pi2);
}

void G_DrawLineI(Vec2i_t p1, Vec2i_t p2)
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
	memset(screenBuffer, 0, sizeof(uint32_t) * _ScreenH * _ScreenW);
}

//p3 = midPoint
void G_RasterTopHalfTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3)
{
	float inv_slope1 = (float)((int)p2.x - (int)p1.x) / ((int)p2.y - (int)p1.y);
	float inv_slope2 = (float)((int)p3.x - (int)p1.x) / ((int)p3.y - (int)p1.y);
	float x_start = (int)p1.x;
	float x_end = (int)p1.x;

	//TOP to BOTTOM
	for (int y = (int)p1.y; y <= (int)p3.y; y++)
	{
		Vec2i_t a, b;
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
void G_RasterBottomHalfTriangle(Vec2_t p1, Vec2_t p2, Vec2_t p3)
{
	//Draw bottom to top
	float inv_slope1 = (float)((int)p3.x - (int)p1.x) / ((int)p3.y - (int)p1.y);
	float inv_slope2 = (float)((int)p3.x - (int)p2.x) / ((int)p3.y - (int)p2.y);
	float x_start = (int)p3.x;
	float x_end = (int)p3.x;

	//BOTTOM to TOP
	for (int y = (int)p3.y; y >= (int)p1.y; y--)
	{
		Vec2i_t a, b;
		a.x = x_start;
		a.y = y;
		b.x = x_end;
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

void G_RenderTexturedTriangle(
	Vec2_t p1, Vec2_t p2, Vec2_t p3, 
	Tex2_t tp1, Tex2_t tp2, Tex2_t tp3, 
	uint32_t* texture)
{
	M_SortTexturedTrianglePointsY(&p1, &p2, &p3, &tp1, &tp2, &tp3);

	int x0 = (int)p1.x;
	int y0 = (int)p1.y;
	int x1 = (int)p2.x;
	int y1 = (int)p2.y;
	int x2 = (int)p3.x;
	int y2 = (int)p3.y;

	Vec2_t point_a = { x0, y0 };
	Vec2_t point_b = { x1, y1 };
	Vec2_t point_c = { x2, y2 };


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
				M_SwapInt(&x_start, &x_end);

			for (int x = x_start; x < x_end; x++)
			{
				G_DrawTexel(
					x, y, texture,
					point_a, point_b, point_c,
					tp1, tp2, tp3
				);
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
				M_SwapInt(&x_start, &x_end);

			for (int x = x_start; x < x_end; x++)
			{
				//G_DrawXYColor(x, y, 0xFFAA00FF);
				G_DrawTexel(
					x, y, texture, 
					point_a, point_b, point_c,
					tp1, tp2, tp3
				);
			}
		}
	}
}

void G_DrawTexel(int x, int y, uint32_t* texture,
	Vec2_t a, Vec2_t b, Vec2_t c,
	Tex2_t uv_a, Tex2_t uv_b, Tex2_t uv_c)
{
	Vec2_t p = { x, y };
	Vec3_t bWeights = M_BarycentricWeights(a, b, c, p);

	float alpha = bWeights.x;
	float beta = bWeights.y;
	float gamma = bWeights.z;

	//perform barycentric weights U V interpolation
	float interp_u = uv_a.u * alpha + uv_b.u * beta + uv_c.u * gamma;
	float interp_v = uv_a.v * alpha + uv_b.v * beta + uv_c.v * gamma;

	//get pixel offset within the texture
	int tex_x = abs((int)(interp_u * T_texWidth));
	int tex_y = abs((int)(interp_v * T_texHeight));

	if ((T_texWidth * tex_y) + tex_x > 64 * 64 * sizeof(uint8_t))
		return;

	G_DrawXYColor(x, y, texture[(T_texWidth * tex_y) + tex_x]);
}