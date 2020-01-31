#pragma once

#include "stdafx.h"
#include "ZeissMicroscope.h"

typedef struct ZSTACKTAB_{

	HWND hOwner;
	HWND hZeissMicroscope;

	HWND hEditNSlice;
	HWND hEditDistance;

	ZSTACK zs;

}ZSTACKTAB;

INT_PTR CALLBACK ZStackProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);