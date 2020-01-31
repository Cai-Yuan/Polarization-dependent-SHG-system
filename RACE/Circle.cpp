#include "stdafx.h"

#include "Circle.h"
#include "RACEGDI.h"

//#include "GDIPlus.h"
//#include "Gdiplusgraphics.h"
using namespace Gdiplus;
//#pragma comment(lib, "Gdiplus.lib")

DWORD DrawCircle(Graphics *g, Pen *pen, CIRCLE c);

CIRCLE GetCircle(UINT r, INT x, INT y)
{
	CIRCLE circle = {0};

	circle.r = r;
	circle.x = x;
	circle.y = y;

	return circle;
}

DWORD DrawCircle(HDC hdc, CIRCLE c, COLORREF cr)
{

	RECT r = {0};

	r.left = c.x-c.r;
	r.right = c.x+c.r;
	r.top = c.y-c.r;
	r.bottom = c.y+c.r;

	HGDIOBJ hdcOld = NULL;
	HPEN hdcPen = CreatePen(PS_DASH, 1, cr);

	hdcOld = SelectObject(hdc, (HGDIOBJ) hdcPen);
	//SelectObject(hdc, GetStockObject(DC_PEN));
	SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	//SetDCPenColor(hdc, cr);
	Ellipse(hdc, r.left, r.top, r.right, r.bottom);

	SelectObject(hdc, hdcOld);
	DeleteObject(hdcPen);

	return TRUE;
}

DWORD DrawCircle(Graphics *g, Pen *pen, CIRCLE c)
{

	RECT r = {0};

	INT w = c.r*2;
	INT h = c.r*2;

	r.left = c.x-c.r;
	r.right = c.x+c.r;
	r.top = c.y-c.r;
	r.bottom = c.y+c.r;

	g->DrawEllipse(pen, Gdiplus::RectF(r.left, r.top, w, h));


	return TRUE;
}

CIRCLE Circle(UINT r, INT x, INT y)
{
	CIRCLE c;
	c.x = x;
	c.y = y;
	c.r = r;
	return c;
}