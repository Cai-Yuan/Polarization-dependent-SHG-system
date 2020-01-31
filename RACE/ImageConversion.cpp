#include "stdafx.h"

//Fast Int image to Uchar
VOID fitocImage(UINT width, UINT height, INT *imgIn, UCHAR *imgOut)
{
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;
	INT max, min;
	FLOAT a;

	I = w * h;
	for(i = 0; i < I; i++){
		if(imgIn[i] < 0)
			imgOut[i] = 0;
		else if(imgIn[i] > 255)
			imgOut[i] = 255;
		else
			imgOut[i] = imgIn[i];
	}
}
//True Int image to Uchar
VOID titocImage(UINT width, UINT height, UINT maxRange, INT *imgIn, UCHAR *imgOut, UCHAR *midPlane)
{
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;
	UINT range;
	UINT temp;
	UINT avg;
	INT max, min, rSum;
	FLOAT a;

	I = w * h;
	max = imgIn[0];
	min = max;
	rSum= 0;

	for(i = 1; i < I; i++){
		if(imgIn[i] <= min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}
	range = max - min;
	a = (FLOAT)range/(FLOAT)maxRange;
	*midPlane = (min) + (maxRange * a);

	for(i = 0; i < I; i++){
		temp = (imgIn[i] + 255) * a;
		if(temp > 255)		imgOut[i] = 255;
		else if(temp < 0)	imgOut[i] = 0;
		else				imgOut[i] = temp;
		rSum += temp;
	}

	avg = rSum/I;
	for(i = 0; i < I; i++){
		imgOut[i] = imgOut[i] + (173 - avg);
	}
}

VOID DerivativeToBMP(UINT width, UINT height, INT *imgIn, PIXEL *imgOut)
{
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;
	UINT range;
	UINT temp;
	UINT avg;
	UINT maxRange = 255;
	UINT midPlane = maxRange/2.0;
	INT max, min, rSum;
	FLOAT a;

	I = w * h;
	max = imgIn[0];
	min = max;
	rSum= 0;

	for(i = 1; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}

	range = max - min;
	a = (FLOAT)range/(FLOAT)maxRange;

	i = 0;
	for(y = 0; y < h; y++){
		for(x = 0; x < w; x++, i++){
			I = ((FLOAT) a * (FLOAT)imgIn[i]) + midPlane;
			if(I > 255)
				imgOut[i].r = 255;
			else if(I < 0)
				imgOut[i].r = 0;
			else
				imgOut[i].r = I;

			imgOut[i].b = imgOut[i].r;
			imgOut[i].g = imgOut[i].r;
		}
	}

}

VOID DerivativeToBMPColor(UINT width, UINT height, INT *imgIn, PIXEL *imgOut)
{
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;
	INT min, max;
	FLOAT a, b;
	INT temp;

	I = w * h;

	
	min = 255;
	max = -255;

	for(i = 0; i < I; i++){
		if(imgIn[i] <= min)
			min = imgIn[i];
		else if(imgIn[i] >= max)
			max = imgIn[i];
	}

	if(min != 0)	a = (FLOAT)255/(FLOAT)min;
	if(max != 0)	b = (FLOAT)255/(FLOAT)max;

	for(i = 0; i < I; i++){
		temp = imgIn[i];
		if(temp < 0){
			imgOut[i].r = temp*a;
			imgOut[i].g = 0;
			imgOut[i].b = 0;
		}	
		else{
			imgOut[i].b = temp*b;
			imgOut[i].g = 0;
			imgOut[i].r = 0;
		}
	}

}

VOID Color256toPIXEL(UINT width, UINT height, UCHAR *imgIn, PIXEL *imgOut)
{
	UINT x, y, i, II;
	UINT w = width;
	UINT h = height;

	if(!imgIn || !imgOut)
		return;

	II = w * h;
	for(i = 0; i < II; i++){
		imgOut[i].r = ((imgIn[i]>>2)%4)*64;
		imgOut[i].g = (imgIn[i]%4)*64;
		imgOut[i].b = (imgIn[i]>>6)*64;
		//imgOut[i].r = (imgIn[i]>>5)*32;
		//imgOut[i].g = ((imgIn[i] & 28)>>2)*32;
		//imgOut[i].b = (imgIn[i] & 3) *64;

	}

}

VOID UCHARtoPIXEL(UINT width, UINT height, UCHAR *imgIn, PIXEL *imgOut)
{
	UINT x, y, i, II;
	UINT w = width;
	UINT h = height;

	if(!imgIn || !imgOut)
		return;

	II = w * h;
	for(i = 0; i < II; i++){
		imgOut[i].r = imgIn[i];
		imgOut[i].g = imgIn[i];
		imgOut[i].b = imgIn[i];
	}

}

VOID UCHARtoPIXEL(UINT width, UINT height, UCHAR *imgR, UCHAR *imgG, UCHAR *imgB, PIXEL *imgOut)
{

	DWORD w = width;
	DWORD h = height;
	DWORD max = w*h;
	DWORD i = NULL;

	BOOL flagR = NULL;
	BOOL flagG = NULL;
	BOOL flagB = NULL;

	if(!imgR){
		imgR = (UCHAR *)calloc(w*h, sizeof(UCHAR));
		flagR = TRUE;
	}
	if(!imgG){
		imgG = (UCHAR *)calloc(w*h, sizeof(UCHAR));
		flagG = TRUE;
	}
	if(!imgB){
		imgB = (UCHAR *)calloc(w*h, sizeof(UCHAR));
		flagB = TRUE;
	}

	memset(imgOut, NULL, width*height*sizeof(PIXEL));

	for(i = 0; i < max; i++)
	{
		imgOut[i].r = imgR[i];
		imgOut[i].g = imgG[i];
		imgOut[i].b = imgB[i];

	}

	if(flagR){
		if(imgR)free(imgR);
	}
	if(flagG){
		if(imgG)free(imgG);
	}
	if(flagB){
		if(imgB)free(imgB);
	}
}

DWORD FLOATtoPIXEL(DWORD width, DWORD height, FLOAT *inR, FLOAT *inG, FLOAT *inB, PIXEL *pix)
{

	DWORD i = NULL;
	DWORD w = width;
	DWORD h = height;
	DWORD max = w*h;

	UCHAR *imgR = NULL;
	UCHAR *imgG = NULL;
	UCHAR *imgB = NULL;

	//imgR = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgG = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgB = (UCHAR *)calloc(max, sizeof(UCHAR));

	imgR = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgG = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgB = (UCHAR *)malloc(max*sizeof(UCHAR));

	if(inR)	ClampData(max, 0, 255, inR, imgR);
	if(inG)	ClampData(max, 0, 255, inG, imgG);
	if(inB)	ClampData(max, 0, 255, inB, imgB);

	for(i = 0; i < max; i++){

		pix[i].r = imgR[i];
		pix[i].g = imgG[i];
		pix[i].b = imgB[i];

	}

	if(imgR)free(imgR);
	if(imgG)free(imgG);
	if(imgB)free(imgB);

	return TRUE;
}

DWORD FLOATtoPIXEL(DWORD width, DWORD height, FLOAT *inGray, FLOAT *inR, FLOAT *inG, FLOAT *inB, PIXEL *pix)
{

	DWORD i = NULL;
	DWORD w = width;
	DWORD h = height;
	DWORD max = w*h;

	UCHAR *imgR = NULL;
	UCHAR *imgG = NULL;
	UCHAR *imgB = NULL;
	UCHAR *imgGray = NULL;

	//imgR = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgG = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgB = (UCHAR *)calloc(max, sizeof(UCHAR));

	imgR = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgG = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgB = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgGray = (UCHAR *)malloc(max*sizeof(UCHAR));

	if(inR)		ClampData(max, 0, 255, inR, imgR);
	if(inG)		ClampData(max, 0, 255, inG, imgG);
	if(inB)		ClampData(max, 0, 255, inB, imgB);
	if(inGray)	ClampData(max, 0, 255, inGray, imgGray);

	for(i = 0; i < max; i++){

		pix[i].r = imgGray[i];
		pix[i].g = imgGray[i];
		pix[i].b = imgGray[i];

		if(imgR[i])
			pix[i].r = (0.5*imgR[i]) + (0.5*pix[i].r);

		if(imgG[i])
			pix[i].g = (0.5*imgG[i]) + (0.5*pix[i].g);

		if(imgB[i])
			pix[i].b = (0.5*imgB[i]) + (0.5*pix[i].b);

	}

	if(imgR)free(imgR);
	if(imgG)free(imgG);
	if(imgB)free(imgB);

	return TRUE;
}

DWORD MaximumIntensityProjection(DWORD width, DWORD height, FLOAT *in, FLOAT *inR, FLOAT *inG, FLOAT *inB, PIXEL *pix)
{

	if(!in || !inR || !inG || !inB || !pix)
		return FALSE;

	DWORD i = NULL;
	DWORD w = width;
	DWORD h = height;
	DWORD max = w*h;

	UCHAR *img = NULL;
	UCHAR *imgR = NULL;
	UCHAR *imgG = NULL;
	UCHAR *imgB = NULL;

	//img = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgR = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgG = (UCHAR *)calloc(max, sizeof(UCHAR));
	//imgB = (UCHAR *)calloc(max, sizeof(UCHAR));

	img = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgR = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgG = (UCHAR *)malloc(max*sizeof(UCHAR));
	imgB = (UCHAR *)malloc(max*sizeof(UCHAR));

	memset(pix, 0x0, width*height*sizeof(PIXEL));

	if(in)	ClampData(max, 0, 255, in, img);
	if(inR)	ClampData(max, 0, 255, inR, imgR);
	if(inG)	ClampData(max, 0, 255, inG, imgG);
	if(inB)	ClampData(max, 0, 255, inB, imgB);

	for(i = 0; i < max; i++){

		if(img[i] > imgR[i] && img[i] > imgG[i] && img[i] > imgB[i]){
			pix[i].r = img[i];
			pix[i].g = img[i];
			pix[i].b = img[i];
		}
		else if(imgB[i] >= imgR[i] && imgB[i] >= imgG[i] && imgB[i] >= img[i]){
			pix[i].r = 0;//img[i];
			pix[i].g = 0;//img[i];
			pix[i].b = imgB[i];		
		}
		else if(imgG[i] >= imgR[i] && imgG[i] >= img[i] && imgG[i] >= imgB[i]){
			pix[i].r = 0;//img[i];
			pix[i].g = imgG[i];
			pix[i].b = 0;//img[i];
		}
		else{ //if(imgR[i] > img[i] && imgR[i] > imgG[i] && imgR[i] > imgB[i]){
			pix[i].r = imgR[i];
			pix[i].g = 0;//img[i];
			pix[i].b = 0;//img[i];		
		}

	}

	if(img)free(img);
	if(imgR)free(imgR);
	if(imgG)free(imgG);
	if(imgB)free(imgB);

	return TRUE;
}

//DWORD FLOATtoPIXEL(DWORD width, DWORD height, FLOAT *gray, FLOAT *mask, PIXEL *pix)
//{
//
//	DWORD i = NULL;
//	DWORD w = width;
//	DWORD h = height;
//	DWORD max = w*h;
//
//	if(!gray || !mask)
//		return FALSE;
//
//	UCHAR *g = (UCHAR *) calloc(w*h, sizeof(UCHAR));
//	UCHAR *m = (UCHAR *) calloc(w*h, sizeof(UCHAR));
//
//	if(gray)ClampData(max, 0, 255, gray, g);
//	if(inG)	ClampData(max, 0, 255, mask, m);
//
//	for(i = 0; i < max; i++){
//
//		pix[i].r = g[i];
//		pix[i].g = imgG[i];
//		pix[i].b = imgB[i];
//
//	}
//
//	if(imgR)free(imgR);
//	if(imgG)free(imgG);
//	if(imgB)free(imgB);
//
//	return TRUE;
//}


VOID INTtoUCHAR(UINT width, UINT height, INT *imgIn, UCHAR *imgOut)
{
	INT I;
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	INT range;
	UINT temp;
	UINT avg;
	UINT maxRange = 255;
	UINT midPlane = maxRange/2.0;
	INT max, min, rSum;
	FLOAT a;

	I = w * h;
	max = imgIn[0];
	min = max;
	rSum= 0;

	GetMaxMin(w*h, &max, &min, imgIn);

	range = max - min;
	a = (FLOAT)maxRange/(FLOAT)range;

	min = Magnitude(min);
	i = 0;
	for(y = 0; y < h; y++){
		for(x = 0; x < w; x++, i++){
			//I = ((FLOAT) a * (FLOAT)imgIn[i]) + midPlane;
			I = ((FLOAT) a * (FLOAT)imgIn[i]) + min;
			if(I > 255){
				imgOut[i] = 255;
			}
			else if(I < 0){
				imgOut[i] = 0;
			}
			else{
				imgOut[i] = I;
			}
		}
	}

}
	

DWORD PIXELtoUCHAR(UINT width, UINT height, PIXEL *imgIn, UCHAR *imgOut, DWORD flag)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	UINT max = w*h;

	if(!imgIn || !imgOut)
		return FALSE;

	DWORD nSame = GetSimilarChannelsBMP(w, h, imgIn);

	if(nSame >= max){
		for(i = 0; i < max; i++){
			//if(imgIn[i].r == imgIn[i].g && imgIn[i].g == imgIn[i].b)
			imgOut[i] = imgIn[i].r;
		}
		//memcpy(imgOut, imgIn, max*sizeof(PIXEL));
	}
	else{
		for(i = 0; i < max; i++){
			//PTU_LUMINOSITY
			imgOut[i] = imgIn[i].r*0.21 + imgIn[i].g*0.71 + imgIn[i].b*0.07;
		}
	}

	return TRUE;
}

DWORD PIXELtoFLOAT(UINT width, UINT height, PIXEL *imgIn, FLOAT *imgOut, DWORD flag)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;
	UINT max = w*h;

	if(!imgIn || !imgOut)
		return FALSE;

	DWORD nSame = GetSimilarChannelsBMP(w, h, imgIn);

	if(nSame >= max){
		for(i = 0; i < max; i++){
			imgOut[i] = imgIn[i].r;
		}
	}
	else{
		for(i = 0; i < max; i++){
			//PTU_LUMINOSITY
			imgOut[i] = imgIn[i].r*0.21 + imgIn[i].g*0.71 + imgIn[i].b*0.07;
		}
	}

	return TRUE;
}

VOID CHARtoPIXEL(UINT width, UINT height, CHAR *imgIn, PIXEL *imgOut)
{
	INT min;
	INT max;
	INT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = imgIn[0];
	max = imgIn[0];

	for(i = 0; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}

	if(max != min){
		scale = (FLOAT)(CHAR_MAX - CHAR_MIN)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-min);
				imgOut[i].r = scale*temp;
				imgOut[i].g = imgOut[i].r;
				imgOut[i].b = imgOut[i].r;

		}
	}
	else{
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-abs(max));
				imgOut[i].r = temp;
				imgOut[i].g = imgOut[i].r;
				imgOut[i].b = imgOut[i].r;

		}
	}

}

//VOID UCHARtoBGR(UINT width, UINT height, UCHAR *imgIn, BGR *imgOut)
//{
//	UINT x, y, i, II;
//	UINT w = width;
//	UINT h = height;
//
//	II = w * h;
//	for(i = 0; i < II; i++){
//		imgOut[i].r = imgIn[i];
//		imgOut[i].g = imgOut[i].r;
//		imgOut[i].b = imgOut[i].r;
//	}
//}

VOID UCHARtoINT(UINT width, UINT height, UCHAR *imgIn, INT *imgOut)
{
	UINT x, y, i, II;
	UINT w = width;
	UINT h = height;

	II = w * h;
	for(i = 0; i < II; i++){
		imgOut[i] = imgIn[i];
	}
}

DWORD UCHARtoFLOAT(UINT width, UINT height, UCHAR *imgIn, FLOAT *imgOut)
{
	UINT x, y, i, II;
	UINT w = width;
	UINT h = height;

	II = w * h;

	if(!imgIn || !imgOut)
		return FALSE;

	for(i = 0; i < II; i++){
		imgOut[i] = imgIn[i];
	}
	
	return i;
}

VOID INTtoPIXEL(UINT width, UINT height, INT *imgIn, PIXEL *imgOut)
{
	INT min;
	INT max;
	INT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = imgIn[0];
	max = imgIn[0];

	for(i = 0; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}

	if(max != min){
		scale = (FLOAT)(255 - 0)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-min);
			temp = scale*temp;
			if(temp <= 255){
				imgOut[i].r = temp;
				imgOut[i].g = imgOut[i].r;
				imgOut[i].b = imgOut[i].r;
			}

		}
	}
	else{
		memset(imgOut, 0x00, w*h*sizeof(PIXEL));
		for(i = 0; i < I; i++){
			if(imgIn[i]){
				imgOut[i].r = 255;
				imgOut[i].g = imgOut[i].r;
				imgOut[i].b = imgOut[i].r;
			}

		}
	}

	//else{
	//	for(i = 0; i < I; i++){
	//		temp = (imgIn[i]-abs(max));
	//		if(temp < 255){
	//			imgOut[i].r = temp;
	//			imgOut[i].g = imgOut[i].r;
	//			imgOut[i].b = imgOut[i].r;
	//		}

	//	}
	//}
}

VOID UINTtoPIXEL(UINT width, UINT height, UINT *imgIn, PIXEL *imgOut)
{
	INT min;
	INT max;
	INT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = imgIn[0];
	max = imgIn[0];

	for(i = 0; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}

	if(max != min){
		scale = (FLOAT)(255 - 0)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-min);
			temp = (INT)((FLOAT)scale*(FLOAT)temp);
			imgOut[i].r = temp/*(UCHAR) ((FLOAT)scale*(FLOAT)temp)*/;
			imgOut[i].g = imgOut[i].r;
			imgOut[i].b = imgOut[i].r;

			//imgOut[i].r = 255;
			if(imgOut[i].r > 255){
				//Clamp
				imgOut[i].r = 255;
				imgOut[i].g = 255;
				imgOut[i].b = 255;
			}
		}
	}
	else{
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-abs(max));
			imgOut[i].r = temp;
			imgOut[i].g = imgOut[i].r;
			imgOut[i].b = imgOut[i].r;

		}
	}

}

VOID UINTtoUCHAR(UINT width, UINT height, UINT *imgIn, UCHAR *imgOut)
{
	INT min;
	INT max;
	FLOAT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = imgIn[0];
	max = imgIn[0];


	if(max != min){
		scale = (FLOAT)(255 - 0)/(FLOAT)(max - min);
		scale = (FLOAT)(255 - 0)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-min);
			imgOut[i] = (UCHAR)((FLOAT)scale*(FLOAT)temp); 

		}
	}
	else{
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-abs(max));
			imgOut[i] = (UCHAR) temp;

		}
	}

}

VOID WindowUINT(UINT width, UINT height, UINT minWindow, UINT maxWindow, UINT *img)
{

	INT min;
	INT max;
	FLOAT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = img[0];
	max = img[0];

	for(i = 0; i < I; i++){
		if(img[i] < min)
			min = img[i];
		else if(img[i] > max)
			max = img[i];
	}

	if(max != min){
		scale = (FLOAT)(maxWindow - minWindow)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (img[i]-min);
			temp = (FLOAT)((FLOAT)scale*(FLOAT)temp); 
			*(img + i) = (UINT) temp;

		}
	}
	//else{
	//	for(i = 0; i < I; i++){
	//		temp = (imgIn[i]-abs(max));
	//		imgOut[i] = (UCHAR) temp;

	//	}
	//}

}

VOID WindowINT(UINT width, UINT height, UINT minWindow, UINT maxWindow, INT *img)
{

	INT min;
	INT max;
	FLOAT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = img[0];
	max = img[0];

	for(i = 0; i < I; i++){
		if(img[i] < min)
			min = img[i];
		else if(img[i] > max)
			max = img[i];
	}

	if(max != min){
		scale = (FLOAT)(maxWindow - minWindow)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (img[i]-min);
			temp = (FLOAT)((FLOAT)scale*(FLOAT)temp); 
			*(img + i) = (INT) temp;

		}
	}
	//else{
	//	for(i = 0; i < I; i++){
	//		temp = (imgIn[i]-abs(max));
	//		imgOut[i] = (UCHAR) temp;

	//	}
	//}

}

VOID WindowFLOAT(UINT width, UINT height, UINT minWindow, UINT maxWindow, FLOAT *img)
{

	INT min;
	INT max;
	FLOAT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = img[0];
	max = img[0];

	for(i = 0; i < I; i++){
		if(img[i] < min)
			min = img[i];
		else if(img[i] > max)
			max = img[i];
	}

	if(max != min){
		scale = (FLOAT)(maxWindow - minWindow)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (img[i]-min);
			temp = (FLOAT)((FLOAT)scale*(FLOAT)temp); 
			*(img + i) = (FLOAT) temp;

		}
	}
	//else{
	//	for(i = 0; i < I; i++){
	//		temp = (imgIn[i]-abs(max));
	//		imgOut[i] = (UCHAR) temp;

	//	}
	//}

}

VOID FLOATtoPIXEL(UINT width, UINT height, FLOAT *imgIn, PIXEL *imgOut)
{
	INT min;
	INT max;
	INT temp;
	FLOAT scale;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = imgIn[0];
	max = imgIn[0];

	for(i = 0; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}

	if(max != min){
		scale = (FLOAT)(255 - 0)/(FLOAT)(max - min);
	
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-min);
				imgOut[i].r = scale*temp;
				imgOut[i].g = imgOut[i].r;
				imgOut[i].b = imgOut[i].r;

		}
	}
	else{
		for(i = 0; i < I; i++){
			temp = (imgIn[i]-abs(max));
				imgOut[i].r = temp;
				imgOut[i].g = imgOut[i].r;
				imgOut[i].b = imgOut[i].r;

		}
	}

}

VOID FLOATtoUCHAR(UINT width, UINT height, FLOAT *imgIn, UCHAR *imgOut)
{
	INT temp;
	FLOAT scale;

	DWORD i = NULL;
	DWORD x = NULL;
	DWORD y = NULL;

	DWORD w = width;
	DWORD h = height;

	DWORD max = w*h;

	for(i = 0; i < max; i++){

		if(imgIn[i] > 255){
			imgOut[i] = 255;
		}
		else if(imgIn[i] < 0){
			imgOut[i] = 0;
		}
		else{
			imgOut[i] = imgIn[i];
		}
	}

}

//VOID FLOATtoUCHAR(UINT width, UINT height, FLOAT *imgIn, UCHAR *imgOut)
//{
//	INT min;
//	INT max;
//	INT temp;
//	FLOAT scale;
//	UINT x, y, i, I;
//	UINT w = width;
//	UINT h = height;
//
//	I = w * h;
//
//	min = imgIn[0];
//	max = imgIn[0];
//
//	for(i = 0; i < I; i++){
//		if(imgIn[i] < min)
//			min = imgIn[i];
//		else if(imgIn[i] > max)
//			max = imgIn[i];
//	}
//
//	if(max != min){
//		scale = (FLOAT)(255 - 0)/(FLOAT)(max - min);
//	
//		for(i = 0; i < I; i++){
//			temp = (imgIn[i]-min);
//				imgOut[i] = (UCHAR) (scale*temp);
//
//
//		}
//	}
//	else{
//		for(i = 0; i < I; i++){
//			temp = (imgIn[i]-abs(max));
//				imgOut[i] = (UCHAR) (temp);
//
//		}
//	}
//
//}

VOID colorINTtoPIXEL(UINT width, UINT height, INT *imgIn, PIXEL *imgOut)
{
	FLOAT min;
	FLOAT max;
	INT temp;
	FLOAT scalePos, scaleNeg;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = 1000;//imgIn[0];
	max = -1000;//imgIn[0];

	for(i = 0; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}
	scalePos = (FLOAT)255 / (FLOAT)max;
	scaleNeg = (FLOAT)255 / (FLOAT)min;
	
	for(i = 0; i < I; i++){
		temp = (imgIn[i]);
		if(temp < 0){
			imgOut[i].r = scaleNeg*temp;
			imgOut[i].g = 0;
			imgOut[i].b = 0;
		}
		else if(temp > 0){
			imgOut[i].b = scalePos*temp;
			imgOut[i].g = 0;
			imgOut[i].r = 0;
		}
		else{
			imgOut[i].r = 0;
			imgOut[i].g = imgOut[i].r;
			imgOut[i].b = imgOut[i].r;
		}
	}


}

VOID colorFLOATtoPIXEL(UINT width, UINT height, FLOAT *imgIn, PIXEL *imgOut)
{
	FLOAT min;
	FLOAT max;
	INT temp;
	FLOAT scalePos, scaleNeg;
	UINT x, y, i, I;
	UINT w = width;
	UINT h = height;

	I = w * h;

	min = 1000;//imgIn[0];
	max = -1000;//imgIn[0];

	for(i = 0; i < I; i++){
		if(imgIn[i] < min)
			min = imgIn[i];
		else if(imgIn[i] > max)
			max = imgIn[i];
	}
	scalePos = (FLOAT)255 / (FLOAT)max;
	scaleNeg = (FLOAT)255 / (FLOAT)min;
	
	for(i = 0; i < I; i++){
		temp = (imgIn[i]);
		if(temp < 0){
			imgOut[i].r = scaleNeg*temp;
			imgOut[i].g = 0;
			imgOut[i].b = 0;
		}
		else if(temp > 0){
			imgOut[i].b = scalePos*temp;
			imgOut[i].g = 0;
			imgOut[i].r = 0;
		}
		else{
			imgOut[i].r = 0;
			imgOut[i].g = imgOut[i].r;
			imgOut[i].b = imgOut[i].r;
		}
	}


}

VOID Conversion12bTo8b(UINT w, UINT h, SHORT *img12, UCHAR *img8)
{

	UINT max = w*h;
	UINT lByte = 0;
	UINT bByte = 0;
	FLOAT temp = 0;

	if(img12 && img8){

		for(INT i = 0; i < max; i++)
		{
			lByte = (img12[i]&255)<<8;
			bByte = img12[i]>>8;
			temp = ((lByte+bByte) / 4096);
			img8[i] = temp;
		}

	}


}

//Compresses an image that uses 2-Bytes per pixel to 1-Byte Per Pixel
VOID Compress(UINT w, UINT h, UCHAR *imgDest, UCHAR *imgSrc)
{

	UINT i = 0;
	UINT p = 0;
	INT lByte;
	INT bByte;
	UINT max = w*h;
	FLOAT temp;

	if(imgDest && imgSrc){

		for(i = 0, p = 0; i < max; i++, p+=2)
		{
			lByte = imgSrc[p];
			bByte = imgSrc[p+1];
			lByte = lByte/*<<8*/;
			bByte = bByte;
			temp = bByte;
			//temp = (temp / (FLOAT)4096);
			//temp = temp * 256.0;
			//temp = (imgSrc[p]<<8+imgSrc[p+1]/*<<8*/)/4096;
			imgDest[i] = (UCHAR) temp;
			imgDest[i] = (UCHAR) temp;

			//imgDest[i] = imgSrc[p];
		}
	}
}

////Copies from the upper left corner to (w,h) from imgZoom to Img
DWORD ZoomCrop(INT width, INT height, INT wZ, INT hZ, UCHAR *imgZoom, UCHAR *img)
{

	DWORD dw = 0;

	if(!img || !imgZoom)
		return FALSE;

	INT i = 0;
	INT iZ = 0;
	INT x = 0;
	INT y = 0;
	INT w = width;
	INT h = height;

	for(y = 0; y < h; y++)
	{

		i = (y*h) + x;
		iZ = (y*hZ) + x;
		for(x = 0; x < w; x++){

			img[i] = imgZoom[iZ];
			i++;
			iZ++;

		}
	}

	return TRUE;
}