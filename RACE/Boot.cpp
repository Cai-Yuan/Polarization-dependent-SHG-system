﻿#include "stdafx.h"

#include "Boot.h"

LRESULT SaveWindowPositions1(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HANDLE hFile = NULL;

	DWORD nBytesToWrite = NULL;
	DWORD nBytesWritten = NULL;

	CHAR idPDM[] = {'P', '7'};
	LONG sizeMetaData = sizeof(PDMWINDOWHEADER);

	RECT rcFrame = {0};
	RECT rcDataView = {0};
	RECT rcTextWindow = {0};
	RECT rcManifest = {0};
	RECT rcPropertyManager = {0};
	RECT rcCommandPrompt = {0};
	RECT rcUNO32 = {0};
	RECT rcCameraProperties = {0};
	RECT rcDataBar = {0};

	CHAR strFilePath[MAX_STRING];
 
	sprintf(strFilePath, "%s%s", lpFrame->strArchivePath, STR_BOOT);

	LoopThroughWNDSET(lpFrame, hwnd);

	if((hFile = CreateFile((LPSTR)strFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

		GetWindowRect(hwnd, &rcFrame);
		//GetWindowRect(lpFrame->hManifest, &rcManifest);	
		//GetWindowRect(lpFrame->hCommandPrompt, &rcCommandPrompt);
		//GetWindowRect(lpFrame->hPropertyManager, &rcPropertyManager);
		GetChildRect(lpFrame->hMDIClient, lpFrame->hManifest, &rcManifest);
		GetChildRect(lpFrame->hMDIClient, lpFrame->hCommandPrompt, &rcCommandPrompt);
		GetChildRect(lpFrame->hMDIClient, lpFrame->hPropertyManager, &rcPropertyManager);
		GetChildRect(lpFrame->hMDIClient, lpFrame->hCameraProperties, &rcCameraProperties);
		GetChildRect(lpFrame->hMDIClient, lpFrame->hUNO32, &rcUNO32);

		PDMWINDOW pdmWindow;
		PDMWINDOWHEADER pdmWindowHeader = {0};
		PDMWINDOWDATA pdmWindowData[256] = {0};

		pdmWindowHeader.typeMetaData = MD_WINDOW;
		pdmWindowHeader.typeData = DT_WINDOW;
		pdmWindowHeader.verMetaData = NULL;
		pdmWindowHeader.nWindows = 7;
		pdmWindowHeader.sizeData = pdmWindowHeader.nWindows*sizeof(PDMWINDOWDATA);

		//Initialize Window Data
		pdmWindowData[0].idWindow = HWND_FRAME;
		memcpy(&pdmWindowData[0].rcWindow, &rcFrame, sizeof(RECT));
		pdmWindowData[1].idWindow = HWND_MANIFEST;
		memcpy(&pdmWindowData[1].rcWindow, &rcManifest, sizeof(RECT));
		pdmWindowData[2].idWindow = HWND_PROPERTYMANAGER;
		memcpy(&pdmWindowData[2].rcWindow, &rcPropertyManager, sizeof(RECT));
		pdmWindowData[3].idWindow = HWND_COMMANDPROMPT;
		memcpy(&pdmWindowData[3].rcWindow, &rcCommandPrompt, sizeof(RECT));
		pdmWindowData[4].idWindow = HWND_CAMERAPROPERTIES;
		memcpy(&pdmWindowData[4].rcWindow, &rcCameraProperties, sizeof(RECT));
		pdmWindowData[5].idWindow = HWND_UNO32;
		memcpy(&pdmWindowData[5].rcWindow, &rcUNO32, sizeof(RECT));

		
		pdmWindow.lpvoid = (VOID *) calloc(pdmWindowHeader.nWindows, sizeof(PDMWINDOWDATA));
 
		memcpy(&pdmWindow.pdmWindowHeader, &pdmWindowHeader, sizeof(PDMWINDOWHEADER));
		memcpy(pdmWindow.lpvoid, &pdmWindowData, pdmWindowHeader.nWindows*sizeof(PDMWINDOWDATA));

		WriteFile(hFile, &idPDM, sizeof(idPDM), &nBytesWritten, NULL);
		WriteFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesWritten, NULL);
		WriteFile(hFile, &pdmWindow.pdmWindowHeader, sizeof(PDMWINDOWHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, pdmWindow.lpvoid, pdmWindow.pdmWindowHeader.sizeData, &nBytesWritten, NULL);

		CloseHandle(hFile);
	}


	return TRUE;
}

LRESULT SaveWindowPositions(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	HANDLE hFile = NULL;

	DWORD nBytesToWrite = NULL;
	DWORD nBytesWritten = NULL;

	CHAR idPDM[] = {'P', '7'};
	LONG sizeMetaData = sizeof(PDMWINDOWHEADER);

	CHAR strFilePath[MAX_STRING];

	sprintf(strFilePath, "%s%s", lpFrame->strArchivePath, STR_BOOT);

	PDMWINDOW pdmWindow = {0};
	PDMWINDOWHEADER pdmWindowHeader = {0};
	PDMWINDOWDATA *pdmWindowData = NULL;

	WNDSET *wndSet = &lpFrame->wndSet;


	DWORD i = 0;
	DWORD nWnd = lpFrame->wndSet.nWnd;
	
	pdmWindowHeader.typeMetaData = MD_WINDOW;
	pdmWindowHeader.typeData = DT_WINDOW;
	pdmWindowHeader.verMetaData = NULL;
	pdmWindowHeader.nWindows = nWnd;
	pdmWindowHeader.sizeData = pdmWindowHeader.nWindows*sizeof(PDMWINDOWDATA);

	pdmWindowData = (PDMWINDOWDATA *)malloc(nWnd*sizeof(PDMWINDOWHEADER));

	if(!pdmWindowData){
		return FALSE;
	}

	for(i = 0; i < nWnd; i++){

		if(IsWindow(lpFrame->wndSet.wnd[i].hwnd)){
			UpdateWindowMetrics(hwnd, lpFrame->wndSet.wnd[i].hwnd, &lpFrame->wndSet);
		}else{
			INT p;
			p = 0;
			p = 0;
		}

		//////REVIEW FOR NULL PTR ACCESS//////////
		pdmWindowData[i].idWindow = (INT) NULL;
		//pdmWindowData[i].idWindow = (INT) wndSet->wnd[i].hwnd;
		memcpy(&pdmWindowData[i].strClass, wndSet->wnd[i].strClass, MAX_STRING*sizeof(CHAR));
		memcpy(&pdmWindowData[i].strText, wndSet->wnd[i].strText, MAX_STRING*sizeof(CHAR));
		memcpy(&pdmWindowData[i].rcWindow, &wndSet->wnd[i].rc, sizeof(RECT));

	}

	memcpy(&pdmWindow.pdmWindowHeader, &pdmWindowHeader, sizeof(PDMWINDOWHEADER));
	pdmWindow.lpvoid = pdmWindowData;


	hFile = CreateFile((LPSTR)strFilePath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(!hFile){
		return FALSE;
	}

	if(hFile != INVALID_HANDLE_VALUE){

		WriteFile(hFile, &idPDM, sizeof(idPDM), &nBytesWritten, NULL);
		WriteFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesWritten, NULL);
		WriteFile(hFile, &pdmWindow.pdmWindowHeader, sizeof(PDMWINDOWHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, pdmWindow.lpvoid, pdmWindow.pdmWindowHeader.sizeData, &nBytesWritten, NULL);

		CloseHandle(hFile);

	}


	return TRUE;
}




DWORD UpdateWindowMetrics(HWND hParent, HWND hwnd, WND *wnd)
{

	RECT rc = {0};
	HWND hGetParent = GetParent(hwnd);

	if(IsWindow(hwnd)){
		GetChildRect(hParent, hwnd, &rc);
		memcpy(&wnd->rc, &rc, sizeof(RECT));

		GetWindowText(hwnd, wnd->strText, MAX_STRING);
		GetClassName(hwnd, wnd->strClass, MAX_STRING);

		GetWindowPlacement(hwnd, &wnd->wndPlacement);//该函数返回指定窗口的显示状态以及被恢复的、最大化的和最小化的窗口位置。

		//wnd->bVisible = IsWindowVisible(hwnd);
	}

	return TRUE;
}

DWORD UpdateWindowMetrics(HWND hParent, HWND hwnd, WNDSET *wndSet)
{
	INT i = NULL;
	INT result = NULL;

	//Loop through the WNDSET to find 'hwnd'
	for(i = 0; i < wndSet->nWnd; i++){
		if(wndSet->wnd[i].hwnd == hwnd){

			//Update Window Metrics
			UpdateWindowMetrics(hParent, hwnd, &wndSet->wnd[i]);
		}
	}

	return TRUE;
}

DWORD LoopThroughWNDSET(LPTRFRAME *lpFrame, HWND hwnd)
{

	WNDSET wndSet = {0};
	INT i = 0;
	INT result;

	memcpy(&wndSet, &lpFrame->wndSet, sizeof(WNDSET));

	for(i = 0; i < wndSet.nWnd; i++){
		
		if(IsWindow(wndSet.wnd[i].hwnd)){
			i = i;
			i = i;
		}
		

	}


	return TRUE;
}


LRESULT ReadWindowPositions1(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	INT i = 0;

	CHAR typeData;
	CHAR typeMetaData;
	PDMWINDOW pdmWindow;
	PDMWINDOWDATA *pdmWindowData;
	CHAR str[MAX_STRING];
	VOID *lpvoid;
	RECT rc = {0};
	HWND hDesktop = GetDesktopWindow();

	sprintf(str, "%s%s", lpFrame->strArchivePath, STR_BOOT);
	ReadPDMWindow(&typeData, &typeMetaData, &pdmWindow, str, &lpvoid);

	if(lpvoid){
		INT nWindow = pdmWindow.pdmWindowHeader.nWindows;
		pdmWindowData = (PDMWINDOWDATA *) calloc(nWindow, sizeof(PDMWINDOWDATA));
		memcpy(pdmWindowData, lpvoid, nWindow*sizeof(PDMWINDOWDATA));

		for(i = 0; i < nWindow; i++){


			memcpy(&rc, &pdmWindowData[i].rcWindow, sizeof(RECT));
			switch(pdmWindowData[i].idWindow)
			{
				case HWND_FRAME:
					MoveWindow(hwnd, rc, TRUE);
					//MoveWindow(hwnd, rc.left, rc.top, GetRectWidth(rc), GetRectHeight(rc), TRUE);
					//SetWindowPos(hwnd, NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER);
					ThrowLastError(hwnd);
					GetWindowRect(hwnd, &rc);
					INT i;
					i = 0;
					break;
				case HWND_MANIFEST:
					MoveWindow(lpFrame->hManifest, rc, TRUE);
					//MoveWindow(lpFrame->hManifest, rc.left, rc.top, GetRectWidth(rc), GetRectHeight(rc), TRUE);
					//SetWindowPos(lpFrame->hManifest, NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER);
					break;
				case HWND_COMMANDPROMPT:
					//SetWindowPos(lpFrame->hCommandPrompt, NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER);
					break;
				case HWND_PROPERTYMANAGER:
					//SetWindowPos(lpFrame->hPropertyManager, NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER);
					break;
				case HWND_CAMERAPROPERTIES:
					MoveWindow(lpFrame->hCameraProperties, rc, TRUE);
					//MoveWindow(lpFrame->hCameraProperties, rc.left, rc.top, GetRectWidth(rc), GetRectHeight(rc), TRUE);
					break;
				case HWND_UNO32:
					MoveWindow(lpFrame->hUNO32, rc, TRUE);
					//MoveWindow(lpFrame->hUNO32, rc.left, rc.top, GetRectWidth(rc), GetRectHeight(rc), TRUE);
					break;
				default:
					break;
			}
		}
	}


	return TRUE;
}

LRESULT ReadWindowPositions(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	INT i = 0;
	INT id = 0;
	INT nWindow = NULL;

	CHAR typeData;
	CHAR typeMetaData;
	PDMWINDOW pdmWindow;
	PDMWINDOWDATA *pdmWindowData;
	CHAR str[MAX_STRING];
	VOID *lpvoid;
	RECT rc = {0};
	HWND hDesktop = GetDesktopWindow();

	sprintf(str, "%s%s", lpFrame->strArchivePath, STR_BOOT);
	ReadPDMWindow(&typeData, &typeMetaData, &pdmWindow, str, &lpvoid);

	if(!lpvoid){
		return FALSE;
	}

	nWindow = pdmWindow.pdmWindowHeader.nWindows;
	pdmWindowData = (PDMWINDOWDATA *)calloc(nWindow, sizeof(PDMWINDOWDATA));
	memcpy(pdmWindowData, lpvoid, nWindow*sizeof(PDMWINDOWDATA));

	for(i = 0; i < nWindow; i++){

		memcpy(&rc, &pdmWindowData[i].rcWindow, sizeof(RECT));

		id = GetWndID(lpFrame, (CHAR *)pdmWindowData[i].strText);

		if(id){
			MoveWindow(lpFrame->wndSet.wnd[id-1].hwnd, rc, TRUE);
		}
	}


	return TRUE;
}

//Returns the ith+1 ID of the window
DWORD GetWndID(LPTRFRAME *lpFrame, HWND hwnd)
{
	WNDSET wndSet = {0};
	INT i = 0;
	INT result;

	memcpy(&wndSet, &lpFrame->wndSet, sizeof(WNDSET));

	for(i = 0; i < wndSet.nWnd; i++){
		
		if(wndSet.wnd[i].hwnd == hwnd){
			return i+1;
		}
		

	}

	return FALSE;
}

DWORD GetWndID(LPTRFRAME *lpFrame, CHAR *strText)
{
	WNDSET wndSet = {0};
	INT i = 0;
	INT result;

	memcpy(&wndSet, &lpFrame->wndSet, sizeof(WNDSET));

	for(i = 0; i < wndSet.nWnd; i++){
		
		if(!strcmp(wndSet.wnd[i].strText, strText)){
			return i+1;
		}
		

	}

	return FALSE;
}

DWORD AddWindow(WNDSET *wndSet, HWND hwnd)
{
	HWND hRoot = GetAncestor(hwnd, GA_ROOT);

	wndSet->wnd[wndSet->nWnd].hwnd = hwnd;
	wndSet->nWnd++;

	UpdateWindowMetrics(hRoot, hwnd, wndSet);

	return TRUE;
}