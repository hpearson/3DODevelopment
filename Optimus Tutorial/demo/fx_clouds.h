#ifndef FX_CLOUDS_H
#define FX_CLOUDS_H

#define CLOUDS_WIDTH 128
#define CLOUDS_HEIGHT 128
#define CLOUDS_SIZE (CLOUDS_WIDTH * CLOUDS_HEIGHT)

#define CLOUDS_NUM 4
#define CLOUD_PALS_NUM 32

void cloudsInit();
void cloudsRun(Item screen, int buttB, int buttC, uint32 time);

#endif
