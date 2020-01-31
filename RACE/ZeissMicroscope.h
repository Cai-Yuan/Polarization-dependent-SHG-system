#pragma once

#include "stdafx.h"
#include "AutoFocusProc.h"

//Property Sheet Defintions
#define PAGE_EXPERIMENT		0
#define PAGE_AUTOFOCUS		1
#define PAGE_MOSAIC			2
#define PAGE_MULTICHANNEL	3
#define PAGE_OBJECTIVE		4
#define PAGE_ZSTACK			5

//Channel Definitions
#define CHNL_BRIGHTFIELD	0
#define CHNL_CY5			1
#define CHNL_DAPI			2
#define CHNL_FITC			3
#define CHNL_RHODAMINE		4

//Objective Definitions
#define OBJECTIVE_10X		0
#define OBJECTIVE_40XOIL	1
#define OBJECTIVE_63XOIL	2
#define OBJECTIVE_4X		3
#define OBJECTIVE_20X		4
#define OBJECTIVE_40X		5

//LightPath Definitions
#define LP_EYEPIECE			0
#define LP_CAMERA			1

//Z-Stack Definitions
#define ZS_TOPDOWN			0
#define ZS_MIDDLE			1
#define ZS_BOTTOMUP			2

//Z-Axis Commands
#define ZEISS_STAGE_GETVER		"HPTv"
#define ZEISS_STAGE_GETZPOS		"FPNr"
#define COMMAND_ZAXIS_UP		"FPZS+"
#define COMMAND_ZAXIS_DOWN		"FPZS-"
#define COMMAND_ZAXIS_GETPOS	"FPNr"
#define COMMAND_ZAXIS_PRIMEMOVE	"FPZG000B89"

#define COARSEFOCUS		75
#define FINEFOCUS		20

//ZSM - Zeiss Stage Message
#define	ZSM_XSTEP_POS	32768
#define ZSM_XSTEP_NEG	32769
#define ZSM_YSTEP_POS	32770
#define ZSM_YSTEP_NEG	32771
#define ZSM_ZSTEP_POS	32772
#define ZSM_ZSTEP_NEG	32773
#define ZSM_BEGINBOOT	32774


#define ZSM_ZUP			32775
#define ZSM_ZDOWN		32776
#define ZSM_ZSTOP		32777

#define ZSM_SETAXIS			32778
#define ZSM_AUTOFOCUS		32779
#define ZSM_SETPOS			32780
#define ZSM_GETPOS			32781
#define ZSM_ACQUISITION		32782
#define ZSM_SETSPEED		32783
#define ZSM_EXIT			32800
#define ZSM_SETCHANNEL		32801
#define ZSM_SETOBJECTIVE	32802
#define ZSM_SETLIGHTPATH	32804
#define ZSM_SETCHANNELMASK	32805

//ZFM - Zeiss Focus Message
#define ZFM_SETNSTEP		32801
#define ZFM_SETDISTANCE		32802
#define ZFM_SETAUTOFOCUS	32803

#define ZSM_SETFILEPATH		32806
#define ZSM_STARTEXPERIMENT	32807
#define ZSM_STOPEXPERIMENT	32808
#define ZSM_SETFIELDOFVIEW	32809
#define ZSM_SETZSTACK		32810
#define ZSM_SETEXPERIMENT	32811


#define ZAXIS_VELOCITY	100
#define ZAXIS_DELAY		101

//Stage Calibration
#define STEPPERUM		5
#define TICKPERUM_20x	5
#define TICKPERUM_40x	5

#define SP_MICRON		1
#define SP_TICK

//Units in um

#define PIX_PER_UM_X 1.5954
#define PIX_PER_UM_Y 1.6

//20x
//#define PIX_PER_UM_X 3.191
//#define PIX_PER_UM_Y 3.152

//40x
//#define PIX_PER_UM_X 6.4558
//#define PIX_PER_UM_Y 6.5853

#define FOV_X_20x	435
#define FOV_Y_20x	326

#define FOV_X_40x	217
#define FOV_Y_40x	170

#define FOV_X_10x	870
#define FOV_Y_10x	670

#define FOV_X_63x	0
#define FOV_Y_63x	0

#define LIGHTPATH_EYEPIECE	1
#define LIGHTPATH_LSIDEPORT	2
#define LIGHTPATH_RSIDEPORT	3



//Multichannel Filters
#define MC_DAPI			1
#define MC_FITC			2
#define MC_RHODAMINE	4
#define MC_CY5			8
#define MC_TRANSLIGHT	16

typedef struct ZEISSPOSITION_{
	INT x;
	INT y;
	INT z;
}ZEISSPOSITION;

typedef struct MULTIFOV_{

	INT nRow;
	INT nCol;
	FLOAT overlap;

}MULTIFOV;

typedef struct FIELDOFVIEW_{
	INT nRow;
	INT nCol;
	FLOAT overlap;
}FIELDOFVIEW;

//flag	-- ZS_TOPDOWN
//		-- ZS_BOTTOMUP
//		-- ZS_MIDDLE
typedef struct ZSTACK_{
	INT nSlice;
	INT distance;
	DWORD flag;
}ZSTACK;

//typedef struct AUTOFOCUS_{
//
//	INT nStep;
//	INT stepDist;
//	DWORD filter;	//Focal Measure Filter
//
//}AUTOFOCUS;

typedef struct MULTICHANNEL_{

	DWORD filter;
	DWORD expDAPI;
	DWORD expFITC;
	DWORD expRhodamine;
	DWORD expCY5;
	DWORD expBrightField;

}MULTICHANNEL;

typedef struct ZEISSEXPERIMENT_{

	DWORD flgRun;

	DWORD flgAutoFocus;
	DWORD flgMosaic;
	DWORD flgMultiChannel;
	DWORD flgZStack;
	DWORD flgTimeLapse;

}ZEISSEXPERIMENT;

typedef struct ZEISSMICROSCOPE_{

	HWND hwnd;
	HWND hOwner;
	HWND hStageControl;
	
	HWND hPropertySheet;
	HWND hAutoFoucs;
	HWND hMosaic;
	HWND hMultiChannel;
	HWND hObjective;
	HWND hZStack;
	HWND hExperiment;

	HWND hCameraProperties;

	HANDLE hMicroscope;
	HANDLE hStage;

	INT zAxisVelocity;
	INT zAxisDelay;
	INT zAxisPos;

	FLOAT varPrev;
	FLOAT varCur;

	BOOL flgGetFrame;
	BOOL flgCalcFocus;
	BOOL flgAutoFocus;
	BOOL flgFocusInit;
	BOOL flgDataIn;
	BOOL flgExit;
	DWORD dirFocus;

	DWORD iImg;

	//Temporary
	UCHAR *img;

	UINT w;
	UINT h;

	INT fNumStep;
	INT fStep;

	AUTOFOCUS af;
	MULTICHANNEL mc;
	MULTIFOV mfov;
	ZSTACK zs;
	FIELDOFVIEW fov;
	ZEISSEXPERIMENT ze;

	CHAR strRootPath[MAX_STRING];

}ZEISSMICROSCOPE;

//typedef struct AUTOFOCUS_{
//	FLOAT varCur;
//	FLOAT varPrev;
//
//	DWORD dirFocus;
//	DWORD iImg;
//
//	UCHAR *img1;
//	UCHAR *img2;
//	UCHAR *img3;
//
//
//}AUTOFOCUS;

DWORD WINAPI InitZeissMicroscopeThread(LPVOID param);

//Testing Functions
DWORD ZeissZAxisTest(HWND hOwner);
HANDLE ZAxisConnect(HWND hOwner, UINT port);
HANDLE ZAxisComConfigure(HWND hOwner, HANDLE hCom);
DWORD ZAxisSendCommand(HANDLE hDevice, CHAR *strMessage, CHAR *strRecieved);
DWORD ConcatCommands(CHAR *str1, CHAR *str2, CHAR *strConcat);


//Microscope Functions
INT GetFocalPos(HANDLE hCom);
INT SetFocalPos(HANDLE hCom, INT pos);
INT SetFocalPos(HANDLE hCom, INT pos, INT flag);
HANDLE GetZeissMicroscope(HWND hOwner, UINT port);
HANDLE GetZeissMicroscopeStage(HWND hOwner, UINT port);
DWORD ZeissMicroscopeBoot(HWND hOwner, HANDLE hMicroscope);
DWORD SendComCommand(HANDLE hDevice, CHAR *strMessage, CHAR *strRecieved);

//Microscope Objective Functions
INT GetObjective(HANDLE hCom);
INT SetObjective(HANDLE hCom, INT objective);

//Microscope Reflector Functions
INT GetReflector(HANDLE hCom);
INT SetReflector(HANDLE hCom, INT reflector);

//Microscope LightPath Functions
INT SetLightPath(HANDLE hCom, INT lightPath);

//Microscope Halogen Lamp Functions
INT GetLampVoltage(HANDLE hCom);
INT SetLampVoltage(HANDLE hCom, INT lampVoltage);


//AutoFocus Functions
DWORD WaitForFocalPos(HANDLE hCom, INT focalPos);
DWORD AutoFocus(ZEISSMICROSCOPE *lpZeissScope);
DWORD GetFocalImage(ZEISSMICROSCOPE *lpZeissScope, DWORD w, DWORD h, FLOAT *fimg);
DWORD GetStreamImage(ZEISSMICROSCOPE *lpZeissScope, DWORD w, DWORD h, UCHAR *img);

DWORD GetStreamImage(ZEISSMICROSCOPE *lpZeissScope, DWORD w, DWORD h, FLOAT *img);
FLOAT GetFocalMeasure(UINT w, UINT h, FLOAT *img);
FLOAT GetFocalMeasure(UINT w, UINT h, FLOAT *img, DWORD filter);
DWORD AdjustFocus(HWND hwnd, ZEISSMICROSCOPE *lpZeissScope);
DWORD WINAPI AutoFocusThread(LPVOID param);
DWORD InitializeAutoFocusDir(ZEISSMICROSCOPE *lpZeissScope);
DWORD UpdateVariance(ZEISSMICROSCOPE *lpZeissScope);

//Multi-FOV Functions
DWORD WINAPI MultiFOVCapture(LPVOID param);
DWORD StitchFOV(UINT w, UINT h, UINT nRow, UINT nCol, UINT xStep, UINT yStep, UCHAR **imgMap);
DWORD DirectCompareMosaic(UINT w, UINT h, UINT *wMosaic, UINT *hMosaic, UINT nRow, UINT nCol, FLOAT overlap, FLOAT **imgSet, UCHAR **mosaic);

DWORD CreateZeissMicroscopeProc(HWND hOwner);
DWORD WINAPI ZeissMicroscopeThread(VOID *lpvoid);
LRESULT CALLBACK ZeissMicroscopeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//MAC5000 XYStage Functions

INT GetXAxisPos(HANDLE hCom);
INT GetYAxisPos(HANDLE hCom);
INT GetZAxisPos(HANDLE hCom);

INT SetXAxisPos(HANDLE hCom, INT pos);
INT SetYAxisPos(HANDLE hCom, INT pos);
INT SetZAxisPos(HANDLE hCom, INT pos);

INT SetXAxisPos(HANDLE hCom, INT pos, DWORD flag);
INT SetYAxisPos(HANDLE hCom, INT pos, DWORD flag);
INT SetZAxisPos(HANDLE hCom, INT pos, DWORD flag);

INT GetXAxisPos(HANDLE hCom, DWORD flag);
INT GetYAxisPos(HANDLE hCom, DWORD flag);
INT GetZAxisPos(HANDLE hCom, DWORD flag);

DWORD WaitForPosition(HANDLE hCom, INT x, INT y);
DWORD WaitForPosition(HANDLE hCom, INT x, INT y, DWORD flag);

INT SetAxisSpeed(HANDLE hCom, INT speed, DWORD flag);

//Automation Functions
DWORD HandleExitFlag(ZEISSMICROSCOPE *lpZeissScope);

//Experiment Thread
DWORD WINAPI ZeissExperimentThread(LPVOID param);
DWORD UpdateFieldOfView(HANDLE hStage, FIELDOFVIEW fov, ZEISSPOSITION *zp, INT *row, INT *col);
DWORD UpdateZStackPosition(HANDLE hObjective, ZSTACK zs, ZEISSPOSITION zpBegin, INT *z);

//Light Channel Functions
DWORD ZeissChannelFromMask(DWORD mask);
DWORD ZeissChannelStringFromMask(DWORD mask, CHAR *str);

DWORD ZeissChannelExposureFromMask(ZEISSMICROSCOPE *lpZeissScope, DWORD mask);