#include "stdafx.h"

#include "ReefWatchdog.h"
#include "ReefWatchdogLighting.h"
#include "EnumerateDevice.h"
#include "ToolsWIN32.h"

DWORD GetPWM(REEFWATCHDOGLIGHTING *lpReefWatchdogLighting);
DWORD WINAPI GetPWMProc(LPVOID param);

INT_PTR CALLBACK ReefWatchdogLightingProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	REEFWATCHDOGLIGHTING *lpReefWatchdogLighting = (REEFWATCHDOGLIGHTING *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpReefWatchdogLighting = (REEFWATCHDOGLIGHTING *)calloc(1, sizeof(REEFWATCHDOGLIGHTING));
		memset(lpReefWatchdogLighting, 0x00, sizeof(REEFWATCHDOGLIGHTING));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpReefWatchdogLighting);
		lpReefWatchdogLighting = (REEFWATCHDOGLIGHTING *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpReefWatchdogLighting == NULL){
			ThrowMessage(hwnd, "WndReefWatchdogLighting: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndReefWatchdogLighting: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpReefWatchdogLighting->hOwner = (HWND) psp.lParam;

		lpReefWatchdogLighting->hSliderA = GetDlgItem(hwnd, IDC_SLIDER_A);
		lpReefWatchdogLighting->hSliderB = GetDlgItem(hwnd, IDC_SLIDER_B);
		lpReefWatchdogLighting->hSliderC = GetDlgItem(hwnd, IDC_SLIDER_C);
		lpReefWatchdogLighting->hSliderD = GetDlgItem(hwnd, IDC_SLIDER_D);

		lpReefWatchdogLighting->hEditA = GetDlgItem(hwnd, IDC_EDIT_A);
		lpReefWatchdogLighting->hEditB = GetDlgItem(hwnd, IDC_EDIT_B);
		lpReefWatchdogLighting->hEditC = GetDlgItem(hwnd, IDC_EDIT_C);
		lpReefWatchdogLighting->hEditD = GetDlgItem(hwnd, IDC_EDIT_D);

		lpReefWatchdogLighting->hCheckPhotoCycle = GetDlgItem(hwnd, IDC_CHECK_AUTOPHOTOCYCLE);

		SendMessage(lpReefWatchdogLighting->hSliderA, TBM_SETRANGEMIN, TRUE, 0);
		SendMessage(lpReefWatchdogLighting->hSliderA, TBM_SETRANGEMAX, TRUE, 255);

		SendMessage(lpReefWatchdogLighting->hSliderB, TBM_SETRANGEMIN, TRUE, 0);
		SendMessage(lpReefWatchdogLighting->hSliderB, TBM_SETRANGEMAX, TRUE, 255);

		SendMessage(lpReefWatchdogLighting->hSliderC, TBM_SETRANGEMIN, TRUE, 0);
		SendMessage(lpReefWatchdogLighting->hSliderC, TBM_SETRANGEMAX, TRUE, 255);

		SendMessage(lpReefWatchdogLighting->hSliderD, TBM_SETRANGEMIN, TRUE, 0);
		SendMessage(lpReefWatchdogLighting->hSliderD, TBM_SETRANGEMAX, TRUE, 255);

		Edit_SetText(lpReefWatchdogLighting->hEditA, "0");
		Edit_SetText(lpReefWatchdogLighting->hEditB, "0");
		Edit_SetText(lpReefWatchdogLighting->hEditC, "0");
		Edit_SetText(lpReefWatchdogLighting->hEditD, "0");

		Button_SetCheck(lpReefWatchdogLighting->hCheckPhotoCycle, TRUE);

		//lpReefWatchdogLighting->hReefWatchdogProc = (HWND) SendMessage(lpReefWatchdogLighting->hOwner, WM_GETHWND, HWND_REEFWATCHDOG, NULL);
		lpReefWatchdogLighting->hDevice = (HANDLE) SendMessage(lpReefWatchdogLighting->hOwner, WM_GETHANDLE, HANDLE_DEVICE, NULL);
		lpReefWatchdogLighting->hDevice = lpReefWatchdogLighting->hDevice;

		SendMessage(lpReefWatchdogLighting->hOwner, WM_SETHWND, HWND_REEFWATCHDOGLIGHTING, (LPARAM) hwnd);
		


		break;
	case WM_GETLONGPTR:
		return (INT_PTR) DialogReturn(hwnd, (LRESULT)lpReefWatchdogLighting);
		break;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_CHECK_AUTOPHOTOCYCLE){
				if(Button_GetCheck(lpReefWatchdogLighting->hCheckPhotoCycle)){
					SerialSend(lpReefWatchdogLighting->hDevice, "!ENABLEPHOTOCYCLE", NULL);
				}else{
					SerialSend(lpReefWatchdogLighting->hDevice, "!DISABLEPHOTOCYCLE", NULL);
				}
				
			}
			break;
		case EN_CHANGE:
			break;
		default:
			break;
		}
		//Control Identifier
		switch(LOWORD(wParam))
		{
		//case IDSET:
		//	return TRUE;
		default:
			break;
		}

		break;
	case WM_HSCROLL:
		INT pos;

		if(LOWORD(wParam) == SB_THUMBTRACK){
			pos = HIWORD(wParam);
		
			if((HWND) lParam == lpReefWatchdogLighting->hSliderA){
				//pos = GetScrollPos(lpReefWatchdogLighting->hSliderA, SB_HORZ);
				sprintf(str, "%d", pos);
				Edit_SetText(lpReefWatchdogLighting->hEditA, str);
				sprintf(str, "!SETPWMA%d", pos);
				SerialSend(lpReefWatchdogLighting->hDevice, str, NULL);

			}
			if((HWND) lParam == lpReefWatchdogLighting->hSliderB){
				sprintf(str, "%d", pos);
				Edit_SetText(lpReefWatchdogLighting->hEditB, str);
				sprintf(str, "!SETPWMB%d", pos);
				SerialSend(lpReefWatchdogLighting->hDevice, str, NULL);

			}
			if((HWND) lParam == lpReefWatchdogLighting->hSliderC){
				sprintf(str, "%d", pos);
				Edit_SetText(lpReefWatchdogLighting->hEditC, str);
				sprintf(str, "!SETPWMC%d", pos);
				SerialSend(lpReefWatchdogLighting->hDevice, str, NULL);

			}
			if((HWND) lParam == lpReefWatchdogLighting->hSliderD){
				sprintf(str, "%d", pos);
				Edit_SetText(lpReefWatchdogLighting->hEditD, str);
				sprintf(str, "!SETPWMD%d", pos);
				SerialSend(lpReefWatchdogLighting->hDevice, str, NULL);

			}
		}

		break;
	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpReefWatchdogLighting->hOwner = (HWND) lParam;

		return TRUE;
	case WM_SETHANDLE:
		if(wParam == HANDLE_DEVICE){
			lpReefWatchdogLighting->hDevice = (HANDLE) lParam;

			if(lpReefWatchdogLighting->hDevice){
				lpReefWatchdogLighting->idTimer = SetTimer(hwnd, NULL, 1000, NULL);
			}
			else{
				KillTimer(hwnd, lpReefWatchdogLighting->idTimer);
			}

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
					SendMessage(lpReefWatchdogLighting->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_REEFWATCHDOG), NULL);

					RECT rc;
					GetWindowRect(pshNotify.hdr.hwndFrom, &rc);
					rc.left = rc.left;
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
	case WM_TIMER:
		//CreateThread(NULL, NULL, GetPWMProc, (LPVOID) lpReefWatchdogLighting, NULL, NULL);
		//GetPWM(lpReefWatchdogLighting);

		return TRUE;
	case WM_CLOSE:

		//SendMessage(lpPropertyManager->hOwner, WM_COMMAND, LOWORD(ID_VIEW_PROPERTYMANAGER), NULL);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD GetPWM(REEFWATCHDOGLIGHTING *lpReefWatchdogLighting)
{
	CHAR str[MAX_STRING];
	INT pos = NULL;
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMA", str);
	sscanf(str, "%d", &pos);
	Edit_SetText(lpReefWatchdogLighting->hEditA, str);
	PostMessage(lpReefWatchdogLighting->hSliderA, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMB", str);
	sscanf(str, "%d", &pos);
	Edit_SetText(lpReefWatchdogLighting->hEditB, str);
	PostMessage(lpReefWatchdogLighting->hSliderB, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMC", str);
	sscanf(str, "%d", &pos);
	Edit_SetText(lpReefWatchdogLighting->hEditC, str);
	PostMessage(lpReefWatchdogLighting->hSliderC, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMD", str);
	sscanf(str, "%d", &pos);
	Edit_SetText(lpReefWatchdogLighting->hEditD, str);
	PostMessage(lpReefWatchdogLighting->hSliderD, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	return TRUE;
}

DWORD WINAPI GetPWMProc(LPVOID param)
{
	REEFWATCHDOGLIGHTING *lpReefWatchdogLighting = (REEFWATCHDOGLIGHTING *)param;

	CHAR str[MAX_STRING];
	INT pos = NULL;
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMA", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpReefWatchdogLighting->hEditA, str);
	SendMessage(lpReefWatchdogLighting->hSliderA, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMB", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpReefWatchdogLighting->hEditB, str);
	SendMessage(lpReefWatchdogLighting->hSliderB, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMC", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpReefWatchdogLighting->hEditC, str);
	SendMessage(lpReefWatchdogLighting->hSliderC, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdogLighting->hDevice, "?GETPWMD", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpReefWatchdogLighting->hEditD, str);
	SendMessage(lpReefWatchdogLighting->hSliderD, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	return TRUE;
}