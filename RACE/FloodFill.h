//#pragma once

//#include "stdafx.h"
#include "Point.h"
#include "PavlidisContour.h"

typedef struct FRONTIER_{

	DWORD size;
	DWORD nFront;
	UINT *front;

}FRONTIER;

DWORD FloodFill(UINT w, UINT h, POINT seed, INT value, INT *img);
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, INT *img);
DWORD FillPolygon(UINT w, UINT h, UCHAR value, POLYGON *polygon, UCHAR *img);
DWORD FloodFill(UINT w, UINT h, INT seed, UCHAR value, UCHAR *img);
DWORD FloodFill(UINT w, UINT h, POINT seed, UCHAR value, UCHAR *img);
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, INT *img, INT *out);
DWORD Alloc(FRONTIER *frontier, DWORD size);
DWORD Push(FRONTIER *frontier, UINT index);
UINT Pop(FRONTIER *frontier);
DWORD IsEmpty(FRONTIER frontier);
DWORD Free(FRONTIER *frontier);

DWORD FloodFill(UINT w, UINT h, INT *imgIn, INT *imgOut);
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, INT *img, INT *out);

DWORD FloodFill(UINT w, UINT h, UCHAR *imgIn, UCHAR *imgOut);
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, UCHAR *img, UCHAR *out);

DWORD FloodFill(UINT w, UINT h, FLOAT *imgIn, FLOAT *imgOut);
DWORD FloodFill(UINT w, UINT h, INT seed, INT value, FLOAT *in, FLOAT *out);

//#define TEMPTEMP 100;
//
//typedef struct RUNNER_{
//	POINT p0; 
//	POINT p1;
//	POINT p2;
//	POINT p3;
//	INT dir;
//}RUNNER;
//
////VOID FillPolygon(UINT w, UINT h, POLYGON *polygon, UCHAR *bitmap, GREYREF greyref)
//DWORD FillPolygon(UINT w, UINT h, UCHAR value, POLYGON *polygon, UCHAR *img);
//VOID FloodFill(UINT w, UINT h, POINT *ptSeed, UCHAR *bitmap, GRAYREF grayref);
//INT FillAroundPoint(UINT w, UINT h, POINT *ptCenter, POINT *point, UCHAR *bitmap, GRAYREF grayref);
//UINT GetEnterDirection(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap /*bmUCHAR*/);
//UINT GetPR(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap);
//UINT GetPL(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap);
//POINT GetPointPL(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap);
//
//POINT GetPointPR(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap);
//UINT SetPR(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap, GRAYREF grayref);
//UINT SetPL(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap, GRAYREF grayref);
//UINT SetP0(UINT w, UINT h, RUNNER *runner, UCHAR *bitmap, GRAYREF grayref);
//
//VOID WildfireFill(UINT w, UINT h, POINT *ptSeed, UCHAR *bitmap, GRAYREF grayref);