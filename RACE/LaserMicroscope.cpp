
#include "stdafx.h"
#include "LaserMicroscope.h"
//#include "AutoFocusDlg.h"
#include "StageProc.h"
#include "SystemProc.h"
#include "DeviceProc.h"
#include "DisplayProc.h"
#include "TrackerProc.h"
#include "AutoFocusProc.h"

#include "CameraProperties.h"
#include "UNO32.h"



HWND CreateLaserMicroscopePropertySheet(HWND hOwner)
{
	//NOTE:
	//PROPSHEETPAGE.lParam is sent to each tab during WM_INITDIALOG
	HWND hFrame = hOwner;
	HWND hMicroscope = NULL;
	HWND hSheet = NULL;
	HWND hPage = NULL;
	HWND hMDI = NULL;
	
	//CreateMessage Window
	hMicroscope = CreateWindowEx(WS_EX_WINDOWEDGE, 
										"LaserMicroscope", 
										NULL, 
										WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, 
										CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
										HWND_MESSAGE, 
										NULL, 
										GetModuleHandle(0), 
										NULL);

	SendMessage(hOwner, WM_REGISTERWINDOW, (WPARAM) hMicroscope, (LPARAM) "LaserMicroscope");
	SendMessage(hOwner, WM_SETHWND, (WPARAM) HWND_LASERMICROSCOPE, (LPARAM)(HWND)hMicroscope);
	SendMessage(hMicroscope, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM)hOwner);

	HPROPSHEETPAGE hpsp = NULL;
	PROPSHEETPAGE psp; //Defines a page in a property sheet.
	PROPSHEETHEADER psh;//Defines the frame and pages of a property sheet.
	memset(&psp, NULL, sizeof(PROPSHEETPAGE));
	memset(&psh, NULL, sizeof(PROPSHEETHEADER));

	//Initialize PROPSHEET-HEADER
	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE
				| PSH_MODELESS
				| PSH_NOAPPLYNOW
				| PSH_USECALLBACK
				| PSH_DEFAULT
				;
	psh.hwndParent = hOwner;
	psh.hInstance = GetModuleHandle(NULL);
	psh.pszCaption = "Laser Microscope";
	psh.nPages = 1;
	//psh.ppsp = &psp;
	psh.nStartPage = 0;
	psh.pfnCallback = PropSheetProc;

	//Initialize PROPSHEET-PAGE
	psp.dwSize = sizeof(PROPSHEETPAGE);
	psp.dwFlags = PSP_DEFAULT
						| PSP_PREMATURE
						| PSP_USETITLE
						;
	psp.hInstance = GetModuleHandle(NULL);
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_SYSTEM);
	psp.pszTitle = "System";
	psp.pfnDlgProc = SystemProc;
	psp.lParam = (LPARAM) hMicroscope;

	psh.ppsp = &psp;
	
	hSheet = (HWND) PropertySheet(&psh);
	
	hMDI = (HWND) SendMessage(hOwner, WM_GETHWND, (WPARAM)HWND_MDI, NULL);
	if(hMDI){
		SetDialogParent(hSheet, hMDI);
	}
	SendMessage(hMicroscope, WM_SETHWND, (WPARAM)HWND_PROPERTYSHEET, (LPARAM) hSheet);

	psp.lParam = (LPARAM) hFrame;
	psp.pszTitle = "Camera";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_CAMERAPROPERTIES);
	psp.pfnDlgProc = CameraPropertiesProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);
	psp.lParam = (LPARAM) hMicroscope;

	psp.pszTitle = "Stage";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_STAGE);
	psp.pfnDlgProc = StageProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);	

	psp.pszTitle = "AutoFocus";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_AUTOFOCUS);
	psp.pfnDlgProc = AutoFocusProc2;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "Device";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_DEVICE);
	psp.pfnDlgProc = DeviceProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "Display";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_DISPLAY);
	psp.pfnDlgProc = DisplayProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "Tracker";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_TRACKER);
	psp.pfnDlgProc = TrackerProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	//TOADD:
	//psp.pszTitle = "Holography";
	//psp.pszTemplate = MAKEINTRESOURCE(IDD_LM_HOLO);
	//psp.pfnDlgProc = HolographyProc;
	//hpsp = CreatePropertySheetPage(&psp);
	//SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	PropSheet_RecalcPageSizes(hSheet);

	SendMessage(hMicroscope, WM_INITMICROSCOPE, NULL, NULL);

	//Reposition Dialog Box
	DWORD wClient = GetClientWidth(hOwner);
	DWORD hClient = GetClientHeight(hOwner);
	DWORD wWindow = GetWindowWidth(hSheet);
	DWORD hWindow = GetWindowHeight(hSheet);
	RECT rectMove;
	rectMove.left = wClient-wWindow;
	rectMove.right = wClient;
	rectMove.top = 0;
	rectMove.bottom = hWindow;
	SetWindowPos(hSheet, HWND_TOP, wClient-wWindow, 0, wWindow, hWindow, SWP_SHOWWINDOW);

	return hMicroscope;


}

DWORD GetNavigationVector(DOUBLE_3DPOINT ptCurr, DOUBLE_3DPOINT ptDest, FPOINT center, VECTOR *vector)
{

	VECTOR v;
	FPOINT ptCur;
	FPOINT ptX;
	FPOINT ptTail;
	FPOINT ptHead;
	FPOINT ptShift;

	ptTail.x = ptCurr.x;
	ptTail.y = ptCurr.y;
	ptHead.x = ptDest.x;
	ptHead.y = ptDest.y;

	GetVector(ptHead, ptTail, &v);
	ptTail.x = 0;
	ptTail.y = 0;
	GetVector(ptTail, v.dir, 160, &v);
	ptShift.x = center.x;
	ptShift.y = center.y;
	ShiftVector(ptShift, &v);
	ptTail.x = v.pHead.x;
	ptTail.y = v.pHead.y;
	GetVector(ptTail, v.dir, 40, vector);

	return TRUE;
}

DWORD GetVelocityVector(LM_DISPLAY *lpDisplay, FLOAT velX, FLOAT velY, VECTOR *vector, TEXTR *text)
{
	if(!lpDisplay)
		return FALSE;
	if(!vector)
		return FALSE;
	if(!text)
		return FALSE;
	if(!velX && !velY)
		return FALSE;

	VECTOR v = {0};
	FPOINT ptTail = {0};
	FPOINT ptHead;
	ptHead.x = velX;
	ptHead.y = velY;

	FLOAT tailPos = lpDisplay->roiDiameter*0.5*lpDisplay->pixPerUm;

	GetVector(ptHead, ptTail, &v);
	GetVector(ptTail, v.dir, tailPos, &v);
	//GetVector(ptTail, v.dir, 25, &v);

	ptTail.x = v.pHead.x;
	ptTail.y = v.pHead.y;

	GetVector(ptTail, v.dir, 30, vector);
	GetVector(ptTail, v.dir, 45, &v);

	FLOAT magnitude = sqrt(velX*velX+velY*velY);

	sprintf(text->str, "%.2f", magnitude);
	text->pt.x = v.pHead.x;
	text->pt.y = v.pHead.y;

	return TRUE;
}

DWORD GetZAxisIndicator(LM_STAGE *lpStage, CIRCLE *circle, TEXTR *text)
{
	if(!lpStage)
		return FALSE;

	LASERMICROSCOPE *lpMicroscope = (LASERMICROSCOPE *) GetWindowUserData(lpStage->hOwner);
	LM_DISPLAY *lpDisplay = lpMicroscope->lpDisplay;

	if(!lpMicroscope || !lpDisplay)
		return FALSE;

	FLOAT zVel = lpStage->zVel;
	FLOAT zDistRemain = GetZDistance(lpStage->dptMoveTo, lpStage->dptCurrent);
	FLOAT zDistTotal = GetZDistance(lpStage->dptMoveTo, lpStage->dptPrev);
	FLOAT zDir = Sign(zDistRemain);

	FLOAT radius = lpDisplay->roiDiameter*lpDisplay->pixPerUm*0.5;

	if(lpStage->flgMoveToZ || lpStage->flgMoveTo){

		zVel = lpStage->zVelMax*zDir*(zDistRemain/zDistTotal);
		circle->x = lpMicroscope->lpDisplay->wDisplay/2;
		circle->y = lpMicroscope->lpDisplay->hDisplay/2;
		circle->r = ((FLOAT)radius*((FLOAT)zVel/(FLOAT)lpStage->zVelMax))-radius;
	}else{
		circle->x = lpMicroscope->lpDisplay->wDisplay/2;
		circle->y = lpMicroscope->lpDisplay->hDisplay/2;	
		circle->r = radius - ((FLOAT)radius*((FLOAT)zVel/(FLOAT)lpStage->zVelMax));
	}

	if(!circle->r){
		circle->r = 2;
	}

	return TRUE;
}