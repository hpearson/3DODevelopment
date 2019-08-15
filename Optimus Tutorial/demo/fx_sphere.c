#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_sphere.h"

ubyte sphereBuffer[SPHERE_SIZE];
ubyte sphereEraseBuffer[SPHERE_SIZE];
uint16 spherePal[256];
CCB *sphereCel;
Point sphereQuad[4];

ubyte dispX[SPHERE_SIZE];
ubyte dispY[SPHERE_SIZE];

uint32 dispX32[SPHERE_SIZE / 4];
uint32 dispY32[SPHERE_SIZE / 4];

void sphereInit()
{
    int x,y,i,k;
    int xc, yc;
    int r;
    int l, sl;
    bool mpike = false;

    sphereCel = CreateCel(SPHERE_WIDTH, SPHERE_HEIGHT, 8, CREATECEL_CODED, sphereBuffer);
    setPal(0,31, 0,0,0, 255,255,255, spherePal);

    i = 0;
    r = (SPHERE_HEIGHT * SPHERE_HEIGHT) / 4;
    for (y=0; y<SPHERE_HEIGHT; y++)
    {
        yc = y - SPHERE_HEIGHT / 2;
        for (x=0; x<SPHERE_WIDTH; x++)
        {
            xc = x - SPHERE_WIDTH / 2;
            l = r - xc*xc - yc*yc;
            if (l < 0) l = -l;
            if (l < 512) l = 0;
          
            sl = Sqrt32(l);
            if (sl==0) sl = 1;
            dispX[i] = 128 + (((xc << 8) / sl) >> 4);
            dispY[i] = 128 + (((yc << 8) / sl) >> 4);
            if (l==0) dispX[i] = dispY[i] = 0;
            sphereEraseBuffer[i] = (l==0);
            i++;
        }
    }
    for (i=0; i<SPHERE_SIZE; i+=4)
    {
        k = i >> 2;
        dispX32[k] = (dispX[i] << 24) | (dispX[i+1] << 16) | (dispX[i+2] << 8) | dispX[i+3];
        dispY32[k] = (dispY[i] << 24) | (dispY[i+1] << 16) | (dispY[i+2] << 8) | dispY[i+3];
    } // 48

    sphereCel->ccb_PLUTPtr = (PLUTChunk*)spherePal;

    SetQuadFromPosAndSize(sphereQuad, SCREEN_WIDTH / 2 - SPHERE_WIDTH / 2 , SCREEN_HEIGHT / 2 - SPHERE_HEIGHT / 2, SPHERE_WIDTH, SPHERE_HEIGHT);
    //SetQuadFromPosAndSize(sphereQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    MapCel(sphereCel, sphereQuad);
}

void sphereRun(Item screen, uint32 time)
{
    int i;
    ubyte px, py;
    uint32 px32, py32;
    uint32 *buff32 = (uint32*)sphereBuffer;
    uint32 *dx32 = dispX32;
    uint32 *dy32 = dispY32;

    px = 64 + (SinF16(time * 42400) >> 11);
    py = 64 + (SinF16(time * 67700) >> 11);
    
    px32 = (px << 24) | (px << 16) | (px << 8) | px;
    py32 = (py << 24) | (py << 16) | (py << 8) | py;
    for (i=SPHERE_SIZE/4; i!=0; i-=8)
    {
        *buff32++ = (*dx32 + px32) ^ (*dy32 + py32);
        *buff32++ = (*(dx32 + 1) + px32) ^ (*(dy32 + 1) + py32);
        *buff32++ = (*(dx32 + 2) + px32) ^ (*(dy32 + 2) + py32);
        *buff32++ = (*(dx32 + 3) + px32) ^ (*(dy32 + 3) + py32);
        *buff32++ = (*(dx32 + 4) + px32) ^ (*(dy32 + 4) + py32);
        *buff32++ = (*(dx32 + 5) + px32) ^ (*(dy32 + 5) + py32);
        *buff32++ = (*(dx32 + 6) + px32) ^ (*(dy32 + 6) + py32);
        *buff32++ = (*(dx32 + 7) + px32) ^ (*(dy32 + 7) + py32);
        dx32 += 8;
        dy32 += 8;
    }

    sphereCel->ccb_SourcePtr = (CelData*)sphereBuffer;
	sphereCel->ccb_Flags |= CCB_BGND;
    DrawCels(screen, sphereCel);

    sphereCel->ccb_SourcePtr = (CelData*)sphereEraseBuffer;
    sphereCel->ccb_Flags &= ~CCB_BGND;
    DrawCels(screen, sphereCel);
}
