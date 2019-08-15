#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_rotblobs.h"
#include "sprite_engine.h"

#define NUM_ROTBLOBS 64

sprite *rotblobs[NUM_ROTBLOBS];

ubyte rotblobbmp[ROTBLOB_SIZE];
uint16 rotblobpal[32];

void rotblobsInit()
{
    int x,y,i;
    int xc, yc;
    int c, r;

    i = 0;
    for (y=0; y<ROTBLOB_HEIGHT; y++)
    {
        yc = y - ROTBLOB_HEIGHT / 2;
        for (x=0; x<ROTBLOB_WIDTH; x++)
        {
            xc = x - ROTBLOB_WIDTH / 2;
            r = xc*xc + yc*yc + ((SinF16(3.0 * Atan2F16(xc, yc)) + 65536) >> 12);
            if (r==0) r = 1;
            c = 1024 / r;
            if (c > 31) c = 31;
            if (c < 8) c = 0;
            rotblobbmp[i++] = c;
        }
    }
    setPal(0,31, 0,0,0, 127, 191, 255, rotblobpal);

    for (i=0; i<NUM_ROTBLOBS; i++)
    {
        rotblobs[i] = newSprite(ROTBLOB_WIDTH, ROTBLOB_HEIGHT, rotblobbmp);
        //setBitmap(rotblobs[i], rotblobbmp);
        setPalette(rotblobs[i], rotblobpal);
        setSpriteAlpha(rotblobs[i], true);
        if (i > 0) LinkCel(rotblobs[i-1]->cel, rotblobs[i]->cel);
    }
}

void rotblobsRun(Item screen, uint32 time)
{
    int px, py;
    int i;

    for (i=0; i<NUM_ROTBLOBS; i++)
    {
        px = (SinF16((i*i * 2800 + 1640 * time) << 4) >> 10) + (SinF16((i*i * 2400 + 940 * time) << 4) >> 10);
        py = SinF16((i*i * 2600 + 2480 * time) << 4) >> 10;
        rotblobs[i]->posX = SCREEN_WIDTH / 2 + px; rotblobs[i]->posY = SCREEN_HEIGHT / 2 + py;
        rotblobs[i]->zoom = 512 + px + py;
        rotblobs[i]->angle = px << 18;
        mapZoomRotateSprite(rotblobs[i]);
    }

    drawSprite(rotblobs[0], screen);
}
