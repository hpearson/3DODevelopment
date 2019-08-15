// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include "stdio.h"
#include "string.h"

#include "controlpad.h"
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

// Number of control pads to use
static const int	sNumControlPads	= 2;

bool InitializeControlPads()
{
	PRT(("Initializing the control pads\n"));

	// Initialize the control pads
	if (InitControlPad(sNumControlPads) <  0)
	{
		PRT(("Can't initialize the control pad\n"));
	
		return	false;
	}

	return	true;
}

void DestroyControlPads()
{
	KillControlPad();
}

void Update()
{
	UpdateDisplay();
}
