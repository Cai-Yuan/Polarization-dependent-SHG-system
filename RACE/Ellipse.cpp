#include "stdafx.h"

#include "Ellipse.h"
#include "Contour.h"
#include "Point.h"

DWORD Free(FELLIPSESET *es)
{
	if(!es){
		return FALSE;
	}

	if(es->ellipse){
		free(es->ellipse);
		es->ellipse = NULL;
	}
	es->nEllipse = NULL;
	es->size = NULL;

	return TRUE;
}
DWORD Alloc(FELLIPSESET *es, DWORD size)
{
	if(!es){
		return FALSE;
	}
	
	Free(es);
	//if(es->ellipse){
	//	Free(es);
		//free(es->ellipse);
		//es->ellipse = NULL;
	//}

	es->ellipse = (FELLIPSE *)malloc(size*sizeof(FELLIPSE));
	memset(es->ellipse, NULL, size*sizeof(FELLIPSE));
	es->nEllipse = NULL;
	es->size = size;

	return TRUE;
}

DWORD GetAt(FELLIPSESET *es, DWORD i, FELLIPSE *e)
{
	if(!es){
		return FALSE;
	}

	if(!e){
		return FALSE;
	}

	if(i >= es->nEllipse){
		return FALSE;
	}

	memcpy(e, &es->ellipse[i], sizeof(ELLIPSE));
	
	return TRUE;
}

DWORD Push(FELLIPSESET *es, FELLIPSE fe)
{
	if(!es){
		return FALSE;
	}

	if(!es->ellipse){
		return FALSE;
	}

	if(es->nEllipse < es->size){
		memcpy(&es->ellipse[es->nEllipse], &fe, sizeof(FELLIPSE));
		es->nEllipse++;
	}else{
		es->nEllipse = es->nEllipse;
	}

	return TRUE;
}

DWORD Clear(FELLIPSESET *es)
{
	if(!es){
		return FALSE;
	}

	if(!es->ellipse){
		return FALSE;
	}
	
	memset(es->ellipse, NULL, es->nEllipse*sizeof(FELLIPSE));
	es->nEllipse = NULL;

	return TRUE;
}

//Moves each point from 
DWORD Transfer(FELLIPSESET *esDest, FELLIPSESET *esSrc)
{
	if(!esDest || !esSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = esSrc->nEllipse;

	//memset(psDest->fpoint, NULL, psDest->nPoint*sizeof(FPOINT));

	//for(i = 0; i < n; i++){
	//	memcpy(psDest->point[i], psSrc->point[i], sizeof(FPOINT));
	//}

	memcpy(esDest->ellipse, esSrc->ellipse, n*sizeof(FELLIPSE));

	memset(esSrc->ellipse, NULL, n*sizeof(FELLIPSE));
	esDest->nEllipse = n;
	esSrc->nEllipse = NULL;

	return TRUE;
}

DWORD Copy(FELLIPSESET *esDest, FELLIPSESET *esSrc)
{
	if(!esDest || !esSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = esSrc->nEllipse;

	if(n > esDest->size){
		n = esDest->size;
	}
	
	memcpy(esDest->ellipse, esSrc->ellipse, n*sizeof(FELLIPSE));
	esDest->nEllipse = n;

	return TRUE;
}


void GetEllipsePoints(UINT nPts, ELLIPSE ellipse, POINT *ptsOut)
{
	UINT i;
	
	FLOAT theta;
	FLOAT TWOPI;
	FLOAT cosa, cost;
	FLOAT sina, sint;
	FLOAT thetaStep;
	FLOAT x, y;

	TWOPI = 2*PI;
	cosa = cos(ellipse.alpha);
	sina = sin(ellipse.alpha);
	thetaStep = (FLOAT)TWOPI/(FLOAT)nPts;

	for(theta = 0, i = 0; theta < TWOPI, i < nPts; theta+=thetaStep, i++){
		cost = cos(theta);
		sint = sin(theta);

		x = ellipse.a*cost*cosa - ellipse.b*sint*sina;
		y = ellipse.a*cost*sina + ellipse.b*sint*cosa;

		ptsOut[i].x = ellipse.x + x;
		ptsOut[i].y = ellipse.y + y;
		
		if(i > 50){
			ptsOut[i].x = ellipse.x + x;
			ptsOut[i].y = ellipse.y + y;
		}
	}
}

void FillEllipseArray(POINT *ellipse, ELLIPSE eli, UINT datapoints)
{
	FLOAT s;
	UINT i = 0;

	FLOAT theta, x, y, PIPI;

	FLOAT cost, cosa, sint, sina;


	PIPI = 2*PI;
	cosa = cos(eli.alpha);
	sina = sin(eli.alpha);
	s = PIPI/datapoints;
	

	//x = eli.a*cos(theta)*cos(eli.alpha) - eli.b*sin(theta)*sin(eli.alpha);
	//y = eli.a*cos(theta)*sin(eli.alpha) + eli.b*sin(theta)*cos(eli.alpha);

	for(theta = 0, i = 0; i < datapoints; i++, theta+=s)
	{
		cost = cos(theta);
		sint = sin(theta);
		x = eli.a*cost*cosa - eli.b*sint*sina;
		y = eli.a*cost*sina + eli.b*sint*cosa;
		x += eli.x;
		y += eli.y;
		ellipse[i].x = x;
		ellipse[i].y = y;
	}
}

void InitEllipseArray(POINT **ellipse, UINT datapoints)
{
	*ellipse = (POINT *)calloc(datapoints, sizeof(POINT));
}

void FreeEllipseArray(POINT *ellipse)
{
	free(ellipse);
}

//Performs very poorly with small segments of ellipses
VOID GetEllipseFromCovarianceMatrix(FLOAT *coMatrix, FLOAT *semiMajor, FLOAT *semiMinor, FLOAT *alphaRad, FLOAT *alphaDeg)
{
	FLOAT a, b, c, theata;
	FLOAT semiMajorLength = 0;
	FLOAT semiMinorLength = 0;
	FLOAT lambda1 = 0;
	FLOAT lambda2 = 0;
	FLOAT determinant = 0;
	FLOAT nu = 0;

	c = coMatrix[0];
	b = -1.0 * coMatrix[1];
	a = coMatrix[3];

	determinant = c*a - (b*b);
	nu = 4 * determinant;

	if(!nu){
		*semiMajor = 0;
		*semiMinor = 0;
		*alphaRad = 0;
		*alphaDeg = 0;
		return;
	}
	c = (FLOAT)c/(FLOAT)nu;
	b = (FLOAT)b/(FLOAT)nu;
	a = (FLOAT)a/(FLOAT)nu;

	lambda1 = ( (FLOAT)nu/(FLOAT)4 /*2*/)* (c + a + sqrt( (c-a)*(c-a) + 4*b*b ) );
	lambda2 = ( (FLOAT)nu/(FLOAT)4 /*2*/)* (c + a - sqrt( (c-a)*(c-a) + 4*b*b ) );

	if(lambda1 > lambda2){
		if(lambda1 < 0) lambda1 = abs(lambda1);
		if(lambda2 < 0) lambda2 = abs(lambda2);
		*semiMajor = 2*sqrt(lambda1);
		*semiMinor = 2*sqrt(lambda2);
	}
	else{
		if(lambda1 < 0) lambda1 = abs(lambda1);
		if(lambda2 < 0) lambda2 = abs(lambda2);
		*semiMajor = 2*sqrt(lambda2);
		*semiMinor = 2*sqrt(lambda1);
	}
	*alphaRad = 0.5 * atan2((-1*2*b),(c-a) );
	*alphaDeg = (*alphaRad) * ((FLOAT)180/(FLOAT)PI);
}



VOID GetEllipseFromPoints(UINT nElements, POINT *dataIn, ELLIPSE *ellipse)
{
	FLOAT xBar, yBar;
	FLOAT *coMatrix;
	FLOAT majorAxis, minorAxis;
	FLOAT alphaRad, alphaDeg;

	coMatrix = (FLOAT *)malloc(4 * sizeof(FLOAT));

	GetMean(nElements, dataIn, &xBar, &yBar);
	GetCovarianceMatrix(nElements, xBar, yBar, dataIn, coMatrix);
	GetEllipseFromCovarianceMatrix(coMatrix, &majorAxis, &minorAxis, &alphaRad, &alphaDeg);

	ellipse->x = xBar;
	ellipse->y = yBar;
	ellipse->a = majorAxis;
	ellipse->b = minorAxis;
	ellipse->alpha = alphaRad;
}

VOID fGetEllipseFromPoints(UINT nElements, POINT *dataIn, fELLIPSE *ellipse)
{
	FLOAT xBar, yBar;
	FLOAT *coMatrix;
	FLOAT majorAxis, minorAxis;
	FLOAT alphaRad, alphaDeg;

	coMatrix = (FLOAT *)malloc(4 * sizeof(FLOAT));

	GetMean(nElements, dataIn, &xBar, &yBar);
	GetCovarianceMatrix(nElements, xBar, yBar, dataIn, coMatrix);
	GetEllipseFromCovarianceMatrix(coMatrix, &majorAxis, &minorAxis, &alphaRad, &alphaDeg);

	ellipse->x = xBar;
	ellipse->y = yBar;
	ellipse->a = majorAxis;
	ellipse->b = minorAxis;
	ellipse->alpha = alphaRad;
}

void GetEllipseFromPoints(UINT nElements, POINT *dataIn, FELLIPSE *ellipse)
{
	FLOAT xBar, yBar;
	FLOAT *coMatrix;
	FLOAT majorAxis, minorAxis;
	FLOAT alphaRad, alphaDeg;

	coMatrix = (FLOAT *)malloc(4 * sizeof(FLOAT));

	GetMean(nElements, dataIn, &xBar, &yBar);
	GetCovarianceMatrix(nElements, xBar, yBar, dataIn, coMatrix);
	GetEllipseFromCovarianceMatrix(coMatrix, &majorAxis, &minorAxis, &alphaRad, &alphaDeg);

	ellipse->x = xBar;
	ellipse->y = yBar;
	ellipse->a = majorAxis;
	ellipse->b = minorAxis;
	ellipse->alpha = alphaRad;

	if(coMatrix)free(coMatrix);
}

VOID GetEllipseCentroid(UINT nEllipse, FELLIPSE *ellipse, FPOINT *centroid)
{
	UINT i = 0;

	for(i = 0; i < nEllipse; i++){
		centroid[i].x = ellipse[i].x;
		centroid[i].y = ellipse[i].y;
	}

}

DWORD ShiftEllipseCentroid(DWORD nEllipse, ELLIPSE *ellipse, INT x, INT y)
{
	if(!ellipse)
		return FALSE;

	INT i = NULL;

	for(i = 0; i < nEllipse; i++){
		ellipse[i].x = ellipse[i].x + x;
		ellipse[i].y = ellipse[i].y + y;
	}

	return TRUE;
}

DWORD ShiftEllipseCentroid(DWORD nEllipse, FELLIPSE *ellipse, FLOAT x, FLOAT y)
{
	if(!ellipse)
		return FALSE;

	INT i = NULL;

	for(i = 0; i < nEllipse; i++){
		ellipse[i].x = ellipse[i].x + x;
		ellipse[i].y = ellipse[i].y + y;
	}

	return TRUE;
}

//Very Poor Job -- Mostly Simplified for Ellipses Similar to Circles
UINT RemoveInnerEllipse(UINT nEllipseIN, UINT nEllipseOUT, FELLIPSE *ellipseIN, FELLIPSE *ellipseOUT)
{
	memset(ellipseOUT, 0x00, nEllipseIN*sizeof(FELLIPSE));
	memcpy(ellipseOUT, ellipseIN, sizeof(FELLIPSE));
	UINT iKeep = 1;
	UINT iK = 0;
	UINT iIn = nEllipseIN;
	UINT iI = 0;
	UINT i;
	FLOAT abAvgOne, abAvgTwo;
	FLOAT dCentroid;

	UINT *iRemove = (UINT *)calloc(nEllipseIN, sizeof(FELLIPSE));

	for(i = 0; i < nEllipseIN; i++){

		for(iI = 1; iI < nEllipseIN; iI++){
			if(i != iI && !iRemove[iI]){

				dCentroid = GetDistance(FPoint(ellipseIN[i].x, ellipseIN[i].y), FPoint(ellipseIN[iI].x, ellipseIN[iI].y));
				if((dCentroid + ellipseIN[iI].b) < (ellipseIN[i].b * 1.2)){
					iRemove[iI] = TRUE;                 
				}

			}
		}
	}

	UINT nKeep = 0;
	for(i = 0; i < nEllipseIN; i++){
		if(!iRemove[i]){
			memcpy(&ellipseOUT[nKeep], &ellipseIN[i], sizeof(FELLIPSE));
			nKeep++;
		}
	}

	//Untested
	if(iRemove)free(iRemove);

	return nKeep;
}

DWORD RemoveInnerEllipse(FELLIPSESET *es)
{

	if(!es){
		return FALSE;
	}

	INT x = NULL;
	INT y = NULL;
	DWORD i = NULL;
	DWORD p = NULL;
	DWORD nEllipse = es->nEllipse;

	FLOAT dist = NULL;
	FLOAT diam = NULL;
	FLOAT e0Size = NULL;
	FLOAT e1Size = NULL;

	FELLIPSE e0 = {0};
	FELLIPSE e1 = {0};
	FELLIPSESET esKeep = {0};
	Alloc(&esKeep, es->nEllipse);

	//e0 Serves as the reference ellipse (each ellipse is only addressed one time)
	//e1 Serves as the measure ellipse
	for(i = 0; i < nEllipse; i++){
		for(p = 0; p < nEllipse; p++){
		
			if(i != p){
				GetAt(es, i, &e0);
				GetAt(es, p, &e1);

				e0Size = e0.a*e0.b;
				e1Size = e1.a*e1.b;

				//If the reference ellipse is not larger, skip analysis
				//Only wish to keep the larger outter ellipse
				if(e0Size < e1Size){
					break;
				}

				x = abs(e0.x-e1.x);
				y = abs(e0.y-e1.y);

				dist = GetDistance(FPoint(e0.x, e0.y), FPoint(e1.x, e1.y));
				
				if((dist + e1.b) < (e0.b*1.2)){
					//e1 is inside of e0
				}else{
					Push(&esKeep, e0);
				}



			}

		}
	}

	Copy(es, &esKeep);
	Free(&esKeep);

	return TRUE;


}

DWORD RemoveDuplicates(FELLIPSESET *es)
{
	if(!es){
		return FALSE;
	}


	DWORD i = NULL;
	DWORD p = NULL;
	DWORD flgStore = TRUE;

	FELLIPSE e = {0};
	FELLIPSE eRef = {0};
	FELLIPSESET esKeep = {0};
	Alloc(&esKeep, es->nEllipse);

	for(i = 0; i < es->nEllipse; i++){
		GetAt(es, i, &eRef);
		for(p = i+1; p < es->nEllipse; p++){
			GetAt(es, p, &e);

			if(eRef.x == e.x && eRef.y == e.y/* && eRef.a == eRef.a && eRef.b == e.b*/){
				flgStore = FALSE;
			}

		}

		if(flgStore){
			Push(&esKeep, eRef);
		}
		flgStore = TRUE;
	}

	Copy(es, &esKeep);
	Free(&esKeep);


	return TRUE;
}

DWORD EllipseSetFromContourSet(DWORD threshold, FELLIPSESET *es, CONTOURSET *cs)
{
	if(!es || !cs){
		return FALSE;
	}

	INT i = NULL;
	INT n = cs->nContour;
	FELLIPSE fe = {0};

	for(i = 0; i < n; i++){

		if(cs->contour[i].nPoint > threshold){
			GetEllipseFromPoints(cs->contour[i].nPoint, cs->contour[i].point, &fe);
			if(!Push(es, fe)){
				break;
			}
		}
	}

	return TRUE;
}

DWORD CentroidFromEllipseSet(FPOINTSET *ps, FELLIPSESET *es)
{
	if(!ps || !es){
		return FALSE;
	}

	INT i = NULL;
	INT n = es->nEllipse;
	FPOINT point = {0};
	Empty(ps);

	for(i = NULL; i < n; i++){
		point.x = es->ellipse[i].x;
		point.y = es->ellipse[i].y;
		Push(ps, point);
	}

	return TRUE;
}

DWORD OffsetCentroid(FELLIPSESET *es, FLOAT x, FLOAT y)
{
	if(!es){
		return FALSE;
	}

	INT i = NULL;
	INT n = es->nEllipse;

	for(i = 0; i < n; i++){
		es->ellipse[i].x += x;
		es->ellipse[i].y += y;
	}

	return TRUE;
}

FLOAT GetEccentricity(FELLIPSE *fe)
{
	FLOAT e;
	FLOAT f;
	FLOAT aa = fe->a*fe->a;
	FLOAT bb = fe->b*fe->b;

	f = GetFocus(fe);
	e = (FLOAT) sqrt((double)(aa-bb)/(double)aa);

	return e;
}

FLOAT GetFocus(FELLIPSE *fe)
{
	FLOAT f;
	FLOAT aa = fe->a*fe->a;
	FLOAT bb = fe->b*fe->b;

	f = sqrt(aa + bb);
	

	return f;
}