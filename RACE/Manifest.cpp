#include "stdafx.h"

HWND CreateManifest(HWND hMDIFrame, HWND hMDIClient)
{
	RECT rcClient;
	HWND hManifest = NULL;

	GetClientRect(hMDIFrame, &rcClient);
	//hManifest = CreateMDIChild(hMDIClient, "TextEditor", "Manifest");


	hManifest = CreateWindow("TextEditor",
							"Manfiest",
							WS_CHILD|WS_BORDER|WS_SIZEBOX/*|WS_HSCROLL|WS_VSCROLL*/,
							0,
							CW_USEDEFAULT,
							600,
							CW_USEDEFAULT,
							hMDIFrame,
							(HMENU)IDC_MANIFEST,
							GetModuleHandle(NULL),
							NULL);	
	SendMessage(hManifest, WM_SETHWND, HWND_OWNER, (LPARAM) hMDIFrame);

	//Make sure window is scrolled to the left
	SetScrollPos(hManifest, SB_HORZ, 0, TRUE); //用于设置所指定滚动条中的滚动按钮的位置。
	
	SetWindowPos(hManifest, NULL, 0, 0, ((rcClient.right-rcClient.left)/3), (rcClient.bottom - rcClient.top)-30, SWP_NOZORDER);
	ShowWindow(hManifest, SW_SHOW);

	if(!hManifest)
		MessageBox(hMDIFrame, "Manfiest Creation Failed!", "Error!", MB_OK);

	return hManifest;
}

DWORD InsertNewLineManifest(HWND hManifest, CHAR *strLine)
{
	CHAR str[MAX_STRING];
	CHAR strSystemTime[MAX_STRING];
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	//GetSystemTime(&systemTime);
	sprintf(strSystemTime, "%2.2d:%2.2d:%2.2d:%3.3d |:| ", systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	HWND hManifestFrame = hManifest;
	HWND hManifestEdit = NULL;

	//OLD//
	//HWND hManifestEdit = (HWND) SendMessage(hManifestFrame, WM_GETHWNDEDIT, NULL, NULL);
	///////

	//REPLACEMENT//11.14.2012
	hManifestEdit = GetDlgItem(hManifestFrame, IDC_CHILD_EDIT);
	///////////////

	SendMessage(hManifestFrame, WM_NEWLINE, NULL, NULL);

	sprintf(str, "%s%s", strSystemTime, strLine);
	SendMessage(hManifestFrame, WM_APPENDTEXT, (WPARAM)str/*Line*/, (LPARAM) NULL);

	return TRUE;
}


/*

Initially under RACE WM_CREATE

////Get hEdit
//hManifestEdit = (HWND) SendMessage(lpFrame->hManifest, WM_GETHWNDEDIT, NULL, NULL);

////Add Text
//SendMessage(hManifestEdit, EM_SETREADONLY, (WPARAM)TRUE, (LPARAM)NULL);
//SendMessage(hManifestFrame, WM_APPENDTEXT, (WPARAM)"Initializing...", (LPARAM) NULL);
//SendMessage(hManifestFrame, WM_NEWLINE, NULL, NULL);
//SendMessage(hManifestFrame, WM_APPENDTEXT, (WPARAM)"Success.", (LPARAM) NULL);

*/