#pragma once

#include "stdafx.h"

typedef struct POLYGON_{
	UINT nPoint;
	POINT *point;

	//\/\/UPDATE \/\/\/
	UINT nVertex;
	POINT *vertex;
	//^^^^^^^^^^^^^^^^

}POLYGON;

DWORD PointSetToPolygon(POINTSET *ps, POLYGON *polygon);