#include "stdafx.h"

#include "MedianFilter.h"

FLOAT Median(DWORD n, FLOAT *set)
{
	if(!set)
		return FALSE;

	FLOAT temp;
	INT i = 0;
	INT j = 0;

	for(i=0; i<n-1; i++){
		for(j=i+1; j<n; j++){
			temp = set[i];
			set[i] = set[j];
			set[j] = temp;
		}
	}

	if(n%2==0){
		//if there is an even number of elements, return mean of the two elements in the middle
		return((FLOAT)(set[n/2] + set[n/2-1])/(FLOAT)2.0);
	}else{
		return set[n/2];
	}

}

DWORD MedianFilter(DWORD w, DWORD h, FLOAT *in, FLOAT *out)
{
	if(!in || !out)
		return FALSE;

	DWORD max = w*h;
	DWORD i = 0;
	DWORD x = 0;
	DWORD y = 0;
	DWORD wMax = w-1;
	DWORD hMax = h-1;

	FLOAT set[9] = {0.0};

	memset(out, 0.0, w*h*sizeof(FLOAT));

	for(y = 1; y < hMax; y++){
		for(x = 1; x < wMax; x++){
		
		i = (y*w)+x;

		set[0] = in[i-w-1];
		set[1] = in[i-w];
		set[2] = in[i-w+1];
		set[3] = in[i-1];
		set[4] = in[i];
		set[5] = in[i+1];
		set[6] = in[i+w-1];
		set[7] = in[i+w];
		set[8] = in[i+w+1];

		out[i] = Median(8, set);

		}
	}

	return TRUE;
}
		