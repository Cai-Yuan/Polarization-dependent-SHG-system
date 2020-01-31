#include "stdafx.h"
#include "SC10Shutter.h"
#include "ToolsWIN32.h"
#include "EnumerateDevice.h"


INT_PTR CALLBACK SC10LinkerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SC10LINKER *lpSC10Linker = (SC10LINKER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	INT sel;
	INT cnt;
	switch(msg)
	{
	case WM_INITDIALOG:

		lpSC10Linker = (SC10LINKER *) InitWindowLong(hwnd, sizeof(SC10LINKER));

		if(lpSC10Linker == NULL){
			ThrowMessage(hwnd, "WndSC10Linker: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndXBoWndSC10LinkerxLinker: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpSC10Linker->hwnd = hwnd;

		SetTimer(hwnd, NULL, 1000, NULL);
		lpSC10Linker->flgThread = TRUE;
		CreateThread(NULL, NULL, ShutterSC10Thread, (LPVOID) lpSC10Linker, NULL, NULL);

		break;


	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpSC10Linker->hOwner = (HWND) lParam;
		//if(wParam == HWND_ACTIVE)
		//	lpSC10Linker->hActive = (HWND) lParam;
		if(wParam == HWND_LINK)
			lpSC10Linker->hLink = (HWND) lParam;
		return TRUE;
	case WM_TODO:
		if(wParam == SHUTTER_OPEN){
			lpSC10Linker->flgOpen = TRUE;
			return TRUE;
		}else if(wParam == SHUTTER_CLOSE){
			lpSC10Linker->flgClose = TRUE;
			return TRUE;
		}else if(wParam == SHUTTER_GETSTATE){
			lpSC10Linker->flgGetState = TRUE;
		}else{
			return TRUE;
		}
	case WM_SETHANDLE:
		if(wParam == HANDLE_DEVICE){
			lpSC10Linker->hDevice = (HANDLE) lParam;
			lpSC10Linker->hDevice = lpSC10Linker->hDevice;
			SendMessage(lpSC10Linker->hOwner, WM_SETHWND, HWND_SC10, (LPARAM)hwnd);
		}
		return TRUE;
	case WM_TIMER:
		PostMessage(hwnd, WM_TODO, SHUTTER_GETSTATE, NULL);
		return TRUE;
	case WM_CLOSE:
		lpSC10Linker->flgThread = FALSE;
		Sleep(100);
		SendMessage(lpSC10Linker->hOwner, WM_COMMAND, LOWORD(ID_VIEW_XBOXLINKER), (LPARAM) hwnd);


		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

//Returns TRUE if open or FALSE if closed
DWORD GetStateShutterSC10(HANDLE hDevice)
{
	CHAR strRecieved[MAX_STRING];
	CHAR strClosed[MAX_STRING];

	SerialSend(hDevice, "ens?", strRecieved);
	sprintf(strClosed, "ens?\r0\r> ");

	if(!strcmp(strRecieved, strClosed)){
		return FALSE;
	}else{
		return TRUE;
	}

	return TRUE;
}

DWORD OpenShutterSC10(HANDLE hDevice)
{
	CHAR strRecieved[MAX_STRING];
	CHAR strClosed[MAX_STRING];

	SerialSend(hDevice, "ens?", strRecieved);

	sprintf(strClosed, "ens?\r0\r> ");
	if(!strcmp(strRecieved, strClosed)){
		SerialSend(hDevice, "ens", strRecieved);
	}


	return TRUE;
}

DWORD CloseShutterSC10(HANDLE hDevice)
{
	CHAR strRecieved[MAX_STRING];
	CHAR strOpened[MAX_STRING];

	SerialSend(hDevice, "ens?", strRecieved);

	sprintf(strOpened, "ens?\r1\r> ");
	
	if(!strcmp(strRecieved, strOpened)){
		SerialSend(hDevice, "ens", strRecieved);
	}

	return TRUE;
}

DWORD WINAPI ShutterSC10Thread(LPVOID param)
{

	SC10LINKER *lpSC10Linker = (SC10LINKER *) param;

	while(lpSC10Linker->flgThread){
		
		if(lpSC10Linker->flgOpen){
			OpenShutterSC10(lpSC10Linker->hDevice);
			lpSC10Linker->flgOpen = FALSE;
		}

		if(lpSC10Linker->flgClose){
			CloseShutterSC10(lpSC10Linker->hDevice);
			lpSC10Linker->flgClose = FALSE;
		}

		if(lpSC10Linker->flgGetState){
			DWORD state = GetStateShutterSC10(lpSC10Linker->hDevice);
			SendMessage(lpSC10Linker->hLink, WM_INPUT, (WPARAM) lpSC10Linker->hwnd, (LPARAM) state);
		}


		Sleep(100);

	}


	return TRUE;
}

DWORD OpenShutterSC10(HWND hSC10Linker)
{
	PostMessage(hSC10Linker, WM_TODO, (WPARAM) SHUTTER_OPEN, NULL);

	return TRUE;
}

DWORD CloseShutterSC10(HWND hSC10Linker)
{
	PostMessage(hSC10Linker, WM_TODO, (WPARAM) SHUTTER_CLOSE, NULL);

	return TRUE;
}