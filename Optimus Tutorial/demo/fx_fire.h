#ifndef FX_FIRE_H
#define FX_FIRE_H

#define FIRE_WIDTH 160
#define FIRE_HEIGHT 120
#define FIRE_SIZE (FIRE_WIDTH * FIRE_HEIGHT)

#include "mathutil.h"

void fireInit();
void fireRun(Item screen, lineList *lnLst, uint32 time);

#endif
