#pragma once

#include "stdafx.h"

DWORD MaskData(DWORD nData, UCHAR *in, INT *mask, UCHAR *out);
DWORD MaskData(DWORD nData, INT *in, INT *mask, INT *out);