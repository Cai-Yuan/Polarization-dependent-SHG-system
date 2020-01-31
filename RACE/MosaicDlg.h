#pragma once

#include "stdafx.h"
#include "ZeissMicroscope.h"

INT_PTR CALLBACK MosaicProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct MOSAICTAB_{
	
	HWND hOwner;
	HWND hZeissMicroscope;

	HWND hCheckAutoFocus;
	HWND hCheckFocalRecall;

	HWND hEditRows;
	HWND hEditCols;
	HWND hEditOverlap;

	HWND hRadioStitched;
	HWND hRadioUnstitched;

	FIELDOFVIEW fov;



}MOSAICTAB;
