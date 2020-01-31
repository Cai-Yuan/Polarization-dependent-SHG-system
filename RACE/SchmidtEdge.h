#pragma once

#include "stdafx.h"
#include "Range.h"

VOID GetSchmidtContourPOINTs(UINT w, UINT h, INT *dxdyThresh, POINT_SET *pointSet);
VOID SchmidtEdgeGradientPlow(UINT w, UINT h, INT thresh, UINT fSize, UCHAR *img, INT *dxdyThresh, POINT_SET *psPoint);