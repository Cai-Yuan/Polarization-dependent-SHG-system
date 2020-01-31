#pragma once

#include "stdafx.h"
#include "MaxMin.h"

#define MAXBINS_UCHAR 256

DWORD GetHistogram(UINT w, UINT h, UCHAR *img, DWORD *dwHistogram);
DWORD PlotHistogram(UINT w, UINT h, DWORD *dwHistogram, PIXEL *bmp, COLORREF colorref);

DWORD GetHistogram(DWORD w, DWORD h, FLOAT *in, FLOAT *hist);
DWORD GetHistogramMedian(DWORD n, FLOAT *in, FLOAT *out);