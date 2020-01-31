#include "stdafx.h"
#include "SystemProc.h"
#include "FilePath.h"

INT_PTR CALLBACK SystemProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LM_SYSTEM *lpWnd = (LM_SYSTEM *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		lpWnd = (LM_SYSTEM *) InitWindowLongRACE(hwnd, sizeof(LM_SYSTEM));

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpWnd->hOwner = (HWND) psp.lParam;

		SendMessage(lpWnd->hOwner, WM_SETHWND, (WPARAM) HWND_SYSTEM, (LPARAM)hwnd);

		lpWnd->hEditProfile = GetDlgItem(hwnd, IDC_EDIT_PROFILE);
		lpWnd->hBtnLoad = GetDlgItem(hwnd, IDC_BTN_LOAD);
		lpWnd->hBtnSave = GetDlgItem(hwnd, IDC_BTN_SAVE);
		lpWnd->hEditSampleRate = GetDlgItem(hwnd, IDC_EDIT_SAMPLERATE);

		lpWnd->roiDiameter = 10;
		lpWnd->pixPerUm = 4.8;

		Edit_SetText(lpWnd->hEditProfile, "Default.rp");
		Edit_SetFloat(lpWnd->hEditSampleRate, 15);

		Button_Disable(lpWnd->hBtnLoad);
		Button_Disable(lpWnd->hBtnSave);

		break;
	case WM_COMMAND:

		switch(HIWORD(wParam)){
		case BN_CLICKED:
			switch(LOWORD(wParam))
			{
				case IDC_BTN_LOAD:
					OnSystemOpenProfile(hwnd, wParam, lParam);
					break;
				case IDC_BTN_SAVE:
					break;
				default:
					break;
			}
			break;
		case EN_CHANGE:
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_PROFILE:
				CHAR str[MAX_STRING];
				Edit_GetText(lpWnd->hEditProfile, str, MAX_STRING);
				break;
			case IDC_EDIT_SAMPLERATE:
				lpWnd->sampleRate = Edit_GetFloat(lpWnd->hEditSampleRate);
				lpWnd->sampleRate = lpWnd->sampleRate;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		break;

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

DWORD OnSystemOpenProfile(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LM_SYSTEM *lpWnd = (LM_SYSTEM *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if(!lpWnd){
		return FALSE;
	}

	FILEPATHSET fps = {0};

	GetOpenFileProfile(hwnd, &fps);
	//LoadProfile
	Edit_SetText(lpWnd->hEditProfile, fps.filePath[0].lpstr);

	return TRUE;
}
