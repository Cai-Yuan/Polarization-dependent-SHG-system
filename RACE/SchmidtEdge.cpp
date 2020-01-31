#include "stdafx.h"

#include "SchmidtEdge.h"


//POINT_SET		Point.h
VOID SchmidtEdgeGradientPlow(UINT w, UINT h, INT thresh, UINT fSize, UCHAR *img, INT *dxdyThresh, POINT_SET *psPoint)
{

	INT *dx = NULL;
	INT *dy = NULL;
	INT *dxAbs = NULL;
	INT *dyAbs = NULL;
	INT *dxdyAbs = NULL;
	INT *xytFiltered = NULL;
	UCHAR *imgLog = NULL;

	UINT maxRange = 255;
	UINT row, col, i;
	UINT max = w * h;
	UCHAR midPlane;

	dx = (INT *)malloc(w * h * sizeof(INT));
	dy = (INT *)malloc(w * h * sizeof(INT));
	dxAbs = (INT *)malloc(w*h*sizeof(INT));
	dyAbs = (INT *)malloc(w*h*sizeof(INT));
	dxdyAbs = (INT *)malloc(w*h*sizeof(INT));
	xytFiltered = (INT *)malloc(w*h*sizeof(INT));
	imgLog = (UCHAR *)malloc(w*h*sizeof(UCHAR));

	memset(dxdyThresh, 0x00, w*h*sizeof(INT));

	//logOperator(w, h, img, imgLog);

	//GradientPlowImageAbsThresh(w, h, thresh, imgLog, dxdyThresh);			//SchmidtEdge.cpp
	////SecondGradientPlowImageAbsThresh(w, h, thresh, imgLog, dxdyThresh);
	//if(fSize){
	//	SchmidtEdgeFilter(w, h, fSize, dxdyThresh, xytFiltered);			//SchmidtEdge.cpp
	//	memcpy(dxdyThresh, xytFiltered, w*h*sizeof(INT));
	//}
	//else{
	//	FiniteFilter(w, h, dxdyThresh, xytFiltered);						//SchmidtEdge.cpp
	//	memcpy(dxdyThresh, xytFiltered, w*h*sizeof(INT));
	//}


	//Remove Singletons
	//UINT x, y;
	//UINT xMax = w-1;
	//UINT yMax = (h-1);

	GetSchmidtContourPOINTs(w, h, dxdyThresh, psPoint);						//SchmidtEdge.cpp

	
	if(dx)			free(dx);
	if(dy)			free(dy);
	if(dxAbs)		free(dxAbs);
	if(dyAbs)		free(dyAbs);
	if(dxdyAbs)		free(dxdyAbs);
	if(xytFiltered)	free(xytFiltered);
	if(imgLog)		free(imgLog);
}

VOID GetSchmidtContourPOINTs(UINT w, UINT h, INT *dxdyThresh, POINT_SET *pointSet)
{

	//if(!w || !h)
	//	return;
	INT off = 3;
	INT row, col, i;
	INT max = w * h;
	UCHAR midPlane;

	//Remove Singletons
	INT x, y;
	INT xMax = w-off;
	INT yMax = h-off;

	//Edge Parameters
	UINT nPoints = 0;
	UINT iPoint = 0;

	POINT *point = (POINT *)calloc(w*h*1, sizeof(POINT));


	//Hysteresis
	//Dependence of a system not only on its current environment but also on its past environment.
	for(y = off; y < yMax; y++){
		for(x = off; x < xMax; x++){
			if(Range(w, h, x, y)){
				i = y*w + x;

				if(!dxdyThresh[i]){
					if(dxdyThresh[i-1]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i+1]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i-w]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i+w]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i+1-w]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i+1+w]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i-1-w]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
					else if(dxdyThresh[i-1+w]){
						point[nPoints].x = x;
						point[nPoints].y = y;
						nPoints++;
					}
				}

			}
		}
	}

	pointSet->typeID = TI_POINT;
	pointSet->nPOINTs = nPoints;
	pointSet->point = (POINT *)malloc(nPoints*sizeof(POINT));
	memcpy(pointSet->point, point, nPoints*sizeof(POINT));
	if(point){
		free(point);
		point = NULL;
	}
}
