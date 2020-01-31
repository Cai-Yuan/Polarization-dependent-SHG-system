#include "stdafx.h"

#include "Subclass.h"

DWORD SubclassWnd(HWND hControl, WNDPROC wpSubclass)
{
	DWORD dw = NULL;

	WNDPROC wpOriginal = NULL;

	wpOriginal = (WNDPROC) SetWindowLongPtr(hControl, GWLP_WNDPROC, (LONG_PTR)wpSubclass);

	SendMessage(hControl, WM_INITSUBCLASS, (WPARAM)NULL, (LPARAM)(WNDPROC) wpOriginal);


	return dw;
}

DWORD SubclassWnd(HWND hParent, HWND hControl, WNDPROC wpSubclass)
{
	DWORD dw = NULL;

	WNDPROC wpOriginal = NULL;

	wpOriginal = (WNDPROC) SetWindowLongPtr(hControl, GWLP_WNDPROC, (LONG_PTR)wpSubclass);

	SendMessage(hControl, WM_INITSUBCLASS, (WPARAM)(WNDPROC) wpOriginal, (LPARAM)(HWND) hParent);


	return dw;
}