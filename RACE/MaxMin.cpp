#include "stdafx.h"

#include "MaxMin.h"

INT Max(INT dw1, INT dw2)
{
	if(dw1 >= dw2)
		return dw1;
	else
		return dw2;
}

DWORD Max(DWORD dw1, DWORD dw2)
{
	if(dw1 >= dw2)
		return dw1;
	else
		return dw2;
}

DWORD Min(DWORD dw1, DWORD dw2)
{
	if(dw1 <= dw2)
		return dw1;
	else
		return dw2;
}

FLOAT MaxFloat(FLOAT f1, FLOAT f2)
{
	if(f1 >= f2)
		return f1;
	else
		return f2;
}

FLOAT MinFloat(FLOAT f1, FLOAT f2)
{
	if(f1 <= f2)
		return f1;
	else
		return f2;
}

DWORD GetMaxMin(UINT n, DWORD *max, DWORD *min, DWORD *dw)
{
	if(!dw)
		return FALSE;
	
	UINT i = NULL;

	*max = dw[0];
	*min = dw[0];

	for(i = 0; i < n; i++)
	{
		if(dw[i] > *max)
			*max = dw[i];
		if(dw[i] < *min)
			*min = dw[i];
	}

	return i;
}

DWORD GetMaxMin(UINT n, FLOAT *max, FLOAT *min, FLOAT *dw)
{
	if(!dw)
		return FALSE;
	
	UINT i = NULL;

	*max = dw[0];
	*min = dw[0];

	for(i = 0; i < n; i++)
	{
		if(dw[i] > *max)
			*max = dw[i];
		if(dw[i] < *min)
			*min = dw[i];
	}

	return i;
}


DWORD GetMaxMin(UINT n, INT *max, INT *min, INT *data)
{
	if(!data)
		return FALSE;

	UINT i = NULL;

	*max = data[0];
	*min = data[0];

	for(i = 0; i < n; i++)
	{
		if(data[i] > *max)
			*max = data[i];
		if(data[i] < *min)
			*min = data[i];
	}
	
	return i;
}

DWORD GetMaxMin(UINT n, DWORD *max, DWORD *min, UCHAR *data)
{
	if(!data)
		return FALSE;
	
	UINT i = NULL;

	*max = data[0];
	*min = data[0];

	for(i = 0; i < n; i++)
	{
		if(data[i] > *max)
			*max = data[i];
		if(data[i] < *min)
			*min = data[i];
	}

	return i;
}
