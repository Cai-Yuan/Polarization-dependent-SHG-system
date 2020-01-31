#pragma once

#include "stdafx.h"
#include "Contour.h"
#include "Range.h"

#define MAX_LOOP		400
#define MAX_ROTATE		4

#define FORWARD			0
#define RIGHT			1
#define LEFT			(-1)

#define DIR_UP			0
#define DIR_RIGHT		1
#define DIR_DOWN		2
#define DIR_LEFT		3
#define DIR_NOTFOUND	4

#define P1		1
#define P2		2
#define P3		3

VOID TheoPavlidisContourTrace(UINT w, UINT h, INT *imgINT, INT *contourINT);
VOID TheoPavlidisContour(UINT w, UINT h, INT *imgINT, CONTOURSET *contourSet);
VOID TheoPavlidisContour(UINT w, UINT h, INT *imgINT, INT *contourINT, CONTOURSET *contourSet);
VOID TheoPavlidisContour(UINT w, UINT h, INT *imgINT, POINT_SET *mask, INT *contourINT, CONTOURSET *contourSet);

INT GetDirection(UINT move, INT dir);
VOID RunContour(UINT w, UINT h, UINT iStart, UINT iPos, UINT dir, INT *imgINT, INT *contourINT);
//VOID LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, INT *imgINT, INT *contourINT);
VOID LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, UINT dirEnter, INT *imgINT, INT *contourINT);
VOID LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, INT *imgINT, INT *contourINT, CONTOUR *contour);
VOID LabelContourTrace(UINT w, UINT h, UINT iStart, UINT iPos, UINT iContour, UINT dirEnter, INT *imgINT, INT *contourINT);


INT GetEnterDirection(UINT w, UINT h, UINT iPos, UINT dirEnter, INT *imgINT);
INT GetSingleton(UINT w, UINT h, UINT i, UINT dir, INT *imgINT);

INT GetP1(UINT w, UINT h, UINT x, UINT y, UINT dir, INT *inINT);
INT GetP2(UINT w, UINT h, UINT x, UINT y, UINT dir, INT *inINT);
INT GetP3(UINT w, UINT h, UINT x, UINT y, UINT dir, INT *inINT);

UINT GetP1Index(UINT w, UINT h, UINT i, UINT dir);
UINT GetP2Index(UINT w, UINT h, UINT i, UINT dir);
UINT GetP3Index(UINT w, UINT h, UINT i, UINT dir);

UINT GetP1Index(UINT w, UINT h, UINT x, UINT y, UINT dir);
UINT GetP2Index(UINT w, UINT h, UINT x, UINT y, UINT dir);
UINT GetP3Index(UINT w, UINT h, UINT x, UINT y, UINT dir);


INT GetP1(UINT w, UINT h, UINT i, UINT dir, INT *inINT);
INT GetP2(UINT w, UINT h, UINT i, UINT dir, INT *inINT);
INT GetP3(UINT w, UINT h, UINT i, UINT dir, INT *inINT);




VOID ContourTraceTest();
INT CheckSingleton(UINT w, UINT h, UINT i, INT *imgINT);