#include "stdafx.h"
#include "WindowData.h"

DWORD WindowData(UINT n, UINT lower, UINT upper, UCHAR *data)
{
	
	if(!data)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++)
	{
		if(data[i] < lower || data[i] > upper)
			data[i] = 0;
	}

	return i;
}

DWORD WindowData(UINT n, UINT lower, UINT upper, INT *data)
{
	
	if(!data)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++)
	{
		if(data[i] < lower || data[i] > upper)
			data[i] = 0;
	}

	return i;
}

DWORD ClampData(UINT n, INT lower, INT upper, FLOAT *data, UCHAR *out)
{

	if(!data || !out)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++)
	{
		if(data[i] > 255)
			out[i] = 255;
		else if(data[i] < 0)
			out[i] = 0;
		else
			out[i] = (UCHAR)data[i];
	}

	return i;

}

DWORD ClampData(UINT n, INT lower, INT upper, FLOAT *data, FLOAT *out)
{

	if(!data || !out)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++)
	{
		if(data[i] > upper)
			out[i] = upper;
		else if(data[i] < lower)
			out[i] = NULL;
		else
			out[i] = data[i];
	}

	return i;

}

DWORD ClampData(UINT n, INT lower, INT upper, INT *data, UCHAR *out)
{

	if(!data || !out)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++)
	{
		if(data[i] > upper)
			out[i] = upper;
		else if(data[i] < lower)
			out[i] = NULL;
		else
			out[i] = (UCHAR) data[i];
	}

	return i;

}

DWORD ClampData(UINT n, INT lower, INT upper, INT *data, INT *out)
{

	if(!data || !out)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++)
	{
		if(data[i] >= upper)
			out[i] = upper;
		else if(data[i] <= lower)
			out[i] = NULL;
		else
			out[i] =  data[i];
	}

	return i;

}

DWORD Clamp(UINT n, INT lower, INT upper, INT *data)
{

	if(!data){
		return FALSE;
	}

	UINT i = NULL;

	for(i = 0; i < n; i++){

		if(data[i] > upper)
			data[i] = upper;
		
		if(data[i] < lower)
			data[i] = lower;

	}

	return i;
}
