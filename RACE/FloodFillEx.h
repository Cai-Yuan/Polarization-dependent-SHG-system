#pragma once

#include "stdafx.h"
#include "FloodFill.h"

DWORD FloodFillEx(UINT w, UINT h, UCHAR *imgIn, UCHAR *imgOut, FPOINTSET *ptSet);
DWORD FloodFillEx(UINT w, UINT h, INT seed, UCHAR value, UCHAR *img, FPOINTSET *ptSet);