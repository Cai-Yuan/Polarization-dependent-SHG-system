#pragma once

#include "stdafx.h"

typedef struct ALGORITHMTHREAD_{

	HWND hwnd;
	DATAVIEW *lpDataView;
	UCHAR *data;

}ALGORITHMTHREAD;

DWORD WINAPI AlgorithmProc(LPVOID param);