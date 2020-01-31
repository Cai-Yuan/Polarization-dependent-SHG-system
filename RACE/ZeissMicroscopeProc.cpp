#include "stdafx.h"

#include "DataManager.h"
#include "StageProperties.h"
#include "ZeissMicroscope.h"
#include "EnumerateDevice.h"
#include "StringFxns.h"
#include "Statistics.h"
#include "GaussianBlurr.h"
#include "Sum.h"
#include "Power.h"
#include "Point.h"
#include "ImageScale.h"
#include "WindowData.h"
#include "DerivativeOperator.h"


LRESULT CALLBACK ZeissMicroscopeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ZEISSMICROSCOPE *lpZeissScope = (ZEISSMICROSCOPE *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	INT i;
	INT pos;
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	switch(msg)
	{
	case WM_GETUSERDATA:
		return (LRESULT) lpZeissScope;
	//LOWORD(wParam) == nCharStr;
	//lParam = pointer to a string
	case ZSM_SETEXPERIMENT:
		memcpy(&lpZeissScope->ze, (VOID *)wParam, sizeof(ZEISSEXPERIMENT));
		break;
	case ZSM_SETZSTACK:
		memcpy(&lpZeissScope->zs, (VOID *)wParam, sizeof(ZSTACK));
		break;
	case ZSM_SETFIELDOFVIEW:
		memcpy(&lpZeissScope->fov, (VOID *)wParam, sizeof(FIELDOFVIEW));
		break;
	case ZSM_SETFILEPATH:
		{
			if(lParam){
				memcpy(lpZeissScope->strRootPath, (VOID *)lParam, LOWORD(wParam)*sizeof(CHAR));
			}
		}
		return TRUE;
	case ZSM_SETCHANNEL:
		switch(wParam){
		case CHNL_BRIGHTFIELD:
			SerialSend(lpZeissScope->hMicroscope, "HPCR1,4", NULL);
			break;
		case CHNL_CY5:
			SerialSend(lpZeissScope->hMicroscope, "HPCR1,5", NULL);
			break;
		case CHNL_DAPI:
			SerialSend(lpZeissScope->hMicroscope, "HPCR1,1", NULL);
			break;
		case CHNL_FITC:
			SerialSend(lpZeissScope->hMicroscope, "HPCR1,2", NULL);
			break;
		case CHNL_RHODAMINE:
			SerialSend(lpZeissScope->hMicroscope, "HPCR1,3", NULL);
			break;
		default:
			break;
		}
		break;
	case ZSM_SETCHANNELMASK:
		memcpy(&lpZeissScope->mc, (VOID *)lParam, sizeof(MULTICHANNEL));
		lpZeissScope->mc.filter = lpZeissScope->mc.filter;
		break;
	case ZSM_SETOBJECTIVE:
		switch(wParam){
		case OBJECTIVE_10X:
			SerialSend(lpZeissScope->hMicroscope, "HPCR2,1", NULL);
			break;
		case OBJECTIVE_40XOIL:
			SerialSend(lpZeissScope->hMicroscope, "HPCR2,2", NULL);
			break;
		case OBJECTIVE_63XOIL:
			SerialSend(lpZeissScope->hMicroscope, "HPCR2,3", NULL);
			break;
		case OBJECTIVE_4X:
			SerialSend(lpZeissScope->hMicroscope, "HPCR2,4", NULL);
			break;
		case OBJECTIVE_20X:
			SerialSend(lpZeissScope->hMicroscope, "HPCR2,5", NULL);
			break;
		case OBJECTIVE_40X:
			SerialSend(lpZeissScope->hMicroscope, "HPCR2,6", NULL);
			break;
		default:
			break;
		}
		break;
	case ZSM_SETLIGHTPATH:
		if(wParam == LP_EYEPIECE){
			SerialSend(lpZeissScope->hMicroscope, "HPCR39,1", NULL);
		}
		if(wParam == LP_CAMERA){
			SerialSend(lpZeissScope->hMicroscope, "HPCR39,2", NULL);
		}
		break;
	case ZSM_SETAXIS:

		switch(wParam){
		case ZAXIS_VELOCITY:
			lpZeissScope->zAxisVelocity = (INT) lParam;
			break;
		case ZAXIS_DELAY:
			lpZeissScope->zAxisDelay = (INT) lParam;
		default:
			break;
		}

		return TRUE;
	case ZSM_SETPOS:

		memset(str, NULL, MAX_STRING*sizeof(CHAR));


		if(wParam == XAXIS){
			pos = (INT) lParam;
			sprintf(str, "%s%d", "MOVE x=", pos);
			SerialSend(lpZeissScope->hStage, str, strAnswer, 10);
			return TRUE;
		}

		if(wParam == YAXIS){
			pos = (INT) lParam;
			sprintf(str, "%s%d", "MOVE y=", pos);
			SerialSend(lpZeissScope->hStage, str, strAnswer, 10);
			return TRUE;
		}

		if(wParam == ZAXIS){
			pos = (INT) lParam;
			CHAR strHex[MAX_STRING];
			memset(str, NULL, MAX_STRING*sizeof(CHAR));
			if(pos > 0){
				sprintf(str, "%s%6X", "FPZT", pos);
			}else{
				sprintf(strHex, "%6X", pos);
				sprintf(str, "%s%s", "FPZT", strHex+(strlen(strHex)-6));

			}
			CharReplace(str, 32, '0');
			SerialSend(lpZeissScope->hMicroscope, "FPZV28", NULL);
			SerialSend(lpZeissScope->hMicroscope, str, NULL);
			return TRUE;
		}

		if(wParam == (XAXIS|YAXIS|ZAXIS)){
			POINT3D p3D;
			memcpy(&p3D, (VOID *)lParam, sizeof(POINT3D));

			sprintf(str, "%s%d%s%d", "MOVE x=", (INT)p3D.x, " y=", (INT)p3D.y);
			SerialSend(lpZeissScope->hStage, str, strAnswer, 10);

			SendMessage(hwnd, ZSM_SETPOS, ZAXIS, p3D.z);
			return TRUE;
		}

		return TRUE;
	case ZSM_GETPOS:
		pos = NULL;
		memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

		if(wParam == (XAXIS|YAXIS|ZAXIS)){
			INT x;
			INT y;
			POINT3D p3D;
			x = GetXAxisPos(lpZeissScope->hStage);
			y = GetYAxisPos(lpZeissScope->hStage);
			//SerialSend(lpZeissScope->hStage, "WHERE XY", strAnswer, 20);

			//sscanf(strAnswer+2, "%d %d", &x, &y);

			memset(&p3D, NULL, sizeof(POINT3D));
			p3D.x = x;
			p3D.y = y;
			p3D.z = GetFocalPos(lpZeissScope->hMicroscope);

			memcpy((VOID *)lParam, &p3D, sizeof(POINT3D));
			return TRUE;
		}
		if(wParam == XAXIS){
			return GetXAxisPos(lpZeissScope->hStage);
		}
		if(wParam == YAXIS){
			return GetYAxisPos(lpZeissScope->hStage);
		}
		if(wParam == ZAXIS){
			return GetZAxisPos(lpZeissScope->hStage);
		}
		break;
	case ZSM_SETSPEED:
		{
			INT speed = lParam;
			SetAxisSpeed(lpZeissScope->hStage, lParam, XAXIS|YAXIS);

		}
		return TRUE;

	//WPARAM - Pointer to AUTOFOCUS struct
	//LPARAM - Flag to indicate if autofocus should begin or not (Begin = TRUE, Idle = FALSE)
	case ZFM_SETAUTOFOCUS:
		if(lpZeissScope->flgAutoFocus){
			lpZeissScope->flgAutoFocus = FALSE;
			return FALSE;
		}

		if(wParam){
			memcpy(&lpZeissScope->af, (VOID *)wParam, sizeof(AUTOFOCUS));
		}
		//lpZeissScope->fStep = HIWORD(wParam);
		//lpZeissScope->fNumStep = LOWORD(wParam);
			
		if(lParam){

			lpZeissScope->flgAutoFocus = TRUE;
			lpZeissScope->flgFocusInit = TRUE;

			{
				HWND *hTemp;
				hTemp = (HWND *)calloc(1, sizeof(HWND));
				memcpy(hTemp, &hwnd, sizeof(HWND));
				CreateThread(NULL, NULL, AutoFocusThread, hTemp, NULL, NULL);
			}
		}

		return TRUE;
	case ZSM_AUTOFOCUS:

		if(lpZeissScope->flgAutoFocus){
			lpZeissScope->flgAutoFocus = FALSE;
			return FALSE;
		}

		lpZeissScope->fStep = HIWORD(wParam);
		lpZeissScope->fNumStep = LOWORD(wParam);
			

		//lpZeissScope->flgGetFrame = TRUE;
		lpZeissScope->flgAutoFocus = TRUE;
		lpZeissScope->flgFocusInit = TRUE;

		{
			HWND *hTemp;
			hTemp = (HWND *)calloc(1, sizeof(HWND));
			memcpy(hTemp, &hwnd, sizeof(HWND));
			CreateThread(NULL, NULL, AutoFocusThread, hTemp, NULL, NULL);
		}

		return TRUE;
	case ZSM_EXIT:
		lpZeissScope->flgExit = TRUE;
		return TRUE;
	case ZSM_ACQUISITION:
		MULTIFOV multiFOV;
		if(wParam){
			memcpy(&lpZeissScope->mfov, (VOID *)wParam, sizeof(MULTIFOV));
		}
		else{
			lpZeissScope->mfov.nCol = lpZeissScope->fov.nCol;
			lpZeissScope->mfov.nRow = lpZeissScope->fov.nRow;
			lpZeissScope->mfov.overlap = lpZeissScope->fov.overlap;
		}

		{		
			HWND *hTemp;
			hTemp = (HWND *)calloc(1, sizeof(HWND));
			memcpy(hTemp, &hwnd, sizeof(HWND));
		

			CreateThread(NULL, NULL, MultiFOVCapture, hTemp, NULL, NULL);
		}

		return TRUE;
	case ZSM_STARTEXPERIMENT:
		{
			lpZeissScope->ze.flgRun = TRUE;
			HWND *hZeissProc;
			hZeissProc = (HWND *)malloc(sizeof(HWND));
			memcpy(hZeissProc, &hwnd, sizeof(HWND));
			CreateThread(NULL, NULL, ZeissExperimentThread, hZeissProc, NULL, NULL);
			//CreateThread
		}
		return TRUE;
	case ZSM_STOPEXPERIMENT:
		{
			lpZeissScope->ze.flgRun = FALSE;
		}
		return TRUE;
	case WM_GETLINKFLAG:
		return (LRESULT) &lpZeissScope->flgGetFrame;
	case WM_DATADROP:		
		INT w;
		INT h;
		
		PDMFRAME pdmFrame;

		w = lpZeissScope->w;
		h = lpZeissScope->h;

		lpZeissScope->flgDataIn = TRUE;
		lpZeissScope->flgGetFrame = FALSE;
		memcpy(&pdmFrame, (PDMFRAME *)wParam, sizeof(PDMFRAME));
		memcpy(lpZeissScope->img, pdmFrame.lpvoid, w*h);
		
		if(pdmFrame.lpvoid) free(pdmFrame.lpvoid);
		if((VOID *)wParam) free((VOID *)wParam);

		return TRUE;
	case WM_SETFRAMESIZE:
		h = (INT) lParam;
		w = (INT) wParam;
		lpZeissScope->w = w;
		lpZeissScope->h = h;

		lpZeissScope->img = (UCHAR *)calloc(w*h, sizeof(UCHAR));

		return TRUE;
	case WM_CREATE:		
		lpZeissScope->hwnd = hwnd;
		//SetTimer(hwnd, NULL, 500, NULL);
		return TRUE;
	case WM_TIMER:
		INT xPos;
		INT yPos;

		lpZeissScope->zAxisPos = GetFocalPos(lpZeissScope->hMicroscope);

		SendMessage(lpZeissScope->hStageControl, WM_SETPOS, ZAXIS, lpZeissScope->zAxisPos);
		return TRUE;
	case WM_SETHWND:
		if(wParam == HWND_OWNER){
			lpZeissScope->hOwner = (HWND)lParam;

			HWND *h;
			h = (HWND *)malloc(sizeof(HWND));
			if(h){
				memcpy(h, &hwnd, sizeof(HWND));
				CreateThread(NULL, NULL, InitZeissMicroscopeThread, (LPVOID) h, NULL, NULL);
			}
			lpZeissScope->hCameraProperties = (HWND) SendMessage(lpZeissScope->hOwner,
														WM_GETHWND,
														HWND_CAMERAPROPERTIES,
														NULL);

			lpZeissScope->hCameraProperties = lpZeissScope->hCameraProperties;
		}
		if(wParam == HWND_PROPERTYSHEET){
			lpZeissScope->hPropertySheet = (HWND) lParam;
		}
		if(wParam == HWND_EXPERIMENT){
			lpZeissScope->hExperiment = (HWND) lParam;
		}
		if(wParam == HWND_MULTICHANNEL){
			lpZeissScope->hMultiChannel = (HWND) lParam;
		}
		if(wParam == HWND_AUTOFOCUS){
			lpZeissScope->hAutoFoucs = (HWND) lParam;
		}
		if(wParam == HWND_MOSAIC){
			lpZeissScope->hMosaic = (HWND) lParam;
		}
		if(wParam = HWND_ZSTACK){
			lpZeissScope->hZStack = (HWND) lParam;
		}
		if(wParam == HWND_OBJECTIVE){
			lpZeissScope->hObjective = (HWND) lParam;
		}

		return TRUE;
	case WM_NCCREATE:
		{
			//Allocate Window Pointer Memory
			lpZeissScope = (ZEISSMICROSCOPE *)malloc(sizeof(ZEISSMICROSCOPE));
			memset(lpZeissScope, NULL, sizeof(ZEISSMICROSCOPE));
			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpZeissScope);
			lpZeissScope = (ZEISSMICROSCOPE *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
			if(!lpZeissScope){
				MessageBox(hwnd, (LPCSTR)"ZiessMicroscopeWnd: GetWindowLongPtr Failed", (LPCSTR)"Error!", MB_OK);
			}

			break;
		}
	default:
		break;
	}

	return TRUE;

}

DWORD WINAPI InitZeissMicroscopeThread(LPVOID param)
{
	HWND hwnd = NULL;
	ZEISSMICROSCOPE *lpZeissScope = NULL;

	if(!param){
		return FALSE;
	}
	else{
		memcpy(&hwnd, param, sizeof(HWND));
		free(param);
	}

	lpZeissScope = (ZEISSMICROSCOPE *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if(!lpZeissScope)
		return FALSE;

	//lpZeissScope->hOwner = (HWND)lParam;
	Sleep(1000);
	lpZeissScope->hMicroscope = GetZeissMicroscope(lpZeissScope->hOwner, 8);
	lpZeissScope->hStage = GetZeissMicroscopeStage(lpZeissScope->hOwner, 1);
	SerialSend(lpZeissScope->hStage, "", NULL);
	SerialSend(lpZeissScope->hStage, "", NULL);
	SendMessage(lpZeissScope->hOwner, WM_SETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, (LPARAM)hwnd);
	lpZeissScope->hStageControl = (HWND) SendMessage(lpZeissScope->hOwner, WM_GETHWND, HWND_STAGECONTROLS, NULL);

	SetObjective(lpZeissScope->hMicroscope, 5);
	SetReflector(lpZeissScope->hMicroscope, /*2*/4);
	SetLightPath(lpZeissScope->hMicroscope, LIGHTPATH_LSIDEPORT);
	//SetLampVoltage(lpZeissScope->hMicroscope, 250);


	INT objective = GetObjective(lpZeissScope->hMicroscope);
	objective = objective;

	INT reflector = GetReflector(lpZeissScope->hMicroscope);
	reflector = reflector;

	INT posFocus = GetFocalPos(lpZeissScope->hMicroscope);
	SendMessage(lpZeissScope->hStageControl, WM_SETPOS, (WPARAM) ZAXIS, (LPARAM) posFocus);
	return TRUE;

}

//case ZSM_ZSTEP_POS:
//	memset(str, NULL, MAX_STRING*sizeof(CHAR));
//	sprintf(str, "%s%6X", "PFZG", lpZeissScope->zAxisVelocity);
//	CharReplace(str, 32, '0');
//
//	SerialSend(lpZeissScope->hMicroscope, str, NULL);
//	SerialSend(lpZeissScope->hMicroscope, "FPZS+", NULL);
//	SerialSend(lpZeissScope->hMicroscope, "HPSb6", NULL);
//	Sleep(lpZeissScope->zAxisDelay);
//	SerialSend(lpZeissScope->hMicroscope, "FPZS", NULL);
//	SerialSend(lpZeissScope->hMicroscope, "FPNr", strAnswer);
//
//	ThrowMessage(lpZeissScope->hOwner, strAnswer);
//	
//	return TRUE;
//case ZSM_ZSTEP_NEG:
//	memset(str, NULL, MAX_STRING*sizeof(CHAR));
//	sprintf(str, "%s%6X", "PFZG", lpZeissScope->zAxisVelocity);
//	CharReplace(str, 32, '0');
//		
//	SerialSend(lpZeissScope->hMicroscope, str, NULL);
//	SerialSend(lpZeissScope->hMicroscope, "FPZS-", NULL);			
//	SerialSend(lpZeissScope->hMicroscope, "HPSb6", NULL);
//	Sleep(lpZeissScope->zAxisDelay);
//	SerialSend(lpZeissScope->hMicroscope, "FPZS", NULL);
//	SerialSend(lpZeissScope->hMicroscope, "FPNr", strAnswer);
//
//	ThrowMessage(lpZeissScope->hOwner, strAnswer);
//	
//	return TRUE;

DWORD WINAPI ZeissMicroscopeThread(VOID *lpvoid)
{
	HWND hOwner;
	if(lpvoid){
		memcpy(&hOwner, lpvoid, sizeof(HWND));
		free(lpvoid);
		lpvoid = NULL;
	}

	MSG msg = {0};
	HWND hwnd;
	WNDCLASSEX wc = {0};

	hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, 
		"ZeissMicroscope", 
		NULL, 
		WS_OVERLAPPEDWINDOW
		|WS_CLIPCHILDREN, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		0, 0, 
		HWND_MESSAGE, 
		NULL, 
		GetModuleHandle(0), 
		NULL);

	if(!hwnd){
		//Throw Error
		ThrowError(hwnd, "Failded to Create Thread ZiessMicroscope");
		return FALSE;
	}

	//Initialize Timer
	//SetTimer(hwnd, 1, 50, NULL);

	//Send the Creating Window the HWND of the new data manager
	SendMessage(hOwner, WM_REGISTERWINDOW, (WPARAM)hwnd, (LPARAM)"ZeissMicroscope");

	//Initialize ZeissMicroscope Owner Handle
	//WARNING: Ownder handle must be initialized before anything else
	SendMessage(hwnd, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM) hOwner);
	
	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}

DWORD CreateZeissMicroscopeProc(HWND hOwner)
{
	HWND *hwnd = (HWND *) calloc(1, sizeof(HWND));
	memcpy(hwnd, &hOwner, sizeof(HWND));
	CreateThread(NULL, NULL, ZeissMicroscopeThread, hwnd, NULL, NULL);
	//Sleep(50);

	return TRUE;
}


DWORD UpdateVariance(ZEISSMICROSCOPE *lpZeissScope)
{
	FLOAT varTemp;

	lpZeissScope->varPrev = lpZeissScope->varCur;

	return TRUE;
}


DWORD WINAPI AutoFocusThread(LPVOID param)
{
	HWND hwnd;
	HANDLE hObjective;
	ZEISSMICROSCOPE *lpZeissScope;
	
	INT n = 3;
	FLOAT sigma = 1.0;

	if(param){
		memcpy(&hwnd, (VOID *)param, sizeof(HWND));
		free(param);
		param = NULL;
	}
	else
		return NULL;

	lpZeissScope = (ZEISSMICROSCOPE *)SendMessage(hwnd, WM_GETUSERDATA, NULL, NULL);

	if(!lpZeissScope)
		return FALSE;

	AutoFocus(lpZeissScope);

	//hObjective = lpZeissScope->hMicroscope;


	//INT i = NULL;
	//INT nStep = lpZeissScope->fNumStep;
	//INT stepSize = lpZeissScope->fStep;
	//INT ref = GetFocalPos(hObjective);
	//INT current = ref - ((nStep/2)*stepSize);

	//FLOAT max = NULL;
	//INT maxPos = current;


	//FLOAT *measure = (FLOAT *)calloc(nStep, sizeof(FLOAT));

	//if(!measure)
	//	return FALSE;

	//UINT w = lpZeissScope->w;
	//UINT h = lpZeissScope->h;

	//FLOAT *fimg = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	//if(!fimg)
	//	return FALSE;

	//memset(measure, NULL, nStep*sizeof(FLOAT));

	//SetFocalPos(lpZeissScope, current);
	//Sleep(50);
	//for(i = 0; i < nStep; i++)
	//{
	//	//GetFocalImage(lpZeissScope, w, h, fimg);
	//	GetFocalImage(lpZeissScope, w, h, fimg);
	//	SetFocalPos(hObjective, current);
	//	Sleep(10);
	//	measure[i] = GetFocalMeasure(w, h, fimg);

	//	if(measure[i] > max){
	//		max = measure[i];
	//		maxPos = current;
	//	}
	//	
	//	current = current+stepSize;

	//	if(HandleExitFlag(lpZeissScope)){
	//		i = nStep;
	//		break;
	//	}

	//}

	//SetFocalPos(hObjective, maxPos);

	//lpZeissScope->flgAutoFocus = FALSE;

	//if(fimg)free(fimg);
	//if(measure)free(measure);


	return TRUE;
}

//Handles the checking and reseting of the automation exit flag
//Returns true if the flag required an exit
DWORD HandleExitFlag(ZEISSMICROSCOPE *lpZeissScope)
{

	if(lpZeissScope->flgExit){
		lpZeissScope->flgExit = FALSE;
		return TRUE;
	}
	
	return FALSE;
}

DWORD WINAPI MultiFOVCapture(LPVOID param)
{
	HWND hwnd;

	HANDLE hStage;
	HANDLE hObjective;
	
	ZEISSMICROSCOPE *lpZeissScope;

	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	
	INT i = NULL;
	INT r = NULL;
	INT c = NULL;
	INT n = NULL;
	INT w = NULL;
	INT h = NULL;

	if(param){
		memcpy(&hwnd, (VOID *)param, sizeof(HWND));
		free(param);
		param = NULL;
	}
	else
		return NULL;

	lpZeissScope = (ZEISSMICROSCOPE *)SendMessage(hwnd, WM_GETUSERDATA, NULL, NULL);

	if(!lpZeissScope)
		return FALSE;

	w = lpZeissScope->w;
	h = lpZeissScope->h;

	hStage = lpZeissScope->hStage;
	hObjective = lpZeissScope->hMicroscope;

	MULTIFOV mfov = {0};
	memcpy(&mfov, &lpZeissScope->mfov, sizeof(MULTIFOV));

	n = mfov.nRow*mfov.nCol;

	UCHAR **imgMap = (UCHAR **) calloc(n, sizeof(UCHAR*));

	for(i = 0; i < n; i++){
		imgMap[i] = (UCHAR *) calloc(w*h, sizeof(UCHAR));
		memset(imgMap[i], NULL, w*h*sizeof(UCHAR));
	}

	//INT wUM = FOV_X_20x;
	//INT hUM = FOV_Y_20x;

	INT wUM = FOV_X_10x;
	INT hUM = FOV_Y_10x;

	//INT wUM = FOV_X_40x;
	//INT hUM = FOV_Y_40x;

	//Position in uM
	//INT xPos = (GetXAxisPos(hStage)/ TICKPERUM_20x);
	//INT yPos = (GetYAxisPos(hStage)/ TICKPERUM_20x);

	//Position in uM
	INT xPos = GetXAxisPos(hStage, SP_MICRON);
	INT yPos = GetYAxisPos(hStage, SP_MICRON);
	//INT xPos = (GetXAxisPos(hStage)/ TICKPERUM_20x);
	//INT yPos = (GetYAxisPos(hStage)/ TICKPERUM_20x);
	INT zPos = (GetZAxisPos(hObjective));

	//Step in uM
	INT xStep = (1*wUM) - (wUM*mfov.overlap);
	INT yStep = (1*hUM) - (hUM*mfov.overlap);

	//in uM
	INT xGoTo = NULL;
	INT yGoTo = NULL;

	GetStreamImage(lpZeissScope, w, h, (UCHAR *) NULL);

	SetAxisSpeed(hStage, 1000, XAXIS|YAXIS);

	i = NULL;
	for(r = 0; r < mfov.nRow; r++){

		for(c = 0; c < mfov.nCol; c++){

			//yGoTo = ((yPos+(r*yStep))*STEPPERUM/*_20x*/);
			//xGoTo = ((xPos-(c*xStep))*STEPPERUM/*_20x*/);
			xGoTo = ((xPos-(c*xStep)));
			yGoTo = ((yPos+(r*yStep)));
			

			SetYAxisPos(hStage, yGoTo, SP_MICRON);
			SetXAxisPos(hStage, xGoTo, SP_MICRON);

			WaitForPosition(hStage, xGoTo, yGoTo, SP_MICRON);

			AutoFocus(lpZeissScope);

			SetYAxisPos(hStage, yGoTo, SP_MICRON);
			SetXAxisPos(hStage, xGoTo, SP_MICRON);
			WaitForPosition(hStage, xGoTo, yGoTo, SP_MICRON);
			
			GetStreamImage(lpZeissScope, w, h, imgMap[(r*mfov.nCol)+c]);
			//GetStreamImage(lpZeissScope, w, h, imgMap[(r*mfov.nCol)+c]);

			sprintf(str, "%s(%d, %d).bmp", "C:\\", r, c);
			SaveBMP(w, h, str, imgMap[(r*mfov.nCol)+c]);
			i++;

			if(HandleExitFlag(lpZeissScope)){
				c = mfov.nCol;
				r = mfov.nRow;
			}

		}
	}

	SetXAxisPos(hStage, xPos, SP_MICRON);
	SetYAxisPos(hStage, yPos, SP_MICRON);
	SetZAxisPos(hObjective, zPos);

	StitchFOV(w, h, mfov.nRow, mfov.nCol, xStep, yStep, imgMap);


	if(imgMap){
		for(i = 0; i < n; i++){
			if(imgMap[i])free(imgMap[i]);
		}
		free(imgMap);
	}


	return TRUE;
}

DWORD StitchFOV(UINT w, UINT h, UINT nRow, UINT nCol, UINT xStep, UINT yStep, UCHAR **imgMap)
{
	DWORD wStitch = (nCol-1)*(xStep*PIX_PER_UM_X)+w;
	DWORD hStitch = (nRow-1)*(yStep*PIX_PER_UM_Y)+h;

	wStitch = wStitch + (4-(wStitch%4));


	UCHAR *img = NULL;
	UCHAR *imgStitch = (UCHAR *) calloc(wStitch*hStitch, sizeof(UCHAR));
	memset(imgStitch, NULL, wStitch*hStitch*sizeof(UCHAR));
	//memset(imgStitch, 1, wStitch*hStitch);

	DWORD xPos = NULL;
	DWORD yPos = NULL;

	DWORD x = NULL;
	DWORD y = NULL;


	DWORD p = NULL;
	INT iImg = NULL;
	DWORD iStch = NULL;
	DWORD n = nRow*nCol;

	DWORD r = NULL;
	DWORD c = NULL;


	for(r = 0; r < nRow; r++){
		for(c = 0; c < nCol; c++){
			yPos = (r*(yStep*PIX_PER_UM_Y));
			xPos = (c*(xStep*PIX_PER_UM_X));

			//if(yPos < 0)
			//	yPos = 0;

			//if(xPos < 0)
			//	xPos = 0;

			img = imgMap[(r*nCol)+(c)];
			for(y = 0; y < h; y++){
				for(x = 0; x < w; x++){
					iImg = (y*w)+x;
					iStch = ((yPos+y)*wStitch) + (x+xPos);
					if(Range((DWORD)wStitch, (DWORD)hStitch, iStch) && Range((DWORD)w, (DWORD)h, iImg)){
						if(!imgStitch[iStch]){
							imgStitch[iStch] = img[iImg];
						}
						else{
							imgStitch[iStch] = (imgStitch[iStch] + img[iImg])/2;
						}
					}
					else{
						INT m;
						m = 0;
						m = 0;
					}

					//iImg++;
					//iStch++;
				}
			}

		}
	}


	PIXEL *pix = (PIXEL *)calloc(wStitch*hStitch, sizeof(PIXEL));
	UCHARtoPIXEL(wStitch, hStitch, imgStitch, pix);

	SaveBMP(wStitch, hStitch, "C:\\MosaicOutput.bmp", /*imgStitch*/pix);

	if(pix)free(pix);
	if(imgStitch)free(imgStitch);


	return TRUE;
}

DWORD DirectCompareMosaic(UINT w, UINT h, UINT *wMosaic, UINT *hMosaic, UINT nRow, UINT nCol, FLOAT overlap, FLOAT **imgSet, UCHAR **mosaic)
{
	FLOAT searchFactor = 0.10;

	if(*mosaic){
		return FALSE;
	}

	if(!imgSet){
		return FALSE;
	}

	INT i = NULL;
	INT iMap = NULL;
	INT iSrc = NULL;
	INT iRef = NULL;

	DWORD x = NULL;
	DWORD y = NULL;

	DWORD xRef = NULL;
	DWORD yRef = NULL;
	DWORD xSrc = NULL;
	DWORD ySrc = NULL;

	DWORD xStep = w - (w*overlap);
	DWORD yStep = h - (h*overlap);

	DWORD wMap = w*nCol - ((nCol-1)*(w*overlap));
	DWORD hMap = h*nRow - ((nRow-1)*(h*overlap));

	wMap = wMap + (4-(wMap%4));

	FLOAT *map = (FLOAT *) calloc(wMap*hMap, sizeof(FLOAT));

	INT txStart = xStep - (searchFactor*xStep);
	INT txEnd = xStep + (searchFactor*xStep);

	//INT tyStart = yStep - (searchFactor*yStep);
	//INT tyEnd = yStep + (searchFactor*yStep);

	INT tyStart = -(h*searchFactor)-20;
	INT tyEnd = h*searchFactor;

	//tyStart = 0;
	//tyEnd = 0;

	RECT rcSearch = {0};

	INT ty = NULL;
	INT tx = NULL;

	INT left = NULL;
	INT right = NULL;
	INT top = NULL;
	INT bottom = NULL;

	DOUBLE error = NULL;
	DOUBLE minError = 1000000000000000000;
	INT txBest = NULL;
	INT tyBest = NULL;

	FLOAT *imgRef = imgSet[0];
	FLOAT *imgSrc = imgSet[1];

	GradientSobel(w, h, 1, imgRef);
	GradientSobel(w, h, 1, imgSrc);

	for(ty = tyStart; ty < tyEnd; ty++){
		for(tx = txStart; tx < txEnd; tx++){

			left = tx;
			right = w+tx;
			top  = ty;
			bottom = h+ty;

			Clamp(1, 0, w, &left);
			Clamp(1, 0, w, &right);
			Clamp(1, 0, h, &top);
			Clamp(1, 0, h, &bottom);

			error = NULL;

			for(yRef = top; yRef < bottom; yRef++){
				for(xRef = left; xRef < right; xRef++){
				
					xSrc = xRef-tx;
					ySrc = yRef-ty;

					iRef = (yRef*w)+xRef;
					iSrc = (ySrc*w)+xSrc;

					if(Range(w, h, iSrc) && Range(w, h, iRef)){

						error = error + (imgSrc[iSrc]-imgRef[iRef])*(imgSrc[iSrc]-imgRef[iRef]);


					}

				}
			}

			error = error / ( (bottom-top)*(right-left) );

			if(error < minError){
				minError = error;
				txBest = tx;
				tyBest = ty;
			}

		}
	}

	for(y = 0; y < h; y++){
		for(x = 0; x < w; x++){

			i = (y*w)+x;
			iMap = (y*wMap)+x;

			if(Range(w, h, i) && Range(wMap, hMap, iMap)){
				map[iMap] = imgRef[i];
			}

		}
	}

	for(y = 0; y < h; y++){
		for(x = 0; x < w; x++){

			i = (y*w)+x;
			iMap = ((y+ty)*wMap)+(x+tx);

			if(Range(w, h, i) && Range(wMap, hMap, iMap)){
				map[iMap] = imgSrc[i];
			}

		}
	}

		

	UCHAR *img = (UCHAR *)malloc(wMap*hMap*sizeof(UCHAR));
	FLOATtoUCHAR(wMap, hMap, map, img);

	SaveBMP(wMap, hMap, "C:\\MosaicTest.bmp", img);

	
			
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "tx:%d  ty:%d", tx, ty);
	MessageBox(NULL, str, "Done!", MB_OK);

	if(map)free(map);
	if(img)free(img);



	return TRUE;
}

//DWORD DirectCompareMosaic(UINT w, UINT h, UINT *wMosaic, UINT *hMosaic, UINT nRow, UINT nCol, FLOAT overlap, FLOAT **imgSet, UCHAR **mosaic)
//{
//	FLOAT searchFactor = 0.01;
//
//	if(*mosaic){
//		return FALSE;
//	}
//
//	if(!imgSet){
//		return FALSE;
//	}
//
//	DWORD xStep = w - (w*overlap);
//	DWORD yStep = h - (h*overlap);
//
//	DWORD i = NULL;
//	DWORD iMap = NULL;
//
//	DWORD iSrc = NULL;
//	DWORD iRef = NULL;
//	DWORD x = NULL;
//	DWORD y = NULL;
//	DWORD wMap = w*nCol - ((nCol-1)*(w*overlap));
//	DWORD hMap = h*nRow - ((nRow-1)*(h*overlap));
//
//	wMap = wMap + (4-(wMap%4));
//
//	FLOAT *map = (FLOAT *) calloc(wMap*hMap, sizeof(FLOAT));
//
//	FLOAT tx = NULL;
//	FLOAT ty = NULL;
//
//	INT xt = NULL;
//	INT yt = NULL;
//
//	INT txStart = xStep - (searchFactor*xStep);
//	INT tyStart = yStep - (searchFactor*yStep);
//	INT txEnd = xStep + (searchFactor*xStep);
//	INT tyEnd = yStep + (searchFactor*yStep);
//
//	FLOAT xOff = NULL;
//	FLOAT yOff = NULL;	
//
//	INT xSrc = NULL;
//	INT ySrc = NULL;
//	FLOAT xRef = NULL;
//	FLOAT yRef = NULL;
//
//	FLOAT error = NULL;
//	FLOAT iError = NULL;
//
//	FLOAT *imgRef = imgSet[0];
//	FLOAT *imgSrc = imgSet[1];
//	//Loop: For offset
//	for(yt = tyStart; yt < tyEnd; ty++){
//		for(xt = txStart; xt < txEnd; tx++){
//
//			//Normalized Offsets
//			tx = ((FLOAT)xt / (FLOAT)w);
//			ty = ((FLOAT)yt / (FLOAT)h);
//
//			error = NULL;
//			iError = NULL;
//			for(y = 0; y < h; y++){
//
//				for(x = 0; x < w; x++){
//
//					iRef = (y*w)+x;
//
//					GetNormalizedCoordinate(w, h, x, h, &xRef, &yRef);
//					GetPixelCoordinate(w, h, xRef+tx, yRef+ty, &xSrc, &ySrc);
//
//					iSrc = (ySrc*w)+xSrc;
//					if(Range((DWORD)w, (DWORD)h, iRef) && Range((DWORD)w, (DWORD)h, iSrc)){
//						error = error + (imgRef[iRef]-imgSrc[iSrc])*(imgRef[iRef]-imgSrc[iSrc]);
//						iError++;
//
//					}
//
//				}
//
//			}
//
//
//
//		}
//	}
//
//	if(map)
//		free(map);
//
//	return TRUE;
//}


DWORD AdjustFocus(HWND hwnd, ZEISSMICROSCOPE *lpZeissScope)
{

	if(lpZeissScope->dirFocus){
		SendMessage(hwnd, ZSM_ZSTEP_POS, NULL, NULL);
		lpZeissScope->flgGetFrame = TRUE;
	}
	else{
		SendMessage(hwnd, ZSM_ZSTEP_NEG, NULL, NULL);
		lpZeissScope->flgGetFrame = TRUE;
	}

	return TRUE;
}

DWORD AutoFocus(ZEISSMICROSCOPE *lpZeissScope)
{
	HANDLE hObjective;

	if(!lpZeissScope)
		return FALSE;

	hObjective = lpZeissScope->hMicroscope;


	INT i = NULL;
	INT nStep = lpZeissScope->af.nStep;
	INT stepSize = lpZeissScope->af.stepDist;
	INT fltrFocus = lpZeissScope->af.filter;
	//INT nStep = lpZeissScope->fNumStep;
	//INT stepSize = lpZeissScope->fStep;
	INT ref = GetFocalPos(hObjective);
	INT current = ref - ((nStep/2)*stepSize);

	FLOAT max = NULL;
	INT maxPos = current;


	FLOAT *measure = (FLOAT *)calloc(nStep, sizeof(FLOAT));

	if(!measure)
		return FALSE;

	UINT w = lpZeissScope->w;
	UINT h = lpZeissScope->h;

	UCHAR *img = (UCHAR *)calloc(w*h, sizeof(UCHAR));
	FLOAT *fimg = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	if(!fimg)
		return FALSE;

	memset(measure, NULL, nStep*sizeof(FLOAT));

	SetFocalPos(hObjective, current, AF_WAIT);

	//SetFocalPos(lpZeissScope, current);
	//SetFocalPos(lpZeissScope, current);
	//SetFocalPos(lpZeissScope, current);

	for(i = 0; i < nStep; i++)
	{
		//WaitForFocalPos(hObjective, current);

		GetStreamImage(lpZeissScope, w, h, img);
		//GetStreamImage(lpZeissScope, w, h, img);

		//Set Focal Plane to the next position
		SetFocalPos(hObjective, current+stepSize, AF_WAIT);

		UCHARtoFLOAT(w, h, img, fimg);
		//measure[i] = GetFocalMeasure(w, h, fimg);
		measure[i] = GetFocalMeasure(w, h, fimg, fltrFocus);

		if(measure[i] > max){
			max = measure[i];
			maxPos = current;
		}
		
		current = current+stepSize;

		if(lpZeissScope->flgExit){
			maxPos = ref;
			i = nStep;
		}

		//CHAR str[MAX_STRING];
		//memset(str, NULL, MAX_STRING*sizeof(CHAR));
		//sprintf(str, "C:\\%d.bmp", i);
		//SaveBMP(w, h, str, img);

	}

	SetFocalPos(hObjective, maxPos, AF_WAIT);

	lpZeissScope->flgAutoFocus = FALSE;

	if(fimg)free(fimg);
	if(measure)free(measure);


	return TRUE;
}

FLOAT GetFocalMeasure(UINT w, UINT h, FLOAT *img)
{
	
	INT n = 3;
	FLOAT sigma = 1.0;
	FLOAT measure = NULL;
	FLOAT mLaplace = NULL;
	FLOAT mSobel = NULL;
	FLOAT mNormVar = NULL;

	//FLOAT *imgLaplace = (FLOAT *)calloc(w*h, sizeof(FLOAT));
	FLOAT *imgSobel = (FLOAT *)calloc(w*h, sizeof(FLOAT));
	FLOAT *imgSqr = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	//GaussianBlur(w, h, n, sigma, img);
	//Laplacian(w, h, 1, img, imgLaplace);
	//mLaplace = Sum(w*h, imgLaplace);
	//Square(w*h, imgLaplace, imgSqr);
	//mLaplace = Sum(w*h, imgSqr);

	GradientSobel(w, h, 1, img, imgSobel);
	Square(w*h, imgSobel, imgSqr);
	mSobel = Sum(w*h, imgSqr);
	//mSobel = Sum(w*h, imgSobel);

	//mNormVar = NormalizedVariance(w*h, img, NULL);

	//measure = Variance(w*h, img, NULL);

	//if(imgLaplace)free(imgLaplace);
	if(imgSobel)free(imgSobel);
	//if(imgSqr)free(imgSqr);

	//mLaplace = mLaplace + mSobel;
	//measure = (FLOAT)(mLaplace+mSobel) / (FLOAT) 2.0;
	measure = (FLOAT)mSobel/* + (FLOAT)mLaplace*/;

	return (FLOAT) measure;
}

FLOAT GetFocalMeasure(UINT w, UINT h, FLOAT *img, DWORD filter)
{
	
	INT n = 3;
	FLOAT sigma = 1.0;
	FLOAT measure = NULL;
	FLOAT mLaplace = NULL;
	FLOAT mSobel = NULL;
	FLOAT mNormVar = NULL;


	FLOAT *imgLaplace = NULL;
	FLOAT *imgSobel = NULL;
	FLOAT *imgSqr = NULL;

	imgSqr = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	if((filter&AF_SOBEL)){
		imgSobel = (FLOAT *)calloc(w*h, sizeof(FLOAT));
		GradientSobel(w, h, 1, img, imgSobel);
		Square(w*h, imgSobel, imgSqr);
		measure += Sum(w*h, imgSqr);
	}
	if((filter&AF_LAPLACIAN)){
		imgLaplace = (FLOAT *)calloc(w*h, sizeof(FLOAT));
		Laplacian(w, h, 1, img, imgLaplace);
		Square(w*h, imgLaplace, imgSqr);
		measure += Sum(w*h, imgSqr);
	}
	if((filter&AF_VARIANCE)){
		Square(w*h, img, imgSqr);
		measure += NormalizedVariance(w*h, imgSqr, NULL);
	}




	//GaussianBlur(w, h, n, sigma, img);
	//Laplacian(w, h, 1, img, imgLaplace);
	//mLaplace = Sum(w*h, imgLaplace);
	//Square(w*h, imgLaplace, imgSqr);
	//mLaplace = Sum(w*h, imgSqr);

	//GradientSobel(w, h, 1, img, imgSobel);
	//Square(w*h, imgSobel, imgSqr);
	//mSobel = Sum(w*h, imgSqr);
	//mSobel = Sum(w*h, imgSobel);

	//mNormVar = NormalizedVariance(w*h, img, NULL);

	//measure = Variance(w*h, img, NULL);

	if(imgLaplace)free(imgLaplace);
	if(imgSobel)free(imgSobel);
	if(imgSqr)free(imgSqr);

	//mLaplace = mLaplace + mSobel;
	//measure = (FLOAT)(mLaplace+mSobel) / (FLOAT) 2.0;
	//measure = (FLOAT)mSobel/* + (FLOAT)mLaplace*/;

	return (FLOAT) measure;
}

//Sets a flag and waits for the 'lpZeissScope->img' pointer to be filled
//Returns a floating point version of 'lpZeissScope->img'
DWORD GetFocalImage(ZEISSMICROSCOPE *lpZeissScope, DWORD w, DWORD h, FLOAT *fimg)
{

	INT cnt = NULL;
	INT cntMax = 500;
	lpZeissScope->flgGetFrame = TRUE;

	for(cnt = 0; cnt < cntMax; cnt++){
		//lpZeissScope->flgGetFrame = TRUE;
		
		if(lpZeissScope->flgDataIn)
			break;
		Sleep(1);
	}

	if(cnt == cntMax)
		MessageBox(NULL, "GetFocalImage Failed", "Error!", MB_OK);


	lpZeissScope->flgDataIn = FALSE;
	UCHARtoFLOAT(w, h, lpZeissScope->img, fimg);

	return TRUE;
}

//Sets a flag and waits for the 'lpZeissScope->img' pointer to be filled
DWORD GetStreamImage(ZEISSMICROSCOPE *lpZeissScope, DWORD w, DWORD h, UCHAR *img)
{

	INT cnt = NULL;
	INT cntMax = 1000;
	lpZeissScope->flgGetFrame = TRUE;

	for(cnt = 0; cnt < cntMax; cnt++){
		//lpZeissScope->flgGetFrame = TRUE;
		
		if(lpZeissScope->flgDataIn)
			break;
		Sleep(1);
	}

	if(cnt == cntMax){
		ThrowMessage(lpZeissScope->hOwner, "GetStreamImage() Failed");
		//MessageBox(NULL, "GetFocalImage Failed", "Error!", MB_OK);
	}

	lpZeissScope->flgDataIn = FALSE;

	if(img){
		memcpy(img, lpZeissScope->img, w*h*sizeof(UCHAR));
	}

	return TRUE;
}

DWORD GetStreamImage(ZEISSMICROSCOPE *lpZeissScope, DWORD w, DWORD h, FLOAT *img)
{

	INT cnt = NULL;
	INT cntMax = 1000;
	lpZeissScope->flgGetFrame = TRUE;

	for(cnt = 0; cnt < cntMax; cnt++){
		//lpZeissScope->flgGetFrame = TRUE;
		
		if(lpZeissScope->flgDataIn)
			break;
		Sleep(1);
	}

	if(cnt == cntMax)
		MessageBox(NULL, "GetFocalImage Failed", "Error!", MB_OK);

	lpZeissScope->flgDataIn = FALSE;

	UCHARtoFLOAT(w, h, lpZeissScope->img, img);

	return TRUE;
}

//MAC5000 XYStage
INT GetXAxisPos(HANDLE hCom)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "WHERE X", strAnswer, 50);

	sscanf(strAnswer+2, "%d", &pos);

	return pos;
}

//MAC5000 XYStage
INT GetYAxisPos(HANDLE hCom)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "WHERE Y", strAnswer, 50);
	sscanf(strAnswer+2, "%d", &pos);

	return pos;
}

//MAC5000 XYStage
INT GetZAxisPos(HANDLE hCom)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "WHERE Z", strAnswer, 50);
	sscanf(strAnswer+2, "%d", &pos);

	return pos;
}

//MAC5000 XYStage
INT GetXAxisPos(HANDLE hCom, DWORD flag)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "WHERE X", strAnswer, 50);
	sscanf(strAnswer+2, "%d", &pos);

	if(flag == SP_MICRON){
		pos = (INT) ((FLOAT)pos / (FLOAT)STEPPERUM);
	}
	return pos;
}

//MAC5000 XYStage
INT GetYAxisPos(HANDLE hCom, DWORD flag)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "WHERE Y", strAnswer, 50);
	sscanf(strAnswer+2, "%d", &pos);

	if(flag == SP_MICRON){
		pos = (INT) ((FLOAT)pos / (FLOAT)STEPPERUM);
	}

	return pos;
}

//MAC5000 XYStage
INT GetZAxisPos(HANDLE hCom, DWORD flag)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "WHERE Z", strAnswer, 50);
	sscanf(strAnswer+2, "%d", &pos);

	if(flag == SP_MICRON){
		pos = (INT) ((FLOAT)pos / (FLOAT)STEPPERUM);
	}

	return pos;
}

INT SetXAxisPos(HANDLE hCom, INT pos)
{
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s%d", "MOVE x=", pos);
	SerialSend(hCom, str, strAnswer, 10);

	return pos;
}

INT SetYAxisPos(HANDLE hCom, INT pos)
{
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s%d", "MOVE y=", pos);
	SerialSend(hCom, str, strAnswer, 10);

	return pos;
}
INT SetZAxisPos(HANDLE hCom, INT pos)
{
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s%d", "MOVE z=", pos);
	SerialSend(hCom, str, strAnswer, 10);

	return pos;
}

INT SetXAxisPos(HANDLE hCom, INT pos, DWORD flag)
{
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(flag == SP_MICRON){
		pos = pos * STEPPERUM;
	}

	sprintf(str, "%s%d", "MOVE x=", pos);
	SerialSend(hCom, str, strAnswer, 10);

	return pos;
}

INT SetYAxisPos(HANDLE hCom, INT pos, DWORD flag)
{
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(flag == SP_MICRON){
		pos = pos * STEPPERUM;
	}

	sprintf(str, "%s%d", "MOVE y=", pos);
	SerialSend(hCom, str, strAnswer, 10);

	return pos;
}
INT SetZAxisPos(HANDLE hCom, INT pos, DWORD flag)
{
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(flag == SP_MICRON){
		pos = pos * STEPPERUM;
	}

	sprintf(str, "%s%d", "MOVE z=", pos);
	SerialSend(hCom, str, strAnswer, 10);

	return pos;
}

INT SetAxisSpeed(HANDLE hCom, INT speed, DWORD flag)
{
	CHAR str[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s", "SPEED ");

	if(flag&XAXIS == XAXIS){
		sprintf(str, "%s%s%d ", str, "x=", speed);
	}

	if(flag&YAXIS == YAXIS){
		sprintf(str, "%s%s%d ", str, "y=", speed);
	}

	SerialSend(hCom, str, NULL, 50);

	return speed;
}

DWORD WaitForPosition(HANDLE hCom, INT x, INT y)
{
	INT i = NULL;
	INT max = 50;

	INT xCurrent = NULL;
	INT yCurrent = NULL;


	for(i = 0; i < max; i++){
		
		xCurrent = GetXAxisPos(hCom);
		yCurrent = GetYAxisPos(hCom);

		if(x == xCurrent && y == yCurrent){
			return TRUE;
		}
		
		Sleep(10);
	}

	return FALSE;

}

DWORD WaitForPosition(HANDLE hCom, INT x, INT y, DWORD flag)
{
	INT i = NULL;
	INT max = 50;

	INT xCurrent = NULL;
	INT yCurrent = NULL;


	for(i = 0; i < max; i++){
		
		xCurrent = GetXAxisPos(hCom, flag);
		yCurrent = GetYAxisPos(hCom, flag);

		if(x == xCurrent && y == yCurrent){
			return TRUE;
		}
		
		Sleep(10);
	}

	return FALSE;

}

DWORD WaitForFocalPos(HANDLE hCom, INT focalPos)
{

	INT i = NULL;
	INT max = 10;
	
	INT zCurrent = NULL;

	for(i = 0; i < max; i++){

		zCurrent = GetFocalPos(hCom);

		if(focalPos == zCurrent){
			return TRUE;
		}

		Sleep(5);
	}

	return FALSE;

}


INT GetFocalPos(HANDLE hCom)
{
	INT pos = NULL;
	CHAR strAnswer[MAX_STRING];
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "FPZp", strAnswer);

	if(!strncmp(strAnswer, "PF", 2)){
		sscanf(strAnswer+2, "%X", &pos);
	}

	return pos;

}

INT SetFocalPos(HANDLE hCom, INT pos)
{
	CHAR str[MAX_STRING];
	CHAR strHex[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strHex, NULL, MAX_STRING*sizeof(CHAR));

	if(pos > 0){
		sprintf(str, "%s%6X", "FPZT", pos);
	}else{
		sprintf(strHex, "%6X", pos);
		sprintf(str, "%s%s", "FPZT", strHex+(strlen(strHex)-6));

	}
	CharReplace(str, 32, '0');
	SerialSend(hCom, "FPZV28", NULL);
	SerialSend(hCom, str, NULL);

	WaitForFocalPos(hCom, pos);

	return TRUE;
}

INT SetFocalPos(HANDLE hCom, INT pos, INT flag)
{
	CHAR str[MAX_STRING];
	CHAR strHex[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strHex, NULL, MAX_STRING*sizeof(CHAR));

	if(pos > 0){
		sprintf(str, "%s%6X", "FPZT", pos);
	}else{
		sprintf(strHex, "%6X", pos);
		sprintf(str, "%s%s", "FPZT", strHex+(strlen(strHex)-6));

	}
	CharReplace(str, 32, '0');
	SerialSend(hCom, "FPZV28", NULL);
	SerialSend(hCom, str, NULL);

	if( (flag&AF_WAIT) == AF_WAIT){
		WaitForFocalPos(hCom, pos);
	}

	return TRUE;
}

INT GetObjective(HANDLE hCom)
{

	INT objective = NULL;
	CHAR str[MAX_STRING];
	CHAR strRecieved[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strRecieved, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "HPCr2,1", strRecieved);

	if(!strncmp("PH", strRecieved, 2)){
		sscanf(strRecieved+2, "%d", &objective);
	}

	return objective;

}

INT GetReflector(HANDLE hCom)
{
	INT reflector = NULL;
	CHAR str[MAX_STRING];
	CHAR strRecieved[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strRecieved, NULL, MAX_STRING*sizeof(CHAR));

	SerialSend(hCom, "HPCr1,1", strRecieved);

	if(!strncmp("PH", strRecieved, 2)){
		sscanf(strRecieved+2, "%d", &reflector);
	}

	return reflector;
}

INT SetObjective(HANDLE hCom, INT objective)
{
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	INT lampVoltage = GetLampVoltage(hCom);

	sprintf(str, "%s%d", "HPCR2,", objective);
	SerialSend(hCom, str, NULL);

	SetLampVoltage(hCom, lampVoltage);

	return objective;

}

INT SetReflector(HANDLE hCom, INT reflector)
{
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s%d", "HPCR1,", reflector);
	SerialSend(hCom, str, NULL);

	return reflector;

}

INT SetLightPath(HANDLE hCom, INT lightPath)
{
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s%d", "HPCR39,", lightPath);
	SerialSend(hCom, str, NULL);

	return lightPath;
}

INT SetLampVoltage(HANDLE hCom, INT lampVoltage)
{
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s%d", "HPCV1,", lampVoltage);
	SerialSend(hCom, str, NULL);

	return lampVoltage;
}

INT GetLampVoltage(HANDLE hCom)
{
	INT lampVoltage = NULL;
	
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "%s", "HPCv1");
	SerialSend(hCom, str, strAnswer);

	sscanf(strAnswer+2, "%d", &lampVoltage);

	return lampVoltage;

}
