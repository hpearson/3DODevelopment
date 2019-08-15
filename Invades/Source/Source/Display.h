// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "types.h"

// Initialize the display
bool InitializeDisplay();

// Destroy the display
void DestroyDisplay();

// Update display
void UpdateDisplay();

// Load background
bool LoadBackground(char *szBackground);

// Get art folder
void GetArtFolder(char *szFolderPath);

// Get art filename
void GetArtFilename(char *szFilename, char *szFullPathname);

// Is NTSC display?
bool isNTSC();

#endif