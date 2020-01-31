#pragma once

#include "stdafx.h"
#include "ZeissMicroscope.h"

typedef struct MUTLICHANNELTAB_{

	HWND hOwner;
	HWND hZeissMicroscope;
	HWND hCameraProperties;

	HWND hBtnBrightField;
	HWND hBtnCY5;
	HWND hBtnDapi;
	HWND hBtnFitc;
	HWND hBtnRhodamine;
	HWND hBtnMeasure;

	HWND hCheckBrightField;
	HWND hCheckCY5;
	HWND hCheckDapi;
	HWND hCheckFitc;
	HWND hCheckRhodamine;
	HWND hCheckMeasure;

	HWND hBtnSelect;

	HWND hEditExposure;

	BOOL fBrightField;
	BOOL fCY5;
	BOOL fDapi;
	BOOL fFitc;
	BOOL fRhodamine;

	BOOL fBtn[5];

	DWORD dwCurrentChannel;
	MULTICHANNEL mc;

}MULTICHANNELDLG;



INT_PTR CALLBACK MultiChannelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD UpdateChannelExposure(MULTICHANNELDLG *lpMultiChannelDlg);