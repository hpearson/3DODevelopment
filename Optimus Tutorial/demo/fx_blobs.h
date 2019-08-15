#ifndef FX_BLOBS_H
#define FX_BLOBS_H

#define BLOB_WIDTH 32
#define BLOB_HEIGHT 32
#define BLOB_SIZE (BLOB_WIDTH * BLOB_HEIGHT)

void blobsInit();
void blobsRun(Item screen, uint32 time);

#endif
