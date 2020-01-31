#pragma once

#include "stdafx.h"

typedef struct PROGRESSVIEW_{
	HWND hOwner;
	HWND hFrame;
	HWND hProgress;

}PROGRESSVIEW;

LRESULT CALLBACK ProgressViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);