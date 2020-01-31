#pragma once

#include "stdafx.h"
#include "ReefWatchdogLighting.h"
#include "ReefWatchdogDosing.h"
#include "ReefWatchdogDevice.h"

#define HWND_REEFWATCHDOGDEVICE		4000
#define HWND_REEFWATCHDOGLIGHTING	4001
#define HWND_REEFWATCHDOGDOSING		4002


typedef struct REEFWATCHDOG_{

	HWND hOwner;
	HWND hPropSheet;
	HWND hRWDevice;
	HWND hRWDosing;
	HWND hRWLighting;

	HANDLE hDevice;
	HANDLE hDeviceLighting;
	HANDLE hDeviceDosing;

	REEFWATCHDOGLIGHTING *lpLighting;
	REEFWATCHDOGDOSING *lpDosing;
	REEFWATCHDOGDEVICE *lpDevice;

	DWORD idTimer;

	DWORD flgBusy;
	

}REEFWATCHDOG;

LRESULT CALLBACK ReefWatchdogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND CreateReefWatchdogPropertySheet(HWND hOwner);
DWORD WINAPI ReefWatchdogUpdateProc(LPVOID param);