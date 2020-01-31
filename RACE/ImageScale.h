#include "stdafx.h"

DWORD GetScaleDimensions(FLOAT scale, INT wSource, INT hSource, INT *wScale, INT *hScale);
DWORD GetNormalizedCoordinate(INT width, INT height, INT x, INT y, FLOAT *xNorm, FLOAT *yNorm);
DWORD GetPixelCoordinate(INT width, INT height, FLOAT xNorm, FLOAT yNorm, INT *x, INT *y);
DWORD ScaleImage(FLOAT scale, INT wSource, INT hSource, UCHAR *imgSource, UCHAR *imgScale);
DWORD ScaleImage(FLOAT scale, INT wSource, INT hSource, PIXEL *imgSource, PIXEL *imgScale);
DWORD RotateImage(FLOAT deg, INT w, INT h, PIXEL *imgIn, PIXEL *imgOut);