#include "types.h"
#include "main.h"
#include "tools.h"
#include "engine_main.h"
#include "engine_mesh.h"
#include "engine_texture.h"
#include "mathutil.h"

void allocateMeshArrays(mesh *ms)
{
    ms->indexNum = ms->quadsNum << 2;
    ms->vrtx = (vertex*)malloc(ms->vrtxNum * sizeof(vertex));
    ms->index = (int*)malloc(ms->indexNum * sizeof(int));
    ms->lineIndex = (int*)malloc(ms->linesNum * sizeof(int));
    ms->quad = (quadData*)malloc(ms->quadsNum * sizeof(quadData));
}

void prepareCelList(mesh *ms)
{
	int i, j;

	for (i=0; i<ms->quadsNum; i++)
    {
		ms->quad[i].cel = CreateCel(ms->quad[i].tex->width, ms->quad[i].tex->height, 8, CREATECEL_CODED, ms->quad[i].tex->bitmap);
		ms->quad[i].cel->ccb_PLUTPtr = (PLUTChunk*)&ms->quad[i].tex->palette[ms->quad[i].palSelect << 5];
        ms->quad[i].cel->ccb_Flags &= ~CCB_ACW;
        ms->quad[i].cel->ccb_Flags |= CCB_ACSC;
        ms->quad[i].cel->ccb_Flags |= CCB_ALSC;
        if (i!=0) LinkCel(ms->quad[i-1].cel, ms->quad[i].cel);
    }
	ms->quad[ms->quadsNum-1].cel->ccb_Flags |= CCB_LAST;
}

mesh *initMesh(int type, int size, int divisions)
{
    int i, j, x, y;
	int xp, yp;
	int dx, dy;

    mesh *ms = (mesh*)malloc(sizeof(mesh));
	ms->numShades = 1;

    switch(type)
    {
        case MESH_PLANE:
            ms->vrtxNum = 4;
            ms->quadsNum = 1;
            ms->linesNum = 4;
            allocateMeshArrays(ms);

            ms->vrtx[0].x = -size/2; ms->vrtx[0].y = -size/2; ms->vrtx[0].z = 0;
            ms->vrtx[1].x = size/2; ms->vrtx[1].y = -size/2; ms->vrtx[1].z = 0;
            ms->vrtx[2].x = size/2; ms->vrtx[2].y = size/2; ms->vrtx[2].z = 0;
            ms->vrtx[3].x = -size/2; ms->vrtx[3].y = size/2; ms->vrtx[3].z = 0;

            for (i=0; i<4; i++)
                ms->index[i] = i;

            ms->quad[0].tex = getTexture(TEXTURE_NOISE);
			ms->quad[0].palSelect = 0;
        break;

        case MESH_CUBE:
            ms->vrtxNum = 8;
            ms->quadsNum = 6;
            ms->linesNum = 12;
            allocateMeshArrays(ms);

            ms->vrtx[0].x = -size/2; ms->vrtx[0].y = -size/2; ms->vrtx[0].z = -size/2;
            ms->vrtx[1].x = size/2; ms->vrtx[1].y = -size/2; ms->vrtx[1].z = -size/2;
            ms->vrtx[2].x = size/2; ms->vrtx[2].y = size/2; ms->vrtx[2].z = -size/2;
            ms->vrtx[3].x = -size/2; ms->vrtx[3].y = size/2; ms->vrtx[3].z = -size/2;
            ms->vrtx[4].x = size/2; ms->vrtx[4].y = -size/2; ms->vrtx[4].z = size/2;
            ms->vrtx[5].x = -size/2; ms->vrtx[5].y = -size/2; ms->vrtx[5].z = size/2;
            ms->vrtx[6].x = -size/2; ms->vrtx[6].y = size/2; ms->vrtx[6].z = size/2;
            ms->vrtx[7].x = size/2; ms->vrtx[7].y = size/2; ms->vrtx[7].z = size/2;

            ms->index[0] = 0; ms->index[1] = 1; ms->index[2] = 2; ms->index[3] = 3;
            ms->index[4] = 1; ms->index[5] = 4; ms->index[6] = 7; ms->index[7] = 2;
            ms->index[8] = 4; ms->index[9] = 5; ms->index[10] = 6; ms->index[11] = 7;
            ms->index[12] = 5; ms->index[13] = 0; ms->index[14] = 3; ms->index[15] = 6;
            ms->index[16] = 3; ms->index[17] = 2; ms->index[18] = 7; ms->index[19] = 6;
            ms->index[20] = 5; ms->index[21] = 4; ms->index[22] = 1; ms->index[23] = 0;

			for (i=0; i<(ms->quadsNum); i++)
			{
				ms->quad[i].tex = getTexture(TEXTURE_XOR);
				ms->quad[i].palSelect = 0;
			}
        break;

        case MESH_GRID:
            ms->vrtxNum = (divisions + 1) * (divisions + 1);
            ms->quadsNum = divisions * divisions;
            ms->linesNum = 2 * 2 * divisions * (divisions + 1);
            allocateMeshArrays(ms);

			dx = size / divisions;
			dy = size / divisions;

			i = 0;
			yp = -size / 2;
			for (y=0; y<=divisions; y++)
			{
				xp = -size / 2;
				for (x=0; x<=divisions; x++)
				{
					ms->vrtx[i].x = xp; ms->vrtx[i].y = yp; ms->vrtx[i].z = -(x * y*i*i) & 511;
					xp += dx;
					i++;
				}
				yp += dy;
			}

			i = 0;
			for (y=0; y<divisions; y++)
			{
				for (x=0; x<divisions; x++)
				{
					ms->index[i] = x + y * (divisions + 1);
					ms->index[i+1] = x + 1 + y * (divisions + 1);
					ms->index[i+2] = x + 1 + (y + 1) * (divisions + 1);
					ms->index[i+3] = x + (y + 1) * (divisions + 1);
					i+=4;
				}
			}

            i = 0;
            for (y=0; y<divisions+1; y++)
            {
                for (x=0; x<divisions; x++)
                {
                    ms->lineIndex[i] = x + y * (divisions+1); ms->lineIndex[i+1] = x + 1 + y * (divisions+1); i+=2;
                }
            }
            for (x=0; x<divisions+1; x++)
            {
                for (y=0; y<divisions; y++)
                {
                    ms->lineIndex[i] = x + y * (divisions+1); ms->lineIndex[i+1] = x + (y+1) * (divisions+1); i+=2;
                }
            }

            j = 0;
			for (i=0; i<(ms->quadsNum); i++)
			{
                int k;
				ms->quad[i].tex = getTexture(TEXTURE_NOISE);
                k = (-(ms->vrtx[ms->index[j]].z + ms->vrtx[ms->index[j+1]].z + ms->vrtx[ms->index[j+2]].z + ms->vrtx[ms->index[j+3]].z)) >> 7;
                j+=4;
				ms->quad[i].palSelect = (k*k) & 15;
			}

            ms->numShades = 16;
        break;

        default:
        break;
    }

	prepareCelList(ms);

    return ms;
}
