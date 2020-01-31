#include "stdafx.h"

#include "Histogram.h"
#include "Magnitude.h"
#include "Sum.h"


//Assumes 246 bins for Type(UCHAR
//The count for each bin is stored in 'dwHistogram'
DWORD GetHistogram(UINT w, UINT h, UCHAR *img, DWORD *dwHistogram)
{

	if(!img || !dwHistogram)
		return FALSE;
	
	memset(dwHistogram, 0x00, MAXBINS_UCHAR*sizeof(dwHistogram));

	UINT i = NULL;
	UINT max = w*h;
	
	for(i = 0; i < max; i++){
		dwHistogram[img[i]]++;
	}

	return i;

}

DWORD PlotHistogram(UINT w, UINT h, DWORD *dwHistogram, PIXEL *bmp, COLORREF colorref)
{

	if(!dwHistogram || !bmp)
		return FALSE;

	//memset(bmp, 0x00, w*h*sizeof(PIXEL));

	INT i = NULL;
	UINT x = NULL;
	UINT y = NULL;
	UINT max = w*h;

	for(x = 0; x < MAXBINS_UCHAR; x++){
		y = h-dwHistogram[x];
		i = y*w + x;
		//Start at the top of the column
		//Step down while marking each pixel
		//This draws a column
		for(; y > 0; y--, i+=w){
			bmp[i].r = 255;
		}
	}

	return TRUE;
}

//Computes the histogram using the magnitude of 'in'
//'nBins' represents the number of bins present in 'hist'
//The histogram vlaue for a given bin represents the frequency of occurence of the input
//Values falling on the border of two bins should be placed in the lower bin
//'k' = 1+log2N - A easy computatin of the number of bins based on the number of observations
//DWORD GetHistogram(DWORD n, DWORD nBins, FLOAT *in, FLOAT *hist)
//{
//	if(!in||!hist)return FALSE;
//	memset(hist, NULL, nBins*sizeof(DWORD));
//	FLOAT *mag = (FLOAT *)malloc(n*sizeof(FLOAT));	
//	if(!mag)return FALSE;
//
//	Magnitude(n, in, mag);
//
//	FLOAT max = 0;
//	FLOAT min = 0;
//	GetMaxMin(n, &max, &min, mag);
//
//	DWORD i = 0;
//	FLOAT del = (max-min)/nBins;
//	
//	for(i = 0; i < max; i++){
//		//hist[(LONG
//	}
//
//	return TRUE;
//}

DWORD GetHistogram(DWORD w, DWORD h, FLOAT *in, FLOAT *hist)
{
	if(!in||!hist)return FALSE;
	memset(hist, (FLOAT) 0.0, w*h*sizeof(DWORD));
	FLOAT *mag = (FLOAT *)malloc(w*h*sizeof(FLOAT));	
	if(!mag)return FALSE;

	DWORD max = w*h;

	Magnitude(max, in, mag);
	Stretch(w, h, 0, 255, mag, mag);

	DWORD i = 0;
	
	for(i = 0; i < max; i++){
		hist[(INT)mag[i]]++;
	}

	return TRUE;
}

//The middle number when the data is listed in order
DWORD GetHistogramMedian(DWORD n, FLOAT *in, FLOAT *out)
{
	if(!in || !out)return FALSE;

	DWORD i = 0;
	FLOAT sum = 0;
	FLOAT sumBy2 = 0;
	FLOAT median = 0;

	sum = Sum(n, in);
	sumBy2 = sum/2.0;

	sum = 0;
	for(i = 0; i < n; i++){
		sum += (i*in[i]);
		if(sum >= sumBy2){
			break;
		}
	}

	median = i;

	for(i = 0; i < n; i++){
		out[i] = median;
	}

	//memset(out, (FLOAT)median, n*sizeof(FLOAT));

	return TRUE;
}

////The middle number when the data is listed in order
//DWORD GetHistogramMedian(DWORD n, FLOAT *in, FLOAT *out)
//{
//	if(!in || !out)return FALSE;
//
//	DWORD i = 0;
//	DWORD lower = n;
//	DWORD upper = 0;
//	FLOAT median = 0;
//	//Find Lower Bin
//	for(i = 0; i < n; i++){
//		if(in[i]){
//			lower = i;
//			break;
//		}
//	}
//
//	//Find Upper Bin
//	for(i = n-1; i > 0; i--){
//		if(in[i]){
//			upper = i;
//			break;
//		}
//	}
//
//	median = (upper-lower/2);
//
//	for(i = 0; i < n; i++){
//		out[i] = median;
//	}
//
//	//memset(out, (FLOAT)median, n*sizeof(FLOAT));
//
//	return TRUE;
//}

