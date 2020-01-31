#pragma once

#include "stdafx.h"

typedef struct LISTVIEW_{

	HANDLE hOwner;
	HWND hListView;

}LISTVIEW;

LRESULT CALLBACK ListViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);