#ifndef MAIN_H
#define MAIN_H

#include "displayutils.h"
#include "debug.h"
#include "nodes.h"
#include "kernelnodes.h"
#include "list.h"
#include "folio.h"
#include "task.h"
#include "kernel.h"
#include "mem.h"
#include "operamath.h"
#include "math.h"
#include "semaphore.h"
#include "io.h"
#include "strings.h"
#include "stdlib.h"
#include "event.h"
#include "controlpad.h"

#include "stdio.h"
#include "graphics.h"
#include "3dlib.h"
#include "Init3DO.h"
#include "Form3DO.h"
#include "Parse3DO.h"
#include "Utils3DO.h"
#include "3d_examples.h"
#include "getvideoinfo.h"


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

enum {PART_FIRE, PART_PLASMA, PART_SPHERE, PART_BLOBS, PART_ROTOBLOBS, PART_STARS, PART_3D, PART_POLAR, PART_BUMP, PART_CLOUDS, PART_TEST, PART_LINETESTS, PART_DOTS, PART_SCENE_TEST, PART_SCENE_GRID, PARTS_NUM};

typedef enum TQuadCornerTag
{
	kQuadTL,
	kQuadTR,
	kQuadBR,
	kQuadBL
} TQuadIndex;

#endif
