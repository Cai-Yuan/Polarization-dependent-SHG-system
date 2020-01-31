#pragma once

#include "stdafx.h"	


//No Contour
#define CW_1111		15
#define CW_0000		0

//Single Segment Points
#define CW_1110		14
#define CW_1101		13
#define CW_1011		11
#define CW_0111		7
#define CW_0001		1
#define CW_0010		2
#define CW_0100		4
#define CW_1000		8
#define CW_1100		12
#define CW_1001		9
#define CW_0011		3
#define CW_0110		6

//Two-Segment Saddle Points
#define CW_1010		10
#define CW_0101		5

#define MASK_0001	1
#define MASK_0010	2
#define MASK_0100	4
#define MASK_1000	8


DWORD MarchingSquares(INT w, INT h, UCHAR *imgBinary, FLINESEGMENTSET *lss);
DWORD MarchingSquares (INT w, INT h, UCHAR threshold, UCHAR *img, CONTOURSET *cs);


UCHAR GetCellMask(UINT r, UINT c);
DWORD GetLineSegments(FLINESEGMENTSET *lss, UCHAR mask);
DWORD AppendLR(FLINESEGMENTSET *lss);
DWORD AppendUR(FLINESEGMENTSET *lss);
DWORD AppendUL(FLINESEGMENTSET *lss);
DWORD AppendLL(FLINESEGMENTSET *lss);
DWORD AppendHorz(FLINESEGMENTSET *lss);
DWORD AppendVert(FLINESEGMENTSET *lss);

UCHAR GetPixelValueUL(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary);
UCHAR GetPixelValueUR(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary);
UCHAR GetPixelValueLL(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary);
UCHAR GetPixelValueLR(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary);

UCHAR SetHigh(UCHAR value, UCHAR x);
UCHAR SetLow(UCHAR value, UCHAR x);
UCHAR SetBit(UCHAR value, UCHAR x, UCHAR state);


UCHAR GetMask(UCHAR pUL, UCHAR pUR, UCHAR pLL, UCHAR pLR);
DWORD GetLineSegments(FLINESEGMENTSET *lss, UCHAR mask);