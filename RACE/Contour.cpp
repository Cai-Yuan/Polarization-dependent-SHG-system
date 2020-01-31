#include "stdafx.h"

#include "Contour.h"

DWORD GetContour(CONTOUR *c, FLINESEGMENTSET *lss)
{
	if(!c || !lss)
		return FALSE;

	Zero(c);
	
	DWORD i = 0;
	DWORD n = lss->nSegment;
	POINT pt = {0};

	for(i = 0; i < n; i++){
		pt.x = (INT) lss->ls[i].ptStart.x;
		pt.y = (INT) lss->ls[i].ptStart.y;
		Append(c, pt);
		pt.x = (INT) lss->ls[i].ptEnd.x;
		pt.y = (INT) lss->ls[i].ptEnd.y;
		Append(c, pt);
	}


	return TRUE;
}

DWORD Append(CONTOUR *c, POINT pt)
{
	if(!c)
		return FALSE;

	if(c->size > c->nPoint)
		return FALSE;

	if(!c->point)
		return FALSE;

	memcpy(&c->point[c->nPoint], &pt, sizeof(POINT));


	return TRUE;
}

DWORD GetContinuousContourSet(CONTOURSET *cs, FLINESEGMENTSET *lss)
{
	if(!cs||!lss)
		return FALSE;

	

	return TRUE;
}

DWORD Zero(CONTOUR *c)
{
	if(!c)
		return FALSE;

	if(!c->point)
		return FALSE;

	memset(c->point, NULL, c->nPoint*sizeof(POINT));
	c->nPoint = NULL;

	return TRUE;
}

DWORD AllocContourSet(CONTOURSET *cs, DWORD size, DWORD lenContour)
{

	if(!cs)
		return FALSE;

	INT i = NULL;

	memset(cs, NULL, sizeof(CONTOURSET));
	cs->contour = (CONTOUR *)malloc(size*sizeof(CONTOUR));

	for(i = 0; i < size; i++){

		cs->contour[i].point = (POINT *)malloc(lenContour*sizeof(POINT));
		cs->contour[i].nPoint = NULL;
		cs->contour[i].size = lenContour;
	}

	cs->size = size;
	cs->nContour = NULL;


	return TRUE;
}

DWORD Free(CONTOURSET *cs)
{
	if(!cs)
		return FALSE;

	INT i = NULL;

	for(i = 0; i < cs->nContour; i++){
		if(cs->contour[i].point){
			free(cs->contour[i].point);
		}
	}

	if(cs->contour){
		free(cs->contour);
	}


	return TRUE;
}

DWORD Zero(CONTOURSET *cs)
{

	if(!cs){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < cs->size; i++){
		memset(cs->contour[i].point, NULL, cs->contour[i].size*sizeof(POINT));
		cs->contour[i].nPoint = NULL;
	}

	cs->nContour = NULL;

	return TRUE;
}

DWORD DrawContour(UINT w, UINT h, PIXEL value, CONTOUR *c, PIXEL *img)
{
	if(!c || !img)
		return FALSE;

	INT i = NULL;
	INT n = NULL;

	for(n = 0; n < c->nPoint; n++){
		i = w*c->point[n].y + c->point[n].x;
		if(Range(w, h, i)){
			//memcpy(&img[i], &value, sizeof(PIXEL));
			img[i].b = value.b;
			img[i].g = value.g;
			img[i].r = value.r;
			//img[i] = value;
		}
	}

	return TRUE;
}

DWORD DrawContour(UINT w, UINT h, UCHAR value, CONTOUR *c, UCHAR *img)
{
	if(!c || !img)
		return FALSE;

	INT i = NULL;
	INT n = NULL;

	for(n = 0; n < c->nPoint; n++){
		i = w*c->point[n].y + c->point[n].x;
		if(Range(w, h, i)){
			img[i] = value;
		}
	}

	return TRUE;
}

DWORD DrawContourSet(UINT w, UINT h, UCHAR value, CONTOURSET *cs, UCHAR *img)
{
	if(!cs || !img)
		return FALSE;

	INT i = NULL;
	INT n = NULL;

	for(n = 0; n < cs->nContour; n++){
		DrawContour(w, h, value, &cs->contour[n], img);
	}

	return TRUE;
}

DWORD DrawContourSet(UINT w, UINT h, PIXEL value, CONTOURSET *cs, PIXEL *img)
{
	if(!cs || !img)
		return FALSE;

	INT i = NULL;
	INT n = NULL;

	for(n = 0; n < cs->nContour; n++){
		DrawContour(w, h, value, &cs->contour[n], img);
	}

	return TRUE;
}