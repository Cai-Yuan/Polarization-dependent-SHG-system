#pragma once

#include "stdafx.h"

typedef struct PROPERTYMANAGER{

	//Field of View
	INT xFieldOfView;
	INT yFieldOfView;
	INT zFieldOfView;

	INT selAlgorithm;

	HWND hEditX;
	HWND hEditY;
	HWND hEditZ;
	HWND hOwner;
	HWND hFocus;
	HWND hAlgorithm;
	HWND hButtonLoop;
	HWND hEditLoop;

	INT fps;

}PROPERTYMANAGER;

INT_PTR CALLBACK PropertyManagerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);