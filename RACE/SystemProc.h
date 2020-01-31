#pragma once

#include "stdafx.h"

typedef struct LM_SYSTEM_{
	HWND hwnd;
	HWND hOwner;

	HWND hEditProfile;
	HWND hBtnLoad;
	HWND hBtnSave;
	HWND hEditSampleRate;

	DWORD flgAutoFocus;
	DWORD flgThread;
	DWORD flgFrameIn;

	FLOAT sampleRate;

	FLOAT pixPerUm;

	FLOAT roiDiameter;
}LM_SYSTEM;

INT_PTR CALLBACK SystemProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD OnSystemOpenProfile(HWND hwnd, WPARAM wParam, LPARAM lParam);