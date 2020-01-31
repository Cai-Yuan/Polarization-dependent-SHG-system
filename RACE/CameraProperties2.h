#pragma once

#include "stdafx.h"
#include "ZeissCam.h"
#include "WndLink.h"

#define INIT_EXPOSURE		40000

#define CAMERA_ZEISS_HSM	100
#define CAMERA_ZEISS_MRM	101
#define CAMERA_AXIOCAM		102
#define CAMERA_PIXELINK		103
#define CAMERA_PHOTONFOCUS	104
#define CAMERA_EMULATE		105
#define CAMERA_DESKTOP		106

#define STR_NOSELECTION		"No Selection"
#define STR_EMULATE_CAM		"Emulate(From File)"
#define STR_ZEISS_HSM		"Zeiss HSm"
#define STR_ZEISS_MRM		"Zeiss MRm"
#define STR_AXIOCAM			"Axiocam"
#define STR_PIXELINK_A781	"PixeLINK - A781"
#define STR_PHOTONFOCUS		"PhotonFocus"
#define STR_DESKTOP			"Desktop"

#define CPM_CAMINIT			36864
#define CPM_CAMCLOSE		36865
#define CPM_CAPTURE		36866
#define CPM_SETWIDTH		36867
#define CPM_SETHEIGHT		36868
#define CPM_GETEXPOSURE		36869
#define CPM_SETEXPOSURE		36870

typedef struct CAPTURE_{
	HANDLE hThread;
	HWND hOwner;
	HWND hRACE;
	HWND hDataManager;

	BOOL flagStream;
	BOOL flagSave;
	BOOL flagClose;
	BOOL flagOpen;
	BOOL flagIdle;
	BOOL flagExposure;

	VOID *lpVoid;
	INT exposure;
	FLOAT frameRate;
	UINT sampleRate;

	DWORD frame;

	WNDLINKSET wls;

}CAPTURE;


typedef struct CAMERAPROPERTIES_{

	HWND hwnd;
	HWND hFocus;
	HWND hOwner;
	HWND hFrame;
	HWND hDataManager;

	//Handles to Controls
	HWND hRadioBit2;
	HWND hRadioBit4;
	HWND hRadioBit8;
	HWND hRadioBit12;

	HWND hRadioBin1;
	HWND hRadioBin2;
	HWND hRadioBin3;
	HWND hRadioBin4;

	HWND hRadioMonochrome;
	HWND hRadioRGB;
	HWND hRadioCMYK;

	HWND hCheckFrameRate;

	HWND hComboCamera;
	HWND hButtonStream;
	HWND hButtonSave;
	HWND hBtnStop;
	HWND hButtonPath;
	HWND hButtonSnap;
	HWND hButtonAutoExposure;
	

	HWND hEditPath;
	HWND hEditExposure;
	HWND hEditWidth;
	HWND hEditHeight;
	HWND hEditFrameRate;
	HWND hEditSampleRate;

	HWND hSlider;

	CHAR strPath[MAX_STRING];

	BOOL flagStream;
	BOOL flagSave;
	BOOL flagPause;

	INT exposure;
	INT nBit;
	INT nBin;
	INT camera;

	FLOAT frameRate;
	FLOAT prevFrameRate;

	ZEISSPROC zeissProc;
	//ZEISSPROC zeissMRm;
	ZEISSPROC zeissProcHSm;

	CAPTURE cs;

	HANDLE hCamera;


}CAMERAPROPERTIES;

DWORD InitializeCameraProperties(HWND hwnd, CAMERAPROPERTIES *lpCamProp);
INT_PTR CALLBACK CameraPropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


//OnMessage Functions
LRESULT OnCameraPropertiesButtonClicked(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesEditChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesSetHWND(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesGetHWND(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesInitializeCamera(HWND hwnd, WPARAM wParam, LPARAM lParam);