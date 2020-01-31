#include "stdafx.h"

#include "ImageSubtract.h"

DWORD ImageSubtract(UINT w, UINT h, UCHAR *frmOne, UCHAR *frmTwo, UCHAR *imgSubtract)
{
	DWORD dw = NULL;
	UINT x, y, i, p;
	UINT max = w*h;
	FLOAT temp;

	memset(imgSubtract, 0x00, w*h*sizeof(UCHAR));

	for(i = 0; i < max; i++){

		//temp = frmOne[i]-frmTwo[i];
		temp = (FLOAT)(frmOne[i]-frmTwo[i])/(FLOAT)2;

		if(temp >= 0)
			imgSubtract[i] = (INT) temp;
		else
			imgSubtract[i] = (INT) 0;
	}

	return dw;
}

DWORD ImageSubtract(UINT w, UINT h, UCHAR *imgOne, UCHAR *imgTwo, INT *imgSubtract)
{
	DWORD dw = NULL;
	UINT x, y, i, p;
	UINT max = w*h;
	FLOAT temp;

	if(!imgOne)
		return FALSE;

	if(!imgTwo)
		return FALSE;

	if(!imgSubtract)
		return FALSE;

	memset(imgSubtract, 0x00, w*h*sizeof(UCHAR));

	for(i = 0; i < max; i++){

		imgSubtract[i] = (INT) imgOne[i]-imgTwo[i];
	}

	return dw;
}

DWORD ImageSubtract(UINT w, UINT h, FLOAT *frmOne, FLOAT *frmTwo, FLOAT *imgSubtract)
{
	DWORD dw = NULL;
	UINT x, y, i, p;
	UINT max = w*h;
	FLOAT temp;

	memset(imgSubtract, 0x00, w*h*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		imgSubtract[i] = (FLOAT)(frmOne[i]-frmTwo[i]);
	}

	return dw;
}

DWORD ConstSubtract(UINT w, UINT h, FLOAT constant, FLOAT *img, FLOAT *out)
{
	DWORD i = NULL;
	DWORD dw = NULL;
	DWORD max = w*h;

	if(!img || !out)
		return FALSE;

	memset(out, NULL, w*h*sizeof(FLOAT));

	for(i = 0; i < max; i++){
		out[i] = img[i]-constant;
	}


	return i;
}