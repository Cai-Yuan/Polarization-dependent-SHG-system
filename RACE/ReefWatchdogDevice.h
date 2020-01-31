#pragma once

#include "stdafx.h"

typedef struct REEFWATCHDOGDEVICE_{

	HWND hwnd;
	HWND hOwner;
	HWND hReefWatchdogProc;
	HANDLE hDevice;
	HANDLE hLighting;
	HANDLE hDosing;

	CHAR numCom;
	CHAR numComDosing;
	CHAR numComLighting;

	HWND hBtnConnect;
	HWND hBtnDownload;
	HWND hBtnUpload;

	HWND hEditStatus;
	HWND hEditTime;

}REEFWATCHDOGDEVICE;

DWORD WINAPI ReefWatchdogDeviceInitProc(LPVOID param);
INT_PTR CALLBACK ReefWatchdogDeviceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
