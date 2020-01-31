#include "stdafx.h"

#include "Line.h"

DWORD Shift(FLINESEGMENTSET *lss, FLOAT x, FLOAT y)
{
	if(!lss)
		return FALSE;

	DWORD i = 0;
	DWORD n = lss->nSegment;

	if(n > lss->size)
		n = lss->size;

	for(i = 0; i < n; i++){
		lss->ls[i].ptStart = (FPOINT) Shift(lss->ls[i].ptStart, x, y);
		lss->ls[i].ptEnd = (FPOINT) Shift(lss->ls[i].ptEnd, x, y);
	}

	return TRUE;
}

DWORD Free(FLINESEGMENTSET *lss)
{

	if(!lss)
		return FALSE;

	if(lss->ls)
		free(lss->ls);

	lss->ls = NULL;
	lss->nSegment = NULL;
	lss->size = NULL;

	return TRUE;
}

DWORD Append(FLINESEGMENTSET *lss, FLINESEGMENTSET ls)
{
	DWORD i = 0;
	DWORD n = ls.nSegment;

	if(!lss)
		return FALSE;

	for(i = 0; i < n; i++){
		Append(lss, ls.ls[i]);
	}


	return TRUE;
}

DWORD Alloc(FLINESEGMENTSET *lss, DWORD length)
{
	if(!lss)
		return FALSE;

	memset(lss, NULL, sizeof(FLINESEGMENTSET));
	lss->ls = (FLINESEGMENT *)malloc(length*sizeof(FLINESEGMENT));
	
	if(lss->ls){
		lss->size = length;
		lss->nSegment = NULL;
	}

	return TRUE;
}

DWORD Append(FLINESEGMENTSET *lss, FLINESEGMENT ls)
{

	if(!lss)
		return FALSE;

	if(!lss->ls)
		return FALSE;

	if(lss->nSegment >= lss->size)
		return FALSE;

	memcpy(&lss->ls[lss->nSegment], &ls, sizeof(FLINESEGMENT));
	lss->nSegment = lss->nSegment + 1;

	return TRUE;
}

//Appends a line segment, generated from 'ptStart' and 'ptEnd' to 'lss'
DWORD Append(FLINESEGMENTSET *lss, FPOINT ptStart, FPOINT ptEnd)
{
	if(!lss)
		return FALSE;

	FLINESEGMENT ls = LineSegment(ptStart, ptEnd);
	Append(lss, ls);

	return TRUE;
}

FLINESEGMENT LineSegment(FPOINT ptStart, FPOINT ptEnd)
{
	FLINESEGMENT ls = {0};

	memcpy(&ls.ptStart, &ptStart, sizeof(FPOINT));
	memcpy(&ls.ptEnd, &ptEnd, sizeof(FPOINT));

	return ls;
}

DWORD Set(FLINESEGMENT *ls, FPOINT ptStart, FPOINT ptEnd)
{

	if(!ls)
		return FALSE;

	memcpy(&ls->ptEnd, &ptEnd, sizeof(FPOINT));
	memcpy(&ls->ptStart, &ptStart, sizeof(FPOINT));

	return TRUE;
}

VOID GetLine(INT x1, INT y1, INT x2, INT y2, LINE *ln)
{
	FLOAT m;

	if(x2!=x1){
		m = (FLOAT)(y2 - y1)/(FLOAT)(x2 - x1);
		ln->A = m;
		ln->B = y1 - m*x1;
		ln->VERT = FALSE;
	}
	else{
		x1++;
		m = (FLOAT)(y2 - y1)/(FLOAT)(x2 - x1);
		ln->A = m;
		ln->B = y1 - m*x1;;
		ln->VERT = TRUE;
		//MessageBox(NULL, _T("Divide by Zero Error"), _T("Line From Points"), MB_OK);
	}
}

VOID GetLine(POINT pt1, POINT pt2, LINE *line)
{
	FLOAT m;

	if(pt2.x!=pt1.x){
		m = (FLOAT)(pt2.y - pt1.y)/(FLOAT)(pt2.x - pt1.x);
		line->A = m;
		line->B = pt1.y - m*pt1.x;
		line->VERT = FALSE;
	}
	else{
		pt1.x++;
		m = (FLOAT)(pt2.y - pt1.y)/(FLOAT)(pt2.x - pt1.x);
		line->A = m;
		line->B = pt1.y - m*pt1.x;
		line->VERT = TRUE;
		//MessageBox(NULL, _T("Divide by Zero Error"), _T("Line From Points"), MB_OK);
	}
}

INT GetLinesegmentPoints(LINESEGMENT linesegment, POINT *point)
{
	INT nPoint = 0;
	INT sign;
	LINE line;
	INT x, y;

	FLOAT x1 = linesegment.ptStart.x;
	FLOAT y1 = linesegment.ptStart.y;
	FLOAT x2 = linesegment.ptEnd.x;
	FLOAT y2 = linesegment.ptEnd.y;

	FLOAT dx = linesegment.ptEnd.x - linesegment.ptStart.x;
	FLOAT dy = linesegment.ptEnd.y - linesegment.ptStart.y;
	FLOAT dyx = dy/dx;
	FLOAT dxy = dx/dy;

	GetLine(x1, y1, x2, y2, &line);

	if(x1 > x2 && y1 > y2){
		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x > x2; x += sign){
				y = line.A*x + line.B;
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y > y2; y += sign){
				x = (y - line.B)/line.A;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
	}
	else if(x1 < x2 && y1 > y2){

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x < x2; x += sign){
				y = line.A*x + line.B;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y > y2; y += sign){
				x = (y - line.B)/line.A;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
	}
	else if(x1 > x2 && y1 < y2){

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x > x2; x += sign){
				y = line.A*x + line.B;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y < y2; y += sign){
				x = (y - line.B)/line.A;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
	}
	else{

		if(abs(dx) > abs(dy)){
			sign = Sign(dx);
			for(x = x1; x < x2; x += sign){
				y = line.A*x + line.B;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
		else{
			sign = Sign(dy);
			for(y = y1; y < y2; y += sign){
				x = (y - line.B)/line.A;
				//DrawPoint(w, h, img, Point(x, y), colorref);
				point[nPoint].x = x;
				point[nPoint].y = y;
				nPoint++;
			}
		}
	}

	return nPoint;
}