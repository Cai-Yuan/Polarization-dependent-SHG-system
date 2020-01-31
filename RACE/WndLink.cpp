#include "stdafx.h"

#include "WndLink.h"
#include "WinUser.h"

DWORD AllocWndLink(WNDLINKSET *wls, DWORD lenLinkSet)
{
	if(!wls)
		return FALSE;

	if(wls->wndLink)
		return FALSE;

	wls->wndLink = (WNDLINK *)malloc(lenLinkSet*sizeof(WNDLINK));
	memset(wls->wndLink, NULL, lenLinkSet*sizeof(WNDLINK));

	wls->nActiveLink = NULL;
	wls->lenLinkSet = lenLinkSet;


	return TRUE;
}

DWORD AddWndLink(WNDLINKSET *wls, HWND hLink)
{

	if(!wls || !wls->wndLink)
		return FALSE;

	wls->wndLink[wls->nActiveLink].hLink = hLink;
	GetClassName(hLink, wls->wndLink[wls->nActiveLink].strClass, MAX_STRING);
	GetWindowText(hLink, wls->wndLink[wls->nActiveLink].strTitle, MAX_STRING);

	wls->nActiveLink++;

	return TRUE;
}

DWORD AddLink(HWND hwnd, HWND hLink, WNDLINKSET *wls, DWORD flgCallBack)
{
	if(!AddWndLink(wls, hLink))
		return FALSE;

	if(flgCallBack){
		SendLink(hwnd, hLink, FALSE);
	}
	
	return TRUE;
}

DWORD SendLink(HWND hwnd, HWND hLink, DWORD flgCallBack)
{
	SendMessage(hLink, WM_CREATELINK, (WPARAM) hwnd, (LPARAM) flgCallBack);

	return TRUE;
}

LRESULT OnCreateLink(WNDLINKSET *wls, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hLink = (HWND) wParam;
	DWORD flgCallBack = (DWORD) lParam;
	AddLink(hwnd, hLink, wls, flgCallBack);

	return TRUE;
}

HWND GetLinkHWND(WNDLINKSET *wls, CHAR *str, DWORD flgSearch)
{
	HWND hwnd = NULL;
	INT i = NULL;

	if(!wls)
		return FALSE;

	//Search By Class
	if(!flgSearch){
		for(i = 0; i < wls->nActiveLink; i++){
			if(!strcmp(str, wls->wndLink[i].strClass)){
				return wls->wndLink[i].hLink;
			}
		}

	//Search By WndText
	}else{
		return NULL;
	}
	return hwnd;
}

DWORD RelayMessage(WNDLINKSET *wls, CHAR *strWndText, CHAR *strClass, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!wls)
		return FALSE;

	HWND hLink = NULL;

	if(strClass){
		hLink = GetLinkHWND(wls, strClass, FALSE);
		SendMessage(hLink, msg, wParam, lParam);
	}


	return TRUE;
}

LRESULT CreateLink(HWND hwnd, HWND hLink, DWORD flgCallBack)
{
	return SendMessage(hwnd, WM_CREATELINK, (WPARAM) hLink, (LPARAM) flgCallBack);

}
