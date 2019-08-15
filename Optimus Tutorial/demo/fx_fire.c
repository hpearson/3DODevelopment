#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_fire.h"

ubyte fireBuffer[FIRE_SIZE];
uint16 firePal[32];
CCB *fireCel;
Point fireQuad[4];

ubyte randPixels[SCREEN_WIDTH * 2];

void fireInit()
{
    int i;

    fireCel = CreateCel(FIRE_WIDTH, FIRE_HEIGHT, 8, CREATECEL_CODED, fireBuffer);
//    setPal(0,31, 8,8,8, 255,255,255, firePal);

    setPal(0,15, 0,0,0, 255,127,0, firePal);
    setPal(16,23, 255,127,0, 255,255,127, firePal);
    setPal(24,31, 255,255,127, 255,255,255, firePal);

    memset(fireBuffer, 0, FIRE_SIZE);

    fireCel->ccb_PLUTPtr = (PLUTChunk*)firePal;

    SetQuadFromPosAndSize(fireQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    MapCel(fireCel, fireQuad);

    fireCel->ccb_PIXC = TRANSLUCENT_CEL;

    for (i=0; i<FIRE_WIDTH * 2; i++)
        randPixels[i] = rand() & 31;
}

void drawBob(int px, int py)
{
    int x, y;
    int i = py * FIRE_WIDTH + px - 2;

    for (y=-2; y<=2; y++)
    {
        for (x=-2; x<=2; x++)
        {
            fireBuffer[i++] = 31;
        }
        i += FIRE_WIDTH - 5;
    }
}

void drawLineThick(int x0, int y0, int x1, int y1)
{
    ubyte *vram = fireBuffer;
    ubyte c = 31;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int ix, iy;
                                         
    int vx = 1, vy = FIRE_WIDTH;
    int remainder = 0;

    if (dx==0 && dy==0) return;

    if (dx<0)
    {
        dx = -dx;
        vx = -vx;
    }
    if (dy<0)
    {
        dy = -dy;
        vy = -vy;
    }

    if (dy>dx)
        remainder = dy;
                                                         
    vram += y0*FIRE_WIDTH + x0;
                                                          
    ix = dx+1;
    iy = dy+1;
    do
    {
    	while (remainder<=dx)
    	{
    		*vram = c;
    		vram+=vx;
    		remainder += dy;
    		ix--;
    		if (ix==0) return;
    	}
    	*vram = c;
    	vram+=vy;
    	remainder-=dx;
    	iy--;
    }while(iy!=0);
}

void drawLine(int x0, int y0, int x1, int y1)
{
    ubyte *vram = fireBuffer;
    ubyte c = 31;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int ix, iy;
                                         
    int vx = 1, vy = FIRE_WIDTH;
    int remainder = 0;

    if (dx==0 && dy==0) return;

    if (dx<0)
    {
        dx = -dx;
        vx = -vx;
    }
    if (dy<0)
    {
        dy = -dy;
        vy = -vy;
    }

    if (dy>dx)
        remainder = dy;
                                                         
    vram += y0*FIRE_WIDTH + x0;
                                                          
    ix = dx+1;
    iy = dy+1;

    if (dx >= dy)
    {
        do
        {
    	    while (remainder<=dx)
    	    {
    		    *vram = c;
    		    vram+=vx;
    		    remainder += dy;
    		    ix--;
    		    if (ix==0) return;
    	    }
    	    vram+=vy;
    	    remainder-=dx;
    	    iy--;
        }while(iy!=0);
    }
    else
    {
        do
        {
    	    while (remainder<=dx)
    	    {
    		    vram+=vx;
    		    remainder += dy;
    		    ix--;
    		    if (ix==0) return;
    	    }
    	    *vram = c;
    	    vram+=vy;
    	    remainder-=dx;
    	    iy--;
        }while(iy!=0);
    }
}

void updateNoise(uint32 time)
{
    int i;
/*    ubyte *fb = (ubyte*)&fireBuffer[FIRE_SIZE - FIRE_WIDTH * 8 - 1];
    for (i=0; i<8; i++)
        memcpy(fb, &randPixels[rand() % FIRE_WIDTH], FIRE_WIDTH); fb += SCREEN_WIDTH;*/

    //for (i=1; i<=16; i++)
        //drawBob(FIRE_WIDTH / 2 + ((SinF16(time * (11500 + 5000 * i)) * 64) >> 16), FIRE_HEIGHT / 2 + ((CosF16(time * (22000 + 8000 * i)) * 48) >> 16));
for (i=1; i<=6; i++)
    drawLine(FIRE_WIDTH / 2 + ((SinF16(time * (11500 + 5000 * i)) * 64) >> 16), 
            FIRE_HEIGHT / 2 + ((CosF16(time * (22000 + 8000 * i)) * 48) >> 16), 
            FIRE_WIDTH / 2 + ((SinF16(time * (11500 + 5000 * (i+1))) * 64) >> 16),
            FIRE_HEIGHT / 2 + ((CosF16(time * (22000 + 8000 * (i+1))) * 48) >> 16));

}

void blur()
{
    int i;
    uint32 *fb = (uint32*)&fireBuffer[FIRE_SIZE - FIRE_WIDTH - 4];
    uint32 *fbl = (uint32*)&fireBuffer[FIRE_SIZE - FIRE_WIDTH - 5];
    uint32 *fbr = (uint32*)&fireBuffer[FIRE_SIZE - FIRE_WIDTH - 3];

    //for (i=FIRE_SIZE - FIRE_WIDTH - 4; i >= FIRE_WIDTH + 4; i-=4)
    for (i=FIRE_SIZE - FIRE_WIDTH - 4 - FIRE_WIDTH - 4; i != 0; i-=4)
    {
        *fb-- = ((*(fb - FIRE_WIDTH / 4) + *(fb + FIRE_WIDTH / 4)) >> 1) & 0x1c1c1c1c;
        //*fb-- = ((*fb + *(fb - 1) + *(fb + 1) + *(fb + (FIRE_WIDTH / 4))) >> 2) & 0x1f1f1f1f;
        //*fb-- = ((*fb + *fbl + *fbr + *(fb + (FIRE_WIDTH / 4))) >> 2) & 0x1f1f1f1f;
        //fbl--; fbr--;
    }
}

void drawLineList(lineList *lnLst)
{
    int i, k = 0;
    int x0, y0, x1, y1;
    for (i=0; i<lnLst->linesNum; i++)
    {
        x0 = lnLst->linePoints[k++];
        y0 = lnLst->linePoints[k++];
        x1 = lnLst->linePoints[k++];
        y1 = lnLst->linePoints[k++];
        if (x0 >= 0 && x0 < FIRE_WIDTH && x1 >= 0 && x1 < FIRE_WIDTH && y0 >= 0 && y0 < FIRE_HEIGHT && y1 >= 0 && y1 < FIRE_HEIGHT) // oneday I will do proper line clipping
            drawLine(x0, y0, x1, y1);
    }
}

void fireRun(Item screen, lineList *lnLst, uint32 time)
{
    //updateNoise(time);

    drawLineList(lnLst);

    blur();
    DrawCels(screen, fireCel);
}
