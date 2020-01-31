#pragma once

#include "stdafx.h"

//typedef struct REGION_{
//	FPOINTSET ps
//}REGION;
//
//typedef struct REGIONSET_{
//	DWORD size;
//	DWORD nRegion;
//	REGION *region;
//}REGIONSET;

DWORD Region(UINT w, UINT h, UCHAR *img, DWORD nPointSet, FPOINTSET **ptSet);