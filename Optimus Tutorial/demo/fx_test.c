#include "types.h"
#include "main.h"
#include "tools.h"
#include "fx_test.h"

ubyte testBuffer[TEST_SIZE];
uint16 testPal[32];
CCB *testCel;
Point testQuad[4];

int b = 0;
int c = 0;

void testInit()
{
    int x,y,i;

    testCel = CreateCel(TEST_WIDTH, TEST_HEIGHT, 8, CREATECEL_CODED, testBuffer);
    setPal(0,31, 8,8,8, 127,95,63, testPal);

    i = 0;
    for (y=0; y<TEST_HEIGHT; y++)
    {
        for (x=0; x<TEST_WIDTH; x++)
        {
            testBuffer[i++] = x*y;
        }
    }

    testCel->ccb_PLUTPtr = (PLUTChunk*)testPal;

    SetQuadFromPosAndSize(testQuad, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    MapCel(testCel, testQuad);
}

void testRun(Item screen, int b, int c, uint32 time)
{
    SetQuadFromPosAndSize(testQuad, b * -SCREEN_WIDTH + c * SCREEN_WIDTH * 2, b * -SCREEN_HEIGHT, (1 + b) * SCREEN_WIDTH + c * SCREEN_WIDTH * 2, (1 + b) * SCREEN_HEIGHT);
    MapCel(testCel, testQuad);
    DrawCels(screen, testCel);
}

// from fullscreen to 2xfull to outside screen totally
// blend on/off
