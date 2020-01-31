#include "stdafx.h"

#include "ReefWatchdog.h"
#include "ReefWatchdogDosing.h"
#include "EnumerateDevice.h"

DWORD GetDosingRate(REEFWATCHDOGDOSING *lpReefWatchdogDosing);
DWORD GetDosingIncrease(REEFWATCHDOGDOSING *lpReefWatchdogDosing); 
DWORD WINAPI GetDosingRateProc(LPVOID param);
DWORD WINAPI GetDosingIncreaseProc(LPVOID param);

INT_PTR CALLBACK ReefWatchdogDosingProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	REEFWATCHDOGDOSING *lpReefWatchdogDosing = (REEFWATCHDOGDOSING *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpReefWatchdogDosing = (REEFWATCHDOGDOSING *)calloc(1, sizeof(REEFWATCHDOGDOSING));
		memset(lpReefWatchdogDosing, 0x00, sizeof(REEFWATCHDOGDOSING));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpReefWatchdogDosing);
		lpReefWatchdogDosing = (REEFWATCHDOGDOSING *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpReefWatchdogDosing == NULL){
			ThrowMessage(hwnd, "WndReefWatchdogDosing: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndReefWatchdogDosing: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpReefWatchdogDosing->hOwner = (HWND) psp.lParam;

		lpReefWatchdogDosing->hRadioA = GetDlgItem(hwnd, IDC_RADIO_A);
		lpReefWatchdogDosing->hRadioB = GetDlgItem(hwnd, IDC_RADIO_B);
		lpReefWatchdogDosing->hRadioC = GetDlgItem(hwnd, IDC_RADIO_C);
		lpReefWatchdogDosing->hRadioD = GetDlgItem(hwnd, IDC_RADIO_D);

		lpReefWatchdogDosing->hEditRate = GetDlgItem(hwnd, IDC_EDIT_RATE);
		lpReefWatchdogDosing->hEditVolume = GetDlgItem(hwnd, IDC_EDIT_VOLUME);
		lpReefWatchdogDosing->hEditIncrease = GetDlgItem(hwnd, IDC_EDIT_INCREASE);

		lpReefWatchdogDosing->hCheckVolume = GetDlgItem(hwnd, IDC_CHECK_VOLUME);

		lpReefWatchdogDosing->hBtnSet = GetDlgItem(hwnd, IDC_BTN_SET);

		SendMessage(lpReefWatchdogDosing->hOwner, WM_SETHWND, HWND_REEFWATCHDOGDOSING, (LPARAM) hwnd);

		Button_SetCheck(lpReefWatchdogDosing->hRadioA, TRUE);

		lpReefWatchdogDosing->motor = 'A';
		Edit_SetText(lpReefWatchdogDosing->hEditRate, "1.0");
		
		lpReefWatchdogDosing->flgRate = FALSE;
		lpReefWatchdogDosing->flgIncrease = FALSE;

		HANDLE hDevice;
		hDevice = (HANDLE) SendMessage(lpReefWatchdogDosing->hOwner, WM_GETHANDLE, HANDLE_DEVICE_DOSING, NULL);
		SendMessage(hwnd, WM_SETHANDLE, HANDLE_DEVICE_DOSING, (LPARAM) hDevice);

		break;

	case WM_GETLONGPTR:
		return (INT_PTR) DialogReturn(hwnd, (LRESULT)lpReefWatchdogDosing);
		break;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_BTN_SET){

				if(lpReefWatchdogDosing->flgRate){
					sprintf(str, "!SETRATE%c%.2f", lpReefWatchdogDosing->motor, lpReefWatchdogDosing->rate);
					SerialSend(lpReefWatchdogDosing->hDevice, str, NULL);
					lpReefWatchdogDosing->flgRate = FALSE;
				}
				if(lpReefWatchdogDosing->flgIncrease){
					sprintf(str, "!SETRATEINCREASE%c%.2f", lpReefWatchdogDosing->motor, lpReefWatchdogDosing->increase);
					SerialSend(lpReefWatchdogDosing->hDevice, str, NULL);
					lpReefWatchdogDosing->flgIncrease = FALSE;
				}
			}
			if(LOWORD(wParam) == IDC_BTN_DOWNLOAD){
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
			}
			if(LOWORD(wParam) == IDC_RADIO_A){
				lpReefWatchdogDosing->motor = 'A';
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
				break;
			}
			if(LOWORD(wParam) == IDC_RADIO_B){
				lpReefWatchdogDosing->motor = 'B';
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
				break;
			}
			if(LOWORD(wParam) == IDC_RADIO_C){
				lpReefWatchdogDosing->motor = 'C';
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
				break;
			}
			if(LOWORD(wParam) == IDC_RADIO_D){
				lpReefWatchdogDosing->motor = 'D';
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
				break;
			}
			if(LOWORD(wParam) == IDC_RADIO_E){
				lpReefWatchdogDosing->motor = 'E';
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
			}
			if(LOWORD(wParam) == IDC_RADIO_F){
				lpReefWatchdogDosing->motor = 'F';
				GetDosingRate(lpReefWatchdogDosing);
				GetDosingIncrease(lpReefWatchdogDosing);
				break;
			}
		case EN_CHANGE:
			if(LOWORD(wParam) == IDC_EDIT_RATE){
				lpReefWatchdogDosing->flgRate = TRUE;
				FLOAT rate;
				Edit_GetText(lpReefWatchdogDosing->hEditRate, str, MAX_STRING);
				sscanf(str, "%f", &rate);
				lpReefWatchdogDosing->rate = rate;
				
			}			
			if(LOWORD(wParam) == IDC_EDIT_INCREASE){
				lpReefWatchdogDosing->flgIncrease = TRUE;
				FLOAT increase;
				Edit_GetText(lpReefWatchdogDosing->hEditIncrease, str, MAX_STRING);
				sscanf(str, "%f", &increase);
				lpReefWatchdogDosing->increase = increase;
				
			}

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
	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpReefWatchdogDosing->hOwner = (HWND) lParam;

		return TRUE;
	case WM_SETHANDLE:

		if(wParam == HANDLE_DEVICE){
		//if(wParam == HANDLE_DEVICE_DOSING){
			lpReefWatchdogDosing->hDevice = (HANDLE) lParam;
			if(lpReefWatchdogDosing->hDevice){
				lpReefWatchdogDosing->idTimer = SetTimer(hwnd, NULL, 2000, NULL);
			}
			else{
				KillTimer(hwnd, lpReefWatchdogDosing->idTimer);
			}
		}
		return TRUE;
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
					SendMessage(lpReefWatchdogDosing->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_REEFWATCHDOG), NULL);

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
	case WM_CLOSE:

		return TRUE;
	case WM_TIMER:
		if(!lpReefWatchdogDosing->flgRate){
			//GetDosingRate(lpReefWatchdogDosing);
			//CreateThread(NULL, NULL, GetDosingRateProc, (LPVOID) lpReefWatchdogDosing, NULL, NULL);
		}
		if(!lpReefWatchdogDosing->flgIncrease){
			//CreateThread(NULL, NULL, GetDosingIncreaseProc, (LPVOID) lpReefWatchdogDosing, NULL, NULL);
			//GetDosingIncrease(lpReefWatchdogDosing);
		}
		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD GetDosingRate(REEFWATCHDOGDOSING *lpReefWatchdogDosing)
{
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(char));

	sprintf(str, "?GETRATE%c", lpReefWatchdogDosing->motor);
	SerialSend(lpReefWatchdogDosing->hDevice, str, str);
	sscanf(str, "%f", &lpReefWatchdogDosing->rate);
	sprintf(str, "%.4f", lpReefWatchdogDosing->rate);
	Edit_SetText(lpReefWatchdogDosing->hEditRate, str);
	lpReefWatchdogDosing->flgRate = FALSE;

	return TRUE;

}

DWORD GetDosingIncrease(REEFWATCHDOGDOSING *lpReefWatchdogDosing)
{
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(char));

	sprintf(str, "?GETRATEINCREASE%c", lpReefWatchdogDosing->motor);
	SerialSend(lpReefWatchdogDosing->hDevice, str, str);
	sscanf(str, "%f", &lpReefWatchdogDosing->increase);
	sprintf(str, "%.5f", lpReefWatchdogDosing->increase);
	Edit_SetText(lpReefWatchdogDosing->hEditIncrease, str);
	lpReefWatchdogDosing->flgIncrease = FALSE;

	return TRUE;
}

DWORD WINAPI GetDosingRateProc(LPVOID param)
{
	REEFWATCHDOGDOSING *lpReefWatchdogDosing = (REEFWATCHDOGDOSING *)param;
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(char));

	sprintf(str, "?GETRATE%c", lpReefWatchdogDosing->motor);
	SerialSend(lpReefWatchdogDosing->hDevice, str, str);
	sscanf(str, "%f", &lpReefWatchdogDosing->rate);
	sprintf(str, "%.4f", lpReefWatchdogDosing->rate);
	Edit_SetText(lpReefWatchdogDosing->hEditRate, str);
	lpReefWatchdogDosing->flgRate = FALSE;

	return TRUE;

}

DWORD WINAPI GetDosingIncreaseProc(LPVOID param)
{
	REEFWATCHDOGDOSING *lpReefWatchdogDosing = (REEFWATCHDOGDOSING *)param;

	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(char));

	sprintf(str, "?GETRATEINCREASE%c", lpReefWatchdogDosing->motor);
	SerialSend(lpReefWatchdogDosing->hDevice, str, str);
	sscanf(str, "%f", &lpReefWatchdogDosing->increase);
	sprintf(str, "%.5f", lpReefWatchdogDosing->increase);
	Edit_SetText(lpReefWatchdogDosing->hEditIncrease, str);
	lpReefWatchdogDosing->flgIncrease = FALSE;

	return TRUE;
}