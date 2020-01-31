#pragma once

#include "stdafx.h"


typedef struct MOTORWATCHDOG_{
	HWND hOwner;
	HWND hListMotor;
	HWND hBtnSet;
	HWND hBtnStop;
	HWND hBtnGoTo;
	HWND hEditRate;
	HWND hEditPosition;

	HANDLE hDevice;

	INT selMotor;
}MOTORWATCHDOG;

INT_PTR CALLBACK MotorWatchdogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT OnInitDialogMotorWatchdog(MOTORWATCHDOG *lpMotorWatchdog, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMotorWatchdogSetHandle(MOTORWATCHDOG *lpMotorWatchdog, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMotorWatchdogCommand(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMotorWatchdogButtonClicked(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMotorWatchdogEditChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMotorWatchdogSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMotorWatchdogSetHWND(MOTORWATCHDOG *lpMotorWatchdog, HWND hwnd, WPARAM wParam, LPARAM lParam);