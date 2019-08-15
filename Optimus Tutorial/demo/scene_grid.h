#ifndef SCENE_GRID_H
#define SCENE_GRID_H

#include "mathutil.h"

lineList *getLinesList();
void sceneGridInit();
void sceneGridRun(Item screen, int buttUp, int buttDown, int buttB, int buttC, uint32 time, bool render);

#endif
