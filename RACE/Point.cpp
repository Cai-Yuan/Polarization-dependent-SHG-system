#include "stdafx.h"

#include "Point.h"

FPOINT Shift(FPOINT pt, FLOAT x, FLOAT y)
{
	pt.x = pt.x+x;
	pt.y = pt.y+y;

	return pt;
}

POINT Point(FPOINT fpt)
{
	POINT pt = {0};
	pt.x = (INT) fpt.x;
	pt.y = (INT) fpt.y;

	return pt;
}

FPOINT FPoint(POINT pt)
{
	FPOINT fpt = {0};
	fpt.x = (FLOAT) pt.x;
	fpt.y = (FLOAT) pt.y;

	return fpt;
}

POINT Point(INT x, INT y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	return pt;
}

FPOINT Point(FLOAT x, FLOAT y)
{
	FPOINT pt;
	pt.x = x;
	pt.y = y;
	return pt;
}

POINT GetPOINTFromIndex(UINT w, UINT h, UINT i)
{
	POINT pt;
	DOUBLE xy = ((DOUBLE)i/(DOUBLE)w);
	DOUBLE x;
	DOUBLE xINT;

	pt.y = (LONG) floor(xy);
	x = (((xy)-pt.y)*w);
	x = modf(x, &xINT);
	if(x >= 0.5)
		pt.x =(LONG) (xINT+1);
	else
		pt.x =(LONG) (xINT);

	return pt;
}

FPOINT GetFPOINTFromIndex(UINT w, UINT h, UINT i)
{
	FPOINT pt;
	pt.y = (FLOAT) (floor((DOUBLE)i/(DOUBLE)w));
	pt.x = (FLOAT) ((((DOUBLE)i/(DOUBLE)w)-pt.y)*w);

	return pt;
}

FLOAT GetDistance(POINT pt1, POINT pt2){

	FLOAT d;
	FLOAT x = (FLOAT)pt2.x - (FLOAT)pt1.x;
	FLOAT y = (FLOAT)pt2.y - (FLOAT)pt1.y;

	d = sqrt(x*x + y*y);
	return d;
}
FLOAT GetDistance(FPOINT pt1, FPOINT pt2){

	FLOAT d;
	FLOAT x = (FLOAT)pt2.x - (FLOAT)pt1.x;
	FLOAT y = (FLOAT)pt2.y - (FLOAT)pt1.y;

	d = sqrt(x*x + y*y);
	return d;
}


FPOINT FPoint(FLOAT x, FLOAT y)
{
	FPOINT pt;
	pt.x = x;
	pt.y = y;
	return pt;
}

UINT GetNumberPoint(FPOINTSET pointSet)
{
	return pointSet.nPoint;
}

//Needs to replace 'GetNumberPoint()'
UINT GetPointSetCount(FPOINTSET pointSet)
{
	return pointSet.nPoint;
}


VOID GetPointFromPointSet(FPOINTSET pointSet, FPOINT *point)
{
	UINT i;

	for(i = 0; i < pointSet.nPoint; i++)
	{
		memcpy(&point[i], &pointSet.point[i], sizeof(FPOINT));
	}

}

POINT3D Point3D(FLOAT x, FLOAT y, FLOAT z)
{
	POINT3D p3D;
	p3D.x = x;
	p3D.y = y;
	p3D.z = z;

	return p3D;
}

DWORD Alloc(FPOINTSET *ps, DWORD size)
{

	if(!ps)
		return FALSE;

	Free(ps);
	//if(ps->point){
	//	free(ps->point);
	//}

	ps->point = (FPOINT *)malloc(size*sizeof(FPOINT));
	
	if(ps->point){
		memset(ps->point, NULL, size*sizeof(FPOINT));
		ps->size = size;
		ps->nPoint = NULL;
		return TRUE;
	}
	else{
		memset(ps, NULL, sizeof(POINTSET));
		return FALSE;
	}

	return TRUE;
}

DWORD AllocPointSet(POINTSET *ps, DWORD size)
{

	if(!ps)
		return FALSE;

	ps->point = (POINT *)malloc(size*sizeof(POINT));
	
	if(ps->point){
		memset(ps->point, NULL, size*sizeof(POINT));
		ps->size = size;
		ps->nPoints = NULL;
	}
	else{
		memset(ps, NULL, sizeof(POINTSET));
	}

	return TRUE;
}

DWORD AppendPoint(POINTSET *ps, POINT pt)
{
	if(!ps){
		return FALSE;
	}

	if(ps->nPoints < ps->size){
		memcpy(&ps->point[ps->nPoints], &pt, sizeof(POINT));
		ps->nPoints++;
		return TRUE;
	}else{
		return FALSE;
	}
}

//Moves each point from 
DWORD Transfer(FPOINTSET *psDest, FPOINTSET *psSrc)
{
	if(!psDest || !psSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = psSrc->nPoint;

	if(n > psDest->size){
		n = psDest->size;
	}


	//memset(psDest->fpoint, NULL, psDest->nPoint*sizeof(FPOINT));

	//for(i = 0; i < n; i++){
	//	memcpy(psDest->point[i], psSrc->point[i], sizeof(FPOINT));
	//}

	memcpy(psDest->point, psSrc->point, n*sizeof(FPOINT));

	memset(psSrc->point, NULL, n*sizeof(FPOINT));
	psDest->nPoint = n;
	psSrc->nPoint = NULL;

	return TRUE;

}

DWORD Empty(FPOINTSET *ps)
{
	if(!ps){
		return FALSE;
	}

	if(ps->point){
		memset(ps->point, NULL, ps->nPoint*sizeof(FPOINT));
		ps->nPoint = NULL;
		return TRUE;
	}else{
		return FALSE;
	}
}

DWORD Copy(FPOINTSET *psDest, FPOINTSET *psSrc)
{
	if(!psDest || !psSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = psSrc->nPoint;

	if(n > psDest->size){
		n = psDest->size;
	}
	
	memcpy(psDest->point, psSrc->point, n*sizeof(FPOINT));
	psDest->nPoint = n;

	return TRUE;
}



DWORD Push(FPOINTSET *ps, FPOINT pt)
{
	if(!ps){
		return FALSE;
	}

	if(ps->nPoint < ps->size){
		memcpy(ps->point+ps->nPoint, &pt, sizeof(FPOINT));
		ps->nPoint++;
	}else{
		return FALSE;
	}

	return TRUE;
}

DWORD Free(FPOINTSET *ps)
{

	if(!ps){
		return FALSE;
	}

	if(ps->point){
		free(ps->point);
		ps->point = NULL;
		ps->size = NULL;
		ps->nPoint = NULL;
		return TRUE;
	}else{
		return FALSE;
	}
}

FLOAT GetMaximumDistance(FPOINTSET &fps1, FPOINTSET &fps2)
{
	FLOAT maxDist = NULL;
	FLOAT dist = NULL;
	
	DWORD n1 = fps1.nPoint;
	DWORD n2 = fps2.nPoint;
	DWORD i1 = NULL;
	DWORD i2 = NULL;

	for(i1 = NULL; i1 < n1; i1++){
		for(i2 = NULL; i2 < n2; i2++){
	
			dist = GetDistance(fps1.point[i1], fps2.point[i2]);

			if(dist > maxDist){
				maxDist = dist;
			}

		}
	}

	return maxDist;
	
}

FLOAT GetMinimumDistance(FPOINTSET &fps1, FPOINTSET &fps2)
{
	FLOAT minDist = NULL;
	FLOAT dist = NULL;
	
	DWORD n1 = fps1.nPoint;
	DWORD n2 = fps2.nPoint;
	DWORD i1 = NULL;
	DWORD i2 = NULL;

	if(fps1.nPoint && fps2.nPoint){
		minDist = GetDistance(fps1.point[0], fps2.point[0]);
	}

	for(i1 = NULL; i1 < n1; i1++){
		for(i2 = NULL; i2 < n2; i2++){
	
			dist = GetDistance(fps1.point[i1], fps2.point[i2]);

			if(dist < minDist){
				minDist = dist;
			}

		}
	}

	return minDist;
	
}

DWORD AreSame(FPOINT p1, FPOINT p2)
{

	if(p1.x != p2.x){
		return FALSE;
	}
	if(p1.y != p2.y){
		return FALSE;
	}

	return TRUE;

}

DWORD Alloc(P3DSET *ps, DWORD size)
{
	if(!ps)
		return FALSE;

	ps->p = (P3D *)malloc(size*sizeof(P3D));

	if(ps){
		ps->size = size;
		ps->nPoint = 0;
	}

	return TRUE;
}

DWORD Append(P3DSET *ps, P3D p)
{
	if(!ps)
		return FALSE;

	if(ps->nPoint == ps->size)
		return FALSE;

	memcpy(&p, &ps->p[ps->nPoint], sizeof(P3D));

	ps->nPoint++;


	return TRUE;
}

DWORD Remove(P3DSET *ps, DWORD iPoint)
{
	if(!ps)
		return FALSE;

	if(iPoint >= ps->nPoint)
		return FALSE;

	//memcpy(ps->p[iPoint], ps->p[iPoint-1], 

	INT i;
	for(i = 0; i < ps->nPoint-1; i++)
	{
		memcpy(&ps->p[iPoint], &ps->p[iPoint+1], sizeof(P3D));
	}

	return TRUE;
}

DWORD Copy(P3D *pDest, P3D pSrc)
{
	if(!pDest)
		return FALSE;

	memcpy(pDest, &pSrc, sizeof(P3D));


	return TRUE;
}

DWORD Copy(P3D *pDest, DOUBLE_3DPOINT pSrc)
{
	if(!pDest)
		return FALSE;

	memcpy(pDest, &pSrc, sizeof(P3D));


	return TRUE;
}

	