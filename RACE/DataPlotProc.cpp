#include "stdafx.h"

#include "stdafx.h"
#include "DataPlot.h"

LRESULT CALLBACK DataPlotProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATAPLOT *lphwnd = (DATAPLOT *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	switch(msg)
	{
	case WM_SIZE:
		{
			return OnSizeDataPlot(lphwnd, hwnd, wParam, lParam);
		}
	case WM_CREATE:
		{
			return OnCreateDataPlot(lphwnd, hwnd, wParam, lParam);
		}
	case WM_NCCREATE:
		{
			lphwnd = (DATAPLOT *) InitWindowLongRACE(hwnd, sizeof(DATAPLOT));
			return TRUE;
		}	
	case WM_CLOSE:
		{
			return OnCloseDataPlot(lphwnd, hwnd, wParam, lParam);
		}
	case WM_DESTROY:
		{
			return OnDestroyDataPlot(lphwnd, hwnd, wParam, lParam);
		}
	default:
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT OnSizeDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam){

	INT wNew;
	INT hNew;
	RECT rcClient;


	GetClientRect(hwnd, &rcClient);
	lphwnd->wPlot = rcClient.right-rcClient.left;
	lphwnd->hPlot = rcClient.bottom-rcClient.top;

	switch(wParam){
		case SIZE_RESTORED:
			break;
		case SIZE_MINIMIZED:
			break;
		case SIZE_MAXSHOW:
			break;
		case SIZE_MAXIMIZED:
			break;
		case SIZE_MAXHIDE:
			break;
		default:
			break;
	}
	

	return TRUE;
}

LRESULT OnCreateDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam){


	return TRUE;
}

LRESULT OnCloseDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam){


	return TRUE;
}

LRESULT OnDestroyDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam){


	return TRUE;
}




//LRESULT DataPlot(DATAPLOT *lphwnd, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
//
//
//	return TRUE;
//}
//
//LRESULT DataPlot(DATAPLOT *lphwnd, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
//
//
//	return TRUE;
//}
//
//LRESULT DataPlot(DATAPLOT *lphwnd, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
//
//
//	return TRUE;
//}