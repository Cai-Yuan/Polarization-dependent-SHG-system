#pragma once

#include "stdafx.h"

DWORD Threshold(UINT w, UINT h, UINT threshold, UCHAR *img);
DWORD Threshold(UINT w, UINT h, UINT threshold, INT *img);
DWORD Threshold(UINT w, UINT h, FLOAT threshold, FLOAT *img);
DWORD Threshold(UINT w, UINT h, UINT threshold, UCHAR *img, UCHAR *imgThresh);
DWORD Threshold(UINT w, UINT h, FLOAT threshold, INT *imgIn, INT *imgOut);
DWORD Threshold(UINT w, UINT h, FLOAT threshold, FLOAT *imgIn, FLOAT *imgOut);
DWORD Threshold(UINT w, UINT h, FLOAT *imgIn, FLOAT *imgRef, FLOAT *imgOut);
DWORD DoubleThreshold(UINT w, UINT h, INT lower, INT upper, INT *in, INT *out);