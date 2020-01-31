#pragma once

#include "stdafx.h"
#include "Constants.h"
#include "Statistics2D.h"
#include "Contour.h"

#define MAX_ELLIPSE 256

typedef struct ELLIPSE_{
	UINT a;
	UINT b;
	UINT x;
	UINT y;
	FLOAT alpha;
}ELLIPSE;

typedef struct fELLIPSE_{
	FLOAT a;
	FLOAT b;
	FLOAT x;
	FLOAT y;
	FLOAT alpha;
}fELLIPSE;

typedef struct FELLIPSE_{
	FLOAT a;
	FLOAT b;
	FLOAT x;
	FLOAT y;
	FLOAT alpha;
}FELLIPSE;

//typedef struct ELLIPSESET_{
//	UINT nEllipse;
//
//	ELLIPSE ellipse[MAX_ELLIPSE];
//	FELLIPSE fellipse[MAX_ELLIPSE];
//}ELLIPSESET;

typedef struct FELLIPSESET_{
	DWORD size;
	DWORD nEllipse;
	FELLIPSE *ellipse;
}FELLIPSESET;

FLOAT GetEccentricity(FELLIPSE *fe);
FLOAT GetFocus(FELLIPSE *fe);


void InitEllipseArray(POINT **ellipse, UINT datapoints);
void FillEllipseArray(POINT *ellipse, ELLIPSE eli, UINT datapoints);
void FreeEllipseArray(POINT *ellipse);

void GetEllipsePoints(UINT nPts, ELLIPSE ellipse, POINT *ptsOut);

void GetEllipseFromCovarianceMatrix(FLOAT *coMatrix, FLOAT *semiMajor, FLOAT *semiMinor, FLOAT *alphaRad, FLOAT *alphaDeg);
void GetEllipseFromPoints(UINT nElements, POINT *dataIn, ELLIPSE *ellipse);
void fGetEllipseFromPoints(UINT nElements, POINT *dataIn, FELLIPSE *ellipse);
void GetEllipseFromPoints(UINT nElements, POINT *dataIn, FELLIPSE *ellipse);

VOID GetEllipseCentroid(UINT nEllipse, FELLIPSE *ellipse, FPOINT *centroid);

DWORD ShiftEllipseCentroid(DWORD nEllipse, ELLIPSE *ellipse, INT x, INT y);
DWORD ShiftEllipseCentroid(DWORD nEllipse, FELLIPSE *ellipse, FLOAT x, FLOAT y);

DWORD RemoveDuplicates(FELLIPSESET *es);
DWORD RemoveInnerEllipse(FELLIPSESET *es);
UINT RemoveInnerEllipse(UINT nEllipseIN, UINT nEllipseOUT, FELLIPSE *ellipseIN, FELLIPSE *ellipseOUT);

DWORD OffsetCentroid(FELLIPSESET *es, FLOAT x, FLOAT y);
DWORD EllipseSetFromContourSet(DWORD threshold, FELLIPSESET *es, CONTOURSET *cs);
DWORD CentroidFromEllipseSet(FPOINTSET *ps, FELLIPSESET *es);

DWORD Free(FELLIPSESET *es);
DWORD Alloc(FELLIPSESET *es, DWORD size);
DWORD Push(FELLIPSESET *es, FELLIPSE fe);
DWORD Clear(FELLIPSESET *es);
DWORD Transfer(FELLIPSESET *esDest, FELLIPSESET *esSrc);
DWORD Copy(FELLIPSESET *esDest, FELLIPSESET *esSrc);
DWORD GetAt(FELLIPSESET *es, DWORD i, FELLIPSE *e);

