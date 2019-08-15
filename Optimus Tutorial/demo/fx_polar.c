#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_polar.h"

ubyte polarBuffer[POLAR_SIZE];
uint16 polarPal[32];
CCB *polarCel;
Point polarQuad[4];

ubyte angleBuffer[POLAR_SIZE];
ubyte radiusBuffer[POLAR_SIZE];

void polarInit()
{
    int x,y,i,xc,yc,c;

    polarCel = CreateCel(POLAR_WIDTH, POLAR_HEIGHT, 8, CREATECEL_CODED, polarBuffer);
    setPal(0,15, 64,8,64, 192,128,255, polarPal);
    setPal(16,31, 192,128,255, 64,8,64, polarPal);

    //memset(polarBuffer, 0, POLAR_SIZE);

    i = 0;
    for (y=0; y<POLAR_HEIGHT; y++)
    {
        yc = y - POLAR_HEIGHT / 2;
        for (x=0; x<POLAR_WIDTH; x++)
        {
            xc = x - POLAR_WIDTH / 2;
            c = (xc*xc + yc*yc) >> 7;
            if (c > 255) c = 255;
            radiusBuffer[i] = c;
            c = (Atan2F16(xc << 16, yc << 16) >> 16) & 127;
            angleBuffer[i] = c << 1;
            i++;
        }
    }

    polarCel->ccb_PLUTPtr = (PLUTChunk*)polarPal;

    SetQuadFromPosAndSize(polarQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    MapCel(polarCel, polarQuad);
}

void polarRun(Item screen, uint32 time)
{
    int i;
    uint32 *ab = (uint32*)angleBuffer;
    uint32 *rb = (uint32*)radiusBuffer;
    uint32 *pb = (uint32*)polarBuffer;
    uint8 t = time & 31;
    uint32 t32 = (t<<24) | (t<<16) | (t<<8) | t;

    for (i=POLAR_SIZE; i!=0; i-=32)
    {
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
        *pb++ = *ab++ + *rb++ - t32;
    }

    DrawCels(screen, polarCel);
}
