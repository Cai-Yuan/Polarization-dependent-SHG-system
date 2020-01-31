#pragma once
#include "stdafx.h"


typedef struct SC10LINKER_{

	HWND hwnd;
	HWND hOwner;
	HWND hLink;
	HANDLE hDevice;

	DWORD flgOpen;
	DWORD flgClose;
	DWORD flgGetState;
	DWORD flgThread;

}SC10LINKER;


#define SHUTTER_OPEN		100
#define SHUTTER_CLOSE		101
#define SHUTTER_GETSTATE	102

INT_PTR CALLBACK SC10LinkerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD OpenShutterSC10(HANDLE hDevice);
DWORD CloseShutterSC10(HANDLE hDevice);
DWORD WINAPI ShutterSC10Thread(LPVOID param);
DWORD OpenShutterSC10(HWND hSC10Linker);
DWORD CloseShutterSC10(HWND hSC10Linker);