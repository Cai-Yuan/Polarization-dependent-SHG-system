#include "stdafx.h"

#include "CUDAFunDll.h"

//tsao
//ma de

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


#include "TFPT.h"
#include "MotionCorrespondence.h"

#include "WinUser.h"

#include <vector>


//#define MAX_FRAMEBUFFER 30

//LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, INT *in, UCHAR *out);
//LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, UCHAR *in, UCHAR *out);
//LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, INT *in, INT *out);

LRESULT CALLBACK ParticleTrackerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PARTICLETRACKER *lpPT = (PARTICLETRACKER *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg){
	case WM_TRACKERENABLE:
		{
			lpPT->flgTrack = (DWORD)wParam;
			if(wParam){	//Begin Tracking Again
				lpPT->flgGetRef = TRUE;
			}

		}
		return TRUE;
	case WM_CREATELINK:
		{
			return OnCreateLink(&lpPT->wls, hwnd, wParam, lParam);
		}
	case DF_INITFILTER:
		{
			return OnInitAlgorithmParticleTrack(lpPT, hwnd, wParam, lParam);
		}
	case WM_TRACK:
		{
			return OnTrackParticleTracker(lpPT, hwnd, msg, wParam, lParam);
		}
	case WM_SETFRAMESIZE:
		{
			return OnSetFrameSizeParticleTracker(lpPT, hwnd, msg, wParam, lParam);
		}
	case WM_DATAIN:
		{
			return OnDataInParticleTracker(lpPT, hwnd, msg, wParam, lParam);
		}
	case WM_DATADROP:
		{
			return OnDataDropParticleTracker(lpPT, hwnd, msg, wParam, lParam);
		}
	case WM_SHOW:
		{
			lpPT->flgShow = wParam;
			SendMessage(lpPT->hDataView, WM_COMMAND, MAKEWPARAM(DVN_SETTOOL, TOOL_CURSOR), NULL);
			switch(wParam){
				case PTS_STREAM:
					//DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfRef);
					DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfIn);
					return TRUE;
				case PTS_FILTER:
					DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPrev);
					break;
				case PTS_MASK_LAT:
					break;
				case PTS_MASK_LON:
					DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfIn);
					break;
				case PTS_TRACKING:
					break;
				case PTS_MASK_FLOW:
					DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfFlow);
					break;
				default:
					break;
			}
			return TRUE;
		}
	case WM_GETMASK:
		{
			if(lParam == PTS_MASK_LON){
				lpPT->flgShow = PTS_STREAM;
				SendMessage(lpPT->hDataView, WM_GETMASK, (WPARAM)hwnd, lParam);
			}else if(lParam == PTS_MASK_LAT){
				lpPT->flgShow = PTS_STREAM;
				SendMessage(lpPT->hDataView, WM_GETMASK, (WPARAM)hwnd, lParam);
			}else if(lParam == PTS_MASK_CHN){
				lpPT->flgShow = PTS_STREAM;
				SendMessage(lpPT->hDataView, WM_GETMASK, (WPARAM)hwnd, lParam);
			}else{
				return FALSE;
			}
			return TRUE;
		}
	case WM_SETMASK:
		{
			PDMFRAME *pdmf;
			pdmf = (PDMFRAME *) lParam;
			switch(wParam){
			case PTS_MASK_LON:
				lpPT->flgShow = PTS_MASK_LON;
				FrameCopy(lpPT->pdmfMaskLon, (PDMFRAME)*pdmf);
				break;
			case PTS_MASK_LAT:
				lpPT->flgShow = PTS_MASK_LAT;
				FrameCopy(lpPT->pdmfMaskLat, (PDMFRAME)*pdmf);
				break;
			case PTS_MASK_CHN:
				lpPT->flgShow = PTS_MASK_CHN;
				FrameCopy(lpPT->pdmfMaskChn, (PDMFRAME)*pdmf);
				break;
			default:
				break;
			}
			return TRUE;
		}
	case WM_SETLIMITS:
		{
			if(!lParam)
				return FALSE;
			LIMIT lim;
			memcpy((VOID *)&lim, (VOID *)lParam, sizeof(LIMIT));

			if(wParam == DIAMETER){
				lpPT->limDiameter.lower = lim.lower;
				lpPT->limDiameter.upper = lim.upper;
				//memcpy((VOID *)&lpPT->limDiameter, (VOID *)lParam, sizeof(LIMIT));
			}else if(wParam == VELOCITY){
				lpPT->maxVelocity = lim.upper;
			}else if(wParam == DVELOCITY){
				lpPT->maxDelta = lim.upper;
			}else if(wParam == ANGLE){
				lpPT->maxAngle = lim.upper;
			}else{}
			return TRUE;
		}
	case WM_SETHWND:
		{
			switch(wParam)
			{
			case HWND_OWNER:
				lpPT->hOwner = (HWND)lParam;
				break;
			case HWND_LINK:
				return TRUE;
			case HWND_DATAVIEW:
				lpPT->hDataView = (HWND) lParam;
				SendLink(hwnd, lpPT->hDataView, TRUE);
				return TRUE;
			default:
				break;
			}
			return TRUE;
		}
	case WM_GETHWND:
		{
			switch(wParam)
			{
			case HWND_OWNER:
				return (LRESULT)(HWND) lpPT->hOwner;
			case HWND_DATAVIEW:
				return (LRESULT)(HWND) lpPT->hDataView;
			default:
				break;
			}
			return TRUE;
		}
	case WM_CREATE:
		{
			OnCreateParticleTracker(lpPT, hwnd, wParam, lParam);
		}
		break;
	case WM_NCCREATE:
		{
			lpPT = (PARTICLETRACKER *)InitWindowLongRACE(hwnd, sizeof(PARTICLETRACKER));
			lpPT = lpPT;
		}
		break;
	default: 
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT OnSetFrameSizeParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	DWORD w = wParam;
	DWORD h = lParam;

	lpPT->w = (DWORD) w;
	lpPT->h = (DWORD) h;

	AllocPDMFrame(&lpPT->pdmf, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfIn, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfIn, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfRef, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfDisp, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfSubI, w, h, DT_INT);
	AllocPDMFrame(&lpPT->pdmfSubII, w, h, DT_INT);
	AllocPDMFrame(&lpPT->pdmfSubIII, w, h, DT_INT);
	AllocPDMFrame(&lpPT->pdmfINT, w, h, DT_INT);
	AllocPDMFrame(&lpPT->pdmfTemp, w, h, DT_INT);
	AllocPDMFrame(&lpPT->pdmfUCHAR, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfPrev, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfMask, w, h, DT_INT);
	AllocPDMFrame(&lpPT->pdmfPIXEL, w, h, DT_PIXEL);
	AllocPDMFrame(&lpPT->pdmfFlow, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfFlowINT, w, h, DT_INT);

	AllocPDMFrame(&lpPT->pdmfMaskLon, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfMaskLat, w, h, DT_UCHAR);
	AllocPDMFrame(&lpPT->pdmfMaskChn, w, h, DT_UCHAR);

	SetPDMFrameUCHAR(&lpPT->pdmfMaskLon, 255);
	SetPDMFrameUCHAR(&lpPT->pdmfMaskLat, 255);
	SetPDMFrameUCHAR(&lpPT->pdmfMaskChn, 255);
	
	AllocContourSet(&lpPT->cs, w+h, w+h);
	Zero(&lpPT->cs);

	SendMessage(lpPT->hDataView, msg, wParam, lParam);

	ShowWindow(lpPT->hDataView, SW_SHOW);

	return TRUE;
}

LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, INT *in, UCHAR *out)
{

	INT i = NULL;
	INT iProc = NULL;
	//DWORD selProc = lpDataView->selProc;
	//UINT w = lpDataView->wSource;
	//UINT h = lpDataView->hSource;
	UINT iSrc = NULL;
	DWORD nProc = ssAlgorithm.nStr;	//nProc = lpDataView->strProc.nStr;
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

	if(!nProc)
		nProc = TRUE;

	INT **imgProc = (INT **)malloc(nProc*sizeof(INT*));
	memset(imgProc, NULL, nProc*sizeof(INT*));

	for(i = 0; i < nProc; i++){
		imgProc[i] = (INT *)malloc(w*h*sizeof(INT));
		if(imgProc[i]){
			memset(imgProc[i], NULL, w*h*sizeof(INT));
		}
	}

	memcpy(imgProc[0], in, w*h*sizeof(INT));
	//UCHARtoINT(w, h, in, (INT *)imgProc[0]);

	for(i = 1; i <= selProc/*nProc*/; i++){	

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
		memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
		
		nChar = ssAlgorithm.string[i].nChar;
		memcpy(str, ssAlgorithm.string[i].str, nChar*sizeof(CHAR));
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
			ClampData(w*h, (INT)pList[1], (INT)pList[2],(INT *) imgProc[iSrc], (INT *)imgProc[iProc]);
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
		else if(!strcmp(strAlgorithm, STR_SUBTRACT)){
			//ImageSubtract(w, h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[(UINT)pList[1]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ADD)){
			Addition(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[(UINT)pList[1]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SQUARE)){
			Square(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MEAN)){
			Mean(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STDDEV)){
			StdDev(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_VARIANCE)){
			Variance(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NORMVARIANCE)){
			NormalizedVariance(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CHAMFER)){
			//Chamfer(w, h, imgProc[(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DILATE)){
			Dilate(w, h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ERODE)){
			Erode(w, h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_THRESHOLD)){
			Threshold(w, h, pList[1], (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else
			i = i;

	}

	ClampData(w*h, 0, 255, (INT *)imgProc[selProc], out);

	for(i = 0; i < nProc; i++){
		if(imgProc[i])free(imgProc[i]);
	}

	if(imgProc) free(imgProc);
	if(img) free(img);

	return TRUE;
}

LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, INT *in, INT *out)
{

	INT i = NULL;
	INT iProc = NULL;
	UINT iSrc = NULL;
	DWORD nProc = ssAlgorithm.nStr;	//nProc = lpDataView->strProc.nStr;
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

	if(!nProc)
		nProc = TRUE;

	INT **imgProc = (INT **)malloc(nProc*sizeof(INT*));
	memset(imgProc, NULL, nProc*sizeof(INT*));

	for(i = 0; i < nProc; i++){
		imgProc[i] = (INT *)malloc(w*h*sizeof(INT));
		if(imgProc[i]){
			memset(imgProc[i], NULL, w*h*sizeof(INT));
		}
	}

	memcpy(imgProc[0], in, w*h*sizeof(INT));

	for(i = 1; i <= selProc; i++){	

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
		memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
		
		nChar = ssAlgorithm.string[i].nChar;
		memcpy(str, ssAlgorithm.string[i].str, nChar*sizeof(CHAR));
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
			ClampData(w*h, (INT)pList[1], (INT)pList[2],(INT *) imgProc[iSrc], (INT *)imgProc[iProc]);
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
		//else if(!strcmp(strAlgorithm, STR_GETFRAME)){
		//	GetFrame(lpDataView->hDataManager, w, h, (DWORD)(lpDataView->fStack + pList[0]), (FLOAT *)imgProc[iProc]);
		//}
		else if(!strcmp(strAlgorithm, STR_SUBTRACT)){
			//ImageSubtract(w, h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[(UINT)pList[1]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ADD)){
			Addition(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[(UINT)pList[1]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SQUARE)){
			Square(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		//else if(!strcmp(strAlgorithm, STR_MIP)){
		//	fRender = TRUE;
		//	procRender = i;
		//}
		else if(!strcmp(strAlgorithm, STR_MEAN)){
			Mean(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STDDEV)){
			StdDev(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_VARIANCE)){
			Variance(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NORMVARIANCE)){
			NormalizedVariance(w*h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DOUBLETHRESHOLD)){
			DoubleThreshold(w, h, (INT)pList[1], (INT)pList[2], (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CHAMFER)){
			//Chamfer(w, h, imgProc[(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DILATE)){
			Dilate(w, h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ERODE)){
			Erode(w, h, (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_THRESHOLD)){
			Threshold(w, h, pList[1], (INT *)imgProc[(UINT)pList[0]], (INT *)imgProc[iProc]);
		}
		//+else if(!strcmp(strAlgorithm, STR_AVERAGE)){
			//AverageImage(w, h, imgProc[(UINT)pList[0]
		else
			i = i;

	}

	memcpy(out, imgProc[selProc], w*h*sizeof(INT));

	for(i = 0; i < nProc; i++){
		if(imgProc[i])free(imgProc[i]);
	}

	if(imgProc) free(imgProc);
	if(img) free(img);

	return TRUE;
}


//Removed 8.30.15

//LRESULT CALLBACK ParticleTrackerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	PARTICLETRACKER *lpPT = (PARTICLETRACKER *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
//
//	CHAR str[MAX_STRING];
//
//	switch(msg){
//	case WM_CREATELINK:
//		{
//			return OnCreateLink(&lpPT->wls, hwnd, wParam, lParam);
//		}
//	case DF_INITFILTER:
//		{
//			return OnInitAlgorithmParticleTrack(lpPT, hwnd, wParam, lParam);
//		}
//	case WM_SETFRAMESIZE:
//		{
//		DWORD w = wParam;
//		DWORD h = lParam;
//
//		lpPT->w = (DWORD) w;
//		lpPT->h = (DWORD) h;
//
//		AllocPDMFrame(&lpPT->pdmfIn, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfIn, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfRef, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfDisp, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfSubI, w, h, DT_INT);
//		AllocPDMFrame(&lpPT->pdmfSubII, w, h, DT_INT);
//		AllocPDMFrame(&lpPT->pdmfSubIII, w, h, DT_INT);
//		AllocPDMFrame(&lpPT->pdmfINT, w, h, DT_INT);
//		AllocPDMFrame(&lpPT->pdmfTemp, w, h, DT_INT);
//		AllocPDMFrame(&lpPT->pdmfUCHAR, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfPrev, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfMask, w, h, DT_INT);
//		AllocPDMFrame(&lpPT->pdmfPIXEL, w, h, DT_PIXEL);
//
//		AllocPDMFrame(&lpPT->pdmfMaskLon, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfMaskLat, w, h, DT_UCHAR);
//		AllocPDMFrame(&lpPT->pdmfMaskChn, w, h, DT_UCHAR);
//
//		SetPDMFrameUCHAR(&lpPT->pdmfMaskLon, 255);
//		SetPDMFrameUCHAR(&lpPT->pdmfMaskLat, 255);
//		SetPDMFrameUCHAR(&lpPT->pdmfMaskChn, 255);
//		
//		AllocContourSet(&lpPT->cs, w+h, w+h);
//		Zero(&lpPT->cs);
//
//		SendMessage(lpPT->hDataView, msg, wParam, lParam);
//
//		ShowWindow(lpPT->hDataView, SW_SHOW);
//		}
//		return TRUE;
//	case WM_DATAIN:
//		{
//		DWORD w = NULL;
//		DWORD h = NULL;
//		DWORD iSub = lpPT->iSub;
//		PDMFRAME *pdmFrame = (PDMFRAME *)wParam;
//
//		w = pdmFrame->pdmfh.width;
//		h = pdmFrame->pdmfh.height;
//		//Should Assert Same Headers
//		FrameCopy(lpPT->pdmfIn, (PDMFRAME) *pdmFrame);
//
//		UCHAR *imgIn = (UCHAR *) pdmFrame->lpvoid;
//		UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
//		UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
//		UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
//		INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
//		INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
//		INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
//		PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
//		UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
//		UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
//		UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;
//
//		if(!lpPT->flgGetRef){
//
//			ParticleTracker_Track(lpPT, pdmFrame);
//
//		}else{
//			FLOAT stdDev = NULL;
//			FLOAT var = NULL;
//	
//			ImageSubtract(w, h, imgIn, imgPrev, imgINT);
//			Threshold(w, h, 1, imgINT);
//			InvertData(w*h, imgINT, imgINT);
//
//			MaskData(w*h, imgIn, imgINT, imgRef);
//			MaskData(w*h, imgINT, imgINT, imgMask);
//
//			FrameCopy(lpPT->pdmfPrev, lpPT->pdmfIn);
//			FrameCopy(lpPT->pdmfDisp, lpPT->pdmfRef);
//
//			if(!(Sum(w*h, imgMask)-(w*h*255))){
//				lpPT->flgGetRef = FALSE;
//				iSub = NULL;
//			}
//		}
//
//		lpPT->iSub = iSub;
//		if(lpPT->flgShow == PTS_FILTER){
//			INTtoUCHAR(w, h, imgINT, imgPrev);
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPrev);
//			return TRUE;
//		}else if(lpPT->flgShow == PTS_REFERENCE){
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfRef);
//			return TRUE;
//		}else if(lpPT->flgShow == PTS_STREAM){
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfIn);
//			return TRUE;
//		}else if(lpPT->flgShow == PTS_TRACKING){
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
//			return TRUE;
//		}else if(lpPT->flgShow == PTS_MASK_LON){
//			AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskLon, imgPIXEL);
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
//			//DataView_ShowFrame(lpPT->hDataView, /*&lpPT->pdmfMaskLon*/);
//			return TRUE;
//		}else if(lpPT->flgShow == PTS_MASK_LAT){
//			AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskLat, imgPIXEL);
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
//			//DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfMaskLat);
//			return TRUE;
//		}else if(lpPT->flgShow == PTS_MASK_CHN){
//			AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskChn, imgPIXEL);
//			DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
//			//DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfMaskChn);
//			return TRUE;
//		}else{}
//		//SendMessage(lpPT->hDataView, WM_SHOWFRAME, (WPARAM) &lpPT->pdmfPIXEL, (LPARAM) lpPT->pdmfPIXEL.lpvoid);
//		//SendMessage(lpPT->hDataView, WM_SHOWFRAME, (WPARAM) &lpPT->pdmfDisp, (LPARAM) lpPT->pdmfDisp.lpvoid);
//
//		}
//		return TRUE;
//	case WM_SHOW:
//		lpPT->flgShow = wParam;
//		SendMessage(lpPT->hDataView, WM_COMMAND, MAKEWPARAM(DVN_SETTOOL, TOOL_CURSOR), NULL);
//		switch(wParam){
//			case PTS_STREAM:
//				DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfRef);
//				return TRUE;
//			case PTS_FILTER:
//				DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPrev);
//				break;
//			case PTS_MASK_LAT:
//				break;
//			case PTS_MASK_LON:
//				DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfIn);
//				//SendMessage(lpPT->hDataView, WM_COMMAND, MAKEWPARAM(DVN_SETTOOL, TOOL_ROIPOLY), (LPARAM)lpPT->hDataView);
//				break;
//			case PTS_TRACKING:
//				break;
//			default:
//				break;
//		}
//		return TRUE;
//	case WM_GETMASK:
//		{
//			if(lParam == PTS_MASK_LON){
//				lpPT->flgShow = PTS_STREAM;
//				SendMessage(lpPT->hDataView, WM_GETMASK, (WPARAM)hwnd, lParam);
//			}else if(lParam == PTS_MASK_LAT){
//				lpPT->flgShow = PTS_STREAM;
//				SendMessage(lpPT->hDataView, WM_GETMASK, (WPARAM)hwnd, lParam);
//			}else if(lParam == PTS_MASK_CHN){
//				lpPT->flgShow = PTS_STREAM;
//				SendMessage(lpPT->hDataView, WM_GETMASK, (WPARAM)hwnd, lParam);
//			}else{
//				return FALSE;
//			}
//			return TRUE;
//		}
//	case WM_SETMASK:
//		{
//			PDMFRAME *pdmf;
//			pdmf = (PDMFRAME *) lParam;
//			switch(wParam){
//			case PTS_MASK_LON:
//				lpPT->flgShow = PTS_MASK_LON;
//				FrameCopy(lpPT->pdmfMaskLon, (PDMFRAME)*pdmf);
//				break;
//			case PTS_MASK_LAT:
//				lpPT->flgShow = PTS_MASK_LAT;
//				FrameCopy(lpPT->pdmfMaskLat, (PDMFRAME)*pdmf);
//				break;
//			case PTS_MASK_CHN:
//				lpPT->flgShow = PTS_MASK_CHN;
//				FrameCopy(lpPT->pdmfMaskChn, (PDMFRAME)*pdmf);
//				break;
//			default:
//				break;
//			}
//			return TRUE;
//		}
//	case WM_SETLIMITS:
//		if(!lParam)
//			return FALSE;
//		LIMIT lim;
//		memcpy((VOID *)&lim, (VOID *)lParam, sizeof(LIMIT));
//
//		if(wParam == DIAMETER){
//			memcpy((VOID *)&lpPT->limDiameter, (VOID *)lParam, sizeof(LIMIT));
//		}else if(wParam == VELOCITY){
//			lpPT->maxVelocity = lim.upper;
//		}else if(wParam == DVELOCITY){
//			lpPT->maxDelta = lim.upper;
//		}else if(wParam == ANGLE){
//			lpPT->maxAngle = lim.upper;
//		}else{}
//		return TRUE;
//	case WM_SETHWND:
//		switch(wParam)
//		{
//		case HWND_OWNER:
//			{
//			lpPT->hOwner = (HWND)lParam;
//			lpPT->hDataView = CreateMDIDataView(lpPT->hOwner, "DataView - ParticleTracker");
//			SendLink(hwnd, lpPT->hDataView, TRUE);
//			ShowWindow(lpPT->hDataView, SW_SHOW);
//
//			//lpPT->hDlg = lpPT->hOwner;
//			//HMODULE hModule = (HMODULE) SendMessage(lpPT->hOwner, WM_GETMODULEHANDLE, NULL, NULL);
//			lpPT->hDlg = (HWND) SendMessage(lpPT->hOwner, WM_CREATEDIALOG, (WPARAM)IDD_PARTICLETRACKER,(LPARAM) ParticleTrackerDlgProc);
//			SendMessage(lpPT->hDlg, WM_SETHWND, (WPARAM) HWND_PARTICLETRACKER, (LPARAM)hwnd);
//			ShowWindow(lpPT->hDlg, SW_SHOW);
//			}
//			break;
//		default:
//			break;
//		}
//		return TRUE;
//	case WM_GETHWND:
//		switch(wParam)
//		{
//		case HWND_OWNER:
//			return (LRESULT)(HWND) lpPT->hOwner;
//		case HWND_DATAVIEW:
//			return (LRESULT)(HWND) lpPT->hDataView;
//		default:
//			break;
//		}
//		return TRUE;
//	case WM_CREATE:
//		{
//			OnCreateParticleTracker(lpPT, hwnd, wParam, lParam);
//		}
//		break;
//	case WM_NCCREATE:
//		{
//			lpPT = (PARTICLETRACKER *)InitWindowLongRACE(hwnd, sizeof(PARTICLETRACKER));
//			lpPT = lpPT;
//		}
//		break;
//	default: 
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//		
//	}
//
//	return DefWindowProc(hwnd, msg, wParam, lParam);
//}

//WM_DATAIN:
			//DWORD w = NULL;
			//DWORD h = NULL;
			//DWORD iSub = lpPT->iSub;
			//PDMFRAME *pdmFrame = (PDMFRAME *)wParam;

			//w = pdmFrame->pdmfh.width;
			//h = pdmFrame->pdmfh.height;
			////Should Assert Same Headers
			//FrameCopy(lpPT->pdmfIn, (PDMFRAME) *pdmFrame);

			//UCHAR *imgIn = (UCHAR *) pdmFrame->lpvoid;
			//UCHAR *imgRef = (UCHAR *) lpPT->pdmfRef.lpvoid;
			//UCHAR *imgDisp = (UCHAR *) lpPT->pdmfDisp.lpvoid;
			//UCHAR *imgPrev = (UCHAR *) lpPT->pdmfPrev.lpvoid;
			//INT *imgINT = (INT *)lpPT->pdmfINT.lpvoid;
			//INT *imgTemp = (INT *)lpPT->pdmfTemp.lpvoid;
			//INT *imgMask = (INT *)lpPT->pdmfMask.lpvoid;
			//PIXEL *imgPIXEL = (PIXEL *)lpPT->pdmfPIXEL.lpvoid;
			//UCHAR *imgMaskLon = (UCHAR *)lpPT->pdmfMaskLon.lpvoid;
			//UCHAR *imgMaskLat = (UCHAR *)lpPT->pdmfMaskLat.lpvoid;
			//UCHAR *imgMaskChn = (UCHAR *)lpPT->pdmfMaskChn.lpvoid;

			//if(!lpPT->flgGetRef){

			//	ParticleTracker_Track(lpPT, pdmFrame);

			//}else{
			//	ParticleTracker_CalculateReference(lpPT, pdmFrame);
			//}

			//lpPT->iSub = iSub;
			//if(lpPT->flgShow == PTS_FILTER){
			//	INTtoUCHAR(w, h, imgINT, imgPrev);
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPrev);
			//	return TRUE;
			//}else if(lpPT->flgShow == PTS_REFERENCE){
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfRef);
			//	return TRUE;
			//}else if(lpPT->flgShow == PTS_STREAM){
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfIn);
			//	return TRUE;
			//}else if(lpPT->flgShow == PTS_TRACKING){
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
			//	return TRUE;
			//}else if(lpPT->flgShow == PTS_MASK_LON){
			//	AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskLon, imgPIXEL);
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
			//	//DataView_ShowFrame(lpPT->hDataView, /*&lpPT->pdmfMaskLon*/);
			//	return TRUE;
			//}else if(lpPT->flgShow == PTS_MASK_LAT){
			//	AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskLat, imgPIXEL);
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
			//	//DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfMaskLat);
			//	return TRUE;
			//}else if(lpPT->flgShow == PTS_MASK_CHN){
			//	AlphaComposite(w, h, 0.05, AB_RED, imgIn, imgMaskChn, imgPIXEL);
			//	DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfPIXEL);
			//	//DataView_ShowFrame(lpPT->hDataView, &lpPT->pdmfMaskChn);
			//	return TRUE;
			//}else{}
			////SendMessage(lpPT->hDataView, WM_SHOWFRAME, (WPARAM) &lpPT->pdmfPIXEL, (LPARAM) lpPT->pdmfPIXEL.lpvoid);
			////SendMessage(lpPT->hDataView, WM_SHOWFRAME, (WPARAM) &lpPT->pdmfDisp, (LPARAM) lpPT->pdmfDisp.lpvoid);
			//return TRUE;