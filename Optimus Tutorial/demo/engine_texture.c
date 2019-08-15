#include "types.h"
#include "main.h"
#include "tools.h"
#include "engine_texture.h"
#include "mathutil.h"

texture *textures[TEXTURE_NUM];

uint16 flatPal[512];

texture* initTexture(int width, int height, uint16 *pal, int palShadesNum, int type)
{
    int i, x, y;
	int size = width * height;

    texture *tex = (texture*)malloc(sizeof(texture));

    tex->width = width;
    tex->height = height;
    tex->bitmap = (ubyte*)malloc(size * sizeof(ubyte));
    tex->palette = pal;
	tex->palShadesNum = palShadesNum;

    switch(type)
    {
        case TEXTURE_FLAT:
            for (i=0; i<size; i++)
                tex->bitmap[i] = 31;
        break;

		case TEXTURE_NOISE:
            for (i=0; i<size; i++)
                tex->bitmap[i] = rand();
        break;

        case TEXTURE_XOR:
            i = 0;
            for (y=0; y<height; y++)
            {
                for (x=0; x<width; x++)
                {
                    tex->bitmap[i++] = x ^ y;
                }
            }
        break;
    }

    return tex;
}

texture *getTexture(int textureNum)
{
    return textures[textureNum];
}

void initTextures()
{
    //setPal(0,31, 8,8,8, 255,255,255, flatPal);
	setPalWithFades(0,31, 8,8,8, 127,255,63, flatPal, 16, 8,8,8);

    textures[TEXTURE_FLAT] = initTexture(32, 32, flatPal, 16, TEXTURE_XOR);
	textures[TEXTURE_NOISE] = initTexture(8, 8, flatPal, 1, TEXTURE_NOISE);
    textures[TEXTURE_XOR] = initTexture(64, 64, flatPal, 1, TEXTURE_XOR);
}
