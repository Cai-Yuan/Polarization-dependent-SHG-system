#pragma once

#include "stdafx.h"
#include "LaserMicroscope.h"

#define OFFSET 7000


typedef struct LM_DEVICE_{
	HWND hOwner;
	HWND hSC10;
	HWND hXBoxLinker;

	HANDLE hAero;

	DWORD flgShutter;

	//LM_XBOX *lpXBox;
	//LM_SC10 *lpSC10;
	struct LM_XBOX_ *lpXBox;
	struct LM_SC10_ *lpSC10;

}LM_DEVICE;


//"DeviceProc.cpp"
INT_PTR CALLBACK DeviceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

