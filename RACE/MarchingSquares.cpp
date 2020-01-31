#include "stdafx.h"

#include "MarchingSquares.h"
#include "Contour.h"
#include "Binarize.h"
#include "Line.h"


DWORD MarchingSquares (INT w, INT h, UCHAR threshold, UCHAR *img, CONTOURSET *cs)
{
	if(!img||!img){
		return FALSE;
	}

	DWORD nCells = w*h;
	DWORD wCorners = w+1;
	DWORD hCorners = h+1;

	UCHAR mask = 0;

	Threshold(w, h, threshold, img);
	Binarize(w, h, img);

	FLINESEGMENTSET lss = {0};
	Alloc(&lss, w*h*2);

	MarchingSquares(w, h, img, &lss);
	GetContour(&cs->contour[0], &lss);

	Free(&lss);

	return TRUE;
}

DWORD MarchingSquares(INT w, INT h, UCHAR *imgBinary, FLINESEGMENTSET *lss)
{
	if(!imgBinary || !lss)
		return FALSE;

	DWORD i = 0;
	DWORD n = 0;
	INT x = 0;
	INT y = 0;

	UCHAR mask = 0;

	UCHAR pUL = 0;
	UCHAR pUR = 0;
	UCHAR pLL = 0;
	UCHAR pLR = 0;

	FLINESEGMENTSET lssTemp = {0};
	Alloc(&lssTemp, 3);

	for(y = 0; y < h-1; y++){
		for(x = 0; x < w-1; x++){
			pUL = GetPixelValueUL(w, h, x, y, imgBinary);
			pUR = GetPixelValueUR(w, h, x, y, imgBinary);
			pLL = GetPixelValueLL(w, h, x, y, imgBinary);
			pLR = GetPixelValueLR(w, h, x, y, imgBinary);
			mask = GetMask(pUL, pUR, pLL, pLR);
			GetLineSegments(&lssTemp, mask);
			Shift(&lssTemp, x, y);
			Append(lss, lssTemp);

		}
	}

	Free(&lssTemp);
	return TRUE;
}

UCHAR GetMask(UCHAR pUL, UCHAR pUR, UCHAR pLL, UCHAR pLR)
{
	UCHAR mask = 0;

	mask = SetBit(mask, 0, pUL);
	mask = SetBit(mask, 1, pUR);
	mask = SetBit(mask, 2, pLR);
	mask = SetBit(mask, 3, pLL);

	return mask;
}

//Will Set bit 'x' high
UCHAR SetHigh(UCHAR value, UCHAR x)
{

	return value |= 1<<x;

}

//Will Set bit 'x' low
UCHAR SetLow(UCHAR value, UCHAR x)
{
	return value &= ~(1<<x);
}

//Will Set bit 'x' to 'state'
UCHAR SetBit(UCHAR value, UCHAR x, UCHAR state)
{
	if(state)
		return SetHigh(value, x);
	else
		return SetLow(value, x);

	return TRUE;
}


UCHAR GetPixelValueUL(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary)
{
	INT i = yCell*h + xCell;
	return imgBinary[i];
}

UCHAR GetPixelValueUR(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary)
{
	INT i = yCell*h + (xCell+1);
	return imgBinary[i];
}

UCHAR GetPixelValueLL(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary)
{
	INT i = (yCell+1)*h + (xCell);
	return imgBinary[i];
}

UCHAR GetPixelValueLR(UINT w, UINT h, INT xCell, INT yCell, UCHAR *imgBinary)
{
	INT i = (yCell+1)*h + (xCell+1);
	return imgBinary[i];
}


UCHAR GetCellMask(UINT r, UINT c)
{
	UCHAR mask = 0;

	return mask;
}

DWORD GetLineSegments(FLINESEGMENTSET *lss, UCHAR mask)
{
	if(!lss)
		return FALSE;

	DWORD clas = 0;
	FPOINT ptStart = {0};
	FPOINT ptEnd = {0};
	FLINESEGMENT ls = {0};

	switch(mask)
	{
	case CW_0000:
		return FALSE;
	case CW_0001:
		AppendLL(lss);
		break;
	case CW_0010:
		AppendLR(lss);
		break;
	case CW_0011:
		AppendHorz(lss);
		break;
	case CW_0100:
		AppendUR(lss);
		break;
	case CW_0101:
		AppendUL(lss);
		AppendLR(lss);
		break;
	case CW_0110:
		AppendVert(lss);
		break;
	case CW_0111:
		AppendUL(lss);
		break;
	case CW_1000:
		AppendUL(lss);
		break;
	case CW_1001:
		AppendVert(lss);
		break;
	case CW_1010:
		AppendLL(lss);
		AppendUR(lss);
		break;
	case CW_1011:
		AppendUR(lss);
		break;
	case CW_1100:
		AppendHorz(lss);
		break;
	case CW_1101:
		AppendLR(lss);
		break;
	case CW_1110:
		AppendLL(lss);
		break;
	case CW_1111:
		return FALSE;
	default:
		break;
	}

	return TRUE;
}

DWORD AppendLR(FLINESEGMENTSET *lss)
{
	if(!lss)
		return FALSE;

	FPOINT ptStart = FPoint(1.0, 0.5);
	FPOINT ptEnd = FPoint(0.5, 1.0);
	Append(lss, ptStart, ptEnd);

	return TRUE;
}

DWORD AppendUR(FLINESEGMENTSET *lss)
{
	if(!lss)
		return FALSE;

	FPOINT ptStart = FPoint(0.5, 0.0);
	FPOINT ptEnd = FPoint(1.0, 0.5);
	Append(lss, ptStart, ptEnd);

	return TRUE;
}

DWORD AppendUL(FLINESEGMENTSET *lss)
{
	if(!lss)
		return FALSE;

	FPOINT ptStart = FPoint(0.0, 0.5);
	FPOINT ptEnd = FPoint(0.5, 0.0);
	Append(lss, ptStart, ptEnd);

	return TRUE;
}


DWORD AppendLL(FLINESEGMENTSET *lss)
{
	if(!lss)
		return FALSE;

	FPOINT ptStart = FPoint(0.5, 1.0);
	FPOINT ptEnd = FPoint(0.0, 0.5);
	Append(lss, ptStart, ptEnd);

	return TRUE;
}

DWORD AppendHorz(FLINESEGMENTSET *lss)
{
	if(!lss)
		return FALSE;

	FPOINT ptStart = FPoint(0.0, 0.5);
	FPOINT ptEnd = FPoint(1.0, 0.5);
	Append(lss, ptStart, ptEnd);

	return TRUE;

}

DWORD AppendVert(FLINESEGMENTSET *lss)
{
	if(!lss)
		return FALSE;

	FPOINT ptStart = FPoint(0.5, 0.0);
	FPOINT ptEnd = FPoint(0.5, 1.0);
	Append(lss, ptStart, ptEnd);

	return TRUE;

}

//DWORD GetLineSegment(FLINESEGMENTSET *lss, UCHAR mask)
//{
//	if(!lss)
//		return FALSE;
//
//	DWORD clas = 0;
//	FPOINT ptStart = {0};
//	FPOINT ptEnd = {0};
//	FLINESEGMENT ls = {0};
//
//	switch(mask)
//	{
//	case CW_0000:
//		return FALSE;
//	case CW_0001:
//		ptStart = FPoint(0.0, 0.5);
//		ptEnd = FPoint(0.5, 1.0);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_0010:
//		ptStart = FPoint(0.5, 1);
//		ptEnd = FPoint(1.0, 0.5);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_0011:
//		ptStart = FPoint(0.0, 0.5);
//		ptEnd = FPoint(1.0, 0.5);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_0100:
//		ptStart = FPoint(0.5, 0.0);
//		ptEnd = FPoint(1.0, 0.5);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_0101:
//		ptStart = FPoint(0.0, 0.5);
//		ptEnd = FPoint(0.5, 0.0);
//		Append(lss, ptStart, ptEnd);
//		ptStart = FPoint(0.5, 1.0);
//		ptEnd = FPoint(1.0, 0.5);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_0110:
//		ptStart = FPoint(0.5, 0.0);
//		ptEnd = FPoint(0.5, 1.0);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_0111:
//		ptStart = FPoint(0.0, 0.5);
//		ptEnd = FPoint(0.5, 0.0);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_1000:
//		ptStart = FPoint(0.0, 0.5);
//		ptEnd = FPoint(0.5, 0.0);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_1001:
//		ptStart = FPoint(0.5, 0.0);
//		ptEnd = FPoint(0.5, 1.0);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_1010:
//		ptStart = FPoint(0.5, 0.0);
//		ptEnd = FPoint(1.0, 0.5);
//		Append(lss, ptStart, ptEnd);
//		ptStart = FPoint(0.5, 1.0);
//		ptEnd = FPoint(0.0, 0.5);
//		Append(lss, ptStart, ptEnd);
//		break;
//	case CW_1011:
//		ptStart = FPoint(0.5, 0.0);
//		ptEnd = FPoint(1.0, 0.5);
//		Apppend(lss, ptStart, ptEnd);
//		break;
//	case CW_1100:
//		ptStart = FPoint(
//		break;
//	case CW_1101:
//		break;
//	case CW_1110:
//		break;
//	case CW_1111:
//		return FALSE;
//	default:
//		break;
//	}
//
//	return clas;
//}

//DWORD ClassifyCellMask(UCHAR mask)
//{
//	DWORD clas = 0;
//
//	switch(mask)
//	{
//	case CW_0000:
//		break;
//	case CW_0001:
//		break;
//	case CW_0010:
//		break;
//	case CW_0011:
//		break;
//	case CW_0100:
//		break;
//	case CW_0101:
//		break;
//	case CW_0110:
//		break;
//	case CW_0111:
//		break;
//	case CW_1000:
//		break;
//	case CW_1001:
//		break;
//	case CW_1010:
//		break;
//	case CW_1011:
//		break;
//	case CW_1100:
//		break;
//	case CW_1101:
//		break;
//	case CW_1110:
//		break;
//	case CW_1111:
//		break;
//	default:
//		break;
//	}
//
//	return clas;
//}