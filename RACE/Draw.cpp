#include "stdafx.h"

#include "Draw.h"
#include "Polygon.h"

VOID DrawRect(UINT w, UINT h, PIXEL *pixel, RECT rect, COLORREF colorref)
{
	FLINESEGMENT lsLeft;
	FLINESEGMENT lsRight;
	FLINESEGMENT lsTop;
	FLINESEGMENT lsBottom;

	lsLeft = LineSegment(FPoint(rect.left, rect.bottom), FPoint(rect.left, rect.top));
	lsTop = LineSegment(FPoint(rect.left, rect.top), FPoint(rect.right, rect.top));
	lsRight = LineSegment(FPoint(rect.right, rect.top), FPoint(rect.right, rect.bottom));
	lsBottom = LineSegment(FPoint(rect.right, rect.bottom), FPoint(rect.left, rect.bottom));

	DrawFLineSegment(w, h, lsLeft, pixel, colorref);
	DrawFLineSegment(w, h, lsTop, pixel, colorref);
	DrawFLineSegment(w, h, lsRight, pixel, colorref);
	DrawFLineSegment(w, h, lsBottom, pixel, colorref);

}

VOID DrawVector(UINT w, UINT h, PIXEL *pixel, VECTOR vector, COLORREF colorref)
{
	FLINESEGMENT lineseg;

	memcpy(&lineseg.ptStart, &vector.pTail, sizeof(FPOINT));
	memcpy(&lineseg.ptEnd, &vector.pHead, sizeof(FPOINT));

	//lineseg.ptEnd.x = vector.pTail.x + vector.i;
	//lineseg.ptEnd.y = vector.pTail.y + vector.j;

	DrawFLineSegment(w, h, lineseg, pixel, colorref);
}

VOID DrawVerticalLineSegment(UINT w, UINT h, FLINESEGMENT ls, PIXEL *bitmap, COLORREF cr)
{
	INT y;
	FLOAT yMax = MaxFloat(ls.ptStart.y, ls.ptEnd.y);
	FLOAT yMin = MinFloat(ls.ptStart.y, ls.ptEnd.y);

	for(y = yMin; y < yMax; y++){
		DrawPoint(w, h, &Point((INT)ls.ptStart.x, (INT)y), bitmap, cr);
	}
}

VOID DrawHorizontalLineSegment(UINT w, UINT h, FLINESEGMENT ls, PIXEL *bitmap, COLORREF cr)
{
	INT x;
	FLOAT xMax = MaxFloat(ls.ptStart.x, ls.ptEnd.x);
	FLOAT xMin = MinFloat(ls.ptStart.x, ls.ptEnd.x);
	
	for(x = xMin; x < xMax; x++){
		DrawPoint(w, h, &Point((INT)x, ls.ptStart.y), bitmap, cr);
	}
}


VOID DrawFLineSegment(UINT w, UINT h, FLINESEGMENT lineseg, PIXEL *bitmap, COLORREF colorref)
{
	INT i;
	INT d;
	INT r, c;
	INT x, y;

	INT sign;

	FLOAT x1 = lineseg.ptStart.x;
	FLOAT y1 = lineseg.ptStart.y;
	FLOAT x2 = lineseg.ptEnd.x;
	FLOAT y2 = lineseg.ptEnd.y;

	FLOAT dx = lineseg.ptEnd.x  - lineseg.ptStart.x;
	FLOAT dy = lineseg.ptEnd.y - lineseg.ptStart.y;
	FLOAT dyx = dy/dx;
	FLOAT dxy = dx/dy;

	if(dx == 0.0){
		DrawVerticalLineSegment(w, h, lineseg, bitmap, colorref);
		return;
	}

	if(dy == 0.0){
		DrawHorizontalLineSegment(w, h, lineseg, bitmap, colorref);
		return;
	}

	LINE line;
	
	GetLine(x1, y1, x2, y2, &line);

	if(x1 > x2 && y1 > y2){
		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x > x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y > y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
	}
	else if(x1 < x2 && y1 > y2){

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x < x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y > y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
	}
	else if(x1 > x2 && y1 < y2){

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x > x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y < y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
	}
	else{
		if(!line.A){
			return;
		}
		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x < x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
		else{

			sign = Sign(dy);
			for(y = y1; y < y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, colorref);
			}
		}
	}


}

VOID DrawCircle(UINT width, UINT height, CIRCLE circle, PIXEL *img, COLORREF val)
{

	static BOOL FILL = TRUE;
	static FLOAT *sin_rad = NULL;
	static FLOAT *cos_rad = NULL;
	static FLOAT two_PI = 2 * PI;
	static FLOAT rad_inc = 0;
	static FLOAT rad = 0;
	static UINT rad_tot = 0;
	UINT row, col, i, c;
	static UINT radius = 0;
	POINT p;

	if(FILL){
		rad_inc = PI / (500);
		rad_tot = (UINT) ((FLOAT)(two_PI) / (FLOAT) rad_inc);
		sin_rad = (FLOAT *)malloc(sizeof(FLOAT) * rad_tot);
		cos_rad = (FLOAT *)malloc(sizeof(FLOAT) * rad_tot);
		for(rad = 0, i = 0; rad < two_PI, i < rad_tot; rad += rad_inc, i++){
			sin_rad[i] = sin(rad);
			cos_rad[i] = cos(rad);
		}
		FILL = FALSE;
	}

	for(c = 0; c < rad_tot; c++){
		p.x = (circle.r * cos_rad[c] + circle.x) + 1;	//'+1' for zero indexed pixel locations
		p.y = (circle.r * sin_rad[c] + circle.y) + 1;
		if(Range(width, height, p.x, p.y)){
			i = (p.y * width) + p.x;
			img[i].b = GetBValue(val);
			img[i].g = GetGValue(val);
			img[i].r = GetRValue(val);
		}
	}
}

VOID DrawCross(UINT w, UINT h, PIXEL *img, POINT pt, COLORREF colorref, UINT size)
{
	UINT i;
	UINT d;
	UINT r, c;
	UINT x, y;

	//Vertical Component
	c = pt.x;
	for(r = (pt.y-size); r < (pt.y+size+1); r++)
	{
		i = (r*w) + c;
		if(Range(w, h, i)){
			d = abs((FLOAT)(pt.y - r));

			img[i].r = GetBValue(colorref);
			img[i].g = GetGValue(colorref);
			img[i].b = GetRValue(colorref);
		}
	}

	r = pt.y;
	//Horizontal Component
	for(c = (pt.x-size); c < (pt.x+size+1); c++){
		i = (r*w) + c;
		if(Range(w, h, i)){
			d = abs((FLOAT)(pt.x - c));

			img[i].r = GetBValue(colorref);
			img[i].g = GetGValue(colorref);
			img[i].b = GetRValue(colorref);
		}
	}
}

VOID DrawCross(UINT w, UINT h, UCHAR *img, POINT pt, UCHAR value, UINT size)
{
	UINT i;
	UINT d;
	UINT r, c;
	UINT x, y;

	//Vertical Component
	c = pt.x;
	for(r = (pt.y-size); r < (pt.y+size+1); r++)
	{
		i = (r*w) + c;
		d = abs((FLOAT)(pt.y - r));

		img[i] = value;
	}

	r = pt.y;
	//Horizontal Component
	for(c = (pt.x-size); c < (pt.x+size+1); c++){
		i = (r*w) + c;
		d = abs((FLOAT)(pt.x - c));

		img[i] = value;
	}
}

VOID DrawEllipse(UINT w, UINT h, PIXEL *img, ELLIPSE eli, COLORREF color)
{
	UINT i;
	//UINT w = width;
	//UINT h = height;

	FLOAT theta, x, y, PIPI;
	FLOAT cost, cosa, sint, sina;


	PIPI = 2*PI;
	cosa = cos(eli.alpha);
	sina = sin(eli.alpha);



	for(theta = 0; theta < PIPI; theta+=0.01)
	{
		cost = cos(theta);
		sint = sin(theta);
		x = eli.a*cost*cosa - eli.b*sint*sina;
		y = eli.a*cost*sina + eli.b*sint*cosa;
		x += eli.x;
		y += eli.y;
		if(Range(w, h, x, y)){
			i = ((INT)y * w) + (INT)x;
			img[i].b = GetRValue(color);
			img[i].g = GetGValue(color);
			img[i].r = GetBValue(color);
		}
		else
		{
			i = 0;
		}
	}
}

void DrawEllipse(UINT w, UINT h, PIXEL *img, FELLIPSE eli, COLORREF color)
{
	UINT i;
	//UINT w = width;
	//UINT h = height;

	FLOAT theta, x, y, PIPI;
	FLOAT cost, cosa, sint, sina;


	PIPI = 2*PI;
	cosa = cos(eli.alpha);
	sina = sin(eli.alpha);

	//x = eli.a*cos(theta)*cos(eli.alpha) - eli.b*sin(theta)*sin(eli.alpha);
	//y = eli.a*cos(theta)*sin(eli.alpha) + eli.b*sin(theta)*cos(eli.alpha);

	for(theta = 0; theta < PIPI; theta+=0.01)
	{
		cost = cos(theta);
		sint = sin(theta);
		x = eli.a*cost*cosa - eli.b*sint*sina;
		y = eli.a*cost*sina + eli.b*sint*cosa;
		x += eli.x/*+1*/;
		y += eli.y/*+1*/;
		if(Range(w, h, x, y)){
			i = ((INT)y * w) + (INT)x;
			img[i].b = GetRValue(color);
			img[i].g = GetGValue(color);
			img[i].r = GetBValue(color);
		}
		else
		{
			i = 0;
		}
	}
}

VOID DrawEllipse(UINT w, UINT h, UCHAR *img, FELLIPSE eli, UCHAR value)
{
	UINT i;
	//UINT w = width;
	//UINT h = height;

	FLOAT theta, x, y, PIPI;
	FLOAT cost, cosa, sint, sina;


	PIPI = 2*PI;
	cosa = cos(eli.alpha);
	sina = sin(eli.alpha);

	//x = eli.a*cos(theta)*cos(eli.alpha) - eli.b*sin(theta)*sin(eli.alpha);
	//y = eli.a*cos(theta)*sin(eli.alpha) + eli.b*sin(theta)*cos(eli.alpha);

	for(theta = 0; theta < PIPI; theta+=0.01)
	{
		cost = cos(theta);
		sint = sin(theta);
		x = eli.a*cost*cosa - eli.b*sint*sina;
		y = eli.a*cost*sina + eli.b*sint*cosa;
		x += eli.x;
		y += eli.y;
		if(Range(w, h, x, y)){
			i = ((INT)y * w) + (INT)x;
			img[i] = value;
		}
		else
		{
			i = 0;
		}
	}
}

VOID DrawEllipses(UINT w, UINT h, UINT nEllipse, PIXEL *img, FELLIPSE *ellipse, COLORREF color)
{

	UINT i; 
	for(i = 0; i < nEllipse; i++){
		DrawEllipse(w, h, img, ellipse[i], color);
	}

}

VOID DrawLineSegment(UINT w, UINT h, LINESEGMENT lineseg, UCHAR *bitmap, GREYREF greyref)
{
	INT i;
	INT d;
	INT r, c;
	INT x, y;

	INT sign;

	FLOAT x1 = lineseg.ptStart.x;
	FLOAT y1 = lineseg.ptStart.y;
	FLOAT x2 = lineseg.ptEnd.x;
	FLOAT y2 = lineseg.ptEnd.y;

	FLOAT dx = lineseg.ptEnd.x  - lineseg.ptStart.x;
	FLOAT dy = lineseg.ptEnd.y - lineseg.ptStart.y;
	FLOAT dyx = dy/dx;
	FLOAT dxy = dx/dy;

	LINE line;
	
	GetLine(x1, y1, x2, y2, &line);

	if(x1 > x2 && y1 > y2){
		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x > x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y > y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
	}
	else if(x1 < x2 && y1 > y2){

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x < x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y > y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
	}
	else if(x1 > x2 && y1 < y2){

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x > x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y < y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
	}
	else{

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x < x2; x += sign){
				y = line.A*x + line.B;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y < y2; y += sign){
				x = (y - line.B)/line.A;
				DrawPoint(w, h, &Point(x, y), bitmap, greyref);
			}
		}
	}


}

VOID DrawPolygon(UINT w, UINT h, POLYGON *polygon, UCHAR *bitmap, GREYREF greyref)
{
	UINT i;
	UINT n;
	UINT nVertex = polygon->nVertex-1;

	if(nVertex < 0)
		nVertex = 0;

	LINESEGMENT lineseg;

	//Make 0th Point the Starting Position for Drawing
	memcpy(&lineseg.ptStart, &Point(polygon->vertex[0].x, polygon->vertex[0].y/*polygon->point[0]*/), sizeof(POINT));

	for(n = 1; n < polygon->nVertex; n++)
	{
		DrawCross(w, h, bitmap, Point(polygon->vertex[n].x, polygon->vertex[n].y), greyref, 2);
		memcpy(&lineseg.ptEnd, &Point(polygon->vertex[n].x, polygon->vertex[n].y), sizeof(POINT));

		DrawLineSegment(w, h, lineseg, bitmap, greyref);

		memcpy(&lineseg.ptStart, &lineseg.ptEnd, sizeof(POINT));

	}

	memcpy(&lineseg.ptEnd,  &Point(polygon->vertex[0].x-1, polygon->vertex[0].y), sizeof(POINT));

	DrawLineSegment(w, h, lineseg, bitmap, greyref);
}

//VOID DrawPolygon(UINT w, UINT h, POLYGON *polygon, PIXEL *bitmap, COLORREF colorref)
//{
//	UINT i;
//	UINT n;
//	UINT nVertex = polygon->nVertex-1;
//
//	LINESEGMENT lineseg;
//
//	//Make 0th Point the Starting Position for Drawing
//	memcpy(&lineseg.ptStart, &Point(polygon->point[0].x, polygon->point[0].y)/*&polygon->point[0]*/, sizeof(POINT));
//	
//
//	for(n = 1; n < polygon->nPoint; n++)
//	{
//		memcpy(&lineseg.ptEnd, &Point(polygon->point[n].x, polygon->point[n].y)/*&polygon->point[n]*/, sizeof(POINT));
//
//		DrawLineSegment(w, h, lineseg, bitmap, colorref);
//
//		memcpy(&lineseg.ptStart, &lineseg.ptEnd, sizeof(POINT));
//
//	}
//
//	memcpy(&lineseg.ptEnd, &Point(polygon->point[0].x, polygon->point[0].y)/*&polygon->point[polygon->nPoint]*/, sizeof(POINT));
//
//	DrawLineSegment(w, h, lineseg, bitmap, colorref);
//}