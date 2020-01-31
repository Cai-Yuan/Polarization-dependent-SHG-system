#pragma once

#include "stdafx.h"

typedef struct ZEISSOBJECTIVEDLG_{

	HWND hOwner;
	HWND hZeissMicroscope;

	HWND hBtn10x;
	HWND hBtn40xOil;
	HWND hBtn63xOil;
	HWND hBtn4x;
	HWND hBtn20x;
	HWND hBtn40x;

	HWND hBtnEyePiece;
	HWND hBtnCamera;

}ZEISSOBJECTIVEDLG;

INT_PTR CALLBACK ZeissObjectiveProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);