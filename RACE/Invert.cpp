#include "stdafx.h"
#include "Invert.h"
#include "MaxMin.h"

DWORD InvertData(DWORD nData, FLOAT *in, FLOAT *out)
{
	if(!in || !out)
		return FALSE;

	DWORD i = NULL;

	for(i = 0; i < nData; i++)
	{
		out[i] = -1.0*in[i];
	}


	return TRUE;

}


DWORD InvertData(DWORD nData, INT *in, INT *out)
{
	if(!in || !out)
		return FALSE;

	INT max = NULL;
	INT min = NULL;

	GetMaxMin(nData, &max, &min, in);

	INT i = NULL;

	for(i = 0; i < nData; i++){
		out[i] = in[i]*(-1) + max;
	}


	return TRUE;
}