#include "Texture.h"
#include "Model.h"

//int T_texWidth;
//int T_texHeight;
//uint32_t* T_meshTexture = NULL;
upng_t* pngTexture = NULL;

texture_t* T_LoadPngTexture(char* fileName)
{
    texture_t* tex = (texture_t*)malloc(sizeof(texture_t));

    pngTexture = upng_new_from_file(fileName);
    if (pngTexture != NULL)
    {
        upng_decode(pngTexture);
        if (upng_get_error(pngTexture) == UPNG_EOK)
        {
            tex->textureData = (uint32_t*)upng_get_buffer(pngTexture);
            tex->width = upng_get_width(pngTexture);
            tex->height = upng_get_height(pngTexture);
        }
    }

    return tex;
}



void T_FreeResources()
{
    upng_free(pngTexture);
}

tex2_t T_Texture2Clone(tex2_t* tex)
{
    tex2_t t;
    t.u = tex->u;
    t.v = tex->v;

    return t;
}