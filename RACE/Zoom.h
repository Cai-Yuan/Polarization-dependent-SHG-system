#pragma once

#include "stdafx.h"

DWORD ZoomPIXEL(UINT w, UINT h, UINT *wZoom, UINT *hZoom, PIXEL *img, PIXEL *zoom);
DWORD ZoomPIXEL(UINT w, UINT h, UINT mag, UINT *wZoom, UINT *hZoom, PIXEL *img, PIXEL *zoom);

DWORD ZoomUCHAR(UINT w, UINT h, UINT *wZoom, UINT *hZoom, UCHAR *img, UCHAR *zoom);