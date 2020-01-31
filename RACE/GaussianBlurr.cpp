#include "stdafx.h"

#include "math.h"
#include "Sum.h"
#include "GaussianBlurr.h"

//Center (0, 0) at the center of the kernel
//G(x, y) = (1 / sqrt((2*PI*(sigma*sigma)))*e^-( x*x + y*y ) / (2*sigma*sigma))
DWORD GetGaussianKernel(INT n, FLOAT sigma, FLOAT *kernel)
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

DWORD GaussianTest(HWND hwnd, INT n, FLOAT sigma)
{

	FLOAT *kernel = (FLOAT *) calloc(n*n, sizeof(FLOAT));
	DOUBLE sum = NULL;

	GetGaussianKernel(n, sigma, kernel);
	sum = Sum(n*n, kernel);

	if(kernel)free(kernel);


	return TRUE;
}

DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, UCHAR *img)
{
	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT k = floor((FLOAT)n/(FLOAT)2);

	INT xx = NULL;
	INT yy = NULL;

	FLOAT sum = NULL;

	FLOAT *kernel = (FLOAT *)calloc(n*n, sizeof(FLOAT));
	UCHAR *blur = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	GetGaussianKernel(n, sigma, kernel);

	//Step through image
	for(y = n; y < h-n; y++){
		for(x = n; x < w-n; x++){

			sum = NULL;
			//Step through kernel at each pixel
			for(yy = 0; yy < n; yy++){
				for(xx = 0; xx < n; xx++){
				
					i = ( (y + (yy-k))*w + (x+xx-k));
					sum += img[i] * kernel[(yy*n) + (xx)];
				}
			}
			i = (y*w) + x;
			if(sum > 255)
				blur[i] = (UCHAR) sum;
			else if(sum < 0)
				blur[i] = (UCHAR) 0;
			else
				blur[i] = (UCHAR) sum;
		}
	}

	memcpy(img, blur, w*h*sizeof(UCHAR));

	if(kernel)free(kernel);
	if(blur)free(blur);

	return i;
}

DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, FLOAT *img, FLOAT *blur)
{
	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT k = floor((FLOAT)n/(FLOAT)2);

	INT xx = NULL;
	INT yy = NULL;

	if(!img || !blur)
		return FALSE;

	FLOAT sum = NULL;

	FLOAT *kernel = (FLOAT *)calloc(n*n, sizeof(FLOAT));

	memcpy(blur, img, w*h*sizeof(FLOAT));
	GetGaussianKernel(n, sigma, kernel);

	//Step through image
	for(y = n; y < h-n; y++){
		for(x = n; x < w-n; x++){

			sum = NULL;
			//Step through kernel at each pixel
			for(yy = 0; yy < n; yy++){
				for(xx = 0; xx < n; xx++){
				
					i = ( (y + (yy-k))*w + (x+xx-k));
					sum += img[i] * kernel[(yy*n) + (xx)];
				}
			}
			i = (y*w) + x;
			if(sum > 255)
				blur[i] = sum;
			else if(sum < 0)
				blur[i] =  0;
			else
				blur[i] = sum;
		}
	}

	//memcpy(img, blur, w*h*sizeof(UCHAR));

	if(kernel)free(kernel);
	//if(blur)free(blur);

	return i;
}

DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, INT *img, INT *blur)
{
	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT k = floor((FLOAT)n/(FLOAT)2);

	INT xx = NULL;
	INT yy = NULL;

	if(!img || !blur)
		return FALSE;

	FLOAT sum = NULL;

	FLOAT *kernel = (FLOAT *)calloc(n*n, sizeof(FLOAT));

	memcpy(blur, img, w*h*sizeof(FLOAT));
	GetGaussianKernel(n, sigma, kernel);

	//Step through image
	for(y = n; y < h-n; y++){
		for(x = n; x < w-n; x++){

			sum = NULL;
			//Step through kernel at each pixel
			for(yy = 0; yy < n; yy++){
				for(xx = 0; xx < n; xx++){
				
					i = ( (y + (yy-k))*w + (x+xx-k));
					sum += img[i] * kernel[(yy*n) + (xx)];
				}
			}
			i = (y*w) + x;
			if(sum > 255)
				blur[i] = sum;
			else if(sum < 0)
				blur[i] =  0;
			else
				blur[i] = sum;
		}
	}

	//memcpy(img, blur, w*h*sizeof(UCHAR));

	if(kernel)free(kernel);
	//if(blur)free(blur);

	return i;
}

DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, FLOAT *img)
{
	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT k = floor((FLOAT)n/(FLOAT)2);

	INT xx = NULL;
	INT yy = NULL;

	

	if(!img)
		return FALSE;

	FLOAT sum = NULL;
	FLOAT *blur = (FLOAT *)calloc(w*h, sizeof(FLOAT));
	FLOAT *kernel = (FLOAT *)calloc(n*n, sizeof(FLOAT));

	memcpy(blur, img, w*h*sizeof(FLOAT));
	GetGaussianKernel(n, sigma, kernel);

	//Step through image
	for(y = n; y < h-n; y++){
		for(x = n; x < w-n; x++){

			sum = NULL;
			//Step through kernel at each pixel
			for(yy = 0; yy < n; yy++){
				for(xx = 0; xx < n; xx++){
				
					i = ( (y + (yy-k))*w + (x+xx-k));
					sum += img[i] * kernel[(yy*n) + (xx)];
				}
			}
			i = (y*w) + x;
			if(sum > 255)
				blur[i] = sum;
			else if(sum < 0)
				blur[i] =  0;
			else
				blur[i] = sum;
		}
	}

	memcpy(img, blur, w*h*sizeof(UCHAR));

	if(kernel)free(kernel);
	if(blur)free(blur);

	return i;
}