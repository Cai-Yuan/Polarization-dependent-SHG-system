#pragma once

#include "stdafx.h"

DWORD GaussianTest(HWND hwnd, INT n, FLOAT sigma);
DWORD GetGaussianKernel(INT n, FLOAT sigma, FLOAT *kernel);
DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, UCHAR *img);
DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, FLOAT *img);
DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, FLOAT *img, FLOAT *blur);
DWORD GaussianBlur(UINT w, UINT h, INT n, FLOAT sigma, INT *img, INT *blur);