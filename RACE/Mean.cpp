#include "stdafx.h"

#include "Statistics.h"


FLOAT Mean(DWORD n, FLOAT *in, FLOAT *out)
{

	DWORD i = NULL;
	DOUBLE sum = NULL;
	FLOAT mean = NULL;

	if(!in)
		return (FLOAT)FALSE/*(-1)*/;

	for(i = 0; i < n; i++){
		sum += in[i];
	}

	mean = (FLOAT)sum / (FLOAT)n;

	if(out){
		for(i = 0; i < n; i++){
			out[i] = mean;
		}
	}

	return mean;
}

FLOAT Mean(DWORD n, INT *in, INT *out)
{

	DWORD i = NULL;
	DOUBLE sum = NULL;
	FLOAT mean = NULL;

	if(!in)
		return (FLOAT)FALSE;

	for(i = 0; i < n; i++){
		sum += in[i];
	}

	mean = ((FLOAT)sum / (FLOAT)n);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = mean;
		}
	}

	return mean;
}


	