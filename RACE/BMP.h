#pragma once

#include "stdafx.h"
#include "Archive.h"
#include "ImageConversion.h"

#define PTU_LUMINOSITY		0		//0.21 R + 0.71 G + 0.07 B.
#define PTU_AVERAGE			1		//(R + G + B)/3
#define PTU_LIGHTNESS		2		//(Max(R, G, B) + Min(R, G, B)) / 2

VOID SaveBMP(UINT w, UINT h, LPCSTR lpcstrFileName, PIXEL *bmp);
VOID SaveBMP(UINT w, UINT h, LPCSTR lpcstrFileName, UCHAR *img);
VOID SaveBMP(UINT w, UINT h, LPCSTR lpcstrFileName, FLOAT *img);

DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, VOID *lpvoid);
DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, VOID **lpvoid);
DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, UINT *nBytePerPixel, VOID **lpvoid);

DWORD GetBMPSize(CHAR *filePath, UINT *width, UINT *height);
DWORD FlipHorzBMP(UINT w, UINT h, PIXEL *bmp);

DWORD WINAPI SaveSequenceBMP(VOID *lpvoid);

DWORD BytesPerPixel(DWORD nBits);

DWORD GetSimilarChannelsBMP(UINT width, UINT height, PIXEL *bmp);


DWORD ReadBMPOpenCV(CHAR *filePath, UINT *width, UINT *height, VOID *lpvoid);