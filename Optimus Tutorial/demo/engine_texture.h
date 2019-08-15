#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

typedef struct texture
{
    int width, height;
    ubyte *bitmap;
    uint16 *palette;
	int palShadesNum;
}texture;

enum {TEXTURE_FLAT, TEXTURE_NOISE, TEXTURE_XOR, TEXTURE_NUM};

void initTextures();
texture *getTexture(int textureNum);

#endif
