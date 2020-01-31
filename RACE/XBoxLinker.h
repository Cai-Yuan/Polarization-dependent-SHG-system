#pragma once

#include "stdafx.h"
#include "XInput.h"

////List Control Positions
//#define LP_CURSOR		0
//#define LP_DATAVIEW		1
//#define LP_XYZSTAGE		2
//
////XBox Linker Messages
//#define XB_LISTSELUP			1024
//#define XB_LISTSELDOWN		1025
//#define XB_INPUT				1026
//#define XB_LEFTSTICK			1027
//#define XB_RIGHTSTICK			1028

typedef struct XBOXLINKER_{

	HWND hOwner;
	HWND hList;
	HWND hActive;
	HWND hLink;
	HANDLE hThread;

	DWORD fXBoxLink;

	XINPUT_STATE xState;
	XINPUT_STATE xStatePrev;

}XBOXLINKER;

INT_PTR CALLBACK XBoxLinkerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD TranslateXBoxState(HWND hwnd, XINPUT_STATE xStatePrev, XINPUT_STATE xState);