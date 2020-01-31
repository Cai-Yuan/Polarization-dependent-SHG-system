
#include "stdafx.h"
#include "HistogramView.h"

LRESULT CALLBACK HistogramViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HISTOGRAMVIEW *lpHistogramView = (HISTOGRAMVIEW *) GetWindowLongPtr(hwnd, GWLP_USERDATA);


	switch(msg)
	{
	case WM_SIZE:

		break;
	case WM_SETHWND:
		
		switch(wParam)
		{
		case HWND_OWNER:
			lpHistogramView->hOwner = (HWND) lParam;
			break;
		default:
			return TRUE;
		}

		return TRUE;

	case WM_CREATE:
		
		//SetWindowLong(hwnd, GWL_STYLE, WS_DLGFRAME);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 256, 256, SWP_NOZORDER);

		break;
	case WM_NCCREATE:
		lpHistogramView = (HISTOGRAMVIEW *)calloc(1, sizeof(HISTOGRAMVIEW));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpHistogramView);
		if(!lpHistogramView){
			MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "HistogramView Error!", MB_OK);
		}
		break;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return TRUE;
		break;
	default:
		break;
	}


	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}