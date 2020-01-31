#include "stdafx.h"

#include "ProgressView.h"

LRESULT CALLBACK ProgressViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PROGRESSVIEW *lpProgressView = (PROGRESSVIEW *) GetWindowLongPtr(hwnd, GWLP_USERDATA);


	switch(msg)
	{
	case WM_SIZE:

		break;
	case WM_SETHWND:
		
		switch(wParam)
		{
		case HWND_OWNER:
			lpProgressView->hOwner = (HWND) lParam;
			break;
		default:
			return TRUE;
		}

		return TRUE;
	case PBM_SETRANGE:
		SendMessage(lpProgressView->hProgress, msg, wParam, lParam);
		break;
	case PBM_SETPOS:
		SendMessage(lpProgressView->hProgress, msg, wParam, lParam);
		break;
	case WM_CREATE:
		
		//SetWindowLong(hwnd, GWL_STYLE, WS_DLGFRAME);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 500, 100, SWP_SHOWWINDOW);
		lpProgressView->hProgress = CreateWindowEx(NULL, PROGRESS_CLASS, "",
													WS_CHILD | WS_VISIBLE | PBS_MARQUEE,
													0, 0, 500, 25,
													hwnd,
													(HMENU) IDC_PROGRESS,
													GetModuleHandle(NULL),
													NULL);

		break;
	case WM_NCCREATE:
		lpProgressView = (PROGRESSVIEW *)calloc(1, sizeof(PROGRESSVIEW));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpProgressView);
		if(!lpProgressView){
			MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "ProgressView Error!", MB_OK);
		}
		break;
	case WM_CLOSE:
		//ShowWindow(hwnd, SW_HIDE);
		//return TRUE;
		break;
	default:
		break;
	}


	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}