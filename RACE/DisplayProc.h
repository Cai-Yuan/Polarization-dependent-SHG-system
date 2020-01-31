#pragma once

#include "stdafx.h"
#include "Object.h"
#include "Microscope.h"
#include "PDM.h"

typedef struct LM_DISPLAY_{
	HWND hOwner;
	HWND hDataView;

	HWND hEditDiameter;
	HWND hEditPixPerUm;
	HWND hEditX;
	HWND hEditY;
	
	HWND hCheckA;
	HWND hCheckB;
	HWND hCheckX;
	HWND hCheckY;

	INT wDisplay;		//Needs Edit Box
	INT hDisplay;		//Needs Edit Box

	DWORD flgAPosition;
	DWORD flgBPosition;
	DWORD flgXPosition;
	DWORD flgYPosition;
	DWORD flgFrameIn;

	FOV fov;
	FLOAT xFOV;
	FLOAT yFOV;
	FLOAT pixPerUm;
	FLOAT roiDiameter;

	PDMFRAME pdmIn;

	OBJECTSET os;
}LM_DISPLAY;

//"DisplayProc.cpp"
INT_PTR CALLBACK DisplayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD UpdateFieldOfView(LM_DISPLAY *lpDisplay);