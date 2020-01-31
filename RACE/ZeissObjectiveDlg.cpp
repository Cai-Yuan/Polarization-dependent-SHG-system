#include "stdafx.h"

#include "ZeissObjectiveDlg.h"
#include "ZeissMicroscope.h"

INT_PTR CALLBACK ZeissObjectiveProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ZEISSOBJECTIVEDLG *lpZeissObjectiveDlg = (ZEISSOBJECTIVEDLG *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpZeissObjectiveDlg = (ZEISSOBJECTIVEDLG *)calloc(1, sizeof(ZEISSOBJECTIVEDLG));
		memset(lpZeissObjectiveDlg, 0x00, sizeof(ZEISSOBJECTIVEDLG));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpZeissObjectiveDlg);
		lpZeissObjectiveDlg = (ZEISSOBJECTIVEDLG *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpZeissObjectiveDlg == NULL){
			ThrowMessage(hwnd, "WndZeissObjectiveDlg: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndZeissObjectiveDlg: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpZeissObjectiveDlg->hOwner = (HWND) psp.lParam;

		lpZeissObjectiveDlg->hBtn10x = GetDlgItem(hwnd, IDC_BTN_10X);
		lpZeissObjectiveDlg->hBtn40xOil = GetDlgItem(hwnd, IDC_BTN_40XOIL);
		lpZeissObjectiveDlg->hBtn63xOil = GetDlgItem(hwnd, IDC_BTN_63XOIL);
		lpZeissObjectiveDlg->hBtn4x = GetDlgItem(hwnd, IDC_BTN_4X);
		lpZeissObjectiveDlg->hBtn20x = GetDlgItem(hwnd, IDC_BTN_20X);
		lpZeissObjectiveDlg->hBtn40x = GetDlgItem(hwnd, IDC_BTN_40X);

		lpZeissObjectiveDlg->hZeissMicroscope = (HWND) SendMessage(lpZeissObjectiveDlg->hOwner, WM_GETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, NULL);
		SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, WM_SETHWND, HWND_MULTICHANNEL, (LPARAM) hwnd);

		break;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:

			switch(LOWORD(wParam))
			{
			case IDC_BTN_10X:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETOBJECTIVE, OBJECTIVE_10X, NULL);

				break;
			case IDC_BTN_40XOIL:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETOBJECTIVE, OBJECTIVE_40XOIL, NULL);

				break;
			case IDC_BTN_63XOIL:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETOBJECTIVE, OBJECTIVE_63XOIL, NULL);

				break;
			case IDC_BTN_4X:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETOBJECTIVE, OBJECTIVE_4X, NULL);

				break;
			case IDC_BTN_20X:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETOBJECTIVE, OBJECTIVE_20X, NULL);

				break;
			case IDC_BTN_40X:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETOBJECTIVE, OBJECTIVE_40X, NULL);

				break;
			case IDC_BTN_EYEPIECE:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETLIGHTPATH, LP_EYEPIECE, NULL);
				break;
			case IDC_BTN_CAMERA:
				SendMessage(lpZeissObjectiveDlg->hZeissMicroscope, ZSM_SETLIGHTPATH, LP_CAMERA, NULL);
				break;
			default:
				break;
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
					SendMessage(lpZeissObjectiveDlg->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);

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

		if(wParam == HWND_OWNER){
			lpZeissObjectiveDlg->hOwner = (HWND) lParam;
		}
		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}