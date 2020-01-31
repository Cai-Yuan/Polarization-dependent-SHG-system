#pragma once

#include "stdafx.h"

#define ID_MDI_FIRSTCHILD	5000

//Used when Creating The Windows
//Can be used to retireve their handles
//Use: GetDlgItem()
#define IDC_MAIN_MDI		101
#define IDC_MAIN_TOOL		102
#define IDC_TOOL			103
#define IDC_TOOL_LEFT		104
#define IDC_TOOL_RIGHT		105

typedef struct MDICREATETHREAD_{
	HWND hParent;
	HWND hMDI;
	MDICREATESTRUCT mcs;
}MDICREATETHREAD;

HWND CreateMDIClient(HWND hwnd, INT posWindowMenu);
BOOL RegisterMDIChild(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszClassName);
BOOL RegisterMDIChild(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszClassName, HCURSOR hCursor);
HWND CreateMDIChild(HWND hMDIClient, LPCTSTR lpszClassName, LPCTSTR lpszWindowTitle);
LRESULT CALLBACK MDIChildListViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID ManageMDIFrameMenu(HWND hwnd, HWND hwndFrame, WPARAM wParam, LPARAM lParam);

INT GetTaskBarHeight();
INT GetTaskBarWidth();
RECT GetTaskBarRect();

DWORD WINAPI CreateThreadedMDIChildProc(LPVOID param);
HWND CreateNewMDIChild(HWND hParent, HWND hMDI, LPCTSTR lpszClassName, LPCTSTR lpszWindowTitle);
DWORD SetActiveMDI(HWND hParent, HWND hChild);