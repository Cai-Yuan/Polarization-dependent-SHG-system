#pragma once

#include "stdafx.h"

typedef struct TEXTR_{
	POINT pt;
	CHAR str[MAX_STRING];
}TEXTR;

DWORD DrawText(HDC hdc, TEXTR text, COLORREF cr);