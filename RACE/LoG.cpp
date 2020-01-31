#include "stdafx.h"
#include "LoG.h"

VOID BlobDetect(UINT width, UINT height, FLOAT s, UCHAR *imgIn, UCHAR *imgOut)
{

	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	FLOAT sigma = (float)(s-1)/(float)3;
	FLOAT n = floor(6*sigma) + 1;
	UINT origin = (n-1)/2;
	UINT xStart = origin;
	UINT yStart = origin;
	UINT xEnd = w - origin;
	UINT yEnd = h - origin;
	INT kernelSum;
	UINT iI; //Index of Image
	UINT iK; //Index of Kernel

	INT *kernel = (INT *) malloc(n*n*sizeof(INT));
	memset(imgOut, 0x00, w*h*sizeof(UCHAR));

	GetBlobDetectKernel(s, kernel);

	INT xN, yN;
	INT xK, yK;
	INT xI, yI;
	for(y = yStart; y < yEnd; y++){
		for(x = xStart; x < xEnd; x++){

			iK = 0;
			kernelSum = 0;
			for(yN = 0; yN < n; yN++){
				for(xN = 0; xN < n; xN++, iK++){

				xK = (xN - origin);
				yK = (yN - origin);
				xI = xK + x;
				yI = yK + y;
				i = yI*w + xI;
				kernelSum += kernel[iK]*imgIn[i];
				}
			}
			i = y*w + x;
			//kernelSum = abs((float)kernelSum/(float)(n*n));
			kernelSum =(float)kernelSum/(float)(n*n);
			kernelSum = 255 + kernelSum;
			if(kernelSum > 255)
				imgOut[i] = 255;
			else if(kernelSum < 0)
				imgOut[i] = 0;
			else
				imgOut[i] = kernelSum;
		}
	}


}

VOID BlobDetect(UINT width, UINT height, FLOAT s, UCHAR *imgIn, INT *imgOut)
{

	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	FLOAT sigma = (float)(s-1)/(float)3;
	FLOAT n = floor(6*sigma) + 1;
	UINT origin = (n-1)/2;
	UINT xStart = origin;
	UINT yStart = origin;
	UINT xEnd = w - origin;
	UINT yEnd = h - origin;
	INT kernelSum;
	UINT iI; //Index of Image
	UINT iK; //Index of Kernel

	INT *kernel = (INT *) malloc(n*n*sizeof(INT));
	memset(imgOut, 0x00, w*h*sizeof(UCHAR));

	GetBlobDetectKernel(s, kernel);

	INT xN, yN;
	INT xK, yK;
	INT xI, yI;
	for(y = yStart; y < yEnd; y++){
		for(x = xStart; x < xEnd; x++){

			iK = 0;
			kernelSum = 0;
			for(yN = 0; yN < n; yN++){
				for(xN = 0; xN < n; xN++, iK++){

				xK = (xN - origin);
				yK = (yN - origin);
				xI = xK + x;
				yI = yK + y;
				i = yI*w + xI;
				kernelSum += kernel[iK]*imgIn[i];
				}
			}
			i = y*w + x;
			//imgOut[i] = abs((float)kernelSum);
			imgOut[i] = kernelSum;
		}
	}


}



VOID GetBlobDetectKernel(FLOAT s, INT *kernel)
{
	FLOAT LoG = 0;
	FLOAT sigma = (float)(s-1)/(float)3;
	FLOAT n = floor(6*sigma) + 1;

	UINT origin = (float)(n-1)/(float)2;

	UINT x, y, i;
	INT X, Y;
	UINT XXYY;
	FLOAT sigmaXXYY;


	for(y = 0; y < n; y++){
		for(x = 0; x < n; x++){

			i = y*n + x;
			X = (x - origin);
			Y = (y - origin);
			XXYY = X*X + Y*Y;
			sigmaXXYY = (float)XXYY/(float)(2*sigma*sigma);
			LoG = -((float)1/(float)(3.1415926*sigma*sigma*sigma*sigma))*((float)1 - (float)sigmaXXYY)*(exp((float)-sigmaXXYY));
			LoG = LoG*1000;
			kernel[i] = LoG;
		}
	}

}