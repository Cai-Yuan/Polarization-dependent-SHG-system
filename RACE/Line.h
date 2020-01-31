#pragma once

#include "stdafx.h"
#include "Sign.h"

typedef struct FLINESEGMENT_{
	FPOINT ptStart;
	FPOINT ptEnd;
}FLINESEGMENT;

typedef struct FLINESEGMENTSET{
	DWORD size;
	DWORD nSegment;
	FLINESEGMENT *ls;
}FLINESEGMENTSET;

typedef struct LINESEGMENT_{
	POINT ptStart;
	POINT ptEnd;
}LINESEGMENT;

typedef struct LINE_{
	FLOAT A;
	FLOAT B;
	BOOL VERT;
}LINE;

typedef struct PLINE_{
	FLOAT r;
	FLOAT theta;
}PLINE;

DWORD Shift(FLINESEGMENTSET *lss, FLOAT x, FLOAT y);

VOID GetLine(POINT pt1, POINT pt2, LINE *line);
VOID GetLine(INT x1, INT y1, INT x2, INT y2, LINE *ln);
INT GetLinesegmentPoints(LINESEGMENT linesegment, POINT *point);

DWORD Set(FLINESEGMENT *ls, FPOINT ptStart, FPOINT ptEnd);
FLINESEGMENT LineSegment(FPOINT ptStart, FPOINT ptEnd);

DWORD Free(FLINESEGMENTSET *lss);
DWORD Alloc(FLINESEGMENTSET *lss, DWORD length);
DWORD Append(FLINESEGMENTSET *lss, FLINESEGMENT ls);
DWORD Append(FLINESEGMENTSET *lss, FLINESEGMENTSET ls);
DWORD Append(FLINESEGMENTSET *lss, FPOINT ptStart, FPOINT ptEnd);

DWORD AppendLR(FLINESEGMENTSET *lss);
DWORD AppendUR(FLINESEGMENTSET *lss);
DWORD AppendUL(FLINESEGMENTSET *lss);
DWORD AppendLL(FLINESEGMENTSET *lss);
DWORD AppendHorz(FLINESEGMENTSET *lss);
DWORD AppendVert(FLINESEGMENTSET *lss);