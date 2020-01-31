#include "stdafx.h"

#include "Boolean.h"

DWORD BooleanAND(UINT w, UINT h, FLOAT *inOne, FLOAT *inTwo, FLOAT *out)
{
	DWORD i = NULL;
	DWORD max = w*h;

	memset(out, NULL, max*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		if(inOne[i] && inTwo[i])
			out[i] = sqrt(inOne[i]*inOne[i] + inTwo[i]*inTwo[i]);
	}

	return i;
}

DWORD BooleanAND(UINT w, UINT h, INT *inOne, INT *inTwo, INT *out)
{
	DWORD i = NULL;
	DWORD max = w*h;

	memset(out, NULL, max*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		if(inOne[i] && inTwo[i])
			out[i] = sqrt((FLOAT)(inOne[i]*inOne[i] + inTwo[i]*inTwo[i]));
	}

	return i;
}

DWORD Boolean(UINT w, UINT h, FLOAT set, FLOAT *in,  FLOAT *out)
{
	DWORD i = NULL;
	DWORD max = w*h;

	memset(out, NULL, max*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		if(in[i])
			out[i] = set;
	}

	return i;
}

DWORD Boolean(UINT w, UINT h, INT set, INT *in, INT *out)
{
	DWORD i = NULL;
	DWORD max = w*h;

	memset(out, NULL, max*sizeof(FLOAT));

	for(i = 0; i < max; i++){

		if(in[i])
			out[i] = set;
	}

	return i;
}