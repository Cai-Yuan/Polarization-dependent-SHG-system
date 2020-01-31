#pragma once

#include "stdafx.h"

#define ZEISS_HSM	100
#define ZEISS_MRM	101
#define ZEISS_MRC	102

typedef struct ZEISSFRAMEINFO_{
	INT val1;
	INT val2;
	INT width;
	INT height;
	INT val3;
	INT val4;
}ZEISSFRAMEINFO;

typedef struct ZEISSCAMINFO_{
	//short int val1;
	//short int val2;
	//short int val3;
	//short int val4;

	short int val1;
	int val2;
	int val3;
	int val4;
}ZEISSCAMINFO;

typedef int (__cdecl *Int_PROC_Void)(void);
typedef signed int (__cdecl *SgnInt_PROC_Void)(void);

typedef signed int (__stdcall *PROC_Void)(void);
typedef signed int (__stdcall *PROC_Int_Ptr)(int *);
typedef signed int (__stdcall *PROC_Int_Val)(int);

typedef signed int (__stdcall *PROC_Int2_Ptr)(int *, int *);
typedef signed int (__stdcall *PROC_Int2_Val)(int, int);
typedef signed int (__stdcall *PROC_IntPtr_IntVal)(int *, int);
typedef signed int (__stdcall *PROC_IntVal_IntPtr)(int, int *);

typedef signed int (__stdcall *SgnInt_PROC_Acquisition)(int*, int, int);

typedef signed int (__stdcall *PROC_ZFD)(ZEISSFRAMEINFO *);

typedef signed int (__stdcall *PROC_Acquisition)(int, int , ZEISSFRAMEINFO *);

typedef signed int (__cdecl  *Sgn_PROC_Void)(void);

typedef HGLOBAL (__cdecl *Gbl_PROC_Void)(void);

typedef int (__thiscall *PROC_Int_ThisPtr)(void *);

typedef signed int (__stdcall *SgnInt_PROC_SgnInt)(signed int);

typedef signed int (__stdcall *SgnInt_PROC_NextAcquisition)(signed int, int*, int , unsigned int);



typedef struct ZEISSPROC_{

	PROC_Void InitCamera;
	PROC_Void CloseCamera;	

	Int_PROC_Void GetNumCamera;
	Int_PROC_Void GetCurrentBitsPerPixel;
	Int_PROC_Void CamIsAvailable;

	PROC_Int_Val SetExposure;

	Sgn_PROC_Void GetResolution;

	PROC_ZFD GetFrameDimensions;
	PROC_Int_Ptr GetCameraInfo;

	PROC_Int_Ptr GetCurrentImageSize;
	PROC_Int_Ptr GetCurrentFrameSize;

	SgnInt_PROC_Acquisition GetFrame;
	Int_PROC_Void StartFastAcquisition;

	PROC_IntVal_IntPtr GetCurrentExposure;

	SgnInt_PROC_SgnInt InitializeAcquisition;

	SgnInt_PROC_NextAcquisition NextFrame;
	//SgnInt_PROC_Acquisition Is

	//Gbl_PROC_Void CloseHSm;


}ZEISSPROC;

//DWORD InitializeZeissCamera(zeissHSm *zp);
//HMODULE LoadZeissLibrary(VOID);
HMODULE LoadZeissLibrary(HWND hwnd);
DWORD ImportZeissFunctions(HMODULE dll, ZEISSPROC *zp);

DWORD WINAPI ZeissHSmCapture(LPVOID param);
DWORD WINAPI ZeissMRmCapture(LPVOID param);

DWORD InitCamera(ZEISSPROC *zp);
DWORD CloseCamera(ZEISSPROC *zp);
DWORD GetNumCamera(ZEISSPROC *zp);
DWORD GetExposure(ZEISSPROC *zp);
DWORD SetExposure(ZEISSPROC *zp, INT exposure);
DWORD GetCurrentImageSize(ZEISSPROC *zp, INT *width, INT *height);

DWORD InitializeZeissCamera(HWND hwnd, DWORD flgCam, ZEISSPROC *zp);
HMODULE LoadZeissLibrary(HWND hwnd, DWORD flgCam);