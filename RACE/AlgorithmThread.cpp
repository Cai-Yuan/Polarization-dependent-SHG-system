#include "stdafx.h"

#include "DataView.h"
#include "AlgorithmThread.h"

DWORD WINAPI AlgorithmProc(LPVOID param)
{

	ALGORITHMTHREAD *at  = (ALGORITHMTHREAD *)param;

	UINT w = at->lpDataView->wSource;
	UINT h = at->lpDataView->hSource;

	AlgorithmProcDataView(at->lpDataView, (UCHAR *) at->data, NULL);

	SendMessage(at->hwnd, WM_DRAW, NULL, NULL);

	if(at->data) free(at->data);
	if(at) free(at);

	return TRUE;

}

