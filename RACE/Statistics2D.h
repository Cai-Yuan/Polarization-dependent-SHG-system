#pragma once

#include "stdafx.h"

typedef struct EIGENVECTOR_{
	FLOAT x;
	FLOAT y;
}EIGENVECTOR;

void GetMean(UINT nElements, POINT *pt, FLOAT *xBar, FLOAT *yBar);
//void GetMean(UINT nElements, ELEMENT *element, FLOAT *xBar, FLOAT *yBar);
void SubtractMean(UINT nElements, FLOAT xBar, FLOAT yBar, POINT *ptsIn, POINT *ptsOut);
void GetCovariance(UINT nElements, FLOAT xBar, FLOAT yBar, POINT *pt, FLOAT *coVar);
void GetCovarianceMatrix(UINT nElements, FLOAT xBar, FLOAT yBar, POINT *pts, FLOAT *coMatrix);
VOID RemoveOutliers(UINT nPointsIn, UINT *nPointsOut, FLOAT sigma, POINT *pointIn, POINT *pointOut);
VOID RemoveOutliers(UINT nPointsIn, UINT iterations, UINT *nPointsOut, FLOAT sigma, POINT *pointIn, POINT *pointOut);
VOID RemoveOutliers(UINT iterations, FLOAT sigma, UINT *nPoints, POINT *point);