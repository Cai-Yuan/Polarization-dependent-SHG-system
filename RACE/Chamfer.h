#pragma once

#include "stdafx.h"

DWORD Chamfer(UINT width, UINT height, UCHAR *img, UCHAR *chamfer);
DWORD Chamfer(UINT width, UINT height, FLOAT *img, FLOAT *chamfer);
DWORD Chamfer(UINT width, UINT height, INT *img, INT *chamfer);
DWORD Chamfer(UINT width, UINT height, FLOAT *thresh, FLOAT *img, FLOAT *chamfer);
INT ChamferMin(INT a, INT b);
FLOAT ChamferMin(FLOAT a, FLOAT b);