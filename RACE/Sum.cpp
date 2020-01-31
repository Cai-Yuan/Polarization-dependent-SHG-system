#include "stdafx.h"

#include "Sum.h"

DOUBLE Sum(DWORD n, DOUBLE *in)
{

	if(!in)
		return FALSE;

	INT i = NULL;
	DOUBLE sum = NULL;

	for(i = 0; i < n; i++){

		sum += in[i];

	}

	return sum;
}

DOUBLE Sum(DWORD n, FLOAT *in)
{

	if(!in)
		return FALSE;

	INT i = NULL;
	DOUBLE sum = NULL;

	for(i = 0; i < n; i++){

		sum += in[i];

	}

	return sum;
}

DWORD Sum(DWORD n, INT *in)
{

	if(!in)
		return FALSE;

	INT i = NULL;
	DOUBLE sum = NULL;

	for(i = 0; i < n; i++){

		sum += in[i];

	}

	return sum;
}