#pragma once

#include "stdafx.h"

DWORD ConstSubtract(UINT w, UINT h, FLOAT constant, FLOAT *img, FLOAT *out);

DWORD ImageSubtract(UINT w, UINT h, UCHAR *frmOne, UCHAR *frmTwo, UCHAR *imgSubtract);
DWORD ImageSubtract(UINT w, UINT h, FLOAT *frmOne, FLOAT *frmTwo, FLOAT *imgSubtract);
DWORD ImageSubtract(UINT w, UINT h, UCHAR *imgOne, UCHAR *imgTwo, INT *imgSubtract);
