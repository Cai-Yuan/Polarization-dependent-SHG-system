#include "stdafx.h"

#include "Power.h"

DWORD Square(DWORD n, FLOAT *img, FLOAT *square)
{
	DWORD i;
	UINT max = n;
	
	if(!img || !square)
		return FALSE;

	memset(square, 0x00, n*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		square[i] = (img[i]*img[i]);
	}

	return n;
}

DWORD Square(DWORD n, INT *img, INT *square)
{
	DWORD i;
	UINT max = n;
	
	if(!img || !square)
		return FALSE;

	//memset(square, 0x00, n*sizeof(FLO));

	for(i = 0; i < max; i++){

		square[i] = (img[i]*img[i]);
	}

	return n;
}

DWORD Square(DWORD n, INT *img, FLOAT *square)
{
	DWORD i;
	UINT max = n;
	
	if(!img || !square)
		return FALSE;

	memset(square, 0x00, n*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		square[i] = (img[i]*img[i]);
	}

	return n;
}

DWORD Square(DWORD n, FLOAT *img)
{
	DWORD i;
	UINT max = n;

	if(!img)
		return FALSE;

	FLOAT *square = (FLOAT *)calloc(n, sizeof(FLOAT));

	for(i = 0; i < max; i++){

		square[i] = (img[i]*img[i]);
	}

	if(square){
		memcpy(img, square, n*sizeof(FLOAT));
		free(square);
	}

	return n;
}

DWORD Square(DWORD n, INT *img)
{
	DWORD i;
	UINT max = n;

	if(!img)
		return FALSE;

	for(i = 0; i < max; i++){

		img[i] = (img[i]*img[i]);
	}

	return n;
}

DWORD SquareRoot(DWORD n, FLOAT *img, FLOAT *out)
{
	DWORD i = 0;

	if(!img)
		return FALSE;

	for(i = 0; i < n; i++){
		out[i] = sqrt(img[i]);
	}

	return i;
}