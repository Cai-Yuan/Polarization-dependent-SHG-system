#pragma once

#include "stdafx.h"

typedef struct CELLSTRETCHER_{

	HWND hOwner;

	HWND hRadioTicks;
	HWND hRadioUm;
	HWND hRadioInch;

	HWND hRadioA;
	HWND hRadioB;
	HWND hRadioC;
	HWND hRadioD;
	HWND hRadioE;
	HWND hRadioF;

	HWND hRadioFull;
	HWND hRadioHalf;
	HWND hRadioQuarter;
	HWND hRadioEighth;

	HWND hRadioCW;
	HWND hRadioCCW;

	HWND hBtnSetHome;
	HWND hBtnGoHome;
	HWND hBtnMove;
	HWND hBtnJog;
	HWND hBtnConnect;

	HWND hComboPort;

	HWND hEditGoTo;
	HWND hEditVelocity;
	HWND hEditPosAbsolute;
	HWND hEditPosRelative;

	long int valGoTo;
	FLOAT valVelocity;

	CHAR port;
	HANDLE hStretcher;

	CHAR motor;

	INT idTimer;

	DWORD flgJog;

}CELLSTRETCHER;


INT_PTR CALLBACK CellStretcherProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT GetCellStretcherControls(CELLSTRETCHER *lpCellStretch, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT InitCellStretcherControls(CELLSTRETCHER *lpCellStretch, HWND hwnd, WPARAM wParam, LPARAM lParam);

HANDLE FindCellStretcher(CHAR *port);