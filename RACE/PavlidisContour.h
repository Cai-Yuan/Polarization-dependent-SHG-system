#pragma once

#include "stdafx.h"
#include "TheoPavlidis.h"

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

DWORD PavlidisContour(UINT w, UINT h, INT *img, INT *contourMap);
DWORD PavlidisContour(UINT w, UINT h, FLOAT *img, FLOAT *contourMap);


//Subfunctions
INT CheckSingleton(UINT w, UINT h, UINT i, FLOAT *img);

FLOAT GetP1(UINT w, UINT h, UINT i, UINT dir, FLOAT *in);
FLOAT GetP2(UINT w, UINT h, UINT i, UINT dir, FLOAT *in);
FLOAT GetP3(UINT w, UINT h, UINT i, UINT dir, FLOAT *in);

FLOAT GetP1(UINT w, UINT h, UINT x, UINT y, UINT dir, FLOAT *in);
FLOAT GetP2(UINT w, UINT h, UINT x, UINT y, UINT dir, FLOAT *in);
FLOAT GetP3(UINT w, UINT h, UINT x, UINT y, UINT dir, FLOAT *in);

//UINT GetP1Index(UINT w, UINT h, UINT i, UINT dir);
//UINT GetP2Index(UINT w, UINT h, UINT i, UINT dir);
//UINT GetP3Index(UINT w, UINT h, UINT i, UINT dir);
//
//UINT GetP1Index(UINT w, UINT h, UINT x, UINT y, UINT dir);
//UINT GetP2Index(UINT w, UINT h, UINT x, UINT y, UINT dir);
//UINT GetP3Index(UINT w, UINT h, UINT x, UINT y, UINT dir);

INT GetEnterDirection(UINT w, UINT h, UINT i, UINT dir, FLOAT *img);

DWORD LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, INT *img, INT *contourMap);
DWORD LabelContourTrace(UINT w, UINT h, UINT i, UINT iContour, FLOAT *img, FLOAT *contourMap);
INT GetDirection(UINT move, INT dir);