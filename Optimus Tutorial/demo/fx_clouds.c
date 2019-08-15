#include "types.h"
#include "main.h"
#include "tools.h"
#include "sprite_engine.h"
#include "fx_clouds.h"

ubyte cloudsBuffer[CLOUDS_SIZE];
uint16 cloudsPal[32 * CLOUD_PALS_NUM];

sprite *clouds[CLOUDS_NUM];

int cloudX[CLOUDS_NUM];
int cloudY[CLOUDS_NUM];
int cloudZ[CLOUDS_NUM];

void cloudsInit()
{
    int x,y,i,c;

    for (i=0; i<CLOUD_PALS_NUM; i++)
    {
        //setPal(0,31, 8,8,8, (191 * (i+1)) / CLOUD_PALS_NUM, (223 * (i+1)) / CLOUD_PALS_NUM, (255 * (i+1)) / CLOUD_PALS_NUM, &cloudsPal[i << 5]);
        setPal(0,31, 0,0,0, (255 * (i+1)) / CLOUD_PALS_NUM, (255 * (i+1)) / CLOUD_PALS_NUM, (255 * (i+1)) / CLOUD_PALS_NUM, &cloudsPal[i << 5]);
    }

    for (i=0; i<CLOUDS_NUM; i++)
    {
        clouds[i] = newSprite(CLOUDS_WIDTH, CLOUDS_HEIGHT, cloudsBuffer);
        //setPalette(clouds[i], cloudsPal);
        if (i!=0) setSpriteAlpha(clouds[i], true);
        if (i > 0) LinkCel(clouds[i-1]->cel, clouds[i]->cel);

        //clouds[i]->cel->ccb_Flags |= CCB_BGND;

        cloudX[i] = cloudY[i] = 0;
        cloudZ[i] = (CLOUDS_NUM - i) << 6;
    }

    i = 0;
    for (y=0; y<CLOUDS_HEIGHT; y++)
    {
        for (x=0; x<CLOUDS_WIDTH; x++)
        {
            c = (SinF16(x << 19) + CosF16(y << 18) + SinF16((x + y) << 17)) >> 10;
            //c = x ^ y;
//            if (c < 128) c = 0;
            if (c > 31) c = 31;
            if (c < 0) c = 0;
            cloudsBuffer[i++] = c;
        }
    }
}

void animateClouds(uint32 time)
{
    int i, px, py, zpal;
    int t = (time * 1) & 15;

    for (i=0; i<CLOUDS_NUM; i++)
    {
        cloudZ[i] = ((CLOUDS_NUM - i) << 4) - t;

        px = (cloudX[i] << 8) / cloudZ[i];
        py = (cloudY[i] << 8) / cloudZ[i];

        clouds[i]->posX = SCREEN_WIDTH / 2 + px; clouds[i]->posY = SCREEN_HEIGHT / 2 + py;
        clouds[i]->zoom = 16384 / (cloudZ[i] + 16);

        zpal = (((CLOUDS_NUM << 4) - cloudZ[i]) >> 3);// + (CLOUD_PALS_NUM >> 3);
zpal <<=1;
        if (zpal < 8) zpal = 8;
        //if (zpal > CLOUD_PALS_NUM - 1) zpal = CLOUD_PALS_NUM - 1;
        if (zpal >= CLOUD_PALS_NUM / 2) zpal = CLOUD_PALS_NUM - 1 - zpal;
        zpal <<= 2;
        if (zpal > CLOUD_PALS_NUM - 1) zpal = CLOUD_PALS_NUM - 1;
        if (zpal < 0) zpal = 0;
        setPalette(clouds[i], &cloudsPal[zpal << 5]);
        mapZoomSprite(clouds[i]);
    }
}

void cloudsRun(Item screen, int buttB, int buttC, uint32 time)
{
    int i;
    animateClouds(time);

    if (buttB)
    {
        for (i=0; i<CLOUDS_NUM; i++)
        {
            clouds[i]->cel->ccb_Flags |= CCB_MARIA;
        }
    }
    else
    {
        for (i=0; i<CLOUDS_NUM; i++)
        {
            clouds[i]->cel->ccb_Flags &= ~CCB_MARIA;
        }
    }

    if (buttC)
    {
        for (i=0; i<CLOUDS_NUM; i++)
        {
            clouds[i]->cel->ccb_Flags &= ~CCB_ACE;
        }
    }
    else
    {
        for (i=0; i<CLOUDS_NUM; i++)
        {
            clouds[i]->cel->ccb_Flags |= CCB_ACE;
        }
    }

    DrawCels(screen, clouds[0]->cel);
}
