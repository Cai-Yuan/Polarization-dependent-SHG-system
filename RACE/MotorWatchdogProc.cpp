#include "stdafx.h"

#include "MotorWatchdog.h"
#include "EnumerateDevice.h"
#include "RACE.h"


INT_PTR CALLBACK MotorWatchdogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MOTORWATCHDOG *lpMotorWatchdog = (MOTORWATCHDOG *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:
		OnInitDialogMotorWatchdog(lpMotorWatchdog, hwnd, wParam, lParam);
		break;
	case WM_COMMAND:
		//OnMotorWatchdogCommand(hwnd, msg, wParam, lParam);
		return FALSE;
	case WM_SETHWND:
		OnMotorWatchdogSetHWND(lpMotorWatchdog,hwnd, wParam, lParam);
		return TRUE;
	case WM_GETHWND:
		//OnMotorWatchdogGetHWND(hwnd, wParam, lParam);
		return TRUE;
	case WM_SETHANDLE:
		OnMotorWatchdogSetHandle(lpMotorWatchdog, hwnd, wParam, lParam);
	case WM_CREATELINK:
		//AddWndLink(&lpCamProp->cActive->wls, (HWND) wParam);
		return FALSE;
	case WM_CLOSING:
		//returnOnMotorWatchdogClosing(hwnd, wParam, lParam);
		return FALSE;
	case WM_CLOSE:
		//return OnMotorWatchdogClose(hwnd, wParam, lParam);
		return FALSE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;
	default:
		return FALSE;
	}

	return FALSE;

}

LRESULT OnInitDialogMotorWatchdog(MOTORWATCHDOG *lpMotorWatchdog, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	lpMotorWatchdog = (MOTORWATCHDOG *) InitWindowLongRACE(hwnd, sizeof(MOTORWATCHDOG));
	if(!lpMotorWatchdog)return FALSE;

	ConnectToDeviceThreaded(hwnd, "?ID", "MotorWatchdog", 115200);
	
	lpMotorWatchdog->hListMotor = GetDlgItem(hwnd, IDC_LIST_MOTOR);
	lpMotorWatchdog->hBtnSet = GetDlgItem(hwnd, IDC_BTN_SET);
	lpMotorWatchdog->hBtnStop = GetDlgItem(hwnd, IDC_BTN_STOP);
	lpMotorWatchdog->hBtnGoTo = GetDlgItem(hwnd, IDC_BTN_GOTO);
	lpMotorWatchdog->hEditRate = GetDlgItem(hwnd, IDC_EDIT_RATE);
	lpMotorWatchdog->hEditPosition = GetDlgItem(hwnd, IDC_EDIT_POSITION);
	
	return TRUE;
}

LRESULT OnMotorWatchdogSetHandle(MOTORWATCHDOG *lpMotorWatchdog, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	
	switch(wParam)
	{
	case HWND_OWNER:
			lpMotorWatchdog->hOwner = (HWND) lParam;
			return FALSE;
	default:
		return TRUE;
	}

	return TRUE;
}

LRESULT OnMotorWatchdogSetHWND(MOTORWATCHDOG *lpMotorWatchdog, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	
	if(wParam == HANDLE_DEVICE){
		lpMotorWatchdog->hDevice = (MOTORWATCHDOG *) lParam;
		if(!lpMotorWatchdog->hDevice){
			ConnectToDeviceThreaded(hwnd, "ID", "MotorWatchdog", 115200);
		}
	}

	
	return TRUE;
}

LRESULT OnMotorWatchdogCommand(HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	switch(HIWORD(wParam))
	{
	case BN_CLICKED:
		return OnMotorWatchdogButtonClicked(hwnd, wParam, lParam);
	case EN_CHANGE:
		return OnMotorWatchdogEditChanged(hwnd, wParam, lParam);
	case CBN_SELCHANGE:
		return OnMotorWatchdogSelChanged(hwnd, wParam, lParam);
	default:
		return TRUE;
	}

}

LRESULT OnMotorWatchdogButtonClicked(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	MOTORWATCHDOG *lpMotorWatchdog = (MOTORWATCHDOG *) GetWindowUserData(hwnd);

	switch(LOWORD(wParam))
	{
	case IDC_BTN_SET:
		return FALSE;
	case IDC_BTN_STOP:
		return FALSE;
	case IDC_BTN_GOTO:
		return FALSE;
	default:
		return TRUE;
	}
}

LRESULT OnMotorWatchdogEditChanged(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	MOTORWATCHDOG *lpMotorWatchdog = (MOTORWATCHDOG *) GetWindowUserData(hwnd);
	lpMotorWatchdog->selMotor = ListBox_GetCurSel(lpMotorWatchdog->hListMotor);

	

	return TRUE;
}

LRESULT OnMotorWatchdogSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}