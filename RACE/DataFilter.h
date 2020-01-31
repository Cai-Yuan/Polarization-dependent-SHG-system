#pragma once

typedef struct DATAFILTER_{
	HWND hwnd;
	STRINGSET ssFilter;
	DWORD selFilterProc;
}DATAFILTER;

LRESULT CALLBACK DataFilterProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD OnCreateDataFilter(DATAFILTER *lpDataFilter, HWND hwnd, WPARAM wParam, LPARAM lParam);
DWORD OnInitFilter(DATAFILTER *lpDataFilter, HWND hwnd, WPARAM wParam, LPARAM lParam);