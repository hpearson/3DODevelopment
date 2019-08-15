#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_dots.h"

#define NUM_DOTS 4096

int dotsX[NUM_DOTS];
int dotsY[NUM_DOTS];
int dotsZ[NUM_DOTS];
int vramLine[SCREEN_HEIGHT];

void dotsInit()
{
    int i;
    for (i=0; i<SCREEN_HEIGHT; i++)
        vramLine[i] = ((i & ~1) * SCREEN_WIDTH + (i & 1)) * 2;
}

void updateDots(uint32 time)
{
    int i;
    for (i=0; i<NUM_DOTS; i++)
    {
        dotsX[i] = (SCREEN_WIDTH / 2 + (SinF16((i*i * 2800 + 1640 * time) << 4) >> 10) + (SinF16((i*i * 2400 + 940 * time) << 4) >> 10)) << 2;
        dotsY[i] = SCREEN_HEIGHT / 2 + (SinF16((i*i * 2600 + 2480 * time) << 4) >> 10);
        dotsZ[i] = 0;
    }
}

void drawDots(ubyte *vram, uint32 time)
{
    int i;
    for (i=0; i<NUM_DOTS; i++)
    {
        vram[vramLine[dotsY[i]] + dotsX[i]] = 0xff;
    }
}

bool test = false;

void dotsRun(ubyte *vram, uint32 time)
{
    if (!test) {updateDots(time); test = true;}
    drawDots(vram, time);
}
