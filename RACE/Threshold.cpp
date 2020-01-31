#include "stdafx.h"

#include "Threshold.h"
#include "FloodFill.h"

DWORD Threshold(UINT w, UINT h, UINT threshold, UCHAR *img)
{

	INT i = NULL;
	INT max = w*h;

	if(!img) return FALSE;

	for(i = 0; i < max; i++){

		if(img[i] >= threshold)
			img[i] = 255;
		else
			img[i] = 0;
	}

	
	return TRUE;
}

DWORD Threshold(UINT w, UINT h, UINT threshold, INT *img)
{

	INT i = NULL;
	INT max = w*h;

	if(!img) return FALSE;

	for(i = 0; i < max; i++){

		if(img[i] >= threshold)
			img[i] = 255;
		else
			img[i] = 0;
	}

	
	return TRUE;
}

DWORD Threshold(UINT w, UINT h, FLOAT threshold, FLOAT *img)
{

	INT i = NULL;
	INT max = w*h;

	if(!img) return FALSE;

	for(i = 0; i < max; i++){

		if(img[i] >= threshold)
			img[i] = 255;
		else
			img[i] = 0;
	}

	return TRUE;
}

DWORD Threshold(UINT w, UINT h, FLOAT threshold, INT *imgIn, INT *imgOut)
{

	INT i = NULL;
	INT max = w*h;

	if(!imgIn || !imgOut) return FALSE;

	for(i = 0; i < max; i++){

		if(imgIn[i] >= threshold)
			imgOut[i] = 255;
		else
			imgOut[i] = 0;
	}

	
	return TRUE;
}

DWORD Threshold(UINT w, UINT h, FLOAT threshold, FLOAT *imgIn, FLOAT *imgOut)
{

	INT i = NULL;
	INT max = w*h;

	if(!imgIn ||!imgOut) return FALSE;

	for(i = 0; i < max; i++){

		if(imgIn[i] >= threshold)
			imgOut[i] = 255;
		else
			imgOut[i] = 0;
	}

	
	return TRUE;
}
//Applies a pixel-to-pixel threshold to 'imgIn' using the values from 'imgRef'
//Keeps the original at in 'imgIn(x,y)' if that pixel is larger than 'imgRef(x,y)'
DWORD Threshold(UINT w, UINT h, FLOAT *imgIn, FLOAT *imgRef, FLOAT *imgOut)
{

	INT i = NULL;
	INT max = w*h;

	if(!imgIn || !imgRef || !imgOut) return FALSE;

	for(i = 0; i < max; i++){
		if(imgIn[i] > imgRef[i])
			imgOut[i] = imgIn[i];
	}

	
	return TRUE;
}

DWORD Threshold(UINT w, UINT h, UINT threshold, UCHAR *img, UCHAR *imgThresh)
{

	INT i = NULL;
	INT max = w*h;

	if(!img) return FALSE;
	if(!imgThresh) return FALSE;

	memset(imgThresh, NULL, max*sizeof(UCHAR));

	for(i = 0; i < max; i++){

		if(img[i] >= threshold)
			imgThresh[i] = 255;
	}

	
	return TRUE;
}


//Does Not Work
DWORD DoubleThreshold(UINT w, UINT h, INT lower, INT upper, INT *in, INT *out)
{
	if(!in || !out){
		return FALSE;
	}

	INT *low = (INT *)malloc(w*h*sizeof(INT));
	INT *high = (INT *)malloc(w*h*sizeof(INT));
	//INT *temp = (INT *)malloc(w*h*sizeof(INT));

	if(!low || !high){
		return FALSE;
	}

	memcpy(low, in, w*h*sizeof(INT));
	memcpy(high, in, w*h*sizeof(INT));
	//memset(out, NULL, w*h*sizeof(INT));
	

	Threshold(w, h, lower, low);
	Threshold(w, h, upper, high);
	//memcpy(temp, low, w*h*sizeof(INT));

	UINT x = NULL;
	UINT y = NULL;	
	UINT i = NULL;
	UINT max = w*h;

	for(i = 0; i < max; i++){
		if(high[i] && low[i] && !out[i]){
			FloodFill(w, h, i, 250, low, out);
		}

	}

	if(low)free(low);
	if(high)free(high);


	return TRUE;
}