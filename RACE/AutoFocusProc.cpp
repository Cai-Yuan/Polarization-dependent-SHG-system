#include "stdafx.h"

#include "AutoFocusProc.h"


INT_PTR CALLBACK AutoFocusProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int flgInit = FALSE;
	LM_AUTOFOCUS *lpWnd = (LM_AUTOFOCUS *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		if(flgInit)
			return 0;

		flgInit = TRUE;

		lpWnd = (LM_AUTOFOCUS *) InitWindowLongRACE(hwnd, sizeof(LM_AUTOFOCUS));

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpWnd->hOwner = (HWND) psp.lParam;

		lpWnd->hCheckLaplacian = GetDlgItem(hwnd, IDC_CHECK_LAPLACIAN);
		lpWnd->hCheckSobel = GetDlgItem(hwnd, IDC_CHECK_SOBEL);
		lpWnd->hCheckVariance = GetDlgItem(hwnd, IDC_CHECK_VARIANCE);

		lpWnd->hEditStep = GetDlgItem(hwnd, IDC_EDIT_STEP);
		lpWnd->hEditDistance = GetDlgItem(hwnd, IDC_EDIT_DISTANCE);

		lpWnd->hBtnAutoFocus = GetDlgItem(hwnd, IDC_BTN_AUTOFOCUS);
		
		lpWnd->hBtnAutoFocus = lpWnd->hBtnAutoFocus;

		lpWnd->af.nStep = 25;
		lpWnd->af.stepDist = 1.0;
		lpWnd->af.filter = AF_SOBEL;

		SetDlgItemInt(hwnd, IDC_EDIT_STEP, lpWnd->af.nStep, TRUE);
		Edit_SetFloat(lpWnd->hEditDistance, lpWnd->af.stepDist, 2);
		SetDlgItemInt(hwnd, IDC_EDIT_DISTANCE, lpWnd->af.stepDist, TRUE);

		Button_SetCheck(lpWnd->hCheckLaplacian, TRUE);

		SendMessage(lpWnd->hOwner, WM_SETHWND, HWND_AUTOFOCUS, (LPARAM) hwnd);

		break;
	case WM_AUTOFOCUS:
		if(!lpWnd->af.filter){
			MessageBox(hwnd, "Please Select a Focal Measure", "AutoFocus Error", MB_OK);
			return FALSE;
		}

		if(lpWnd->flgAutoFocus)
			lpWnd->flgAutoFocus = FALSE;

		Sleep(100);
		lpWnd->flgAutoFocus = TRUE;
		lpWnd->flgInitAutoFocus = TRUE;

		return TRUE;
	case WM_COMMAND:

		switch(HIWORD(wParam)){
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_BTN_AUTOFOCUS){
				SendMessage(hwnd, WM_AUTOFOCUS, (WPARAM) &lpWnd->af, TRUE);
			}
			if(LOWORD(wParam) == IDC_CHECK_LAPLACIAN){
				lpWnd->af.filter = UpdateCheckBoxFilter(lpWnd->hCheckLaplacian, lpWnd->af.filter, AF_LAPLACIAN);
				lpWnd->af.filter = lpWnd->af.filter;
			}
			if(LOWORD(wParam) == IDC_CHECK_SOBEL){
				lpWnd->af.filter = UpdateCheckBoxFilter(lpWnd->hCheckSobel, lpWnd->af.filter, AF_SOBEL);
				lpWnd->af.filter= lpWnd->af.filter;
			}
			if(LOWORD(wParam) == IDC_CHECK_VARIANCE){
				lpWnd->af.filter = UpdateCheckBoxFilter(lpWnd->hCheckVariance, lpWnd->af.filter, AF_VARIANCE);
				lpWnd->af.filter= lpWnd->af.filter;
			}

			break;
		case EN_CHANGE:
			if(LOWORD(wParam) == IDC_EDIT_STEP){
				lpWnd->af.nStep = GetDlgItemInt(hwnd, IDC_EDIT_STEP, NULL, FALSE);
			}
			if(LOWORD(wParam) == IDC_EDIT_DISTANCE){
				lpWnd->af.stepDist = Edit_GetFloat(lpWnd->hEditDistance);
				//lpWnd->af.stepDist = GetDlgItemInt(hwnd, IDC_EDIT_DISTANCE, NULL, FALSE);

			}
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
					//SendMessage(lpWnd->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);
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
			lpWnd->hOwner = (HWND) lParam;
			return TRUE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}


	return FALSE;

}