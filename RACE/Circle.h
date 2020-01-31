#pragma once

#include "stdafx.h"
//#include "RACEGDI.h"
//#include "GDIPlus.h"
//#include "Gdiplusgraphics.h"

#define PI 3.14159265

typedef struct CIRCLE_{
	UINT r;
	INT x;
	INT y;
}CIRCLE;

CIRCLE Circle(UINT r, INT x, INT y);

CIRCLE GetCircle(UINT r, INT x, INT y);

DWORD DrawCircle(HDC hdc, CIRCLE c, COLORREF cr);
//DWORD DrawCircle(Graphics *g, Pen *pen, CIRCLE c);