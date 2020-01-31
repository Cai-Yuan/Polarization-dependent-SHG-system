#include "stdafx.h"

#include "TheoPavlidis.h"
//#include "Contour.h"
//#include "Range.h"


VOID TheoPavlidisContour(UINT w, UINT h, INT *imgINT, CONTOURSET *contourSet)
{
	UINT nContour = 0;
	INT off = 1;
	INT x, y, i;
	INT max = w*h;
	INT dir = DIR_RIGHT;
	INT dirEnter = dir;
	INT iStart;
	INT iPos;

	if(!imgINT || !contourSet)
		return;

	INT *contourINT = NULL;
	contourINT = (INT *) calloc(w*h, sizeof(INT));

	//memset(contourINT, 0x00, w*h*sizeof(INT));


	for(i = 0; i < max;){

		if(imgINT[i-1] && imgINT[i] && imgINT[i+1]){
			//Inside The Region
			//Keep Moving
			i++;
		}
		else if(!imgINT[i-1] && imgINT[i] && imgINT[i+1] && !contourINT[i]){
			//Entering Non-Contoured Region
			//Contour it
			if(nContour < contourSet->size){
				LabelContourTrace(w, h, i, nContour, imgINT, contourINT, &contourSet->contour[nContour]);
				nContour++;
			}
			i++;
		}
		else if(imgINT[i-1] && imgINT[i] && !contourINT[i] && imgINT[i+1]){
			//Leaving Non-Contoured Region
			//Contour it
			if(nContour < contourSet->size){
				LabelContourTrace(w, h, i, nContour, imgINT, contourINT, &contourSet->contour[nContour]);
				nContour++;
			}
			i++;
		}
		else{
			i++;
		}

	}

	contourSet->nContour = nContour;
	if(contourINT) free(contourINT);
}

VOID TheoPavlidisContour(UINT w, UINT h, INT *imgINT, INT *contourINT, CONTOURSET *contourSet)
{
	UINT nContour = 0;
	INT off = 1;
	INT x, y, i;
	INT max = w*h;
	INT dir = DIR_RIGHT;
	INT dirEnter = dir;
	INT iStart;
	INT iPos;

	if(!imgINT || !contourINT || !contourSet)
		return;

	memset(contourINT, 0x00, w*h*sizeof(INT));


	for(i = 0; i < max;){

		if(imgINT[i-1] && imgINT[i] && imgINT[i+1]){
			//Inside The Region
			//Keep Moving
			i++;
		}
		else if(!imgINT[i-1] && imgINT[i] && !contourINT[i]){
			//Entering Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, imgINT, contourINT, &contourSet->contour[nContour]);
			nContour++;
			i++;
		}
		else if(imgINT[i-1] && imgINT[i] && !contourINT[i] && imgINT[i+1]){
			//Leaving Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, imgINT, contourINT, &contourSet->contour[nContour]);
			nContour++;
			i++;
		}
		else{
			i++;
		}

	}

	contourSet->nContour = nContour;
}

VOID TheoPavlidisContour(UINT w, UINT h, INT *imgINT, POINT_SET *mask, INT *contourINT, CONTOURSET *contourSet)
{
	UINT nContour = 0;
	INT off = 1;
	INT x, y, i, p;
	INT max = w*h;
	INT dir = DIR_RIGHT;
	INT dirEnter = dir;
	INT iStart;
	INT iPos;

	if(!mask || !contourINT || !contourSet)
		return;

	memset(contourINT, 0x00, w*h*sizeof(INT));


	for(p = 0; p < mask->nPOINTs; p++){
	//for(i = 0; i < max;){
		i = mask->point[p].y*w + mask->point[p].x;
		if(imgINT[i-1] && imgINT[i] && imgINT[i+1]){
			//Inside The Region
			//Keep Moving
			//i++;
		}
		else if(!imgINT[i-1] && imgINT[i] && !contourINT[i]){
			//Entering Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, imgINT, contourINT, &contourSet->contour[nContour]);
			nContour++;
			//i++;
		}
		else if(imgINT[i-1] && imgINT[i] && !contourINT[i] && imgINT[i+1]){
			//Leaving Non-Contoured Region
			//Contour it
			LabelContourTrace(w, h, i, nContour, imgINT, contourINT, &contourSet->contour[nContour]);
			nContour++;
			//i++;
		}
		else{
			//i++;
		}

	}

	contourSet->nContour = nContour;
}

//Should Terminate on Singleton Values
	//The runner will enter the single white pixel
	//The entering direction is marked(noted)
	//If no other white pixels are found the runner will turn right
	//This will occur until the runner turns 4 times
		//The runner will be at it's starting position and in it's starting direction
		//This event will break out of the while loop
VOID LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, INT *imgINT, INT *contourINT, CONTOUR *contour)
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

	if(!imgINT || !contourINT || !contour)
		return;

	if(CheckSingleton(w, h, i, imgINT)){
		contour->nPoint = 0;
		return;
	}

	pt = GetPOINTFromIndex(w, h, i);
	dir = GetEnterDirection(w, h, i, dir, imgINT);
	dirEnter = dir;

	while(1){
		if(GetP1(w, h, i, dir, imgINT)){
			i = GetP1Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourINT[i] = iContour;
			dir = GetDirection(LEFT, dir);
			if(nPoint < contour->size){
				memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
				nPoint++;
			}
			nTurn++;
		}
		else if(GetP2(w, h, i, dir, imgINT)){
			i = GetP2Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourINT[i] = iContour;
			if(nPoint < contour->size){
				memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
				nPoint++;
			}
		}
		else if(GetP3(w, h, i, dir, imgINT)){
			i = GetP3Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourINT[i] = iContour;
			if(nPoint < contour->size){
				memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
				nPoint++;
			}
		}
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
		if(i != iStart)
			break;
		if(loopExit > MAX_LOOP)
			break;
		loopExit++;
	}

	loopExit = NULL;
	while((i != iStart)){
		if(nTurn > 4)
		//if(nTurn > 5)
			break;
		pt = GetPOINTFromIndex(w, h, i);
		p1 = GetP1(w, h, i, dir, imgINT);
		p2 = GetP2(w, h, i, dir, imgINT);
		p3 = GetP3(w, h, i, dir, imgINT);
		
		if(p1){
			i = GetP1Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourINT[i] = iContour;
			dir = GetDirection(LEFT, dir);
			if(nPoint < contour->size){
				memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
				nPoint++;
			}
			nTurn++;

		}
		else if(p2){
			i = GetP2Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourINT[i] = iContour;
			if(nPoint < contour->size){
				memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
				nPoint++;
			}
			nTurn = 0;
		}
		else if(p3){
			i = GetP3Index(w, h, i, dir);
			pt = GetPOINTFromIndex(w, h, i);
			contourINT[i] = iContour;
			if(nPoint < contour->size){
				memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
				nPoint++;
			}
			nTurn = 0;
		}	
		//All Pixels Are Black
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
		loopExit++;
		if(loopExit > MAX_LOOP)
			break;
	}
	contour->nPoint = nPoint;
}

//VOID LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, INT *imgINT, INT *contourINT, CONTOUR *contour)
//{
//	POINT pt;
//	UINT nPoint = 0;
//	UINT r = 0;
//	UINT iStart = i;
//	UINT dir = DIR_RIGHT;
//	UINT nRotations;
//	UINT nTurn = 0;
//	UINT dirEnter = 0;
//
//	if(CheckSingleton(w, h, i, imgINT)){
//		contour->nPoint = 0;
//		return;
//	}
//
//	pt = GetPOINTFromIndex(w, h, i);
//	dir = GetEnterDirection(w, h, i, dir, imgINT);
//	dirEnter = dir;
//
//	if(GetP1(w, h, i, dir, imgINT)){
//		i = GetP1Index(w, h, i, dir);
//		contourINT[i] = iContour;
//		dir = GetDirection(LEFT, dir);
//		memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
//		nPoint++;
//	}
//	else if(GetP2(w, h, i, dir, imgINT)){
//		i = GetP2Index(w, h, i, dir);
//		contourINT[i] = iContour;
//		memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
//		nPoint++;
//	}
//	else if(GetP3(w, h, i, dir, imgINT)){
//		i = GetP3Index(w, h, i, dir);
//		contourINT[i] = iContour;
//		memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
//		nPoint++;
//	}
//	else{
//		dir = GetDirection(RIGHT, dir);
//		
//	}
//
//	while((i != iStart || dir != dirEnter)){
//		if(nTurn > 100)
//			break;
//		pt = GetPOINTFromIndex(w, h, i);
//		if(GetP1(w, h, i, dir, imgINT)){
//			i = GetP1Index(w, h, i, dir);
//			contourINT[i] = iContour;
//			dir = GetDirection(LEFT, dir);
//			memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
//			nPoint++;
//			nTurn++;
//
//		}
//		else if(GetP2(w, h, i, dir, imgINT)){
//			i = GetP2Index(w, h, i, dir);
//			contourINT[i] = iContour;
//			memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
//			nPoint++;
//		}
//		else if(GetP3(w, h, i, dir, imgINT)){
//			i = GetP3Index(w, h, i, dir);
//			contourINT[i] = iContour;
//			memcpy(&contour->point[nPoint], &pt, sizeof(POINT));
//			nPoint++;
//		}	
//		//All Pixels Are Black
//		else{
//			dir = GetDirection(RIGHT, dir);
//			
//		}
//	}
//	contour->nPoint = nPoint;
//}


VOID TheoPavlidisContourTrace(UINT w, UINT h, INT *imgINT, INT *contourINT)
{
	UINT iContour = 100;
	INT off = 1;
	INT x, y, i;
	INT max = w*h;
	INT dir = DIR_RIGHT;
	INT dirEnter = dir;
	INT iStart;
	INT iPos;

	memset(contourINT, 0x00, w*h*sizeof(INT));

	for(i = 0; i < max;/* i++*/){
		if(imgINT[i]){

			if(!contourINT[i]){

				if(GetSingleton(w, h, i, dirEnter, imgINT)){
					i++;
				}
				else{
					if(i > w*h)
						i = i;
					LabelContourTrace(w, h, i,/* i, */iContour, dirEnter, imgINT, contourINT);
					iContour+=1;
					i++;
				}
			}


			//Cut Through White Region
				//Increment the pointer by one each time
				//Stop when you hit a black pixel
			else{
				i++;
				while((imgINT[i]/*&&!contourINT[i]*/) && i < max){
					i++;
				}
			}


		}
		else
			i++;
	}
}

//Should Terminate on Singleton Values
	//The runner will enter the single white pixel
	//The entering direction is marked
	//If no other white pixels are found the runner will turn right
	//This will occur until the runner turns 4 times
		//The runner will be at it's starting position and in it's starting direction
		//This event will break out of the while loop


VOID LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, UINT dirEnter, INT *imgINT, INT *contourINT)
{
	UINT r = 0;
	UINT iStart = i;
	UINT dir = dirEnter;
	UINT nRotations;
	UINT nTurn = 0;

	if(i > w*h)
		i = i;

	dir = GetEnterDirection(w, h, i, dir, imgINT);
	dirEnter = dir;

	if(GetP1(w, h, i, dir, imgINT)){
		i = GetP1Index(w, h, i, dir);
		/*iPos = GetP1Index(w, h, i, dir);*/
		contourINT[i] = iContour;
		dir = GetDirection(LEFT, dir);
	}
	else if(GetP2(w, h, i, dir, imgINT)){
		i = GetP2Index(w, h, i, dir);
		/*iPos = GetP2Index(w, h, i, dir);*/
		contourINT[i] = iContour;
	}
	else if(GetP3(w, h, i, dir, imgINT)){
		i = GetP3Index(w, h, i, dir);
		/*iPos = GetP3Index(w, h, i, dir);*/
		contourINT[i] = iContour;
	}
	else{
		//if(CheckSingleton(w, h, i, imgINT))
			dir = GetDirection(RIGHT, dir);
		//if(imgINT[i] && !imgINT[i-1-w] && !imgINT[i-w] && !imgINT[i+1-w] && !imgINT[i-1] && !imgINT[i+1] && !imgINT[i-1+w] && !imgINT[i+w] && !imgINT[i+1+w])
			
	}

	while((i != iStart || dir != dirEnter)){
		if(nTurn > 1000)
			break;
		//dir = GetDirection(LEFT, dir);
		if(GetP1(w, h, i, dir, imgINT)){
			i = GetP1Index(w, h, i, dir);
			contourINT[i] = iContour;
			dir = GetDirection(LEFT, dir);
			nTurn++;
		}
		else if(GetP2(w, h, i, dir, imgINT)){
			i = GetP2Index(w, h, i, dir);
			contourINT[i] = iContour;
		}
		else if(GetP3(w, h, i, dir, imgINT)){
			i = GetP3Index(w, h, i, dir);
			contourINT[i] = iContour;
		}	
		//All Pixels Are Black
		else{
			dir = GetDirection(RIGHT, dir);
			
		}
	}
}
//VOID LabelContourTrace(UINT w, UINT h, UINT iStart, UINT iPos, UINT iContour, UINT dirEnter, INT *imgINT, INT *contourINT)
//{
//	UINT r = 0;
//	UINT i = iStart;
//	UINT dir = dirEnter;
//	UINT pos = iPos;
//	UINT nRotations;
//	UINT nTurn = 0;
//
//	if(i > w*h)
//		i = i;
//
//	dir = GetEnterDirection(w, h, i, dir, imgINT);
//	dirEnter = dir;
//
//	if(GetP1(w, h, i, dir, imgINT)){
//		i = GetP1Index(w, h, i, dir);
//		/*iPos = GetP1Index(w, h, i, dir);*/
//		contourINT[i] = iContour;
//		dir = GetDirection(LEFT, dir);
//	}
//	else if(GetP2(w, h, i, dir, imgINT)){
//		i = GetP2Index(w, h, i, dir);
//		/*iPos = GetP2Index(w, h, i, dir);*/
//		contourINT[i] = iContour;
//	}
//	else if(GetP3(w, h, i, dir, imgINT)){
//		i = GetP3Index(w, h, i, dir);
//		/*iPos = GetP3Index(w, h, i, dir);*/
//		contourINT[i] = iContour;
//	}
//	else{
//		//if(CheckSingleton(w, h, i, imgINT))
//			dir = GetDirection(RIGHT, dir);
//		//if(imgINT[i] && !imgINT[i-1-w] && !imgINT[i-w] && !imgINT[i+1-w] && !imgINT[i-1] && !imgINT[i+1] && !imgINT[i-1+w] && !imgINT[i+w] && !imgINT[i+1+w])
//			
//	}
//
//	while((i != iStart || dir != dirEnter)){
//		if(nTurn > 1000)
//			break;
//		//dir = GetDirection(LEFT, dir);
//		if(GetP1(w, h, i, dir, imgINT)){
//			i = GetP1Index(w, h, i, dir);
//			contourINT[i] = iContour;
//			dir = GetDirection(LEFT, dir);
//			nTurn++;
//		}
//		else if(GetP2(w, h, i, dir, imgINT)){
//			i = GetP2Index(w, h, i, dir);
//			contourINT[i] = iContour;
//		}
//		else if(GetP3(w, h, i, dir, imgINT)){
//			i = GetP3Index(w, h, i, dir);
//			contourINT[i] = iContour;
//		}	
//		//All Pixels Are Black
//		else{
//			dir = GetDirection(RIGHT, dir);
//			
//		}
//	}
//}

INT GetEnterDirection(UINT w, UINT h, UINT i, UINT dir, INT *imgINT)
{

	for(dir = DIR_UP; dir < DIR_LEFT; dir++){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i-1)){
				if(imgINT[i] && !imgINT[i-1])
					return dir;
			}
			else
				break;
			break;
		case DIR_RIGHT:
			if(Range(w, h, i-w)){
				if(imgINT[i] && !imgINT[i-w])
					return dir;
			}
			else
				break;
		case DIR_DOWN:
			if(Range(w, h, i+1)){
				if(imgINT[i] && !imgINT[i+1])
					return dir;
			}
			else 
				break;
			break;
		case DIR_LEFT:
			if(Range(w, h, i+w)){
				if(imgINT[i] && !imgINT[i+w])
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




////May Remove Single
//INT GetEnterDirection(UINT w, UINT h, UINT i, UINT dir, INT *imgINT)
//{
//	//UINT nTurns;
//	//if(GetP1(w, h, iPos, dirEnter, imgINT))
//	//	return	GetDirection(LEFT, dirEnter);
//
//	if(i == 5139)
//		i = i;
//
//	for(dir = DIR_UP; dir < DIR_LEFT; dir++){
//		if(!GetP1(w, h, i, dir, imgINT) && (GetP2(w, h, i, dir, imgINT) || GetP3(w, h, i, dir, imgINT))){
//			return dir;
//		}
//	}
//	//for(dir = DIR_UP; dir < DIR_LEFT; dir++){
//	//	if(!GetP1(w, h, i, dir, imgINT)){
//	//		if(GetP2(w, h, i, dir, imgINT))
//	//			return dir;
//	//		if(GetP3(w, h, i, dir, imgINT))
//	//			return dir;
//	//	}
//	//}
//	//MessageBox(NULL, "Could Not Determine Direction", "Contour Error!", MB_OK);
//	return DIR_RIGHT;
//
//
//}

//May Remove Single
INT GetSingleton(UINT w, UINT h, UINT i, UINT dir, INT *imgINT)
{
	UINT dirEnter = dir;
	UINT nTurns;
	UINT p1, p2, p3;

	//Loop through all possible positions
	for(dir = DIR_UP; dir < DIR_LEFT; dir++){

		p1 = GetP1(w, h, i, dir, imgINT);
		p2 = GetP2(w, h, i, dir, imgINT);
		p3 = GetP3(w, h, i, dir, imgINT);
		if(!p1 && !p2 && !p3){
			p1 = p1;
		}
		else			
			return	FALSE;

		//if(	!GetP1(w, h, i, dir, imgINT) && 
		//	!GetP2(w, h, i, dir, imgINT) && 
		//	!GetP3(w, h, i, dir, imgINT)){
		//}
		//else			
		//	return	FALSE;
	}
	return TRUE;


}

//VOID TheoPavlidisContourTrace(UINT w, UINT h, INT *imgINT, INT *contourINT)
//{
//	UINT off = 1;
//	UINT x, y, i;
//	UINT xMax = w-off;
//	UINT yMax = h-off;
//	UINT dir = DIR_RIGHT;
//	UINT iStart;
//	UINT iPos;
//
//	memset(contourINT, 0x00, w*h*sizeof(INT));
//
//	for(y = off; y < yMax; y++){
//		for(x = off; x < xMax; x++){
//
//			i = (y*w) + x;
//			
//			if(imgINT[i] && !contourINT[i]){
//				iStart = i;
//				contourINT[i] = 255;
//
//				if(GetP1(w, h, x, y, dir, imgINT)){
//					iPos = GetP1Index(w, h, x, y, dir);
//					while(iPos != iStart){
//						
//						//contourINT[i]
//					}
//				}
//			}
//		}
//	}
//
//}

INT GetP1(UINT w, UINT h, UINT i, UINT dir, INT *inINT)
{
	if(Range(w, h, i)){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i-1-w))
				return inINT[i-1-w];
			else
				return 0;
		case DIR_RIGHT:
			if(Range(w, h, i+1-w))
				return inINT[i+1-w];
			else
				return 0;
		case DIR_DOWN:
			if(Range(w, h, i+1+w))
				return inINT[i+1+w];
			else
				return 0;
		case DIR_LEFT:
			if(Range(w, h, i-1+w))
				return inINT[i-1+w];
			else 
				return 0;
		}
	}

	return FALSE;
}

INT GetP2(UINT w, UINT h, UINT i, UINT dir, INT *inINT)
{
	if(Range(w, h, i)){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i-w))
				return inINT[i-w];
			else
				return 0;
		case DIR_RIGHT:
			if(Range(w, h, i+1))
				return inINT[i+1];
			else
				return 0;
		case DIR_DOWN:
			if(Range(w, h, i+w))
				return inINT[i+w];
			else	
				return 0;
		case DIR_LEFT:
			if(Range(w, h, i-1))
				return inINT[i-1];
			else
				return 0;
		}
	}

	return FALSE;
}

INT GetP3(UINT w, UINT h, UINT i, UINT dir, INT *inINT)
{
	if(Range(w, h, i)){
		switch(dir)
		{
		case DIR_UP:
			if(Range(w, h, i+1-w))
				return inINT[i+1-w];
			else
				return 0;
		case DIR_RIGHT:
			if(Range(w, h, i+1+w))
				return inINT[i+1+w];
			else
				return 0;
		case DIR_DOWN:
			if(Range(w, h, i-1+w))
				return inINT[i-1+w];
			else
				return 0;
		case DIR_LEFT:
			if(Range(w, h, i-1-w))
				return inINT[i-1-w];
			else
				return 0;
		}
	}

	return FALSE;
}

INT GetP1(UINT w, UINT h, UINT x, UINT y, UINT dir, INT *inINT)
{
	UINT i;
	if(Range(w, h, x, y)){
		i = (y*w) + x;
		switch(dir)
		{
		case DIR_UP:
			return inINT[i-1-w];
		case DIR_RIGHT:
			return inINT[i+1-w];
		case DIR_DOWN:
			return inINT[i+w];
		case DIR_LEFT:
			return inINT[i-1+w];
		}
	}

	return FALSE;
}

INT GetP2(UINT w, UINT h, UINT x, UINT y, UINT dir, INT *inINT)
{
	UINT i;
	if(Range(w, h, x, y)){
		i = (y*w) + x;
		switch(dir)
		{
		case DIR_UP:
			return inINT[i-w];
		case DIR_RIGHT:
			return inINT[i+1];
		case DIR_DOWN:
			return inINT[i+w];
		case DIR_LEFT:
			return inINT[i-1];
		}
	}

	return FALSE;
}

INT GetP3(UINT w, UINT h, UINT x, UINT y, UINT dir, INT *inINT)
{
	UINT i;
	if(Range(w, h, x, y)){
		i = (y*w) + x;
		switch(dir)
		{
		case DIR_UP:
			return inINT[i+1-w];
		case DIR_RIGHT:
			return inINT[i+1+w];
		case DIR_DOWN:
			return inINT[i-1+w];
		case DIR_LEFT:
			return inINT[i-1-w];
		}
	}

	return FALSE;
}

UINT GetP1Index(UINT w, UINT h, UINT i, UINT dir)
{
	switch(dir)
	{
	case DIR_UP:
		return i-1-w;
	case DIR_RIGHT:
		return i+1-w;
	case DIR_DOWN:
		return i+1+w;
	case DIR_LEFT:
		return i-1+w;
	}

	return FALSE;
}

UINT GetP2Index(UINT w, UINT h, UINT i, UINT dir)
{
	switch(dir)
	{
	case DIR_UP:
		return i-w;
	case DIR_RIGHT:
		return i+1;
	case DIR_DOWN:
		return i+w;
	case DIR_LEFT:
		return i-1;
	}

	return FALSE;
}

UINT GetP3Index(UINT w, UINT h, UINT i, UINT dir)
{
	switch(dir)
	{
	case DIR_UP:
		return i+1-w;
	case DIR_RIGHT:
		return i+1+w;
	case DIR_DOWN:
		return i-1+w;
	case DIR_LEFT:
		return i-1-w;
	}

	return FALSE;
}

UINT GetP1Index(UINT w, UINT h, UINT x, UINT y, UINT dir)
{
	UINT i = (y*w) + x;
	switch(dir)
	{
	case DIR_UP:
		return i-1-w;
	case DIR_RIGHT:
		return i+1-w;
	case DIR_DOWN:
		return i+w;
	case DIR_LEFT:
		return i-1+w;
	}

	return FALSE;
}

UINT GetP2Index(UINT w, UINT h, UINT x, UINT y, UINT dir)
{
	UINT i = (y*w) + x;
	switch(dir)
	{
	case DIR_UP:
		return i-w;
	case DIR_RIGHT:
		return i+1;
	case DIR_DOWN:
		return i+w;
	case DIR_LEFT:
		return i-1;
	//default:
	//	return NULL;
	}

	return FALSE;
}

UINT GetP3(UINT w, UINT h, UINT x, UINT y, UINT dir)
{
	UINT i = (y*w) + x;
	switch(dir)
	{
	case DIR_UP:
		return i+1-w;
	case DIR_RIGHT:
		return i+1+w;
	case DIR_DOWN:
		return i-1+w;
	case DIR_LEFT:
		return i-1-w;
	//default:
	//	return NULL;
	}

	return FALSE;
}

INT GetDirection(UINT move, INT dir)
{
	dir = dir + move;
	if(dir < 0)
		dir = 3;
	if(dir > 3)
		dir = 0;

	return dir;
}

VOID ContourTraceTest()
{
	UINT off;
	UINT w = 11;
	UINT h = 13;

	UINT x, y, i;
	
	INT *contourINT = (INT *)malloc(w*h*sizeof(INT));

	INT img[] = {	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
					0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
					0,		0,		0,		0,		255,	255,	0,		0,		0,		0,		0,
					0,		0,		0,		255,	0,		0,		255,	255,	0,		0,		0,
					0,		0,		255,	0,		0,		0,		255,	0,		0,		0,		0,
					0,		0,		255,	255,	0,		255,	0,		0,		0,		0,		0,
					0,		0,		255,	0,		0,		0,		255,	0,		0,		0,		0,
					0,		0,		255,	255,	255,	0,		0,		255,	0,		0,		0,
					0,		0,		0,		0,		0,		255,	255,	0,		0,		0,		0,
					0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
					0,		0,		0,		255,	0,		0,		0,		0,		0,		0,		0,
					0,		0,		0,		255,	0,		0,		0,		0,		0,		0,		0,
					0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0};

	//for(y = off; y < h-off; y++){
	//	for(x = off; x < w-off; x++){
	//		i = (y*w)+x;

	TheoPavlidisContourTrace(w, h, img, contourINT);

}

INT CheckSingleton(UINT w, UINT h, UINT i, INT *imgINT)
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
		imgUL = imgINT[UL];
	if(Range(w, h, UP))
		imgUP = imgINT[UP];
	if(Range(w, h, UR))
		imgUR = imgINT[UR];
	if(Range(w, h, L))
		imgL = imgINT[L];
	if(Range(w, h, R))
		imgR = imgINT[R];
	if(Range(w, h, LL))
		imgLL = imgINT[LL];
	if(Range(w, h, DN))
		imgDN = imgINT[DN];
	if(Range(w, h, LR))
		imgLR = imgINT[LR];

	if(!imgUL && !imgUP && !imgUR && !imgL && !imgR && !imgLL && !imgDN && !imgLR)
		return TRUE;
	else
		return FALSE;
}
