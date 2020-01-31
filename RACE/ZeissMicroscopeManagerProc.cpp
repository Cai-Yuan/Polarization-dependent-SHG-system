#include "stdafx.h"
#include "ZeissMicroscope.h"

//LRESULT CALLBACK ZeissMicroscopeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	ZEISSMICROSCOPE *lpZeissScope = (ZEISSMICROSCOPE *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
//
//	INT i;
//	INT pos;
//	CHAR str[MAX_STRING];
//	CHAR strAnswer[MAX_STRING];
//
//	switch(msg)
//	{
//	case WM_SETHWND:
//		if(wParam == HWND_OWNER){
//			lpZeissScope->hOwner = (HWND)lParam;
//
//			HWND *h;
//			h = (HWND *)malloc(sizeof(HWND));
//			if(h){
//				memcpy(h, &hwnd, sizeof(HWND));
//				CreateThread(NULL, NULL, InitZeissMicroscopeThread, (LPVOID) h, NULL, NULL);
//			}
//		}
//		return TRUE;
//	case WM_GETUSERDATA:
//		return (LRESULT) lpZeissScope;
//	case ZSM_BEGINBOOT:
//		return ZeissMicroscopeBoot(lpZeissScope->hOwner, lpZeissScope->hMicroscope);
//	case ZSM_SETAXIS:
//
//		switch(wParam){
//		case ZAXIS_VELOCITY:
//			lpZeissScope->zAxisVelocity = (INT) lParam;
//			break;
//		case ZAXIS_DELAY:
//			lpZeissScope->zAxisDelay = (INT) lParam;
//		default:
//			break;
//		}
//
//		return TRUE;
//	case ZSM_SETPOS:
//
//		memset(str, NULL, MAX_STRING*sizeof(CHAR));
//
//
//		if(wParam == XAXIS){
//			pos = (INT) lParam;
//			sprintf(str, "%s%d", "MOVE x=", pos);
//			SerialSend(lpZeissScope->hStage, str, strAnswer, 10);
//			return TRUE;
//		}
//
//		if(wParam == YAXIS){
//			pos = (INT) lParam;
//			sprintf(str, "%s%d", "MOVE y=", pos);
//			SerialSend(lpZeissScope->hStage, str, strAnswer, 10);
//			return TRUE;
//		}
//
//		if(wParam == ZAXIS){
//			pos = (INT) lParam;
//			CHAR strHex[MAX_STRING];
//			memset(str, NULL, MAX_STRING*sizeof(CHAR));
//			if(pos > 0){
//				sprintf(str, "%s%6X", "FPZT", pos);
//			}else{
//				sprintf(strHex, "%6X", pos);
//				sprintf(str, "%s%s", "FPZT", strHex+(strlen(strHex)-6));
//
//			}
//			CharReplace(str, 32, '0');
//			SerialSend(lpZeissScope->hMicroscope, "FPZV28", NULL);
//			SerialSend(lpZeissScope->hMicroscope, str, NULL);
//			return TRUE;
//		}
//
//		if(wParam == (XAXIS|YAXIS|ZAXIS)){
//			POINT3D p3D;
//			memcpy(&p3D, (VOID *)lParam, sizeof(POINT3D));
//
//			sprintf(str, "%s%d%s%d", "MOVE x=", (INT)p3D.x, " y=", (INT)p3D.y);
//			SerialSend(lpZeissScope->hStage, str, strAnswer, 10);
//
//			SendMessage(hwnd, ZSM_SETPOS, ZAXIS, p3D.z);
//			return TRUE;
//		}
//
//		return TRUE;
//	case ZSM_GETPOS:
//		pos = NULL;
//		memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));
//
//		if(wParam == (XAXIS|YAXIS|ZAXIS)){
//			INT x;
//			INT y;
//			POINT3D p3D;
//			x = GetXAxisPos(lpZeissScope->hStage);
//			y = GetYAxisPos(lpZeissScope->hStage);
//			//SerialSend(lpZeissScope->hStage, "WHERE XY", strAnswer, 20);
//
//			//sscanf(strAnswer+2, "%d %d", &x, &y);
//
//			memset(&p3D, NULL, sizeof(POINT3D));
//			p3D.x = x;
//			p3D.y = y;
//			p3D.z = GetFocalPos(lpZeissScope->hMicroscope);
//
//			memcpy((VOID *)lParam, &p3D, sizeof(POINT3D));
//			return TRUE;
//		}
//		if(wParam == XAXIS){
//			return GetXAxisPos(lpZeissScope->hStage);
//		}
//		if(wParam == YAXIS){
//			return GetYAxisPos(lpZeissScope->hStage);
//		}
//		if(wParam == ZAXIS){
//			return GetZAxisPos(lpZeissScope->hStage);
//		}
//		break;
//	case ZSM_SETSPEED:
//		{
//			INT speed = lParam;
//			SetAxisSpeed(lpZeissScope->hStage, lParam, XAXIS|YAXIS);
//
//		}
//		return TRUE;
//	case ZSM_AUTOFOCUS:
//
//		if(lpZeissScope->flgAutoFocus){
//			lpZeissScope->flgAutoFocus = FALSE;
//			return FALSE;
//		}
//
//		lpZeissScope->fStep = HIWORD(wParam);
//		lpZeissScope->fNumStep = LOWORD(wParam);
//			
//
//		//lpZeissScope->flgGetFrame = TRUE;
//		lpZeissScope->flgAutoFocus = TRUE;
//		lpZeissScope->flgFocusInit = TRUE;
//
//		{
//			HWND *hTemp;
//			hTemp = (HWND *)calloc(1, sizeof(HWND));
//			memcpy(hTemp, &hwnd, sizeof(HWND));
//			CreateThread(NULL, NULL, AutoFocusThread, hTemp, NULL, NULL);
//		}
//
//		return TRUE;
//	case ZSM_EXIT:
//		lpZeissScope->flgExit = TRUE;
//		return TRUE;
//	case ZSM_ACQUISITION:
//
//		MULTIFOV multiFOV;
//		memcpy(&lpZeissScope->mfov, (VOID *)wParam, sizeof(MULTIFOV));
//
//		{		
//			HWND *hTemp;
//			hTemp = (HWND *)calloc(1, sizeof(HWND));
//			memcpy(hTemp, &hwnd, sizeof(HWND));
//		
//
//		
//			CreateThread(NULL, NULL, MultiFOVCapture, hTemp, NULL, NULL);
//		}
//
//		return TRUE;
//	case WM_GETLINKFLAG:
//		return (LRESULT) &lpZeissScope->flgGetFrame;
//	case WM_DATADROP:		
//		INT w;
//		INT h;
//		
//		PDMFRAME pdmFrame;
//
//		w = lpZeissScope->w;
//		h = lpZeissScope->h;
//
//		lpZeissScope->flgDataIn = TRUE;
//		lpZeissScope->flgGetFrame = FALSE;
//		memcpy(&pdmFrame, (PDMFRAME *)wParam, sizeof(PDMFRAME));
//		memcpy(lpZeissScope->img, pdmFrame.lpvoid, w*h);
//		
//		if(pdmFrame.lpvoid) free(pdmFrame.lpvoid);
//		if((VOID *)wParam) free((VOID *)wParam);
//
//		return TRUE;
//	case WM_SETFRAMESIZE:
//		h = (INT) lParam;
//		w = (INT) wParam;
//		lpZeissScope->w = w;
//		lpZeissScope->h = h;
//
//		lpZeissScope->img = (UCHAR *)calloc(w*h, sizeof(UCHAR));
//		lpZeissScope->img1 = (UCHAR *)calloc(w*h, sizeof(UCHAR));
//		lpZeissScope->img2 = (UCHAR *)calloc(w*h, sizeof(UCHAR));
//		lpZeissScope->img3 = (UCHAR *)calloc(w*h, sizeof(UCHAR));
//		lpZeissScope->fImg = (FLOAT *)calloc(w*h, sizeof(FLOAT));
//
//		return TRUE;
//	case WM_CREATE:		
//		//SetTimer(hwnd, NULL, 500, NULL);
//		return TRUE;
//	case WM_TIMER:
//		INT xPos;
//		INT yPos;
//
//		lpZeissScope->zAxisPos = GetFocalPos(lpZeissScope->hMicroscope);
//
//		SendMessage(lpZeissScope->hStageControl, WM_SETPOS, ZAXIS, lpZeissScope->zAxisPos);
//		return TRUE;
//	case WM_NCCREATE:
//		{
//			//Allocate Window Pointer Memory
//			lpZeissScope = (ZEISSMICROSCOPE *)malloc(sizeof(ZEISSMICROSCOPE));
//			memset(lpZeissScope, NULL, sizeof(ZEISSMICROSCOPE));
//			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpZeissScope);
//			lpZeissScope = (ZEISSMICROSCOPE *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
//			if(!lpZeissScope){
//				MessageBox(hwnd, (LPCSTR)"ZiessMicroscopeWnd: GetWindowLongPtr Failed", (LPCSTR)"Error!", MB_OK);
//			}
//
//			break;
//		}
//	default:
//		break;
//	}
//
//	return TRUE;
