#include "stdafx.h"
#include "CaptureThreads.h"


HWND GetLinkedProc(HWND hwnd, BOOL *flag)
{
	HWND hLink = NULL;
	HWND hFrame = NULL;
	HWND hStageControls = NULL;
	hFrame = (HWND) SendMessage(hwnd, WM_GETHWND, HWND_FRAME, NULL);
	hStageControls = (HWND)SendMessage(hwnd, WM_GETHWND, HWND_STAGECONTROLS, NULL);
	hLink = (HWND) SendMessage(hStageControls, WM_GETHWND, HWND_ACTIVESTAGE, NULL);

	if(!hLink)
		return NULL;

	flag = (BOOL *)SendMessage(hLink, WM_GETLINKFLAG, NULL, (LPARAM) NULL);

	return hLink;

}

DWORD ReleaseLinkedData(HWND hLink, PDMFRAME *pdmFrame)
{
	DWORD dataSize = pdmFrame->pdmfh.width*pdmFrame->pdmfh.height;
	PDMFRAME *pdmFrameDrop = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
	pdmFrameDrop->lpvoid = (VOID *)malloc(dataSize);

	memcpy(pdmFrameDrop->lpvoid, pdmFrame->lpvoid, dataSize);
	PostMessage(hLink, WM_DATADROP, (WPARAM) pdmFrameDrop, NULL);

	return TRUE;

}