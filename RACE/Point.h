#pragma once

#include "stdafx.h"
#include "math.h"
#include "Image.h"
#include "PDM.h"

//Type Identifier
#define TI_CHAR		0x0000
#define TI_UCHAR	0x0001
#define TI_INT		0x0002
#define TI_UINT		0x0003
#define TI_FLOAT	0x0004
#define TI_PIXEL	0x0005
#define	TI_POINT	0x0006
#define TI_FPOINT	0x0007
#define TI_EDGE		0x0008
#define TI_FEDGE	0x0009
#define TI_ELLIPSE	0x0010
#define TI_FELLIPSE	0x0011
#define TI_CIRCLE	0x0012
#define TI_FCIRCLE	0x0013
#define TI_LINE		0x0014
#define TI_FLINE	0x0015
#define TI_PPM		0x0016
#define TI_PGM		0x0017
#define TI_PBM		0x0018

POINT Point(INT x, INT y);

//typedef struct POINT_{
//
//	long int x;
//	long int y;
//
//}POINT;

typedef struct DOUBLE_3DPOINT{
	double x;
	double y;
	double z;
}DOUBLE_3DPOINT;

typedef struct POINTSET_3D{
	DWORD nPoint;
	DWORD size;
	DOUBLE_3DPOINT *ptSet;
}POINTSET_3D;

typedef struct P3D{
	double x;
	double y;
	double z;
} P3D;

typedef struct P3DSET{
	DWORD nPoint;
	DWORD size;
	P3D *p;
}P3DSET;


typedef struct FPOINT_{
	FLOAT x;
	FLOAT y;
}FPOINT;

typedef struct POINT3D_{

	FLOAT x;
	FLOAT y;
	FLOAT z;
}POINT3D;

typedef struct POINT_SET_{
	UINT typeID;
	CHAR strTYPE[MAX_STRING];
	UINT nPOINTs;

	POINT *point;
	FPOINT *fpoint;

}POINT_SET;

//typedef struct FPOINTSET_{
//	UINT nPoints;
//	FPOINT *point;//NEW
//	FPOINT *fpoint;
//}FPOINTSET;

typedef struct FPOINTSET_{
	DWORD size;
	DWORD nPoint;
	FPOINT *point;
}FPOINTSET;

typedef struct POINTSET_{
	DWORD size;
	UINT nPoints;
	POINT *point;
}POINTSET;


FPOINT Shift(FPOINT pt, FLOAT x, FLOAT y);

POINT Point(FPOINT fpt);
POINT3D Point3D(FLOAT x, FLOAT y, FLOAT z);

POINT GetPOINTFromIndex(UINT w, UINT h, UINT i);
FPOINT GetFPOINTFromIndex(UINT w, UINT h, UINT i);

VOID DrawPoint(UINT w, UINT h, POINT *point, UCHAR *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, FPOINT *point, UCHAR *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, POINT *point, CHAR *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, FPOINT *point, CHAR *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, POINT *point, INT *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, FPOINT *point, INT *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, POINT *point, UINT *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, FPOINT *point, UINT *bitmap, GREYREF grayref);
VOID DrawPoint(UINT w, UINT h, POINT *point, PIXEL *bitmap, COLORREF colorref);
VOID DrawPoint(UINT w, UINT h, FPOINT *point, PIXEL *bitmap, COLORREF colorref);

FLOAT GetDistance(POINT pt1, POINT pt2);
FLOAT GetDistance(FPOINT pt1, FPOINT pt2);
FPOINT FPoint(FLOAT x, FLOAT y);
FPOINT FPoint(POINT pt);
//FPOINT Point(FLOAT x, FLOAT y);

UINT GetNumberPoint(FPOINTSET pointSet);
UINT GetCountPoint(FPOINTSET pointSet);

VOID GetPointFromPointSet(FPOINTSET pointSet, FPOINT *point);

DWORD AllocPointSet(POINTSET *ps, DWORD size);
DWORD AppendPoint(POINTSET *ps, POINT pt);


DWORD Alloc(FPOINTSET *ps, DWORD size);
DWORD Transfer(FPOINTSET *psDest, FPOINTSET *psSrc);
DWORD Push(FPOINTSET *ps, FPOINT pt);
DWORD Free(FPOINTSET *ps);
DWORD Empty(FPOINTSET *ps);
DWORD Copy(FPOINTSET *psDest, FPOINTSET *psSrc);

DWORD AreSame(FPOINT p1, FPOINT p2);

FLOAT GetMaximumDistance(FPOINTSET &fps1, FPOINTSET &fps2);
FLOAT GetMinimumDistance(FPOINTSET &fps1, FPOINTSET &fps2);


DWORD Alloc(P3DSET *ps, DWORD size);
DWORD Append(P3DSET *ps, P3D p);
DWORD Remove(P3DSET *ps, DWORD iPoint);


DWORD Copy(P3D *pDest, P3D pSrc);
DWORD Copy(P3D *pDest, DOUBLE_3DPOINT pSrc);

