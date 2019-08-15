#ifndef FX_ROTBLOBS_H
#define FX_ROTBLOBS_H

#define ROTBLOB_WIDTH 32
#define ROTBLOB_HEIGHT 32
#define ROTBLOB_SIZE (ROTBLOB_WIDTH * ROTBLOB_HEIGHT)

void rotblobsInit();
void rotblobsRun(Item screen, uint32 time);

#endif
