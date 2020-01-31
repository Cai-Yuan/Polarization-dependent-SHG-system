#pragma once

#include "stdafx.h"
#include "ZeissMicroscope.h"

#define XAXIS	0
#define YAXIS	1
#define ZAXIS	2

typedef struct STAGEPROPERTIES_{

	HWND hOwner;

	HWND hButtonZUp;
	HWND hButtonZDown;
	HWND hButtonAutoFocus;
	HWND hButtonGoTo;
	HWND hButtonGoToZ;
	HWND hButtonStart;
	HWND hButtonBrowse;

	HWND hEditVelocity;
	HWND hEditXYVelocity;
	HWND hEditDelay;
	HWND hEditGoTo;
	HWND hEditXPos;
	HWND hEditYPos;
	HWND hEditZPos;
	HWND hEditZStep;
	HWND hEditFNStep;
	HWND hEditFStep;
	HWND hEditRow;
	HWND hEditCol;
	HWND hEditOverlap;
	HWND hEditPath;

	HWND hRadioRecall;

	HWND hZeissScope;

	BOOL flgButtonZUp;
	BOOL flgButtonZDown;
	BOOL flgFocusRecall;
	BOOL flgAutoFocus;
	BOOL flgMosaic;

	INT	zAxisVelocity;
	INT zAxisDelay;

	INT xyAxisVelocity;

	INT xAxisPos;
	INT yAxisPos;
	INT zAxisPos;

	INT xAxisGoTo;
	INT yAxisGoTo;
	INT zAxisGoTo;

	INT zStep;

	INT fNumStep;
	INT fStep;
	
	MULTIFOV mfov;

	CHAR strPath[MAX_STRING];

}STAGEPROPERTIES;

INT_PTR CALLBACK StagePropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);