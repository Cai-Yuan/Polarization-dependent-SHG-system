#include "stdafx.h"

#include "tiff.h"
#include "tiffio.h"

#include "ReadTIFF.h"

UINT32 ReadTIFFWidth(CHAR *fileName)
{

	UINT32 width = 0;

#ifndef _WIN64

	TIFF *tif = TIFFOpen(fileName, "r");

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width); 
	
	TIFFClose(tif);

#endif

	return width;
}

UINT32 ReadTIFFHeight(CHAR *fileName)
{
	UINT32 height = 0;

#ifndef _WIN64

	TIFF *tif = TIFFOpen(fileName, "r");

	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);

	TIFFClose(tif);

#endif

	return height;
}

BOOL VerifyStackDimensionsTIFF(FILEPATHSET *fps)
{

	LONG w = 0;
	LONG h = 0;
	UINT nPaths = fps->nPaths;
	UINT i;

	w = ReadTIFFWidth(fps->filePath[0].lpstr);
	h = ReadTIFFHeight(fps->filePath[0].lpstr);

	for(i = 0+1; i < nPaths; i++){

		if(w != ReadTIFFWidth(fps->filePath[i].lpstr) || h != ReadTIFFHeight(fps->filePath[i].lpstr))
			return FALSE;

	}



	return TRUE;
}

//Compresses an image that uses 2-Bytes per pixel to 1-Byte Per Pixel
VOID CompressTIFF2(UINT w, UINT h, UCHAR *imgSrc, UCHAR *imgDest)
{

	if(!imgSrc || !imgDest)return;
	UINT i = 0;
	UINT p = 0;
	INT lByte;
	INT bByte;
	UINT max = w*h;
	FLOAT temp;

	SHORT *img = (SHORT *) imgSrc;

	for(i = 0, p = 0; i < max; i++, p+=2)
	{
		temp = (FLOAT)img[i];

		imgDest[i] = (UCHAR) (temp*(255.0/4096.0));
		//imgDest[i] = (UCHAR) imgSrc[p/*+1*/];
	}

	return;
}

//Compresses an image that uses 2-Bytes per pixel to 1-Byte Per Pixel
VOID CompressTIFF3(UINT w, UINT h, UCHAR *imgSrc, UCHAR *imgDest)
{

	if(!imgDest || !imgSrc)return;
	UINT i = 0;
	UINT p = 0;
	INT lByte;
	INT bByte;
	UINT max = w*h;
	FLOAT temp;

	for(i = 0, p = 0; i < max; i++, p+=3)
	{
		//lByte = imgSrc[p];
		//bByte = imgSrc[p+1];
		//lByte = lByte/*<<8*/;
		//bByte = bByte;
		//temp = bByte;
		imgDest[i] = (UCHAR) imgSrc[p+3]/*temp*/;
		//imgDest[i] = (UCHAR) temp;
	}

	return;
}

////Compresses an image that uses 2-Bytes per pixel to 1-Byte Per Pixel
//VOID CompressTIFF(UINT w, UINT h, UCHAR *imgSrc, UCHAR *imgDest)
//{
//
//	UINT i = 0;
//	UINT p = 0;
//	INT lByte;
//	INT bByte;
//	UINT max = w*h;
//	FLOAT temp;
//
//	if(imgDest && imgSrc){
//
//		for(i = 0, p = 0; i < max; i++, p+=2)
//		{
//			//lByte = imgSrc[p];
//			//bByte = imgSrc[p+1];
//			//lByte = lByte/*<<8*/;
//			//bByte = bByte;
//			//temp = bByte;
//			imgDest[i] = (UCHAR) imgSrc[p+1]/*temp*/;
//			//imgDest[i] = (UCHAR) temp;
//		}
//	}
//}