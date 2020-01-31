#pragma once

#include "stdafx.h"
#include "ZeissMicroscope.h"

#define AF_NSTEPS		10
#define AF_DISTANCE		100


INT_PTR CALLBACK AutoFocusProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct AUTOFOCUSTAB_{

	HWND hOwner;
	HWND hZeissMicroscope;

	HWND hCheckLaplacian;
	HWND hCheckSobel;
	HWND hCheckVariance;

	HWND hEditStep;
	HWND hEditDistance;

	HWND hBtnAutoFocus;

	INT nSteps;
	INT distance;

	AUTOFOCUS af;
		
}AUTOFOCUSTAB;