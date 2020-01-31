#include "stdafx.h"

#include "ReefWatchdog.h"
#include "ReefWatchdogDevice.h"
#include "EnumerateDevice.h"
#include "ToolsWIN32.h"

INT_PTR CALLBACK ReefWatchdogDeviceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	REEFWATCHDOGDEVICE *lpReefWatchdogDevice = (REEFWATCHDOGDEVICE *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpReefWatchdogDevice = (REEFWATCHDOGDEVICE *)calloc(1, sizeof(REEFWATCHDOGDEVICE));
		memset(lpReefWatchdogDevice, 0x00, sizeof(REEFWATCHDOGDEVICE));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpReefWatchdogDevice);
		lpReefWatchdogDevice = (REEFWATCHDOGDEVICE *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpReefWatchdogDevice == NULL){
			ThrowMessage(hwnd, "WndReefWatchdogDevice: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndReefWatchdogDevice: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpReefWatchdogDevice->hwnd = hwnd;

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpReefWatchdogDevice->hOwner = (HWND) psp.lParam;

		lpReefWatchdogDevice->hBtnConnect = GetDlgItem(hwnd, IDC_BTN_CONNECT);
		lpReefWatchdogDevice->hBtnDownload = GetDlgItem(hwnd, IDC_BTN_DOWNLOAD);
		lpReefWatchdogDevice->hBtnUpload = GetDlgItem(hwnd, IDC_BTN_UPLOAD);

		lpReefWatchdogDevice->hEditStatus = GetDlgItem(hwnd, IDC_EDIT_STATUS);
		lpReefWatchdogDevice->hEditTime = GetDlgItem(hwnd, IDC_EDIT_TIME);
		

		CreateThread(NULL, NULL, ReefWatchdogDeviceInitProc, (LPVOID) lpReefWatchdogDevice, NULL, NULL);
		/*
		//lpReefWatchdogDevice->hDosing = ConnectToDevice(&lpReefWatchdogDevice->numComDosing, "?ID", "ReefWatchdogDoser");
		lpReefWatchdogDevice->hDevice = ConnectToDevice(&lpReefWatchdogDevice->numComLighting, "?ID", "ReefWatchdog");
		//lpReefWatchdogDevice->hDosing = ConnectToDevice(&lpReefWatchdogDevice->numComDosing, "?ID", "ReefWatchdogDoser", 2500);
		//lpReefWatchdogDevice->hDevice = ConnectToDevice(&lpReefWatchdogDevice->numComLighting, "?ID", "ReefWatchdog", 2500);
		SendMessage(lpReefWatchdogDevice->hOwner, WM_SETHWND, HWND_REEFWATCHDOGDEVICE, (LPARAM) hwnd);

		SendMessage(lpReefWatchdogDevice->hOwner, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM) lpReefWatchdogDevice->hDevice);
		
		SendMessage(lpReefWatchdogDevice->hOwner, WM_SETHANDLE, HANDLE_DEVICE_DOSING, (LPARAM) lpReefWatchdogDevice->hDosing);
		
		if(lpReefWatchdogDevice->hDevice){
			memset(str, NULL, MAX_STRING*sizeof(CHAR));
			sprintf(str, "L(COM%d)D(COM%d)", lpReefWatchdogDevice->numComLighting, lpReefWatchdogDevice->numComDosing);
			Edit_SetText(lpReefWatchdogDevice->hEditStatus, str);
			Button_SetText(lpReefWatchdogDevice->hBtnConnect, "Disconnect");
		}
		else{
			Edit_SetText(lpReefWatchdogDevice->hEditStatus, "Not Found");
		}*/

		break;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_BTN_UPLOAD){
				SYSTEMTIME st;
				memset(&st, NULL, sizeof(SYSTEMTIME));

				GetLocalTime(&st);
				//sprintf(str, "!SETTIME%d:%d:%d", 23, 59, 0);
				sprintf(str, "!SETTIME%d:%d:%d", st.wHour, st.wMinute, st.wSecond);
				SerialSend(lpReefWatchdogDevice->hDevice, str, NULL);
				SerialSend(lpReefWatchdogDevice->hDosing, str, NULL);

			}
			if(LOWORD(wParam) == IDC_BTN_DOWNLOAD){
				CHAR strTimeDosing[MAX_STRING];
				CHAR strTimeLighting[MAX_STRING];

				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				memset(strTimeDosing, NULL, MAX_STRING*sizeof(CHAR));
				memset(strTimeLighting, NULL, MAX_STRING*sizeof(CHAR));

				SerialSend(lpReefWatchdogDevice->hDosing, "?GETTIME", strTimeDosing);
				SerialSend(lpReefWatchdogDevice->hDevice, "?GETTIME", strTimeLighting);

				sprintf(str, "L(%s)D(%s)", strTimeLighting, strTimeDosing);
				Edit_SetText(lpReefWatchdogDevice->hEditTime, str);
			}
			if(LOWORD(wParam) == IDC_BTN_CONNECT){
				if(!lpReefWatchdogDevice->hDevice){
					//lpReefWatchdogDevice->hDosing = ConnectToDevice(&lpReefWatchdogDevice->numComDosing, "?ID", "ReefWatchdogDoser");
					lpReefWatchdogDevice->hDevice = ConnectToDevice(&lpReefWatchdogDevice->numComLighting, "?ID", "ReefWatchdog");

					if(lpReefWatchdogDevice->hDevice){
						//sprintf(str, "L(COM%d)D(COM%d)", lpReefWatchdogDevice->numComLighting, lpReefWatchdogDevice->numComDosing);
						//sprintf(str, "Connected (COM%d)", lpReefWatchdogDevice->numCom);
						sprintf(str, "Connected (COM%d)", lpReefWatchdogDevice->numComLighting);
						Edit_SetText(lpReefWatchdogDevice->hEditStatus, str);
						Button_SetText(lpReefWatchdogDevice->hBtnConnect, "Disconnect");
						SendMessage(lpReefWatchdogDevice->hOwner, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM) lpReefWatchdogDevice->hDevice);
						//SendMessage(lpReefWatchdogDevice->hOwner, WM_SETHANDLE, HANDLE_DEVICE_DOSING, (LPARAM) lpReefWatchdogDevice->hDosing);
					}
					else{
						Button_SetText(lpReefWatchdogDevice->hBtnConnect, "Connect");
						Edit_SetText(lpReefWatchdogDevice->hEditStatus, "Not Found");
					}
				}
				else{
					
					CloseHandle(lpReefWatchdogDevice->hDevice);
					//CloseHandle(lpReefWatchdogDevice->hDosing);
					lpReefWatchdogDevice->hDevice = NULL;
					lpReefWatchdogDevice->numCom = 0;
					lpReefWatchdogDevice->numComLighting = 0;
					lpReefWatchdogDevice->numComDosing = 0;
					Button_SetText(lpReefWatchdogDevice->hBtnConnect, "Connect");
					Edit_SetText(lpReefWatchdogDevice->hEditStatus, "Disconnected");
					SendMessage(lpReefWatchdogDevice->hOwner, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM) lpReefWatchdogDevice->hDevice);
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
	case WM_GETLONGPTR:
		return (INT_PTR) DialogReturn(hwnd, (LRESULT)lpReefWatchdogDevice);
	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpReefWatchdogDevice->hOwner = (HWND) lParam;

		return TRUE;
	case WM_SETHANDLE:

		if(wParam == HANDLE_DEVICE){
			lpReefWatchdogDevice->hDevice = (HANDLE) lParam;
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
					SendMessage(lpReefWatchdogDevice->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_REEFWATCHDOG), NULL);

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

		//SendMessage(lpPropertyManager->hOwner, WM_COMMAND, LOWORD(ID_VIEW_PROPERTYMANAGER), NULL);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD WINAPI ReefWatchdogDeviceInitProc(LPVOID param)
{
	REEFWATCHDOGDEVICE *lpDevice = (REEFWATCHDOGDEVICE *)param;

	Sleep(100);

	lpDevice->hDevice = ConnectToDevice(&lpDevice->numComLighting, "?ID", "ReefWatchdog");

	SendMessage(lpDevice->hOwner, WM_SETHWND, HWND_REEFWATCHDOGDEVICE, (LPARAM) lpDevice->hwnd);

	SendMessage(lpDevice->hOwner, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM) lpDevice->hDevice); 
	
	CHAR str[MAX_STRING];

	if(lpDevice->hDevice){
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "L(COM%d)D(COM%d)", lpDevice->numComLighting, lpDevice->numComDosing);
		Edit_SetText(lpDevice->hEditStatus, str);
		Button_SetText(lpDevice->hBtnConnect, "Disconnect");
	}
	else{
		Edit_SetText(lpDevice->hEditStatus, "Not Found");
	}

	return TRUE;
}