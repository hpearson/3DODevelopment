#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	WORDSwap(type) (((type >> 8) & 0x00FF) | ((type << 8)  & 0xFF00))
#define	DWORDSwap(type) (((type >> 24) & 0x000000FF) | ((type >> 8) & 0x0000FF00) | ((type << 8)  & 0x00FF0000) | ((type << 24) & 0xFF000000))

DWORD	Width;
DWORD	Height;
BYTE	*BmpBuffer;

BYTE	BannerHeader[]	= {0x01, 0x41, 0x50, 0x50, 0x53, 0x43, 0x52, 0x4E};
BYTE	BannerFlags[]	= {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
BYTE	BannerSignature[64];

// Banner sizes
static const int	s_iBannerWidth	= 320;
static const int	s_iBannerHeight	= 240;

void Usage();
bool Convert(char *Filename, char *ImageFilename);
bool LoadBMP(char *Filename);

int main(int argc, char *argv[])
{
	printf("MakeBanner v1.0b\n");
	printf("\tBy Charles Doty\n");

	if (argc < 3)
	{
		Usage();

		return	1;
	}

	if (false == Convert(argv[1], argv[2]))
	{
		return	1;
	}
	
	return	0;
}

void Usage()
{
	printf("Usage: MakeBanner Infile.bmp Outfile.banner\n");
}

bool Convert(char *Filename, char *ImageFilename)
{
	// Load bitmap, into BmpBuffer, and BmpPalette. Memory is allocated in LoadBMP.
	if (false == LoadBMP(Filename))
	{
		return	false;
	}

	DWORD	*WriteBuffer	= new DWORD[Width * Height / 2];

	if (NULL == WriteBuffer)
	{
		printf("Unable to allocate memory.\n");
		
		return	false;
	}

	for (DWORD OuterLoop = 0; OuterLoop < Height; OuterLoop += 2)
	{
		for (DWORD InnerLoop = 0; InnerLoop < Width; InnerLoop++)
		{
			DWORD	Value;

			BYTE	Red		= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3] >> 3);
			BYTE	Green	= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3 + 1] >> 3);
			BYTE	Blue	= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3 + 2] >> 3);

			Value	= ((DWORD)((Red << 10) | (Green << 5) | Blue));
			Value <<= 16;
			
			Red		= (BmpBuffer[((OuterLoop + 1) * Width + InnerLoop) * 3] >> 3);
			Green	= (BmpBuffer[((OuterLoop + 1) * Width + InnerLoop) * 3 + 1] >> 3);
			Blue	= (BmpBuffer[((OuterLoop + 1) * Width + InnerLoop) * 3 + 2] >> 3);

			Value	|= ((DWORD)((Red << 10) | (Green << 5) | Blue));

			WriteBuffer[(OuterLoop / 2) * Width + InnerLoop]	= DWORDSwap(Value);
		}
	}

	// Write image file
	FILE	*Handle	= fopen(ImageFilename, "wb");
	
	if (NULL == Handle)
	{
		printf("Unable to create image file %s.\n", ImageFilename);
	
		return	false;
	}
	
	// Write banner header
	fwrite(BannerHeader, sizeof(BYTE), sizeof(BannerHeader), Handle);
	
	// Write size
	DWORD	Value	= Width * Height * 2;
	Value	= DWORDSwap(Value);
	
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// Write height
	WORD	Size	= (WORD)Height;
	Size	= WORDSwap(Size);
	
	fwrite(&Size, sizeof(WORD), 1, Handle);

	// Write width
	Size	= (WORD)Width;
	Size	= WORDSwap(Size);
	
	fwrite(&Size, sizeof(WORD), 1, Handle);

	// Write banner flags
	fwrite(BannerFlags, sizeof(BYTE), sizeof(BannerFlags), Handle);
	
	// Write converted graphics
	fwrite(WriteBuffer, sizeof(WORD), Width * Height, Handle);
	
	// Write signature block
	memset(BannerSignature, 0, sizeof(BannerSignature));

	fwrite(BannerSignature, sizeof(BYTE), sizeof(BannerSignature), Handle);

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

	if (Width != s_iBannerWidth && Height != s_iBannerHeight)
	{
		printf("Banner must be %d by %d pixels.\n", s_iBannerWidth, s_iBannerHeight);

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
