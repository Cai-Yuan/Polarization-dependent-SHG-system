#pragma once

#include "stdafx.h"


//Center (0, 0) at the center of the kernel
DWORD GetLoGKernel(INT n, FLOAT sigma, FLOAT *kernel)
{
	if(!kernel)
		return FALSE;

	INT i = 0;
	INT x = NULL;
	INT y = NULL;
	INT k = floor((FLOAT)n/(FLOAT)2);
	FLOAT gLevel = NULL;
	DOUBLE temp = NULL;

	FLOAT sigsig = 2*sigma*sigma;

	for(y = 0; y < n; y++){
		for(x = 0; x < n; x++){

			i = (y*n) + x;
			temp = (x-k)*(x-k) + (y-k)*(y-k);
			temp = -1.0*((FLOAT)temp / (FLOAT)sigsig);
			gLevel = (1.0 / /*sqrt(*/(DOUBLE)(PI*sigsig)/*)*/ * exp(temp));
			kernel[i] = gLevel;
			gLevel = gLevel;

		}
	}



	return TRUE;
}