// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#ifndef __GAME_H
#define __GAME_H

#include "types.h"

// Initialize game
bool InitializeGame();

// Game loop
bool GameLoop();

// Show rastersoft
void ShowRasterSoft();

// Show invades
void ShowInvades();

// Show the
void ShowThe();

// Show 3DO
void Show3DO();

// Show alien
void ShowAlien();

// Move alien 
void MoveAlien();

// Reset
void Reset();

// Read control pad
void ReadControlPad();

#endif