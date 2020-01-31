#pragma once
#include "stdafx.h"
#include "Line.h"

typedef struct CONTOUR_{
	DWORD size;
	DWORD nPoint;
	POINT *point;
}CONTOUR;

typedef struct FCONTOUR_{
	DWORD size;
	DWORD nPoint;
	POINT *point;
}FCONTOUR;

typedef struct CONTOURSET_{
	DWORD size;
	DWORD nContour;
	CONTOUR *contour;
}CONTOURSET;

typedef struct FCONTOURSET_{
	DWORD size;
	DWORD nContour;
	FCONTOUR *contour;
}FCONTOURSET;

//typedef struct CONTOUR_{
//	UINT nPoint;
//	POINT *point;
//}CONTOUR;
//
//typedef struct CONTOURSET_{
//	UINT nContour;
//	CONTOUR *contour;
//}CONTOURSET;

DWORD Zero(CONTOUR *c);

DWORD GetContour(CONTOUR *c, FLINESEGMENTSET *lss);

DWORD DrawContour(UINT w, UINT h, PIXEL value, CONTOUR *c, PIXEL *img);
DWORD DrawContourSet(UINT w, UINT h, PIXEL value, CONTOURSET *cs, PIXEL *img);

DWORD DrawContour(UINT w, UINT h, UCHAR value, CONTOUR *c, UCHAR *img);
DWORD DrawContourSet(UINT w, UINT h, UCHAR value, CONTOURSET *cs, UCHAR *img);

DWORD Append(CONTOUR *c, POINT pt);
DWORD Zero(CONTOURSET *cs);
DWORD Free(CONTOURSET *cs);
DWORD AllocContourSet(CONTOURSET *cs, DWORD nContour, DWORD lenContour);