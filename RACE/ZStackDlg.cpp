#include "stdafx.h"

#include "ZStackDlg.h"


INT_PTR CALLBACK ZStackProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ZSTACKTAB *lpZStackTab = (ZSTACKTAB *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpZStackTab = (ZSTACKTAB *)calloc(1, sizeof(ZSTACKTAB));
		memset(lpZStackTab, 0x00, sizeof(ZSTACKTAB));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpZStackTab);
		lpZStackTab = (ZSTACKTAB *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpZStackTab == NULL){
			ThrowMessage(hwnd, "WndAUTOFOCUSTAB: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndAUTOFOCUSTAB: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpZStackTab->hOwner = (HWND) psp.lParam;

		lpZStackTab->hZeissMicroscope = (HWND) SendMessage(lpZStackTab->hOwner, WM_GETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, NULL);
		
		lpZStackTab->hOwner = (HWND) psp.lParam;
		lpZStackTab->hEditNSlice = GetDlgItem(hwnd, IDC_EDIT_NUMSLICE);
		lpZStackTab->hEditDistance = GetDlgItem(hwnd, IDC_EDIT_DISTANCE);

		lpZStackTab->zs.nSlice = 10;
		lpZStackTab->zs.distance = 50;

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "%d", lpZStackTab->zs.nSlice);
		Edit_SetText(lpZStackTab->hEditNSlice, str);
		sprintf(str, "%d", lpZStackTab->zs.distance);
		Edit_SetText(lpZStackTab->hEditDistance, str);


		break;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
			
			case EN_CHANGE:
				if(LOWORD(wParam) == IDC_EDIT_NUMSLICE){
					Edit_GetText(lpZStackTab->hEditNSlice, str, MAX_STRING);
					sscanf(str, "%d", &lpZStackTab->zs.nSlice);
				}
				if(LOWORD(wParam) == IDC_EDIT_DISTANCE){
					Edit_GetText(lpZStackTab->hEditDistance, str, MAX_STRING);
					sscanf(str, "%d", &lpZStackTab->zs.distance);
				}
				
				SendMessage(lpZStackTab->hZeissMicroscope, ZSM_SETZSTACK, (WPARAM) &lpZStackTab->zs, NULL);
				break;
			default:
				break;
		}
		break;
	case WM_NOTIFY:
		{
			PSHNOTIFY pshNotify = {0};
			memcpy(&pshNotify, (VOID *)lParam, sizeof(PSHNOTIFY));

			INT i = 0;
			switch(pshNotify.hdr.code)
			{
			case PSN_APPLY:
				return DialogReturn(hwnd, PSNRET_INVALID_NOCHANGEPAGE); /*PSNRET_NOERROR*/
			case PSN_QUERYCANCEL:
					SendMessage(lpZStackTab->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);

					//ShowWindow(pshNotify.hdr.hwndFrom, SW_HIDE);
				return DialogReturn(hwnd, PSNRET_INVALID);
			case PSN_KILLACTIVE:
				return DialogReturn(hwnd, FALSE);
			case PSN_SETACTIVE:
				return TRUE;
			default:
				break;
			}

		}
		return (LRESULT) FALSE;
	default:
		return FALSE;
	}


	return FALSE;

}