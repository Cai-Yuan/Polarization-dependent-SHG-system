#pragma once

typedef struct LIMIT_{
	float upper;
	float lower;
}LIMIT;

DWORD Range(FLOAT value, LIMIT limit);
DWORD Set(FLOAT lower, FLOAT upper, LIMIT *limit);
LRESULT SendLimit(HWND hwnd, DWORD flgLimit, LIMIT limit);