#include "types.h"
#include "main.h"
#include "tools.h"
#include "engine_main.h"
#include "engine_mesh.h"
#include "engine_texture.h"
#include "scene_grid.h"
#include "mathutil.h"

mesh *gridMesh;
int posZ = 0;

lineList *linesLst;

void sceneGridInit()
{
	gridMesh = initMesh(MESH_GRID, 4096, 12);
    linesLst = (lineList*)malloc(sizeof(lineList));
    linesLst->linesNum = gridMesh->linesNum;
    linesLst->linePoints = (int*)malloc(2 * 2 * linesLst->linesNum * sizeof(int));
}

lineList *getLinesList()
{
    int i, j = 0, k = 0;
    vertex *vrtx = getVertices();

    for (i=0; i<gridMesh->linesNum; i++)
    {
        linesLst->linePoints[k++] = vrtx[gridMesh->lineIndex[j]].x;
        linesLst->linePoints[k++] = vrtx[gridMesh->lineIndex[j]].y;
        linesLst->linePoints[k++] = vrtx[gridMesh->lineIndex[j+1]].x;
        linesLst->linePoints[k++] = vrtx[gridMesh->lineIndex[j+1]].y;
        j+=2;
    }

    return linesLst;
}


void sceneGridRun(Item screen, int buttUp, int buttDown, int buttB, int buttC, uint32 time, bool render)
{
    int i;

    uploadVertices(gridMesh);
    rotateVertices(0, 0, time);
	rotateVertices(35, 0, 0);
	translateVertices(0, 512, 2048 + 512 + posZ);
    projectVertices();

    if (render)
	    renderTransformedGeometry(screen, gridMesh);

    //if (buttUp) posZ += 64;
    //if (buttDown) posZ -= 64;
}
