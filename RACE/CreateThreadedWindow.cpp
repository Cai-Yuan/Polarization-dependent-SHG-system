
#include "stdafx.h"
#include "CreateThreadedWindow.h"

HWND CreateWindowExThreaded(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, INT x, INT y, INT nWidth, INT nHeight, HWND hwndParent, HMENU hMenu, LPVOID lpParam)
{
	DWORD dwThreadExitCode = NULL;
	HANDLE hThread = NULL;
	CREATETHREADEDWINDOW ctw = {0};

	ctw.dwExStyle = dwExStyle;
	ctw.lpClassName = lpClassName;
	ctw.lpWindowName = lpWindowName;
	ctw.dwStyle = dwStyle;
	ctw.x = x;
	ctw.y = y;
	ctw.nWidth = nWidth;
	ctw.nHeight = nHeight;
	ctw.hwndParent = hwndParent;
	ctw.hMenu = hMenu;
	ctw.lpParam = lpParam;
	ctw.flgReturn = NULL;

	hThread = CreateThread(NULL, NULL, WindowProcThread, &ctw, NULL, NULL);

	while(!ctw.flgReturn && (GetExitCodeThread(hThread, &dwThreadExitCode)==STILL_ACTIVE)){

		ctw.flgReturn = ctw.flgReturn;
		ctw.flgReturn = ctw.flgReturn;
		ctw.flgReturn = ctw.flgReturn;
	}

	Sleep(100);

	return ctw.hwnd;
}


DWORD WINAPI WindowProcThread(VOID *lpvoid)
{

	CREATETHREADEDWINDOW *cwt = (CREATETHREADEDWINDOW *) lpvoid;

	MSG msg = {0};
	HWND hwnd;

	hwnd = CreateWindowEx(cwt->dwExStyle,
								cwt->lpClassName,
								cwt->lpWindowName,
								cwt->dwStyle,
								cwt->x,
								cwt->y,
								cwt->nWidth,
								cwt->nHeight,
								cwt->hwndParent,
								cwt->hMenu,
								GetModuleHandle(0), 
								cwt->lpParam);
	cwt->hwnd = hwnd;
	cwt->flgReturn = TRUE;

	if(!hwnd){
		MessageBox(NULL, "Error!", "CreateWindowExThreaded() Failed!", MB_OK);
		return FALSE;
	}

	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}