#include "stdafx.h"

#include "ReefWatchdog.h"
#include "ReefWatchdogDevice.h"
#include "ReefWatchdogDosing.h"
#include "ReefWatchdogLighting.h"
#include "EnumerateDevice.h"
#include "ToolsWIN32.h"

LRESULT CALLBACK ReefWatchdogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	REEFWATCHDOG *lpReefWatchdog = (REEFWATCHDOG *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	INT i;
	INT pos;
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	switch(msg)
	{
	case WM_SETHWND:
		if(wParam == HWND_OWNER){
			lpReefWatchdog->hOwner = (HWND)lParam;
			HWND hMDI = (HWND) SendMessage(lpReefWatchdog->hOwner, WM_GETHWND, HWND_MDI, NULL);
			SetDialogParent(lpReefWatchdog->hPropSheet, hMDI);
		}
		if(wParam == HWND_REEFWATCHDOGDEVICE){
			lpReefWatchdog->hRWDevice = (HWND) lParam;
			lpReefWatchdog->lpDevice = (REEFWATCHDOGDEVICE *)SendMessage(lpReefWatchdog->hRWDevice, WM_GETLONGPTR, NULL, NULL);
		}
		if(wParam == HWND_REEFWATCHDOGDOSING){
			lpReefWatchdog->hRWDosing = (HWND) lParam;
			lpReefWatchdog->lpDosing = (REEFWATCHDOGDOSING *)SendMessage(lpReefWatchdog->hRWDosing, WM_GETLONGPTR, NULL, NULL);
		}
		if(wParam == HWND_REEFWATCHDOGLIGHTING){
			lpReefWatchdog->hRWLighting = (HWND) lParam;
			lpReefWatchdog->lpLighting = (REEFWATCHDOGLIGHTING *) SendMessage(lpReefWatchdog->hRWLighting, WM_GETLONGPTR, NULL, NULL);
		}
		return TRUE;
	case WM_GETHANDLE:
		if(wParam == HANDLE_DEVICE){
			return (LRESULT) lpReefWatchdog->hDevice;
		}
		if(wParam == HANDLE_DEVICE_DOSING){
			return (LRESULT) lpReefWatchdog->hDeviceDosing;
		}
		if(wParam == HANDLE_DEVICE_LIGHTING){
			return (LRESULT) lpReefWatchdog->hDeviceLighting;
		}
		return TRUE;
	case WM_SETHANDLE:
		if(wParam == HANDLE_DEVICE){
			lpReefWatchdog->hDevice = (HANDLE) lParam;

			
			SendMessage(lpReefWatchdog->hRWDosing, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM)lpReefWatchdog->hDevice);
			SendMessage(lpReefWatchdog->hRWLighting, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM)lpReefWatchdog->hDevice);

			if(lpReefWatchdog->hDevice){
				lpReefWatchdog->idTimer = SetTimer(hwnd, NULL, 1000, NULL);
			}
			else{
				KillTimer(hwnd, lpReefWatchdog->idTimer);
			}
			
		}

		return TRUE;
	case WM_CREATE:
		lpReefWatchdog->hPropSheet = CreateReefWatchdogPropertySheet(hwnd);
		ShowWindow(lpReefWatchdog->hPropSheet, SW_SHOW);

		lpReefWatchdog->idTimer = SetTimer(hwnd, NULL, 1000, NULL);

		return TRUE;
	case WM_TIMER:
		if(!lpReefWatchdog->flgBusy){
			CreateThread(NULL, NULL, ReefWatchdogUpdateProc, (LPVOID) lpReefWatchdog, NULL, NULL);
		}
		return TRUE;
	case WM_NCCREATE:
		{
			//Allocate Window Pointer Memory
			lpReefWatchdog = (REEFWATCHDOG *)malloc(sizeof(REEFWATCHDOG));
			memset(lpReefWatchdog, NULL, sizeof(REEFWATCHDOG));
			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpReefWatchdog);
			lpReefWatchdog = (REEFWATCHDOG *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
			
			if(!lpReefWatchdog){
				MessageBox(hwnd, (LPCSTR)"ReefWatchdogWnd: GetWindowLongPtr Failed", (LPCSTR)"Error!", MB_OK);
			}

			break;
		}
	default:
		break;
	}

	return TRUE;

}


HWND CreateReefWatchdogPropertySheet(HWND hOwner)
{
	HWND hPage = NULL;
	HWND hSheet = NULL;
	HWND hReefWatchdog = NULL;


	PROPSHEETPAGE psp;
	memset(&psp, NULL, sizeof(PROPSHEETPAGE));

	psp.dwSize = sizeof(PROPSHEETPAGE);
	psp.dwFlags = PSP_DEFAULT
						| PSP_PREMATURE
						| PSP_USETITLE
						;
	psp.hInstance = GetModuleHandle(NULL);
	psp.pszTemplate = MAKEINTRESOURCE(IDD_RW_DEVICE);
	psp.pszTitle = "Device";
	psp.pfnDlgProc = ReefWatchdogDeviceProc;
	psp.lParam = (LPARAM) hOwner;

	PROPSHEETHEADER psh;
	memset(&psh, NULL, sizeof(PROPSHEETHEADER));

	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE
				| PSH_MODELESS
				| PSH_NOAPPLYNOW
				| PSH_USECALLBACK
				| PSH_DEFAULT;

	psh.hwndParent = hOwner;
	psh.hInstance = GetModuleHandle(NULL);
	psh.pszCaption = "ReefWatchdog Control Panel";
	psh.nPages = 1;
	psh.ppsp = &psp;
	psh.nStartPage = 0;
	psh.pfnCallback = PropSheetProc;

	
	hSheet = (HWND) PropertySheet(&psh);

	HPROPSHEETPAGE hpsp = NULL;	
	
	psp.pszTitle = "Dosing";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_RW_DOSING);
	psp.pfnDlgProc = ReefWatchdogDosingProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);
	hPage = (HWND) PropSheet_IndexToHwnd(hSheet, 1);
	SendMessage(hPage, WM_SETHWND, HWND_OWNER, (LPARAM)hOwner);
	SendMessage(hPage, WM_SETHWND, HWND_PROPERTYSHEET, (LPARAM) hSheet);
	
	psp.pszTitle = "Lighting";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_RW_LIGHTING);
	psp.pfnDlgProc = ReefWatchdogLightingProc;
	hpsp = CreatePropertySheetPage(&psp);

	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);
	hPage = (HWND) PropSheet_IndexToHwnd(hSheet, 2);
	SendMessage(hPage, WM_SETHWND, HWND_OWNER, (LPARAM)hOwner);
	SendMessage(hPage, WM_SETHWND, HWND_PROPERTYSHEET, (LPARAM) hSheet);

	PropSheet_RecalcPageSizes(hSheet);

	return hSheet;


}

DWORD WINAPI ReefWatchdogUpdateProc(LPVOID param)
{
	REEFWATCHDOG *lpReefWatchdog = (REEFWATCHDOG *)param;

	
	REEFWATCHDOGDEVICE *lpDevice = lpReefWatchdog->lpDevice;
	REEFWATCHDOGDOSING *lpDosing = lpReefWatchdog->lpDosing;
	REEFWATCHDOGLIGHTING *lpLighting = lpReefWatchdog->lpLighting;

	//if(lpReefWatchdog->flgBusy){
	//	return FALSE;
	//}
	//lpReefWatchdog->flgBusy = TRUE;

	if(!lpReefWatchdog->hDevice){
		return FALSE;
	}

	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(char));

	////////////////////DEVICE///////////////////////
	SerialSend(lpReefWatchdog->hDevice, "?GETTIME", str);
	sprintf(str, "%s", str);
	Edit_SetText(lpDevice->hEditTime, str);


	////////////////////DOSING///////////////////////
	if(!lpDosing->flgIncrease){
		sprintf(str, "?GETRATEINCREASE%c", lpDosing->motor);
		SerialSend(lpReefWatchdog->hDevice, str, str);
		sscanf(str, "%f", &lpDosing->increase);
		sprintf(str, "%.5f", lpDosing->increase);
		Edit_SetText(lpDosing->hEditIncrease, str);
		lpDosing->flgIncrease = FALSE;
	}

	if(!lpDosing->flgRate){
		sprintf(str, "?GETRATE%c", lpDosing->motor);
		SerialSend(lpReefWatchdog->hDevice, str, str);
		sscanf(str, "%f", &lpDosing->rate);
		sprintf(str, "%.4f", lpDosing->rate);
		Edit_SetText(lpDosing->hEditRate, str);
		lpDosing->flgRate = FALSE;
	}



	////////////////////LIGHTING///////////////////////

	INT pos = NULL;

	SerialSend(lpReefWatchdog->hDevice, "?GETPWMA", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpLighting->hEditA, str);
	SendMessage(lpLighting->hSliderA, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdog->hDevice, "?GETPWMB", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpLighting->hEditB, str);
	SendMessage(lpLighting->hSliderB, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdog->hDevice, "?GETPWMC", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpLighting->hEditC, str);
	SendMessage(lpLighting->hSliderC, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(lpReefWatchdog->hDevice, "?GETPWMD", str);
	sscanf(str, "%d", &pos);
	sprintf(str, "%d (%.1f %c)", pos, (FLOAT)100*((FLOAT)pos/(FLOAT)255), '%');
	Edit_SetText(lpLighting->hEditD, str);
	SendMessage(lpLighting->hSliderD, TBM_SETPOS, TRUE, (LPARAM) pos);
	memset(str, NULL, MAX_STRING*sizeof(CHAR));


	//lpReefWatchdog->flgBusy = FALSE;

	return TRUE;
}