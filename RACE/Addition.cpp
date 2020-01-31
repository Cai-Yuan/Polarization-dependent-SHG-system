#include "stdafx.h"

#include "Addition.h"

DWORD Addition(DWORD n, FLOAT *add1, FLOAT *add2, FLOAT *out)
{

	if(!add1 || !add2 || !out)return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = add1[i] + add2[i];

	}

	return i;

}

DWORD Addition(DWORD n, INT *add1, INT *add2, INT *out)
{

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = add1[i] + add2[i];

	}

	return i;

}

//Adds 'img' values to 'imgOut' at each position
DWORD Add(DWORD n, UCHAR *img, INT *imgOut)
{

	DWORD i = 0;

	if(!img || !imgOut)
		return FALSE;

	for(i = 0; i < n; i++)
	{
		imgOut[i] = imgOut[i]+img[i];
	}

	return TRUE;
}

DWORD Add(DWORD n, INT *img, INT *imgOut)
{

	DWORD i = 0;

	if(!img || !imgOut)
		return FALSE;

	for(i = 0; i < n; i++)
	{
		imgOut[i] = imgOut[i]+img[i];
	}

	return TRUE;
}

