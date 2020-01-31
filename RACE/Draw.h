#pragma once

#include "stdafx.h"
#include "PDM.h"
#include "Point.h"
#include "Line.h"
#include "Vector.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Range.h"
#include "Polygon.h"


VOID DrawRect(UINT w, UINT h, PIXEL *pixel, RECT rect, COLORREF colorref);
VOID DrawVector(UINT w, UINT h, PIXEL *pixel, VECTOR vector, COLORREF colorref);
VOID DrawFLineSegment(UINT w, UINT h, FLINESEGMENT lineseg, PIXEL *bitmap, COLORREF colorref);
VOID DrawCircle(UINT width, UINT height, CIRCLE circle, PIXEL *img, COLORREF val);
VOID DrawCross(UINT w, UINT h, PIXEL *img, POINT pt, COLORREF colorref, UINT size);
VOID DrawCross(UINT w, UINT h, UCHAR *img, POINT pt, UCHAR value, UINT size);

VOID DrawEllipse(UINT w, UINT h, UCHAR *img, FELLIPSE eli, UCHAR value);
VOID DrawEllipse(UINT w, UINT h, PIXEL *img, ELLIPSE eli, COLORREF color);
VOID DrawEllipse(UINT w, UINT h, PIXEL *img, FELLIPSE eli, COLORREF color);
VOID DrawEllipses(UINT w, UINT h, UINT nEllipse, PIXEL *img, FELLIPSE *ellipse, COLORREF color);
VOID DrawLineSegment(UINT w, UINT h, LINESEGMENT lineseg, UCHAR *bitmap, GREYREF greyref);
VOID DrawPolygon(UINT w, UINT h, POLYGON *polygon, UCHAR *bitmap, GREYREF greyref);
VOID DrawPolygon(UINT w, UINT h, POLYGON *polygon, PIXEL *bitmap, COLORREF colorref);
