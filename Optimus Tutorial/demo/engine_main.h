#ifndef ENGINE_MAIN_H
#define ENGINE_MAIN_H

#include "engine_mesh.h"

#define MAX_VERTICES_NUM 512

void initEngine();
void uploadVertices(mesh *ms);
void rotateVertices(int rotX, int rotY, int rotZ);
void translateVertices(int posX, int posY, int posZ);
void projectVertices();
vertex *getVertices();
void uploadTransformAndProjectMesh(mesh *ms);
void renderTransformedGeometry(Item screen, mesh *ms);

#endif
