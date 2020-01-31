#pragma once

typedef struct CREATETHREADEDWINDOW_{

	DWORD dwExStyle;
	LPCTSTR lpClassName;
	LPCTSTR lpWindowName;
	DWORD dwStyle;
	INT x;
	INT y; 
	INT nWidth;
	INT nHeight;
	HWND hwndParent;
	HMENU hMenu;
	LPVOID lpParam;

	HWND hwnd;
	DWORD flgReturn;

}CREATETHREADEDWINDOW;


DWORD WINAPI WindowProcThread(VOID *lpvoid);
//DWORD WINAPI CreateWindowExThread(VOID *lpvoid);
HWND CreateWindowExThreaded(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, INT x, INT y, INT nWidth, INT nHeight, HWND hwndParent, HMENU hMenu, LPVOID lpParam);