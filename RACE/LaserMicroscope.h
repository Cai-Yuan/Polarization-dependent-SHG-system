#pragma once

#include "stdafx.h"
#include "Microscope.h"

#include "StageProc.h"
#include "SystemProc.h"
#include "DeviceProc.h"
#include "DisplayProc.h"
#include "TrackerProc.h"
#include "AutoFocusProc.h"

#include "UNO32.h"
//#include "A3200.h"
#include "A3200Stage.h"

typedef struct LM_XBOX_{
	FLOAT xStick;
	FLOAT yStick;
	FLOAT zStick;

	INT xPolStick;
	INT yPolStick;
	INT zPolStick;

	DWORD flgLeftShoulder;
	DWORD flgRightShoulder;
	DWORD flgAButton;
	DWORD flgBButton;
	DWORD flgXButton;
	DWORD flgYButton;
	DWORD flgStartButton;
	DWORD flgBackButton;
	DWORD flgLeftTrigger;
	DWORD flgRightTrigger;

	DWORD flgDPadLeft;
	DWORD flgDPadRight;
	DWORD flgDPadUp;
	DWORD flgDPadDown;

	DWORD flgXInput;
	DWORD flgYInput;
	DWORD flgZInput;

}LM_XBOX;

typedef struct LM_SC10_{
	DWORD flgShutter;
}LM_SC10;

typedef struct LASERMICROSCOPE_{

	LM_STAGE *lpStage;
	LM_SYSTEM *lpSystem;
	//LM_DEVICE *lpDevice;
	struct LM_DEVICE_ *lpDevice;
	LM_DISPLAY *lpDisplay;
	LM_TRACKER *lpTracker;
	//LM_AUTOFOCUS *lpAutoFocus;
	struct LM_AUTOFOCUS_ *lpAutoFocus;

	HWND hSystem;
	HWND hStage;
	HWND hDataView;
	HWND hCamProp;

	HWND hwnd;
	HWND hOwner;
	HWND hShutter;
	HWND hDevice;
	HWND hDisplay;
	HWND hTracker;
	HWND hAutoFocus;

	DWORD wFrame;
	DWORD hFrame;

	HWND hEditPixPerUm;
	HWND hEditRoiDiameter;

	DWORD flgThread;

	DWORD flgZStack;
	DWORD flgAutoFocus;
	DWORD flgGetFrame;
	DWORD flgFrameIn;

	WPARAM wParam;
	LPARAM lParam;

	PDMFRAME pdmf;

	//Z-Stack Parameters
	INT slice;
	FLOAT stepSize;
	DWORD nSlice;
	CHAR *strFolder;
	DOUBLE_3DPOINT ptZRef;
	//PDMFRAME pdmIn;



}LASERMICROSCOPE;

//"LaserMicroscope.h"
HWND CreateLaserMicroscopePropertySheet(HWND hOwner);
DWORD GetNavigationVector(DOUBLE_3DPOINT ptCurr, DOUBLE_3DPOINT ptDest, FPOINT center, VECTOR *vector);
DWORD GetVelocityVector(LM_DISPLAY *lpDisplay, FLOAT velX, FLOAT velY, VECTOR *vector, TEXTR *text);
DWORD GetZAxisIndicator(LM_STAGE *lpStage, CIRCLE *circle, TEXTR *text);

//"LaserMicroscopeProc.cpp"
LRESULT CALLBACK LaserMicroscopeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//"LaserMicroscopeThread.cpp"
DWORD WINAPI LaserMicroscopeThread(LPVOID param);
DWORD DisplayLaserPosition(LASERMICROSCOPE *lpLaserMicroscope, DOUBLE_3DPOINT pt);
DWORD UpdateVelocity(LASERMICROSCOPE *lpLaserMicroscope);
DWORD CreateDisplayObjects(LASERMICROSCOPE *lpLaserMicroscope);
DWORD UpdateInflightVelocity(LASERMICROSCOPE *lpMicroscope);
//DWORD UpdateVelocity(LM_STAGE *lpStage);

//"DeviceProc.cpp"
DWORD XBoxInput(LASERMICROSCOPE *lpMicroscope, WPARAM wParam, LPARAM lParam);