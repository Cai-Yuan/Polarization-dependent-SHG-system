#pragma once

#include "stdafx.h"

FLOAT ** CallocImageStack(DWORD width, DWORD height, DWORD nSlice);
VOID **CallocImageSet(DWORD width, DWORD height, DWORD nSlice, DWORD sizePixel);
DWORD FreeImageStack(DWORD nSlice, FLOAT **stack);