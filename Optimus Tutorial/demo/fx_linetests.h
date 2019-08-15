#ifndef FX_LINETESTS_H
#define FX_LINETESTS_H

#define LINETESTS_WIDTH 320
#define LINETESTS_HEIGHT 240
#define LINETESTS_SIZE (LINETESTS_WIDTH * LINETESTS_HEIGHT)

void linetestsInit();
void linetestsRun(Item screen, int b, ubyte *vram, uint32 time);

#endif
