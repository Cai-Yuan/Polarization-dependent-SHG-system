#pragma once

#include "stdafx.h"
#include "ZeissCam.h"
#include "WndLink.h"

#define INIT_EXPOSURE			40000

#define CAMERA_ZEISS_HSM		100
#define CAMERA_ZEISS_MRM		101
#define CAMERA_AXIOCAM			102
#define CAMERA_PIXELINK			103
#define CAMERA_PHOTONFOCUS		104
#define CAMERA_EMULATE			105
#define CAMERA_DESKTOP			106
#define CAMERA_RACE				107
#define CAMERA_CHAMELEON		108

#define STR_NOSELECTION				"No Selection"
#define STR_EMULATE_CAM				"Emulate(From File)"
#define STR_ZEISS_HSM				"Zeiss HSm"
#define STR_ZEISS_MRM				"Zeiss MRm"
#define STR_AXIOCAM					"Axiocam"
#define STR_PIXELINK_A781			"PixeLINK - A781"
#define STR_PHOTONFOCUS				"PhotonFocus"
#define STR_DESKTOP					"Desktop"
#define STR_CHAMELEON_CMLN_13S2M	"Chameleon CMLN-13S2M"

#define CPM_CAMINIT			36864
#define CPM_CAMCLOSE		36865
#define CPM_CAPTURE			36866
#define CPM_SETWIDTH		36867
#define CPM_SETHEIGHT		36868
#define CPM_GETEXPOSURE		36869
#define CPM_SETEXPOSURE		36870

typedef struct VOIDSET_{
	DWORD szType;
	DWORD szPtr;
	DWORD n;
	VOID *ptr;
}VOIDSET;

typedef struct CAPTURE_{
	HWND hOwner;
	HWND hRACE;
	HWND hDataManager;
	HWND hDataView;
	HANDLE hThread;

	CHAR strID[MAX_STRING];
	CHAR strSource[MAX_STRING];
	CHAR strDest[MAX_STRING];
	CHAR strPath[MAX_STRING];

	BOOL flgStream;
	BOOL flgSave;
	BOOL flgClose;
	BOOL flgPause;
	BOOL flgOpen;
	BOOL flgIdle;
	BOOL flgExposure;

	INT exposure;
	FLOAT frameRate;
	FLOAT baseFrameRate;
	UINT sampleRate;

	DWORD frame;

	WNDLINKSET wls;
	VOID *lpVoid;
}CAPTURE;

typedef struct CAPTURESET_{
	DWORD size;
	DWORD nCapture;
	CAPTURE *c;
}CAPTURESET;


typedef struct CAMERAPROPERTIES_{

	HWND hwnd;
	HWND hFocus;
	HWND hOwner;
	HWND hFrame;
	HWND hDataManager;
	HWND hCamCapture;

	HWND hCheckFrameRate;

	HWND hComboCamera;
	HWND hListStream;
	HWND hBtnStream;
	HWND hBtnSave;
	HWND hBtnStop;
	HWND hBtnPath;
	HWND hBtnSnap;
	HWND hBtnAutoExposure;
	

	HWND hEditPath;
	HWND hEditExposure;
	HWND hEditWidth;
	HWND hEditHeight;
	HWND hEditFrameRate;
	HWND hEditSampleRate;

	HWND hSlider;

	CHAR strPath[MAX_STRING];

	BOOL flgStream;
	BOOL flgSave;
	BOOL flgPause;

	INT exposure;
	INT camera;

	FLOAT frameRate;
	UINT sampleRate;
	FLOAT prevFrameRate;

	ZEISSPROC zeissProc;
	//ZEISSPROC zeissMRm;
	ZEISSPROC zeissProcHSm;

	//CAPTURE capture;
	CAPTURE *cActive;
	CAPTURESET cs;

	HANDLE hCamera;


}CAMERAPROPERTIES;

LRESULT OnCameraPropertiesChangeCapture(CAMERAPROPERTIES *lpCamProp);
LRESULT OnCameraPropertiesCapture(CAMERAPROPERTIES *lpCamProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT OnInitDialogCameraProperties(CAMERAPROPERTIES *lpCamProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD InitializeCameraProperties(HWND hwnd, CAMERAPROPERTIES *lpCamProp);
INT_PTR CALLBACK CameraPropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD OnUpdateActiveCameraProperteis(CAMERAPROPERTIES *lpCamProp);

//OnMessage Functions
LRESULT OnCameraPropertiesButtonClicked(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesEditChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesSetHWND(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesGetHWND(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesInitializeCamera(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesClose(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesComboSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesListSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesClosing(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCameraPropertiesChangeActive(CAMERAPROPERTIES *lpCamProp);
LRESULT OnCameraPropertiesDiscoverDevices(CAMERAPROPERTIES *lpCamProp);
LRESULT OnCameraPropertiesStreamClosed(HWND hwnd, WPARAM wParam, LPARAM lParam);

DWORD Alloc(CAPTURESET *cs, DWORD size);
DWORD Append(CAPTURESET *cs, CAPTURE c);
DWORD Delete(CAPTURESET *cs, DWORD index);
DWORD Find(CAPTURESET *cs, HWND hwnd, DWORD *index);
DWORD Find(CAPTURESET *cs, CHAR *strID, DWORD *index);


DWORD WINAPI DesktopCaptureThread(LPVOID param);

DWORD CameraProperties_EnableStreamControls(HWND hwnd, DWORD state);