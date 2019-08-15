// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include "debug3do.h"

#include "Game.h"
#include "System.h"

int main(int argc, char** argv)
{
	PRT(("Game started\n"));
	
	// Initialize the display
	if (false == InitializeDisplay())
	{
		return	0;
	}
		
	// Initialize the control pads
	if (false == InitializeControlPads())
	{
		return	0;
	}
			
	// Initialize the game
	if (false == InitializeGame())
	{
		return	0;
	}
			
	while (false == GameLoop())
	{
	}
	
	// Cleanup
	DestroyControlPads();
	DestroyDisplay();
	
	return	0;
}