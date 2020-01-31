#include "stdafx.h"

#include "Statistics.h"

FLOAT StdDev(DWORD n, FLOAT *in, FLOAT *out)
{

	DWORD i = NULL;
	FLOAT mean = Mean(n, in, NULL);
	DOUBLE sum = NULL;
	FLOAT stdDev = NULL;

	for(i = 0; i < n; i++){
		sum = sum + ((in[i]-mean)*(in[i]-mean));
	}

	stdDev = (DOUBLE)sum / (FLOAT)(n);
	stdDev = sqrt(stdDev);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = stdDev;
		}
	}
	
	return stdDev;
}

FLOAT StdDev(DWORD n, INT *in, INT *out)
{

	DWORD i = NULL;
	FLOAT mean = Mean(n, in, NULL);
	DOUBLE sum = NULL;
	FLOAT stdDev = NULL;

	for(i = 0; i < n; i++){
		sum = sum + ((in[i]-mean)*(in[i]-mean));
	}

	stdDev = (DOUBLE)sum / (FLOAT)(n);
	stdDev = sqrt(stdDev);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = stdDev;
		}
	}
	
	return stdDev;
}

FLOAT Variance(DWORD n, FLOAT *in, FLOAT *out)
{

	DWORD i = NULL;
	FLOAT mean = Mean(n, in, NULL);
	DOUBLE sum = NULL;
	FLOAT variance = NULL;

	for(i = 0; i < n; i++){
		sum = sum +((in[i]-mean)*(in[i]-mean));
	}

	variance = (DOUBLE)sum / (FLOAT)(n);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = variance;
		}
	}
	
	return variance;
}

FLOAT Variance(DWORD n, INT *in, INT *out)
{

	DWORD i = NULL;
	FLOAT mean = Mean(n, in, NULL);
	DOUBLE sum = NULL;
	FLOAT variance = NULL;

	for(i = 0; i < n; i++){
		sum = sum +((in[i]-mean)*(in[i]-mean));
	}

	variance = (DOUBLE)sum / (FLOAT)(n);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = (INT) variance;
		}
	}
	
	return variance;
}


FLOAT NormalizedVariance(DWORD n, FLOAT *in, FLOAT *out)
{

	DWORD i = NULL;
	FLOAT mean = Mean(n, in, NULL);
	DOUBLE sum = NULL;
	FLOAT variance = NULL;

	for(i = 0; i < n; i++){
		sum = sum +((in[i]-mean)*(in[i]-mean));
	}

	variance = (DOUBLE)sum / (FLOAT)(n*mean);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = variance;
		}
	}
	
	return variance;
}

FLOAT NormalizedVariance(DWORD n, INT *in, INT *out)
{

	DWORD i = NULL;
	FLOAT mean = Mean(n, in, NULL);
	DOUBLE sum = NULL;
	FLOAT variance = NULL;

	for(i = 0; i < n; i++){
		sum = sum +((in[i]-mean)*(in[i]-mean));
	}

	variance = (DOUBLE)sum / (FLOAT)(n*mean);

	if(out){
		for(i = 0; i < n; i++){
			out[i] = variance;
		}
	}
	
	return variance;
}