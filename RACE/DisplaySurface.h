#pragma once

#include "stdafx.h"

HBITMAP CreateDisplaySurface(HWND hwnd, HDC *hdcMem, HDC *hdcDisp, UINT w, UINT h, PIXEL **buffer);
