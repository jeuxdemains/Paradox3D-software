#include "Model.h"


void ModelInit(Model_t* model)
{
	model->scale = M_NewVec3(1.0f, 1.0f, 1.0f);
	model->rotation = M_NewVec3(0, 0, 0);
	model->translation = M_NewVec3(0, 0, 0);
	model->faces = NULL;
	model->facesCnt = 0;
	model->modelPosition = M_NewVec3(0, 0, 0);
	model->vecCnt = 0;
	model->vertices = NULL;
}

void ModelLoadPngTexture(char* fileName, Model_t* model)
{
    texture_t* tex = T_LoadPngTexture(fileName);
    model->texture = tex->textureData;
    model->textureW = tex->width;
    model->textureH = tex->height;

    //upng_t* pngTexture = upng_new_from_file(fileName);
    //if (pngTexture != NULL)
    //{
    //    upng_decode(pngTexture);
    //    if (upng_get_error(pngTexture) == UPNG_EOK)
    //    {
    //        model->texture = (uint32_t*)upng_get_buffer(pngTexture);
    //        model->textureW = upng_get_width(pngTexture);
    //        model->textureH = upng_get_height(pngTexture);
    //    }
    //}
}