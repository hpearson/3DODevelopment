#ifndef SPRITE_ENGINE_H
#define SPRITE_ENGINE_H

#include "types.h"
#include "main.h"

typedef struct sprite
{
    int width;
    int height;
    
    int posX, posY;
    int angle, zoom;

    CCB *cel;
    Point quad[4];
}sprite;

sprite *newSprite(int width, int height, ubyte *bmp);
void setPalette(sprite *spr, uint16* pal);
void setSpriteAlpha(sprite *spr, bool enabled);

void mapSprite(sprite *spr);
void mapZoomSprite(sprite *spr);
void mapZoomRotateSprite(sprite *spr);

int32 drawSprite(sprite *spr, Item screen);

#endif
