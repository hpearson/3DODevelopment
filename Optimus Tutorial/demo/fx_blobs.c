#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_blobs.h"
#include "sprite_engine.h"

#define NUM_BLOBS 96

sprite *blobs[NUM_BLOBS];

ubyte blobbmp[BLOB_SIZE];
uint16 blobpal[32];

void blobsInit()
{
    int x,y,i;
    int xc, yc;
    int c, r;

    i = 0;
    for (y=0; y<BLOB_HEIGHT; y++)
    {
        yc = y - BLOB_HEIGHT / 2;
        for (x=0; x<BLOB_WIDTH; x++)
        {
            xc = x - BLOB_WIDTH / 2;
            r = xc*xc + yc*yc;
            if (r==0) r = 1;
            c = 1024 / r;
            if (c > 31) c = 31;
            if (c < 8) c = 0;
            blobbmp[i++] = c;
        }
    }
    setPal(0,31, 0,0,0, 127, 191, 255, blobpal);

    for (i=0; i<NUM_BLOBS; i++)
    {
        blobs[i] = newSprite(BLOB_WIDTH, BLOB_HEIGHT, blobbmp);
        //setBitmap(blobs[i], blobbmp);
        setPalette(blobs[i], blobpal);
        //setSpriteAlpha(blobs[i], true);
        if (i > 0) LinkCel(blobs[i-1]->cel, blobs[i]->cel);
    }
}

void blobsRun(Item screen, uint32 time)
{
    int px, py;
    int i;
    int t1 = 1640 * time;
    int t2 = 940 * time;
    int t3 = 2480 * time;

    for (i=0; i<NUM_BLOBS; i++)
    {
        px = (SinF16((i * i * 2800 + t1) << 4) >> 10) + (SinF16((i * i * 2400 + t2) << 4) >> 10);
        py = SinF16((i * i * 2600 + t3) << 4) >> 10;
        blobs[i]->posX = SCREEN_WIDTH / 2 + px; blobs[i]->posY = SCREEN_HEIGHT / 2 + py;
        mapSprite(blobs[i]);
    }
    
    drawSprite(blobs[0], screen);
}
