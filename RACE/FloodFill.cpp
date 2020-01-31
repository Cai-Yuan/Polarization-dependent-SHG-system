#include "stdafx.h"

#include "FloodFill.h"
#include "Polygon.h"
#include "Draw.h"
#include "PavlidisContour.h"
#include "Range.h"

DWORD FillPolygon(UINT w, UINT h, UCHAR value, POLYGON *polygon, UCHAR *img)
{

	if(!img || !polygon){
		return FALSE;
	}

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT fill = NULL;
	DWORD flgSeed = NULL;
	POINT seed = {0};

	memset(img, NULL, w*h*sizeof(UCHAR));

	DrawPolygon(w, h, polygon, img, 255);

	for(y = 0; y < h; y++){
		for(x = 0; x < w-1; x++){
			i = y*w+x;

			if(img[i] && !img[i+1]){
				seed.x = x+1;
				seed.y = y;
				flgSeed = TRUE;
			}

			if(flgSeed == TRUE){
				break;
			}
		}
		if(flgSeed == TRUE){
			break;
		}
	}

	FloodFill(w, h, seed, 250, img);


	
	return TRUE;
}

//Referenced 'DoubleThreshold()' for implementaion
DWORD FloodFill(UINT w, UINT h, UCHAR *imgIn, UCHAR *imgOut)
{
	if(!imgIn || !imgOut)
		return FALSE;

	INT i = 0;
	UCHAR v = 100;
	INT max = w*h;

	memcpy(imgOut, imgIn, w*h*sizeof(UCHAR));

	for(i = 0; i < max; i++){
		if(imgOut[i] == 255){
			FloodFill(w, h, i, v, imgOut);
			v++;
			//if(v > 250)
			//	v = 50;
		}
	}

	return v;
}

DWORD FloodFill(UINT w, UINT h, INT *imgIn, INT *imgOut)
{
	if(!imgIn || !imgOut)
		return FALSE;

	INT i = 0;
	INT v = 5;
	INT max = w*h;

	for(i = 0; i < max; i++){
		if(imgIn[i] && !imgOut[i]){
			FloodFill(w, h, i, v, imgIn, imgOut);
			v+=5;
			if(v > 250){
				v = 100;
			}
		}
	}

	return v;
}

DWORD FloodFill(UINT w, UINT h, FLOAT *imgIn, FLOAT *imgOut)
{
	if(!imgIn || !imgOut)
		return FALSE;

	INT i = 0;
	INT v = 5;
	INT max = w*h;

	for(i = 0; i < max; i++){
		if(imgIn[i] && !imgOut[i]){
			FloodFill(w, h, i, v, imgIn, imgOut);
			v+=5;
			if(v > 250){
				v = 100;
			}
		}
	}

	return v;
}

DWORD FloodFill(UINT w, UINT h, POINT seed, UCHAR value, UCHAR *img)
{
	if(!img/* || !fill*/){
		return FALSE;
	}


	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT iUpper = NULL;
	INT iLower = NULL;
	INT iLeft = NULL;
	INT iRight = NULL;
	
	INT iSeed = seed.y*w + seed.x;
	UCHAR vOld = img[iSeed];
	UCHAR vNew = value;

	//memset(fill, NULL, w*h*sizeof(UCHAR));

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	img[iSeed] = value;


	while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(img[iUpper] == vOld){
				Push(&front, iUpper);
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

DWORD FloodFill(UINT w, UINT h, INT seed, UCHAR value, UCHAR *img)
{
	if(!img/* || !fill*/){
		return FALSE;
	}


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

	//memset(fill, NULL, w*h*sizeof(UCHAR));

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
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



DWORD FloodFill(UINT w, UINT h, POINT seed, INT value, INT *img)
{
	if(!img/* || !fill*/){
		return FALSE;
	}


	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT iUpper = NULL;
	INT iLower = NULL;
	INT iLeft = NULL;
	INT iRight = NULL;
	
	INT iSeed = seed.y*w + seed.x;
	UCHAR vOld = img[iSeed];
	UCHAR vNew = value;

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	img[iSeed] = value;


	while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(img[iUpper] == vOld){
				Push(&front, iUpper);
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

DWORD FloodFill(UINT w, UINT h, INT seed, INT value, INT *img)
{
	if(!img/* || !fill*/){
		return FALSE;
	}


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

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	img[iSeed] = value;

	//Possible Remove?
	//if(vOld == value){
	//	value = vOld - 1;
	//}


	while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(img[iUpper] == vOld && img[iUpper] != value){
				Push(&front, iUpper);
				img[iUpper] = value;
			}
		}
		if(Range(w, h, iLower)){
			if(img[iLower] == vOld && img[iLower] != value){
				Push(&front, iLower);
				img[iLower] = value;
			}
		}
		if(Range(w, h, iRight)){
			if(img[iRight] == vOld && img[iRight] != value){
				Push(&front, iRight);
				img[iRight] = value;
			}
		}	
		if(Range(w, h, iLeft)){
			if(img[iLeft] == vOld && img[iLeft] != value){
				Push(&front, iLeft);
				img[iLeft] = value;
			}
		}

	}


	Free(&front);
	return TRUE;
}

//Untested
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, UCHAR *img, UCHAR *out)
{
	if(!img || !out){
		return FALSE;
	}


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

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	img[iSeed] = value;


	while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(img[iUpper] == vOld && out[iUpper] != value){
				Push(&front, iUpper);
				out[iUpper] = value;
			}
		}
		if(Range(w, h, iLower)){
			if(img[iLower] == vOld && out[iLower] != value){
				Push(&front, iLower);
				out[iLower] = value;
			}
		}
		if(Range(w, h, iRight)){
			if(img[iRight] == vOld && out[iRight] != value){
				Push(&front, iRight);
				out[iRight] = value;
			}
		}	
		if(Range(w, h, iLeft)){
			if(img[iLeft] == vOld && out[iLeft] != value){
				Push(&front, iLeft);
				out[iLeft] = value;
			}
		}

	}


	Free(&front);
	return TRUE;

}

//Untested
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, INT *img, INT *out)
{
	if(!img || !out){
		return FALSE;
	}


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

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	img[iSeed] = value;


	while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(img[iUpper] == vOld && out[iUpper] != value){
				Push(&front, iUpper);
				out[iUpper] = value;
			}
		}
		if(Range(w, h, iLower)){
			if(img[iLower] == vOld && out[iLower] != value){
				Push(&front, iLower);
				out[iLower] = value;
			}
		}
		if(Range(w, h, iRight)){
			if(img[iRight] == vOld && out[iRight] != value){
				Push(&front, iRight);
				out[iRight] = value;
			}
		}	
		if(Range(w, h, iLeft)){
			if(img[iLeft] == vOld && out[iLeft] != value){
				Push(&front, iLeft);
				out[iLeft] = value;
			}
		}

	}


	Free(&front);
	return TRUE;

}

//2.20.15
//UNTESTED
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, FLOAT *in, FLOAT *out)
{
	if(!in || !out){
		return FALSE;
	}


	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT iUpper = NULL;
	INT iLower = NULL;
	INT iLeft = NULL;
	INT iRight = NULL;
	
	INT iSeed = seed;
	UCHAR vOld = in[iSeed];
	UCHAR vNew = value;

	FRONTIER front = {0};

	if(!Alloc(&front, w*h)){
		return FALSE;
	}

	//Put Seed Point on Frontier
	Push(&front, iSeed);
	in[iSeed] = value;

	while(!IsEmpty(front)){
		i = Pop(&front);
		iUpper = i-w;
		iLower = i+w;
		iRight = i+1;
		iLeft = i-1;
		
		if(Range(w, h, iUpper)){
			if(in[iUpper] == vOld && out[iUpper] != value){
				Push(&front, iUpper);
				out[iUpper] = value;
			}
		}
		if(Range(w, h, iLower)){
			if(in[iLower] == vOld && out[iLower] != value){
				Push(&front, iLower);
				out[iLower] = value;
			}
		}
		if(Range(w, h, iRight)){
			if(in[iRight] == vOld && out[iRight] != value){
				Push(&front, iRight);
				out[iRight] = value;
			}
		}	
		if(Range(w, h, iLeft)){
			if(in[iLeft] == vOld && out[iLeft] != value){
				Push(&front, iLeft);
				out[iLeft] = value;
			}
		}

	}


	Free(&front);
	return TRUE;

}

DWORD Alloc(FRONTIER *frontier, DWORD size)
{

	frontier->front = (UINT *)malloc(size*sizeof(UINT));

	if(frontier->front){
		frontier->size = size;
		frontier->nFront = NULL;
		return TRUE;
	}else{
		frontier->size = NULL;
		frontier->nFront = NULL;
		return FALSE;
	}

}

DWORD Push(FRONTIER *frontier, UINT index)
{
	if(!frontier){
		return FALSE;
	}

	if(frontier->nFront > (frontier->size-1)){
		return FALSE;
	}

	frontier->front[frontier->nFront] = index;
	frontier->nFront++;

	return TRUE;

}

UINT Pop(FRONTIER *frontier)
{
	//DWORD i = frontier->nFront-1;
	if(frontier->nFront){
		frontier->nFront--;
		return (UINT) frontier->front[frontier->nFront];
	}

	return FALSE;
}

DWORD IsEmpty(FRONTIER frontier)
{
	if(frontier.nFront){
		return FALSE;
	}else{
		return TRUE;
	}
}

DWORD Free(FRONTIER *frontier)
{
	if(!frontier){
		return FALSE;
	}
	if(frontier->front)free(frontier->front);

	frontier->size = NULL;
	frontier->nFront = NULL;

	return TRUE;
}