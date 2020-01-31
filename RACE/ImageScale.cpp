#include "stdafx.h"

#include "ImageScale.h"
#include "Point.h"

DWORD GetScaleDimensions(FLOAT scale, INT wSource, INT hSource, INT *wScale, INT *hScale)
{

	*wScale = (INT) ((FLOAT) scale*wSource);
	*hScale = (INT) ((FLOAT) scale*hSource);

	if((*wScale)%4){
		*wScale = *wScale + (4-(*wScale)%4);
	}

	return TRUE;
}


DWORD GetNormalizedCoordinate(INT width, INT height, INT x, INT y, FLOAT *xNorm, FLOAT *yNorm)
{
	INT max = Max(width, height);

	*xNorm = (FLOAT)((2*x) - (width)) / (FLOAT)(max);
	*yNorm = (FLOAT)((2*y) - (height)) / (FLOAT)(max);


	return TRUE;
}

DWORD GetPixelCoordinate(INT width, INT height, FLOAT xNorm, FLOAT yNorm, INT *x, INT *y)
{
	INT max = Max(width, height);

	FLOAT xFloat = NULL;
	FLOAT yFloat = NULL;

	xFloat = (( (FLOAT) (xNorm*max) + width ) / (FLOAT) (2));
	yFloat = (( (FLOAT) (yNorm*max) + height) / (FLOAT) (2));

	if(x && y){
		*x = (INT) xFloat;
		*y = (INT) yFloat;
	}

	return TRUE;
}

DWORD ScaleImage(FLOAT scale, INT wSource, INT hSource, UCHAR *imgSource, UCHAR *imgScale)
{

	DWORD i = 0;
	INT x = NULL;
	INT y = NULL;

	DWORD iSrc = NULL;
	INT xSrc = NULL;
	INT ySrc = NULL;

	DWORD max = NULL;
	INT wScale = NULL;
	INT hScale = NULL;

	FLOAT xNorm = NULL;
	FLOAT yNorm = NULL;

	if(!imgSource || !imgScale)
		return FALSE;

	GetScaleDimensions(scale, wSource, hSource, &wScale, &hScale);
	max = wScale*hScale;

	for(y = 0; y < hScale; y++){
		for(x = 0; x < wScale; x++, i++){

			GetNormalizedCoordinate(wScale, hScale, x, y, &xNorm, &yNorm);
					
			GetPixelCoordinate(wSource, hSource, xNorm, yNorm, &xSrc, &ySrc);
			i = (y*wScale) + x;
			iSrc = (ySrc*wSource)+xSrc;
			if(Range(wSource, hSource, iSrc) && Range(wScale, hScale, i)){
			
				imgScale[i] = imgSource[iSrc];
				imgScale[i] = imgScale[i];
			}

		}

	}

	return i;

}

DWORD ScaleImage(FLOAT scale, INT wSource, INT hSource, PIXEL *imgSource, PIXEL *imgScale)
{

	DWORD i = 0;
	DWORD iSrc = NULL;

	INT x = NULL;
	INT y = NULL;
	
	INT xSrc = NULL;
	INT ySrc = NULL;

	FLOAT xScale = NULL;
	FLOAT yScale = NULL;

	INT wScale = NULL;
	INT hScale = NULL;

	if(!imgSource || !imgScale)
		return FALSE;

	GetScaleDimensions(scale, wSource, hSource, &wScale, &hScale);

	xScale = (FLOAT)wSource / (FLOAT) wScale;
	yScale = (FLOAT)hSource / (FLOAT) hScale;

	for(y = 0; y < hScale; y++){
		for(x = 0; x < wScale; x++, i++){

			xSrc = x*xScale;
			ySrc = y*yScale;
			iSrc = (ySrc*wSource)+xSrc;
			//if(Range(wSource, hSource, iSrc) && Range(wScale, hScale, i)){
				memcpy(&imgScale[i], &imgSource[iSrc], sizeof(PIXEL));
			//}


		}

	}


	return i;

}

DWORD RotateImage(FLOAT deg, INT w, INT h, PIXEL *imgIn, PIXEL *imgOut)
{
	if(!imgIn || !imgOut)
		return FALSE;

	DWORD i = 0;
	DWORD iSrc = 0;
	LONG xSrc = 0;
	LONG ySrc = 0;
	LONG xCenter = w/2;
	LONG yCenter = h/2;
	LONG x = 0;
	LONG y = 0;
	FLOAT rad = ((deg)*3.1415926)/180.0;
	FLOAT det = (1.0 / (cos(rad)*cos(rad) + sin(rad)*sin(rad)));

	PIXEL *imgTemp = (PIXEL *)malloc(w*h*sizeof(PIXEL));

	if(!imgTemp)
		return FALSE;

	memset(imgTemp, NULL, w*h*sizeof(PIXEL));


	for(y = 0; y < h; y++){
		for(x = 0; x < w; x++, i++){

			xSrc = (x-xCenter)*cos(rad) + (y-yCenter)*sin(rad);
			ySrc = (y-yCenter)*cos(rad) - (x-xCenter)*sin(rad);
			xSrc = xSrc+xCenter;
			ySrc = ySrc+yCenter;

			iSrc = w*ySrc + xSrc;
			if(Range(w, h, iSrc)){
				imgTemp[i].r = imgIn[iSrc].r;
				imgTemp[i].g = imgIn[iSrc].g;
				imgTemp[i].b = imgIn[iSrc].b;
			}
		}
	}

	memcpy(imgOut, imgTemp, w*h*sizeof(PIXEL));

	if(imgTemp);free(imgTemp);

	return i;
}
//DWORD ScaleImage(FLOAT scale, INT wSource, INT hSource, PIXEL *imgSource, PIXEL *imgScale)
//{
//
//	DWORD i = 0;
//	INT x = NULL;
//	INT y = NULL;
//
//	DWORD iSrc = NULL;
//	INT xSrc = NULL;
//	INT ySrc = NULL;
//
//	DWORD max = NULL;
//	INT wScale = NULL;
//	INT hScale = NULL;
//
//	FLOAT xNorm = NULL;
//	FLOAT yNorm = NULL;
//
//	if(!imgSource || !imgScale)
//		return FALSE;
//
//	GetScaleDimensions(scale, wSource, hSource, &wScale, &hScale);
//	max = wScale*hScale;
//
//	for(y = 0; y < hScale; y++){
//		for(x = 0; x < wScale; x++, i++){
//
//			GetNormalizedCoordinate(wScale, hScale, x, y, &xNorm, &yNorm);
//					
//			GetPixelCoordinate(wSource, hSource, xNorm, yNorm, &xSrc, &ySrc);
//			i = (y*wScale) + x;
//			iSrc = (ySrc*wSource)+xSrc;
//			if(Range(wSource, hSource, iSrc) && Range(wScale, hScale, i)){
//			
//				memcpy(&imgScale[i], &imgSource[iSrc], sizeof(PIXEL));
//				//imgScale[i].r = imgSource[iSrc].r;
//				//imgScale[i].g = imgSource[iSrc].g;
//				//imgScale[i].b = imgSource[iSrc].b;
//			}
//
//
//		}
//
//	}
//
//
//	return i;
//
//}
