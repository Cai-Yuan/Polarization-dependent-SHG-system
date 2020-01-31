#include "stdafx.h"
#include "Contrast.h"


//'c' - Contrast Value
DWORD Contrast(DWORD w, DWORD h, DWORD c, FLOAT *in, FLOAT *out)
{
	if(!in || !out)
		return FALSE;

	DWORD i = 0;
	DWORD max = w*h;
	FLOAT f = (259.0*(c+255.0))/(255.0*(259.0-c));
	for(i = 0; i < max; i++){
		out[i] = f*(in[i]-128)+128;
	}


	return TRUE;
}