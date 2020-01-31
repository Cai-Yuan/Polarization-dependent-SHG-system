#pragma once

#include "stdafx.h"

INT Max(INT dw1, INT dw2);

DWORD Max(DWORD dw1, DWORD dw2);
DWORD Min(DWORD dw1, DWORD dw2);
FLOAT MaxFloat(FLOAT f1, FLOAT f2);
FLOAT MinFloat(FLOAT f1, FLOAT f2);

DWORD GetMaxMin(UINT n, DWORD *max, DWORD *min, DWORD *dw);
DWORD GetMaxMin(UINT n, DWORD *max, DWORD *min, UCHAR *data);
DWORD GetMaxMin(UINT n, INT *max, INT *min, INT *data);
DWORD GetMaxMin(UINT n, FLOAT *max, FLOAT *min, FLOAT *dw);