// RasterSoft Invades 3DO demo by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include "Sprite.h"
#include "System.h"

#include "celutils.h"
#include "debug3do.h"
#include "displayutils.h"
#include "mem.h"

// Sprite list
sSprite	SpriteList[MAX_SPRITES];

// Screen context
extern	ScreenContext	*gScreenContext;

void InitializeSprites()
{
	int	iLoop;
	
	PRT(("Initialize sprites\n"));

	// Set all sprites to unused
	for (iLoop = 0; iLoop < MAX_SPRITES; ++iLoop)
	{
		SpriteList[iLoop].pAnim	= NULL;
		SpriteList[iLoop].bUsed	= false;
		SpriteList[iLoop].bShow	= false;
	}
}

void UpdateSprites(int iCurrentScreen)
{
	int	iLoop;
	
	Point	ptQuad[4];

	Item	itmBitmapItems	= gScreenContext->sc_BitmapItems[iCurrentScreen];
	
	// Draw each cel
	for (iLoop = 0; iLoop < MAX_SPRITES; ++iLoop)
	{
		if (true == SpriteList[iLoop].bUsed && SpriteList[iLoop].pAnim	!= NULL && true == SpriteList[iLoop].bShow)
		{
			int	iX	= SpriteList[iLoop].ptPosition.pt_X;
			int	iY	= SpriteList[iLoop].ptPosition.pt_Y;

			CCB	*pCCB	= SpriteList[iLoop].pCCB;
			
			pCCB	= GetAnimCel(SpriteList[iLoop].pAnim, 0);
			
			ptQuad[eTopLeft].pt_X		= iX;
			ptQuad[eTopLeft].pt_Y		= iY;
			ptQuad[eTopRight].pt_X		= iX + pCCB->ccb_Width;
			ptQuad[eTopRight].pt_Y		= iY;
			
			ptQuad[eBottomLeft].pt_X	= iX;
			ptQuad[eBottomLeft].pt_Y	= iY + pCCB->ccb_Height;
			ptQuad[eBottomRight].pt_X	= iX + pCCB->ccb_Width;
			ptQuad[eBottomRight].pt_Y	= iY + pCCB->ccb_Height;

			MapCel(pCCB, ptQuad);
			LAST_CEL(pCCB);

			PRT(("Drawing cel %d %d %d\n", iLoop, iX, iY));
			
			DrawCels(itmBitmapItems, pCCB);
		}
	}
}

int	AddSprite(char *szCelFile)
{
	char	szFilename[128];
	
	// Find the next available sprite
	int	iSpriteIndex	= FindAvailableSprite();

	if (iSpriteIndex >= 0)
	{
		SpriteList[iSpriteIndex].bUsed	= true;

		GetArtFilename(szCelFile, szFilename);
			
		PRT(("Add sprite %s\n", szFilename));

		// Setup the animation
		SpriteList[iSpriteIndex].pAnim	= LoadAnim(szFilename, MEMTYPE_CEL);
	
		if (NULL == SpriteList[iSpriteIndex].pAnim)
		{
			PRT(("Unable to load animation %s\n", szFilename));
		
			return	-1;
		}

		SpriteList[iSpriteIndex].pCCB	= SetCommonCCB(SpriteList[iSpriteIndex].pAnim);
	}

	return	iSpriteIndex;
}

void RemoveSprite(int iIndex)
{
	// Remove a sprite by setting it to unused.
	SpriteList[iIndex].bUsed	= false;

	// Unload the animation
	if (SpriteList[iIndex].pAnim != NULL)
	{
		UnloadAnim(SpriteList[iIndex].pAnim);
		
		SpriteList[iIndex].pAnim	= NULL;
	}
}

void SetSpritePosition(int iIndex, int iX, int iY)
{
	SpriteList[iIndex].ptPosition.pt_X	= iX;
	SpriteList[iIndex].ptPosition.pt_Y	= iY;
}

void SetSpriteVisible(int iIndex, bool bVisible)
{
	SpriteList[iIndex].bShow	= bVisible;
}

void SetSpriteFrame(int iIndex, int iFrame)
{
	SpriteList[iIndex].pAnim->cur_Frame	= Convert32_F16(iFrame);
}

int FindAvailableSprite()
{
	int	iLoop;
	
	for (iLoop = 0; iLoop < MAX_SPRITES; ++iLoop)
	{
		if (false == SpriteList[iLoop].bUsed)
		{
			return	iLoop;
		}
	}

	PRT(("Unable to find an available sprite\n"));
	
	return	-1;
}

CCB *SetCommonCCB(ANIM* pAnim)
{
	int		iFrameIndex;
	CCB*	pCCB	= pAnim->pentries[0].af_CCB;
	
	for (iFrameIndex = 0; iFrameIndex < pAnim->num_Frames; iFrameIndex++)
	{
		pAnim->pentries[iFrameIndex].af_CCB	= pCCB;
	}

	return	pCCB;
}

int GetSpriteWidth(int iIndex)
{
	return	SpriteList[iIndex].pCCB->ccb_Width;
}

int GetSpriteHeight(int iIndex)
{
	return	SpriteList[iIndex].pCCB->ccb_Height;
}