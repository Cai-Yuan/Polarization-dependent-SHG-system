#include "stdafx.h"

#include "DataBar.h"
#include "DataView.h"
#include "Subclass.h"
#include "String.h"
#include "RACE.h"
#include "TreeViewFxns.h"


LRESULT CALLBACK DataBarSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATABAR *lpDataBar = (DATABAR *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_SETCURSOR:
		HCURSOR hCursor;
		hCursor = NULL;
		
		if((HWND)wParam == lpDataBar->hTabControl){
			
			switch(LOWORD(lParam))
			{
			case HTLEFT:
				hCursor = LoadCursor(NULL, IDC_SIZEWE);
				break;
			default:
				hCursor = LoadCursor(NULL, IDC_ARROW);
				break;
			}
			SetCursor(hCursor);
			return TRUE;
		}
		else
			return FALSE;

		break;
	case WM_SETFRAMEFOCUS:
		{
			SendMessage(lpDataBar->hSlider, TBM_SETPOS, TRUE, wParam);
		}	
		return TRUE;
	case CB_SETCURSEL:
		{
			SendMessage(lpDataBar->hTabControl, msg, wParam, lParam);
		}
		return TRUE;
	case WM_SETHWND:
		{
			if(wParam == HWND_ACTIVE){
				lpDataBar->hDataView = (HWND) lParam;
				SendMessage(lpDataBar->hTabControl, WM_SETHWND, HWND_ACTIVE, lParam);
			}
			if(wParam == HWND_FRAME)
				lpDataBar->hFrame = (HWND) lParam;
		}
		break;
	case CB_ADDSTRING:
		{
			SendMessage(lpDataBar->hTabControl, msg, wParam, lParam);
		}
		return TRUE;
	case WM_NOTIFY:
		{
			INT iTab;
			iTab = 0;
			NMHDR nmhdr;
			memcpy(&nmhdr, (VOID *)lParam, sizeof(NMHDR));

			switch(nmhdr.code)
			{
			case TCN_SELCHANGE:
				iTab = SendMessage(lpDataBar->hTabControl, TCM_GETCURSEL, NULL, NULL);
				wParam = MAKEWPARAM(DT_SELTABCHANGE, DT_SELTABCHANGE);
				SendMessage(lpDataBar->hTabControl, WM_COMMAND, wParam, (LPARAM) iTab);
				break;
			default:
				break;
			}
			
			iTab = iTab;
		}
		break;
	case DB_ADDSTRINGSET:
		{
			UINT f;
			UINT nPaths;
			CHAR str[MAX_STRING];
			FILEPATHSET *fps;

			//Delete All Previous Strings
			SendMessage(lpDataBar->hTabControl, DT_RESETCONTENT/*DT_DELETESTRINGSET*/, NULL, NULL);

			if(wParam){
				memcpy((VOID *)&lpDataBar->fps, (VOID *)wParam, sizeof(FILEPATHSET));
				nPaths = lpDataBar->fps.nPaths;
				for(f = 0; f < nPaths; f++){
					FileNameFromPath(MAX_STRING, lpDataBar->fps.filePath[f].lpstr, str);
					SendMessage(hwnd, CB_ADDSTRING, NULL, (LPARAM) str);
				}
			}
		}
		break;
	case PBM_SETRANGE:
		{
			lpDataBar->fProgress = TRUE;
			ShowWindow(lpDataBar->hProgress, SW_SHOW);
			SendMessage(lpDataBar->hProgress, PBM_SETRANGE, wParam, lParam);
			SendMessage(hwnd, WM_SIZE, NULL, NULL);
		}
		return TRUE;
	case PBM_SETPOS:
		{
			INT prevPos;
			INT rLower;
			INT rUpper;
			rLower = SendMessage(lpDataBar->hProgress, PBM_GETRANGE, TRUE, NULL);
			rUpper = SendMessage(lpDataBar->hProgress, PBM_GETRANGE, FALSE, NULL);
			prevPos = SendMessage(lpDataBar->hProgress, PBM_SETPOS, wParam, lParam);
			prevPos = prevPos;
			return prevPos;
		}
	case PBM_GETRANGE:
		{
			return SendMessage(lpDataBar->hProgress, msg, wParam, lParam);
		}
	case WM_ENDTRANSMISSION:
		{
			lpDataBar->fProgress = FALSE;
			ShowWindow(lpDataBar->hProgress, SW_HIDE);
			SendMessage(hwnd, WM_SIZE, NULL, NULL);
		}
		break;
	case WM_SIZE:
		{
			INT hProgress = NULL;
			INT hClient = NULL;
			//INT hTree = NULL;
		
			RECT rcClient;
			GetClientRect(hwnd, &rcClient);
			hClient = rcClient.bottom-rcClient.top;

			RECT rcSlider;
			GetWindowRect(lpDataBar->hSlider, &rcSlider);

			RECT rcProgress;
			GetWindowRect(lpDataBar->hProgress, &rcProgress);

			INT wSlider = WIDTH_SLIDER;
			INT wClient = rcClient.right-rcClient.left;
		
			if(lpDataBar->fProgress)
				hProgress = HEIGHT_PROGRESS;


			SetWindowPos(lpDataBar->hTabControl, 
						NULL, 
						0, 0, wClient-wSlider, (hClient - hProgress),
						SWP_NOZORDER);

			SetWindowPos(lpDataBar->hSlider,
						NULL,
						rcClient.right-wSlider, 0, wSlider, rcClient.bottom - hProgress,
						SWP_NOZORDER);

			SetWindowPos(lpDataBar->hProgress,
						NULL,
						rcClient.left, rcClient.bottom-hProgress, rcClient.right-rcClient.left, hProgress,
						SWP_NOZORDER);

			SendMessage(lpDataBar->hTabControl, WM_SIZE, NULL, NULL);

			break;
		}
	case WM_INITSUBCLASS:
		{

		lpDataBar = (DATABAR *)calloc(1, sizeof(DATABAR));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpDataBar);
		if(!lpDataBar){
			ThrowMessage(hwnd, (CHAR *) "Error DataBarSubclass: GetWindowLongPtr Failed");
		}

		lpDataBar->wpOriginal = (WNDPROC) wParam;
		lpDataBar->hDataView = (HWND) lParam;

		//Create Subcontrols
		//Create TabControl to show loaded files
		lpDataBar->hTabControl = CreateWindowEx(NULL, WC_TABCONTROL,
												TEXT(""),
												WS_CHILD|WS_OVERLAPPED|WS_VISIBLE, 
												0, 0, 200, 200,
												hwnd, 
												NULL,
												GetModuleHandle(NULL),
												NULL);
		
		InitializeTabControl(lpDataBar->hTabControl);	

		SubclassWnd(hwnd, lpDataBar->hTabControl, DataTabSubclassProc);

		lpDataBar->hSlider = CreateWindowEx(0,
										TRACKBAR_CLASS,
										"Trackbar",
										WS_CHILD|
										WS_VISIBLE|
										TBS_AUTOTICKS|
										TBS_VERT|
										TBS_ENABLESELRANGE,
										CW_USEDEFAULT, 
										CW_USEDEFAULT, 
										CW_USEDEFAULT, 
										CW_USEDEFAULT,
										hwnd,
										(HMENU)NULL,
										(HINSTANCE)GetModuleHandle(0),
										NULL);

		lpDataBar->hProgress = CreateWindowEx(NULL, PROGRESS_CLASS, "ProgressBar",
											WS_CHILD | WS_VISIBLE/* | PBS_MARQUEE*/,
											0, 0, 500, 25,
											hwnd,
											(HMENU) IDC_PROGRESS,
											GetModuleHandle(NULL),
											NULL);

		SendMessage(lpDataBar->hProgress, PBM_SETSTATE, PBST_NORMAL, NULL);
		//SendMessage(lpDataBar->hProgress, PBM_SETBKCOLOR, NULL, RGB(255, 255, 0));

		SendMessage(hwnd, WM_SIZE, NULL, NULL);
		SendMessage(lpDataBar->hTabControl, WM_SIZE, NULL, NULL);

			return TRUE;
		}
	case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				if(lParam == (LPARAM) lpDataBar->hButtonHide){
					//lpDataBar->isHidden = !lpDataBar->isHidden;

					//if(lpDataBar->isHidden)
					//	SendMessage(lpDataBar->hDataView, WM_SIZEDATABAR, (WPARAM) DB_COLLAPSE, NULL);	//Change to a WM_COMMAND Message
					//else
					//	SendMessage(lpDataBar->hDataView, WM_SIZEDATABAR, (WPARAM) DB_EXPAND, NULL);	
				}
				break;
			case CBN_SELCHANGE:
				SendMessage(lpDataBar->hDataView, msg, wParam, lParam);
				return TRUE;
			default:
				break;
			}

			switch(LOWORD(wParam))
			{
			case DBN_SETSTART:
				lpDataBar->fStart = (INT) lParam;
				break;
			case DBN_SETSTOP:
				LRESULT lResult;
				lpDataBar->fStop = (INT) lParam;

				lResult = SendMessage(lpDataBar->hSlider, 
										TBM_SETRANGE, 
										TRUE, 
										MAKELPARAM(lpDataBar->fStart, lpDataBar->fStop));

				lResult = SendMessage(lpDataBar->hSlider,
										TBM_SETSEL,
										TRUE,
										MAKELPARAM(lpDataBar->fStart+2, lpDataBar->fStop-2));
				lResult = lResult;
				break;

			default:
				break;
			}
		}
	case WM_VSCROLL:
		{
			INT pos = 0;
			INT upper;
			INT lower;

			GetScrollRange(lpDataBar->hSlider, SB_VERT, &lower, &upper);

			
			switch(LOWORD(wParam))
			{
			case TB_LINEDOWN:
				pos = SendMessage(lpDataBar->hSlider, TBM_GETPOS, NULL, NULL);
				SendMessage(lpDataBar->hDataView, WM_SETFRAMEFOCUS, pos, NULL);
				return TRUE;
			case TB_LINEUP:
				pos = SendMessage(lpDataBar->hSlider, TBM_GETPOS, NULL, NULL);
				SendMessage(lpDataBar->hDataView, WM_SETFRAMEFOCUS, pos, NULL);
				return TRUE;
			case TB_PAGEDOWN:
				pos = SendMessage(lpDataBar->hSlider, TBM_GETPOS, NULL, NULL);
				SendMessage(lpDataBar->hDataView, WM_SETFRAMEFOCUS, pos, NULL);
				return TRUE;
			case TB_PAGEUP:
				pos = SendMessage(lpDataBar->hSlider, TBM_GETPOS, NULL, NULL);
				SendMessage(lpDataBar->hDataView, WM_SETFRAMEFOCUS, pos, NULL);
				return TRUE;
			case TB_THUMBPOSITION:
				pos = SendMessage(lpDataBar->hSlider, TBM_GETPOS, NULL, NULL);
				SendMessage(lpDataBar->hDataView, WM_SETFRAMEFOCUS, pos, NULL);
				return TRUE;
			case TB_THUMBTRACK:
				pos = SendMessage(lpDataBar->hSlider, TBM_GETPOS, NULL, NULL);
				SendMessage(lpDataBar->hDataView, WM_SETFRAMEFOCUS, pos, NULL);
				return TRUE;
			default:
				break;
			}

		}
		break;
	default:
		{
			if(lpDataBar)
				return CallWindowProc(lpDataBar->wpOriginal, hwnd, msg, wParam, lParam);
			else
				return DefWindowProc(hwnd, msg, wParam, lParam);
			/*DefWindowProc(hwnd, msg, wParam, lParam)*/
		}
	}

	if(lpDataBar)
		return CallWindowProc(lpDataBar->wpOriginal, hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
	/*DefWindowProc(hwnd, msg, wParam, lParam)*/
}


