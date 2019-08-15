#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_bump.h"

ubyte bumpBuffer[BUMP_SIZE];
uint16 bumpPal[32];
CCB *bumpCel;
Point bumpQuad[4];

ubyte *bumpPattern;
uint32 bumpDXDY[BUMP_SIZE];
ubyte bumpLightMap[BUMP_SIZE];
ubyte bumpLight[LIGHT_SIZE];

void bumpInit()
{
    int x,y,i,xc,yc,c,r,n;
    int dx, dy;

    bumpPattern = (ubyte*)malloc(sizeof(ubyte) * BUMP_SIZE);

    bumpCel = CreateCel(BUMP_WIDTH, BUMP_HEIGHT, 8, CREATECEL_CODED, bumpBuffer);
    setPal(0,31, 16,16,64, 192,224,255, bumpPal);

    memset(bumpLightMap, 0, BUMP_SIZE);

    for (i=0; i<BUMP_SIZE; i++)
        bumpPattern[i] = rand() & 255;

    for (n=0; n<3; n++)
    {
        for (i=0; i<BUMP_SIZE; i++)
        {
            bumpPattern[i] = (bumpPattern[(i-1) & (BUMP_SIZE-1)] + bumpPattern[(i+1) & (BUMP_SIZE-1)] + bumpPattern[(i-BUMP_WIDTH) & (BUMP_SIZE-1)] + bumpPattern[(i+BUMP_WIDTH) & (BUMP_SIZE-1)]) >> 2;
        }
    }

    i = 0;
    for (y=0; y<BUMP_HEIGHT; y++)
    {
        for (x=0; x<BUMP_WIDTH; x++)
        {
            dx = x + bumpPattern[(i+1) & (BUMP_SIZE-1)] - bumpPattern[(i-1) & (BUMP_SIZE-1)];
            dy = y + bumpPattern[(i+BUMP_WIDTH) & (BUMP_SIZE-1)] - bumpPattern[(i-BUMP_WIDTH) & (BUMP_SIZE-1)];
            if (dx < 0) dx = 0; if (dx > BUMP_WIDTH - 1) dx = BUMP_WIDTH - 1;
            if (dy < 0) dy = 0; if (dy > BUMP_HEIGHT - 1) dy = BUMP_HEIGHT - 1;
            bumpDXDY[i++] = (dy * BUMP_WIDTH + dx) & (BUMP_SIZE - 1);
        }
    }

    free(bumpPattern);

    i = 0;
    for (y=0; y<LIGHT_HEIGHT; y++)
    {
        yc = y - LIGHT_HEIGHT / 2;
        for (x=0; x<LIGHT_WIDTH; x++)
        {
            xc = x - LIGHT_WIDTH / 2;
            r = xc*xc + yc*yc; if (r==0) r = 1;
            c = 32768 / r;
            if (c > 255) c = 255;
            bumpLight[i++] = (c >> 3);
        }
    }

    bumpCel->ccb_PLUTPtr = (PLUTChunk*)bumpPal;

    SetQuadFromPosAndSize(bumpQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //SetQuadFromPosAndSize(bumpQuad, SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    MapCel(bumpCel, bumpQuad);
}

void drawLightToLightmap(int px, int py)
{
    int x, y;
    int x0, x1, y0, y1;

    ubyte *bl = bumpLight;
    ubyte *blm = bumpLightMap;

    x0 = px - LIGHT_WIDTH / 2;
    y0 = py - LIGHT_HEIGHT / 2;
    x1 = x0 + LIGHT_WIDTH;
    y1 = y0 + LIGHT_HEIGHT;

    memset(bumpLightMap, 0, BUMP_SIZE);

    if (x1 < 0 || x0 > BUMP_WIDTH - 1 || y1 < 0 || y0 > BUMP_HEIGHT - 1) return;

    if (x0 < 0)
    {
        bl -= x0;
        x1 += x0;
        x0 = 0;
    }
    if (x1 > BUMP_WIDTH - 1)
    {
        x1 = BUMP_WIDTH - 1;
    }
    if (y0 < 0)
    {
        bl -= y0 * LIGHT_WIDTH;
        y1 += y0;
        y0 = 0;
    }
    if (y1 > BUMP_HEIGHT - 1)
    {
        y1 = BUMP_HEIGHT - 1;
    }

    blm += y0 * BUMP_WIDTH + x0;

    for (y=y0; y<y1; y++)
    {
        if (x1 > x0)
            memcpy(blm, bl, x1 - x0);
        blm += BUMP_WIDTH;
        bl += LIGHT_WIDTH;

        /*for (x=x0; x<x1; x++)
        {
            *blm++ |= *bl++;
        }
        blm += -(x1 - x0) + BUMP_WIDTH;
        bl += LIGHT_WIDTH - (x1 - x0);*/
    }
}

CCB *bumpRun(Item screen, uint32 time, bool render)
{
    int px, py, pi, i;
    //ubyte *bb = &bumpBuffer[time & 1];
    //uint32 *bd = &bumpDXDY[time & 1];

    ubyte *bb = &bumpBuffer[(time & 1) * BUMP_WIDTH];
    uint32 *bd = &bumpDXDY[(time & 1) * BUMP_WIDTH];

    px = BUMP_WIDTH / 2 + (SinF16(time << 15) >> 12);
    py = BUMP_HEIGHT / 2 + (SinF16(time << 16) >> 12);
    drawLightToLightmap(px, py);

/*    for (i=BUMP_SIZE; i!=0; i-=8)
    {
        *bb = bumpLightMap[*bd]; bb+=2; bd+=2;
        *bb = bumpLightMap[*bd]; bb+=2; bd+=2;
        *bb = bumpLightMap[*bd]; bb+=2; bd+=2;
        *bb = bumpLightMap[*bd]; bb+=2; bd+=2;
    }*/

    for (i=BUMP_HEIGHT; i!=0; i-=2)
    {
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];

        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];

        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];

        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];
        *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++]; *bb++ = bumpLightMap[*bd++];

        bb+=BUMP_WIDTH; bd+=BUMP_WIDTH;
    }

    if (render)
    {
        DrawCels(screen, bumpCel);
        return 0;
    }
    else
        return bumpCel;
}
