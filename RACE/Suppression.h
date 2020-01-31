#pragma once

#include "stdafx.h"

DWORD NonMaximumSuppression(UINT w, UINT h, UCHAR *img);
DWORD NonMaximumSuppression(UINT w, UINT h, FLOAT *in, FLOAT *out);
DWORD NonMaximumSuppression(UINT w, UINT h, INT *in, INT *out);

//Experimental 7.20.13
DWORD NonMaximumSuppressionExp(UINT w, UINT h, FLOAT *in, FLOAT *out);