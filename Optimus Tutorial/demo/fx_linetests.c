#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_linetests.h"

#define NUM_POINTS 64

ubyte linetestsBuffer[LINETESTS_SIZE];
uint16 linetestsPal[32];
CCB *linetestsCel;
Point linetestsQuad[4];

ubyte lineColorBuffer[1];

ubyte randPixels[SCREEN_WIDTH * 2];

#define CCBTotal 0x200

static CCB CCBArray[CCBTotal];		/* Array of CCB structs */
static CCB *CurrentCCB = &CCBArray[0];	/* Pointer to empty CCB */

int points[4 * NUM_POINTS];

Item VideoItem;

void linetestsInit()
{
    int i, t = 6128;
    int halfWidth = LINETESTS_WIDTH / 2 - LINETESTS_WIDTH / 16;
    int halfHeight = LINETESTS_HEIGHT / 2 - LINETESTS_HEIGHT / 16;

    lineColorBuffer[0] = 31;
    {
	CCB *CCBPtr = CCBArray;
    i = CCBTotal;
	do {
		CCBPtr->ccb_NextPtr = (CCB *)(sizeof(CCB)-8);
		CCBPtr->ccb_HDDX = 0;
		CCBPtr->ccb_HDDY = 0;

        ++CCBPtr;
	} while (--i);
    }
/*
    for (i=0; i<CCBTotal; i++)
    {
        if (i > 0) CCBArray[i-1].ccb_NextPtr = &CCBArray[i];
        //CCBArray[i].ccb_HDDX = 0;
        //CCBArray[i].ccb_HDDY = 0;
    }
*/

    linetestsCel = CreateCel(LINETESTS_WIDTH, LINETESTS_HEIGHT, 8, CREATECEL_CODED, linetestsBuffer);
    setPal(0,31, 0,0,0, 255,255,255, linetestsPal);

    memset(linetestsBuffer, 0, LINETESTS_SIZE);

    linetestsCel->ccb_PLUTPtr = (PLUTChunk*)linetestsPal;

    SetQuadFromPosAndSize(linetestsQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    MapCel(linetestsCel, linetestsQuad);

    linetestsCel->ccb_PIXC = TRANSLUCENT_CEL;

    for (i=0; i<LINETESTS_WIDTH * 2; i++)
        randPixels[i] = rand() & 31;

    for (i=0; i<4*NUM_POINTS; i+=4)
    {
        points[i] = LINETESTS_WIDTH / 2 + ((SinF16(t * (11500 + 5000 * i)) * halfWidth) >> 16);
        points[i+1] = LINETESTS_HEIGHT / 2 + ((CosF16(t * (22000 + 8000 * i)) * halfHeight) >> 16);
        points[i+2] = LINETESTS_WIDTH / 2 + ((SinF16(t * (11500 + 5000 * (i+1))) * halfWidth) >> 16);
        points[i+3] = LINETESTS_HEIGHT / 2 + ((CosF16(t * (22000 + 8000 * (i+1))) * halfHeight) >> 16);

        //points[i] = (SinF16(t * (11500 + 5000 * i)) * halfWidth) >> 16;
        //points[i+1] = (CosF16(t * (22000 + 8000 * i)) * halfHeight) >> 16;
        //points[i+2] = (SinF16(t * (11500 + 5000 * (i+1))) * halfWidth) >> 16;
        //points[i+3] = (CosF16(t * (22000 + 8000 * (i+1))) * halfHeight) >> 16;
    }
}

void drawLineThick(int x0, int y0, int x1, int y1, ubyte c)
{
    ubyte *vram = linetestsBuffer;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int ix, iy;
                                         
    int vx = 1, vy = LINETESTS_WIDTH;
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
                                                         
    vram += y0*LINETESTS_WIDTH + x0;
                                                          
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

void drawLine16(int *points, ushort *vram, ushort c)
{
    int x0 = *points++;
    int y0 = *points++;
    int x1 = *points++;
    int y1 = *points;

    int dx = x1 - x0;
    int dy = y1 - y0;
    int ix, iy;
                                         
    int vx = 1, vy = LINETESTS_WIDTH;
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
                                                         
    vram += y0*LINETESTS_WIDTH + x0;
                                                          
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

void drawLine(int *points, ubyte c)
{
    int x0 = *points++;
    int y0 = *points++;
    int x1 = *points++;
    int y1 = *points;

    ubyte *vram = linetestsBuffer;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int ix, iy;
                                         
    int vx = 1, vy = LINETESTS_WIDTH;
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
                                                         
    vram += y0*LINETESTS_WIDTH + x0;
                                                          
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

void FlushCCBs()
{
	CCB* NewCCB;

	NewCCB = CurrentCCB;
	if (NewCCB!=&CCBArray[0]) {
		--NewCCB;		/* Get the last used CCB */
		NewCCB->ccb_Flags |= CCB_LAST;	/* Mark as the last one */
		DrawCels(VideoItem,(CCB *)&CCBArray[0]);	/* Draw all the cels in one shot */
		CurrentCCB = &CCBArray[0];		/* Reset the empty entry */
	}
}

void drawLineCCB(int *points, ushort color)
{
    int x1 = *points++;
    int y1 = *points++;
    int x2 = *points++;
    int y2 = *points;
	CCB* DestCCB;			/* Pointer to new CCB entry */

	DestCCB = CurrentCCB;		/* Copy pointer to local */
	if (DestCCB>=&CCBArray[CCBTotal]) {		/* Am I full already? */
		FlushCCBs();				/* Draw all the CCBs/Lines */
		DestCCB=CCBArray;
	}
	DestCCB->ccb_Flags = CCB_LDSIZE|CCB_LDPRS|
		CCB_LDPPMP|CCB_CCBPRE|CCB_YOXY|CCB_ACW|CCB_ACCW|
		CCB_ACE|CCB_BGND|CCB_NOBLK;	/* ccb_flags */

	DestCCB->ccb_PIXC = 0x1F00;		/* PIXC control */
	DestCCB->ccb_PRE0 = 0x40000016;		/* Preamble */
	DestCCB->ccb_PRE1 = 0x03FF1000;		/* Second preamble */
	DestCCB->ccb_SourcePtr = (CelData *)0;	/* Save the source ptr */
	DestCCB->ccb_PLUTPtr = (void *)(color<<16);		/* Set the color pixel */

	if ((int)x2<(int)x1) {	/* By sorting the x and y's I can only draw lines */
		short Temp;		/* in two types, x>=y or x<y */
		Temp = x1;		/* Swap the x's */
		x1 = x2;
		x2 = Temp;
		Temp = y1;		/* Swap the y's */
		y1 = y2;
		y2 = Temp;
	}

	//y1=-y1;			/* The y's are upside down!! */
	//y2=-y2;

	x2=(x2-x1)+1;	/* Get the DELTA value (Always positive) */
	y2=y2-y1;		/* But add 1 for inclusive size depending on sign */
	//x1+=160;	/* Move the x coords to the CENTER of the screen */
	//y1+=80;		/* Vertically flip and then CENTER the y */

	if (y2&0x8000) {	/* Negative y? */
		y2-=1;			/* Widen by 1 pixel */
		if (x2<(-y2)) {	/* Quadrant 7? */
			x1-=1;
			y1+=1;
			goto Quadrant7;	/* OK */
		}		/* Quadrant 6 */
		goto Quadrant6;		/* OK */
	}
	++y2;
	if (x2<y2) {	/* Quadrant 7? */
Quadrant7:
		DestCCB->ccb_HDX = 1<<20;
		DestCCB->ccb_HDY = 0<<20;
		DestCCB->ccb_VDX = x2<<16;
		DestCCB->ccb_VDY = y2<<16;
	} else {		/* Quadrant 6 */
		--y1;
Quadrant6:
		DestCCB->ccb_VDX = 0<<16;
		DestCCB->ccb_VDY = 1<<16;
		DestCCB->ccb_HDX = x2<<20;
		DestCCB->ccb_HDY = y2<<20;
	}
	
	DestCCB->ccb_XPos = x1<<16;		/* Set the x and y coord for start */
	DestCCB->ccb_YPos = y1<<16;

	++DestCCB;			/* Next CCB */
	CurrentCCB = DestCCB;	/* Save the CCB pointer */
}

void drawLines(uint32 time)
{
    int i, t = time & 31;
    for (i=0; i<4*NUM_POINTS; i+=4)
        drawLineCCB(&points[i], (t << 10) | (t << 5) | t);
        //drawLine(&points[i], time);
        //drawLine16(&points[i], (ushort*)vram, time);
        //drawLineThick(points[i], points[i+1], points[i+2], points[i+3], time);

    FlushCCBs();
}

// 73 -> 80

void linetestsRun(Item screen, int b, ubyte *vram, uint32 time)
{
    int i, t = time & 31;
    VideoItem = screen;

    for (i=0; i<4*NUM_POINTS; i+=4)
    {
        if (b==0)
        {
            drawLine(&points[i], t);
        }
        else
        {
            drawLineCCB(&points[i], 0x7fff);
        }
    }
    if (b==0)
        DrawCels(screen, linetestsCel);
    else
        FlushCCBs();
}
