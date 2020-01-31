#pragma once

#include "stdafx.h"
#include "RACE.h"

//Tab Positions
#define TP_STACK	0
#define TP_FILTER	1
#define TP_DISPLAY	2

#define TP_GRAYSCALE		0
#define TP_IMAGESUBTRACT	1

typedef struct DATATAB_{

	HWND hwnd;
	HWND hComboBox;
	HWND hBorder;
	HWND hDataBar;
	HWND hEdit;
	HWND hTree;
	HWND hActive;

	DWORD flagDrag;
	HTREEITEM htDrag;

	WNDPROC wpOriginal;

}DATATAB;

LRESULT CALLBACK DataTabSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT OnCommandDataTab(DATATAB *lpDataTab, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnNotifyDataTab(DATATAB *lpDataTab, HWND hwnd, WPARAM wParam, LPARAM lParam);

DWORD InitializeTreeDataTab(HWND hwnd, HWND hTree);