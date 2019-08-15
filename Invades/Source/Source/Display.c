// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include "stdio.h"
#include "string.h"

#include "debug3do.h"
#include "displayutils.h"
#include "event.h"
#include "getvideoinfo.h"
#include "graphics.h"
#include "mem.h"

#include "Defines.h"
#include "Display.h"
#include "Sprite.h"
#include "System.h"

// Screen context
ScreenContext	*gScreenContext	= NULL;

// Display type
int		gDisplayType;

// VRAM IO req
Item	gVRAMIOReq		= -1;

// Display
Item	gDisplay		= -1;

// Background image
ubyte	*gBackground	= NULL;

int		gCurrentScreen	= 0;

bool InitializeDisplay()
{
	int32	iErrorCode;

	PRT(("Initializing the display\n"));

	// Create an IO request for VRAM
	gVRAMIOReq	= CreateVRAMIOReq();
	
	if (gVRAMIOReq < 0)
	{
		PRT(("Unable to create VRAM IO Req\n"));
		
		return	false;
	}
	
	gScreenContext	= (ScreenContext *)AllocMem(sizeof(ScreenContext), MEMTYPE_ANY);
    
    if (NULL == gScreenContext)
    {
		PRT(("Unable to allocate memory for screen context\n"));
		
		return	false;
    }

	iErrorCode	= OpenGraphicsFolio();
	
	if (iErrorCode < 0)
	{
		DIAGNOSE_SYSERR(iErrorCode, ("Can't open the graphics folio\n"));

		return	false;
	}
				
	gDisplayType	= GetDisplayType();
	
	if (gDisplayType < 0)
	{
		PRT(("Bad display type returned\n"));
		
		return	false;
	}
	
	if (DI_TYPE_PAL1 == gDisplayType || DI_TYPE_PAL2 == gDisplayType)
	{
		gDisplayType	= DI_TYPE_PAL2;
	}
	
	else
	{
		gDisplayType	= DI_TYPE_NTSC;
	}

	// Create display, with two screens
	gDisplay	= CreateBasicDisplay(gScreenContext, gDisplayType, 2);
	
	if (gDisplay < 0)
	{
		PRT(("Can't initialize the display\n"));
		
		return	false;
	}
	
	return	true;
}

void DestroyDisplay()
{
	// Fade the screen out
	if (gScreenContext != NULL)
	{
		FadeToBlack(gScreenContext, 60);
	}

	// Free the background memory
	if (gBackground != NULL)
	{
		int	iImageSize	= gScreenContext->sc_nFrameByteCount;
		
		FreeMem(gBackground, iImageSize);
	
		gBackground	= NULL;
	}

	// Delete the display
	if (gDisplay >= 0)
	{
		DeleteBasicDisplay(gScreenContext);
	
		gDisplay	= -1;
	}
		
	// Close graphics folio
	CloseGraphicsFolio();

	// Free the memory allocated from the screen context
	if (gScreenContext != NULL)
	{
		FreeMem(gScreenContext, sizeof(ScreenContext));
	
		gScreenContext	= NULL;
	}
		
	// Free the VRAM IO req
	if (gVRAMIOReq >= 0)
	{
		DeleteVRAMIOReq(gVRAMIOReq);
	
		gVRAMIOReq	= -1;
	}
}

void UpdateDisplay()
{
	CopyVRAMPages(gVRAMIOReq, gScreenContext->sc_Bitmaps[gCurrentScreen]->bm_Buffer, gBackground, gScreenContext->sc_nFrameBufferPages, 
		0xFFFFFFFF);

	UpdateSprites(gCurrentScreen);
	
	DisplayScreen(gScreenContext->sc_Screens[gCurrentScreen], 0);

	gCurrentScreen	= 1 - gCurrentScreen;
}

bool LoadBackground(char *szBackground)
{
	char	szFilename[128];

	PRT(("Loading background %s\n", szBackground));

	// Allocate VRAM memory for the background
	gBackground	= (ubyte *)AllocMem((int)(gScreenContext->sc_nFrameByteCount), MEMTYPE_STARTPAGE | MEMTYPE_VRAM | MEMTYPE_CEL);

	if (NULL == gBackground)
	{
		PRT(("Can't allocate memory for background\n"));
        
        return	false;
	}

	// Clear background VRAM
	SetVRAMPages(gVRAMIOReq, gBackground, 0, gScreenContext->sc_nFrameBufferPages, -1);

	// Load the background file from the correct directory
	GetArtFilename(szBackground, szFilename);
	
	// Load background image into the background
	if (0 == LoadImage(szFilename,  gBackground, (VdlChunk **)NULL, gScreenContext))
	{
		PRT(("Can't load image %s\n", szFilename));
	
		return	false;
	}

	return	true;
}

void GetArtFolder(char *szFolderPath)
{
	if (PAL_DISPLAY(gDisplayType))
	{
		strcpy(szFolderPath, PAL_FOLDER);
	}
	
	else
	{
		strcpy(szFolderPath, NTSC_FOLDER);
	}
	
	strcat(szFolderPath, ART_FOLDER);
}

void GetArtFilename(char *szFilename, char *szFullPathname)
{
	GetArtFolder(szFullPathname);
	strcat(szFullPathname, szFilename);
}

bool isNTSC()
{
	if (PAL_DISPLAY(gDisplayType))
	{
		return	false;
	}
	
	return	true;
}