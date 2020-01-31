#include "stdafx.h"

#include "RACE.h"
#include "ToolsWIN32.h"

#define STR_BOOT "boot.pdm"

LRESULT SaveWindowPositions(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT ReadWindowPositions(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam);
DWORD LoopThroughWNDSET(LPTRFRAME *lpFrame, HWND hwnd);

DWORD UpdateWindowMetrics(HWND hParent, HWND hChild, WND *wnd);

DWORD UpdateWindowMetrics(HWND hParent, HWND hwnd, WND *wnd);
DWORD UpdateWindowMetrics(HWND hParent, HWND hwnd, WNDSET *wndSet);

DWORD GetWndID(LPTRFRAME *lpFrame, HWND hwnd);
DWORD GetWndID(LPTRFRAME *lpFrame, CHAR *strText);

DWORD AddWindow(WNDSET *wndSet, HWND hwnd);