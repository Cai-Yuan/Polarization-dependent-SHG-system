#pragma once

#include "stdafx.h"

DWORD WINAPI ZStackAlgorithm(LPVOID param);

DWORD ZStack(DWORD width, DWORD height, DWORD nSlice, FLOAT **stack, FLOAT *imgZStack);