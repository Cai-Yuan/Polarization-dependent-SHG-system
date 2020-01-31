#include "stdafx.h"
#include "IntegralImage.h"




//Untested
VOID GetIntegralImage(UINT width, UINT height, UCHAR *imgIn, UINT *imgIntegral)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	UINT xMax = w;
	UINT yMax = h*w;

	memset(imgIntegral, 0x00, w*h*sizeof(unsigned int));
	
	//First Pixel
	imgIntegral[0] = imgIn[0];

	//First Row
	y = 0;
	for(x = 1; x < w; x++){
		imgIntegral[x] = imgIn[x] + imgIntegral[x-1];
	}

	//First Column
	x = 0;
	for(y = w; y < h; y+=w){
		imgIntegral[y] = imgIn[y] + imgIntegral[y - w];
	}

	//Remaining Image
	for(y = w; y < h; y+=w){
		for(x = 1; x < w; x++){
			imgIntegral[y] = imgIn[x+y] - imgIntegral[x + y - w - 1] + imgIntegral[x + y - 1] + imgIntegral[x + y - w];
		}
	}

}

VOID GetIntegralImage(UINT width, UINT height, UINT *imgIn, FLOAT *imgIntegral)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	UINT xMax = w;
	UINT yMax = h*w;

	memset(imgIntegral, 0x00, w*h*sizeof(unsigned int));
	
	//First Pixel
	imgIntegral[0] = imgIn[0];

	//First Row
	y = 0;
	for(x = 1; x < w; x++){
		imgIntegral[x] = imgIn[x] + imgIntegral[x-1];
	}

	//First Column
	x = 0;
	for(y = w; y < h; y+=w){
		imgIntegral[y] = imgIn[y] + imgIntegral[y - w];
	}

	//Remaining Image
	for(y = w; y < h; y+=w){
		for(x = 1; x < w; x++){
			imgIntegral[x+y] = imgIn[x+y] - imgIntegral[x + y - w - 1] + imgIntegral[x + y - 1] + imgIntegral[x + y - w];
			if(imgIntegral[x+y] > 0)
				imgIntegral[x+y] = imgIntegral[x+y];
		}
	}

}

VOID GetIntegralImage(UINT width, UINT height, UCHAR *imgIn, FLOAT *imgIntegral)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	UINT xMax = w;
	UINT yMax = h*w;

	memset(imgIntegral, 0x00, w*h*sizeof(unsigned int));
	
	//First Pixel
	imgIntegral[0] = imgIn[0];

	//First Row
	y = 0;
	for(x = 1; x < w; x++){
		i = y*w + x;
		imgIntegral[i] = imgIn[i] + imgIntegral[i-1];
		//imgIntegral[x] = imgIn[x] + imgIntegral[x-1];
	}

	//First Column
	x = 0;
	for(y = w; y < h; y+=w){
		i = y*w + x;
		imgIntegral[i] = imgIn[i] + imgIntegral[i - w];
		//imgIntegral[y] = imgIn[y] + imgIntegral[y - w];
	}

	//Remaining Image
	for(y = 1; y < h; y++){
		for(x = 1; x < w; x++){
			i = y*w + x;
			imgIntegral[i] = imgIn[i] - imgIntegral[i - w - 1] + imgIntegral[i - 1] + imgIntegral[i - w];
			if(imgIntegral[i] > 0)
				imgIntegral[i] = imgIntegral[i];

			//imgIntegral[x+y] = imgIn[x+y] - imgIntegral[x + y - w - 1] + imgIntegral[x + y - 1] + imgIntegral[x + y - w];
			//if(imgIntegral[x+y] > 0)
			//	imgIntegral[x+y] = imgIntegral[x+y];
		}
	}

}

//Supports up to a 8-Bit Grayscale 4104x4104
VOID GetIntegralImage(UINT width, UINT height, UCHAR *imgIn, UINT64 *imgIntegral)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	UINT xMax = w;
	UINT yMax = h*w;

	memset(imgIntegral, 0x00, w*h*sizeof(unsigned int));
	
	//First Pixel
	imgIntegral[0] = imgIn[0];

	//First Row
	y = 0;
	for(x = 1; x < w; x++){
		imgIntegral[x] = imgIn[x] + imgIntegral[x-1];
	}

	//First Column
	x = 0;
	for(y = w; y < h; y+=w){
		imgIntegral[y] = imgIn[y] + imgIntegral[y - w];
	}

	//Remaining Image
	for(y = w; y < h; y+=w){
		for(x = 1; x < w; x++){
			imgIntegral[y] = imgIn[x+y] - imgIntegral[x + y - w - 1] + imgIntegral[x + y - 1] + imgIntegral[x + y - w];
		}
	}

}

INT64 GetIntegralImageSum(UINT width, UINT height, UINT left, UINT right, UINT top, UINT bottom, FLOAT *imgIntegral)
{	
	UINT i;
	UINT w = width;
	UINT h = height; 
	UINT wSum = (right-left);
	UINT hSum = (bottom-top);
	UINT sum = 0;

	UINT dx = right - left;
	UINT dy = bottom - top;

	i = top*w + left;

	sum+= imgIntegral[i];

	//if(sum > 0)
	//	sum = sum;

	i = bottom*w + right;
	sum+= imgIntegral[i];

	i = bottom*w + left;
	sum-= imgIntegral[i];

	i = top*w + right;
	sum-= imgIntegral[i];

	//sum = sum / (wSum*hSum);

	return sum;
}


//Only Written from Odd Kernel Sizes
VOID GetImageFromIntegralImage(UINT width, UINT height, UINT n, FLOAT *imgIntegral, UCHAR *imgOut, UINT flags)
{
	//nxn is the kernel size
	//Assure it is Odd Sized
	if(n%3){
		return;
	}

	UINT w, h;
	UINT64 temp;
	//UINT temp;
	temp  = flags&II_LOWDETAIL;
	if((flags&II_LOWDETAIL) == II_LOWDETAIL){}

	w = width - (width%n);
	h = height - (height%n);
	
	UINT wOut = w/n;
	UINT hOut = h/n;
	UINT xOut = 0;
	UINT yOut = 0;

	UINT x, y, i;
	UINT s = (n-1)/2;

	for(y = s; y < h-s; y+=n, yOut++){
		xOut = 0;
		for(x = s; x < w-s; x+=n, xOut++){
			i = (yOut*wOut) + xOut;
			temp = (UINT) GetIntegralImageSum(w, h, x-s, x+s, y-s, y+s, imgIntegral);
			if(temp > 255)
				imgOut[i] = 255;
			else if(temp < 0)
				imgOut[i] = (-1)*(temp);
			else
				imgOut[i] = temp;
		}
	}
	
}
	



