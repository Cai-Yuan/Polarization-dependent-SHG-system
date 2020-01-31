#pragma once

#include "stdafx.h"
#include "Microscope.h"

typedef struct LM_STAGE_{
	HWND hwnd;
	HWND hOwner;

	HWND hEditXPos;
	HWND hEditYPos;
	HWND hEditZPos;
	HWND hEditXVel;
	HWND hEditYVel;
	HWND hEditZVel;
	
	HWND hEditXVelFine;
	HWND hEditXVelCoarse;
	HWND hEditYVelFine;
	HWND hEditYVelCoarse;
	HWND hEditZVelFine;
	HWND hEditZVelCoarse;

	HWND hEditXVelAuto;
	HWND hEditYVelAuto;
	HWND hEditZVelAuto;

	HWND hCheckXAxis;
	HWND hCheckYAxis;
	HWND hCheckZAxis;
	HWND hCheckZDelay;

	HWND hBtnZStack;

	FLOAT xVel;
	FLOAT yVel;
	FLOAT zVel;
	FLOAT xVelPrev;
	FLOAT yVelPrev;
	FLOAT zVelPrev;
	FLOAT xVelFine;
	FLOAT yVelFine;
	FLOAT zVelFine;
	FLOAT xVelCoarse;
	FLOAT yVelCoarse;
	FLOAT zVelCoarse;
	FLOAT xVelMax;
	FLOAT yVelMax;
	FLOAT zVelMax;

	FLOAT xScale;
	FLOAT yScale;
	FLOAT zScale;
	FLOAT xPolStage;
	FLOAT yPolStage;
	FLOAT zPolStage;
	FLOAT xPolStick;
	FLOAT yPolStick;
	FLOAT zPolStick;

	DWORD flgHold;
	DWORD flgZero;
	DWORD flgInitMoveTo;
	DWORD flgMoveTo;
	DWORD flgMoveToZ;
	DWORD flgZDelay;
	DWORD flgAuto;
	DWORD flgAPosition;
	DWORD flgBPosition;
	DWORD flgXPosition;
	DWORD flgYPosition;
	DWORD flgVelUpdate;

	//DWORD flgShutter;		//Move to a more suitable struct

	DOUBLE_3DPOINT dptHold;
	DOUBLE_3DPOINT dptPrev;
	DOUBLE_3DPOINT dptCurrent;
	DOUBLE_3DPOINT dptMoveTo;
	DOUBLE_3DPOINT dptA;
	DOUBLE_3DPOINT dptB;
	DOUBLE_3DPOINT dptX;
	DOUBLE_3DPOINT dptY;
	P3DSET *pSet;


}LM_STAGE;


INT_PTR CALLBACK StageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);