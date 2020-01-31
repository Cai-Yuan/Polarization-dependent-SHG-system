#include "stdafx.h"

#include "Polygon.h"


DWORD PointSetToPolygon(POINTSET *ps, POLYGON *polygon)
{
	if(!ps || !polygon){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < ps->nPoints; i++){

		if(polygon->point){
			memcpy(&polygon->point[i], &ps->point[i], sizeof(POINT));
		}
		if(polygon->vertex){
			memcpy(&polygon->vertex[i], &ps->point[i], sizeof(POINT));
		}
	}

	polygon->nPoint = ps->nPoints;
	polygon->nVertex = ps->nPoints;

	return TRUE;

}
