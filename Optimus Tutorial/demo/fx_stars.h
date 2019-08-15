#ifndef FX_STARS_H
#define FX_STARS_H

#define STAR_WIDTH 32
#define STAR_HEIGHT 32
#define STAR_SIZE (STAR_WIDTH * STAR_HEIGHT)

void starsInit();
void starsRun(Item screen, uint32 time);

#endif
