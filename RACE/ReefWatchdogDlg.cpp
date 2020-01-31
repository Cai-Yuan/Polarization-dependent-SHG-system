#include "stdafx.h"

#include "ReefWatchdog.h"
#include "ReefWatchdogDlg.h"
#include "EnumerateDevice.h"

/*	case WM_INITDIALOG:

INT_PTR CALLBACK ReefWatchdogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	REEFWATCHDOG *lpReefWatchdog = (REEFWATCHDOG *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
		lpReefWatchdog = (REEFWATCHDOG *)malloc(sizeof(REEFWATCHDOG));
		memset(lpReefWatchdog, NULL, sizeof(REEFWATCHDOG));
		SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lpReefWatchdog);

		lpReefWatchdog = (REEFWATCHDOG *) GetWindowLongPtr(hwnd, (-21));

		if(!lpReefWatchdog){
			ThrowMessage(hwnd, "DlgReefWatchdog: Error Getting Long Ptr");
			ThrowMessage(hwnd, "DlgReefWatchdog: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpReefWatchdog->hEditStatus = GetDlgItem(hwnd, IDC_EDIT_STATUS);
		lpReefWatchdog->hEditTime = GetDlgItem(hwnd, IDC_EDIT_TIME);
		lpReefWatchdog->hBtnConnect = GetDlgItem(hwnd, IDC_BTN_CONNECT);
		lpReefWatchdog->hBtnRefresh = GetDlgItem(hwnd, IDC_BTN_REFRESH);
		lpReefWatchdog->hBtnLoad = GetDlgItem(hwnd, IDC_BTN_LOAD);
		lpReefWatchdog->hEditCW = GetDlgItem(hwnd, IDC_EDIT_CW);
		lpReefWatchdog->hEditRB = GetDlgItem(hwnd, IDC_EDIT_RB);
		lpReefWatchdog->hEditMIX = GetDlgItem(hwnd, IDC_EDIT_MIX);
		lpReefWatchdog->hEditUV = GetDlgItem(hwnd, IDC_EDIT_UV);

		lpReefWatchdog->hSliderCW = GetDlgItem(hwnd, IDC_SLIDER_CW);
		lpReefWatchdog->hSliderRB = GetDlgItem(hwnd, IDC_SLIDER_RB);
		lpReefWatchdog->hSliderMIX = GetDlgItem(hwnd, IDC_SLIDER_MIX);
		lpReefWatchdog->hSliderUV = GetDlgItem(hwnd, IDC_SLIDER_UV);

		SendMessage(lpReefWatchdog->hSliderCW, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
		SendMessage(lpReefWatchdog->hSliderRB, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
		SendMessage(lpReefWatchdog->hSliderMIX, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
		SendMessage(lpReefWatchdog->hSliderUV, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));

		lpReefWatchdog->hDevice = ConnectToDevice(&lpReefWatchdog->port, "?ID", "ReefWatchdog");
		if(lpReefWatchdog->hDevice){
			if(lpReefWatchdog->hDevice){
				sprintf(str, "Connected (COM%d)", lpReefWatchdog->port);
				Edit_SetText(lpReefWatchdog->hEditStatus, str);
				Button_SetText(lpReefWatchdog->hBtnConnect, "Disconnect");
			}
			else{
				CloseHandle(lpReefWatchdog->hDevice);
				lpReefWatchdog->hDevice = NULL;
				lpReefWatchdog->port = -1;
				Button_SetText(lpReefWatchdog->hBtnConnect, "Connect");
				Edit_SetText(lpReefWatchdog->hEditStatus, 0, "Disconnected");
			}
		}

		if(!lpReefWatchdog->hDevice)
			Edit_SetText(lpReefWatchdog->hEditStatus, "Not Connected");

		//Load Current Reefwatchdog Microcontroller values
		INT pwm;
		sprintf(str, "?GETPWMA");
		SerialSend(lpReefWatchdog->hDevice, str, str);
		sscanf(str, "PWMA=%d", &pwm);
		sprintf(str, "%d", pwm);
		SendMessage(lpReefWatchdog->hSliderCW, TBM_SETPOS, TRUE, pwm);
		Edit_SetText(lpReefWatchdog->hEditCW, str);

		break;
	case WM_COMMAND:

		switch(HIWORD(wParam))
		{
		case BN_CLICKED:

			switch(LOWORD(wParam))
			{
			case IDC_BTN_CONNECT:
				if(!lpReefWatchdog->hDevice){
					lpReefWatchdog->hDevice = ConnectToDevice(&lpReefWatchdog->port, "?ID", "ReefWatchdog");
					if(lpReefWatchdog->hDevice){
						sprintf(str, "Connected (COM%d)", lpReefWatchdog->port);
						Edit_SetText(lpReefWatchdog->hEditStatus, str);
						Button_SetText(lpReefWatchdog->hBtnConnect, "Disconnect");
					}
					else{
						Button_SetText(lpReefWatchdog->hBtnConnect, "Connect");
						Edit_SetText(lpReefWatchdog->hEditStatus, "Not Found");
					}
				}
				else{
					CloseHandle(lpReefWatchdog->hDevice);
					lpReefWatchdog->hDevice = NULL;
					lpReefWatchdog->port = -1;
					Button_SetText(lpReefWatchdog->hBtnConnect, "Connect");
					Edit_SetText(lpReefWatchdog->hEditStatus, "Disconnected");
				}
				break;
			case IDC_BTN_REFRESH:
				SerialSend(lpReefWatchdog->hDevice, "?GETTIME", str);
				Edit_SetText(lpReefWatchdog->hEditTime, str);
				break;
			case IDC_BTN_LOAD:
				SYSTEMTIME st;
				memset(&st, NULL, sizeof(SYSTEMTIME));

				GetLocalTime(&st);
				//sprintf(str, "!SETTIME%d:%d:%d", 23, 59, 0);
				sprintf(str, "!SETTIME%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
				SerialSend(lpReefWatchdog->hDevice, str, NULL);

				break;
			default:
				break;
			}
			case EN_CHANGE:
				switch(LOWORD(wParam)){
					case IDC_EDIT_CW:
						{
							INT dc;
							CHAR error[MAX_STRING];
							memset(error, NULL, MAX_STRING*sizeof(CHAR));

							Edit_GetText(lpReefWatchdog->hEditCW, str, MAX_STRING);
							dc = atoi(str);
							sprintf(str, "!SETPWMA%d", dc);
							SerialSend(lpReefWatchdog->hDevice, str, error);
							dc = dc;
							dc = dc;
						}
						break;
					case IDC_EDIT_RB:
						{
							INT dc;
							CHAR error[MAX_STRING];
							memset(error, NULL, MAX_STRING*sizeof(CHAR));

							Edit_GetText(lpReefWatchdog->hEditRB, str, MAX_STRING);
							dc = atoi(str);
							sprintf(str, "!SETPWMB%d", dc);
							SerialSend(lpReefWatchdog->hDevice, str, error);
							dc = dc;
							dc = dc;
						}
						break;
					case IDC_EDIT_MIX:
						{
							INT dc;
							CHAR error[MAX_STRING];
							memset(error, NULL, MAX_STRING*sizeof(CHAR));

							Edit_GetText(lpReefWatchdog->hEditMIX, str, MAX_STRING);
							dc = atoi(str);
							sprintf(str, "!SETPWMC%d", dc);
							SerialSend(lpReefWatchdog->hDevice, str, error);
							dc = dc;
							dc = dc;
						}
						break;
					case IDC_EDIT_UV:
						{
							INT dc;
							CHAR error[MAX_STRING];
							memset(error, NULL, MAX_STRING*sizeof(CHAR));

							Edit_GetText(lpReefWatchdog->hEditUV, str, MAX_STRING);
							dc = atoi(str);
							sprintf(str, "!SETPWMD%d", dc);
							SerialSend(lpReefWatchdog->hDevice, str, error);
							dc = dc;
							dc = dc;
						}
						break;
					default:
						break;
				}
				break;
		}
		break;
	case WM_SETHWND:
		switch(wParam)
		{
			case HWND_OWNER:
				lpReefWatchdog->hOwner = (HWND)lParam;
				break;
			case HWND_PROPERTYSHEET:
				lpReefWatchdog->hPropSheet = (HWND)lParam;
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
				return DialogReturn(hwnd, PSNRET_INVALID_NOCHANGEPAGE); //PSNRET_NOERROR
			case PSN_QUERYCANCEL:
					//SendMessage(lpReefWatchdog->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);

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
	case WM_HSCROLL:
		
		//lParam - Used if the scroll comes from a control
		if(lParam){
			DWORD id;
			INT pos;
			id = GetDlgCtrlID((HWND)lParam);
			pos = SendMessage((HWND)lParam, TBM_GETPOS, NULL, NULL);
			sprintf(str, "%d", pos);
			switch(id)
			{
			case IDC_SLIDER_CW:
				Edit_SetText(lpReefWatchdog->hEditCW, str);
				sprintf(str, "!SETPWMA%d", pos);
				SerialSend(lpReefWatchdog->hDevice, str, NULL);
				break;
			case IDC_SLIDER_RB:
				Edit_SetText(lpReefWatchdog->hEditRB, str);
				sprintf(str, "!SETPWMB%d", pos);
				SerialSend(lpReefWatchdog->hDevice, str, NULL);
				break;
			case IDC_SLIDER_MIX:
				Edit_SetText(lpReefWatchdog->hEditMIX, str);
				sprintf(str, "!SETPWMC%d", pos);
				SerialSend(lpReefWatchdog->hDevice, str, NULL);
				break;
			case IDC_SLIDER_UV:
				Edit_SetText(lpReefWatchdog->hEditUV, str);
				sprintf(str, "!SETPWMD%d", pos);
				SerialSend(lpReefWatchdog->hDevice, str, NULL);
				break;
			default:
				break;
			}

		}
		break;
	case WM_CLOSE:

		//SendMessage(lpStageProp->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_STAGEPROPERTIES), NULL);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}
*/
