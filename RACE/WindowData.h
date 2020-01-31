#pragma once

#include "stdafx.h"

DWORD WindowData(UINT n, UINT lower, UINT upper, INT *data);
DWORD WindowData(UINT n, UINT lower, UINT upper, UCHAR *data);
DWORD ClampData(UINT n, INT lower, INT upper, INT *data, INT *out);
DWORD ClampData(UINT n, INT lower, INT upper, INT *data, UCHAR *out);
DWORD ClampData(UINT n, INT lower, INT upper, FLOAT *data, UCHAR *out);
DWORD ClampData(UINT n, INT lower, INT upper, FLOAT *data, FLOAT *out);
DWORD Clamp(UINT n, INT lower, INT upper, INT *data);