// RasterSoft 3DO tools by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#include "CCB.h"

#define	WORDSwap(type) (((type >> 8) & 0x00FF) | ((type << 8)  & 0xFF00))
#define	DWORDSwap(type) (((type >> 24) & 0x000000FF) | ((type >> 8) & 0x0000FF00) | ((type << 8)  & 0x00FF0000) | ((type << 24) & 0xFF000000))

DWORD	Width;
DWORD	Height;
DWORD	FrameWidth;
DWORD	FrameHeight;
BYTE	*BmpBuffer;

void Usage();
bool Convert(char *Filename, char *ImageFilename);
bool LoadBMP(char *Filename);
void GetTile(int iX, int iY, int iWidth, int iHeight, WORD* pSource, WORD* pDestination);

int main(int argc, char *argv[])
{
	printf("BMPTo3DOAnim v0.6a\n");
	printf("\tBy Charles Doty\n");

	if (argc < 5)
	{
		Usage();

		return	1;
	}

	FrameWidth	= atol(argv[1]);
	FrameHeight	= atol(argv[2]);
	
	if ((FrameHeight % 2) != 0)
	{
		printf("Frame height must be a multiple of 2\n");
		
		return	1;
	}
	
	if (false == Convert(argv[3], argv[4]))
	{
		return	1;
	}
	
	return	0;
}

void Usage()
{
	printf("Usage: BMPTo3DOAnim FrameWidth FrameHeight Infile.bmp Outfile.anim\n");
}

bool Convert(char *Filename, char *ImageFilename)
{
	// Load bitmap into BmpBuffer. Memory is allocated in LoadBMP.
	if (false == LoadBMP(Filename))
	{
		return	false;
	}

	if ((Width % FrameWidth) != 0)
	{
		printf("Width must be a multiple of frame width\n");
		
		return	false;
	}
	
	if ((Height % FrameHeight) != 0)
	{
		printf("Height must be a multiple of frame height\n");
		
		return	false;
	}
	
	WORD	*WriteBuffer	= new WORD[Width * Height];

	if (NULL == WriteBuffer)
	{
		printf("Unable to allocate memory.\n");
		
		return	false;
	}

	// Convert the buffer to 15 bit
	for (DWORD OuterLoop = 0; OuterLoop < Height; OuterLoop++)
	{
		for (DWORD InnerLoop = 0; InnerLoop < Width; InnerLoop++)
		{
			WORD	Value;

			BYTE	Red		= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3] >> 3);
			BYTE	Green	= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3 + 1] >> 3);
			BYTE	Blue	= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3 + 2] >> 3);

			Value	= ((WORD)((Red << 10) | (Green << 5) | Blue));

			// Don't word swap here, it's done in the tile routine
			WriteBuffer[OuterLoop * Width + InnerLoop]	= Value;
		}
	}

	sCCB	CCBChunk;
	
	// Set CCB chunk ID
	CCBChunk.chunk_ID	= ' BCC';
	
	// Set chunk size
	DWORD	Value	= sizeof(sCCB);
	CCBChunk.chunk_size	= DWORDSwap(Value);
	
	// Set version
	CCBChunk.ccbversion	= 0;
	
	// Set flags. See 5gpgl.html. End of linked sprites.
	// HDx, VDx, and DDx values are includes.
	// Preamble words are in CCB, and not source data. Write coordinates to cel engine.
	// Enable clockwise and counterclockwise rendering.
	Value	= 0x47660000;
	CCBChunk.ccb_Flags	= DWORDSwap(Value);
	
	// Pointers are initialized after load
	CCBChunk.ccb_NextPtr	= NULL;
	CCBChunk.ccb_CelData	= NULL;
	CCBChunk.ccb_PIPPtr		= NULL;
	
	// Set source x and y to 0.
	CCBChunk.ccb_X	= 0;
	CCBChunk.ccb_Y	= 0;
	
	// Set horiz offsets (in 12.20 format)
	Value	= 1 << 20;
	CCBChunk.ccb_hdx	= DWORDSwap(Value);
	CCBChunk.ccb_hdy	= 0;
	
	// Set vertical offsets (in 16.16 format)
	CCBChunk.ccb_vdx	= 0;
	Value	= 1 << 16;
	CCBChunk.ccb_vdy	= DWORDSwap(Value);
	
	// Set horiz and vertical changes.
	CCBChunk.ccb_ddx	= 0;
	CCBChunk.ccb_ddy	= 0;
	
	// Set PIXC. See 5gpgl.html. Not used for normal images
	Value	= 0x1F001F00;
	CCBChunk.ccb_PPMPC	= DWORDSwap(Value);
	
	// Set PRE0. See 5gpge.html. 0x1E sets unencoded and 16 bit. No initial skip value is set.
	// There are half as many rows in a buffer formatted image
	Value	= ((FrameHeight / 2 - 1) << 6) | 0x1E;
	
	CCBChunk.ccb_PRE0	= DWORDSwap(Value);
	
	// Set PRE1. See 5gpge.html.
	// Set the (32 bit) words between rows - 2. Using the buffer format has two rows of data. 
	// Blue repeat settings.
	// Pixels in buffer format and pixels per row - 1.
	Value	= ((FrameWidth - 2) << 16) | (1 << 12) | 0x800 | (FrameWidth - 1);
	CCBChunk.ccb_PRE1	= DWORDSwap(Value);
	
	// Set width and height
	Value	= FrameWidth;
	CCBChunk.ccb_Width	= DWORDSwap(Value);

	Value	= FrameHeight;
	CCBChunk.ccb_Height	= DWORDSwap(Value);
	
	// Write image file
	FILE	*Handle	= fopen(ImageFilename, "wb");
	
	if (NULL == Handle)
	{
		printf("Unable to create image file %s.\n", ImageFilename);
	
		return	false;
	}
	
	// Write CCB
	fwrite(&CCBChunk, sizeof(CCBChunk), 1, Handle);
	
	WORD	*pTile	= new WORD[FrameWidth * FrameHeight];
			
	if (NULL == pTile)
	{
		printf("Unable to allocate memory for sprite frame\n");
	
		return	false;
	}
	
	for (DWORD iOuterLoop = 0; iOuterLoop < Height / FrameHeight; iOuterLoop++)
	{
		for (DWORD iInnerLoop = 0; iInnerLoop < Width / FrameWidth; iInnerLoop++)
		{
			GetTile(iInnerLoop * FrameWidth, iOuterLoop * FrameHeight, FrameWidth, FrameHeight, (WORD *)WriteBuffer, pTile),
			
			// Write image tag (PDAT)
			Value	= 'TADP';
			fwrite(&Value, sizeof(DWORD), 1, Handle);

			int	iExtraBit	= (Width * Height * 2) % 4;
			
			// Write chunk size.
			Value	= FrameWidth * FrameHeight * 2 + 8;
			
			if (iExtraBit != 0)
			{
				// Pad out to DWORD.
				Value	+= 2;
			}
			
			// Write buffer size
			Value	= DWORDSwap(Value);	
			fwrite(&Value, sizeof(DWORD), 1, Handle);
			
			// Write converted graphics
			fwrite(pTile, sizeof(WORD), FrameWidth * FrameHeight, Handle);
			
			if (iExtraBit != 0)
			{
				// Write an extra word to pad out to DWORD.
				fwrite(WriteBuffer, sizeof(WORD), 1, Handle);
			}
		}
	}

	delete [] pTile;
	
	fclose(Handle);	

	return	true;
}

bool LoadBMP(char *Filename)
{
	BYTE	*FileBuffer	= NULL;

	FILE	*Handle	= fopen(Filename, "rb");

	if (NULL == Handle)
	{
		printf("Unable to load BMP %s.\n", Filename);

		return	false;
	}
		
	fseek(Handle, 0, SEEK_END);

	DWORD	Length	= ftell(Handle);
	rewind(Handle);

	FileBuffer	= new BYTE[Length];

	if (NULL == FileBuffer)
	{
		printf("Unable to allocate memory.\n");

		return	false;
	}
	
	fread(FileBuffer, 1, Length, Handle);

	fclose(Handle);

	BYTE	*FilePointer	= FileBuffer;

	BITMAPFILEHEADER	*FileHeader	= (BITMAPFILEHEADER *)FilePointer;
	DWORD	BitmapSize				= FileHeader->bfSize;
	DWORD	Offset					= FileHeader->bfOffBits;

	FilePointer	+=	sizeof(BITMAPFILEHEADER);

	BITMAPINFO	*BmpInfo	= (BITMAPINFO *)FilePointer;

	if (BmpInfo->bmiHeader.biCompression != BI_RGB || BmpInfo->bmiHeader.biBitCount != 24)
	{
		printf("BMP is not 24 bit, or is compressed.\n");

		return	false;
	}

	long	BMPWidth	= BmpInfo->bmiHeader.biWidth;
	long	BMPHeight	= BmpInfo->bmiHeader.biHeight;
	long	Increment	= (BMPWidth * BmpInfo->bmiHeader.biBitCount / 8);

	FilePointer	= FileBuffer + Offset;
		
	if (BMPHeight > 0)
	{
		Increment	= -(BMPWidth * 2 * BmpInfo->bmiHeader.biBitCount / 8);
		FilePointer	+= (BMPWidth * BMPHeight * BmpInfo->bmiHeader.biBitCount / 8) - 
			(BMPWidth * BmpInfo->bmiHeader.biBitCount / 8);
	}

	else
	{
		BMPHeight		= -BMPHeight;
	}

	Width	= BMPWidth;
	Height	= BMPHeight;

	if ((Width % 4) != 0)
	{
		printf("Width must be a multiple of 4\n");

		return	false;
	}
	
	if ((Height % 2) != 0)
	{
		printf("Height must be a multiple of 2\n");

		return	false;
	}
	
	BmpBuffer	= new BYTE[Width * Height * 3];

	if (NULL == BmpBuffer)
	{
		printf("Unable to allocate memory.\n");

		return	false;
	}
	
	BYTE	*TmpBuffer	= BmpBuffer;

	for (long OuterLoop = 0; OuterLoop < abs(BMPHeight); OuterLoop++)
	{
		for (long InnerLoop = 0; InnerLoop < BMPWidth; InnerLoop++)			
		{		
			BYTE	Blue	= FilePointer[OuterLoop * BMPWidth * 3 + InnerLoop * 3 + 0];
			BYTE	Green	= FilePointer[OuterLoop * BMPWidth * 3 + InnerLoop * 3 + 1];
			BYTE	Red		= FilePointer[OuterLoop * BMPWidth * 3 + InnerLoop * 3 + 2];
			
			// Handle tranparent pixel.
			if (255 == Red && 0 == Green && 255 == Blue)
			{
				Red		= 0;
				Green	= 0;
				Blue	= 0;
			}
			
			// Handle near black pixel. Add a slight bit to the red so that it's not treated as black.
			// Value will be converted to 5 bit.
			else if (Red < 8 && Green < 8 && Blue < 8)
			{
				Red	= 1 << 3;
			}
			
			*TmpBuffer	= Red;
			TmpBuffer++;

			*TmpBuffer	= Green;
			TmpBuffer++;

			*TmpBuffer	= Blue;
			TmpBuffer++;
		}

		FilePointer	+= Increment;
	}
		
	if (FileBuffer != NULL)
	{
		delete [] FileBuffer;

		FileBuffer	= NULL;
	}

	return	true;
}

void GetTile(int iX, int iY, int iWidth, int iHeight, WORD* pSource, WORD* pDestination)
{
	WORD*	pSrc	= (pSource + iY * Width + iX);
	DWORD*	pDest	= (DWORD*)pDestination;
	
	// Grab a tile, and convert it to 3DO buffer format.
	for (int iOuterLoop = 0; iOuterLoop < iHeight / 2; iOuterLoop++)
	{
		for (int iInnerLoop = 0; iInnerLoop < iWidth; iInnerLoop++)
		{
			DWORD	Value	= ((DWORD)*pSrc) << 16;
			Value	|= ((DWORD)*(pSrc + Width));
			
			*pDest	= DWORDSwap(Value);
			
			pSrc++;
			pDest++;
		}
	
		// Skip to the start of the 2nd line below the current one.
		pSrc	+= Width * 2 - iWidth;
	}
}