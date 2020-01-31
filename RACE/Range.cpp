#include "stdafx.h"

#include "Range.h"

INT Range(UINT width, UINT height, INT x, INT y)
{
	if((x < 0) || (x >= width))
		return(0);
	if((y < 0) || (y >= height))
		return(0);
	return(1);
}

INT Range(UINT w, UINT h, UINT i)
{
	UINT max = w*h;

	if(i > 0 && i < max)
		return 1;
	else
		return 0;
}

DWORD Range(DWORD w, DWORD h, DWORD i)
{
	DWORD max = w*h;
	max = max-1;

	if(i >=0 && i < max)
		return TRUE;
	else
		return FALSE;
}

DWORD RangeInt(INT i, INT lower, INT upper)
{
	if(i < upper && i > lower){
		return TRUE;
	}

	return FALSE;
}