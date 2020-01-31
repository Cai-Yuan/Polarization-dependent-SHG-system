#include "stdafx.h"

BOOL RegisterWndClass(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszClassName)
{
	DWORD lastError = NULL;
	HWND hwnd = NULL;
	DWORD dw;

	WNDCLASSEX wc;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = lpfnWndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
    wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
    wc.lpszMenuName	 = NULL;
    wc.lpszClassName = lpszClassName;
    wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	SetLastError(NULL);
	if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Child Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		lastError = GetLastError();
        return FALSE;
    }


	return TRUE;
		
}