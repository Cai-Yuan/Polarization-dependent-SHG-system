#include "stdafx.h"

#include "Point.h"
#include "Range.h"


VOID DrawPoint(UINT w, UINT h, POINT *point, UCHAR *bitmap, GREYREF grayref)
{
	UINT i;
	if(Range(w, h, point->x, point->y)){
		i = point->y*w + point->x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, FPOINT *point, UCHAR *bitmap, GREYREF grayref)
{
	UINT i;
	INT x = floor(point->x);
	INT y = floor(point->y);

	if(Range(w, h, x, y)){
		i = y*w + x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, POINT *point, CHAR *bitmap, GREYREF grayref)
{
	UINT i;
	if(Range(w, h, point->x, point->y)){
		i = point->y*w + point->x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, FPOINT *point, CHAR *bitmap, GREYREF grayref)
{
	UINT i;
	INT x = floor(point->x);
	INT y = floor(point->y);

	if(Range(w, h, x, y)){
		i = y*w + x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, POINT *point, INT *bitmap, GREYREF grayref)
{
	UINT i;
	if(Range(w, h, point->x, point->y)){
		i = point->y*w + point->x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, FPOINT *point, INT *bitmap, GREYREF grayref)
{
	UINT i;
	INT x = floor(point->x);
	INT y = floor(point->y);

	if(Range(w, h, x, y)){
		i = y*w + x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, POINT *point, UINT *bitmap, GREYREF grayref)
{
	UINT i;
	if(Range(w, h, point->x, point->y)){
		i = point->y*w + point->x;
		bitmap[i] = grayref;
	}
}

VOID DrawPoint(UINT w, UINT h, FPOINT *point, UINT *bitmap, GREYREF grayref)
{
	UINT i;
	INT x = floor(point->x);
	INT y = floor(point->y);

	if(Range(w, h, x, y)){
		i = y*w + x;
		bitmap[i] = grayref;
	}
}


VOID DrawPoint(UINT w, UINT h, POINT *point, PIXEL *bitmap, COLORREF colorref)
{
	UINT i;
	if(Range(w, h, point->x, point->y)){
		i = point->y*w + point->x;
		bitmap[i].b = GetRValue(colorref);
		bitmap[i].g = GetGValue(colorref);
		bitmap[i].r = GetBValue(colorref);
	}
}

VOID DrawPoint(UINT w, UINT h, FPOINT *point, PIXEL *bitmap, COLORREF colorref)
{
	UINT i;
	INT x = floor(point->x);
	INT y = floor(point->y);
	if(Range(w, h, x, y)){
		i = floor(point->y)*w + floor(point->x);
		bitmap[i].b = GetRValue(colorref);
		bitmap[i].g = GetGValue(colorref);
		bitmap[i].r = GetBValue(colorref);
	}
}
