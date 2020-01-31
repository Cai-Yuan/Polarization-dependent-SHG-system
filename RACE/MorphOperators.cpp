#include "stdafx.h"

#include "MorphOperators.h"

DWORD Dilate(UINT w, UINT h, INT *img, INT *imgDilate)
{
	UINT off = 1;
	UINT x, y, i;
	UINT xMax = w-off;
	UINT yMax = h-off;

	INT m;
	INT m0;
	INT m1;
	INT m2;
	INT m3;
	INT m4;

	if(!imgDilate){
		return FALSE;
	}

	memset(imgDilate, 0x00, w*h*sizeof(INT));

	for(y = off; y < yMax; y++){
		for(x = off; x < xMax; x++){

			i = (y*w)+x;
			m = max(img[i], img[i+1]);
			m = max(m, img[i-1]);
			m = max(m, img[i+w]);
			m = max(m, img[i-w]);
			//m = max(m, img[i-1+w]);
			//m = max(m, img[i+1+w]);
			//m = max(m, img[i+1-w]);
			//m = max(m, img[i-1-w]);

			//Faster Implementation possible?
			//m = img[i];
			//m0 = max(img[i+1], img[i-1]);
			//m1 = max(img[i+w], img[i-w]);
			//m2 = max(img[i+1+w], img[i-1+w]);
			//m3 = max(img[i+1-w], img[i-1-w]);
			
			//m4 = max(m, m0);
			//m5 = max(m1, m2);

			imgDilate[i] = m;

		}
	}

	return TRUE;
}

//DWORD Dilate(UINT w, UINT h, INT *img, INT *imgDilate)
//{
//	UINT off = 1;
//	UINT x, y, i;
//	UINT xMax = w-off;
//	UINT yMax = h-off;
//
//	INT m;
//	INT m0;
//	INT m1;
//	INT m2;
//	INT m3;
//	INT m4;
//
//	if(!imgDilate){
//		return FALSE;
//	}
//
//	memset(imgDilate, 0x00, w*h*sizeof(INT));
//
//	for(y = off; y < yMax; y++){
//		for(x = off; x < xMax; x++){
//
//			i = (y*w)+x;
//			m = max(img[i], img[i+1]);
//			m = max(m, img[i-1]);
//			m = max(m, img[i+w]);
//			m = max(m, img[i-w]);
//			m = max(m, img[i-1+w]);
//			m = max(m, img[i+1+w]);
//			m = max(m, img[i+1-w]);
//			m = max(m, img[i-1-w]);
//
//			//Faster Implementation possible?
//			//m = img[i];
//			//m0 = max(img[i+1], img[i-1]);
//			//m1 = max(img[i+w], img[i-w]);
//			//m2 = max(img[i+1+w], img[i-1+w]);
//			//m3 = max(img[i+1-w], img[i-1-w]);
//			
//			//m4 = max(m, m0);
//			//m5 = max(m1, m2);
//
//			imgDilate[i] = m;
//
//		}
//	}
//
//	return TRUE;
//}

DWORD Erode(UINT w, UINT h, INT *img, INT *imgErode)
{

	UINT off = 1;
	UINT x, y, i;
	UINT xMax = w-off;
	UINT yMax = h-off;

	INT m;

	if(!imgErode){
		return FALSE;
	}

	memset(imgErode, 0x00, w*h*sizeof(INT));

	for(y = off; y < yMax; y++){
		for(x = off; x < xMax; x++){

			i = (y*w)+x;

			m = min(img[i], img[i+1]);
			m = min(m, img[i-1]);
			m = min(m, img[i+w]);
			m = min(m, img[i-w]);
			m = min(m, img[i-1+w]);
			m = min(m, img[i+1+w]);
			m = min(m, img[i+1-w]);
			m = min(m, img[i-1-w]);
			//imgErode[i] = (max(max(max(max(max(max(max(img[i], img[i+1]), img[i-1]), img[i+w]), img[i-w]), img[i-1+w]), img[i+1+w]), img[i-1-w]), img[i+1-w]);
			//imgErode[i] = min(min(min(min(min(min(min(min(img[i-w], img[i-1-w]), img[i+1-w]), img[i-1+w]), img[i+1+w]), img[i-w]), img[i]), img[i+w]), img[i+w]);
			//imgErode[i] = min(min(min(min(min(imgErode[i], img[i-1]), img[i-1]), img[i]), img[i+1]), img[i+1]);

			imgErode[i] = m;
		}
	}


	return TRUE;
}

DWORD Dilate(UINT w, UINT h, FLOAT *img, FLOAT *imgDilate)
{
	UINT off = 1;
	UINT x, y, i;
	UINT xMax = w-off;
	UINT yMax = h-off;

	if(!imgDilate){
		return FALSE;
	}

	memset(imgDilate, 0x00, w*h*sizeof(FLOAT));

	for(y = off; y < yMax; y++){
		for(x = off; x < xMax; x++){

			i = (y*w)+x;
			////imgDilate[i] = (max(max(max(max(max(max(max(img[i], img[i+1]), img[i-1]), img[i+w]), img[i-w]), img[i-1+w]), img[i+1+w]), img[i-1-w]), img[i+1-w]);
			//imgDilate[i] = max(max(max(max(max(max(max(max(img[i-w], img[i-1-w]), img[i+1-w]), img[i-1+w]), img[i+1+w]), img[i-w]), img[i]), img[i+w]), img[i+w]);
			//imgDilate[i] = max(max(max(max(max(imgDilate[i], img[i-1]), img[i-1]), img[i]), img[i+1]), img[i+1]);
			imgDilate[i] = max(max(max(max(max(max(max(max(img[i], img[i-1]), img[i+1]), img[i-w]), img[i+w]), img[i-w-1]), img[i-w+1]), img[i+w-1]), img[i+w+1]);
		}
	}

	return TRUE;
}

DWORD Erode(UINT w, UINT h, FLOAT *img, FLOAT *imgErode)
{

	UINT off = 1;
	UINT x, y, i;
	UINT xMax = w-off;
	UINT yMax = h-off;

	if(!imgErode){
		return FALSE;
	}

	memset(imgErode, 0x00, w*h*sizeof(FLOAT));

	for(y = off; y < yMax; y++){
		for(x = off; x < xMax; x++){

			i = (y*w)+x;
			////imgErode[i] = (max(max(max(max(max(max(max(img[i], img[i+1]), img[i-1]), img[i+w]), img[i-w]), img[i-1+w]), img[i+1+w]), img[i-1-w]), img[i+1-w]);
			//imgErode[i] = min(min(min(min(min(min(min(min(img[i-w], img[i-1-w]), img[i+1-w]), img[i-1+w]), img[i+1+w]), img[i-w]), img[i]), img[i+w]), img[i+w]);
			//imgErode[i] = min(min(min(min(min(imgErode[i], img[i-1]), img[i-1]), img[i]), img[i+1]), img[i+1]);
			imgErode[i] = min(min(min(min(min(min(min(min(img[i], img[i-1]), img[i+1]), img[i-w]), img[i+w]), img[i-w-1]), img[i-w+1]), img[i+w-1]), img[i+w+1]);
		}
	}


	return TRUE;
}

//Dilates 'num' times
DWORD Dilate(UINT w, UINT h, UINT num, FLOAT *img, FLOAT *dilate)
{

	FLOAT *imgDilate = (FLOAT *)malloc(w*h*sizeof(FLOAT));
	
	INT i = NULL;

	memcpy(imgDilate, img, w*h*sizeof(FLOAT));
	//Dilate(w, h, img, imgDilate);

	for(i = 0; i < num; i++){

		Dilate(w, h, imgDilate, dilate);
		memcpy(imgDilate, dilate, w*h*sizeof(FLOAT));
	}

	if(imgDilate)free(imgDilate);

	return TRUE;
}

//Erodes 'num' times
DWORD Erode(UINT w, UINT h, UINT num, FLOAT *img, FLOAT *erode)
{

	FLOAT *imgErode = (FLOAT *)malloc(w*h*sizeof(FLOAT));
	
	INT i = NULL;

	memcpy(imgErode, img, w*h*sizeof(FLOAT));
	//Erode(w, h, img, imgErode);

	for(i = 0; i < num; i++){

		Erode(w, h, imgErode, erode);
		memcpy(imgErode, erode, w*h*sizeof(FLOAT));
	}

	if(imgErode)free(imgErode);

	return TRUE;
}

			//imgDilate[i] = max(max(max(max(max(max(max(max(img[i], img[i+1]), img[i-1]), img[i+w]), img[i-w]), img[i-1+w]), img[i+1+w]), img[i-1-w]), img[i+1-w]);
			//imgDilate[i] = max(max(max(max(max(max(max(max(img[i-w], img[i-1-w]), img[i+1-w]), img[i-1+w]), img[i+1+w]), img[i-w]), img[i]), img[i+w]), img[i+w]);
			//imgDilate[i] = max(max(max(max(max(imgDilate[i], img[i-1]), img[i-1]), img[i]), img[i+1]), img[i+1]);