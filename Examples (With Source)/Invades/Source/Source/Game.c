// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include "controlpad.h"
#include "debug3do.h"
#include "event.h"

#include "Game.h"
#include "Display.h"
#include "Sprite.h"
#include "System.h"

// Joystick buttons
uint32	gButtons;

// Background image
static char*	sBackgroundImage	= "Background.imag";

// RasterSoft cel
static char*	sRasterSoft			= "RasterSoft.cel";

// Invades cel
static char*	sInvades			= "Invades.cel";

// The cel
static char*	sThe				= "The.cel";

// 3DO cel
static char*	s3DO				= "3DO.cel";

// Alien anim
static char*	sAlien				= "Alien.anim";

// RasterSoft sprite index
int	gRasterSoftSprite;

// Invades sprite index
int	gInvadesSprite;

// The sprite index
int	gTheSprite;

// 3DO sprite index
int	g3DOSprite;

// Alien sprite index
int	gAlienSprite;

// Alien position
int	gAlienX;
int	gAlienY;

int	gStartAlienX;
int	gStartAlienY;

// Screen bounds
int	gTopY;
int	gBottomY;
int gLeftX;
int gRightX;

// Movement speed
int	gMoveX	= 2;
int	gMoveY	= 1;

// Animation
int	sAnimDelay		= 6;
int	gAnimDelay;
int gAnimFrame		= 0;
int sMaxAnimFrame	= 4;
int	gFacing			= 0;

// Demo status
int gStatus;
int gDelay;
int sDelay	= 30;

bool InitializeGame()
{
	int	iYPos	= 16;
	int	iScreenCenterX;
	
	PRT(("Initializing the game\n"));

	if (isNTSC())
	{
		iScreenCenterX	= 320 /2;
	}
	
	else
	{
		iScreenCenterX	= 384 /2;
		iYPos			+= 24;
	}
	
	// Load a background image
	if (false == LoadBackground(sBackgroundImage))
	{
		return	false;
	}
			
	// Initialize the sprites
	InitializeSprites();
	
	// Add RasterSoft sprite
	gRasterSoftSprite	= AddSprite(sRasterSoft);
	
	SetSpritePosition(gRasterSoftSprite, iScreenCenterX - (GetSpriteWidth(gRasterSoftSprite) >> 1), iYPos);
	
	iYPos	+= 44;

	// Add invades sprite
	gInvadesSprite		= AddSprite(sInvades);
	
	SetSpritePosition(gInvadesSprite, iScreenCenterX - (GetSpriteWidth(gInvadesSprite) >> 1), iYPos);
	
	iYPos	+= 50;

	// Add the sprite
	gTheSprite			= AddSprite(sThe);
	
	SetSpritePosition(gTheSprite, iScreenCenterX - (GetSpriteWidth(gTheSprite) >> 1), iYPos);
	
	iYPos	+= 38;

	// Add 3DO sprite
	g3DOSprite			= AddSprite(s3DO);
	
	SetSpritePosition(g3DOSprite, iScreenCenterX - (GetSpriteWidth(g3DOSprite) >> 1), iYPos);
	
	iYPos	+= 36;

	// Add alien sprite
	gAlienSprite			= AddSprite(sAlien);
	
	gAlienX	= iScreenCenterX - (GetSpriteWidth(gAlienSprite) >> 1);
	gAlienY = iYPos;
		
	gStartAlienX	= gAlienX;
	gStartAlienY	= gAlienY;
	
	SetSpritePosition(gAlienSprite, gAlienX, gAlienY);
	SetSpriteFrame(gAlienSprite, gAnimFrame + (gFacing << 2));
	
	if (isNTSC())
	{
		gTopY		= 160;
		gBottomY	= 240 - GetSpriteHeight(gAlienSprite);
		gLeftX		= 0;
		gRightX		= 320 - GetSpriteWidth(gAlienSprite);
	}
	
	else
	{
		gTopY		= 160 + 24;
		gBottomY	= 240 + 24 - GetSpriteHeight(gAlienSprite);
		gLeftX		= 0;
		gRightX		= 384 - GetSpriteWidth(gAlienSprite);
	}

	gAnimDelay	= sAnimDelay;
	gStatus		= 0;
	gDelay		= sDelay;
	
	return	true;
}

bool GameLoop()
{
	if (gDelay > 0)
	{
		gDelay--;
	}
	
	if (0 == gDelay)
	{
		switch (gStatus)
		{
			case 0:
				ShowRasterSoft();
				
				break;

			case 1:
				ShowInvades();
				
				break;

			case 2:
				ShowThe();
				
				break;

			case 3:
				Show3DO();
				
				break;
				
			case 4:
				ShowAlien();
			
			case 5:
				MoveAlien();
		}
	}
	
	Update();
	
	return	false;
}

void ShowRasterSoft()
{
	SetSpriteVisible(gRasterSoftSprite, true);
	
	gDelay	= sDelay;
	gStatus++;
}

void ShowInvades()
{
	SetSpriteVisible(gInvadesSprite, true);
	
	gDelay	= sDelay;
	gStatus++;
}

void ShowThe()
{
	SetSpriteVisible(gTheSprite, true);
	
	gDelay	= sDelay;
	gStatus++;
}

void Show3DO()
{
	SetSpriteVisible(g3DOSprite, true);
	
	gDelay	= sDelay;
	gStatus++;
}

void ShowAlien()
{
	SetSpriteVisible(gAlienSprite, true);
	
	gStatus++;
}

void MoveAlien()
{	
	bool	bMoving	= false;
	
	ReadControlPad();
	
	if (gButtons & ControlStart)
	{
		Reset();
	}
	
	else if (gButtons & ControlUp)
	{
		if (gAlienY >= gTopY + gMoveY)
		{
			gAlienY	-= gMoveY;
		
			// Set the sprite group
			gFacing	= 3;
		
			// Moving, update the animation
			bMoving	= true;
		}
	}
	
	else if (gButtons & ControlDown)
	{
		if (gAlienY <= gBottomY - gMoveY)
		{
			gAlienY	+= gMoveY;
		
			// Set the sprite group
			gFacing	= 1;

			// Moving, update the animation
			bMoving	= true;
		}
	}
	
	if (gButtons & ControlLeft)
	{
		if (gAlienX >= gLeftX + gMoveX)
		{
			gAlienX	-= gMoveX;
		
			// Set the sprite group
			gFacing	= 0;

			// Moving, update the animation
			bMoving	= true;
		}
	}
	
	else if (gButtons & ControlRight)
	{
		if (gAlienX <= gRightX - gMoveX)
		{
			gAlienX	+= gMoveX;
		
			// Set the sprite group
			gFacing	= 2;

			// Moving, update the animation
			bMoving	= true;
		}
	}

	if (true == bMoving)
	{
		gAnimDelay--;
		
		if (0 == gAnimDelay)
		{
			gAnimDelay	= sAnimDelay;
			
			gAnimFrame++;
			
			if (gAnimFrame >= sMaxAnimFrame)
			{
				gAnimFrame	= 0;
			}
		}
	}
	
	SetSpritePosition(gAlienSprite, gAlienX, gAlienY);
	SetSpriteFrame(gAlienSprite, gAnimFrame + (gFacing << 2));
}

void Reset()
{
	gAnimDelay	= sAnimDelay;
	gStatus		= 0;
	gDelay		= sDelay;

	gAlienX	= gStartAlienX;
	gAlienY = gStartAlienY;
		
	gAnimFrame	= 0;
	gFacing		= 0;
	
	SetSpritePosition(gAlienSprite, gAlienX, gAlienY);
	SetSpriteFrame(gAlienSprite, gAnimFrame + (gFacing << 2));
	
	SetSpriteVisible(gRasterSoftSprite, false);
	SetSpriteVisible(gInvadesSprite, false);
	SetSpriteVisible(gTheSprite, false);
	SetSpriteVisible(g3DOSprite, false);
	SetSpriteVisible(gAlienSprite, false);
}

void ReadControlPad()
{
 	// Read the first pad
 	int	iStatus	= DoControlPad(1, &gButtons, (ControlUp | ControlDown | ControlLeft | ControlRight));
}
