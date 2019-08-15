#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_plasma.h"

ubyte plasmaBuffer[PLASMA_SIZE];
uint16 plasmaPal[32];
CCB *plasmaCel;
Point plasmaQuad[4];

ubyte *fsin1; //[PLASMA_WIDTH];
ubyte fsin2[4*(PLASMA_HEIGHT + 64)];
ubyte *fsin3; //[4*(PLASMA_WIDTH + PLASMA_HEIGHT)];

uint32 plasmImgXY[PLASMA_SIZE];

void plasmaInit()
{
    int i, j;
    int x, y;

    uint32 *pixy;
    uint32 *fs1;
    uint32 *fs3;

    plasmaCel = CreateCel(PLASMA_WIDTH, PLASMA_HEIGHT, 8, CREATECEL_CODED, plasmaBuffer);

    setPal(0,11, 0,63,127, 255,127,0, plasmaPal);
    setPal(12,23, 255,127,0, 255,63,127, plasmaPal);
    setPal(24,31, 255,63,127, 0,63,127, plasmaPal);

    plasmaCel->ccb_PLUTPtr = (PLUTChunk*)plasmaPal;

    SetQuadFromPosAndSize(plasmaQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    MapCel(plasmaCel, plasmaQuad);

    // SinF16 wants 16:16 fixed point. Where angle is 256 degrees. So, full cicle is 256 * 65536
    // Returns 16:16, from -1 to 1, so it is from -65536 to 65536. We add 65536, mul by our prefered integer range and last divide by 131072 (shift right 17)

    fsin1 = (ubyte*)malloc(PLASMA_WIDTH * sizeof(ubyte));
    fsin3 = (ubyte*)malloc(4*(PLASMA_WIDTH + PLASMA_HEIGHT) * sizeof(ubyte));

    for (i=0; i<8*(PLASMA_HEIGHT + 64); i++)
        fsin2[i] = ((SinF16(((i >> 2) << 16) << 2) + 65536) * 48) >> 17;

	for (i=0; i<PLASMA_WIDTH; i++)
		fsin1[i] = ((SinF16((i << 16) << 3) + 65536) * 36) >> 17;

    for (j=0; j<4; j++)
        for (i=0; i<(PLASMA_WIDTH + PLASMA_HEIGHT); i++)
		    fsin3[j*(PLASMA_WIDTH + PLASMA_HEIGHT) + i] = ((SinF16(((i + j) << 16) << 1) + 65536) * 63) >> 17;

    pixy = plasmImgXY;
	for (y=0; y<PLASMA_HEIGHT; y++)
	{
        fs1 = (uint32*)fsin1;
        fs3 = (uint32*)(&fsin3[(y & 3) * (PLASMA_WIDTH + PLASMA_HEIGHT) + (y - (y & 3))]);
		for (x=0; x<PLASMA_WIDTH; x+=4)
		{
            *pixy++ = *fs1++ + *fs3++;
		}
	}

    free(fsin1);
    free(fsin3);
}

// ---------------------------------------------------------------------------
// Plasma
// ---------------------------------------------------------------------------
void plasmaRun(Item screen, uint32 time)
{
    int i;
	int x, y;
    uint32 yc;
 	uint32 k1 = time >> 1;
    uint32 *pixy = plasmImgXY;
    uint32 *vram32 = (uint32*)plasmaBuffer;

    k1 = k1 & 63;
	for (y=PLASMA_HEIGHT; y!=0; y--)
	{
        yc = *((uint32*)(&fsin2[(y+k1) << 2]));
		for (x=PLASMA_WIDTH; x!=0; x-=32)
		{
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
            *vram32++ = *pixy++ + yc;
		}
	}
//91-94-122
    DrawCels(screen, plasmaCel);
}

// What if different sizes of texture?
// check if coded/uncoded difference.
// check if disable rendering while access

// More effects:
// polar radial
// 2d bump.

// Cells zooming out, maybe with plasma or polar effect. Mosaic picture.
// rotozooming scroller fonts

// Glow
// 3D Engine: other. envmap. city. octagons. glow.
