#pragma once

#include "stdafx.h"
#include "time.h"
#include "Winuser.h"
#include "DataManager.h"
#include "ParticleTrack.h"
#include "Particle.h"
#include "Vector.h"
#include "Subtract.h"
#include "BMP.h"
#include "Zoom.h"
#include "Histogram.h"
#include "Stretch.h"
#include "WindowData.h"
#include "Scale.h"
#include "StringFxns.h"
#include "WndLink.h"
#include "Object.h"

//#define MAX_TRACK 300
#define MAX_ZOOM	12
#define MAX_PARAM	256
#define MAX_POLY	256

#define OBJECTSET_SIZE 500

#define TOOL_CURSOR			0
#define TOOL_HAND			1
#define TOOL_CROSS			2
#define TOOL_TRACKPOINT		3
#define TOOL_MEASUREDIST	4
#define TOOL_ROIPOLY		5
#define TOOL_CROP			6

#define ZOOM_IN		1
#define ZOOM_OUT	2

#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4A
#define VK_K	0x4B
#define VK_L	0x4C
#define VK_M	0x4D
#define VK_N	0x4E
#define VK_O	0x4F
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5A

//Temporarily stores update parameters for DataView windows
//Incase they are being blocked by the Algorithm Worker Thread
typedef struct UPDATE_{

	INT upFrame;
	INT upQuit;
	INT upZoom;
	INT upZoomDir;
	INT upAlgroithm;
	STRINGSET ssUpdateAlgorithm;

}UPDATE;

typedef struct DATAVIEW_{
	
	HWND hwnd;
	HWND hView;
	HWND hDataManager;
	HWND hBorder;
	HWND hDataBar;
	HWND hOwner;
	HWND hMDI;
	HWND hSend;
	HWND hPeripheral;

	RECT rcFrame;
	RECT rcView;
	RECT rcPreview;
	RECT rcComboBox;
	RECT rcStatusBar;
	RECT rcToolBar;
	RECT rcTabControl;
	RECT rcButtonHide;
	RECT rcDataBar;

	INT iFrame;
	INT fStack;
	INT wStack;
	INT hStack;

	INT wSource;
	INT hSource;

	INT fStart;
	INT fStop;

	PDMFRAME pdmf;
	PDMFRAME pdmfRendering;
	PIXEL *pixBmp;
	PIXEL *pixSrc;
	HDC hdcMem;
	HDC hdcDisp;
	HBITMAP hbmp;

	POINT ptGrab;
	POINT ptCursor;
	INT hPos;
	INT vPos;

	INT i;
	UINT wStackPreview;
	UINT hStackPreview;
	INT zoom;
	INT zoomBuffer;
	FLOAT scale;
	FLOAT scalePrev;

	DWORD flagBlock;
	BOOL flagBlockObject;
	BOOL flagQuit;
	BOOL flagLoop;
	BOOL flagActive;
	BOOL flagInit;
	BOOL flagObjectSet;
	BOOL flgFilter;

	UPDATE update;

	//Algorithm Params
	FLOAT paramList[MAX_PARAM];
	STRINGSET strProc;
	STRINGSET strProcBuffer;
	DWORD selProc;
	DWORD selProcBuffer;
	

	DWORD tool;

	//Temporary
	BOOL isParticleTrack;
	BOOL isMeasureDistance;
	DWORD sOpen;
	DWORD nParticles;

	FLOAT xFOV;
	FLOAT yFOV;
	FLOAT zFOV;

	DWORD idAlgorithm;
	DWORD idDataManager;

	CHAR strArchivePath[MAX_STRING];
	CHAR strAlgorithm[MAX_STRING];

	DWORD nPt;
	FPOINT pt[MAX_TRACK];
	DWORD ptFrame[MAX_TRACK];

	DWORD nPtMeasure;
	FPOINT ptMeasure[2];
	
	DWORD flgCropStart;
	DWORD flgCropEnd;
	FPOINT ptCropStart;
	FPOINT ptCropEnd;

	FILEPATHSET	fps;

	DWORD nShown;

	DWORD idTimer;

	clock_t tStart;
	clock_t tLast;
	
	TIMESTAMP tsData;
	TIMESTAMP tsDataPrev;

	WNDLINKSET wls;
	
	//ROIPOLY Variables
	POINTSET ptSet;
	
	DWORD idMask;

	OBJECTSET os;
	//OBJECTSET osDisplay;
	OBJECTSET osBuffer;
	OBJECTSET osDataView;

	FLOAT fAngleStep;
	FLOAT fAngle;

}DATAVIEW;

typedef struct DATASET_{

	UINT idType;
	UINT count;
	UINT sizeType;

	VOID *data;

}DATASET;

typedef struct HDATA_{

	UINT type;
	UINT count;
	UINT size;

	VOID *data;

}HDATA;

//DataView Creation
	HWND CreateDataView(HWND hOwner, CHAR *strTitle);
	HWND CreateMDIDataView(HWND hMDI, CHAR *strTitle);
	HWND CreateMDIDataView(HWND hMDI, LONG w, LONG h, CHAR *strTitle);
	HWND CreateDataView(HWND hOwner,  INT w, INT h, CHAR *strTitle);
//DataView Initialize
	DWORD DataView_SetWidthHeight(HWND hDataView, UINT w, UINT h);
	DWORD DataView_GetWidthHeight(HWND hDataView, UINT *w, UINT *h);
	DWORD DataView_SetStartFrame(HWND hDataView, UINT frameStart);
	DWORD DataView_SetEndFrame(HWND hDataView, UINT frameEnd);
	DWORD DataView_ShowFrame(HWND hDataView, UINT frame);
	LRESULT DataView_ShowFrame(HWND hDataView, PDMFRAME *pdmf);
	DWORD DataView_SetDimensions(HWND hDataView, LONG w, LONG h);
	DWORD DataView_GetDimensions(HWND hDataViwe, LONG *w, LONG *h);
	LRESULT DataView_SizeFrame(HWND hwnd, DATAVIEW *lpDataView);
	LRESULT DataView_SizeClient(HWND hwnd, UINT width, UINT height);
	POINT DataView_GetCursorPixelCoordinate(DATAVIEW *lpDataView);
	LRESULT DataView_UpdateCursorPixelPosition(DATAVIEW *lpDataView, FLOAT xNorm, FLOAT yNorm);
	LRESULT DataView_Draw(HWND hDataView);
	LRESULT DataView_Paint(HWND hDataView);
	LRESULT DataView_GetPDMFrame(HWND hDataView, PDMFRAME *pdm);
	LRESULT DataView_GetPDMPTR(HWND hDataView);
	LRESULT DataView_GetPDMPTR(HWND hDataView, DWORD source);
	LRESULT DataView_SnapFrame(HWND hDataView);
	

//DataView Internal Functions

LRESULT UpdateFromBlock(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK DataViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DataTabProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD InitializeTabControl(HWND hTabControl);
LRESULT AlgorithmProcDataView(DATAVIEW *lpDataView, UCHAR *in, UCHAR *out);
LRESULT ZoomDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);

DWORD WINAPI DataViewDrawProc(LPVOID param);

DWORD OnDelayClosingDataView(HWND hwnd, DWORD msDelay);
DWORD WINAPI ClosingDataViewThread(LPVOID param);



HCURSOR LoadCursorDataView(DATAVIEW *lpDataView);
DWORD ParticleTrack(HWND hOwner, DATAVIEW *dataView, WPARAM wParam, LPARAM lParam);
DWORD ParticleTrackGradient(HWND hOwner, DATAVIEW *dataView, WPARAM wParam, LPARAM lParam);
DWORD CalculateParticleVelocity(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
DWORD CalculateDistance(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnDestroyDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnAddFileListDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnKeyDownDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSaveAsDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnPaintDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetCursorDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetToolDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCommandDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMouseLeaveDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnDrawDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnShowFrameDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMDIActivateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMouseWheelDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnMouseMoveDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCreateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetFrameSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetFrameFocusDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnTimerDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCloseDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnUpdateViewPosDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnInitAlgorithm(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCreateLinkDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetViewPosDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetHwndDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wparam, LPARAM lParam);
LRESULT OnZoomDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnLButtonUpDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnLButtonDownDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetObjectDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnGetFrameSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnGetPDMDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnDataDropDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnGetPDMPTRDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSnapFrameDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam);


//ToDo:
//HWND hSend;
//WM_GETMASK
//WM_SETMASK
