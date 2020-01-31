#pragma once

#include "stdafx.h"

POINT GetRotationPosition(CIRCLE c, FLOAT v, FLOAT t, FLOAT fps);
DWORD GenerateCircumfrencialPoints(UINT w, UINT h, UCHAR *img, FLOAT v, FLOAT t, FLOAT fps);
DWORD TestHologramPointGen(HWND hwnd, UINT w, UINT h);
