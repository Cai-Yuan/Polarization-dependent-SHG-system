#pragma once

#include "stdafx.h"

typedef struct REEFWATCHDOGLIGHTING_{

	HWND hOwner; 
	HWND hReefWatchdogProc;
	HANDLE hDevice;

	HWND hSliderA;
	HWND hSliderB;
	HWND hSliderC;
	HWND hSliderD;

	HWND hEditA;
	HWND hEditB;
	HWND hEditC;
	HWND hEditD;

	HWND hCheckPhotoCycle;

	DWORD idTimer;

}REEFWATCHDOGLIGHTING;

INT_PTR CALLBACK ReefWatchdogLightingProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);