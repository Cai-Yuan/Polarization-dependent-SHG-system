#pragma once

#include "stdafx.h"
#include "LaserMicroscope.h"

//Auto Focus Flags
#define AF_WAIT	100		//Set if the stage should wait for position

//Auto Focus Filters
#define AF_LAPLACIAN	1
#define AF_SOBEL		2
#define AF_VARIANCE		4

typedef struct AUTOFOCUS{

	INT nStep;
	FLOAT stepDist;
	DWORD filter;	//Focal Measure Filter
	DOUBLE *measure;

}AUTOFOCUS;

typedef struct LM_AUTOFOCUS_{
	HWND hOwner;
	
	HWND hCheckLaplacian;
	HWND hCheckSobel;
	HWND hCheckVariance;

	HWND hEditStep;
	HWND hEditDistance;

	HWND hBtnAutoFocus;

	INT nSteps;
	INT distance;

	AUTOFOCUS af;
	DWORD flgAutoFocus;
	DWORD flgInitAutoFocus;

	PDMFRAME pdmf;

}LM_AUTOFOCUS;

INT_PTR CALLBACK AutoFocusProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//AutoFocus.cpp
DWORD GetStreamImage(struct LASERMICROSCOPE_ *laserMicroscope, DWORD w, DWORD h, UCHAR *img);
FLOAT CalculateFocalMeasure(UINT w, UINT h, FLOAT *img, DWORD filter);

DWORD InitAutoFocus(struct LASERMICROSCOPE_ *lpMicroscope);
FLOAT GetFocalMeasure(struct LASERMICROSCOPE_ *lpMicroscope);