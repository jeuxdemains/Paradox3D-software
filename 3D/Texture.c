#include "Texture.h"

int T_texWidth;
int T_texHeight;
uint32_t* T_meshTexture = NULL;
upng_t* pngTexture = NULL;

void T_LoadPngTexture(char* fileName)
{
    pngTexture = upng_new_from_file(fileName);
    if (pngTexture != NULL)
    {
        upng_decode(pngTexture);
        if (upng_get_error(pngTexture) == UPNG_EOK)
        {
            T_meshTexture = (uint32_t*)upng_get_buffer(pngTexture);
            T_texWidth = upng_get_width(pngTexture);
            T_texHeight = upng_get_height(pngTexture);
        }
    }
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