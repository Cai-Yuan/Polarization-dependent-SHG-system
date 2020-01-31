#include "stdafx.h"
#include "Point.h"
#include "FloodFillEx.h"


DWORD FloodFillEx(UINT w, UINT h, UCHAR *imgIn, UCHAR *imgOut, FPOINTSET **ptSet)
{
	if(!imgIn || !imgOut)
		return FALSE;

	INT i = 0;		//Stores the ith pixel in the image
	INT r = 0;		//Stores number of regions
	UCHAR v = 50;
	INT max = w*h;

	memcpy(imgOut, imgIn, w*h*sizeof(UCHAR));

	for(i = 0; i < max; i++){
		if(imgOut[i] == 255){
			FloodFillEx(w, h, i, v, imgOut, ptSet[r]);
			v+=5;
			r++;
			if(v > 250)
				v = 50;
		}
	}

	return v;
}

//'region' - stores all the indicies of the flood filled region
DWORD FloodFillEx(UINT w, UINT h, INT seed, UCHAR value, UCHAR *img, FPOINTSET *ptSet)
{
	if(!img/* || !fill*/){
		return FALSE;
	}

	INT p = NULL;
	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT iUpper = NULL;
	INT iLower = NULL;
	INT iLeft = NULL;
	INT iRight = NULL;
	
	INT iSeed = seed;
	UCHAR vOld = img[iSeed];
	UCHAR vNew = value;

	FPOINTSET ptFront = {0};		//Stores the points of the front
	FRONTIER front = {0};		//Stores the indicies of the front

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	if(!Alloc(&ptFront, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	img[iSeed] = value;


	while(front.nFront){
	//while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(img[iUpper] == vOld){
				Push(&front, iUpper);
				//Push(ptSet, GetPointFromIndex(w, h, i));
				img[iUpper] = value;
			}
		}
		if(Range(w, h, iLower)){
			if(img[iLower] == vOld){
				Push(&front, iLower);
				img[iLower] = value;
			}
		}
		if(Range(w, h, iRight)){
			if(img[iRight] == vOld){
				Push(&front, iRight);
				img[iRight] = value;
			}
		}	
		if(Range(w, h, iLeft)){
			if(img[iLeft] == vOld){
				Push(&front, iLeft);
				img[iLeft] = value;
			}
		}

	}


	Free(&front);

	return TRUE;
}