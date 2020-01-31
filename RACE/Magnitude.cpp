#include "stdafx.h"

#include "Magnitude.h"

FLOAT Magnitude(FLOAT value)
{

	if(value < 0)
		value = value * (FLOAT) (-1);

	return value;
}

DOUBLE Magnitude(DOUBLE value)
{

	if(value < 0)
		value = value * (DOUBLE) (-1);

	return value;
}

DWORD Magnitude(DWORD value)
{

	if(value < 0)
		value = value * (DWORD) (-1);

	return value;
}

INT Magnitude(INT value)
{

	if(value < 0)
		value = value * (DWORD) (-1);

	return value;
}

UINT Magnitude(UINT value)
{

	if(value < 0)
		value = value * (DWORD) (-1);

	return value;
}


//2.24.16
DWORD Magnitude(DWORD n, FLOAT *in, FLOAT *out)
{
	if(!in || !out)return FALSE;

	DWORD i = 0;

	for(i = 0; i < n; i++){
		out[i] = Magnitude(in[i]);
	}

	return TRUE;
}
