#ifndef FX_BUMP_H
#define FX_BUMP_H

#define BUMP_WIDTH 64
#define BUMP_HEIGHT 64
#define BUMP_SIZE (BUMP_WIDTH * BUMP_HEIGHT)

#define LIGHT_WIDTH 64
#define LIGHT_HEIGHT 64
#define LIGHT_SIZE (LIGHT_WIDTH * LIGHT_HEIGHT)

void bumpInit();
CCB *bumpRun(Item screen, uint32 time, bool render);

#endif
