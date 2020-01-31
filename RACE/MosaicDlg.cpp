#include "stdafx.h"

#include "MosaicDlg.h"

INT_PTR CALLBACK MosaicProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MOSAICTAB *lpMosaicTab = (MOSAICTAB *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpMosaicTab = (MOSAICTAB *)calloc(1, sizeof(MOSAICTAB));
		memset(lpMosaicTab, 0x00, sizeof(MOSAICTAB));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpMosaicTab);
		lpMosaicTab = (MOSAICTAB *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpMosaicTab == NULL){
			ThrowMessage(hwnd, "WndMosaicTab: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndMosaicTab: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpMosaicTab->hOwner = (HWND) psp.lParam;

		//lpMosaicTab->hCheckAutoFocus = GetDlgItem(hwnd, IDC_CHECK_AUTOFOCUS);
		//lpMosaicTab->hCheckFocalRecall = GetDlgItem(hwnd, IDC_CHECK_FOCALRECALL);

		lpMosaicTab->hEditRows = GetDlgItem(hwnd, IDC_EDIT_ROWS);
		lpMosaicTab->hEditCols = GetDlgItem(hwnd, IDC_EDIT_COLS);
		lpMosaicTab->hEditOverlap = GetDlgItem(hwnd, IDC_EDIT_OVERLAP);

		lpMosaicTab->hRadioStitched = GetDlgItem(hwnd, IDC_RADIO_STITCHED);
		lpMosaicTab->hRadioUnstitched = GetDlgItem(hwnd, IDC_RADIO_UNSTITCHED);

		lpMosaicTab->hZeissMicroscope = (HWND) SendMessage(lpMosaicTab->hOwner, WM_GETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, NULL);
		
		lpMosaicTab->fov.nRow = 5;
		lpMosaicTab->fov.nCol = 5;
		lpMosaicTab->fov.overlap = 0.05;
		SendMessage(lpMosaicTab->hZeissMicroscope, ZSM_SETFIELDOFVIEW, (WPARAM) &lpMosaicTab->fov, NULL);

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "%d", lpMosaicTab->fov.nRow);
		Edit_SetText(lpMosaicTab->hEditRows, str);
		sprintf(str, "%d", lpMosaicTab->fov.nCol);
		Edit_SetText(lpMosaicTab->hEditCols, str);
		sprintf(str, "%.2f", lpMosaicTab->fov.overlap);
		Edit_SetText(lpMosaicTab->hEditOverlap, str);

		Button_SetCheck(lpMosaicTab->hRadioUnstitched, TRUE);

		break;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
			
			case EN_CHANGE:
				if(LOWORD(wParam) == IDC_EDIT_ROWS){
					Edit_GetText(lpMosaicTab->hEditRows, str, MAX_STRING);
					sscanf(str, "%d", &lpMosaicTab->fov.nRow);
				}
				if(LOWORD(wParam) == IDC_EDIT_COLS){
					Edit_GetText(lpMosaicTab->hEditCols, str, MAX_STRING);
					sscanf(str, "%d", &lpMosaicTab->fov.nCol);
				}
				if(LOWORD(wParam) == IDC_EDIT_OVERLAP){
					Edit_GetText(lpMosaicTab->hEditOverlap, str, MAX_STRING);
					sscanf(str, "%f", &lpMosaicTab->fov.overlap);
				}
				lpMosaicTab->fov.nRow = lpMosaicTab->fov.nRow;
				lpMosaicTab->fov.nCol = lpMosaicTab->fov.nCol;
				lpMosaicTab->fov.overlap = lpMosaicTab->fov.overlap;
				lpMosaicTab->fov.overlap = lpMosaicTab->fov.overlap;
				
				SendMessage(lpMosaicTab->hZeissMicroscope, ZSM_SETFIELDOFVIEW, (WPARAM) &lpMosaicTab->fov, NULL);
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
					SendMessage(lpMosaicTab->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);

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
	case WM_SETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			lpMosaicTab->hOwner = (HWND) lParam;
			return TRUE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}


	return FALSE;

}