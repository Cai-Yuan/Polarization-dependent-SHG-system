#pragma once

#include "stdafx.h"

DWORD Average(DWORD nData, INT *imgI, INT *imgII, INT *imgOut);
DWORD Average(DWORD nData, UCHAR *imgI, UCHAR *imgII, UCHAR *imgOut);
DWORD Blend(DWORD nData, FLOAT aI, FLOAT aII, UCHAR *imgI, UCHAR *imgII, UCHAR *imgOut);