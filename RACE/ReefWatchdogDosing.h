#pragma once
#include "stdafx.h"

#include "ReefWatchdog.h"

typedef struct REEFWATCHDOGDOSING_{

	HWND hOwner;
	HWND hReefWatchdogProc;

	HWND hRadioA;
	HWND hRadioB;
	HWND hRadioC;
	HWND hRadioD;

	HWND hEditRate;
	HWND hEditVolume;
	HWND hEditIncrease;

	HWND hCheckVolume;

	HWND hBtnSet;

	HANDLE hDevice;

	CHAR motor;
	FLOAT rate;
	FLOAT increase;

	DWORD flgRate;
	DWORD flgIncrease;

	DWORD idTimer;

}REEFWATCHDOGDOSING;

INT_PTR CALLBACK ReefWatchdogDosingProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

