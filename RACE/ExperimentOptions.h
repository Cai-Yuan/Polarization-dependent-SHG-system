#pragma once

#include "stdafx.h"
#include "ZeissMicroscope.h"

typedef struct EXPERIMENTOPTIONS_{

	HWND hOwner;
	HWND hPropertySheet;
	HWND hZeissMicroscope;

	HWND hButtonStart;

	HWND hCheckAutoFocus;
	HWND hCheckMosaic;
	HWND hCheckMultiChannel;
	HWND hCheckTimeLapse;
	HWND hCheckZStack;

	HWND hEditPath;

	ZEISSEXPERIMENT ze;
	

	CHAR strPath[MAX_STRING];

}EXPERIMENTOPTIONS;

INT_PTR CALLBACK ExperimentOptionsProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);