#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_stars.h"
#include "sprite_engine.h"

#define NUM_STARS 96
#define NUM_FADE_PALS 32

sprite *stars[NUM_STARS];

ubyte starbmp[STAR_SIZE];
uint16 starpal[32 * NUM_FADE_PALS];

int starX[NUM_STARS];
int starY[NUM_STARS];
int starZ[NUM_STARS];
int starV[NUM_STARS];

int starZoom[2048];

void randomizeStar(int starNum)
{
    starX[starNum] = (rand() & 255) - 128;
    starY[starNum] = (rand() & 255) - 128;
    starZ[starNum] = (rand() & 1023) + 1024;
    starV[starNum] = -7 + (rand() & 3);
}

void starsInit()
{
    int x,y,i;
    int xc, yc;
    int c, r;

    i = 0;
    for (i=1; i<2048; i++)
        starZoom[i] = 32768 / i;

    i = 0;
    for (y=0; y<STAR_HEIGHT; y++)
    {
        yc = y - STAR_HEIGHT / 2;
        for (x=0; x<STAR_WIDTH; x++)
        {
            xc = x - STAR_WIDTH / 2;
            r = xc*xc + yc*yc;
            if (r==0) r = 1;
            c = 1024 / r;
            if (c > 31) c = 31;
            if (c < 8) c = 0;
            starbmp[i++] = c;
        }
    }

    for (i=0; i<NUM_FADE_PALS; i++)
    {
        setPal(0,31, 0,0,0, (127 * i) / NUM_FADE_PALS, (191 * i) / NUM_FADE_PALS, (255 * i) / NUM_FADE_PALS, &starpal[i << 5]);
    }

    for (i=0; i<NUM_STARS; i++)
    {
        stars[i] = newSprite(STAR_WIDTH, STAR_HEIGHT, starbmp);
        //setPalette(stars[i], starpal);
        setSpriteAlpha(stars[i], true);
        if (i > 0) LinkCel(stars[i-1]->cel, stars[i]->cel);

        randomizeStar(i);
    }
}

void updateStars()
{
    int i, px, py, zpal;

    for (i=0; i<NUM_STARS; i++)
    {
        px = (starX[i] << 8) / starZ[i];
        py = (starY[i] << 8) / starZ[i]; // with 512 and zoom 32768 and 256 stars crashes.

        stars[i]->posX = SCREEN_WIDTH / 2 + px; stars[i]->posY = SCREEN_HEIGHT / 2 + py;
        stars[i]->zoom = starZoom[starZ[i]]; //32768 / starZ[i];

        zpal = (((1024 - starZ[i]) * NUM_FADE_PALS) >> 10) + (NUM_FADE_PALS >> 3);
        if (zpal < 0) zpal = 0;
        if (zpal > NUM_FADE_PALS - 1) zpal = NUM_FADE_PALS - 1;
        setPalette(stars[i], &starpal[zpal << 5]);
        mapZoomSprite(stars[i]);

        starZ[i] += starV[i];
        if (starZ[i] <=0) randomizeStar(i);
    }
}

void starsRun(Item screen, uint32 time)
{
    updateStars();
    drawSprite(stars[0], screen);
}
