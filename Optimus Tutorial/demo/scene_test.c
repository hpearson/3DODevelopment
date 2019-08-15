#include "types.h"
#include "main.h"
#include "tools.h"
#include "engine_main.h"
#include "engine_mesh.h"
#include "engine_texture.h"
#include "scene_test.h"

mesh *cubeMesh;

void sceneTestInit()
{
	cubeMesh = initMesh(MESH_CUBE, 256, 1);
}

void sceneTestRun(Item screen, CCB *updatedCel, uint32 time)
{
    int i;
    time >>=1;
	cubeMesh->posX = 0; cubeMesh->posY = 0; cubeMesh->posZ = 512;
	cubeMesh->rotX = time; cubeMesh->rotY = time; cubeMesh->rotZ = time;

    for (i=0; i<cubeMesh->quadsNum; i++)
    {
        cubeMesh->quad[i].cel->ccb_SourcePtr = updatedCel->ccb_SourcePtr;
        cubeMesh->quad[i].cel->ccb_PLUTPtr = updatedCel->ccb_PLUTPtr;
    }

	uploadTransformAndProjectMesh(cubeMesh);
	renderTransformedGeometry(screen, cubeMesh);
}
