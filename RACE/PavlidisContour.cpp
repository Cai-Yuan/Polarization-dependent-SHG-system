#include "stdafx.h"
#include "PavlidisContour.h"

DWORD PavlidisContour(UINT w, UINT h, FLOAT *img, FLOAT *contourMap/*, CONTOURSET *contourSet*/)
{
	UINT nContour = 0;
	INT off = 1;
	INT x, y, i;
	INT max = w*h;
	INT dir = DIR_RIGHT;
	INT dirEnter = dir;
	INT iStart;
	INT iPos;

	if(!img)
		return FALSE;
	if(!contourMap)
		return FALSE;

	memset(contourMap, 0x00, w*h*sizeof(FLOAT));


	for(i = 0; i < max;){

		if(img[i-1] && img[i] && img[i+1]){
			//Inside The Region
			//Keep Moving
			i++;
		}
		else if(!img[i-1] && img[i] && !contourMap[i]){
			//Entering Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, img, contourMap);
			nContour++;
			i++;
		}
		else if(img[i-1] && img[i] && !contourMap[i] && img[i+1]){
			//Leaving Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, img, contourMap);
			nContour++;
			i++;
		}
		else{
			i++;
		}

	}

	return nContour;

}

DWORD PavlidisContour(UINT w, UINT h, INT *img, INT *contourMap)
{
	UINT nContour = 0;
	INT off = 1;
	INT x, y, i;
	INT max = w*h;
	INT dir = DIR_RIGHT;
	INT dirEnter = dir;
	INT iStart;
	INT iPos;

	if(!img)
		return FALSE;
	if(!contourMap)
		return FALSE;

	memset(contourMap, 0x00, w*h*sizeof(FLOAT));


	for(i = 0; i < max;){

		if(img[i-1] && img[i] && img[i+1]){
			//Inside The Region
			//Keep Moving
			i++;
		}
		else if(!img[i-1] && img[i] && !contourMap[i]){
			//Entering Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, img, contourMap);
			nContour++;
			i++;
		}
		else if(img[i-1] && img[i] && !contourMap[i] && img[i+1]){
			//Leaving Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, img, contourMap);
			nContour++;
			i++;
		}
		else{
			i++;
		}

	}

	return nContour;
	//contourSet->nContour = nContour;
}

//Should Terminate on Singleton Values
	//The runner will enter the single white pixel
	//The entering direction is marked(noted)
	//If no other white pixels are found the runner will turn right
	//This will occur until the runner turns 4 times
		//The runner will be at it's starting position and in it's starting direction
		//This event will break out of the while loop
DWORD LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, FLOAT *img, FLOAT *contourMap/*, CONTOUR *contour*/)
{
	POINT pt;
	UINT nPoint = 0;
	UINT r = 0;
	UINT iStart = i;
	UINT dir = DIR_RIGHT;
	UINT nRotations;
	UINT nTurn = 0;
	UINT dirEnter = 0;
	INT p1, p2, p3;

	DWORD loopExit = NULL;

	if(CheckSingleton(w, h, i, img)){
		//contour->nPoint = 0;
		return FALSE;
	}

	pt = GetPOINTFromIndex(w, h, i);
	dir = GetEnterDirection(w, h, i, dir, img);
	dirEnter = dir;

	while(1){
		if(GetP1(w, h, i, dir, img)){
			i = GetP1Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			dir = GetDirection(LEFT, dir);
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn++;
		}
		else if(GetP2(w, h, i, dir, img)){
			i = GetP2Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
		}
		else if(GetP3(w, h, i, dir, img)){
			i = GetP3Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
		}
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
		if(i != iStart)
			break;
		//if(loopExit > MAX_LOOP)
		//	break;
		//loopExit++;
	}

	//loopExit = NULL;
	while((i != iStart)){
		if(nTurn > 4)
		//if(nTurn > 5)
			break;
		pt = GetPOINTFromIndex(w, h, i);
		p1 = GetP1(w, h, i, dir, img);
		p2 = GetP2(w, h, i, dir, img);
		p3 = GetP3(w, h, i, dir, img);
		
		if(p1){
			i = GetP1Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			dir = GetDirection(LEFT, dir);
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn++;

		}
		else if(p2){
			i = GetP2Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn = 0;
		}
		else if(p3){
			i = GetP3Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn = 0;
		}	
		//All Pixels Are Black
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
		//loopExit++;
		//if(loopExit > MAX_LOOP)
		//	break;
	}
	return nPoint;
}

INT GetEnterDirection(UINT w, UINT h, UINT i, UINT dir, FLOAT *img)
{

	for(dir = DIR_UP; dir < DIR_LEFT; dir++){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i-1)){
				if(img[i] && !img[i-1])
					return dir;
			}
			else
				break;
			break;
		case DIR_RIGHT:
			if(Range(w, h, i-w)){
				if(img[i] && !img[i-w])
					return dir;
			}
			else
				break;
		case DIR_DOWN:
			if(Range(w, h, i+1)){
				if(img[i] && !img[i+1])
					return dir;
			}
			else 
				break;
			break;
		case DIR_LEFT:
			if(Range(w, h, i+w)){
				if(img[i] && !img[i+w])
					return dir;
			}
			else
				break;
			break;
		default:
			break;
		}
	}

	return DIR_RIGHT;
}


INT CheckSingleton(UINT w, UINT h, UINT i, FLOAT *img)
{
	UINT UL = i-1-w;
	UINT UP = i-w;
	UINT UR	= i+1-w;
	UINT L = i-1;
	UINT R = i+1;
	UINT LL = i-1+w;
	UINT DN	= i+w;
	UINT LR = i+1+w;

	INT imgUL = 0;
	INT imgUP = 0;
	INT imgUR = 0;
	INT imgL = 0;
	INT imgR = 0;
	INT imgLL = 0;
	INT imgDN = 0;
	INT imgLR = 0;

	if(Range(w, h, UL))
		imgUL = img[UL];
	if(Range(w, h, UP))
		imgUP = img[UP];
	if(Range(w, h, UR))
		imgUR = img[UR];
	if(Range(w, h, L))
		imgL = img[L];
	if(Range(w, h, R))
		imgR = img[R];
	if(Range(w, h, LL))
		imgLL = img[LL];
	if(Range(w, h, DN))
		imgDN = img[DN];
	if(Range(w, h, LR))
		imgLR = img[LR];

	if(!imgUL && !imgUP && !imgUR && !imgL && !imgR && !imgLL && !imgDN && !imgLR)
		return TRUE;
	else
		return FALSE;
}

FLOAT GetP1(UINT w, UINT h, UINT i, UINT dir, FLOAT *in)
{
	if(Range(w, h, i)){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i-1-w))
				return in[i-1-w];
			else
				return 0;
		case DIR_RIGHT:
			if(Range(w, h, i+1-w))
				return in[i+1-w];
			else
				return 0;
		case DIR_DOWN:
			if(Range(w, h, i+1+w))
				return in[i+1+w];
			else
				return 0;
		case DIR_LEFT:
			if(Range(w, h, i-1+w))
				return in[i-1+w];
			else 
				return 0;
		}
	}

	return FALSE;
}

FLOAT GetP2(UINT w, UINT h, UINT i, UINT dir, FLOAT *in)
{
	if(Range(w, h, i)){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i-w))
				return in[i-w];
			else
				return 0;
		case DIR_RIGHT:
			if(Range(w, h, i+1))
				return in[i+1];
			else
				return 0;
		case DIR_DOWN:
			if(Range(w, h, i+w))
				return in[i+w];
			else	
				return 0;
		case DIR_LEFT:
			if(Range(w, h, i-1))
				return in[i-1];
			else
				return 0;
		}
	}

	return FALSE;
}

FLOAT GetP3(UINT w, UINT h, UINT i, UINT dir, FLOAT *in)
{
	if(Range(w, h, i)){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i+1-w))
				return in[i+1-w];
			else
				return 0;
		case DIR_RIGHT:
			if(Range(w, h, i+1+w))
				return in[i+1+w];
			else
				return 0;
		case DIR_DOWN:
			if(Range(w, h, i-1+w))
				return in[i-1+w];
			else
				return 0;
		case DIR_LEFT:
			if(Range(w, h, i-1-w))
				return in[i-1-w];
			else
				return 0;
		}
	}

	return FALSE;
}

FLOAT GetP1(UINT w, UINT h, UINT x, UINT y, UINT dir, FLOAT *in)
{
	UINT i;
	if(Range(w, h, x, y)){
		i = (y*w) + x;
		switch(dir)
		{
		case DIR_UP:
			return in[i-1-w];
		case DIR_RIGHT:
			return in[i+1-w];
		case DIR_DOWN:
			return in[i+w];
		case DIR_LEFT:
			return in[i-1+w];
		}
	}

	return FALSE;
}

FLOAT GetP2(UINT w, UINT h, UINT x, UINT y, UINT dir, FLOAT *in)
{
	UINT i;
	if(Range(w, h, x, y)){
		i = (y*w) + x;
		switch(dir)
		{
		case DIR_UP:
			return in[i-w];
		case DIR_RIGHT:
			return in[i+1];
		case DIR_DOWN:
			return in[i+w];
		case DIR_LEFT:
			return in[i-1];
		}
	}

	return FALSE;
}

FLOAT GetP3(UINT w, UINT h, UINT x, UINT y, UINT dir, FLOAT *in)
{
	UINT i;
	if(Range(w, h, x, y)){
		i = (y*w) + x;
		switch(dir)
		{
		case DIR_UP:
			return in[i+1-w];
		case DIR_RIGHT:
			return in[i+1+w];
		case DIR_DOWN:
			return in[i-1+w];
		case DIR_LEFT:
			return in[i-1-w];
		}
	}

	return FALSE;
}

////Should Terminate on Singleton Values
//	//The runner will enter the single white pixel
//	//The entering direction is marked(noted)
//	//If no other white pixels are found the runner will turn right
//	//This will occur until the runner turns 4 times
//		//The runner will be at it's starting position and in it's starting direction
//		//This event will break out of the while loop
DWORD LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, INT *img, INT *contourMap/*, CONTOUR *contour*/)
{
	POINT pt;
	UINT nPoint = 0;
	UINT r = 0;
	UINT iStart = i;
	UINT dir = DIR_RIGHT;
	UINT nRotations;
	UINT nTurn = 0;
	UINT dirEnter = 0;
	INT p1, p2, p3;

	DWORD loopExit = NULL;

	if(CheckSingleton(w, h, i, img)){
		//contour->nPoint = 0;
		return FALSE;
	}

	pt = GetPOINTFromIndex(w, h, i);
	dir = GetEnterDirection(w, h, i, dir, img);
	dirEnter = dir;

	while(1){
		if(GetP1(w, h, i, dir, img)){
			i = GetP1Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			dir = GetDirection(LEFT, dir);
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn++;
		}
		else if(GetP2(w, h, i, dir, img)){
			i = GetP2Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
		}
		else if(GetP3(w, h, i, dir, img)){
			i = GetP3Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
		}
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
		if(i != iStart)
			break;
		//if(loopExit > MAX_LOOP)
		//	break;
		//loopExit++;
	}

	//loopExit = NULL;
	while((i != iStart)){
		if(nTurn > 5)
			break;
		pt = GetPOINTFromIndex(w, h, i);
		p1 = GetP1(w, h, i, dir, img);
		p2 = GetP2(w, h, i, dir, img);
		p3 = GetP3(w, h, i, dir, img);
		
		if(p1){
			i = GetP1Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			dir = GetDirection(LEFT, dir);
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn++;

		}
		else if(p2){
			i = GetP2Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn = 0;
		}
		else if(p3){
			i = GetP3Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourMap[i] = 255;
			//contourMap[i] = iContour;
			//memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
			nPoint++;
			nTurn = 0;
		}	
		//All Pixels Are Black
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
		//loopExit++;
		//if(loopExit > MAX_LOOP)
		//	break;
	}
	return nPoint;
}

//UINT GetP1Index(UINT w, UINT h, UINT i, UINT dir)
//{
//	switch(dir)
//	{
//	case DIR_UP:
//		return i-1-w;
//	case DIR_RIGHT:
//		return i+1-w;
//	case DIR_DOWN:
//		return i+1+w;
//	case DIR_LEFT:
//		return i-1+w;
//	}
//}
//
//UINT GetP2Index(UINT w, UINT h, UINT i, UINT dir)
//{
//	switch(dir)
//	{
//	case DIR_UP:
//		return i-w;
//	case DIR_RIGHT:
//		return i+1;
//	case DIR_DOWN:
//		return i+w;
//	case DIR_LEFT:
//		return i-1;
//	}
//}
//
//UINT GetP3Index(UINT w, UINT h, UINT i, UINT dir)
//{
//	switch(dir)
//	{
//	case DIR_UP:
//		return i+1-w;
//	case DIR_RIGHT:
//		return i+1+w;
//	case DIR_DOWN:
//		return i-1+w;
//	case DIR_LEFT:
//		return i-1-w;
//	}
//}
//
//UINT GetP1Index(UINT w, UINT h, UINT x, UINT y, UINT dir)
//{
//	UINT i = (y*w) + x;
//	switch(dir)
//	{
//	case DIR_UP:
//		return i-1-w;
//	case DIR_RIGHT:
//		return i+1-w;
//	case DIR_DOWN:
//		return i+w;
//	case DIR_LEFT:
//		return i-1+w;
//	}
//}
//
//UINT GetP2Index(UINT w, UINT h, UINT x, UINT y, UINT dir)
//{
//	UINT i = (y*w) + x;
//	switch(dir)
//	{
//	case DIR_UP:
//		return i-w;
//	case DIR_RIGHT:
//		return i+1;
//	case DIR_DOWN:
//		return i+w;
//	case DIR_LEFT:
//		return i-1;
//	//default:
//	//	return NULL;
//	}
//}


//INT GetDirection(UINT move, INT dir)
//{
//	dir = dir + move;
//	if(dir < 0)
//		dir = 3;
//	if(dir > 3)
//		dir = 0;
//
//	return dir;
//}