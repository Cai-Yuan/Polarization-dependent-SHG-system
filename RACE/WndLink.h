#pragma once

#define MAX_LINK 10

typedef struct WNDLINK_{
	HWND hLink;
	CHAR strTitle[MAX_STRING];
	CHAR strClass[MAX_STRING];
}WNDLINK;

typedef struct WNDLINKSET_{
	WNDLINK *wndLink;
	DWORD lenLinkSet;
	DWORD nActiveLink;
}WNDLINKSET;

DWORD AllocWndLink(WNDLINKSET *wls, DWORD lenLinkSet);
DWORD AddWndLink(WNDLINKSET *wls, HWND hLink);
DWORD AddLink(HWND hwnd, HWND hLink, WNDLINKSET *wls, DWORD flgCallBack);
//DWORD CreateLink(HWND hwnd, HWND hLink, DWORD flgCallBack);
DWORD SendLink(HWND hwnd, HWND hLink, DWORD flgCallBack);
HWND GetLinkHWND(WNDLINKSET *wls, CHAR *str, DWORD flgSearch);

LRESULT CreateLink(HWND hwnd, HWND hLink, DWORD flgCallBack);
LRESULT OnCreateLink(WNDLINKSET *wls, HWND hwnd, WPARAM wParam, LPARAM lParam);

DWORD RelayMessage(WNDLINKSET *wls, CHAR *strWndText, CHAR *strClass, UINT msg, WPARAM wParam, LPARAM lParam);