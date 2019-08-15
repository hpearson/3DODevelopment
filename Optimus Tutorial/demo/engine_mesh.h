#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include "engine_texture.h"
#include "mathutil.h"

typedef struct quadData
{
	texture *tex;
	CCB *cel;
	int palSelect;
}quadData;

typedef struct mesh
{
    vertex *vrtx;
    int vrtxNum;

    int *index;
    int indexNum;

    int *lineIndex;
    int linesNum;
    
    quadData *quad;
    int quadsNum;

	int numShades;
    
    int posX, posY, posZ;
    int rotX, rotY, rotZ;
}mesh;


enum {MESH_PLANE, MESH_CUBE, MESH_GRID};

mesh *initMesh(int type, int size, int divisions);

#endif
