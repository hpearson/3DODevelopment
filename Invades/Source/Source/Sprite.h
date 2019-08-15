// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#ifndef __SPRITE_H
#define __SPRITE_H

#include "types.h"
#include "animutils.h"
#include "graphics.h"

// Maximum number of sprites
#define	MAX_SPRITES	8

// Position corners
enum eCorners
{
	eTopLeft,
	eTopRight,
	eBottomRight,
	eBottomLeft
};

// Sprite element
typedef struct
{
	// Position
	Point	ptPosition;
	
	// Sprite cel/anim
	ANIM	*pAnim;

	// Sprite CCB
	CCB		*pCCB;
	
	// Is this sprite used
	bool	bUsed;

	// Show the sprite
	bool	bShow;
} sSprite;

// Initialize sprites
void InitializeSprites();

// Update sprites
void UpdateSprites(int iCurrentScreen);

// Add sprite
int	AddSprite(char *szCelFile);

// Remove sprite
void RemoveSprite(int iIndex);

// Set sprite position
void SetSpritePosition(int iIndex, int iX, int iY);

// Set sprite frame
void SetSpriteFrame(int iIndex, int iFrame);

// Set sprite visibility
void SetSpriteVisible(int iIndex, bool bVisible);

// Find available sprite
int FindAvailableSprite();

// Set common CCB
CCB *SetCommonCCB(ANIM* pAnim);

// Get sprite width
int GetSpriteWidth(int iIndex);

// Get sprite height
int GetSpriteHeight(int iIndex);

#endif