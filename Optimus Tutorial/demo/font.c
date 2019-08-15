#include "types.h"
#include "item.h"
#include "mem.h"
#include "nodes.h"
#include "debug.h"
#include "list.h"
#include "device.h"
#include "driver.h"
#include "kernel.h"
#include "kernelnodes.h"
#include "io.h"
#include "graphics.h"
#include "filestream.h"
#include "filestreamfunctions.h"
#include "filesystem.h"
//#include "filesystemdefs.h"
#include "discdata.h"
#include "event.h"

#include "folio.h"
#include "task.h"
#include "semaphore.h"
#include "strings.h"
#include "stdlib.h"

#include "directory.h"
#include "directoryfunctions.h"

#include "filefunctions.h"
#include "Form3DO.h"
#include "Init3DO.h"
#include "Parse3DO.h"
#include "Utils3DO.h"


int32 MaxCharacterCount;


int32 myFontPLUT[] = 
	{
	0x18C66318, 0x63186318, 0x63186318, 0x63186318, 
	};


CCB MenuFontCCB =
	{
	/* ulong ccb_Flags; */
	CCB_LAST | CCB_NPABS | CCB_SPABS | CCB_PPABS | CCB_LDSIZE | CCB_LDPRS
			| CCB_LDPPMP | CCB_LDPLUT | CCB_YOXY | CCB_ACW | CCB_ACCW 
			| PMODE_ZERO,
/*???CCB_PACKED*/

	/* struct CCB *ccb_NextPtr; CelData *ccb_CelData; void *ccb_PLUTPtr; */
	&MenuFontCCB, NULL, myFontPLUT, 

	/* Coord ccb_X; Coord ccb_Y; */
	/* long ccb_HDX, ccb_HDY, ccb_VDX, ccb_VDY, ccb_DDX, ccb_DDY; */
	0,0,
	ONE_12_20,0, 0,ONE_16_16, 0,0, 

	/* ulong ccb_PPMPC; */
	(PPMP_MODE_NORMAL << PPMP_0_SHIFT)|(PPMP_MODE_AVERAGE << PPMP_1_SHIFT), 
	};

Font MenuFont = 
	{
	/* uint8     font_Height; */
	8, 
	/* uint8     font_Flags; */
	FONT_ASCII | FONT_ASCII_UPPERCASE, 
	/* CCB      *font_CCB; */
	&MenuFontCCB,
	/* FontChar *font_FontCharArray[]; */
	NULL,
	};

Font *SystemFont;

void
InitFont()
{
	ResetCurrentFont();
	SystemFont = GetCurrentFont();
//	MenuFont.font_FontEntries = SystemFont->font_FontEntries;
//	SetCurrentFont( &MenuFont );
	MenuFontCCB.ccb_SourcePtr = SystemFont->font_CCB->ccb_SourcePtr;
//	OpenRAMFont(&MenuFont);
	SetCurrentFontCCB(&MenuFontCCB);
	}

void CloseAFont(void) {
	ResetCurrentFont();
}

void DrawFont(Item *pScreen16) {
	int32 i;
	GrafCon gcon;
	int32 height, top, width, left, right, bottom;
	char *ptr;
	int32 lineheight;

    Item *pS;
	pS = pScreen16;

        SetFGPen( &gcon, 0x2C02 );

		myFontPLUT[0] = 0x00006318;
		ptr = "ABC123";
		i = strlen ( ptr ) * 8;
		gcon.gc_PenX =  (320 - i) / 2;;
		gcon.gc_PenY = 50;
		DrawText8( &gcon, *pS, ptr );
}
