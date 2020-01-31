#include "stdafx.h"
#include "MDI.h"

#define MDICREATE_XOFF		100
#define MDICREATE_YOFF		50
#define MDICREATE_WIDTH		500
#define MDICREATE_HEIGHT	700

HWND CreateMDIClient(HWND hwnd, INT posWindowMenu)
{
	HWND hMDIClient;
	CLIENTCREATESTRUCT ccs;//Contains information about the menu and first multiple-document interface (MDI) child window of an MDI client window. An application passes a pointer to this structure as the lpParam parameter of the CreateWindow function when creating an MDI client window.

	ccs.hWindowMenu = GetSubMenu(GetMenu(hwnd), posWindowMenu);
	ccs.idFirstChild = ID_MDI_FIRSTCHILD;

	hMDIClient = CreateWindowEx(WS_EX_CLIENTEDGE
								/*| WS_EX_COMPOSITED*/, 
		"MDIClient",
		NULL, 
		WS_CHILD|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL|WS_VISIBLE, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		hwnd, 
		(HMENU)IDC_MAIN_MDI, 
		GetModuleHandle(NULL), 
		(LPVOID)&ccs);

	if(!hMDIClient)
		MessageBox(hwnd, "Error Creating MDI Client", "Error!", MB_OK);

	return hMDIClient;
}


BOOL RegisterMDIChild(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszClassName)
{

	HWND hwnd = NULL;
	DWORD dw;

	WNDCLASSEX wc;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = lpfnWndProc; //窗口类的过程函数
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
    wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
    wc.lpszMenuName	 = NULL;
    wc.lpszClassName = lpszClassName;//类的名字
    wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "MDI Child Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }


	return TRUE;
		
}

BOOL RegisterMDIChild(HINSTANCE hInstance, WNDPROC lpfnWndProc, LPCTSTR lpszClassName, HCURSOR hCursor)
{

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
    wc.hCursor		 = hCursor;
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
    wc.lpszMenuName	 = NULL;
    wc.lpszClassName = lpszClassName;
    wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "MDI Child Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }


	return TRUE;
		
}

HWND CreateMDIChild(HWND hMDIClient, LPCTSTR lpszClassName, LPCTSTR lpszWindowTitle)
{ 
    MDICREATESTRUCT mcs;
    HWND hChild;
	HWND hMDIFrame;

    mcs.szTitle = lpszWindowTitle;
    mcs.szClass = lpszClassName;
    mcs.hOwner  = GetModuleHandle(NULL);
    mcs.x = 0/*MDICREATE_XOFF*/; mcs.cx = MDICREATE_WIDTH/*CW_USEDEFAULT*/;
    mcs.y = 0/*MDICREATE_YOFF*/; mcs.cy = MDICREATE_HEIGHT/*CW_USEDEFAULT*/;
    mcs.style = MDIS_ALLCHILDSTYLES;
	mcs.lParam = NULL;

    hChild = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LONG)&mcs);
    if(!hChild)
    {
		
		DWORD error = GetLastError();

		//Errors
		//1407 -- ERROR_CANNOT_FIND_WND_CLASS

        MessageBox(hMDIClient, 
					"MDI Child creation failed.", 
					"Error!",
					MB_ICONEXCLAMATION | MB_OK);

		
		return NULL;
    }
	
	ShowWindow(hChild, SW_HIDE);
	//Tell the MDI Parent window to manage the window count
	hMDIFrame = GetParent(hMDIClient);
	SendMessage(hMDIFrame, WM_REGISTERWINDOW, (WPARAM)hChild, (LPARAM)lpszClassName);

    return hChild;
}

//hParent --> MDI Parent
DWORD SetActiveMDI(HWND hParent, HWND hChild)
{
	DWORD result;
	HWND hMDIClient;

	hMDIClient = (HWND) SendMessage(hParent, WM_GETHWND, (WPARAM)HWND_MDI, NULL);

	result = SendMessage(hMDIClient, WM_MDIACTIVATE, (WPARAM) hChild, NULL);

	return TRUE;
}