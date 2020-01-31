#include "stdafx.h"

#include "XBox.h"
#include "DeviceProc.h"
#include "LaserMicroscope.h"
#include "SC10Shutter.h"

INT_PTR CALLBACK DeviceProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int flgInit = FALSE;
	LM_DEVICE *lpWnd = (LM_DEVICE *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		if(lpWnd){
			return FALSE;
		}

		//Multiple 'WM_INITDIALOG' messages were being recieved
		//To stop multple copies of 'DeviceProc' from running, a static 'flgInit' flag
		//is used to force only a single instance is generated
		if(flgInit){
			return TRUE;
		}
		flgInit = TRUE;

		lpWnd = (LM_DEVICE *) InitWindowLongRACE(hwnd, sizeof(LM_DEVICE));

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpWnd->hOwner = (HWND) psp.lParam;

		lpWnd->lpSC10 = (LM_SC10 *)malloc(sizeof(LM_SC10));
		lpWnd->lpXBox = (LM_XBOX *)malloc(sizeof(LM_XBOX));
		memset(lpWnd->lpSC10, NULL, sizeof(LM_SC10));
		memset(lpWnd->lpXBox, NULL, sizeof(LM_XBOX));

		lpWnd->lpXBox->xPolStick = 1;
		lpWnd->lpXBox->yPolStick = 1;
		lpWnd->lpXBox->zPolStick = 1;

		SendMessage(lpWnd->hOwner, WM_SETHWND, (WPARAM) HWND_DEVICE, (LPARAM)hwnd);
		lpWnd->hXBoxLinker = (HWND) SendMessage(lpWnd->hOwner, WM_GETHWND, (WPARAM) HWND_XBOXLINKER, NULL);
		lpWnd->hSC10 = (HWND) SendMessage(lpWnd->hOwner, WM_GETHWND, (WPARAM) HWND_SC10, NULL);
		
		if(lpWnd->hXBoxLinker){
			SendMessage(lpWnd->hXBoxLinker, WM_SETHWND, HWND_LINK, (LPARAM) hwnd);
		}
		if(lpWnd->hSC10){
			SendMessage(lpWnd->hSC10, WM_SETHWND, HWND_LINK, (LPARAM) hwnd);
		}
		if(!lpWnd->hSC10 || !lpWnd->hXBoxLinker){
			SetTimer(hwnd, NULL, 1000, NULL);
		}
		return FALSE;
	case WM_COMMAND:
		//switch(HIWORD(wParam))
		//{
		//case BN_CLICKED:
		//	break;
		//case EN_CHANGE:
		//  break;
		//	default:
		//		break;
		//	}
		//}
		return TRUE;
	case WM_INPUT:
		if((HWND)wParam == lpWnd->hSC10){		//lParam-	OPEN					CLOSED
			lpWnd->flgShutter = lParam;			//			TRUE					FALSE
		}
		return TRUE;
	case WM_SETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			lpWnd->hOwner = (HWND) lParam;
			return TRUE;
		default:
			return FALSE;
		}
	case WM_XBINPUT:
		LASERMICROSCOPE *lpMicroscope;
		lpMicroscope = (LASERMICROSCOPE *)GetWindowUserData(lpWnd->hOwner);
		//lpMicroscope = (LASERMICROSCOPE *) GetWindowLongPtr(lpWnd->hOwner, GWLP_USERDATA);
		return XBoxInput(lpMicroscope, wParam, lParam);
	case WM_TIMER:
		if(!lpWnd->hXBoxLinker){
			lpWnd->hXBoxLinker = (HWND) SendMessage(lpWnd->hOwner, WM_GETHWND, (WPARAM) HWND_XBOXLINKER, NULL);
		}
		if(!lpWnd->hSC10){
			lpWnd->hSC10 = (HWND) SendMessage(lpWnd->hOwner, WM_GETHWND, (WPARAM) HWND_SC10, NULL);
		}

		if(lpWnd->hSC10 && lpWnd->hXBoxLinker){
			KillTimer(hwnd, NULL);
		}
		return FALSE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD XBoxInput(LASERMICROSCOPE *lpMicroscope, WPARAM wParam, LPARAM lParam)
{
	LM_DEVICE *lpDevice = lpMicroscope->lpDevice;
	LM_XBOX *lpXBox = lpDevice->lpXBox;
	LM_SC10 *lpSC10	= lpDevice->lpSC10;

	CHAR str[MAX_STRING];

	FLOAT average = NULL;
	FLOAT coef = NULL;
	
	FLOAT xJog = NULL;
	FLOAT yJog = NULL;
	FLOAT zJog = NULL;
	SHORT *x;
	SHORT *y;
	SHORT *z;
	switch(wParam){
		case XB_LEFTSTICK_X:
			x = (SHORT *)lParam;

			xJog = CalculateStickPositionXBOX(*x, OFFSET, 1);

			lpXBox->xStick = xJog;

			lpXBox->flgXInput = TRUE;
			return TRUE;
		case XB_LEFTSTICK_Y:
			y = (SHORT *)lParam;

			yJog = CalculateStickPositionXBOX(*y, OFFSET, 1);

			lpXBox->yStick = yJog;

			lpXBox->flgYInput = TRUE;
			return TRUE;
		case XB_RIGHTSTICK_Y:
			z = (SHORT *)lParam;

			zJog = CalculateStickPositionXBOX(*z, OFFSET, 1);

			lpXBox->zStick = zJog;

			lpXBox->flgZInput = TRUE;
			return TRUE;
		case XB_LEFTTRIGGER_DOWN:
			lpSC10->flgShutter = !lpSC10->flgShutter;
			lpXBox->flgLeftTrigger = TRUE;

			if(!lpSC10->flgShutter){
				CloseShutterSC10(lpDevice->hSC10);
				UpdateVelocity(lpMicroscope);
			}else{
				OpenShutterSC10(lpDevice->hSC10);
				UpdateVelocity(lpMicroscope);
			}
			return TRUE;
		case XB_RIGHTTRIGGER_DOWN:
			lpXBox->flgRightTrigger = lParam;
			//SendMessage(lpMicroscope->hAutoFocus, WM_AUTOFOCUS, NULL, NULL);
			//lpMicroscope->flgAutoFocus = TRUE;
			return TRUE;
		case XB_RIGHTTRIGGER_UP:
			lpXBox->flgRightTrigger = lParam;
			return TRUE;
		case XB_BUTTON_START:
			lpXBox->flgStartButton = lParam;
			return TRUE;
		case XB_BUTTON_A:
			lpXBox->flgAButton = lParam;
			return TRUE;
		case XB_BUTTON_B:
			lpXBox->flgBButton = lParam;
			return TRUE;
		case XB_BUTTON_X:
			lpXBox->flgXButton = lParam;
			return TRUE;
		case XB_BUTTON_Y:
			lpXBox->flgYButton = lParam;
			return TRUE;
		case XB_LEFTTRIGGER_UP:
			return TRUE;
		case XB_LEFTSHOULDER:
			lpXBox->flgLeftShoulder = lParam;
			return TRUE;
		case XB_RIGHTSHOULDER:
			lpXBox->flgRightShoulder = lParam;
			return TRUE;
		case XB_BUTTON_BACK:
			lpXBox->flgBackButton = lParam;
			return TRUE;
		case XB_DPAD_DOWN:
			Edit_SetFloat(lpMicroscope->lpStage->hEditZVelFine, lpMicroscope->lpStage->zVelFine-1);
			lpXBox->flgDPadDown = TRUE;
			return TRUE;
		case XB_DPAD_UP:
			Edit_SetFloat(lpMicroscope->lpStage->hEditZVelFine, lpMicroscope->lpStage->zVelFine+1);
			lpXBox->flgDPadUp = TRUE;
			return TRUE;
		case XB_DPAD_LEFT:
			Edit_SetFloat(lpMicroscope->lpStage->hEditXVelFine, lpMicroscope->lpStage->xVelFine-1);
			Edit_SetFloat(lpMicroscope->lpStage->hEditYVelFine, lpMicroscope->lpStage->yVelFine-1);
			lpXBox->flgDPadLeft = TRUE;
			return TRUE;
		case XB_DPAD_RIGHT:
			Edit_SetFloat(lpMicroscope->lpStage->hEditXVelFine, lpMicroscope->lpStage->xVelFine+1);
			Edit_SetFloat(lpMicroscope->lpStage->hEditYVelFine, lpMicroscope->lpStage->yVelFine+1);
			lpXBox->flgDPadRight = TRUE;
			return TRUE;
		default:
			return TRUE;
	}

	return TRUE;
}