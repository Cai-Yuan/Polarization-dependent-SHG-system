#include "stdafx.h"

void GetMean(UINT nElements, POINT *pts, FLOAT *xBar, FLOAT *yBar)
{
	UINT i = 0;
	INT xRun = 0;
	INT yRun = 0;
	
	if(nElements <= 0) return;

	for(i = 0; i < nElements; i++){
		xRun += pts[i].x;
		yRun += pts[i].y;
	}

	*xBar = ((FLOAT)xRun /(FLOAT)nElements);
	*yBar = ((FLOAT)yRun /(FLOAT)nElements);

}

//ELEMENT not defined
//void GetMean(UINT nElements, ELEMENT *element, FLOAT *xBar, FLOAT *yBar)
//{
//	UINT i = 0;
//	INT xRun = 0;
//	INT yRun = 0;
//	
//	if(nElements <= 0) return;
//
//	for(i = 0; i < nElements; i++){
//		xRun += element[i].x;
//		yRun += element[i].y;
//	}
//
//	*xBar = ((FLOAT)xRun /(FLOAT)nElements);
//	*yBar = ((FLOAT)yRun /(FLOAT)nElements);
//
//}

void GetCovariance(UINT nElements, FLOAT xBar, FLOAT yBar, POINT *pt, FLOAT *coVar)
{

	UINT i;
	UINT coVarRun = 0;
	
	if(nElements <= 0){
		return;
	}

	for(i = 0; i < nElements; i++){
		coVarRun += ( (pt[i].x - xBar)*(pt[i].y - yBar) );
	}

	*coVar = coVarRun/(nElements - 1);
}

void SubtractMean(UINT nElements, FLOAT xBar, FLOAT yBar, POINT *ptsIn, POINT *ptsOut)
{

	UINT i;

	for(i = 0; i < nElements; i++){
		ptsOut[i].x = ptsIn[i].x - xBar;
		ptsOut[i].y = ptsIn[i].y - yBar;
	}

}

void GetCovarianceMatrix(UINT nElements, FLOAT xBar, FLOAT yBar, POINT *pts, FLOAT *coMatrix)
{
	UINT i;
	LONG coVarRun;
	
	if(nElements > 1){
		for(i = 0, coVarRun = 0; i < nElements; i++){
			coVarRun += ( (pts[i].x - xBar)*(pts[i].x - xBar) );
		}
		coMatrix[0] = (FLOAT)coVarRun/(FLOAT)(nElements - 1);

		for(i = 0, coVarRun = 0; i < nElements; i++){
			coVarRun += ( (pts[i].x - xBar)*(pts[i].y - yBar) );
		}
		coMatrix[1] = (FLOAT)coVarRun/(FLOAT)(nElements - 1);
		coMatrix[2] = coMatrix[1];

		for(i = 0, coVarRun = 0; i < nElements; i++){
			coVarRun += ( (pts[i].y - yBar)*(pts[i].y - yBar) );
		}
		coMatrix[3] = (FLOAT)coVarRun/(FLOAT)(nElements - 1);
	}
	else{
		for(i = 0, coVarRun = 0; i < nElements; i++){
			coVarRun += ( (pts[i].x - xBar)*(pts[i].x - xBar) );
		}
		coMatrix[0] = (FLOAT)coVarRun/(FLOAT)(nElements);

		for(i = 0, coVarRun = 0; i < nElements; i++){
			coVarRun += ( (pts[i].x - xBar)*(pts[i].y - yBar) );
		}
		coMatrix[1] = (FLOAT)coVarRun/(FLOAT)(nElements);
		coMatrix[2] = (FLOAT)coMatrix[1];

		for(i = 0, coVarRun = 0; i < nElements; i++){
			coVarRun += ( (pts[i].y - yBar)*(pts[i].y - yBar) );
		}
		coMatrix[3] = (FLOAT)coVarRun/(FLOAT)(nElements);
	}


}

VOID RemoveOutliers(UINT nPointsIn, UINT *nPointsOut, FLOAT sigma, POINT *pointIn, POINT *pointOut)
{
	UINT i, m;
	UINT nPts;
	UINT nPointsTemp = nPointsIn;
	UINT iVarMax = 0;
	FLOAT coVariance = 0;
	FLOAT variance = 0;
	FLOAT varMax = 0;
	FLOAT xBar = 0;
	FLOAT yBar = 0;
	FLOAT xBarPrev = 0;
	FLOAT yBarPrev = 0;
	POINT *pointTemp = NULL;

	pointTemp = (POINT *)malloc(nPointsIn*sizeof(POINT));

	*nPointsOut = 0;
	GetMean(nPointsIn, pointIn, &xBar, &yBar);
	GetCovariance(nPointsIn, xBar, yBar, pointIn, &coVariance);
	xBarPrev = xBar;
	yBarPrev = yBar;

	memset(pointOut, 0x00, nPointsIn*sizeof(POINT));
	memset(pointTemp, 0x00, nPointsIn*sizeof(POINT));
	memcpy(pointTemp, pointIn, nPointsIn*sizeof(POINT));

	nPts = 0;
	for(m = 0; m < nPointsIn; m++){

		varMax = 0;
		for(i = 0; i < nPointsTemp; i++){
			variance = abs(((pointTemp[i].x - xBar)*(pointTemp[i].y - yBar) / sqrt(coVariance)));
			if(variance > varMax){
				varMax = variance;
				iVarMax = i;
			}
		}

		if(varMax >= sigma){
			pointTemp[iVarMax].x = pointTemp[nPointsTemp-1].x;
			pointTemp[iVarMax].y = pointTemp[nPointsTemp-1].y;
			nPointsTemp--;
		}
		else{
			memcpy(pointOut, pointTemp, nPointsTemp*sizeof(POINT));
			*nPointsOut = nPointsTemp;
			break;
		}

		//Push Data from pointOut into pointTemp 
		GetMean(nPointsTemp, pointTemp, &xBar, &yBar);

		//See if the algorithm converged
		//The mean won't change then
		if(xBarPrev == xBar && yBarPrev == yBar){
			memcpy(pointOut, pointTemp, nPointsTemp*sizeof(POINT));
			*nPointsOut = nPointsTemp;
			break;
		}
		else{
			xBarPrev = xBar;
			yBarPrev = yBar;
			GetCovariance(nPointsTemp, xBar, yBar, pointTemp, &coVariance);
			//nPointsTemp = nPts;
			nPts = 0;
		}
	}
	memcpy(pointOut, pointTemp, nPointsTemp*sizeof(POINT));
	*nPointsOut = nPointsTemp;

		

	free(pointTemp);

}

VOID RemoveOutliers(UINT nPointsIn, UINT iterations, UINT *nPointsOut, FLOAT sigma, POINT *pointIn, POINT *pointOut)
{
	UINT i, m;
	UINT nPts;
	UINT nPointsTemp = nPointsIn;
	UINT iVarianceMax = 0;
	FLOAT coVariance = 0;
	FLOAT variance = 0;
	FLOAT varMax = 0;
	FLOAT xBar = 0;
	FLOAT yBar = 0;
	FLOAT xBarPrev = 0;
	FLOAT yBarPrev = 0;
	POINT *pointTemp = NULL;

	pointTemp = (POINT *)malloc(nPointsIn*sizeof(POINT));

	*nPointsOut = 0;
	GetMean(nPointsIn, pointIn, &xBar, &yBar);
	GetCovariance(nPointsIn, xBar, yBar, pointIn, &coVariance);
	xBarPrev = xBar;
	yBarPrev = yBar;

	memset(pointOut, 0x00, nPointsIn*sizeof(POINT));
	memset(pointTemp, 0x00, nPointsIn*sizeof(POINT));
	memcpy(pointTemp, pointIn, nPointsIn*sizeof(POINT));

	nPts = 0;
	for(m = 0; m < iterations; m++){

		varMax = 0;
		for(i = 0; i < nPointsTemp; i++){
			variance = abs(((pointTemp[i].x - xBar)*(pointTemp[i].y - yBar) / sqrt(coVariance)));
			if(variance > varMax){
				varMax = variance;
				iVarianceMax = i;
			}
		}

		if(varMax >= sigma){
			pointTemp[iVarianceMax].x = pointTemp[nPointsTemp-1].x;
			pointTemp[iVarianceMax].y = pointTemp[nPointsTemp-1].y;
			nPointsTemp--;
		}
		else{
			memcpy(pointOut, pointTemp, nPointsTemp*sizeof(POINT));
			*nPointsOut = nPointsTemp;
			break;
		}

		//Push Data from pointOut into pointTemp 
		GetMean(nPointsTemp, pointTemp, &xBar, &yBar);

		//See if the algorithm converged
		//The mean won't change then
		if(xBarPrev == xBar && yBarPrev == yBar){
			memcpy(pointOut, pointTemp, nPointsTemp*sizeof(POINT));
			*nPointsOut = nPointsTemp;
			break;
		}
		else{
			xBarPrev = xBar;
			yBarPrev = yBar;
			GetCovariance(nPointsTemp, xBar, yBar, pointTemp, &coVariance);
			//nPointsTemp = nPts;
			nPts = 0;
		}
	}
	memcpy(pointOut, pointTemp, nPointsTemp*sizeof(POINT));
	*nPointsOut = nPointsTemp;

		

	//free(pointTemp);

}

VOID RemoveOutliers(UINT iterations, FLOAT sigma, UINT *nPoints, POINT *point)
{
	UINT i, m;
	UINT nPts;
	UINT nPointsTemp = *nPoints;
	UINT iVarianceMax = 0;
	FLOAT coVariance = 0;
	FLOAT variance = 0;
	FLOAT varianceMax = 0;
	FLOAT xBar = 0;
	FLOAT yBar = 0;
	FLOAT xBarPrev = 0;
	FLOAT yBarPrev = 0;

	GetMean(*nPoints, point, &xBar, &yBar);
	GetCovariance(*nPoints, xBar, yBar, point, &coVariance);
	xBarPrev = xBar;
	yBarPrev = yBar;

	nPts = *nPoints;
	for(m = 0; m < iterations; m++){

		varianceMax = 0;
		for(i = 0; i < nPts; i++){
			variance = abs(((point[i].x - xBar)*(point[i].y - yBar) / sqrt(coVariance)));
			if(variance > varianceMax){
				varianceMax = variance;
				iVarianceMax = i;
			}
		}

		//Check to see if there is an outlier
		//If so, remove it by replacing it with the last data
		//If not, there are no more noisy data points, so abort
		if(varianceMax >= sigma){
			point[iVarianceMax].x = point[nPts-1].x;
			point[iVarianceMax].y = point[nPts-1].y;
			if(nPts > 0)
				nPts--;
		}
		else{
			*nPoints = nPts;
			return;
		}

		GetMean(nPts, point, &xBar, &yBar);

		//See if the algorithm converged
		//The mean won't change then
		if(xBarPrev == xBar && yBarPrev == yBar){
			*nPoints = nPts;
			return;
		}
		else{
			xBarPrev = xBar;
			yBarPrev = yBar;
			GetCovariance(nPts, xBar, yBar, point, &coVariance);
		}
	}

	*nPoints = nPts;

}

//ZScore should be less than 3 to be considered data
VOID GetZScore(FLOAT xBar, FLOAT yBar, FLOAT xStdDev, FLOAT yStdDev, POINT point, FLOAT *zX, FLOAT *zY)
{
	*zX = (point.x - xBar)/xStdDev;
	*zY = (point.y - yBar)/yStdDev;
}

//VOID RemoveOutliers(UINT nPointsIn, UINT *nPointsOut, FLOAT sigma, POINT *pointIn, POINT *pointOut)
//{
//	UINT i, m;
//	UINT nPts;
//	UINT nPointsTemp = nPointsIn;
//	FLOAT coVariance = 0;
//	FLOAT variance = 0;
//	FLOAT xBar = 0;
//	FLOAT yBar = 0;
//	FLOAT xBarPrev = 0;
//	FLOAT yBarPrev = 0;
//	POINT *pointTemp = NULL;
//
//	pointTemp = (POINT *)malloc(nPointsIn*sizeof(POINT));
//
//	*nPointsOut = 0;
//	GetMean(nPointsIn, pointIn, &xBar, &yBar);
//	GetCovariance(nPointsIn, xBar, yBar, pointIn, &coVariance);
//	xBarPrev = xBar;
//	yBarPrev = yBar;
//
//	memset(pointOut, 0x00, nPointsIn*sizeof(POINT));
//	memset(pointTemp, 0x00, nPointsIn*sizeof(POINT));
//	memcpy(pointTemp, pointIn, nPointsIn*sizeof(POINT));
//
//	nPts = 0;
//	for(m = 0; m < 100; m++){
//
//		for(i = 0; i < nPointsTemp; i++){
//			variance = abs(((pointTemp[i].x - xBar)*(pointTemp[i].y - yBar) / coVariance));
//			if(variance < sigma){
//				pointOut[nPts].x = pointTemp[i].x;
//				pointOut[nPts].y = pointTemp[i].y;
//				nPts++;
//			}
//		}
//
//		//Push Data from pointOut into pointTemp
//		memset(pointTemp, 0x00, nPointsTemp*sizeof(POINT));
//		memcpy(pointTemp, pointOut, nPts*sizeof(POINT)); 
//		GetMean(nPts, pointTemp, &xBar, &yBar);
//
//		//See if the algorithm converged
//		//The mean won't change then
//		if(xBarPrev == xBar && yBarPrev == yBar){
//			*nPointsOut = nPts;
//			break;
//		}
//		else{
//			xBarPrev = xBar;
//			yBarPrev = yBar;
//			GetCovariance(nPts, xBar, yBar, pointTemp, &coVariance);
//			nPointsTemp = nPts;
//			nPts = 0;
//		}
//	}
//
//		
//
//	free(pointTemp);
//
//}


//void GetEigenVectorsFromCovarianceMatrix(FLOAT *coMatrix, EIGENVECTOR eVector)
//{
//	FLOAT a, b, c, theata;
//	FLOAT semiMajorLength;
//	FLOAT semiMinorLength;
//	FLOAT lambda1;
//	FLOAT lambda2;
//	FLOAT determinant;
//	FLOAT nu;
//
//	c = coMatrix[0];
//	b = -1.0 * coMatrix[1];
//	a = coMatrix[3];
//
//	determinant = c*a - (b*b);
//	nu = 4 * determinant;
//
//	c = (FLOAT)c/(FLOAT)nu;
//	b = (FLOAT)b/(FLOAT)nu;
//	a = (FLOAT)a/(FLOAT)nu;
//
//	lambda1 = ( (FLOAT)nu/(FLOAT)4 )* (c + a + sqrt( (c-a)*(c-a) + 4*b*b ) );
//	lambda2 = ( (FLOAT)nu/(FLOAT)4 )* (c + a - sqrt( (c-a)*(c-a) + 4*b*b ) );
//
//	if(lambda1 > lambda2){
//		if(lambda1 < 0) lambda1 = abs(lambda1);
//		if(lambda2 < 0) lambda2 = abs(lambda2);
//		*semiMajor = 2*sqrt(lambda1);
//		*semiMinor = 2*sqrt(lambda2);
//	}
//	else{
//		if(lambda1 < 0) lambda1 = abs(lambda1);
//		if(lambda2 < 0) lambda2 = abs(lambda2);
//		*semiMajor = 2*sqrt(lambda2);
//		*semiMinor = 2*sqrt(lambda1);
//	}
//	*alphaRad = 0.5 * atan2((2*b),(c-a) );
//	*alphaDeg = (*alphaRad) * ((FLOAT)180/(FLOAT)PI);
//}
//
////Least Squares Method
////-minimizes the perpendicular distance between the data points and line
//void GetLineFromPoints(UINT nElements, POINT *dataIn, LINE_ *line)
//{
//	FLOAT xBar, yBar;
//	FLOAT *coMatrix;
//	FLOAT majorAxis, minorAxis;
//	FLOAT alphaRad, alphaDeg;
//
//	coMatrix = (FLOAT *)malloc(4 * sizeof(FLOAT));
//
//	GetMean(nElements, dataIn, &xBar, &yBar);
//	GetCovarianceMatrix(nElements, xBar, yBar, dataIn, coMatrix);
//	GetEllipseFromCovarianceMatrix(coMatrix, &majorAxis, &minorAxis, &alphaRad, &alphaDeg);
//
//}




