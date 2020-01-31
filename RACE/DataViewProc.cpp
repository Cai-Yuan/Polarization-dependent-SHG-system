#include "stdafx.h"
#include "time.h"

#include "DataView.h"
#include "PDM.h"
#include "String.h"
#include "ImageConversion.h"
#include "DataPreview.h"
#include "Subclass.h"
#include "LogOperator.h"
#include "DerivativeOperator.h"
#include "Draw.h"
#include "Vector.h"
#include "Suppression.h"
#include "ImageSubtract.h"
#include "DataBar.h"
#include "AlgorithmFxns.h"
#include "GaussianBlurr.h"
#include "StringFxns.h"
#include "Boolean.h"
#include "PavlidisContour.h"
#include "Invert.h"
#include "AlgorithmThread.h"
#include "Chamfer.h"
#include "Power.h"
#include "Statistics.h"
#include "Addition.h"
#include "ImageScale.h"
#include "MorphOperators.h"
#include "WndLink.h"
#include "Polygon.h"
#include "FloodFill.h"
#include "Object.h"
#include "Gdiplus.h"
#include "Threshold.h"
#include "ImageMath.h"
#include "Contrast.h"
#include "MedianFilter.h"

#define DATAVIEW2		TRUE

#define SCALE 0.25;

//#define DATAVIEWFILTER	FALSE

DWORD SetToolDataView(HWND hwnd, DWORD tool);
DWORD GetSystemTimeTest(HWND hwnd, DWORD nIterations, DWORD nSubIterations);
DWORD InitializeTabControl(HWND hTabControl);
DWORD SetZoomDataView(DATAVIEW *lpDataView, HWND hwnd, FLOAT scale);

#ifdef DATAVIEW2

LRESULT CALLBACK DataViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATAVIEW *lpDataView = (DATAVIEW *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_SETOBJECT:
		return OnSetObjectDataView(lpDataView, hwnd, wParam, lParam);
	case WM_GETMASK:
		{
			lpDataView->hSend = (HWND)wParam;
			lpDataView->idMask = (DWORD)lParam;
			SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(DVN_SETTOOL, TOOL_ROIPOLY), (LPARAM)lpDataView->hSend);
			return TRUE;
		}
	case WM_CREATELINK:
		{
			return OnCreateLink(&lpDataView->wls, hwnd, wParam, lParam);
		}
	case WM_SETTEXT:
		{
			//LPARAM - A Pointer to a Null-Terminated string that is the window text
			SendMessage(lpDataView->hOwner, WM_REGISTERWINDOW, (WPARAM) hwnd, lParam);
			break;
		}
	case WM_ERASEBKGND:
		if(lParam)
			break;
		else
			return TRUE;
	case DF_INITFILTER:
		{
			RelayMessage(&lpDataView->wls, NULL, "ParticleTracker", msg, wParam, lParam);
			return OnInitAlgorithm(lpDataView, hwnd, wParam, lParam);
		}
	case WM_SHOWFRAME:
		{
			return OnShowFrameDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_DATADROP:
		{
			return OnDataDropDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_DRAW:
		{
			return OnDrawDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			OnPaintDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_ADDFILELIST:
		{
			OnAddFileListDataView(lpDataView, hwnd, wParam, lParam);			
		}
		return TRUE;
	case WM_MDIACTIVATE:
		{
			OnMDIActivateDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_SETFRAMESIZE:
		{
			return OnSetFrameSizeDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_GETFRAMESIZE:
		{
			return OnGetFrameSizeDataView(lpDataView, hwnd, wParam, lParam);
		}
	case DVM_FRAMEFORWARD:
		{
			if(lpDataView->flagBlock)
				return FALSE;

			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
		}
		break;
	case DVM_FRAMEBACKWARD:
		{
			if(lpDataView->flagBlock)
				return FALSE;
			
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack-1, NULL);
		}
		break;
	case DVM_ZOOM:
		{		
			return OnZoomDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_SETHWND:
		{
			return OnSetHwndDataView(lpDataView, hwnd, wParam, lParam);
		}
	case DV_SETVIEWPOS:
		{
			return OnSetViewPosDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case DV_GETVIEWPOS:
		{
		}
		break;
	case DV_UPDATEVIEWPOS:
		{
			return OnUpdateViewPosDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_GETHWND:
		{
			switch(wParam)
			{
			case HWND_DATAMANAGER:
				return (LRESULT) lpDataView->hDataManager;
			case HWND_FRAME:
				return (LRESULT) SendMessage(lpDataView->hOwner, msg, wParam, lParam);
			default:
				break;
			}
		}
		break;
	case WM_SETFRAMEFOCUS:
		{
			OnSetFrameFocusDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_COMMAND:
		{
			OnCommandDataView(lpDataView, hwnd, wParam, lParam);	
		}
        break;
	case WM_MOUSEWHEEL:
		{
			OnMouseWheelDataView(lpDataView, hwnd, wParam, lParam);	
		}
		break;
	case WM_MOUSEMOVE:
		{
			OnMouseMoveDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDownDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnLButtonUpDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		{
			OnKeyDownDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_SETCURSOR:
		{
			return OnSetCursorDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_MOUSELEAVE:
		{
			return OnMouseLeaveDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_TIMER:
		{
			return OnTimerDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_GETPDM:
		{
			return OnGetPDMDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_GETPDMPTR:
		{
			return OnGetPDMPTRDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_SNAPFRAME:
		{
			return OnSnapFrameDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_SIZE:
		{
			OnSizeDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			OnCreateDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_NCCREATE:
		{
			lpDataView = (DATAVIEW *)malloc(sizeof(DATAVIEW));
			memset(lpDataView, NULL, sizeof(DATAVIEW));
			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpDataView);
			if(!lpDataView){
				MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "DataView Error!", MB_OK);
			}
		}
		break;
	case WM_CLOSE:
		{
			return OnCloseDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		{
			return OnDestroyDataView(lpDataView, hwnd, wParam, lParam);
		}
	default:
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}

	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

LRESULT OnGetPDMDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(!wParam)
		return FALSE;

	PDMFRAME *pdm = (PDMFRAME *)wParam;
	
	
	if(pdm->pdmfh.typeData == DT_UCHAR){
		if(pdm->pdmfh.width == lpDataView->wSource && pdm->pdmfh.height == lpDataView->hSource){
			pdm->pdmfh.width = lpDataView->wSource;
			pdm->pdmfh.height = lpDataView->hSource;
			pdm->pdmfh.frame = lpDataView->iFrame;
			pdm->pdmfh.sizeData = pdm->pdmfh.width*pdm->pdmfh.height*sizeof(UCHAR);
			PIXELtoUCHAR(pdm->pdmfh.width, pdm->pdmfh.height, lpDataView->pixSrc, (UCHAR *)pdm->lpvoid, NULL);
			//memcpy(pdm->lpvoid, lpDataView->pixSrc, pdm->pdmfh.width*pdm->pdmfh.height*sizeof(UCHAR));
		}
	}

	return TRUE;
}
LRESULT OnSnapFrameDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(!lpDataView)
		return FALSE;

	CHAR strWnd[MAX_STRING];
	CHAR strSrc[MAX_STRING];
	CHAR strTime[MAX_STRING];
	CHAR strPath[MAX_STRING];
	TIMESTAMP ts = {0};

	
	GetLocalTime(&ts);
	//GetSystemTime(&ts);
	GetWindowText(hwnd, strSrc, MAX_STRING);
	sprintf(strTime, "%d-%d-%d_%d-%d-%d-%d", ts.wYear, ts.wMonth, ts.wDay, ts.wHour, ts.wMinute, ts.wSecond, ts.wMilliseconds);
	sprintf(strWnd, "SnapShot: %s", strTime);
	sprintf(strPath, "C:\\Archive\\Snap\\%s.bmp", strTime);
	SaveBMP(lpDataView->wSource, lpDataView->hSource, strPath, lpDataView->pixSrc);
	HWND hDataView = CreateMDIDataView(lpDataView->hOwner, lpDataView->pdmf.pdmfh.width, lpDataView->pdmf.pdmfh.height, strWnd);
	DataView_ShowFrame(hDataView, &lpDataView->pdmf);
	RECT rSrc;
	GetWindowRect(hwnd, &rSrc);
	rSrc = ScreenToClient(lpDataView->hOwner, rSrc);
	SetWindowPos(hDataView, HWND_TOP, rSrc.right, rSrc.top, SWP_SHOWWINDOW);
	ShowWindow(hDataView, SW_SHOW);
	SetActiveWindow(lpDataView->hwnd);


	return (LRESULT) TRUE;
}
LRESULT OnGetPDMPTRDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(wParam == DT_RAW)
		return (LRESULT) &lpDataView->pdmf;
	if(wParam == DT_RENDERING)
		return (LRESULT) &lpDataView->pdmfRendering;

	return FALSE;
}

LRESULT OnGetFrameSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	INT *w = (INT *) wParam;
	INT *h = (INT *) lParam;

	*w = lpDataView->wSource;
	*h = lpDataView->hSource;


	return TRUE;
}

LRESULT OnSetObjectDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	OBJECTSET *os = (OBJECTSET *) lParam;

	if(!lpDataView)
		return FALSE;

	if(lpDataView->flagQuit)
		return FALSE;

	if(!lpDataView->flagBlockObject){
		lpDataView->flagBlockObject = TRUE;
		Copy(&lpDataView->osBuffer, *os);
		lpDataView->flagObjectSet = TRUE;
		lpDataView->flagBlockObject = FALSE;
	}

	return TRUE;
}

DWORD InitializeTabControl(HWND hTabControl)
{

	TCITEM tcitem;
	HIMAGELIST hImgList;

	memset(&tcitem, NULL, sizeof(TCITEM));
	memset(&hImgList, NULL, sizeof(HIMAGELIST));

	hImgList = ImageList_Create(18, 18, ILC_COLOR32, 3, 1);

	tcitem.mask = TCIF_TEXT|TCIF_IMAGE;


	tcitem.pszText = "Stack";
	tcitem.iImage = 0;
	TabCtrl_InsertItem(hTabControl, 1, &tcitem);

	tcitem.pszText = "Filters";
	tcitem.iImage = -1;
	TabCtrl_InsertItem(hTabControl, 1, &tcitem);

	tcitem.pszText = "Display";
	tcitem.iImage = -1;
	TabCtrl_InsertItem(hTabControl, 2, &tcitem);

	ImageList_Destroy(hImgList);

	return TRUE;

}

//Points are stored in source coordinates
DWORD CalculateParticleVelocity(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	DWORD dw = NULL;
	CHAR str[MAX_STRING*2];

	HWND hText;
	hText = (HWND) SendMessage(lpDataView->hDataManager, WM_GETHWND, (WPARAM) HWND_TEXT, NULL);

	DWORD p = lpDataView->nPt-1;
	DWORD pFrame = lpDataView->ptFrame[p];

	TIMESTAMP t1 = {0};
	TIMESTAMP t2 = {0};

	FLOAT deltaTime = NULL;
	FLOAT deltaDistance = NULL;
	FLOAT delX = NULL;
	FLOAT delY = NULL;


	PDMFRAMEHEADER pdmfh = {0};

	//Get Time Stamps
	SendMessage(lpDataView->hDataManager, WM_GETFRAMEHEADER, (WPARAM) lpDataView->ptFrame[p], (LPARAM) &pdmfh);
	memcpy(&t1, &pdmfh.timeStamp, sizeof(TIMESTAMP));
	if(p > 1){
		SendMessage(lpDataView->hDataManager, WM_GETFRAMEHEADER, (WPARAM) lpDataView->ptFrame[p-1], (LPARAM) &pdmfh);
		memcpy(&t2, &pdmfh.timeStamp, sizeof(TIMESTAMP));
	}

	FLOAT xPos = lpDataView->pt[p].x;
	FLOAT yPos = lpDataView->pt[p].y;


	if(p > 0){
		deltaTime = Subtract(t1, t2);
		delX = (lpDataView->pt[p].x - lpDataView->pt[p-1].x);
		delY = (lpDataView->pt[p].y - lpDataView->pt[p-1].y);
		////delX = (xUmPerPixel*lpDataView->pt[p].x - xUmPerPixel*lpDataView->pt[p-1].x);
		////delY = (yUmPerPixel*lpDataView->pt[p].y - yUmPerPixel*lpDataView->pt[p-1].y);
		deltaDistance = sqrt( (delX*delX) + (delY*delY) );

		sprintf(str, "%3d\t%3.2f\t%3.2f\t\t%3.3f\t%3.3f\t%3.3f", p, xPos, yPos, (deltaDistance/(deltaTime/1000)), deltaDistance, deltaTime);
		//sprintf(str, "%3d: (%3.2f, %3.2f)\t\t%3.3f\t%3.3f\t%3.3f", p, xPos, yPos, (deltaDistance/(deltaTime/1000)), deltaDistance, deltaTime);
		InsertNewLine(hText, str);

		if((deltaDistance / deltaTime) < 0.1)
			deltaDistance = deltaDistance;
	}

	return dw;
}

DWORD CalculateDistance(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	DWORD dw = NULL;

	CHAR str[MAX_STRING];

	FLOAT deltaDistance = NULL;
	FLOAT delX = NULL;
	FLOAT delY = NULL;

	FLOAT xUmPerPixel = ((FLOAT)lpDataView->xFOV / (FLOAT)lpDataView->wSource);
	FLOAT yUmPerPixel = ((FLOAT)lpDataView->yFOV / (FLOAT)lpDataView->hSource);

	if(lpDataView->nPtMeasure == 2){
		delX = xUmPerPixel*(lpDataView->ptMeasure[0].x - lpDataView->ptMeasure[1].x);
		delY = yUmPerPixel*(lpDataView->ptMeasure[0].y - lpDataView->ptMeasure[1].y);
		deltaDistance = sqrt( (delX*delX) + (delY*delY) );
		//deltaDistance = (deltaDistance / lpDataView->zoom);

		sprintf(str, "Distance: %3.2f um", deltaDistance);
		ThrowMessage(hwnd, str);
		//sprintf(str, "Distance: %3.2f pixels, 
	}

	return dw;
}

LRESULT OnAddFileListDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	UINT f;
	UINT nPaths;
	CHAR strFileName[MAX_STRING];
	FILEPATHSET *fps;
	//fps = (FILEPATHSET *) calloc(1, sizeof(FILEPATHSET));
	fps = (FILEPATHSET *) malloc(sizeof(FILEPATHSET));
	memset(fps, NULL, sizeof(FILEPATHSET));

	if(fps && wParam){
		memcpy(fps, (VOID *) wParam, sizeof(FILEPATHSET));
		nPaths = fps->nPaths;

		for(f = 0; f < nPaths; f++){
			FileNameFromPath(MAX_STRING, fps->filePath[f].lpstr, strFileName);
			SendMessage(lpDataView->hDataBar, CB_ADDSTRING, NULL, (LPARAM) strFileName);
		}
		SendMessage(lpDataView->hOwner, DB_ADDSTRINGSET, (WPARAM)fps, NULL);
		SendMessage(lpDataView->hDataBar, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);

		memcpy((VOID *)&lpDataView->fps,(VOID *)fps, sizeof(FILEPATHSET));
		free(fps);
	}

	return TRUE;
}

LRESULT OnCloseDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	if(lpDataView->flagBlock){
		lpDataView->flagQuit = TRUE;
		OnDelayClosingDataView(hwnd, NULL);
		if(wParam != WM_CLOSE){
			return TRUE;
		}
	}else{
		lpDataView->flagBlock = TRUE;
	}

	if(lpDataView->hPeripheral){
		if(!SendMessage(lpDataView->hPeripheral, WM_CLOSING, NULL, (LPARAM) hwnd)){
			return TRUE;
		}
	}

	SendMessage(lpDataView->hOwner, WM_CLOSING, NULL, (LPARAM) hwnd);
	//if(!SendMessage(lpDataView->hOwner, WM_CLOSING, NULL, (LPARAM) hwnd)){
	//	lpDataView->flagQuit = FALSE;
	//}
		
	//HCURSOR hCursor;
	//hCursor = LoadCursor(NULL, IDC_ARROW);
	//SetCursor(hCursor);

	//ThrowMessage(hwnd, "Window Destroyed: DataView");
	//SendMessage(lpDataView->hDataManager, WM_CLOSE, NULL, NULL);

	////Clear Data Tab File Entries
	//memset(&lpDataView->fps, NULL, sizeof(FILEPATHSET));
	//SendMessage(lpDataView->hOwner, DB_ADDSTRINGSET, (WPARAM)&lpDataView->fps, NULL);

	//SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) 0, NULL);

	////SetScrollRange
	////HWND hActive = (HWND) SendMessage(lpDataView->hOwner, WM_GETHWND, (WPARAM) HWND_ACTIVE, NULL);
	////if(!hActive){
	////	SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), 0);
	////	SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), 0);
	////}

	//SendMessage(lpDataView->hOwner, PBM_SETRANGE, NULL, MAKELONG(0, 1));
	//SendMessage(lpDataView->hOwner, PBM_SETPOS, NULL, NULL);

	KillTimer(hwnd, lpDataView->idTimer);

	if(lpDataView->hbmp)
	{
		if(!DeleteObject(lpDataView->hbmp)){
			ThrowMessage(hwnd, "DeleteObject() Returned NULL");
			ThrowLastError(hwnd);
		}
		//Destory Display Surface
	}
	if(lpDataView->hdcMem)
		DeleteDC(lpDataView->hdcMem);
	if(lpDataView->hdcDisp)
		DeleteDC(lpDataView->hdcDisp);

	FreePDM(&lpDataView->pdmf);
	FreePDM(&lpDataView->pdmfRendering);

	if(lpDataView){
		free(lpDataView);
		lpDataView = NULL;
	}

	SendMessage(hwnd, WM_DESTROY, NULL, NULL);

	return FALSE;
	//return TRUE;

}

DWORD OnDelayClosingDataView(HWND hwnd, DWORD msDelay)
{
		
	CreateThread(NULL, NULL, ClosingDataViewThread, (LPVOID) hwnd, NULL, NULL);


	return TRUE;
}

DWORD WINAPI ClosingDataViewThread(LPVOID param)
{
	HWND hwnd = (HWND) param;
	Sleep(500);
	SendMessage(hwnd, WM_CLOSE, (WPARAM) WM_CLOSE, NULL);
	return TRUE;
}

LRESULT OnDestroyDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	ShowWindow(hwnd, SW_HIDE);
	
	HCURSOR hCursor;
	hCursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(hCursor);

	ThrowMessage(hwnd, "Window Destroyed: DataView");
	SendMessage(lpDataView->hDataManager, WM_CLOSE, NULL, NULL);

	//Sleep(2000);

	//Clear Data Tab File Entries
	memset(&lpDataView->fps, NULL, sizeof(FILEPATHSET));
	SendMessage(lpDataView->hOwner, DB_ADDSTRINGSET, (WPARAM)&lpDataView->fps, NULL);

	SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) 0, NULL);

	SendMessage(lpDataView->hOwner, PBM_SETRANGE, NULL, MAKELONG(0, 1));
	SendMessage(lpDataView->hOwner, PBM_SETPOS, NULL, NULL);

	KillTimer(hwnd, lpDataView->idTimer);

	if(lpDataView->hbmp)
	{
		if(!DeleteObject(lpDataView->hbmp)){
			ThrowMessage(hwnd, "DeleteObject() Returned NULL");
			ThrowLastError(hwnd);
		}
		//Destory Display Surface
	}
	if(lpDataView->hdcMem)
		DeleteDC(lpDataView->hdcMem);
	if(lpDataView->hdcDisp)
		DeleteDC(lpDataView->hdcDisp);

	if(lpDataView) free(lpDataView);

	//DestroyWindow(hwnd);

	//PostQuitMessage(0);
	Sleep(50);
	return TRUE;
}

LRESULT OnLButtonDownDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	SetFocus(hwnd);
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
			
    memcpy(&lpDataView->ptGrab, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));

	if(lpDataView->tool == TOOL_CURSOR){
		lpDataView->tool = TOOL_HAND;
		SendMessage(hwnd, WM_SETCURSOR, (WPARAM) hwnd, HTCLIENT);
	}

	if(lpDataView->tool == TOOL_CROP){

		if(lpDataView->flgCropStart){
			lpDataView->ptCropEnd.x = pt.x + lpDataView->hPos;
			lpDataView->ptCropEnd.y = pt.y + lpDataView->vPos;
			lpDataView->flgCropStart = FALSE;
			return TRUE;
		}

		if(!lpDataView->flgCropStart){
			lpDataView->ptCropStart.x = pt.x + lpDataView->hPos;
			lpDataView->ptCropStart.y = pt.y + lpDataView->vPos;
			lpDataView->flgCropStart = TRUE;
			return TRUE;
		}

		//if(!lpDataView->flgCropEnd){
		//	lpDataView->ptCropEnd.x = pt.x;
		//	lpDataView->ptCropEnd.y = pt.y;
		//	lpDataView->flgCropStart = TRUE;
		//	lpDataView->flgCropEnd = TRUE;
		//}
	}

	return TRUE;
}


LRESULT OnLButtonUpDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	DWORD nPt;
	SHORT keyState;
	POINT pt;

	nPt = lpDataView->nPt;

	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	if(lpDataView->tool == TOOL_HAND){
		lpDataView->tool = TOOL_CURSOR;
		SendMessage(hwnd, WM_SETCURSOR, (WPARAM) hwnd, HTCLIENT);
	}

	if(lpDataView->tool == TOOL_TRACKPOINT){
		FLOAT xNorm;
		FLOAT yNorm;

		lpDataView->ptFrame[lpDataView->nPt] = lpDataView->fStack;
		
		xNorm = (FLOAT)(pt.x + lpDataView->hPos) / (FLOAT)(lpDataView->wStack);
		yNorm = (FLOAT)(pt.y + lpDataView->vPos) / (FLOAT)(lpDataView->hStack);

		lpDataView->pt[nPt].x = xNorm*lpDataView->wSource;
		lpDataView->pt[nPt].y = yNorm*lpDataView->hSource;

		//lpDataView->pt[nPt].x = lpDataView->pt[nPt].x - 1;
		//lpDataView->pt[nPt].y = lpDataView->pt[nPt].y - 1;
		lpDataView->nPt++;
		SendMessage(hwnd, WM_DRAW, NULL, NULL);

		CalculateParticleVelocity(lpDataView, hwnd, wParam, lParam);
	}

	if(lpDataView->tool == TOOL_MEASUREDIST){
		FLOAT xNorm;
		FLOAT yNorm;

		if(lpDataView->nPtMeasure > 1){
			lpDataView->nPtMeasure = 0;

			SendMessage(hwnd, WM_DRAW, NULL, NULL);
		}


		DWORD nPtMeasure = lpDataView->nPtMeasure;
		
		xNorm = (FLOAT)(pt.x + lpDataView->hPos) / (FLOAT)(lpDataView->wStack);
		yNorm = (FLOAT)(pt.y + lpDataView->vPos) / (FLOAT)(lpDataView->hStack);

		lpDataView->ptMeasure[nPtMeasure].x = xNorm*lpDataView->wSource;
		lpDataView->ptMeasure[nPtMeasure].y = yNorm*lpDataView->hSource;

		//lpDataView->ptMeasure[nPtMeasure].x = lpDataView->ptMeasure[nPtMeasure].x-1;
		//lpDataView->ptMeasure[nPtMeasure].y = lpDataView->ptMeasure[nPtMeasure].y-1;
		lpDataView->nPtMeasure++;
		SendMessage(hwnd, WM_DRAW, NULL, NULL);

		
		CalculateDistance(lpDataView, hwnd, wParam, lParam);

	}

	if(lpDataView->tool == TOOL_ROIPOLY){
		pt.x = ((pt.x + lpDataView->hPos)/lpDataView->scale);
		pt.y = ((pt.y + lpDataView->vPos)/lpDataView->scale);
		AppendPoint(&lpDataView->ptSet, pt); 
		SendMessage(hwnd, WM_DRAW, NULL, NULL);
		//SendMessage(hwnd, WM_PAINT, NULL, NULL);
	}


	return TRUE;

}

LRESULT OnKeyDownDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	POINT pt;
	GetCursorPos(&pt);
    switch (wParam) 
    { 
        case VK_LEFT: 
            // Process the LEFT ARROW key. 
			SetCursorPos(pt.x-1, pt.y);
            break; 
         case VK_RIGHT: 
            // Process the RIGHT ARROW key. 
			 SetCursorPos(pt.x+1, pt.y);
            break; 
        case VK_UP:   
            // Process the UP ARROW key. 
			SetCursorPos(pt.x, pt.y-1);
            break; 
        case VK_DOWN:   
            // Process the DOWN ARROW key.  
			SetCursorPos(pt.x, pt.y+1);
            break;
		case VK_RETURN:
			if(lpDataView->isParticleTrack){
				DWORD nPt;
				nPt = lpDataView->nPt;
				memcpy(&pt, &Point(pt.x, pt.y), sizeof(POINT));
				lpDataView->ptFrame[lpDataView->nPt] = lpDataView->fStack;
				
				lpDataView->pt[nPt].x = pt.x + lpDataView->hPos/* - 2*/;
				lpDataView->pt[nPt].y = pt.y + lpDataView->vPos/* - 1*/;
				lpDataView->nPt++;
				SendMessage(hwnd, WM_PAINT, NULL, NULL);

				CalculateParticleVelocity(lpDataView, hwnd, wParam, lParam);
			}
			if(lpDataView->tool == TOOL_ROIPOLY){
				HWND hText;
				CHAR str[MAX_STRING];

				PDMFRAME pdmf = {0};
				POLYGON polygon = {0};
				UINT w = lpDataView->wSource;
				UINT h = lpDataView->hSource;

				AllocPDMFrame(&pdmf, w, h, DT_UCHAR);
				SetPDMFrameUCHAR(&pdmf, 255);
				polygon.point = (POINT *)malloc(lpDataView->ptSet.nPoints*sizeof(POINT));
				polygon.vertex = (POINT *)malloc(lpDataView->ptSet.nPoints*sizeof(POINT));
				PointSetToPolygon(&lpDataView->ptSet, &polygon);
				FillPolygon(w, h, 0, &polygon, (UCHAR *)pdmf.lpvoid);
				SendMessage(lpDataView->hSend, WM_SETMASK, (WPARAM) lpDataView->idMask, (LPARAM)&pdmf);

				if(polygon.point){free(polygon.point);}
				if(pdmf.lpvoid){free(pdmf.lpvoid);}


				lpDataView->ptSet.nPoints = 0;
				
				SetToolDataView(hwnd, TOOL_CURSOR);

				SendMessage(hwnd, WM_DRAW, NULL, NULL);
				SendMessage(hwnd, WM_PAINT, NULL, NULL);
			}
			break;				
		case VK_ESCAPE:
			if(lpDataView->tool == TOOL_TRACKPOINT){
				if(MessageBox(hwnd, "Disable Tracking Tool?", "DataView", MB_OKCANCEL) == IDOK){
					HWND hText;
					CHAR str[MAX_STRING];

					
					lpDataView->nPt = 0;
					lpDataView->nParticles++;
					
					hText = (HWND) SendMessage(lpDataView->hDataManager, WM_GETHWND, (WPARAM) HWND_TEXT, NULL);

					sprintf(str, "**********Particle %4d**********", lpDataView->nParticles);
					InsertNewLine(hText, str);

					SetToolDataView(hwnd, TOOL_CURSOR);

					SendMessage(hwnd, WM_DRAW, NULL, NULL);
					
				}
			}
			if(lpDataView->tool == TOOL_ROIPOLY){
				if(MessageBox(hwnd, "Disable Region Of Interest Tool?", "DataView", MB_OKCANCEL) == IDOK){

					lpDataView->ptSet.nPoints = 0;
					
					SetToolDataView(hwnd, TOOL_CURSOR);

					SendMessage(hwnd, WM_DRAW, NULL, NULL);
					SendMessage(hwnd, WM_PAINT, NULL, NULL);
				}
			}
			else
				SetToolDataView(hwnd, TOOL_CURSOR);
			break;
		case VK_D:
			if(lpDataView->tool != TOOL_MEASUREDIST){
				SetToolDataView(hwnd, TOOL_MEASUREDIST);
			}
			else{
				SetToolDataView(hwnd, TOOL_CURSOR);
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)10, (LPARAM)NULL);
			}

			break;
		case VK_T:
			if(lpDataView->tool != TOOL_TRACKPOINT){
				SetToolDataView(hwnd, TOOL_TRACKPOINT);
			}
			else{
				SetToolDataView(hwnd, TOOL_CURSOR);
			}
			break;
		case VK_Z:
			if(GetKeyState(VK_LCONTROL)){ 
				lpDataView->nPt--;

				if(lpDataView->nPt < 0)
					lpDataView->nPt = 0;
			
				SendMessage(hwnd, WM_DRAW, NULL, NULL);
			}
			break;
		case VK_S:
			if(GetKeyState(VK_LCONTROL)&0x8000){ 
				SendMessage(hwnd, WM_COMMAND,(WPARAM) ID_FILE_SAVEAS, (LPARAM) NULL);
			}
			else{
				//lpDataView->hPos++;
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)NULL, (LPARAM)10);
			}
			break;
		case VK_W:
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)NULL, (LPARAM)-10);
			break;
		case VK_A:
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)-10, (LPARAM)NULL);
			break;
		case VK_ADD:
			//If the high-order bit is 1, the key is down
			if(GetKeyState(VK_LCONTROL)&0x8000){
				//Zoom In

				if(lpDataView->flagBlock){
					lpDataView->update.upZoom = lpDataView->zoom + 1;
					return TRUE;
				}
				
				SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_IN, NULL);
				//INT zoom = NULL;
				//lpDataView->zoom++;				
				//if(lpDataView->zoom > MAX_ZOOM)
				//	lpDataView->zoom = MAX_ZOOM;

				//zoom = lpDataView->zoom;
				//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
				//SendMessage(hwnd, WM_DRAW, NULL, NULL);
				//SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);
			}
			else{
				SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
				SendMessage(lpDataView->hDataBar, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
			}
			break;
		case VK_SUBTRACT:
			//If the high-order bit is 1, the key is down
			if(GetKeyState(VK_LCONTROL)&0x8000)
			{
				//Zoom Out
				if(lpDataView->flagBlock){
					lpDataView->update.upZoom = lpDataView->zoom - 1;
					return TRUE;
				}

				SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_OUT, NULL);

				//INT zoom = NULL;
				//lpDataView->zoom--;
				//if(lpDataView->zoom < 1)
				//	lpDataView->zoom = 1;

				//zoom = lpDataView->zoom;
				//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
				//SendMessage(hwnd, WM_DRAW, NULL, NULL);
				//SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);

			}
			else
			{
				SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack-1, NULL);
				SendMessage(lpDataView->hDataBar, WM_SETFRAMEFOCUS, lpDataView->fStack-1, NULL);
			}
			break;
		case VK_F6:
			{
				DataView_SizeFrame(hwnd, lpDataView);
				lpDataView->vPos = 0;
				lpDataView->hPos = 0;
				SendMessage(hwnd, WM_PAINT, NULL, NULL);
			}
			return TRUE;
        default: 
            break; 
    } 

	return lResult;
}

LRESULT OnMDIActivateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hActive;
	HWND hDeActive;	
	hActive = (HWND) lParam;	//A handle to the MDI child window being activated
	hDeActive = (HWND) wParam;	//A handle to the MDI child window being deactivated

	CHAR str[MAX_STRING];

	//BringWindowToTop(hwnd);

	if(hwnd == hActive){

			lpDataView->flagActive = TRUE;
			HWND hParent = GetParent(hwnd);
			hParent = GetParent(hParent);
			SendMessage(lpDataView->hOwner, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);
			SendMessage(lpDataView->hDataManager, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);

			SendMessage(hParent, DB_ADDSTRINGSET, (WPARAM)&lpDataView->fps, NULL);
			SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);

			//SetScrollRange
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), lpDataView->fStart);
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), lpDataView->fStop);

			SendMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);

			sprintf(str, "Zoom: %.2f%c", lpDataView->scale*100, '%');
			SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
	}

	if(hwnd == hDeActive){
		lpDataView->flagActive = FALSE;
	}

	return TRUE;	//*An MDI child window should return zero is it processes this message
}

//LRESULT OnMDIActivateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
//{
//	HWND hChild;
//	HWND hActive;
//	HWND hDeActive;
//	UINT enableFlag;
//	hChild = (HWND)wParam;		//A handle to the MDI child window being deactivated
//	hActive = (HWND) lParam;	//A handle to the MDI child window being activated
//	hDeActive = (HWND) wParam;
//
//	CHAR str[MAX_STRING];
//
//	BringWindowToTop(hwnd);
//
//	if(hChild){
//		if(hwnd == hActive){
//
//			lpDataView->flagActive = TRUE;
//			HWND hParent = GetParent(hwnd);
//			hParent = GetParent(hParent);
//			SendMessage(lpDataView->hOwner, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);
//			SendMessage(lpDataView->hDataManager, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);
//
//			SendMessage(hParent, DB_ADDSTRINGSET, (WPARAM)&lpDataView->fps, NULL);
//			SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);
//
//			//SetScrollRange
//			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), lpDataView->fStart);
//			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), lpDataView->fStop);
//
//			SendMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);
//
//			sprintf(str, "Zoom: %.2f%c", lpDataView->scale*100, '%');
//			SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
//			//sprintf(str, "Zoom: %d%c", lpDataView->zoom*100, '%');
//			//SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
//		}
//		else
//			lpDataView->flagActive = FALSE;
//
//		enableFlag = MF_ENABLED;
//	}
//	else
//	{
//		enableFlag = MF_DISABLED;
//	}
//
//	return TRUE;	//*An MDI child window should return zero is it processes this message
//}

LRESULT OnUpdateViewPosDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	RECT rcView = {0};

	INT hAdd = NULL;
	INT vAdd = NULL;

	INT hPos = NULL;
	INT vPos = NULL;

	INT wView = NULL;
	INT hView = NULL;

	hAdd = (INT) wParam;
	vAdd = (INT) lParam;

	GetClientRect(hwnd, &rcView);
	wView = rcView.right - rcView.left;
	hView = rcView.bottom - rcView.top;


	hPos = lpDataView->hPos + hAdd;
	vPos = lpDataView->vPos + vAdd;

	if(hPos < 0)								hPos = 0;
	if((hPos + wView) > lpDataView->wStack)		hPos = (lpDataView->wStack - wView);

	if(vPos < 0)								vPos = 0;
	if((vPos + hView) > lpDataView->hStack)		vPos = (lpDataView->hStack - hView);

	lpDataView->hPos = hPos;
	lpDataView->vPos = vPos;
	
	SendMessage(hwnd, WM_PAINT, NULL, NULL);

	return TRUE;
}

LRESULT OnZoomDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	INT zoom;
	CHAR str[MAX_STRING];

	if(lpDataView->flagBlock){
		if(wParam == ZOOM_IN){
			lpDataView->update.upZoom = lpDataView->zoom+1;
			lpDataView->update.upZoomDir = ZOOM_IN;
			lpDataView->zoomBuffer = lpDataView->zoom+1;
		}
		else{
			lpDataView->update.upZoom = lpDataView->zoom - 1;
			lpDataView->update.upZoomDir = ZOOM_OUT;
			lpDataView->zoomBuffer = lpDataView->zoom-1;
		}
		return FALSE;
	}

	//ScaleDataView
	ZoomDataView(lpDataView, hwnd, wParam, lParam);
	//Copy(&lpDataView->osDisplay, lpDataView->os);
	//ScaleObjectSet(&lpDataView->osDisplay, lpDataView->scale);

	if(lpDataView->flagActive){
		sprintf(str, "Zoom: %.2f%c", lpDataView->scale*100, '%');
		SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
	}

	return TRUE;
}

LRESULT OnSetViewPosDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	RECT rcView = {0};

	INT hPos = NULL;
	INT vPos = NULL;

	INT wView = NULL;
	INT hView = NULL;


	GetClientRect(hwnd, &rcView);
	wView = rcView.right - rcView.left;
	hView = rcView.bottom - rcView.top;


	hPos = (INT) wParam;
	vPos = (INT) lParam;

	if(hPos < 0)								hPos = 0;
	if((hPos + wView) > lpDataView->wStack)		hPos = (lpDataView->wStack - wView);

	if(vPos < 0)								vPos = 0;
	if((vPos + hView) > lpDataView->hStack)		vPos = (lpDataView->hStack - hView);

	lpDataView->hPos = hPos;
	lpDataView->vPos = vPos;
	
	SendMessage(hwnd, WM_PAINT, NULL, NULL);

	return TRUE;
}

LRESULT OnSetHwndDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case HWND_DATAMANAGER:
		lpDataView->hDataManager = (HWND) lParam;
		return TRUE;
	case HWND_OWNER:
		lpDataView->hOwner = (HWND) lParam;

		//Set This DataView as Active After Registration
		SendMessage(lpDataView->hOwner, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);
		SetActiveMDI(lpDataView->hOwner, hwnd);

		//???Works???
		SendMessage(lpDataView->hOwner, WM_GETARCHIVEDIR, (WPARAM) lpDataView->strArchivePath, NULL);
		lpDataView->idDataManager = (DWORD) SendMessage(lpDataView->hDataManager, WM_GETHWND, HWND_ID, NULL);
		return TRUE;
	case HWND_PERIPHERAL:
		//Added so that the Camera Properties window could see when a DataView is being closed
		lpDataView->hPeripheral = (HWND) lParam;
		return TRUE;
	default:
		return FALSE;
	}
}

LRESULT OnSaveAsDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	CHAR str[MAX_PATH];
	CHAR filePath[MAX_PATH];

	OPENFILENAME ofn;
	memset(&ofn, NULL, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = (LPSTR) str;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = sizeof(filePath);
	ofn.lpstrFilter = TEXT("BMP(*.bmp)\0*.bmp\0\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_EXPLORER;

	GetSaveFileName(&ofn);

	sprintf(filePath, "%s.bmp", str);

	SaveBMP(lpDataView->wSource, lpDataView->hSource, filePath, lpDataView->pixSrc);
	//SaveBMP(lpDataView->wStack, lpDataView->hStack, filePath, lpDataView->pixBmp);

	PDMFRAME  pdmf = {0};
	AllocPDMFrame(&pdmf, lpDataView->wSource, lpDataView->hSource, DT_PIXEL);
	//AllocPDMFrame(&pdmf, lpDataView->wStack, lpDataView->hStack, DT_PIXEL);
	HWND hDataView = NULL;
	hDataView = CreateMDIDataView(lpDataView->hOwner, str);
	DataView_SetWidthHeight(hDataView, lpDataView->wSource, lpDataView->hSource);
	SetPDMFramePIXEL(&pdmf, lpDataView->pixSrc);
	//SetPDMFramePIXEL(&pdmf, lpDataView->pixBmp);
	DataView_ShowFrame(hDataView, &pdmf);

	FreePDMFrameData(&pdmf);

	ShowWindow(hDataView, SW_SHOW);

	return lResult;

}

LRESULT OnPaintDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	HDC hdc;
	HDC hdcWnd;
	HDC hdcPreview;
	RECT rect;
	RECT rcPreview;
	PAINTSTRUCT ps;
	PAINTSTRUCT psPreview;

	INT xPaint = NULL;
	INT yPaint = NULL;

	INT wView = NULL;
	INT hView = NULL;
	lpDataView->flagBlockObject = TRUE;

	hdcWnd = BeginPaint(hwnd, &ps);

	RECT rc = {0};
	rc.right = 200;
	rc.bottom = 200;
	SelectObject(lpDataView->hdcMem, GetStockObject(DC_PEN));
	SetBkMode(lpDataView->hdcMem, TRANSPARENT);


	if(lpDataView){
		if(lpDataView->hdcMem){		
			hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rect);
			wView = rect.right-rect.left;
			hView = rect.bottom-rect.top;

			xPaint = -lpDataView->hPos;
			yPaint = -lpDataView->vPos;

			BitBlt(hdc, 
				/*-lpDataView->hPos*/xPaint, 
				/*-lpDataView->vPos*/yPaint, 
				rect.right + lpDataView->hPos, 
				rect.bottom + lpDataView->vPos, 
				lpDataView->hdcMem, 
				0, 0, SRCCOPY);

			ReleaseDC(hwnd, hdc);
		}
		else
			ThrowMessage(hwnd, "DataView: Lost HDC");
	}

	EndPaint(hwnd, &ps);

	//Removed 8.19.14 (Untested)
	lpDataView->flagBlockObject = FALSE;

	return lResult;

}

LRESULT OnSetToolDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;
	HWND hParticleData = NULL;

	hParticleData = (HWND) SendMessage(lpDataView->hDataManager, WM_GETHWND, HWND_TEXT, NULL);

	lpDataView->hSend = (HWND)lParam;

	if(lpDataView->tool == (DWORD) lParam)
		lpDataView->tool = TOOL_CURSOR;
	else{
		lpDataView->tool = (DWORD) lParam;
	}
	

	if(lpDataView->tool == TOOL_TRACKPOINT)
		ShowWindow(hParticleData, SW_SHOW);
	else
		ShowWindow(hParticleData, SW_HIDE);

	switch(lpDataView->tool)
	{
		case TOOL_CURSOR:
			ThrowMessage(hwnd, "DataView: Cursor Tool Enabled");
			break;
		case TOOL_HAND:
			break;
		case TOOL_CROSS:
			break;
		case TOOL_MEASUREDIST:
			ThrowMessage(hwnd, "DataView: Distance Tool Enabled");
			break;
		case TOOL_TRACKPOINT:
			ThrowMessage(hwnd, "DataView: Track Tool Enabled");
			break;
		case TOOL_ROIPOLY:
			ThrowMessage(hwnd, "DataView: RoiPoly Tool Enabled");
			break;
		case TOOL_CROP:
			ThrowMessage(hwnd, "DataView: Crop Tool Enabled");
			break;
		default:
			break;
	}

	SendMessage(hwnd, WM_DRAW, NULL, NULL);
	SendMessage(hwnd, WM_PAINT, NULL, NULL);

	return lResult;

}

LRESULT OnSetFrameFocusDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(lpDataView->flagBlock)
		return (LRESULT) (-1);

	if((INT)wParam >= lpDataView->fStart && (INT)wParam <= lpDataView->fStop){
		lpDataView->fStack = (INT) wParam;
		if(lpDataView->flagActive){
			lpDataView->tStart = clock();
			PostMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
			PostMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);

			//SendMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
			//SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);
		}
		PostMessage(lpDataView->hDataManager, WM_GETFRAME, (WPARAM) lpDataView->fStack, (LPARAM) FS_RAW);
	}

	return lpDataView->fStack;
}

LRESULT OnCommandDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;
	DWORD sel = NULL;
	CHAR str[MAX_STRING] = {0};

    switch(LOWORD(wParam))
    {
        case ID_FILE_SAVEAS:
			OnSaveAsDataView(lpDataView, hwnd, wParam, lParam);
			break;
		case ID_FILE_CLOSE:
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;
		case CBN_SELCHANGE:
			sel = SendMessage((HWND) lParam, CB_GETCURSEL, NULL, NULL); 
			sel = sel;
			break;
		case ID_SAVEAS_BMP:
			ThrowMessage(hwnd, "Saving .bmp");
			SendMessage(lpDataView->hDataManager, WM_COMMAND, wParam, lParam);
			ThrowMessage(hwnd, ".bmp Saved.");
			break;
		case ID_TOOLS_PARTICLETRACK:
			lpDataView->isParticleTrack = !lpDataView->isParticleTrack;

			if(lpDataView->isParticleTrack)
				ThrowMessage(hwnd, "DataView: Particle Track Enabled");
			else
				ThrowMessage(hwnd, "DataView: Particle Track Disabled");

			break;
		case DVN_SETALGORITHM:
				lpDataView->idAlgorithm = lParam;
			break;
		case DVN_SETALGORITHM2:
				UINT len;
				len = strlen((CHAR *)lParam);
				memcpy(lpDataView->strAlgorithm, (VOID *)lParam, len*sizeof(CHAR));
				ThrowMessage(hwnd, str);
			break;
		case DVN_SETALGORITHMSTRING:
			
			CHAR strAlgorithm[MAX_STRING];
			CHAR strRoot[MAX_STRING];
			DWORD aParams[MAX_STRING];

			memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
			memset(strRoot, NULL, MAX_STRING*sizeof(CHAR));

			memcpy(strAlgorithm, (VOID *)lParam, MAX_STRING*sizeof(CHAR));
			GetAlgorithmParams(strAlgorithm, strRoot, lpDataView->paramList);
			sprintf(str, "DataView Algorithm Selected: %s", strAlgorithm);
			ThrowMessage(hwnd, str);

			if(!strcmp(strAlgorithm, STR_GRAYSCALE))
				lpDataView->idAlgorithm = IDA_NONE;
			else if(!strcmp(strAlgorithm, STR_IMAGESUBTRACT))
				lpDataView->idAlgorithm = IDA_IMAGESUBTRACT;
			else if(!strcmp(strRoot, STR_GRADIENT))
				lpDataView->idAlgorithm = IDA_GRADIENTPLOW;
			else if(!strcmp(strAlgorithm, STR_LOG1X))
				lpDataView->idAlgorithm = IDA_LOG1X;
			else if(!strcmp(strAlgorithm, STR_LOG2X))
				lpDataView->idAlgorithm = IDA_LOG2X;
			else if(!strcmp(strRoot, STR_STRETCH))
				lpDataView->idAlgorithm = IDA_STRETCH;
			else if(!strcmp(strRoot, STR_WINDOW))
				lpDataView->idAlgorithm = IDA_WINDOW;
			else if(!strcmp(strRoot, STR_PARTICLETRACK))
				lpDataView->idAlgorithm = IDA_PARTICLETRACK;
			else if(!strcmp(strRoot, STR_GRADIENT2))
				lpDataView->idAlgorithm = IDA_GRADIENT2;
			else if(!strcmp(strRoot, STR_GAUSSBLUR))
				lpDataView->idAlgorithm = IDA_GAUSSBLUR;
			else 
				lpDataView->idAlgorithm = lpDataView->idAlgorithm;

			SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
			
			break;
		case DVN_UPDATE:
			PostMessage(lpDataView->hDataManager, WM_GETFRAME, (WPARAM) lpDataView->fStack, (LPARAM) FS_RAW);
			break;
		case DVN_SETXFOV:
			lpDataView->xFOV = (FLOAT) lParam;
			sprintf(str, "DataView: xFOV = %4.2fum", lpDataView->xFOV);
			ThrowMessage(hwnd, str);
			break;
		case DVN_SETYFOV:
			lpDataView->yFOV = (FLOAT) lParam;
			sprintf(str, "DataView: yFOV = %4.2fum", lpDataView->yFOV);
			ThrowMessage(hwnd, str);
			break;
		case DVN_SETZFOV:
			lpDataView->zFOV = (FLOAT) lParam;
			sprintf(str, "DataView: zFOV = %4.2fum", lpDataView->zFOV);
			ThrowMessage(hwnd, str);
			break;
		case DVN_SETTOOL:
			OnSetToolDataView(lpDataView, hwnd, wParam, lParam);
			break;
		case DVN_SETSTARTFRAME:
			lpDataView->fStart = (INT) lParam;
			if(lpDataView->fStack < lpDataView->fStart || lpDataView->fStack > lpDataView->fStop)
				lpDataView->fStack = lpDataView->fStart;
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), lParam);
			break;
		case DVN_SETSTOPFRAME:
			lpDataView->fStop = (INT) lParam;
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), lParam);
			break;
		case DVN_LOOP:
			lpDataView->flagLoop = !lpDataView->flagLoop;
			if(lpDataView->flagLoop)
				lpDataView->idTimer = SetTimer(hwnd, lpDataView->idTimer, lParam, NULL);
			else{
				KillTimer(hwnd, lpDataView->idTimer);
				lpDataView->idTimer = NULL;
			}

		default:
			break;
    }

	switch(HIWORD(wParam))
	{
		case CBN_SELCHANGE:
			{
				WPARAM fSelect = SendMessage((HWND) lParam, CB_GETCURSEL, NULL, NULL);
				SendMessage(hwnd, WM_SETFRAMEFOCUS, (fSelect + lpDataView->fStart), NULL);
			}
			return TRUE;
		default:
			break;
	}

	return lResult;
}

//wParam = Handle to the window that contains the cursor
//LOWORD(lParam) = hit-test code
//HIWORD(lParam) = identifier of mouse message	
LRESULT OnSetCursorDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;
	DWORD dw = NULL;
	HCURSOR hCursor = NULL;

	if((HWND) wParam == hwnd){

		switch(LOWORD(lParam))
		{
		case HTCLIENT:
			hCursor = LoadCursorDataView(lpDataView);
			break;
		case HTLEFT:
			hCursor = LoadCursor(NULL, IDC_SIZEWE);
			break;
		case HTRIGHT:
			hCursor = LoadCursor(NULL, IDC_SIZEWE);
			break;
		case HTBOTTOM:
			hCursor = LoadCursor(NULL, IDC_SIZENS);
			break;
		case HTTOP:
			hCursor = LoadCursor(NULL, IDC_SIZENS);
			break;
		case HTBOTTOMRIGHT:
			hCursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
		case HTBOTTOMLEFT:
			hCursor = LoadCursor(NULL, IDC_SIZENESW);
			break;
		case HTTOPLEFT:
			hCursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
		case HTTOPRIGHT:
			hCursor = LoadCursor(NULL, IDC_SIZENESW);
			break;
		default:
			hCursor = LoadCursor(NULL, IDC_ARROW);
			break;
		}
		SetCursor(hCursor);
		return TRUE;
	}
	else
		return FALSE;


	//return FALSE;
}

LRESULT OnSetFrameSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	INT wStack;
	INT hStack;

	wStack = (INT) wParam;
	hStack = (INT) lParam;

	lpDataView->wStack = wStack;
	lpDataView->hStack = hStack;

	INT mod = lpDataView->wStack%4;

	if(lpDataView->wStack%4){
		lpDataView->wStack = lpDataView->wStack + (4-(lpDataView->wStack%4));
	}

	wStack = lpDataView->wStack;

	if(lpDataView->zoom == 1 && !lpDataView->wSource && !lpDataView->hSource){
		lpDataView->wSource = wStack;
		lpDataView->hSource = hStack;

		if(lpDataView->flagInit){
			lpDataView->pixSrc = (PIXEL *) calloc(lpDataView->wStack*lpDataView->hStack, sizeof(PIXEL));
			lpDataView->flagInit = FALSE;
		}
	}

	if(lpDataView->pdmf.lpvoid){
		free(lpDataView->pdmf.lpvoid);
		lpDataView->pdmf.lpvoid = NULL;
	}

	AllocPDMFrame(&lpDataView->pdmf, lpDataView->wSource, lpDataView->hSource, DT_UCHAR);
	AllocPDMFrame(&lpDataView->pdmfRendering, lpDataView->wSource, lpDataView->hSource, DT_PIXEL);

	//Resize the window frame if the input image is smaller than it's current dimensions
	INT width;
	INT height;
	width = GetWindowWidth(hwnd);
	height = GetWindowHeight(hwnd);

	GetClientDimensions(hwnd, &width, &height);

	if(lpDataView->wStack < width || lpDataView->hStack < height)
		SendMessage(hwnd, WM_SIZE, NULL, NULL);
	//End Resize

	if(lpDataView->hbmp)
	{
		if(!DeleteObject(lpDataView->hbmp)){
			ThrowMessage(hwnd, "DeleteObject() Returned NULL");
			ThrowLastError(hwnd);
		}
		//Destory Display Surface
	}
	if(lpDataView->hdcMem)
		DeleteDC(lpDataView->hdcMem);
	if(lpDataView->hdcDisp)
		DeleteDC(lpDataView->hdcDisp);

	lpDataView->hbmp  = CreateDisplaySurface(hwnd, 
											&lpDataView->hdcMem, 
											&lpDataView->hdcDisp, 
											wStack, hStack, 
											&lpDataView->pixBmp);


	SendMessage(hwnd, WM_NCPAINT, NULL, NULL);

	return TRUE;
}


LRESULT OnMouseLeaveDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	HCURSOR hCursor = NULL;

	//hCursor = LoadCursor(NULL, IDC_ARROW);
	//SetCursor(hCursor);


	return lResult;
}

HCURSOR LoadCursorDataView(DATAVIEW *lpDataView)
{
	HCURSOR hCursor = NULL;

	switch(lpDataView->tool)
	{
	case TOOL_CURSOR:
		return LoadCursor(NULL, IDC_ARROW);
		//return LoadCursor(NULL, IDC_HAND);
	case TOOL_HAND:
		return LoadCursor(NULL, IDC_SIZEALL);
		//return LoadCursor(NULL, IDC_HAND);
	case TOOL_CROSS:
		return LoadCursor(NULL, IDC_ARROW);
	case TOOL_TRACKPOINT:
		return LoadCursor(NULL, IDC_CROSS);
	case TOOL_MEASUREDIST:
		return LoadCursor(NULL, IDC_CROSS);
	case TOOL_ROIPOLY:
		return LoadCursor(NULL, IDC_CROSS);
	default:
		return LoadCursor(NULL, IDC_ARROW);
	}
}

DWORD SetToolDataView(HWND hwnd, DWORD tool)
{
	DWORD dw = NULL;

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETTOOL), (LPARAM) tool);

	return dw;
}

LRESULT OnDrawDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	if(lpDataView->flagQuit){
		//lpDataView->flagBlock = FALSE;
		//SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		return FALSE;
	}

	CreateThread(NULL, NULL, DataViewDrawProc, lpDataView, NULL, NULL);

	return TRUE;

}

	
LRESULT OnShowFrameDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(lpDataView->flagBlock)
		return FALSE;

	if(lpDataView->flagQuit){
		//lpDataView->flagBlock = FALSE;
		//SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		return FALSE;
	}

	if(!wParam)
		return FALSE;

	lpDataView->flagBlock = TRUE;

	//Load Primed Algorithm
	if(lpDataView->flagInit){
		FreeStringSet(&lpDataView->strProc);
		CallocStringSet(lpDataView->strProcBuffer.nStr, MAX_STRING, &lpDataView->strProc);
		MemcpyStringSet(&lpDataView->strProc, &lpDataView->strProcBuffer);
		lpDataView->selProc = lpDataView->selProcBuffer;
		lpDataView->flagInit = FALSE;
	}

	//lpDataView->flagBlock = FALSE;
	

	INT w;
	INT h;
	HANDLE hThread = NULL;

	w = lpDataView->wSource;
	h = lpDataView->hSource;


	PDMFRAME pdmFrame;
	memcpy(&pdmFrame, (VOID *)wParam, sizeof(PDMFRAME));

	memcpy(&lpDataView->tsDataPrev, &lpDataView->tsData, sizeof(TIMESTAMP));
	memcpy(&lpDataView->tsData, &pdmFrame.pdmfh.timeStamp, sizeof(TIMESTAMP));

	if(lpDataView->flgFilter){
		ALGORITHMTHREAD *at = (ALGORITHMTHREAD *)calloc(1, sizeof(ALGORITHMTHREAD));
		at->hwnd = hwnd;
		at->lpDataView = lpDataView;
		at->data = (UCHAR *) calloc(w*h, sizeof(UCHAR));

		if(pdmFrame.pdmfh.typeData == DT_PIXEL){
			UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));

			if(lParam)
				PIXELtoUCHAR(w, h, (PIXEL *)lParam, img, NULL);
			else
				PIXELtoUCHAR(w, h, (PIXEL *)pdmFrame.lpvoid, img, NULL);

			memcpy(lpDataView->pdmf.lpvoid, (VOID *)img, w*h*sizeof(UCHAR));
			memcpy(at->data, (VOID *)img, w*h*sizeof(UCHAR));
			if(img)free(img);
		}
		else if(pdmFrame.pdmfh.typeData == DT_UCHAR){
			if(lParam){
				memcpy(at->data, (VOID *)lParam, w*h*sizeof(UCHAR));
				CopyPDMFrame(&lpDataView->pdmf, &pdmFrame);
			}
			else{
				memcpy(at->data, (VOID *)lpDataView->pdmf.lpvoid, w*h*sizeof(UCHAR));
				CopyPDMFrame(&lpDataView->pdmf, &pdmFrame);
			}
		}


		lpDataView->nShown++;

		hThread = CreateThread(NULL, NULL, AlgorithmProc, at, NULL, NULL);

		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	else{
		lpDataView->flagBlock = TRUE;
		switch(pdmFrame.pdmfh.typeData){
			case DT_PIXEL:
				if(lParam){
					memcpy(lpDataView->pixSrc, (VOID *)lParam, w*h*sizeof(PIXEL));
				}else{
					memcpy(lpDataView->pixSrc, pdmFrame.lpvoid, w*h*sizeof(PIXEL));
				}
				break;
			case DT_UCHAR:
				if(lParam){
					UCHARtoPIXEL(w, h, (UCHAR *)lParam, (PIXEL *)lpDataView->pixSrc);
					//UCHARtoPIXEL(w, h, (UCHAR *)pdmFrame.lpvoid, (PIXEL *)lpDataView->pixSrc);
					CopyPDMFrame(&lpDataView->pdmf, &pdmFrame);
				}else{
					UCHARtoPIXEL(w, h, (UCHAR *)pdmFrame.lpvoid, (PIXEL *)lpDataView->pixSrc);
					CopyPDMFrame(&lpDataView->pdmf, &pdmFrame);
				}
				break;
			default:
				break;
		}

		lpDataView->nShown++;

		SendMessage(hwnd, WM_DRAW, NULL, NULL);
	}

	return TRUE;

}

LRESULT OnDataDropDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(!lpDataView)
		return FALSE;

	if(lpDataView->flagBlock){
		FreeDrop((PDMFRAME *)wParam);
		return FALSE;
	}

	if(lpDataView->flagQuit){
		FreeDrop((PDMFRAME *)wParam);
		//lpDataView->flagBlock = FALSE;
		//SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		return FALSE;
	}

	lpDataView->flagBlock = TRUE;

	//Load Primed Algorithm
	if(lpDataView->flagInit){
		FreeStringSet(&lpDataView->strProc);
		CallocStringSet(lpDataView->strProcBuffer.nStr, MAX_STRING, &lpDataView->strProc);
		MemcpyStringSet(&lpDataView->strProc, &lpDataView->strProcBuffer);
		lpDataView->selProc = lpDataView->selProcBuffer;
		lpDataView->flagInit = FALSE;
	}
	

	INT w;
	INT h;
	HANDLE hThread = NULL;

	w = lpDataView->wSource;
	h = lpDataView->hSource;

	PDMFRAME pdmFrame;
	memcpy(&pdmFrame, (VOID *)wParam, sizeof(PDMFRAME));

	CopyPDMFrame(&lpDataView->pdmf, &pdmFrame);

	memcpy(&lpDataView->tsDataPrev, &lpDataView->tsData, sizeof(TIMESTAMP));
	memcpy(&lpDataView->tsData, &pdmFrame.pdmfh.timeStamp, sizeof(TIMESTAMP));

	switch(pdmFrame.pdmfh.typeData){
		case DT_PIXEL:
			if(lParam){
				memcpy(lpDataView->pixSrc, (VOID *)lParam, w*h*sizeof(PIXEL));
			}else{
				memcpy(lpDataView->pixSrc, pdmFrame.lpvoid, w*h*sizeof(PIXEL));
			}
			break;
		case DT_UCHAR:
			if(lParam){
				UCHARtoPIXEL(w, h, (UCHAR *)lParam, (PIXEL *)lpDataView->pixSrc);
			}else{
				UCHARtoPIXEL(w, h, (UCHAR *)pdmFrame.lpvoid, (PIXEL *)lpDataView->pixSrc);
			}
			break;
		default:
			break;
	}

	lpDataView->nShown++;

	FreeDrop((PDMFRAME *)wParam);
	SendMessage(hwnd, WM_DRAW, NULL, NULL);

	return TRUE;

}

LRESULT OnMouseWheelDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	CHAR str[MAX_STRING];
	INT posWheel;

	SetFocus(hwnd);
	SetActiveWindow(hwnd);

	posWheel = (SHORT) HIWORD(wParam);

	if(GetKeyState(VK_LCONTROL)&0x8000){
		INT zoom = NULL;

		if(posWheel > 0){
			SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_IN, NULL);
		}
		else{
			SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_OUT, NULL);
		}

	}
	else if(GetKeyState(VK_R)&0x8000){
		if(posWheel > 0){
			lpDataView->fAngle = lpDataView->fAngle + lpDataView->fAngleStep;
		}else{
			lpDataView->fAngle = lpDataView->fAngle - lpDataView->fAngleStep;
		}
		SendMessage(hwnd, WM_DRAW, NULL, NULL);
	}
	else
	{
		if(posWheel > 0){
			lpDataView->fStack--;
			if(lpDataView->fStack < lpDataView->fStart){
				lpDataView->fStack = lpDataView->fStart;
			}
		}
		else{			
			lpDataView->fStack++;
			if(lpDataView->fStack > lpDataView->fStop){
				lpDataView->fStack = lpDataView->fStop;
			}

			
		}
		SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
	}

	return TRUE;
}

//LRESULT OnMouseWheelDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
//{
//
//	CHAR str[MAX_STRING];
//	INT posWheel;
//
//	SetFocus(hwnd);
//	SetActiveWindow(hwnd);
//
//	posWheel = (SHORT) HIWORD(wParam);
//
//	if(GetKeyState(VK_LCONTROL)&0x8000){
//		INT zoom = NULL;
//
//		if(posWheel > 0){
//			SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_IN, NULL);
//		}
//		else{
//			SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_OUT, NULL);
//		}
//
//	}
//	else
//	{
//		if(posWheel > 0){
//			lpDataView->fStack--;
//			if(lpDataView->fStack < lpDataView->fStart){
//				lpDataView->fStack = lpDataView->fStart;
//			}
//		}
//		else{			
//			lpDataView->fStack++;
//			if(lpDataView->fStack > lpDataView->fStop){
//				lpDataView->fStack = lpDataView->fStop;
//			}
//
//			
//		}
//		SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
//	}
//
//	return TRUE;
//}

LRESULT OnMouseMoveDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	RECT rcView;

	DWORD wView;
	DWORD hView;

	INT hPos;
	INT vPos;

	CHAR str[MAX_STRING];

	memcpy(&lpDataView->ptCursor, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
	memcpy(&pt, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));

	if(wParam&MK_LBUTTON){

		GetClientRect(hwnd, &rcView);

		wView = rcView.right - rcView.left;
		hView = rcView.bottom - rcView.top;


		hPos = lpDataView->ptGrab.x - pt.x + lpDataView->hPos;
		vPos = lpDataView->ptGrab.y - pt.y + lpDataView->vPos;

		if(hPos < 0)								hPos = 0;
		if((hPos + wView) > lpDataView->wStack)		hPos = (lpDataView->wStack - wView);

		if(vPos < 0)								vPos = 0;
		if((vPos + hView) > lpDataView->hStack)		vPos = (lpDataView->hStack - hView);

		lpDataView->hPos = hPos;
		lpDataView->vPos = vPos;

		lpDataView->ptGrab.x = pt.x;
		lpDataView->ptGrab.y = pt.y;
			
		SendMessage(hwnd, WM_PAINT, NULL, NULL);

	}

	if(lpDataView->tool == TOOL_CROP){
		if(lpDataView->flgCropStart){
			lpDataView->ptCropEnd.x = lpDataView->ptCursor.x + lpDataView->hPos;
			lpDataView->ptCropEnd.y = lpDataView->ptCursor.y + lpDataView->vPos;
			DataView_Paint(hwnd);
		}
	}

	if(!lpDataView->flagActive)
		return TRUE;

	FLOAT xNorm = (FLOAT)(pt.x + lpDataView->hPos) / (FLOAT)(lpDataView->wStack);
	FLOAT yNorm = (FLOAT)(pt.y + lpDataView->vPos) / (FLOAT)(lpDataView->hStack);

	FLOAT x = xNorm*lpDataView->wSource;
	FLOAT y = yNorm*lpDataView->hSource;

	DWORD i = ((INT)(pt.y + lpDataView->vPos)*lpDataView->wStack)+(INT)(lpDataView->hPos + pt.x);
	//DWORD i = (INT)(y*lpDataView->wStack) + (INT)(x)

	sprintf(str, "(%.1f, %.1f)", x, y);
	SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_CURSOR, (LPARAM) str);

	if(lpDataView->pixBmp){
		if(i < lpDataView->wStack*lpDataView->hStack){
			sprintf(str, "(%d, %d, %d)", lpDataView->pixBmp[i].r, lpDataView->pixBmp[i].g, lpDataView->pixBmp[i].b);
			SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_PIXELVAL, (LPARAM) str);
		}
	}

	return TRUE;
}

LRESULT OnSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	INT wFrame;
	INT hFrame;
	INT hClient;
	INT wClient;

	INT hClientSet;
	INT wClientSet;

	RECT rcFrame;
	RECT rcClient;

	//Frame
	GetClientRect(hwnd, &rcFrame);
	wFrame = rcFrame.right - rcFrame.left;
	hFrame = rcFrame.bottom - rcFrame.top;

	//Client
	GetClientRect(hwnd, &rcClient);
	GetClientRect(lpDataView->hView, &lpDataView->rcView);

	wClient = rcClient.right - rcClient.left;
	hClient = rcClient.bottom - rcClient.top;

	wClientSet = wClient;
	hClientSet = hClient;

	if(0){
	if(wParam == SIZE_MAXIMIZED){
		INT i = NULL;
		FLOAT scale = lpDataView->scale;
		INT w = LOWORD(lParam);
		INT h = HIWORD(lParam);
		INT wSrc = lpDataView->wSource;
		INT hSrc = lpDataView->hSource;
		INT wNew = NULL;
		INT hNew = NULL;
		if(w < wSrc || h < hSrc){
			for(i = 0; i < 10; i++){
				scale+=0.5;
				
				wNew = wSrc*scale;
				hNew = hSrc*scale;

				if(w < wNew && h < hNew){
					break;
				}

			}

			lpDataView->scalePrev = lpDataView->scale;
			lpDataView->scale = scale;

			SetZoomDataView(lpDataView, hwnd, scale);
			return TRUE;
		}

	}
	}

	//Called when using size bar or upper right window restore button
	if(wParam == SIZE_RESTORED){
		INT BREAK;
		BREAK = 0;
		BREAK = 0;
	}

	if(lpDataView){

		if(lpDataView->wStack && lpDataView->hStack){
			if(wClient > lpDataView->wStack)
				wClientSet = lpDataView->wStack;
			if(hClient > lpDataView->hStack)
				hClientSet = lpDataView->hStack;

			DataView_SizeClient(hwnd, wClientSet, hClientSet);
		}

		if(lpDataView->wStack && lpDataView->hStack){
			if(lpDataView->wStack > wClient || lpDataView->hStack > hClient){
				INT delX = lpDataView->wStack - lpDataView->hPos;
				INT delY = lpDataView->hStack - lpDataView->vPos;


				if(wClient > delX || hClient > delY){

					if(wClient < delX){
						delX = NULL;
						
					}
					if(hClient < delY){
						delY = NULL;
					}

					SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM) delX, (LPARAM)delY);

					return TRUE;
				}
			}
		}
	}

	if(!wParam){
		SendMessage(hwnd, WM_PAINT, NULL, NULL);
	}


	return TRUE;
}

LRESULT OnTimerDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	if(lpDataView->flagLoop){
		if(lpDataView->fStack >= lpDataView->fStop)
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStart, NULL);
		else
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
	}

	return TRUE;
}

LRESULT OnCreateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	DWORD wndProc;

	lpDataView->hwnd = hwnd;
	wndProc = NULL;

	//Removes Resizing Flicker
	LONG_PTR lptr;
	lptr = GetWindowLongPtr(hwnd, GWL_STYLE);
	lptr = lptr|WS_CLIPCHILDREN;
	
	if(!SetWindowLongPtr(hwnd, GWL_STYLE, lptr))
		ThrowMessage(hwnd, "DataView Error: SetWindowLongPtr(DataView)!!");

	memset(&lpDataView->pdmf, NULL, sizeof(PDMFRAME));

	//Temporary
	lpDataView->nParticles = 1;

	lpDataView->zoom = 1;
	lpDataView->flagInit = TRUE;
	lpDataView->scale = 1;
	lpDataView->xFOV = 1000;
	lpDataView->yFOV = 1000;

	AllocWndLink(&lpDataView->wls, MAX_LINK);
	AllocPointSet(&lpDataView->ptSet, MAX_POLY);

	Alloc(&lpDataView->os, OBJECTSET_SIZE);
	Alloc(&lpDataView->osBuffer, OBJECTSET_SIZE);
	Alloc(&lpDataView->osDataView, OBJECTSET_SIZE);
	//Alloc(&lpDataView->osDisplay, OBJECTSET_SIZE);

	//Initizalize Crop Tool
	//lpDataView->tool = TOOL_CROP;
	lpDataView->ptCropStart = FPoint(0, 0);
	lpDataView->ptCropEnd = FPoint(0, 0);

	lpDataView->flgFilter = TRUE;
	lpDataView->flagBlock = FALSE;

	lpDataView->fAngleStep = 2.5;
	lpDataView->fAngle = 0.0;
	//lpDataView->flgFilter = FALSE;

	return TRUE;

}

LRESULT ZoomDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	FLOAT zoom = NULL;
	FLOAT scale = NULL;

	//lpDataView->flagBlock = TRUE;

	if(wParam == ZOOM_IN){
		lpDataView->zoom++;
		lpDataView->scale+=SCALE;
		//lpDataView->scale+=0.50;
		if(lpDataView->zoom > MAX_ZOOM)
			lpDataView->zoom = MAX_ZOOM;

	}

	if(wParam == ZOOM_OUT){
		lpDataView->zoom--;
		lpDataView->scale-=SCALE;
		//lpDataView->scale-=0.50;
		if(lpDataView->scale <= 0.25)
			lpDataView->scale = 0.25;

	}

	POINT ptPixel = DataView_GetCursorPixelCoordinate(lpDataView);
	FLOAT xNorm = NULL;
	FLOAT yNorm = NULL;
	GetNormalizedCoordinate(lpDataView->wStack, lpDataView->hStack, ptPixel.x, ptPixel.y, &xNorm, &yNorm);
	
	zoom = lpDataView->scale;
	scale = lpDataView->scale;
	SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
	
	INT width = NULL;
	INT height = NULL;
	GetClientDimensions(hwnd, &width, &height);

	if(lpDataView->wStack < width || lpDataView->hStack < height)
		SendMessage(hwnd, WM_SIZE, NULL, NULL);
		//DataView_SizeFrame(hwnd, lpDataView);

	

	DataView_UpdateCursorPixelPosition(lpDataView, xNorm, yNorm);

	SendMessage(hwnd, WM_DRAW, NULL, NULL);

	//if(wParam == ZOOM_OUT){
	//	DataView_UpdateCursorPixelPosition(lpDataView, xNorm, yNorm);
	//	//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)(-lpDataView->ptCursor.x), (LPARAM)(-lpDataView->ptCursor.y));
	//}else{
	//	//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)lpDataView->ptCursor.x, (LPARAM)lpDataView->ptCursor.y);
	//	DataView_UpdateCursorPixelPosition(lpDataView, xNorm, yNorm);
	//}

	

	return TRUE;

}

DWORD SetZoomDataView(DATAVIEW *lpDataView, HWND hwnd, FLOAT scale)
{
	FLOAT zoom = scale;
	lpDataView->scalePrev = scale;

	lpDataView->flagBlock = TRUE;

	POINT ptPixel = DataView_GetCursorPixelCoordinate(lpDataView);
	FLOAT xNorm = NULL;
	FLOAT yNorm = NULL;
	GetNormalizedCoordinate(lpDataView->wStack, lpDataView->hStack, ptPixel.x, ptPixel.y, &xNorm, &yNorm);
	
	zoom = lpDataView->scale;
	scale = lpDataView->scale;
	SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
	
	INT width = NULL;
	INT height = NULL;
	GetClientDimensions(hwnd, &width, &height);

	if(lpDataView->wStack < width || lpDataView->hStack < height)
		SendMessage(hwnd, WM_SIZE, NULL, NULL);

	DataView_UpdateCursorPixelPosition(lpDataView, xNorm, yNorm);

	SendMessage(hwnd, WM_DRAW, NULL, NULL);


	return TRUE;

}
LRESULT OnInitAlgorithm(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CHAR str[MAX_STRING];
	DWORD selProc = (DWORD) wParam;
	STRINGSET *strSet = (STRINGSET *)lParam;

	//Check For Blocked Thread
	if(lpDataView->flagBlock){

		//Prime Algorithm Buffer
		lpDataView->flagInit = TRUE;
		if(strSet->nStr){
			FreeStringSet(&lpDataView->strProcBuffer);
			CallocStringSet(strSet->nStr, strSet->string[0].nChar, &lpDataView->strProcBuffer);
			MemcpyStringSet(&lpDataView->strProcBuffer, strSet);
			lpDataView->selProcBuffer = selProc;
		}
		return FALSE;
	}

	lpDataView->selProc = selProc;

	if(strSet->nStr){
		FreeStringSet(&lpDataView->strProc);
		CallocStringSet(strSet->nStr, strSet->string[0].nChar, &lpDataView->strProc);
		MemcpyStringSet(&lpDataView->strProc, strSet);
		SortAlgorithmProcedures(selProc, strSet);

		//Remove This
		SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
	}

	return TRUE;
}

DWORD GetSystemTimeTest(HWND hwnd, DWORD nIterations, DWORD nSubIterations)
{
	SYSTEMTIME systemTime;
	CHAR strMessage[MAX_STRING];
	DWORD i = 0;
	DWORD s = 0;

	clock_t cStart = 0;

	for(i = 0; i < nIterations; i++){

		cStart = clock();
		for(s = 0; s < nSubIterations; s++){
		
			GetSystemTime(&systemTime);

		}
		sprintf(strMessage, "Iteration: %d - (%d ms)", i, clock()-cStart);
		ThrowMessage(hwnd, strMessage);
	}

	return TRUE;
}

//-[1]Iterate(5)
//		-Function(1, 2, 3, 4, 5)


//-Matching Features
//		-InverseLaplacian(template)
//		-InverseLaplacian(population)
//		-Generate a transformation vector field
//			-Comparing the IL(template) to the IL(population) using subtraction or eigen vectors
//		-Overlay IL(template) to IL_TRANSFORMED(population) and do a correlation match
//			-Matching should be based on a function of (subtraction, fft, etc..)

LRESULT AlgorithmProcDataView(DATAVIEW *lpDataView, UCHAR *in, UCHAR *out)
{
	if(lpDataView->flagQuit)
		return FALSE;

	INT i = NULL;
	INT iProc = NULL;
	DWORD selProc = lpDataView->selProc;
	UINT w = lpDataView->wSource;
	UINT h = lpDataView->hSource;
	UINT iSrc = NULL;
	UINT src1 = NULL;
	UINT src2 = NULL;
	DWORD nProc = TRUE;
	DWORD nChar = NULL;
	FLOAT pList[MAX_PARAM];

	DWORD fRender = NULL;
	DWORD procRender = NULL;

	CHAR str[MAX_STRING];
	CHAR strIn[MAX_STRING];
	CHAR strAlgorithm[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));

	UCHAR *img = (UCHAR *) malloc(w*h*sizeof(UCHAR));

	nProc = lpDataView->strProc.nStr;

	if(!nProc)
		nProc = TRUE;

	FLOAT **imgProc = (FLOAT **)malloc(nProc*sizeof(FLOAT*));
	memset(imgProc, NULL, nProc*sizeof(FLOAT));

	for(i = 0; i < nProc; i++){
		imgProc[i] = (FLOAT *)malloc(w*h*sizeof(FLOAT));
		if(imgProc[i]){
			memset(imgProc[i], NULL, w*h*sizeof(FLOAT));
		}
	}

	UCHARtoFLOAT(w, h, in, (FLOAT *)imgProc[0]);

	for(i = 1; i <= selProc; i++){	

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
		memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
		
		nChar = lpDataView->strProc.string[i].nChar;
		memcpy(str, lpDataView->strProc.string[i].str, nChar*sizeof(CHAR));
		ReadStringWithin(str, strIn, '[', ']');
		iProc = atoi(strIn);
		RemoveStringWithin(str, MAX_STRING, '[', ']');
		
		memset(pList, NULL, MAX_PARAM*sizeof(DWORD));
		GetAlgorithmParams(str, strAlgorithm, pList);

		src1 = (UINT)pList[0];
		src2 = (UINT)pList[1];

		iSrc = (UINT)pList[0];
		//iSrc2 = (UINT)pList[1];

		if(!strcmp(strAlgorithm, STR_GRADIENT)){
			GradientFirstOrder(w, h, pList[1], (FLOAT *)imgProc[iSrc], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CONST)){
			DWORD i = 0;
			DWORD max = w*h;
			for(i = 0; i < max; i++){
				imgProc[iProc][i] = src1;
			}
		}
		else if(!strcmp(strAlgorithm, STR_PI)){
			DWORD i = 0;
			DWORD max = w*h;
			for(i = 0; i < max; i++){
				imgProc[iProc][i] = PI;
			}

		}
		else if(!strcmp(strAlgorithm, STR_LOG)){
			LogOperator(w, h, imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_FLOODFILL)){
			FloodFill(w, h, (FLOAT *)imgProc[iSrc], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_AND)){
			AND(w*h, imgProc[src1], imgProc[src2], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_XOR)){
			XOR(w*h, imgProc[src1], imgProc[src2], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_LAPLACIAN)){
			Laplacian(w, h, src2/*pList[1]*/, imgProc[src1/*iSrc*/], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_INVERSELAPLACIAN)){
			InverseLaplacian(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_LAPLACIANANGLE)){
			LaplacianAngle(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MULTIPLY)){
			Multiply(w*h, imgProc[src1], imgProc[src2], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MAX)){
			Max(w*h, imgProc[src1], imgProc[src2], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DEGREES)){
			DWORD i = 0;
			DWORD max = w*h;
			for(i = 0; i < max; i++){
				imgProc[iProc][i] = imgProc[src1][i]*(180.0/PI);
			}
		}
		else if(!strcmp(strAlgorithm, STR_RGB)){
			fRender = TRUE;
			procRender = i;
		}
		else if(!strcmp(strAlgorithm, STR_SOBEL)){
			GradientSobel(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if (!strcmp(strAlgorithm, STR_GRADIENTANGLE)){
			GradientAngle(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CENTRALDIFFERENCE)){
			GradientCentralDifference(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STRETCH)){
			Stretch(w, h, pList[1], pList[2], imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_BOOLEANAND)){
			BooleanAND(w, h, imgProc[iSrc], imgProc[(UINT)pList[1]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_BOOLEAN)){
			Boolean(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_PAVLIDISCONTOUR)){
			PavlidisContour(w, h, imgProc[/*iSrc*/(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_WINDOW)){
			ClampData(w*h, pList[1], pList[2], imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_GAUSSBLUR)){
			GaussianBlur(w, h, (UINT)pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NONMAXIMUM)){
			NonMaximumSuppression(w, h, imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NONMAXIMUMEXP)){
			NonMaximumSuppressionExp(w, h, imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_INVERT)){
			InvertData(w*h,  imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_GETFRAME)){
			GetFrame(lpDataView->hDataManager, w, h, (DWORD)(lpDataView->fStack + pList[0]), (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_GETSTATICFRAME)){
			GetFrame(lpDataView->hDataManager, w, h, (DWORD)(pList[0]+lpDataView->fStart), (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SUBTRACT)){
			ImageSubtract(w, h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ADD)){
			Addition(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DIVIDE)){
			Divide(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NORMALIZE)){
			Normalize(w, h, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SQUARE)){
			Square(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SQUAREROOT)){
			SquareRoot(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ERODE)){
			Erode(w, h, (UINT)pList[1], (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DILATE)){
			Dilate(w, h, (UINT)pList[1], (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MEDIANFILTER)){
			MedianFilter(w, h, (FLOAT *)imgProc[src1], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MEAN)){
			Mean(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STDDEV)){
			StdDev(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_VARIANCE)){
			Variance(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NORMVARIANCE)){
			NormalizedVariance(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CHAMFER)){
			Chamfer(w, h, imgProc[src1/*(UINT)pList[0]*/], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CONTRAST)){
			Contrast(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_THRESHOLDCONST)){
			Threshold(w, h, (UINT)src2, (FLOAT *)imgProc[src1], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_THRESHOLD)){
			Threshold(w, h, (FLOAT *)imgProc[src1], imgProc[src2], imgProc[iProc]);
			//Threshold(w, h, pList[1], (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_HISTOGRAM)){
			GetHistogram(w, h, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MEDIAN)){
			GetHistogramMedian(w*h, imgProc[src1], imgProc[iProc]);
			//Median(w*h, imgProc[src1], imgProc[iProc]);
			//Threshold(w, h, pList[1], (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_HESSIANDXX)){
			HessianDxx(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_HESSIANDYY)){
			HessianDyy(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_HESSIANDXY)){
			HessianDxy(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_HESSIANDYX)){
			HessianDyx(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ZEROCROSSING)){
			ZeroCrossing(w, h, src2, imgProc[src1], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MIP)){
			fRender = TRUE;
			procRender = i;
		}
		//else if(!strcmp(strAlgorithm, STR_ITERATE)){
			//Find the child string
			//Create a loop that calls the string child function iteratively
			//Use the child string input parameters for each
			//Iterate(
		//}
		//else if(!strcmp(strAlgorithm, STR_CHANVESE)){
		//	Threshold(w, h, (UINT)src2, (FLOAT *)imgProc[src1], (FLOAT *)imgProc[iProc]);
		//}
		//TOADD: Significant Curvature
		//-Highlights the Ridges and Valleys of a gradient
		else{
			i = i;
		}

	}

	if(fRender){
		UINT nParam;
		memset(str, NULL, MAX_STRING*sizeof(UCHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(UCHAR));
		nChar = lpDataView->strProc.string[procRender].nChar;
		memcpy(str, lpDataView->strProc.string[procRender].str, nChar*sizeof(CHAR));
		nParam = GetAlgorithmParams(str, strAlgorithm, pList);
		if(nParam == 3)
			FLOATtoPIXEL(w, h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], lpDataView->pixSrc);
		if(nParam == 4){
			MaximumIntensityProjection(w, h, (FLOAT *)imgProc[(UINT)pList[0]],(FLOAT *) imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], (FLOAT *)imgProc[(UINT)pList[3]], lpDataView->pixSrc);
		}
	}
	else{
		ClampData(w*h, 0, 255, (FLOAT *)imgProc[selProc], img);
		UCHARtoPIXEL(w, h, (UCHAR *) img, lpDataView->pixSrc);
	}

	for(i = 0; i < nProc; i++){
		if(imgProc[i])free(imgProc[i]);
	}

	if(imgProc) free(imgProc);
	if(img) free(img);

	return TRUE;
}

LRESULT UpdateFromBlock(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	INT zoom = NULL;

	if(!lpDataView->flagBlock){
	
		if(lpDataView->update.upZoom){
			if(lpDataView->zoom < MAX_ZOOM && lpDataView->zoom > 0)
				lpDataView->zoom = lpDataView->update.upZoom;
		
			zoom = lpDataView->zoom;

			SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);
			lpDataView->update.upZoom = NULL;
		}

	}

	return TRUE;
}


DWORD WINAPI DataViewDrawProc(LPVOID param)
{
	DATAVIEW *lpDataView = (DATAVIEW *)param;

	if(lpDataView->flagQuit)
		return FALSE;

	lpDataView->flagBlock = TRUE;

	INT i = NULL;

	Clear(&lpDataView->osDataView);

	if(lpDataView->tool == TOOL_CROP){
		RECT rect;
		INT w = lpDataView->wSource;
		INT h = lpDataView->hSource;
		
		rect.left = (LONG) MinFloat(lpDataView->ptCropStart.x, lpDataView->ptCropEnd.x);
		rect.right = (LONG) MaxFloat(lpDataView->ptCropStart.x, lpDataView->ptCropEnd.x);
		rect.top = (LONG) MinFloat(lpDataView->ptCropStart.y, lpDataView->ptCropEnd.y);
		rect.bottom = (LONG) MaxFloat(lpDataView->ptCropStart.y, lpDataView->ptCropEnd.y);
		
		FPOINT ptUL;
		FPOINT ptUR;
		FPOINT ptLL;
		FPOINT ptLR;
		VECTOR vTop;
		VECTOR vRight;
		VECTOR vLeft;
		VECTOR vBottom;

		ptUL = FPoint(rect.left, rect.top);
		ptUR = FPoint(rect.right, rect.top);
		ptLL = FPoint(rect.left, rect.bottom);
		ptLR = FPoint(rect.right, rect.bottom);
		GetVector(ptUL, ptUR, &vTop);
		GetVector(ptUR, ptLR, &vRight);
		GetVector(ptLR, ptLL, &vBottom);
		GetVector(ptLL, ptUL, &vLeft);

		Append(&lpDataView->osDataView, vTop, 1, ColorR(0, 255, 255), 0.5, NULL);
		Append(&lpDataView->osDataView, vRight,1,  ColorR(0, 255, 255), 0.5, NULL);
		Append(&lpDataView->osDataView, vBottom, 1, ColorR(0, 255, 255), 0.5, NULL);
		Append(&lpDataView->osDataView, vLeft, 1, ColorR(0, 255, 255), 0.5, NULL);
		
	}

	if(lpDataView->nPtMeasure && lpDataView->tool == TOOL_MEASUREDIST){
		INT p;
		for(p = 0; p < lpDataView->nPtMeasure; p++){
			DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, Point((lpDataView->ptMeasure[p].x), (lpDataView->ptMeasure[p].y)), RGB(255, 0, 0), 3);

		}
	}

	if(lpDataView->nPt && lpDataView->tool == TOOL_TRACKPOINT){
		INT p;
		for(p = 0; p < lpDataView->nPt; p++)
		{
			DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, Point(lpDataView->pt[p].x, lpDataView->pt[p].y), RGB(0, 0, 255), 3);
		}
	}

	if(lpDataView->ptSet.nPoints && lpDataView->tool == TOOL_ROIPOLY){
		INT p = NULL;
		for(p = 0; p < lpDataView->ptSet.nPoints; p++){
			DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, lpDataView->ptSet.point[p], RGB(255, 0, 0), 5);
		}
	}



	{
		UINT w = lpDataView->wSource;
		UINT h = lpDataView->hSource;
		UINT wZoom = NULL;
		UINT hZoom = NULL;
		INT wScale = NULL;
		INT hScale = NULL;

		GetScaleDimensions(lpDataView->scale, w, h, &wScale, &hScale);

		if(wScale != lpDataView->wSource && hScale != lpDataView->hSource){
			if(lpDataView->fAngle){
				PIXEL *temp = (PIXEL *)malloc(w*h*sizeof(PIXEL));
				if(temp){
					RotateImage(lpDataView->fAngle, lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, temp);
					ScaleImage(lpDataView->scale, w, h, temp, lpDataView->pixBmp);
					free(temp);
				}
			}
			else{
				ScaleImage(lpDataView->scale, w, h, lpDataView->pixSrc, lpDataView->pixBmp);
			}
		}else{
			if(lpDataView->fAngle){
				RotateImage(lpDataView->fAngle, lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, lpDataView->pixBmp);
			}else{
				if(lpDataView->pixBmp&&lpDataView->pixSrc){
					memcpy(lpDataView->pixBmp, lpDataView->pixSrc, w*h*sizeof(PIXEL));
				}
			}
		}
	}




	if(lpDataView->flagObjectSet){
		if(!lpDataView->flagBlockObject){
			Copy(&lpDataView->os, lpDataView->osBuffer);
			lpDataView->flagObjectSet = FALSE;
		}
	}

	SelectObject(lpDataView->hdcMem, GetStockObject(DC_PEN));
	SetBkMode(lpDataView->hdcMem, TRANSPARENT);
	DrawObjectSet(lpDataView->hdcMem, lpDataView->os);
	DrawObjectSet(lpDataView->hdcMem, lpDataView->osDataView);

	if(lpDataView->pixSrc && lpDataView->pdmfRendering.lpvoid){
		memcpy(lpDataView->pdmfRendering.lpvoid, lpDataView->pixSrc, lpDataView->pdmfRendering.pdmfh.width*lpDataView->pdmfRendering.pdmfh.height*sizeof(PIXEL));
	}

	if(lpDataView->flagActive){
		CHAR str[MAX_STRING];
		INT tFrameRate = Subtract(lpDataView->tsData, lpDataView->tsDataPrev);
		INT tCalc = clock() - lpDataView->tStart;
		INT tDisp = clock() - lpDataView->tLast;
		lpDataView->tLast = clock();
		sprintf(str, "%.1f FPS", ((FLOAT)(1000) / (FLOAT)(tDisp)));
		SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_FRAME, (LPARAM)str);
		sprintf(str, "%.1f FPS", ((FLOAT)(1000) / (FLOAT)(tFrameRate)));
		SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_FRAMERATE, (LPARAM)str);
	}

	DataView_Paint(lpDataView->hwnd);

	lpDataView->flagBlock = FALSE;

	return TRUE;

}


#endif

LRESULT DataView_SizeFrame(HWND hwnd, DATAVIEW *lpDataView)
{

	WINDOWPLACEMENT wp;
	memset(&wp, NULL, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &wp);

	if(wp.showCmd == SW_MAXIMIZE)
		return FALSE;

	RECT rect;
	memset(&rect, NULL, sizeof(RECT));
	GetWindowRect(hwnd, &rect);

	POINT ptUpperLeft;

	ptUpperLeft.x = rect.left;
	ptUpperLeft.y = rect.top;

	HWND hParent = GetParent(hwnd);
	//HWND hMDIClient = (HWND) SendMessage(lpDataView->hOwner, WM_GETHWND, HWND_MDI, NULL);

	ScreenToClient(hParent/*hMDIClient*/, &ptUpperLeft);

	WINDOWINFO wi = {0};
	wi.cbSize = sizeof(wi);
	GetWindowInfo(hwnd, &wi);
	
	rect.left = 0;
	rect.right = lpDataView->wStack;
	rect.top = 0;
	rect.bottom = lpDataView->hStack;
	AdjustWindowRect(&rect, wi.dwStyle/*MDIS_ALLCHILDSTYLES*/, FALSE);

	DWORD wFrame = Magnitude(rect.left) + Magnitude(rect.right);
	DWORD hFrame = Magnitude(rect.bottom) + Magnitude(rect.top);
	MoveWindow(hwnd, ptUpperLeft.x, ptUpperLeft.y, wFrame, hFrame, TRUE);

	if(lpDataView->hPos < 0) lpDataView->hPos = 0;
	if(lpDataView->vPos < 0) lpDataView->vPos = 0;

	//SendMessage(hwnd, WM_PAINT, NULL, NULL);

	return TRUE;

}

LRESULT DataView_SizeClient(HWND hwnd, UINT width, UINT height)
{

	WINDOWPLACEMENT wp;
	memset(&wp, NULL, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd, &wp);

	if(wp.showCmd == SW_MAXIMIZE)
		return FALSE;

	RECT rect;
	memset(&rect, NULL, sizeof(RECT));
	GetWindowRect(hwnd, &rect);

	POINT ptUpperLeft;

	ptUpperLeft.x = rect.left;
	ptUpperLeft.y = rect.top;

	HWND hParent = GetParent(hwnd);
	//HWND hMDIClient = (HWND) SendMessage(lpDataView->hOwner, WM_GETHWND, HWND_MDI, NULL);

	ScreenToClient(hParent, &ptUpperLeft);

	WINDOWINFO wi = {0};
	wi.cbSize = sizeof(wi);
	GetWindowInfo(hwnd, &wi);
	
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;
	AdjustWindowRect(&rect, wi.dwStyle/*MDIS_ALLCHILDSTYLES*/, FALSE);

	DWORD wFrame = Magnitude(rect.left) + Magnitude(rect.right);
	DWORD hFrame = Magnitude(rect.bottom) + Magnitude(rect.top);
	MoveWindow(hwnd, ptUpperLeft.x, ptUpperLeft.y, wFrame, hFrame, TRUE);


	return TRUE;

}




#ifdef DATAVIEW1



//Works 6.1.12
//Updating Upper Code
LRESULT CALLBACK DataViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATAVIEW *lpDataView = (DATAVIEW *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_ERASEBKGND:
		if(lParam)
			break;
		else
			return TRUE;
	case DF_INITFILTER:
		{
			return OnInitAlgorithm(lpDataView, hwnd, wParam, lParam);
		}
	case WM_SHOWFRAME:
		{
			return OnShowFrameDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_DRAW:
		{
			return OnDrawDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			OnPaintDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_ADDFILELIST:
		{
			OnAddFileListDataView(lpDataView, hwnd, wParam, lParam);			
		}
		return TRUE;
	case WM_MDIACTIVATE:
		{
			OnMDIActivateDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_SETFRAMESIZE:
		{
			return OnSetFrameSizeDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case DVM_FRAMEFORWARD:
		{
			if(lpDataView->flagBlock)
				return FALSE;

			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
		}
		break;
	case DVM_FRAMEBACKWARD:
		{
			if(lpDataView->flagBlock)
				return FALSE;
			
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack-1, NULL);
		}
		break;
	case DVM_ZOOM:
		{		
			INT zoom;

			if(lpDataView->flagBlock){
				if(wParam == ZOOM_IN){
					lpDataView->update.upZoom = lpDataView->zoom+1;
					lpDataView->update.upZoomDir = ZOOM_IN;
				}
				else{
					lpDataView->update.upZoom = lpDataView->zoom - 1;
					lpDataView->update.upZoomDir = ZOOM_OUT;
				}
				return FALSE;
			}

			ZoomDataView(lpDataView, hwnd, wParam, lParam);

			//if(wParam == ZOOM_IN){
			//	lpDataView->zoom++;
			//	if(lpDataView->zoom > MAX_ZOOM)
			//		lpDataView->zoom = MAX_ZOOM;

			//}

			//if(wParam == ZOOM_OUT){
			//	lpDataView->zoom--;
			//	if(lpDataView->zoom < 1)
			//		lpDataView->zoom = 1;

			//}

			//zoom = lpDataView->zoom;
			//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
			//SendMessage(hwnd, WM_DRAW, NULL, NULL);

			//if(wParam == ZOOM_OUT)
			//	SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)(-lpDataView->ptCursor.x), (LPARAM)(-lpDataView->ptCursor.y));
			//else
			//	SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)lpDataView->ptCursor.x, (LPARAM)lpDataView->ptCursor.y);

			if(lpDataView->flagActive){
				sprintf(str, "Zoom: %d%c", lpDataView->zoom*100, '%');
				SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
			}
		
		}
		return TRUE;
	case WM_SETHWND:
		{
			switch(wParam)
			{
			case HWND_DATAMANAGER:
				lpDataView->hDataManager = (HWND) lParam;
				return TRUE;
			case HWND_OWNER:
				lpDataView->hOwner = (HWND) lParam;

				//Set This DataView as Active After Registration
				SendMessage(lpDataView->hOwner, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);
				SetActiveMDI(lpDataView->hOwner, hwnd);
				return TRUE;
			default:
				return FALSE;
			}
		}
		break;
	case DV_SETVIEWPOS:
		{

		}
		break;
	case DV_GETVIEWPOS:
		{
		}
		break;
	case DV_UPDATEVIEWPOS:
		{
			return OnUpdateViewPosDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	//case DV_GETVIEWPOS:
	//	{
	case WM_GETHWND:
		{
			switch(wParam)
			{
			case HWND_DATAMANAGER:
				return (LRESULT) lpDataView->hDataManager;
			default:
				break;
			}
		}
		break;
	case WM_SETFRAMEFOCUS:
		{
			OnSetFrameFocusDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_COMMAND:
		{
			OnCommandDataView(lpDataView, hwnd, wParam, lParam);	
		}
        break;
	case WM_MOUSEWHEEL:
		{
			OnMouseWheelDataView(lpDataView, hwnd, wParam, lParam);	
		}
		break;
	case WM_MOUSEMOVE:
		{
			OnMouseMoveDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetFocus(hwnd);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			
     		memcpy(&lpDataView->ptGrab, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
		}
		break;
	case WM_LBUTTONUP:
		{
			DWORD nPt;
			SHORT keyState;
			POINT pt;

			nPt = lpDataView->nPt;

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			if(lpDataView->tool == TOOL_TRACKPOINT){
				//memcpy(&pt, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
				lpDataView->ptFrame[lpDataView->nPt] = lpDataView->fStack;
				
				lpDataView->pt[nPt].x = (pt.x + lpDataView->hPos - 0)/lpDataView->zoom;
				lpDataView->pt[nPt].y = (pt.y + lpDataView->vPos - 0)/lpDataView->zoom;
				lpDataView->pt[nPt].x = lpDataView->pt[nPt].x - 1;
				lpDataView->pt[nPt].y = lpDataView->pt[nPt].y + 1;
				lpDataView->nPt++;
				SendMessage(hwnd, WM_DRAW, NULL, NULL);

				CalculateParticleVelocity(lpDataView, hwnd, wParam, lParam);
			}

			if(lpDataView->tool == TOOL_MEASUREDIST/*lpDataView->isMeasureDistance*/){

				if(lpDataView->nPtMeasure > 1){
					lpDataView->nPtMeasure = 0;

					SendMessage(hwnd, WM_DRAW, NULL, NULL);
				}

				DWORD nPtMeasure = lpDataView->nPtMeasure;
				//memcpy(&pt, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
				lpDataView->ptMeasure[nPtMeasure].x = (pt.x + lpDataView->hPos)/lpDataView->zoom;
				lpDataView->ptMeasure[nPtMeasure].y = (pt.y + lpDataView->vPos)/lpDataView->zoom;
				lpDataView->ptMeasure[nPtMeasure].x = lpDataView->ptMeasure[nPtMeasure].x-1;
				lpDataView->ptMeasure[nPtMeasure].y = lpDataView->ptMeasure[nPtMeasure].y+1;
				lpDataView->nPtMeasure++;
				SendMessage(hwnd, WM_DRAW, NULL, NULL);

				CalculateDistance(lpDataView, hwnd, wParam, lParam);

			}

		}
		break;
	case WM_KEYDOWN:
		{
			OnKeyDownDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_SETCURSOR:
		{
			return OnSetCursorDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_MOUSELEAVE:
		{
			return OnMouseLeaveDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_TIMER:
		{
			return OnTimerDataView(lpDataView, hwnd, wParam, lParam);
		}
	case WM_SIZE:
		{
			OnSizeDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			OnCreateDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	case WM_NCCREATE:
		{
			//lpDataView = (DATAVIEW *)calloc(1, sizeof(DATAVIEW));
			lpDataView = (DATAVIEW *)malloc(sizeof(DATAVIEW));
			memset(lpDataView, NULL, sizeof(DATAVIEW));
			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpDataView);
			if(!lpDataView){
				MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "DataView Error!", MB_OK);
			}
		}
		break;
	case WM_CLOSE:
		{
			OnCloseDataView(lpDataView, hwnd, wParam, lParam);
		}
		break;
	default:
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}

	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

DWORD InitializeTabControl(HWND hTabControl)
{

	TCITEM tcitem;
	HIMAGELIST hImgList;

	memset(&tcitem, NULL, sizeof(TCITEM));
	memset(&hImgList, NULL, sizeof(HIMAGELIST));

	hImgList = ImageList_Create(18, 18, ILC_COLOR32, 3, 1);

	tcitem.mask = TCIF_TEXT|TCIF_IMAGE;


	tcitem.pszText = "Stack";
	tcitem.iImage = 0;
	TabCtrl_InsertItem(hTabControl, 1, &tcitem);

	tcitem.pszText = "Filters";
	tcitem.iImage = -1;
	TabCtrl_InsertItem(hTabControl, 1, &tcitem);

	tcitem.pszText = "Display";
	tcitem.iImage = -1;
	TabCtrl_InsertItem(hTabControl, 2, &tcitem);

	ImageList_Destroy(hImgList);

	return TRUE;

}

DWORD CalculateParticleVelocity(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	DWORD dw = NULL;
	CHAR str[MAX_STRING*2];

	HWND hText;
	hText = (HWND) SendMessage(lpDataView->hDataManager, WM_GETHWND, (WPARAM) HWND_TEXT, NULL);

	FLOAT xUmPerPixel = ((FLOAT)lpDataView->xFOV / (FLOAT)lpDataView->wStack);
	FLOAT yUmPerPixel = ((FLOAT)lpDataView->yFOV / (FLOAT)lpDataView->hStack);

	DWORD p = lpDataView->nPt-1;
	DWORD pFrame = lpDataView->ptFrame[p];

	TIMESTAMP t1;
	TIMESTAMP t2;

	FLOAT deltaTime = NULL;
	FLOAT deltaDistance = NULL;
	FLOAT delX = NULL;
	FLOAT delY = NULL;


	PDMFRAMEHEADER pdmfh = {0};

	//Get Time Stamps
	SendMessage(lpDataView->hDataManager, WM_GETFRAMEHEADER, (WPARAM) lpDataView->ptFrame[p], (LPARAM) &pdmfh);
	memcpy(&t1, &pdmfh.timeStamp, sizeof(TIMESTAMP));
	SendMessage(lpDataView->hDataManager, WM_GETFRAMEHEADER, (WPARAM) lpDataView->ptFrame[p-1], (LPARAM) &pdmfh);
	memcpy(&t2, &pdmfh.timeStamp, sizeof(TIMESTAMP));


	FLOAT xPos = xUmPerPixel*lpDataView->pt[p].x;
	FLOAT yPos = yUmPerPixel*lpDataView->pt[p].y;

	//sprintf(str, "Position: %3d: (%3.2f, %3.2f)", p, xPos, yPos);
	//InsertNewLine(hText, str);

	if(p > 0){
		deltaTime = Subtract(t1, t2);
		delX = (xUmPerPixel*lpDataView->pt[p].x - yUmPerPixel*lpDataView->pt[p-1].x);
		delY = (xUmPerPixel*lpDataView->pt[p].y - yUmPerPixel*lpDataView->pt[p-1].y);
		deltaDistance = sqrt( (delX*delX) + (delY*delY) );

		sprintf(str, "%3d: (%3.2f, %3.2f)\t\t%3.3f\t%3.3f\t%3.3f", p, xPos, yPos, (deltaDistance/(deltaTime/1000)), deltaDistance, deltaTime);
		InsertNewLine(hText, str);

		if((deltaDistance / deltaTime) < 0.1)
			deltaDistance = deltaDistance;
	}


	return dw;
}

DWORD CalculateDistance(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	DWORD dw = NULL;

	CHAR str[MAX_STRING];

	FLOAT deltaDistance = NULL;
	FLOAT delX = NULL;
	FLOAT delY = NULL;

	if(lpDataView->nPtMeasure == 2){
		delX = (lpDataView->ptMeasure[0].x - lpDataView->ptMeasure[1].x);
		delY = (lpDataView->ptMeasure[0].y - lpDataView->ptMeasure[1].y);
		deltaDistance = sqrt( (delX*delX) + (delY*delY) );
		//deltaDistance = (deltaDistance / lpDataView->zoom);


		sprintf(str, "Distance: %3.2f pixels", deltaDistance);
		ThrowMessage(hwnd, str);
		//sprintf(str, "Distance: %3.2f pixels, 
	}

	return dw;
}

LRESULT OnAddFileListDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	UINT f;
	UINT nPaths;
	CHAR strFileName[MAX_STRING];
	FILEPATHSET *fps;
	//fps = (FILEPATHSET *) calloc(1, sizeof(FILEPATHSET));
	fps = (FILEPATHSET *) malloc(sizeof(FILEPATHSET));
	memset(fps, NULL, sizeof(FILEPATHSET));

	if(fps && wParam){
		memcpy(fps, (VOID *) wParam, sizeof(FILEPATHSET));
		nPaths = fps->nPaths;

		for(f = 0; f < nPaths; f++){
			FileNameFromPath(MAX_STRING, fps->filePath[f].lpstr, strFileName);
			SendMessage(lpDataView->hDataBar, CB_ADDSTRING, NULL, (LPARAM) strFileName);
		}
		SendMessage(lpDataView->hOwner, DB_ADDSTRINGSET, (WPARAM)fps, NULL);
		SendMessage(lpDataView->hDataBar, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);

		memcpy((VOID *)&lpDataView->fps,(VOID *)fps, sizeof(FILEPATHSET));
		free(fps);
	}

	return TRUE;
}

LRESULT OnCloseDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	if(lpDataView->flagBlock){
		lpDataView->update.upQuit = TRUE;
		return TRUE;
	}
		
	HCURSOR hCursor;
	hCursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(hCursor);

	ThrowMessage(hwnd, "Window Destoryed: DataView");
	SendMessage(lpDataView->hDataManager, WM_CLOSE, NULL, NULL);

	//Clear Data Tab File Entries
	memset(&lpDataView->fps, NULL, sizeof(FILEPATHSET));
	SendMessage(lpDataView->hOwner, DB_ADDSTRINGSET, (WPARAM)&lpDataView->fps, NULL);

	SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) 0, NULL);

	//SetScrollRange
	//HWND hActive = (HWND) SendMessage(lpDataView->hOwner, WM_GETHWND, (WPARAM) HWND_ACTIVE, NULL);
	//if(!hActive){
	//	SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), 0);
	//	SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), 0);
	//}

	SendMessage(lpDataView->hOwner, PBM_SETRANGE, NULL, MAKELONG(0, 1));
	SendMessage(lpDataView->hOwner, PBM_SETPOS, NULL, NULL);

	KillTimer(hwnd, lpDataView->idTimer);

	if(lpDataView->hbmp)
	{
		if(!DeleteObject(lpDataView->hbmp)){
			ThrowMessage(hwnd, "DeleteObject() Returned NULL");
			ThrowLastError(hwnd);
		}
		//Destory Display Surface
	}
	if(lpDataView->hdcMem)
		DeleteDC(lpDataView->hdcMem);
	if(lpDataView->hdcDisp)
		DeleteDC(lpDataView->hdcDisp);

	if(lpDataView) free(lpDataView);

	DestroyWindow(hwnd);

	return TRUE;

}

LRESULT OnKeyDownDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	POINT pt;
	GetCursorPos(&pt);
    switch (wParam) 
    { 
        case VK_LEFT: 
            // Process the LEFT ARROW key. 
			SetCursorPos(pt.x-1, pt.y);
            break; 
         case VK_RIGHT: 
            // Process the RIGHT ARROW key. 
			 SetCursorPos(pt.x+1, pt.y);
            break; 
        case VK_UP:   
            // Process the UP ARROW key. 
			SetCursorPos(pt.x, pt.y-1);
            break; 
        case VK_DOWN:   
            // Process the DOWN ARROW key.  
			SetCursorPos(pt.x, pt.y+1);
            break;
		case VK_RETURN:
			if(lpDataView->isParticleTrack){
				DWORD nPt;
				nPt = lpDataView->nPt;
				memcpy(&pt, &Point(pt.x, pt.y), sizeof(POINT));
				lpDataView->ptFrame[lpDataView->nPt] = lpDataView->fStack;
				
				lpDataView->pt[nPt].x = pt.x + lpDataView->hPos/* - 2*/;
				lpDataView->pt[nPt].y = pt.y + lpDataView->vPos/* - 1*/;
				lpDataView->nPt++;
				SendMessage(hwnd, WM_PAINT, NULL, NULL);

				CalculateParticleVelocity(lpDataView, hwnd, wParam, lParam);
			}
			break;				
		case VK_ESCAPE:
			if(lpDataView->tool == TOOL_TRACKPOINT){
				if(MessageBox(hwnd, "Disable Tracking Tool?", "DataView: Tracking Tool", MB_OKCANCEL) == IDOK){
					HWND hText;
					CHAR str[MAX_STRING];

					
					lpDataView->nPt = 0;
					lpDataView->nParticles++;
					
					hText = (HWND) SendMessage(lpDataView->hDataManager, WM_GETHWND, (WPARAM) HWND_TEXT, NULL);

					sprintf(str, "**********Particle %4d**********", lpDataView->nParticles);
					InsertNewLine(hText, str);

					SetToolDataView(hwnd, TOOL_CURSOR);

					SendMessage(hwnd, WM_DRAW, NULL, NULL);
				}
			}
			else
				SetToolDataView(hwnd, TOOL_CURSOR);
			break;
		case VK_D:
			if(lpDataView->tool != TOOL_MEASUREDIST){
				SetToolDataView(hwnd, TOOL_MEASUREDIST);
			}
			else{
				SetToolDataView(hwnd, TOOL_CURSOR);
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)10, (LPARAM)NULL);
			}

			break;
		case VK_T:
			if(lpDataView->tool != TOOL_TRACKPOINT){
				SetToolDataView(hwnd, TOOL_TRACKPOINT);
			}
			else{
				SetToolDataView(hwnd, TOOL_CURSOR);
			}
			break;
		case VK_Z:
			if(GetKeyState(VK_LCONTROL)){ 
				lpDataView->nPt--;

				if(lpDataView->nPt < 0)
					lpDataView->nPt = 0;
			
				SendMessage(hwnd, WM_DRAW, NULL, NULL);
			}
			break;
		case VK_S:
			if(GetKeyState(VK_LCONTROL)&0x8000){ 
				SendMessage(hwnd, WM_COMMAND,(WPARAM) ID_FILE_SAVEAS, (LPARAM) NULL);
			}
			else{
				//lpDataView->hPos++;
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)NULL, (LPARAM)10);
			}
			break;
		case VK_W:
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)NULL, (LPARAM)-10);
			break;
		case VK_A:
				//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)-10, (LPARAM)NULL);
			break;
		case VK_ADD:
			//If the high-order bit is 1, the key is down
			if(GetKeyState(VK_LCONTROL)&0x8000){
				//Zoom In

				if(lpDataView->flagBlock){
					lpDataView->update.upZoom = lpDataView->zoom + 1;
					return TRUE;
				}
				
				SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_IN, NULL);
				//INT zoom = NULL;
				//lpDataView->zoom++;				
				//if(lpDataView->zoom > MAX_ZOOM)
				//	lpDataView->zoom = MAX_ZOOM;

				//zoom = lpDataView->zoom;
				//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
				//SendMessage(hwnd, WM_DRAW, NULL, NULL);
				//SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);
			}
			else{
				SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
				SendMessage(lpDataView->hDataBar, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
			}
			break;
		case VK_SUBTRACT:
			//If the high-order bit is 1, the key is down
			if(GetKeyState(VK_LCONTROL)&0x8000)
			{
				//Zoom Out
				if(lpDataView->flagBlock){
					lpDataView->update.upZoom = lpDataView->zoom - 1;
					return TRUE;
				}

				SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_OUT, NULL);

				//INT zoom = NULL;
				//lpDataView->zoom--;
				//if(lpDataView->zoom < 1)
				//	lpDataView->zoom = 1;

				//zoom = lpDataView->zoom;
				//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
				//SendMessage(hwnd, WM_DRAW, NULL, NULL);
				//SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);

			}
			else
			{
				SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack-1, NULL);
				SendMessage(lpDataView->hDataBar, WM_SETFRAMEFOCUS, lpDataView->fStack-1, NULL);
			}
			break;
        default: 
            break; 
    } 

	return lResult;
}

LRESULT OnMDIActivateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hChild;
	HWND hActive;
	HWND hDeActive;
	UINT enableFlag;
	hChild = (HWND)wParam;
	hActive = (HWND) lParam;
	hDeActive = (HWND) wParam;

	CHAR str[MAX_STRING];

	BringWindowToTop(hwnd);

	if(hChild){
		if(hwnd == hActive){

			lpDataView->flagActive = TRUE;
			HWND hParent = GetParent(hwnd);
			hParent = GetParent(hParent);
			SendMessage(lpDataView->hOwner, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);
			SendMessage(lpDataView->hDataManager, WM_SETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) hwnd);

			SendMessage(hParent, DB_ADDSTRINGSET, (WPARAM)&lpDataView->fps, NULL);
			SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);

			//SetScrollRange
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), lpDataView->fStart);
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), lpDataView->fStop);

			SendMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);

			sprintf(str, "Zoom: %d%c", lpDataView->zoom*100, '%');
			SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
		}
		else
			lpDataView->flagActive = FALSE;

		enableFlag = MF_ENABLED;
	}
	else
	{
		enableFlag = MF_DISABLED;
	}

	return TRUE;
}

LRESULT OnUpdateViewPosDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	RECT rcView = {0};

	INT hAdd = NULL;
	INT vAdd = NULL;

	INT hPos = NULL;
	INT vPos = NULL;

	INT wView = NULL;
	INT hView = NULL;

	hAdd = (INT) wParam;
	vAdd = (INT) lParam;

	//if(!hAdd && !vAdd)	return TRUE;

	GetClientRect(hwnd, &rcView);
	wView = rcView.right - rcView.left;
	hView = rcView.bottom - rcView.top;


	hPos = lpDataView->hPos + hAdd;
	vPos = lpDataView->vPos + vAdd;

	if(hPos < 0)								hPos = 0;
	if((hPos + wView) > lpDataView->wStack)		hPos = (lpDataView->wStack - wView);

	if(vPos < 0)								vPos = 0;
	if((vPos + hView) > lpDataView->hStack)		vPos = (lpDataView->hStack - hView);

	lpDataView->hPos = hPos;
	lpDataView->vPos = vPos;
	
	SendMessage(hwnd, WM_PAINT, NULL, NULL);

	return TRUE;
}

LRESULT OnSaveAsDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	CHAR str[MAX_PATH];
	CHAR filePath[MAX_PATH];

	OPENFILENAME ofn;
	memset(&ofn, NULL, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = (LPSTR) str;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = sizeof(filePath);
	ofn.lpstrFilter = TEXT("BMP(*.bmp)\0*.bmp\0\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_EXPLORER;

	GetSaveFileName(&ofn);

	sprintf(filePath, "%s.bmp", str);

	SaveBMP(lpDataView->wStack, lpDataView->hStack, filePath, lpDataView->pixBmp);

	return lResult;

}

LRESULT OnPaintDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	HDC hdc;
	HDC hdcWnd;
	HDC hdcPreview;
	RECT rect;
	RECT rcPreview;
	PAINTSTRUCT ps;
	PAINTSTRUCT psPreview;

	INT xPaint = NULL;
	INT yPaint = NULL;

	INT wView = NULL;
	INT hView = NULL;

	hdcWnd = BeginPaint(hwnd, &ps);
	//hdcPreview = BeginPaint(lpDataView->hPreview, &psPreview);


	if(lpDataView){
		if(lpDataView->hdcMem){		
			hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rect);
			wView = rect.right-rect.left;
			hView = rect.bottom-rect.top;

			//if(lpDataView->wStack < wView || lpDataView->wStack < hView){
			//	SendMessage(hwnd, WM_ERASEBKGND, (WPARAM)hdc/*NULL*/, TRUE);

			//	if(lpDataView->wStack < wView)
			//		xPaint = (FLOAT)(wView-lpDataView->wStack)/(FLOAT)2;
			//	else
			//		xPaint = -lpDataView->hPos;

			//	if(lpDataView->hStack < hView)
			//		yPaint = -(FLOAT)(hView-lpDataView->hStack)/(FLOAT)2;
			//	else
			//		yPaint = -lpDataView->vPos;

			//}
			//else{
				xPaint = -lpDataView->hPos;
				yPaint = -lpDataView->vPos;
			//}

			BitBlt(hdc, 
				/*-lpDataView->hPos*/xPaint, 
				/*-lpDataView->vPos*/yPaint, 
				rect.right + lpDataView->hPos, 
				rect.bottom + lpDataView->vPos, 
				lpDataView->hdcMem, 
				0, 0, SRCCOPY);

			ReleaseDC(hwnd, hdc);
		}
		else
			ThrowMessage(hwnd, "DataView: Lost HDC");
	}

	EndPaint(hwnd, &ps);

	return lResult;

}

LRESULT OnSetToolDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;
	HWND hParticleData = NULL;

	hParticleData = (HWND) SendMessage(lpDataView->hDataManager, WM_GETHWND, HWND_TEXT, NULL);

	lpDataView->tool = (DWORD) lParam;

	if(lpDataView->tool == TOOL_TRACKPOINT)
		ShowWindow(hParticleData, SW_SHOW);
	else
		ShowWindow(hParticleData, SW_HIDE);

	switch(lpDataView->tool)
	{
		case TOOL_CURSOR:
			ThrowMessage(hwnd, "DataView: Cursor Tool Enabled");
			break;
		case TOOL_HAND:
			break;
		case TOOL_CROSS:
			break;
		case TOOL_MEASUREDIST:
			ThrowMessage(hwnd, "DataView: Distance Tool Enabled");
			break;
		case TOOL_TRACKPOINT:
			ThrowMessage(hwnd, "DataView: Track Tool Enabled");
			break;
		default:
			break;
	}

	SendMessage(hwnd, WM_DRAW, NULL, NULL);
	SendMessage(hwnd, WM_PAINT, NULL, NULL);

	return lResult;

}

LRESULT OnSetFrameFocusDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(lpDataView->flagBlock)
		return (LRESULT) (-1);

	if((INT)wParam >= lpDataView->fStart && (INT)wParam <= lpDataView->fStop){
		lpDataView->fStack = (INT) wParam;
		if(lpDataView->flagActive){
			lpDataView->tStart = clock();
			PostMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
			PostMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);

			//SendMessage(lpDataView->hOwner, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
			//SendMessage(lpDataView->hOwner, CB_SETCURSEL, (WPARAM) (lpDataView->fStack-lpDataView->fStart), NULL);
		}
		PostMessage(lpDataView->hDataManager, WM_GETFRAME, (WPARAM) lpDataView->fStack, (LPARAM) FS_RAW);
	}

	return lpDataView->fStack;
}

LRESULT OnCommandDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;
	DWORD sel = NULL;
	CHAR str[MAX_STRING] = {0};

    switch(LOWORD(wParam))
    {
        case ID_FILE_SAVEAS:
			OnSaveAsDataView(lpDataView, hwnd, wParam, lParam);
			break;
		case ID_FILE_CLOSE:
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;
		case CBN_SELCHANGE:
			sel = SendMessage((HWND) lParam, CB_GETCURSEL, NULL, NULL); 
			sel = sel;
			break;
		case ID_SAVEAS_BMP:
			ThrowMessage(hwnd, "Saving .bmp");
			SendMessage(lpDataView->hDataManager, WM_COMMAND, wParam, lParam);
			ThrowMessage(hwnd, ".bmp Saved.");
			break;
		case ID_TOOLS_PARTICLETRACK:
			lpDataView->isParticleTrack = !lpDataView->isParticleTrack;

			if(lpDataView->isParticleTrack)
				ThrowMessage(hwnd, "DataView: Particle Track Enabled");
			else
				ThrowMessage(hwnd, "DataView: Particle Track Disabled");

			break;
		case DVN_SETALGORITHM:
				lpDataView->idAlgorithm = lParam;
			break;
		case DVN_SETALGORITHM2:
				UINT len;
				len = strlen((CHAR *)lParam);
				memcpy(lpDataView->strAlgorithm, (VOID *)lParam, len*sizeof(CHAR));
				ThrowMessage(hwnd, str);
			break;
		case DVN_SETALGORITHMSTRING:
			
			CHAR strAlgorithm[MAX_STRING];
			CHAR strRoot[MAX_STRING];
			DWORD aParams[MAX_STRING];

			memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
			memset(strRoot, NULL, MAX_STRING*sizeof(CHAR));

			memcpy(strAlgorithm, (VOID *)lParam, MAX_STRING*sizeof(CHAR));
			GetAlgorithmParams(strAlgorithm, strRoot, lpDataView->paramList);
			sprintf(str, "DataView Algorithm Selected: %s", strAlgorithm);
			ThrowMessage(hwnd, str);

			if(!strcmp(strAlgorithm, STR_GRAYSCALE))
				lpDataView->idAlgorithm = IDA_NONE;
			else if(!strcmp(strAlgorithm, STR_IMAGESUBTRACT))
				lpDataView->idAlgorithm = IDA_IMAGESUBTRACT;
			else if(!strcmp(strRoot, STR_GRADIENT))
				lpDataView->idAlgorithm = IDA_GRADIENTPLOW;
			else if(!strcmp(strAlgorithm, STR_LOG1X))
				lpDataView->idAlgorithm = IDA_LOG1X;
			else if(!strcmp(strAlgorithm, STR_LOG2X))
				lpDataView->idAlgorithm = IDA_LOG2X;
			else if(!strcmp(strRoot, STR_STRETCH))
				lpDataView->idAlgorithm = IDA_STRETCH;
			else if(!strcmp(strRoot, STR_WINDOW))
				lpDataView->idAlgorithm = IDA_WINDOW;
			else if(!strcmp(strRoot, STR_PARTICLETRACK))
				lpDataView->idAlgorithm = IDA_PARTICLETRACK;
			else if(!strcmp(strRoot, STR_GRADIENT2))
				lpDataView->idAlgorithm = IDA_GRADIENT2;
			else if(!strcmp(strRoot, STR_GAUSSBLUR))
				lpDataView->idAlgorithm = IDA_GAUSSBLUR;
			else 
				lpDataView->idAlgorithm = lpDataView->idAlgorithm;

			SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
			
			break;
		case DVN_UPDATE:
			PostMessage(lpDataView->hDataManager, WM_GETFRAME, (WPARAM) lpDataView->fStack, (LPARAM) FS_RAW);
			break;
		case DVN_SETXFOV:
			lpDataView->xFOV = (FLOAT) lParam;
			sprintf(str, "DataView: xFOV = %4.2fum", lpDataView->xFOV);
			ThrowMessage(hwnd, str);
			break;
		case DVN_SETYFOV:
			lpDataView->yFOV = (FLOAT) lParam;
			sprintf(str, "DataView: yFOV = %4.2fum", lpDataView->yFOV);
			ThrowMessage(hwnd, str);
			break;
		case DVN_SETZFOV:
			lpDataView->zFOV = (FLOAT) lParam;
			sprintf(str, "DataView: zFOV = %4.2fum", lpDataView->zFOV);
			ThrowMessage(hwnd, str);
			break;
		case DVN_SETTOOL:
			OnSetToolDataView(lpDataView, hwnd, wParam, lParam);
			break;
		case DVN_SETSTARTFRAME:
			lpDataView->fStart = (INT) lParam;
			if(lpDataView->fStack < lpDataView->fStart || lpDataView->fStack > lpDataView->fStop)
				lpDataView->fStack = lpDataView->fStart;
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTART), lParam);
			break;
		case DVN_SETSTOPFRAME:
			lpDataView->fStop = (INT) lParam;
			SendMessage(lpDataView->hOwner, WM_COMMAND, LOWORD(DBN_SETSTOP), lParam);
			break;
		case DVN_LOOP:
			lpDataView->flagLoop = !lpDataView->flagLoop;
			if(lpDataView->flagLoop)
				lpDataView->idTimer = SetTimer(hwnd, lpDataView->idTimer, lParam, NULL);
			else{
				KillTimer(hwnd, lpDataView->idTimer);
				lpDataView->idTimer = NULL;
			}

		default:
			break;
    }

	switch(HIWORD(wParam))
	{
		case CBN_SELCHANGE:
			{
				WPARAM fSelect = SendMessage((HWND) lParam, CB_GETCURSEL, NULL, NULL);
				SendMessage(hwnd, WM_SETFRAMEFOCUS, (fSelect + lpDataView->fStart), NULL);
			}
			return TRUE;
		default:
			break;
	}

	return lResult;
}

//wParam = Handle to the window that contains the cursor
//LOWORD(lParam) = hit-test code
//HIWORD(lParam) = identifier of mouse message	
LRESULT OnSetCursorDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;
	DWORD dw = NULL;
	HCURSOR hCursor = NULL;

	if((HWND) wParam == hwnd){

		switch(LOWORD(lParam))
		{
		case HTCLIENT:
			hCursor = LoadCursorDataView(lpDataView);
			break;
		case HTLEFT:
			hCursor = LoadCursor(NULL, IDC_SIZEWE);
			break;
		case HTRIGHT:
			hCursor = LoadCursor(NULL, IDC_SIZEWE);
			break;
		case HTBOTTOM:
			hCursor = LoadCursor(NULL, IDC_SIZENS);
			break;
		case HTTOP:
			hCursor = LoadCursor(NULL, IDC_SIZENS);
			break;
		case HTBOTTOMRIGHT:
			hCursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
		case HTBOTTOMLEFT:
			hCursor = LoadCursor(NULL, IDC_SIZENESW);
			break;
		case HTTOPLEFT:
			hCursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
		case HTTOPRIGHT:
			hCursor = LoadCursor(NULL, IDC_SIZENESW);
			break;
		default:
			hCursor = LoadCursor(NULL, IDC_ARROW);
			break;
		}
		SetCursor(hCursor);
		return TRUE;
	}
	else
		return FALSE;


	//return FALSE;
}

LRESULT OnSetFrameSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	//if(lpDataView->flagBlock)
	//	return FALSE;

	INT wStack;
	INT hStack;

	wStack = (INT) wParam;
	hStack = (INT) lParam;

	lpDataView->wStack = wStack;
	lpDataView->hStack = hStack;

	if(lpDataView->zoom == 1 && !lpDataView->wSource && !lpDataView->hSource){
		lpDataView->wSource = wStack;
		lpDataView->hSource = hStack;

		if(lpDataView->flagInit){
			lpDataView->pixSrc = (PIXEL *) calloc(lpDataView->wStack*lpDataView->hStack, sizeof(PIXEL));
			lpDataView->flagInit = FALSE;
		}
	}

	if(lpDataView->hbmp)
	{
		if(!DeleteObject(lpDataView->hbmp)){
			ThrowMessage(hwnd, "DeleteObject() Returned NULL");
			ThrowLastError(hwnd);
		}
		//Destory Display Surface
	}
	if(lpDataView->hdcMem)
		DeleteDC(lpDataView->hdcMem);
	if(lpDataView->hdcDisp)
		DeleteDC(lpDataView->hdcDisp);

	lpDataView->hbmp  = CreateDisplaySurface(hwnd, 
											&lpDataView->hdcMem, 
											&lpDataView->hdcDisp, 
											wStack, hStack, 
											&lpDataView->pixBmp);

	//if(lpDataView->pixSrc) free(lpDataView->pixSrc);
	//lpDataView->pixSrc = (PIXEL *) calloc(lpDataView->wStack*lpDataView->hStack, sizeof(PIXEL));


	//Temporary
	SendMessage(lpDataView->hOwner, WM_GETARCHIVEDIR, (WPARAM) lpDataView->strArchivePath, NULL);
	lpDataView->idDataManager = (DWORD) SendMessage(lpDataView->hDataManager, WM_GETHWND, HWND_ID, NULL);

	SendMessage(hwnd, WM_NCPAINT, NULL, NULL);

	return TRUE;
}


LRESULT OnMouseLeaveDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	HCURSOR hCursor = NULL;

	//hCursor = LoadCursor(NULL, IDC_ARROW);
	//SetCursor(hCursor);


	return lResult;
}

HCURSOR LoadCursorDataView(DATAVIEW *lpDataView)
{
	HCURSOR hCursor = NULL;

	switch(lpDataView->tool)
	{
	case TOOL_CURSOR:
		return LoadCursor(NULL, IDC_HAND);
	case TOOL_HAND:
		return LoadCursor(NULL, IDC_HAND);
	case TOOL_CROSS:
		return LoadCursor(NULL, IDC_ARROW);
	case TOOL_TRACKPOINT:
		return LoadCursor(NULL, IDC_CROSS);
	case TOOL_MEASUREDIST:
		return LoadCursor(NULL, IDC_CROSS);
	default:
		return LoadCursor(NULL, IDC_ARROW);
	}
}

DWORD SetToolDataView(HWND hwnd, DWORD tool)
{
	DWORD dw = NULL;

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETTOOL), (LPARAM) tool);

	return dw;
}

LRESULT OnDrawDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	//if(lpDataView->update.upZoom){
	//	ZoomDataView(lpDataView, hwnd, lpDataView->update.upZoomDir/*wParam*/, lpDataView->update.upZoom);
	//	lpDataView->update.upZoomDir = NULL;
//	lpDataView->update.upZoom = NULL;
	//}

	CreateThread(NULL, NULL, DataViewDrawProc, lpDataView, NULL, NULL);

	//INT i = NULL;

	//if(lpDataView->nPtMeasure && lpDataView->tool == TOOL_MEASUREDIST){
	//	INT p;
	//	for(p = 0; p < lpDataView->nPtMeasure; p++){
	//		DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, Point((lpDataView->ptMeasure[p].x), (lpDataView->ptMeasure[p].y)), RGB(255, 0, 0), 3);

	//	}
	//}

	//if(lpDataView->nPt && lpDataView->tool == TOOL_TRACKPOINT){
	//	INT p;
	//	for(p = 0; p < lpDataView->nPt; p++)
	//	{
	//		DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, Point(lpDataView->pt[p].x, lpDataView->pt[p].y), RGB(0, 0, 255), 3);
	//	}
	//}

	//if(lpDataView->ellipseSet.nEllipse)
	//{
	//	for(i = 0; i < lpDataView->ellipseSet.nEllipse; i++){
	//		DrawEllipse(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, lpDataView->ellipseSet.fellipse[i], RGB(0, 0, 255));
	//	}
	//}

	//if(lpDataView->zoom > 1){
	//	UINT w = lpDataView->wSource;
	//	UINT h = lpDataView->hSource;
	//	UINT wZoom = NULL;
	//	UINT hZoom = NULL;

	//	ZoomPIXEL(w, h, lpDataView->zoom, &wZoom, &hZoom, lpDataView->pixSrc, lpDataView->pixBmp);
	//}
	//else{
	//	memcpy(lpDataView->pixBmp, 
	//		lpDataView->pixSrc, 
	//		lpDataView->wStack*lpDataView->hStack*sizeof(PIXEL));
	//}

	//SendMessage(hwnd, WM_PAINT, NULL, NULL);

	//if(lpDataView->flagActive){
	//	CHAR str[MAX_STRING];
	//	INT tCalc = clock() - lpDataView->tStart;
	//	INT tDisp = clock() - lpDataView->tLast;
	//	lpDataView->tLast = clock();
	//	sprintf(str, "%.1f", ((FLOAT)(1000) / (FLOAT)(tDisp)));
	//	SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_FRAME, (LPARAM)str);
	//	sprintf(str, "%.1f", ((FLOAT)(1000) / (FLOAT)(tCalc)));
	//	SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_FRAMERATE, (LPARAM)str);
	//}

	return TRUE;



}

	
LRESULT OnShowFrameDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(lpDataView->flagBlock)
		return FALSE;

	//Load Primed Algorithm
	if(lpDataView->flagInit){
		FreeStringSet(&lpDataView->strProc);
		CallocStringSet(lpDataView->strProcBuffer.nStr, MAX_STRING, &lpDataView->strProc);
		MemcpyStringSet(&lpDataView->strProc, &lpDataView->strProcBuffer);
		lpDataView->selProc = lpDataView->selProcBuffer;
		lpDataView->flagInit = FALSE;
	}
		

	lpDataView->flagBlock = TRUE;

	INT w;
	INT h;
	HANDLE hThread = NULL;

	w = lpDataView->wSource;
	h = lpDataView->hSource;

	PDMFRAME pdmFrame;
	memcpy(&pdmFrame, (VOID *)wParam, sizeof(PDMFRAME));

	ALGORITHMTHREAD *at = (ALGORITHMTHREAD *)calloc(1, sizeof(ALGORITHMTHREAD));
	at->hwnd = hwnd;
	at->lpDataView = lpDataView;
	at->data = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	if(lParam){
		if(pdmFrame.pdmfh.typeData == DT_PIXEL){
			UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));
			PIXELtoUCHAR(w, h, (PIXEL *)lParam, img, NULL);
			pdmFrame.pdmfh.typeData = DT_UCHAR;
			memcpy(at->data, (VOID *)img, w*h*sizeof(UCHAR));
			//SendMessage(hwnd, WM_DRAW, NULL, NULL);
			//return TRUE;
			if(img)free(img);
		}
		else{
			memcpy(at->data, (VOID *)lParam, w*h*sizeof(UCHAR));
		}
	}


	lpDataView->nShown++;

	hThread = CreateThread(NULL, NULL, AlgorithmProc, at, NULL, NULL);

	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	return TRUE;

}

LRESULT OnMouseWheelDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	CHAR str[MAX_STRING];
	INT posWheel;

	SetFocus(hwnd);
	SetActiveWindow(hwnd);

	posWheel = (SHORT) HIWORD(wParam);

	if(GetKeyState(VK_LCONTROL)&0x8000){
		INT zoom = NULL;

		if(posWheel > 0){

			SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_IN, NULL);
			//if(lpDataView->flagBlock){
			//	lpDataView->update.upZoom = lpDataView->zoom + 1;
			//	return TRUE;
			//}

			//lpDataView->zoom++;
			//if(lpDataView->zoom > MAX_ZOOM){
			//	lpDataView->zoom = MAX_ZOOM;
			//	return TRUE;
			//}

			//zoom = lpDataView->zoom;
			//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
			//SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);

			//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)lpDataView->ptCursor.x, (LPARAM)lpDataView->ptCursor.y);


		}
		else{

			SendMessage(hwnd, DVM_ZOOM, (WPARAM) ZOOM_OUT, NULL);
			//if(lpDataView->flagBlock){
			//	lpDataView->update.upZoom = lpDataView->zoom - 1;
			//	return TRUE;
			//}

			//lpDataView->zoom--;
			//if(lpDataView->zoom < 1){
			//	lpDataView->zoom = 1;
			//	return TRUE;
			//}

			//zoom = lpDataView->zoom;
			//SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
			//SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);

			//SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)(-lpDataView->ptCursor.x), (LPARAM)(-lpDataView->ptCursor.y));
		}

		//if(lpDataView->flagActive){
		//	sprintf(str, "Zoom: %d%c", zoom*100, '%');
		//	SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_ZOOM, (LPARAM) str);
		//}

	}
	else
	{
		if(posWheel > 0){
			lpDataView->fStack--;
			if(lpDataView->fStack < lpDataView->fStart){
				lpDataView->fStack = lpDataView->fStart;
			}
		}
		else{			
			lpDataView->fStack++;
			if(lpDataView->fStack > lpDataView->fStop){
				lpDataView->fStack = lpDataView->fStop;
			}

			
		}
		SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
	}

	return TRUE;
}

LRESULT OnMouseMoveDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	RECT rcView;

	DWORD wView;
	DWORD hView;

	INT hPos;
	INT vPos;

	CHAR str[MAX_STRING];

	memcpy(&lpDataView->ptCursor, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
	memcpy(&pt, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
	if(wParam&MK_LBUTTON && lpDataView->flagActive){


		//GetClientRect(lpDataView->hView, &rcView);
		GetClientRect(hwnd, &rcView);

		wView = rcView.right - rcView.left;
		hView = rcView.bottom - rcView.top;

		//if(lpDataView->wStack > wView && lpDataView->hStack > hView){

			hPos = lpDataView->ptGrab.x - pt.x + lpDataView->hPos;
			vPos = lpDataView->ptGrab.y - pt.y + lpDataView->vPos;

			if(hPos < 0)								hPos = 0;
			if((hPos + wView) > lpDataView->wStack)		hPos = (lpDataView->wStack - wView);

			if(vPos < 0)								vPos = 0;
			if((vPos + hView) > lpDataView->hStack)		vPos = (lpDataView->hStack - hView);

			lpDataView->hPos = hPos;
			lpDataView->vPos = vPos;

			lpDataView->ptGrab.x = pt.x;
			lpDataView->ptGrab.y = pt.y;
			
			SendMessage(hwnd, WM_PAINT, NULL, NULL);
		//}

	}

	if(!lpDataView->flagActive)
		return TRUE;

	FLOAT x = (FLOAT)(lpDataView->hPos + pt.x)/(FLOAT)lpDataView->zoom;
	FLOAT y = (FLOAT)(lpDataView->vPos + pt.y)/(FLOAT)lpDataView->zoom;
	x = x - 1;
	y = y + 1;
	DWORD i = ((INT)(pt.y + lpDataView->vPos)*lpDataView->wStack)+(INT)(lpDataView->hPos + pt.x);

	sprintf(str, "(%.1f, %.1f)", x, y);
	SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_CURSOR, (LPARAM) str);

	if(lpDataView->pixBmp){
		if(i < lpDataView->wStack*lpDataView->hStack){
			sprintf(str, "(%d, %d, %d)", lpDataView->pixBmp[i].r, lpDataView->pixBmp[i].g, lpDataView->pixBmp[i].b);
			SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_PIXELVAL, (LPARAM) str);
		}
	}

	return TRUE;
}

LRESULT OnSizeDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	INT wFrame;
	INT hFrame;
	INT hClient;
	INT wClient;

	RECT rcFrame;
	RECT rcClient;

	//Frame
	GetClientRect(hwnd, &rcFrame);
	wFrame = rcFrame.right - rcFrame.left;
	hFrame = rcFrame.bottom - rcFrame.top;

	//Client
	GetClientRect(hwnd, &rcClient);
	GetClientRect(lpDataView->hView, &lpDataView->rcView);

	wClient = rcClient.right - rcClient.left;
	hClient = rcClient.bottom - rcClient.top;


	if(lpDataView && lpDataView->wStack && lpDataView->hStack){
		if(lpDataView->wStack > wClient && lpDataView->hStack > hClient){
			INT delX = lpDataView->wStack - lpDataView->hPos;
			INT delY = lpDataView->hStack - lpDataView->vPos;
			if(wClient > delX || hClient > delY){

				if(wClient < delX)
					delX = NULL;
				if(hClient < delY)
					delY = NULL;

				SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM) delX, (LPARAM)delY);
			}
		}
	}

	if(!wParam){
		SendMessage(hwnd, WM_PAINT, NULL, NULL);
	}


	return TRUE;
}

LRESULT OnTimerDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	if(lpDataView->flagLoop){
		if(lpDataView->fStack >= lpDataView->fStop)
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStart, NULL);
		else
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack+1, NULL);
	}

	return TRUE;
}

LRESULT OnCreateDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	DWORD wndProc;

	lpDataView->hwnd = hwnd;
	wndProc = NULL;

	//Removes Resizing Flicker
	LONG_PTR lptr;
	lptr = GetWindowLongPtr(hwnd, GWL_STYLE);
	lptr = lptr|WS_CLIPCHILDREN;
	
	if(!SetWindowLongPtr(hwnd, GWL_STYLE, lptr))
		ThrowMessage(hwnd, "DataView Error: SetWindowLongPtr(DataView)!!");

	//Temporary
	lpDataView->nParticles = 1;

	lpDataView->zoom = 1;
	lpDataView->flagInit = TRUE;

	return TRUE;

}

LRESULT ZoomDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	INT zoom = NULL;

	lpDataView->flagBlock = TRUE;

	if(wParam == ZOOM_IN){
		lpDataView->zoom++;
		if(lpDataView->zoom > MAX_ZOOM)
			lpDataView->zoom = MAX_ZOOM;

	}

	if(wParam == ZOOM_OUT){
		lpDataView->zoom--;
		if(lpDataView->zoom < 1)
			lpDataView->zoom = 1;

	}

	zoom = lpDataView->zoom;
	SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
	SendMessage(hwnd, WM_DRAW, NULL, NULL);

	if(wParam == ZOOM_OUT)
		SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)(-lpDataView->ptCursor.x), (LPARAM)(-lpDataView->ptCursor.y));
	else
		SendMessage(hwnd, DV_UPDATEVIEWPOS, (WPARAM)lpDataView->ptCursor.x, (LPARAM)lpDataView->ptCursor.y);

	return TRUE;

}

LRESULT OnInitAlgorithm(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CHAR str[MAX_STRING];
	DWORD selProc = (DWORD) wParam;
	STRINGSET *strSet = (STRINGSET *)lParam;

	//Check For Blocked Thread
	if(lpDataView->flagBlock){

		//Prime Algorithm Buffer
		lpDataView->flagInit = TRUE;
		if(strSet->nStr){
			FreeStringSet(&lpDataView->strProcBuffer);
			CallocStringSet(strSet->nStr, strSet->string[0].nChar, &lpDataView->strProcBuffer);
			MemcpyStringSet(&lpDataView->strProcBuffer, strSet);
			lpDataView->selProcBuffer = selProc;
		}
		return FALSE;
	}

	//memset(str, NULL, MAX_STRING*sizeof(CHAR));
	//memcpy(str, strSet->string[selProc].str, MAX_STRING*sizeof(CHAR));
	//RemoveStringWithin(str, MAX_STRING, '[', ']');

	lpDataView->selProc = selProc;

	if(strSet->nStr){
		FreeStringSet(&lpDataView->strProc);
		CallocStringSet(strSet->nStr, strSet->string[0].nChar, &lpDataView->strProc);
		MemcpyStringSet(&lpDataView->strProc, strSet);
		SortAlgorithmProcedures(selProc, strSet);

		//Remove This
		SendMessage(hwnd, WM_SETFRAMEFOCUS, (WPARAM) lpDataView->fStack, NULL);
	}

	return TRUE;
}

DWORD GetSystemTimeTest(HWND hwnd, DWORD nIterations, DWORD nSubIterations)
{
	SYSTEMTIME systemTime;
	CHAR strMessage[MAX_STRING];
	DWORD i = 0;
	DWORD s = 0;

	clock_t cStart = 0;

	for(i = 0; i < nIterations; i++){

		cStart = clock();
		for(s = 0; s < nSubIterations; s++){
		
			GetSystemTime(&systemTime);

		}
		sprintf(strMessage, "Iteration: %d - (%d ms)", i, clock()-cStart);
		ThrowMessage(hwnd, strMessage);
	}

	return TRUE;
}

LRESULT AlgorithmProcDataView(DATAVIEW *lpDataView, UCHAR *in, UCHAR *out)
{

	INT i = NULL;
	INT iProc = NULL;
	DWORD selProc = lpDataView->selProc;
	UINT w = lpDataView->wSource;
	UINT h = lpDataView->hSource;
	UINT iSrc = NULL;
	DWORD nProc = TRUE;
	DWORD nChar = NULL;
	FLOAT pList[MAX_PARAM];

	DWORD fRender = NULL;
	DWORD procRender = NULL;

	CHAR str[MAX_STRING];
	CHAR strIn[MAX_STRING];
	CHAR strAlgorithm[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));

	UCHAR *img = (UCHAR *) malloc(w*h*sizeof(UCHAR));

	nProc = lpDataView->strProc.nStr;

	if(!nProc)
		nProc = TRUE;

	//FLOAT **imgProc = (FLOAT **)calloc(/*(selProc+1)*/nProc, sizeof(FLOAT));
	FLOAT **imgProc = (FLOAT **)malloc(nProc*sizeof(FLOAT*));
	memset(imgProc, NULL, nProc*sizeof(FLOAT));

	for(i = 0; i < nProc; i++){
		//imgProc[i] = (FLOAT *)calloc(w*h, sizeof(FLOAT));
		imgProc[i] = (FLOAT *)malloc(w*h*sizeof(FLOAT));
		if(imgProc[i]){
			memset(imgProc[i], NULL, w*h*sizeof(FLOAT));
		}
	}

	UCHARtoFLOAT(w, h, in, (FLOAT *)imgProc[0]);

	for(i = 1; i <= selProc/*nProc*/; i++){	

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
		memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
		
		nChar = lpDataView->strProc.string[i].nChar;
		memcpy(str, lpDataView->strProc.string[i].str, nChar*sizeof(CHAR));
		ReadStringWithin(str, strIn, '[', ']');
		iProc = atoi(strIn);
		RemoveStringWithin(str, MAX_STRING, '[', ']');
		
		memset(pList, NULL, MAX_PARAM*sizeof(DWORD));
		GetAlgorithmParams(str, strAlgorithm, pList);

		iSrc = (UINT)pList[0];

		if(!strcmp(strAlgorithm, STR_GRADIENT)){
			GradientFirstOrder(w, h, pList[1], (FLOAT *)imgProc[iSrc], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_LOG)){
			LogOperator(w, h, imgProc[(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_LAPLACIAN)){
			Laplacian(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_RGB)){
			fRender = TRUE;
			procRender = i;
		}
		else if(!strcmp(strAlgorithm, STR_SOBEL)){
			GradientSobel(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CENTRALDIFFERENCE)){
			GradientCentralDifference(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STRETCH)){
			Stretch(w, h, pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_BOOLEANAND)){
			BooleanAND(w, h, imgProc[iSrc], imgProc[(UINT)pList[1]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_BOOLEAN)){
			Boolean(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_PAVLIDISCONTOUR)){
			PavlidisContour(w, h, imgProc[/*iSrc*/(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_WINDOW)){
			ClampData(w*h, pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_GAUSSBLUR)){
			GaussianBlur(w, h, (UINT)pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NONMAXIMUM)){
			NonMaximumSuppression(w, h, imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_INVERT)){
			InvertData(w*h,  imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_GETFRAME)){
			GetFrame(lpDataView->hDataManager, w, h, (DWORD)(lpDataView->fStack + pList[0]), (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SUBTRACT)){
			ImageSubtract(w, h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ADD)){
			Addition(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SQUARE)){
			Square(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MIP)){
			fRender = TRUE;
			procRender = i;
		}
		else if(!strcmp(strAlgorithm, STR_MEAN)){
			Mean(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STDDEV)){
			StdDev(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_VARIANCE)){
			Variance(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NORMVARIANCE)){
			NormalizedVariance(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CHAMFER)){
			Chamfer(w, h, imgProc[(UINT)pList[0]], imgProc[iProc]);
		}
		//+else if(!strcmp(strAlgorithm, STR_AVERAGE)){
			//AverageImage(w, h, imgProc[(UINT)pList[0]
		else
			i = i;

	}

	if(fRender){
		UINT nParam;
		memset(str, NULL, MAX_STRING*sizeof(UCHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(UCHAR));
		nChar = lpDataView->strProc.string[procRender].nChar;
		memcpy(str, lpDataView->strProc.string[procRender].str, nChar*sizeof(CHAR));
		nParam = GetAlgorithmParams(str, strAlgorithm, pList);
		if(nParam == 3)
			FLOATtoPIXEL(w, h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], lpDataView->pixSrc);
		if(nParam == 4){
			//FLOATtoPIXEL(w, h, (FLOAT *)imgProc[(UINT)pList[0]],(FLOAT *) imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], (FLOAT *)imgProc[(UINT)pList[3]], lpDataView->pixSrc);
			MaximumIntensityProjection(w, h, (FLOAT *)imgProc[(UINT)pList[0]],(FLOAT *) imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], (FLOAT *)imgProc[(UINT)pList[3]], lpDataView->pixSrc);
		}
	}
	else{
		ClampData(w*h, 0, 255, (FLOAT *)imgProc[selProc], img);
		UCHARtoPIXEL(w, h, (UCHAR *) img, lpDataView->pixSrc);
	}

	for(i = 0; i < nProc; i++){
		if(imgProc[i])free(imgProc[i]);
	}

	if(imgProc) free(imgProc);
	if(img) free(img);

	//SendMessage(lpDataView->hwnd, WM_DRAW, NULL, NULL);
	//lpDataView->flagBlock = FALSE;

	return TRUE;
}

LRESULT UpdateFromBlock(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	INT zoom = NULL;

	if(!lpDataView->flagBlock){
	
		if(lpDataView->update.upZoom){
			if(lpDataView->zoom < MAX_ZOOM && lpDataView->zoom > 0)
				lpDataView->zoom = lpDataView->update.upZoom;
		
			zoom = lpDataView->zoom;

			SendMessage(hwnd, WM_SETFRAMESIZE, lpDataView->wSource*zoom, lpDataView->hSource*zoom);
			SendMessage(hwnd, WM_SETFRAMEFOCUS, lpDataView->fStack, NULL);
			lpDataView->update.upZoom = NULL;
		}

	}

	return TRUE;
}


DWORD WINAPI DataViewDrawProc(LPVOID param)
{
	DATAVIEW *lpDataView = (DATAVIEW *)param;

	//if(lpDataView->update.upZoom)
	//{
	//	ZoomDataView(lpDataView, hwnd, wParam, lParam);

	lpDataView->flagBlock = TRUE;

	INT i = NULL;

	if(lpDataView->nPtMeasure && lpDataView->tool == TOOL_MEASUREDIST){
		INT p;
		for(p = 0; p < lpDataView->nPtMeasure; p++){
			DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, Point((lpDataView->ptMeasure[p].x), (lpDataView->ptMeasure[p].y)), RGB(255, 0, 0), 3);

		}
	}

	if(lpDataView->nPt && lpDataView->tool == TOOL_TRACKPOINT){
		INT p;
		for(p = 0; p < lpDataView->nPt; p++)
		{
			DrawCross(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, Point(lpDataView->pt[p].x, lpDataView->pt[p].y), RGB(0, 0, 255), 3);
		}
	}

	if(lpDataView->ellipseSet.nEllipse)
	{
		for(i = 0; i < lpDataView->ellipseSet.nEllipse; i++){
			DrawEllipse(lpDataView->wSource, lpDataView->hSource, lpDataView->pixSrc, lpDataView->ellipseSet.fellipse[i], RGB(0, 0, 255));
		}
	}

	if(lpDataView->zoom > 1){
		UINT w = lpDataView->wSource;
		UINT h = lpDataView->hSource;
		UINT wZoom = NULL;
		UINT hZoom = NULL;

		ZoomPIXEL(w, h, lpDataView->zoom, &wZoom, &hZoom, lpDataView->pixSrc, lpDataView->pixBmp);
	}
	else{
		memcpy(lpDataView->pixBmp, 
			lpDataView->pixSrc, 
			lpDataView->wStack*lpDataView->hStack*sizeof(PIXEL));
	}

	SendMessage(lpDataView->hwnd, WM_PAINT, NULL, NULL);

	if(lpDataView->flagActive){
		CHAR str[MAX_STRING];
		INT tCalc = clock() - lpDataView->tStart;
		INT tDisp = clock() - lpDataView->tLast;
		lpDataView->tLast = clock();
		sprintf(str, "%.1f", ((FLOAT)(1000) / (FLOAT)(tDisp)));
		SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_FRAME, (LPARAM)str);
		sprintf(str, "%.1f", ((FLOAT)(1000) / (FLOAT)(tCalc)));
		SendMessage(lpDataView->hOwner, RM_SETTEXT, SBP_FRAMERATE, (LPARAM)str);
	}

	lpDataView->flagBlock = FALSE;

	return TRUE;

}

#endif


//LRESULT OnShowFrameDataView(DATAVIEW *lpDataView, HWND hwnd, WPARAM wParam, LPARAM lParam)
//{
//	if(lpDataView->flagBlock)
//		return FALSE;
//
//	lpDataView->flagBlock = TRUE;
//
//	//Load Primed Algorithm
//	if(lpDataView->flagInit){
//		FreeStringSet(&lpDataView->strProc);
//		CallocStringSet(lpDataView->strProcBuffer.nStr, MAX_STRING, &lpDataView->strProc);
//		MemcpyStringSet(&lpDataView->strProc, &lpDataView->strProcBuffer);
//		lpDataView->selProc = lpDataView->selProcBuffer;
//		lpDataView->flagInit = FALSE;
//	}
//		
//	
//
//	INT w;
//	INT h;
//	HANDLE hThread = NULL;
//
//	w = lpDataView->wSource;
//	h = lpDataView->hSource;
//
//	PDMFRAME pdmFrame;
//	memcpy(&pdmFrame, (VOID *)wParam, sizeof(PDMFRAME));
//
//#ifdef DATAVIEWFILTER
//	ALGORITHMTHREAD *at = (ALGORITHMTHREAD *)calloc(1, sizeof(ALGORITHMTHREAD));
//	at->hwnd = hwnd;
//	at->lpDataView = lpDataView;
//	at->data = (UCHAR *) calloc(w*h, sizeof(UCHAR));
//
//	if(lParam){
//		if(pdmFrame.pdmfh.typeData == DT_PIXEL){
//			UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));
//			PIXELtoUCHAR(w, h, (PIXEL *)lParam, img, NULL);
//			pdmFrame.pdmfh.typeData = DT_UCHAR;
//			memcpy(at->data, (VOID *)img, w*h*sizeof(UCHAR));
//			if(img)free(img);
//		}
//		else{
//			memcpy(at->data, (VOID *)lParam, w*h*sizeof(UCHAR));
//		}
//	}
//	hThread = CreateThread(NULL, NULL, AlgorithmProc, at, NULL, NULL);
//	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
//#else
//	switch(pdmFrame.pdmfh.typeData){
//		case DT_PIXEL:
//			memcpy(lpDataView->pixSrc, pdmFrame.lpvoid, w*h*sizeof(PIXEL));
//			break;
//		case DT_UCHAR:
//			UCHARtoPIXEL(w, h, (UCHAR *)pdmFrame.lpvoid, (PIXEL *)lpDataView->pixSrc);
//			break;
//		default:
//			break;
//	}
//	//lpDataView->flagBlock = FALSE;
//	SendMessage(hwnd, WM_DRAW, NULL, NULL);
//	
//#endif
//	
//	lpDataView->nShown++;
//	
//
//	return TRUE;
//
//}


	//HWND hText;
	//CHAR str[MAX_STRING];

	////HWND hMDI = NULL;
	////HWND hDataView = NULL;
	////if(!lpDataView->hSend){
	////	hMDI = (HWND) SendMessage(lpDataView->hOwner, WM_GETHWND, HWND_MDI, NULL);
	////	hDataView = CreateMDIDataView(lpDataView->hOwner, "Mask");
	////}else{
	////	hDataView = lpDataView->hSend;
	////}

	//PDMFRAME pdmf = {0};
	//POLYGON polygon = {0};
	//UINT w = lpDataView->wSource;
	//UINT h = lpDataView->hSource;

	////if(!lpDataView->hSend){
	////	DataView_SetWidthHeight(hDataView, w, h);
	////	DataView_SetStartFrame(hDataView, 0);
	////}

	//AllocPDMFrame(&pdmf, w, h, DT_UCHAR);
	//polygon.point = (POINT *)malloc(lpDataView->ptSet.nPoints*sizeof(POINT));
	//polygon.vertex = (POINT *)malloc(lpDataView->ptSet.nPoints*sizeof(POINT));
	//PointSetToPolygon(&lpDataView->ptSet, &polygon);
	//FillPolygon(w, h, 255, &polygon, (UCHAR *)pdmf.lpvoid);
	//SendMessage(lpDataView->hSend, WM_SETMASK, (WPARAM) lpDataView->idMask, (LPARAM)&pdmf);
	////DataView_ShowFrame(hDataView, &pdmf);
	////ShowWindow(hDataView, SW_SHOW);
	//if(polygon.point){free(polygon.point);}
	//if(pdmf.lpvoid){free(pdmf.lpvoid);}


	//lpDataView->ptSet.nPoints = 0;
	//
	//SetToolDataView(hwnd, TOOL_CURSOR);

	//SendMessage(hwnd, WM_DRAW, NULL, NULL);
	//SendMessage(hwnd, WM_PAINT, NULL, NULL);