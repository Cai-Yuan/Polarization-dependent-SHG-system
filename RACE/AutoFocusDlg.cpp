#pragma once

#include "stdafx.h"
#include "AutoFocusDlg.h"
#include "ZeissMicroscope.h"
#include "ToolsWin32.h"

INT_PTR CALLBACK AutoFocusProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	AUTOFOCUSTAB *lpAutoFocusTab = (AUTOFOCUSTAB *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		lpAutoFocusTab = (AUTOFOCUSTAB *)calloc(1, sizeof(AUTOFOCUSTAB));
		memset(lpAutoFocusTab, 0x00, sizeof(AUTOFOCUSTAB));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpAutoFocusTab);
		lpAutoFocusTab = (AUTOFOCUSTAB *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpAutoFocusTab == NULL){
			ThrowMessage(hwnd, "WndAUTOFOCUSTAB: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndAUTOFOCUSTAB: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpAutoFocusTab->hOwner = (HWND) psp.lParam;

		lpAutoFocusTab->hCheckLaplacian = GetDlgItem(hwnd, IDC_CHECK_LAPLACIAN);
		lpAutoFocusTab->hCheckSobel = GetDlgItem(hwnd, IDC_CHECK_SOBEL);
		lpAutoFocusTab->hCheckVariance = GetDlgItem(hwnd, IDC_CHECK_VARIANCE);

		lpAutoFocusTab->hEditStep = GetDlgItem(hwnd, IDC_EDIT_STEP);
		lpAutoFocusTab->hEditDistance = GetDlgItem(hwnd, IDC_EDIT_DISTANCE);

		lpAutoFocusTab->hBtnAutoFocus = GetDlgItem(hwnd, IDC_BTN_AUTOFOCUS);
		
		lpAutoFocusTab->hBtnAutoFocus = lpAutoFocusTab->hBtnAutoFocus;

		lpAutoFocusTab->af.nStep = AF_NSTEPS;
		lpAutoFocusTab->af.stepDist = AF_DISTANCE;
		lpAutoFocusTab->af.filter = AF_SOBEL;
		//lpAutoFocusTab->nSteps = AF_NSTEPS;
		//lpAutoFocusTab->distance = AF_DISTANCE;

		SetDlgItemInt(hwnd, IDC_EDIT_STEP, lpAutoFocusTab->af.nStep, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_DISTANCE, lpAutoFocusTab->af.stepDist, TRUE);

		//Button_Enable(lpAutoFocusTab->hCheckLaplacian, FALSE);
		//Button_Enable(lpAutoFocusTab->hCheckVariance, FALSE);

		Button_SetCheck(lpAutoFocusTab->hCheckSobel, TRUE);

		lpAutoFocusTab->hZeissMicroscope = (HWND) SendMessage(lpAutoFocusTab->hOwner, WM_GETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, NULL);
		SendMessage(lpAutoFocusTab->hZeissMicroscope, WM_SETHWND, HWND_AUTOFOCUS, (LPARAM) hwnd);

		SendMessage(lpAutoFocusTab->hZeissMicroscope, ZFM_SETAUTOFOCUS, MAKEWPARAM(lpAutoFocusTab->nSteps, lpAutoFocusTab->distance), FALSE);

		break;
	case WM_COMMAND:

		switch(HIWORD(wParam)){
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_BTN_AUTOFOCUS){

				if(!lpAutoFocusTab->af.filter){
					MessageBox(hwnd, "Please Select a Focal Measure", "AutoFocus Error", MB_OK);
					return FALSE;
				}

				SendMessage(lpAutoFocusTab->hZeissMicroscope, ZFM_SETAUTOFOCUS, (WPARAM) &lpAutoFocusTab->af/*MAKEWPARAM(lpAutoFocusTab->nSteps, lpAutoFocusTab->distance)*/, TRUE);
			}
			if(LOWORD(wParam) == IDC_CHECK_LAPLACIAN){
				lpAutoFocusTab->af.filter = UpdateCheckBoxFilter(lpAutoFocusTab->hCheckLaplacian, lpAutoFocusTab->af.filter, AF_LAPLACIAN);
				lpAutoFocusTab->af.filter = lpAutoFocusTab->af.filter;
			}
			if(LOWORD(wParam) == IDC_CHECK_SOBEL){
				lpAutoFocusTab->af.filter = UpdateCheckBoxFilter(lpAutoFocusTab->hCheckSobel, lpAutoFocusTab->af.filter, AF_SOBEL);
				lpAutoFocusTab->af.filter= lpAutoFocusTab->af.filter;
			}
			if(LOWORD(wParam) == IDC_CHECK_VARIANCE){
				lpAutoFocusTab->af.filter = UpdateCheckBoxFilter(lpAutoFocusTab->hCheckVariance, lpAutoFocusTab->af.filter, AF_VARIANCE);
				lpAutoFocusTab->af.filter= lpAutoFocusTab->af.filter;
			}

			break;
		case EN_CHANGE:
			if(LOWORD(wParam) == IDC_EDIT_STEP){
				lpAutoFocusTab->af.nStep = GetDlgItemInt(hwnd, IDC_EDIT_STEP, NULL, FALSE);
				SendMessage(lpAutoFocusTab->hZeissMicroscope, ZFM_SETAUTOFOCUS, (WPARAM)&lpAutoFocusTab->af, FALSE);
				//lpAutoFocusTab->nSteps = GetDlgItemInt(hwnd, IDC_EDIT_STEP, NULL, FALSE);
				//SendMessage(lpAutoFocusTab->hZeissMicroscope, ZFM_SETAUTOFOCUS, MAKEWPARAM(lpAutoFocusTab->nSteps, lpAutoFocusTab->distance), FALSE);
			}
			if(LOWORD(wParam) == IDC_EDIT_DISTANCE){
				lpAutoFocusTab->af.stepDist = GetDlgItemInt(hwnd, IDC_EDIT_DISTANCE, NULL, FALSE);
				SendMessage(lpAutoFocusTab->hZeissMicroscope, ZFM_SETAUTOFOCUS, (WPARAM)&lpAutoFocusTab->af, FALSE);
				//lpAutoFocusTab->distance = GetDlgItemInt(hwnd, IDC_EDIT_DISTANCE, NULL, FALSE);
				//SendMessage(lpAutoFocusTab->hZeissMicroscope, ZFM_SETAUTOFOCUS, MAKEWPARAM(lpAutoFocusTab->nSteps, lpAutoFocusTab->distance), FALSE);
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
					SendMessage(lpAutoFocusTab->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);

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
			lpAutoFocusTab->hOwner = (HWND) lParam;
			return TRUE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}


	return FALSE;

}