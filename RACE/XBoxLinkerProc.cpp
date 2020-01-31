#include "stdafx.h"

#include "XBox.h"
#include "XBoxLinker.h"

INT_PTR CALLBACK XBoxLinkerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	XBOXLINKER *lpXBoxLinker = (XBOXLINKER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	INT sel;
	INT cnt;
	switch(msg)
	{
	case WM_INITDIALOG:

		lpXBoxLinker = (XBOXLINKER *)calloc(1, sizeof(XBOXLINKER));
		memset(lpXBoxLinker, 0x00, sizeof(XBOXLINKER));

		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpXBoxLinker);
		lpXBoxLinker = (XBOXLINKER *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpXBoxLinker == NULL){
			ThrowMessage(hwnd, "WndXBoxLinker: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndXBoxLinker: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}


		lpXBoxLinker->hList = GetDlgItem(hwnd, IDC_LIST);

		SendMessage(lpXBoxLinker->hList, LB_ADDSTRING, NULL, (LPARAM)"Cursor");
		SendMessage(lpXBoxLinker->hList, LB_ADDSTRING, NULL, (LPARAM)"Data View");
		SendMessage(lpXBoxLinker->hList, LB_ADDSTRING, NULL, (LPARAM)"UNO32");

		SendMessage(lpXBoxLinker->hList, LB_SETCURSEL, (WPARAM) 2/*LP_CURSOR*/, NULL);

		lpXBoxLinker->hThread = CreateThread(NULL, NULL, XBoxCaptureThread, (LPVOID) hwnd, NULL, NULL);

		break;
	case XB_LISTSELUP:
		cnt = SendMessage(lpXBoxLinker->hList, LB_GETCOUNT, NULL, NULL);
		sel = SendMessage(lpXBoxLinker->hList, LB_GETCURSEL, NULL, NULL);

		sel--;
		if(sel < 0)
			sel = 0;

		SendMessage(lpXBoxLinker->hList, LB_SETCURSEL, (WPARAM)sel, NULL);

		lpXBoxLinker->fXBoxLink = sel;
		break;
	case XB_LISTSELDOWN:
		cnt = SendMessage(lpXBoxLinker->hList, LB_GETCOUNT, NULL, NULL);
		sel = SendMessage(lpXBoxLinker->hList, LB_GETCURSEL, NULL, NULL);

		sel++;
		if(sel > cnt-1)
			sel = cnt-1;

		SendMessage(lpXBoxLinker->hList, LB_SETCURSEL, (WPARAM)sel, NULL);

		lpXBoxLinker->fXBoxLink = sel;
		break;
	case WM_COMMAND:

		switch(HIWORD(wParam))
		{
		case EN_CHANGE:
			return TRUE;
		case CBN_SELCHANGE:
			break;
		default:
			break;
		}

		switch(LOWORD(wParam))
		{
		case IDSET:
			return TRUE;
		case IDC_BUTTON_LOOP:
			return TRUE;
		default:
			break;
		}

		break;

	case XB_INPUT:
		{

			if(wParam){
				memcpy((LPVOID)&lpXBoxLinker->xStatePrev, (LPVOID)&lpXBoxLinker->xState, sizeof(XINPUT_STATE));
				memcpy((LPVOID)&lpXBoxLinker->xState, (LPVOID) wParam, sizeof(XINPUT_STATE));
				//RemoveStickDeadZone(7000, &lpXBoxLinker->xState);
				TranslateXBoxState(lpXBoxLinker->hLink, lpXBoxLinker->xStatePrev, lpXBoxLinker->xState);
			}

		}
		break;
	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpXBoxLinker->hOwner = (HWND) lParam;
		if(wParam == HWND_ACTIVE)
			lpXBoxLinker->hActive = (HWND) lParam;
		if(wParam == HWND_LINK)
			lpXBoxLinker->hLink = (HWND) lParam;

		return TRUE;
	case WM_CLOSE:

		SendMessage(lpXBoxLinker->hOwner, WM_COMMAND, LOWORD(ID_VIEW_XBOXLINKER), (LPARAM) hwnd);

		return TRUE;

	case WM_DESTROY:
		DWORD value;
		ShowWindow(hwnd, SW_HIDE);
		value = TerminateThread(lpXBoxLinker->hThread, NULL);
		Sleep(300);
		PostQuitMessage(0);
		return FALSE;
	default:
		return FALSE;
	}


	return FALSE;

}