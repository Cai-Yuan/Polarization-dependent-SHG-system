#pragma once

#include "stdafx.h"

DWORD Stretch(UINT w, UINT h, UINT lower, UINT upper, UCHAR *img);
DWORD Stretch(UINT w, UINT h, FLOAT lower, FLOAT upper, FLOAT *in, FLOAT *out);
DWORD Stretch(UINT w, UINT h, INT lower, INT upper, INT *in, INT *out);
DWORD Stretch(UINT w, UINT h, INT lower, INT upper, INT *in, UCHAR *out);