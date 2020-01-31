#include "stdafx.h"

#include "CUDAFunDll.h"

#include "ParticleTracker.h"
#include "ParticleTrackerDlg.h"
#include "RegisterWnd.h"
#include "ToolsWin32.h"
#include "DataView.h"
#include "Image.h"
#include "PDM.h"
#include "CreateThreadedWindow.h"

#include "ImageSubtract.h"
#include "Power.h"
#include "Stretch.h"
#include "Threshold.h"
#include "MorphOperators.h"
#include "WndLink.h"
#include "Statistics.h"
#include "Invert.h"
#include "Mask.h"
#include "Sum.h"
#include "PavlidisContour.h"

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
#include "AlphaBlend.h"
#include "Contour.h"
#include "Ellipse.h"
#include "Trajectory.h"
#include "Average.h"
#include "MarchingSquares.h"
#include "Threshold.h"
#include "FloodFill.h"

#include "TFPT.h"
#include "MotionCorrespondence.h"

#include "WinUser.h"

#include <vector>

DWORD OnCreateParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	lpPT->hwnd = hwnd;
	lpPT->flgGetRef = TRUE;


	AllocWndLink(&lpPT->wls, MAX_LINK);
	lpPT->iTrack = 3;
	Alloc(&lpPT->fpsF0, 100);
	Alloc(&lpPT->fpsF1, 100);
	Alloc(&lpPT->fpsF2, 100);
	Alloc(&lpPT->es, 500);
	Alloc(&lpPT->es0, 500);
	Alloc(&lpPT->es1, 500);
	Alloc(&lpPT->es2, 500);

	Alloc(&lpPT->ts, 1000, 100);

	FELLIPSE fe = {0};

	CallocStringSet(50, MAX_STRING, &lpPT->ssAlgorithm);
	Append(&lpPT->ssAlgorithm, "Particle Track (Dilation)");
	Append(&lpPT->ssAlgorithm, "[1]Window(0, 20, 1000)");
	Append(&lpPT->ssAlgorithm, "[2]Square(1)");
	Append(&lpPT->ssAlgorithm, "[3]Dilate(2)");
	Append(&lpPT->ssAlgorithm, "[4]Window(3, 50, 50000)");
	Append(&lpPT->ssAlgorithm, "[5]PavlidisContour(4)");

	return TRUE;
}


//Creates the multi-threaded particle tracker and the necessary dialog box to set tracking parameters
HWND CreateParticleTrackerSystem(HWND hOwner, CHAR *strTitle)
{
	DWORD lastError = NULL;
	HWND hParticleTracker = NULL;
	BOOL flgFree = NULL;
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "ParticleTracker");
		flgFree = TRUE;
	}

	SetLastError(NULL);
	
	hParticleTracker = (HWND) CreateWindowExThreaded(NULL,
									"ParticleTracker", 
									strTitle, 
									WS_OVERLAPPEDWINDOW,
									NULL,
									NULL,
									200,
									200,
									HWND_MESSAGE,
									NULL,
									NULL);
	if(!hParticleTracker){
		lastError = GetLastError();
	}

	SetHWND(hParticleTracker, hOwner, HWND_OWNER);

	HWND hTrackerDlg = NULL;
	HWND hFrame = NULL;

	hFrame = GetAncestor(hOwner, GA_ROOT);
	hTrackerDlg = (HWND) SendMessage(hFrame, WM_CREATEDIALOG, (WPARAM)IDD_LM_PARTICLETRACKER, (LPARAM)ParticleTrackerDlgProc);
	SendMessage(hTrackerDlg, WM_SETHWND, (WPARAM)HWND_PARTICLETRACKER, (LPARAM)hParticleTracker);
	ShowWindow(hTrackerDlg, SW_SHOW);
	
	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hParticleTracker;
}

//Creates just the multi-threaded particle tracker
HWND CreateParticleTracker(HWND hOwner, CHAR *strTitle)
{
	DWORD lastError = NULL;
	HWND hParticleTracker = NULL;
	BOOL flgFree = NULL;
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "ParticleTracker");
		flgFree = TRUE;
	}

	SetLastError(NULL);
	
	hParticleTracker = (HWND) CreateWindowExThreaded(NULL,
									"ParticleTracker", 
									strTitle, 
									WS_OVERLAPPEDWINDOW,
									NULL,
									NULL,
									200,
									200,
									HWND_MESSAGE,
									NULL,
									NULL);
	if(!hParticleTracker){
		lastError = GetLastError();
	}

	SetHWND(hParticleTracker, hOwner, HWND_OWNER);
	
	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hParticleTracker;
}

//HWND CreateParticleTracker(HWND hOwner, CHAR *strTitle)
//{
//	DWORD lastError = NULL;
//	HWND hDataFilter = NULL;
//	BOOL flgFree = NULL;
//	CHAR *str = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
//	memset(str, NULL, MAX_STRING*sizeof(CHAR));
//
//	if(!strTitle){
//		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
//		memset(str, NULL, MAX_STRING*sizeof(CHAR));
//		sprintf(str, "ParticleTracker");
//		flgFree = TRUE;
//	}
//
//	SetLastError(NULL);
//	
//	hDataFilter = (HWND) CreateWindowExThreaded(NULL,
//									"ParticleTracker", 
//									strTitle, 
//									WS_OVERLAPPEDWINDOW,
//									NULL,
//									NULL,
//									200,
//									200,
//									HWND_MESSAGE,
//									NULL,
//									NULL);
//	if(!hDataFilter){
//		lastError = GetLastError();
//	}
//
//	SetHWND(hDataFilter, hOwner, HWND_OWNER);
//	
//	if(flgFree){
//		if(strTitle){
//			free(strTitle);
//		}
//	}
//
//	return (HWND) hDataFilter;
//}

DWORD ParticleTracker_Enable(HWND hwnd, DWORD bEnable)
{

	SendMessage(hwnd, WM_TRACKERENABLE, (WPARAM) bEnable, NULL);

	return TRUE;
}

DWORD ParticleTracker_SetFrameSize(HWND hwnd, UINT w, UINT h)
{

	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM) h);

	return TRUE;
}

LRESULT OnCreateLinkParticleTracker(PARTICLETRACKER *lpParticleTrack, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hLink = (HWND)wParam;
	DWORD flgCallBack = (DWORD)lParam;
	AddLink(hwnd, hLink, &lpParticleTrack->wls, flgCallBack);

	return TRUE;
}

LRESULT OnInitAlgorithmParticleTrack(PARTICLETRACKER *lpParticleTrack, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CHAR str[MAX_STRING];
	DWORD selProc = (DWORD) wParam;
	STRINGSET *strSet = (STRINGSET *)lParam;


	lpParticleTrack->selProc = selProc;

	if(strSet->nStr){
		FreeStringSet(&lpParticleTrack->ssAlgorithm);
		CallocStringSet(strSet->nStr, strSet->string[0].nChar, &lpParticleTrack->ssAlgorithm);
		MemcpyStringSet(&lpParticleTrack->ssAlgorithm, strSet);
		SortAlgorithmProcedures(selProc, strSet);
	}

	return TRUE;
}

LRESULT OnTrackParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if(!lpPT->flgTrack)
		return FALSE;

	lpPT->flgBlock = TRUE;


	DWORD w = lpPT->w;
	DWORD h = lpPT->h;
	DWORD iSub = lpPT->iSub;

	UCHAR *img = (UCHAR *) lpPT->pdmf.lpvoid;
	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
	PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
	UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
	UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
	UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;


	if(lpPT->flgGetFlow){
		ParticleTracker_CalculateFlowMask(lpPT);
	}else if(lpPT->flgGetRef){
		ParticleTracker_CalculateReference(lpPT, lpPT->nReference);
	}else if(!lpPT->flgGetRef){
		ParticleTracker_Track(lpPT);
	}else{}

	lpPT->iSub = iSub;
	if(lpPT->flgShow == PTS_FILTER){
		INTtoUCHAR(w, h, imgINT, imgPrev);
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPrev);
	}else if(lpPT->flgShow == PTS_REFERENCE){
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfRef);
	}else if(lpPT->flgShow == PTS_STREAM){
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfIn);
	}else if(lpPT->flgShow == PTS_TRACKING){
		//AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgINT, imgPIXEL);
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
	}else if(lpPT->flgShow == PTS_MASK_LON){
		AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskLon, imgPIXEL);
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
	}else if(lpPT->flgShow == PTS_MASK_LAT){
		AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskLat, imgPIXEL);
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
	}else if(lpPT->flgShow == PTS_MASK_CHN){
		AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskChn, imgPIXEL);
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
	}else if(lpPT->flgShow == PTS_MASK_FLOW){
		DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfFlow);
	}else{}
	
	lpPT->flgBlock = FALSE;

	return TRUE;
}

LRESULT OnDataInParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if(!lpPT->flgTrack)
		return FALSE;

	if(lpPT->flgBlock)
		return FALSE;

	DWORD w = NULL;
	DWORD h = NULL;
	DWORD iSub = lpPT->iSub;
	PDMFRAME *pdmFrame = (PDMFRAME *)wParam;

	w = pdmFrame->pdmfh.width;
	h = pdmFrame->pdmfh.height;

	if(!lpPT->w ||!lpPT->h){
		ParticleTracker_SetFrameSize(hwnd, w, h);
		return FALSE;
	}
	//Should Assert Same Headers
	FrameCopy(lpPT->pdmfIn, (PDMFRAME) *pdmFrame);

	PostMessage(hwnd, WM_TRACK, NULL, NULL);

	return TRUE;
}

LRESULT OnDataDropParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!lpPT->flgTrack || lpPT->flgBlock){
		return FreeDrop((PDMFRAME *)wParam);
	}

	if(wParam){

		DWORD w;
		DWORD h;
		DWORD iSub = lpPT->iSub;
		PDMFRAME *pdmFrame = (PDMFRAME *)wParam;



		w = pdmFrame->pdmfh.width;
		h = pdmFrame->pdmfh.height;

		if(!lpPT->w || !lpPT->h){
			ParticleTracker_SetFrameSize(hwnd, w, h);
			return FALSE;
		}

		FrameCopy(lpPT->pdmfIn, (PDMFRAME)*pdmFrame);
	
		FreeDrop((PDMFRAME *)wParam);

		PostMessage(hwnd, WM_TRACK, NULL, NULL);
	}

	return TRUE;
}

DWORD ParticleTracker_CalculateFlowMask(PARTICLETRACKER *lpPT)
{
	
	DWORD w = NULL;
	DWORD h = NULL;
	DWORD iSub = lpPT->iSub;
	DWORD nFrame = lpPT->nReference;

	w = lpPT->pdmfDisp.pdmfh.width;
	h = lpPT->pdmfDisp.pdmfh.height;

	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
	INT *imgFlowINT = (INT *)lpPT->pdmfFlowINT.lpvoid;
	UCHAR *imgFlow = (UCHAR *)lpPT->pdmfFlow.lpvoid;


	if(lpPT->iAvg == 0){
		memset(imgFlow, 0, w*h*sizeof(UCHAR));
		memset(imgFlowINT, 0, w*h*sizeof(INT));
		//memcpy(imgRef, imgIn, w*h*sizeof(UCHAR));
		lpPT->flgShow == PTS_MASK_FLOW;
	}

	ImageSubtract(w, h, imgIn, imgRef, imgINT);
	WindowData(w*h, 0, 255, imgINT);
	Add(w*h, imgINT, imgFlowINT);
	INTtoUCHAR(w, h, imgFlowINT, imgFlow);
	
	
	lpPT->iAvg = lpPT->iAvg + 1;

	FrameCopy(lpPT->pdmfPrev, lpPT->pdmfIn);
	FrameCopy(lpPT->pdmfDisp, lpPT->pdmfRef);

	iSub = NULL;

	if(lpPT->iAvg > nFrame){
		lpPT->iAvg = 0;
		lpPT->flgGetFlow = FALSE;
		//lpPT->flgShow == PTS_FILTER;
		
	}

	return TRUE;
}

DWORD ParticleTracker_CalculateReference(PARTICLETRACKER *lpPT, DWORD nFrames)
{

	DWORD w = NULL;
	DWORD h = NULL;
	DWORD iSub = lpPT->iSub;

	w = lpPT->pdmfDisp.pdmfh.width;
	h = lpPT->pdmfDisp.pdmfh.height;

	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;

	if(lpPT->iAvg == 0){
		memcpy(imgRef, imgIn, w*h*sizeof(UCHAR));
		lpPT->flgShow == PTS_REFERENCE;
	}

	BlendSubtract(w, h, imgIn, imgRef, imgRef);
	
	lpPT->iAvg = lpPT->iAvg + 1;

	FrameCopy(lpPT->pdmfPrev, lpPT->pdmfIn);
	FrameCopy(lpPT->pdmfDisp, lpPT->pdmfRef);

	iSub = NULL;

	if(lpPT->iAvg > nFrames){
		lpPT->iAvg = 0;
		lpPT->flgGetRef = FALSE;
		lpPT->flgShow == PTS_FILTER;
	}

	


	return TRUE;
}

//DWORD ParticleTracker_CalculateReference(PARTICLETRACKER *lpPT, DWORD nFrames)
//{
//
//	DWORD w = NULL;
//	DWORD h = NULL;
//	DWORD iSub = lpPT->iSub;
//
//	w = lpPT->pdmfDisp.pdmfh.width;
//	h = lpPT->pdmfDisp.pdmfh.height;
//
//	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
//	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
//	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
//	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
//	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
//	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
//	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
//	//PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
//	//UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
//	//UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
//	//UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;
//
//	if(lpPT->iAvg == 0){
//		memcpy(imgRef, imgIn, w*h*sizeof(UCHAR));
//		lpPT->flgShow == PTS_REFERENCE;
//	}
//	
//
//	double num = (double)nFrames - (float) lpPT->iAvg;
//	FLOAT blend = 0.0;
//	
//	//blend = ((float)lpPT->iAvg-(float)nFrames);
//	blend = (float)num / (float)nFrames;
//	blend = abs(blend);
//	if(blend < 0)
//		blend = 0.0;
//	Blend(w*h, 1-blend, blend, imgIn, imgRef, imgRef);
//	Average(w*h, imgIn, imgRef, imgRef);
//	
//	lpPT->iAvg = lpPT->iAvg + 1;
//
//	FrameCopy(lpPT->pdmfPrev, lpPT->pdmfIn);
//	FrameCopy(lpPT->pdmfDisp, lpPT->pdmfRef);
//
//	iSub = NULL;
//
//	if(lpPT->iAvg > nFrames){
//		lpPT->iAvg = 0;
//		lpPT->flgGetRef = FALSE;
//		lpPT->flgShow == PTS_FILTER;
//	}
//
//	
//
//
//	return TRUE;
//}

DWORD BlendSubtract(DWORD w, DWORD h, UCHAR *imgI, UCHAR *imgII, UCHAR *imgOut)
{

	if(!imgI||!imgII||!imgOut)
		return FALSE;

	DWORD n = w*h;
	INT *imgINT = NULL;
	PDMFRAME pdmfINT = {0};
	AllocPDMFrame(&pdmfINT, w, h, DT_INT);

	imgINT = (INT *) pdmfINT.lpvoid;

	ImageSubtract(w, h, imgI, imgII, imgINT);

	AlphaComposite(w, h, imgI, imgII, imgINT, imgOut);

	FreePDM(&pdmfINT);



	return TRUE;
}

//DWORD ParticleTracker_CalculateReference(PARTICLETRACKER *lpPT)
//{
//
//	FLOAT stdDev = NULL;
//	FLOAT var = NULL;
//
//	DWORD w = NULL;
//	DWORD h = NULL;
//	DWORD iSub = lpPT->iSub;
//
//	w = lpPT->pdmfDisp.pdmfh.width;
//	h = lpPT->pdmfDisp.pdmfh.height;
//	//w = pdmf->pdmfh.width;
//	//h = pdmf->pdmfh.height;
//
//	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
//	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
//	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
//	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
//	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
//	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
//	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
//	//PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
//	//UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
//	//UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
//	//UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;
//
//	ImageSubtract(w, h, imgIn, imgPrev, imgINT);
//	Threshold(w, h, 1, imgINT);
//	InvertData(w*h, imgINT, imgINT);
//
//	MaskData(w*h, imgIn, imgINT, imgRef);
//	MaskData(w*h, imgINT, imgINT, imgMask);
//
//	FrameCopy(lpPT->pdmfPrev, lpPT->pdmfIn);
//	FrameCopy(lpPT->pdmfDisp, lpPT->pdmfRef);
//
//	if(!(Sum(w*h, imgMask)-(w*h*255))){
//		lpPT->flgGetRef = FALSE;
//		iSub = NULL;
//	}
//
//	
//
//
//	return TRUE;
//}

//0 - Newest Frame	(Red)
//1 - Middle Frame (Green)
//2 - Oldest Frame (Blue)
DWORD ParticleTracker_Track(PARTICLETRACKER *lpPT)
{
	if(!lpPT){
		return FALSE;
	}

	if(!lpPT->flgTrack)
		return FALSE;

	INT i = NULL;
	FLOAT radius = NULL;
	DWORD nEllipse = 0;
	FELLIPSE ellipse = {0};
	DWORD w = lpPT->pdmfDisp.pdmfh.width;
	DWORD h = lpPT->pdmfDisp.pdmfh.height;
	DWORD iSub = lpPT->iSub;
	DWORD iTrack = lpPT->iTrack;

	INT *imgSubI = (INT *)lpPT->pdmfSubI.lpvoid;
	INT *imgSubII = (INT *)lpPT->pdmfSubII.lpvoid;
	INT *imgSubIII = (INT *)lpPT->pdmfSubIII.lpvoid;
	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
	PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
	UCHAR *img = (UCHAR *)lpPT->pdmf.lpvoid;
	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
	UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
	UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
	UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;
	CONTOURSET *cs = &lpPT->cs;

	ImageSubtract(w, h, imgRef, imgIn, imgSubIII);
	//ImageSubtract(w, h, imgIn, imgRef, imgSubIII);
	AlgorithmProcINT(w, h, lpPT->selProc, lpPT->ssAlgorithm, imgSubIII, imgINT);
	TheoPavlidisContour(w, h, imgINT, &lpPT->cs);
	UCHARtoPIXEL(w, h, imgIn, imgPIXEL);

	
	FPOINTSET *fpsF0 = &lpPT->fpsF0;
	FPOINTSET *fpsF1 = &lpPT->fpsF1;
	FPOINTSET *fpsF2 = &lpPT->fpsF2;

	FELLIPSESET *es = &lpPT->es;
	FELLIPSESET *es0 = &lpPT->es0;
	FELLIPSESET *es1 = &lpPT->es1;
	FELLIPSESET *es2 = &lpPT->es2;
	
	Clear(es);

	FELLIPSESET esTrack = {0};
	Alloc(&esTrack, 1000);

	TRAJECTORYSET tsG = {0};
	TRAJECTORYSET tsRB = {0};
	TRAJECTORYSET tsBR = {0};
	TRAJECTORYSET ts = {0};
	Alloc(&ts, 500, 3);
	Alloc(&tsBR, 500, 3);
	Alloc(&tsRB, 500, 3);
	Alloc(&tsG, 500, 3);

	TRAJECTORYSET *tsGlobal = &lpPT->ts;

	EllipseSetFromContourSet(20, es, cs);
	OffsetCentroid(es, 1, 1);

	FELLIPSESET fes = {0};
	Alloc(&fes, es->nEllipse);

	{
		INT i = NULL;
		FELLIPSE e = {0};
		for(i = 0; i < es->nEllipse; i++){
			GetAt(es, i, &e);
			//memcpy(&ellipse, &es->ellipse[i], sizeof(FELLIPSE));
			radius = ((FLOAT)(e.a + e.b)*0.5);
			if(Range(radius, lpPT->limDiameter)){
				if((FLOAT)e.a / (FLOAT)e.b) 
				Push(&fes, e);
			}

		}
	}
	RemoveDuplicates(&fes);
	//RemoveInnerEllipse(&fes);
	Copy(&esTrack, &fes);
	
	VECTOR *v = (VECTOR *) malloc(1000*sizeof(VECTOR));
	memset(v, NULL, 1000*sizeof(VECTOR));

	if(!iTrack){	
		DWORD nVector;
		Transfer(es2, es1);
		Transfer(es1, es0);
		//Transfer(es0, es1);
		//Transfer(es1, es2);
		Copy(es0, &fes);
		//Copy(es2, &fes);
		Transfer(fpsF2, fpsF1);						//Move Green to Blue
		Transfer(fpsF1, fpsF0);						//Move Red to Green
		CentroidFromEllipseSet(fpsF0, &esTrack);	///New Contents to 0 (Red)
		MotionCorrespondanceCenterReference(fpsF2, fpsF1, fpsF0, &tsG);

		//9.4.15
		//Creates a crash due to incorrect memory freeing
		TrajecotryManagmentTest(tsGlobal, &tsG);

	}else if(iTrack == 1){
		Copy(es0, &fes);
		CentroidFromEllipseSet(fpsF0, es0);
		lpPT->iTrack--;
	}else if(iTrack == 2){
		Copy(es1, &fes);
		CentroidFromEllipseSet(fpsF1, es1);
		lpPT->iTrack--;
	}else if(iTrack == 3){
		Copy(es2, &fes);
		CentroidFromEllipseSet(fpsF2, es2);
		lpPT->iTrack--;
	}else{}

	//Begin Drawing

	PIXEL value;
	value.r = 255;
	value.g = 255;
	value.b = 255;
	DrawContourSet(w, h, value, &lpPT->cs, imgPIXEL); 

	for(i = 0; i < es0->nEllipse; i++){
		memcpy(&ellipse, &es0->ellipse[i], sizeof(FELLIPSE));
		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
		if(Range(radius, lpPT->limDiameter)){
			nEllipse++;
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 0, 255));
			//DrawEllipse(w, h, imgPIXEL, ellipse, RGB(255, 0, 0));
			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
		}
	}
	for(i = 0; i < es1->nEllipse; i++){
		memcpy(&ellipse, &es1->ellipse[i], sizeof(FELLIPSE));
		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
		if(Range(radius, lpPT->limDiameter)){
			nEllipse++;
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 255, 0));
			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
		}
	}
	for(i = 0; i < es2->nEllipse; i++){
		memcpy(&ellipse, &es2->ellipse[i], sizeof(FELLIPSE));
		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
		if(Range(radius, lpPT->limDiameter)){
			nEllipse++;
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(255, 0, 0));
			//DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 0, 255));
			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
		}else{
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 255, 255));
		}
	}

	//BLUE
	//DrawTrajectorySet(w, h, imgPIXEL, *tsGlobal, RGB(255, 0, 50));
	//DEEP PINK
	DrawTrajectorySet(w, h, imgPIXEL, *tsGlobal, RGB(255, 20, 147));
	//DrawTrajectorySet(w, h, imgPIXEL, tsG, RGB(255, 0, 255));

	////End Drawing


	Empty(&ts);

	if(v)free(v);
	Free(&fes);
	Free(&esTrack);
	Free(&tsBR);
	Free(&tsRB);
	Free(&ts);
	Free(&tsG);

	return TRUE;
}


////0 - Newest Frame	(Red)
////1 - Middle Frame (Green)
////2 - Oldest Frame (Blue)
//DWORD ParticleTracker_Track(PARTICLETRACKER *lpPT)
//{
//	if(!lpPT){
//		return FALSE;
//	}
//
//	if(!lpPT->flgTrack)
//		return FALSE;
//
//	INT i = NULL;
//	FLOAT radius = NULL;
//	DWORD nEllipse = 0;
//	FELLIPSE ellipse = {0};
//	DWORD w = lpPT->pdmfDisp.pdmfh.width;
//	DWORD h = lpPT->pdmfDisp.pdmfh.height;
//	DWORD iSub = lpPT->iSub;
//	DWORD iTrack = lpPT->iTrack;
//
//	//FrameCopy(lpPT->pdmfIn, (PDMFRAME) *pdmf);
//	INT *imgSubI = (INT *)lpPT->pdmfSubI.lpvoid;
//	INT *imgSubII = (INT *)lpPT->pdmfSubII.lpvoid;
//	INT *imgSubIII = (INT *)lpPT->pdmfSubIII.lpvoid;
//	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
//	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
//	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
//	PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
//	UCHAR *imgIn = (UCHAR *) lpPT->pdmfIn.lpvoid;
//	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
//	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
//	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
//	UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
//	UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
//	UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;
//	CONTOURSET *cs = &lpPT->cs;
//
//	//ImageSubtract(w, h, imgRef, imgIn, imgSubIII);
//	ImageSubtract(w, h, imgIn, imgRef, imgSubIII);
//	AlgorithmProcINT(w, h, lpPT->selProc, lpPT->ssAlgorithm, imgSubIII, imgINT);
//	TheoPavlidisContour(w, h, imgINT, &lpPT->cs);
//	UCHARtoPIXEL(w, h, imgIn, imgPIXEL);
//
//	
//	FPOINTSET *fpsF0 = &lpPT->fpsF0;
//	FPOINTSET *fpsF1 = &lpPT->fpsF1;
//	FPOINTSET *fpsF2 = &lpPT->fpsF2;
//
//	FELLIPSESET *es = &lpPT->es;
//	FELLIPSESET *es0 = &lpPT->es0;
//	FELLIPSESET *es1 = &lpPT->es1;
//	FELLIPSESET *es2 = &lpPT->es2;
//	
//	Clear(es);
//
//	FELLIPSESET esTrack = {0};
//	Alloc(&esTrack, 1000);
//
//	TRAJECTORYSET tsG = {0};
//	TRAJECTORYSET tsRB = {0};
//	TRAJECTORYSET tsBR = {0};
//	TRAJECTORYSET ts = {0};
//	Alloc(&ts, 500, 3);
//	Alloc(&tsBR, 500, 3);
//	Alloc(&tsRB, 500, 3);
//	Alloc(&tsG, 500, 3);
//
//	TRAJECTORYSET *tsGlobal = &lpPT->ts;
//
//	EllipseSetFromContourSet(20, es, cs);
//	OffsetCentroid(es, 1, 1);
//
//	FELLIPSESET fes = {0};
//	Alloc(&fes, es->nEllipse);
//
//	{
//		INT i = NULL;
//		FELLIPSE e = {0};
//		for(i = 0; i < es->nEllipse; i++){
//			GetAt(es, i, &e);
//			//memcpy(&ellipse, &es->ellipse[i], sizeof(FELLIPSE));
//			radius = ((FLOAT)(e.a + e.b)*0.5);
//			if(Range(radius, lpPT->limDiameter)){
//				if((FLOAT)e.a / (FLOAT)e.b) 
//				Push(&fes, e);
//			}
//
//		}
//	}
//	RemoveDuplicates(&fes);
//	//RemoveInnerEllipse(&fes);
//	Copy(&esTrack, &fes);
//	
//	VECTOR *v = (VECTOR *) malloc(1000*sizeof(VECTOR));
//	memset(v, NULL, 1000*sizeof(VECTOR));
//
//	if(!iTrack){	
//		DWORD nVector;
//		Transfer(es2, es1);
//		Transfer(es1, es0);
//		//Transfer(es0, es1);
//		//Transfer(es1, es2);
//		Copy(es0, &fes);
//		//Copy(es2, &fes);
//		Transfer(fpsF2, fpsF1);						//Move Green to Blue
//		Transfer(fpsF1, fpsF0);						//Move Red to Green
//		CentroidFromEllipseSet(fpsF0, &esTrack);	///New Contents to 0 (Red)
//		////MotionCorrespondance(fpsF0, fpsF1, fpsF2, &ts);
//		//MotionCorrespondance(fpsF0, fpsF1, fpsF2, &tsBR);
//		//MotionCorrespondance(fpsF2, fpsF1, fpsF0, &tsRB);
//		//MotionCorrespondanceCenterReference(lpPT->maxAngle, lpPT->maxVelocity, fpsF2, fpsF1, fpsF0, &tsG);
//		MotionCorrespondanceCenterReference(fpsF2, fpsF1, fpsF0, &tsG);
//
//		//Reverse(&tsG);
//		//DiscardMismatches(&tsRB, &tsG, &ts);
//		TrajecotryManagmentTest(tsGlobal, &tsG);
//		//AppendRemoveTrajectories(tsGlobal, &tsG);
//		//Reverse(&tsBR);
//		//DiscardMismatches(&tsRB, &tsBR, &ts);
//		//AppendRemoveTrajectories(tsGlobal, &ts);
//		//AppendRemoveTrajectories(tsGlobal, &tsRB);
//
//
//		//AppendMatchedTrajectories(tsGlobal, &ts);
//
//		//MotionCorrespondanceSmart(fpsF0, fpsF1, fpsF2, &ts);
//		//AppendRemoveTrajectories(tsGlobal, &tsRB);
//		//AppendMatchedTrajectories(tsGlobal, &tsRB);
//
//		//CHAR str[MAX_STRING];
//		//memset(str, NULL, MAX_STRING*sizeof(CHAR));
//		////sprintf(str, "time: %d  num: %d", dif, fpsF0->nPoint*fpsF1->nPoint*fpsF2->nPoint);
//		//sprintf(str, "R(%d)G(%d)B(%d)", fpsF0->nPoint, fpsF1->nPoint, fpsF2->nPoint);
//		//ThrowMessage(lpPT->hOwner, str);
//		//{
//		//	INT i = NULL;
//		//	for(i = 0; i < es0->nEllipse; i++){
//		//		sprintf(str, "X(%.1f)Y(%.1f)A(%.1f)B(%.1f)", es2->ellipse[i].x, es2->ellipse[i].y, es2->ellipse[i].a, es2->ellipse[i].b); 
//		//		ThrowMessage(lpPT->hOwner, str);
//		//	}
//		//}
//		
//	}else if(iTrack == 1){
//		Copy(es0, &fes);
//		CentroidFromEllipseSet(fpsF0, es0);
//		lpPT->iTrack--;
//	}else if(iTrack == 2){
//		Copy(es1, &fes);
//		CentroidFromEllipseSet(fpsF1, es1);
//		lpPT->iTrack--;
//	}else if(iTrack == 3){
//		Copy(es2, &fes);
//		CentroidFromEllipseSet(fpsF2, es2);
//		lpPT->iTrack--;
//	}else{}
//
//	//Begin Drawing
//	for(i = 0; i < es0->nEllipse; i++){
//		memcpy(&ellipse, &es0->ellipse[i], sizeof(FELLIPSE));
//		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
//		if(Range(radius, lpPT->limDiameter)){
//			nEllipse++;
//			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 0, 255));
//			//DrawEllipse(w, h, imgPIXEL, ellipse, RGB(255, 0, 0));
//			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
//		}
//	}
//	for(i = 0; i < es1->nEllipse; i++){
//		memcpy(&ellipse, &es1->ellipse[i], sizeof(FELLIPSE));
//		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
//		if(Range(radius, lpPT->limDiameter)){
//			nEllipse++;
//			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 255, 0));
//			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
//		}
//	}
//	for(i = 0; i < es2->nEllipse; i++){
//		memcpy(&ellipse, &es2->ellipse[i], sizeof(FELLIPSE));
//		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
//		if(Range(radius, lpPT->limDiameter)){
//			nEllipse++;
//			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(255, 0, 0));
//			//DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 0, 255));
//			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
//		}else{
//			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 255, 255));
//		}
//	}
//
//	DrawTrajectorySet(w, h, imgPIXEL, *tsGlobal, RGB(255, 0, 50));
//	////DrawTrajectorySet(w, h, imgPIXEL, ts, RGB(255, 0, 255));
//	////DrawTrajectorySet(w, h, imgPIXEL, tsBR, RGB(255, 255, 0));
//	////DrawTrajectorySet(w, h, imgPIXEL, tsRB, RGB(0, 255, 255));
//	//DrawTrajectorySet(w, h, imgPIXEL, tsG, RGB(100, 50, 200));
//	////End Drawing
//
//
//	Empty(&ts);
//
//	if(v)free(v);
//	Free(&fes);
//	Free(&esTrack);
//	Free(&tsBR);
//	Free(&tsRB);
//	Free(&ts);
//	Free(&tsG);
//
//	return TRUE;
//}

//0 - Newest Frame	(Red)
//1 - Middle Frame (Green)
//2 - Oldest Frame (Blue)
DWORD ParticleTracker_Track(PARTICLETRACKER *lpPT, PDMFRAME *pdmf)
{
	if(!lpPT){
		return FALSE;
	}

	if(!lpPT->flgTrack)
		return FALSE;

	INT i = NULL;
	FLOAT radius = NULL;
	DWORD w = pdmf->pdmfh.width;
	DWORD h = pdmf->pdmfh.height;
	DWORD iSub = lpPT->iSub;
	DWORD iTrack = lpPT->iTrack;
	//PDMFRAME *pdmFrame = (PDMFRAME *)wParam;

	FrameCopy(lpPT->pdmfIn, (PDMFRAME) *pdmf);

	UCHAR *imgIn = (UCHAR *) pdmf->lpvoid;
	UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
	UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
	UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
	INT *imgSubI = (INT *)lpPT->pdmfSubI.lpvoid;
	INT *imgSubII = (INT *)lpPT->pdmfSubII.lpvoid;
	INT *imgSubIII = (INT *)lpPT->pdmfSubIII.lpvoid;
	INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
	INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
	INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
	PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
	UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
	UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
	UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;

	CONTOURSET *cs = &lpPT->cs;

	//INT cudaTime = NULL;
	//cudaTime = SubtractCUDA(w, h, imgIn, imgRef, imgSubIII);
	//ccudaMallocHost(&imgSubIII, 1);
	//UCHARtoINT(w, h, imgSubCUDA, imgSubIII); 
	ImageSubtract(w, h, imgIn, imgRef, imgSubIII);
	AlgorithmProcINT(w, h, lpPT->selProc, lpPT->ssAlgorithm, imgSubIII, imgINT);
	TheoPavlidisContour(w, h, imgINT, &lpPT->cs);
	UCHARtoPIXEL(w, h, imgIn, imgPIXEL);
	DWORD nEllipse = 0;
	FELLIPSE ellipse;
	
	FPOINTSET *fpsF0 = &lpPT->fpsF0;
	FPOINTSET *fpsF1 = &lpPT->fpsF1;
	FPOINTSET *fpsF2 = &lpPT->fpsF2;

	FELLIPSESET *es0 = &lpPT->es0;
	FELLIPSESET *es1 = &lpPT->es1;
	FELLIPSESET *es2 = &lpPT->es2;
	FELLIPSESET *es = &lpPT->es;
	Clear(es);

	FELLIPSESET esTrack = {0};
	Alloc(&esTrack, 1000);

	TRAJECTORYSET tsG = {0};
	TRAJECTORYSET tsRB = {0};
	TRAJECTORYSET tsBR = {0};
	TRAJECTORYSET ts = {0};
	Alloc(&ts, 500, 3);
	Alloc(&tsBR, 500, 3);
	Alloc(&tsRB, 500, 3);
	Alloc(&tsG, 500, 3);

	TRAJECTORYSET *tsGlobal = &lpPT->ts;

	EllipseSetFromContourSet(20, es, cs);
	OffsetCentroid(es, 1, 1);

	FELLIPSESET fes = {0};
	Alloc(&fes, es->nEllipse);

	{
		INT i = NULL;
		FELLIPSE e = {0};
		for(i = 0; i < es->nEllipse; i++){
			GetAt(es, i, &e);
			//memcpy(&ellipse, &es->ellipse[i], sizeof(FELLIPSE));
			radius = ((FLOAT)(e.a + e.b)*0.5);
			if(Range(radius, lpPT->limDiameter)){
				Push(&fes, e);
			}

		}
	}
	RemoveDuplicates(&fes);
	//RemoveInnerEllipse(&fes);
	Copy(&esTrack, &fes);
	
	VECTOR *v = (VECTOR *) malloc(1000*sizeof(VECTOR));
	memset(v, NULL, 1000*sizeof(VECTOR));

	if(!iTrack){	
		DWORD nVector;
		Transfer(es2, es1);
		Transfer(es1, es0);
		//Transfer(es0, es1);
		//Transfer(es1, es2);
		Copy(es0, &fes);
		//Copy(es2, &fes);
		Transfer(fpsF2, fpsF1);						//Move Green to Blue
		Transfer(fpsF1, fpsF0);						//Move Red to Green
		CentroidFromEllipseSet(fpsF0, &esTrack);	///New Contents to 0 (Red)
		//MotionCorrespondance(fpsF0, fpsF1, fpsF2, &ts);
		MotionCorrespondance(fpsF0, fpsF1, fpsF2, &tsBR);
		MotionCorrespondance(fpsF2, fpsF1, fpsF0, &tsRB);
		MotionCorrespondanceCenterReference(fpsF2, fpsF1, fpsF0, &tsG);

		//Reverse(&tsG);
		//DiscardMismatches(&tsRB, &tsG, &ts);
		TrajecotryManagmentTest(tsGlobal, &tsG);
		//AppendRemoveTrajectories(tsGlobal, &tsG);
		//Reverse(&tsBR);
		//DiscardMismatches(&tsRB, &tsBR, &ts);
		//AppendRemoveTrajectories(tsGlobal, &ts);
		//AppendRemoveTrajectories(tsGlobal, &tsRB);


		//AppendMatchedTrajectories(tsGlobal, &ts);

		//MotionCorrespondanceSmart(fpsF0, fpsF1, fpsF2, &ts);
		//AppendRemoveTrajectories(tsGlobal, &tsRB);
		//AppendMatchedTrajectories(tsGlobal, &tsRB);

		//CHAR str[MAX_STRING];
		//memset(str, NULL, MAX_STRING*sizeof(CHAR));
		////sprintf(str, "time: %d  num: %d", dif, fpsF0->nPoint*fpsF1->nPoint*fpsF2->nPoint);
		//sprintf(str, "R(%d)G(%d)B(%d)", fpsF0->nPoint, fpsF1->nPoint, fpsF2->nPoint);
		//ThrowMessage(lpPT->hOwner, str);
		//{
		//	INT i = NULL;
		//	for(i = 0; i < es0->nEllipse; i++){
		//		sprintf(str, "X(%.1f)Y(%.1f)A(%.1f)B(%.1f)", es2->ellipse[i].x, es2->ellipse[i].y, es2->ellipse[i].a, es2->ellipse[i].b); 
		//		ThrowMessage(lpPT->hOwner, str);
		//	}
		//}
		
	}else if(iTrack == 1){
		Copy(es0, &fes);
		CentroidFromEllipseSet(fpsF0, es0);
		lpPT->iTrack--;
	}else if(iTrack == 2){
		Copy(es1, &fes);
		CentroidFromEllipseSet(fpsF1, es1);
		lpPT->iTrack--;
	}else if(iTrack == 3){
		Copy(es2, &fes);
		CentroidFromEllipseSet(fpsF2, es2);
		lpPT->iTrack--;
	}else{}

	//Begin Drawing
	for(i = 0; i < es0->nEllipse; i++){
		memcpy(&ellipse, &es0->ellipse[i], sizeof(FELLIPSE));
		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
		if(Range(radius, lpPT->limDiameter)){
			nEllipse++;
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 0, 255));
			//DrawEllipse(w, h, imgPIXEL, ellipse, RGB(255, 0, 0));
			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
		}
	}
	for(i = 0; i < es1->nEllipse; i++){
		memcpy(&ellipse, &es1->ellipse[i], sizeof(FELLIPSE));
		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
		if(Range(radius, lpPT->limDiameter)){
			nEllipse++;
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 255, 0));
			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
		}
	}
	for(i = 0; i < es2->nEllipse; i++){
		memcpy(&ellipse, &es2->ellipse[i], sizeof(FELLIPSE));
		radius = ((FLOAT)(ellipse.a + ellipse.b) / (FLOAT)2.0);
		if(Range(radius, lpPT->limDiameter)){
			nEllipse++;
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(255, 0, 0));
			//DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 0, 255));
			DrawCross(w, h, imgPIXEL, Point((INT)ellipse.x, (INT)ellipse.y), RGB(255, 0, 0), 3);
		}else{
			DrawEllipse(w, h, imgPIXEL, ellipse, RGB(0, 255, 255));
		}
	}

	DrawTrajectorySet(w, h, imgPIXEL, *tsGlobal, RGB(255, 0, 50));
	//DrawTrajectorySet(w, h, imgPIXEL, ts, RGB(255, 0, 255));
	//DrawTrajectorySet(w, h, imgPIXEL, tsBR, RGB(255, 255, 0));
	//DrawTrajectorySet(w, h, imgPIXEL, tsRB, RGB(0, 255, 255));
	DrawTrajectorySet(w, h, imgPIXEL, tsG, RGB(100, 50, 200));
	//End Drawing


	Empty(&ts);

	if(v)free(v);
	Free(&fes);
	Free(&esTrack);
	Free(&tsBR);
	Free(&tsRB);
	Free(&ts);
	Free(&tsG);

	return TRUE;
}