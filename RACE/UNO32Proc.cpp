#include "stdafx.h"
#include "UNO32.h"
#include "XBox.h"
#include "EnumerateDevice.h"
//#include "CircularBuffer.h"
#include "Sum.h"
#include "SC10Shutter.h"
#include "ToolsWin32.h"
#include "Point.h"
#include "Object.h"
#include "Text.h"
#include "Magnitude.h"
#include "Norm.h"
#include "FilePath.h"
#include "GetFolderPath.h"
#include "Microscope.h"

#include "A3200.h"
#include "A3200Stage.h"

#define MAXSTICK 32767
//#define OFFSET 7000


INT_PTR CALLBACK UNO32WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WNDUNO32 *lpWndUNO32 = (WNDUNO32 *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:
		
		lpWndUNO32 = (WNDUNO32 *)InitWindowLong(hwnd, sizeof(WNDUNO32));

		if(lpWndUNO32 == NULL){
			ThrowMessage(hwnd, "WndUNO32: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndUNO33: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		Alloc(&lpWndUNO32->os, 100);

		lpWndUNO32->hwnd = hwnd;
		lpWndUNO32->driveA.dir = 1;
		lpWndUNO32->driveB.dir = 1;
		lpWndUNO32->driveC.dir = 1;
		lpWndUNO32->driveA.cDrive = 'A';
		lpWndUNO32->driveB.cDrive = 'B';
		lpWndUNO32->driveC.cDrive = 'C';

		lpWndUNO32->sampleRate = 15.0;
		lpWndUNO32->xScale = 0.001;
		lpWndUNO32->yScale = 0.001;
		lpWndUNO32->zScale = 0.001;
		
		//lpWndUNO32->pixPerUm = 6.56; //Pixels per um
		lpWndUNO32->pixPerUm = 4.8; //Pixels per um


		lpWndUNO32->hEditXPos = GetDlgItem(hwnd, IDC_EDIT_XPOS);
		lpWndUNO32->hEditYPos = GetDlgItem(hwnd, IDC_EDIT_YPOS);
		lpWndUNO32->hEditZPos = GetDlgItem(hwnd, IDC_EDIT_ZPOS);
		lpWndUNO32->hEditXVel = GetDlgItem(hwnd, IDC_EDIT_XVEL);
		lpWndUNO32->hEditYVel = GetDlgItem(hwnd, IDC_EDIT_YVEL);
		lpWndUNO32->hEditZVel = GetDlgItem(hwnd, IDC_EDIT_ZVEL);

		lpWndUNO32->hEditXVelFine = GetDlgItem(hwnd, IDC_EDIT_XVEL_FINE);
		lpWndUNO32->hEditXVelCoarse = GetDlgItem(hwnd, IDC_EDIT_XVEL_COARSE);
		lpWndUNO32->hEditYVelFine = GetDlgItem(hwnd, IDC_EDIT_YVEL_FINE);
		lpWndUNO32->hEditYVelCoarse = GetDlgItem(hwnd, IDC_EDIT_YVEL_COARSE);
		lpWndUNO32->hEditZVelFine = GetDlgItem(hwnd, IDC_EDIT_ZVEL_FINE);
		lpWndUNO32->hEditZVelCoarse = GetDlgItem(hwnd, IDC_EDIT_ZVEL_COARSE);

		lpWndUNO32->hEditPixPerUm = GetDlgItem(hwnd, IDC_EDIT_PIXPERUM);

		//lpWndUNO32->hCheckXAxisStage = GetDlgItem(hwnd, IDC_CHECK_XAXIS_STAGE);
		//lpWndUNO32->hCheckYAxis = GetDlgItem(hwnd, IDC_CHECK_YAXIS_STAGE);
		//lpWndUNO32->hCheckZAxis = GetDlgItem(hwnd, IDC_CHECK_ZAXIS_STAGE);
		//lpWndUNO32->hCheckXAxis = GetDlgItem(hwnd, IDC_CHECK_XAXIS_STAGE);
		//lpWndUNO32->hCheckYAxis = GetDlgItem(hwnd, IDC_CHECK_YAXIS_STAGE);
		//lpWndUNO32->hCheckZAxis = GetDlgItem(hwnd, IDC_CHECK_ZAXIS_STAGE);

		lpWndUNO32->hEditRoiDiameter = GetDlgItem(hwnd, IDC_EDIT_ROIDIAMETER);

		lpWndUNO32->hEditSampleRate = GetDlgItem(hwnd, IDC_EDIT_SAMPLERATE);

		lpWndUNO32->hBtnZero = GetDlgItem(hwnd, IDC_BTN_ZERO);

		lpWndUNO32->hBtnCapture = GetDlgItem(hwnd, IDC_BTN_CAPTURE);
		lpWndUNO32->hEditNSlice = GetDlgItem(hwnd, IDC_EDIT_NSLICE);
		lpWndUNO32->hEditStepSize = GetDlgItem(hwnd, IDC_EDIT_STEPSIZE);

		lpWndUNO32->hCheckZDelay = GetDlgItem(hwnd, IDC_CHECK_ZDELAY);

		lpWndUNO32->flgZDelay = TRUE;

		Edit_SetFloat(lpWndUNO32->hEditXPos, 0.0);
		Edit_SetFloat(lpWndUNO32->hEditYPos, 0.0);
		Edit_SetFloat(lpWndUNO32->hEditZPos, 0.0);

		Edit_SetInt(lpWndUNO32->hEditNSlice, 100);
		Edit_SetFloat(lpWndUNO32->hEditStepSize, 2.0);

		Edit_SetFloat(lpWndUNO32->hEditRoiDiameter, 7.0);

		Edit_SetFloat(lpWndUNO32->hEditPixPerUm, lpWndUNO32->pixPerUm);

		lpWndUNO32->xVelCoarse = 500;
		lpWndUNO32->yVelCoarse = 500;
		lpWndUNO32->zVelCoarse = 100;
		lpWndUNO32->xVelFine = 150;
		lpWndUNO32->yVelFine = 150;
		lpWndUNO32->zVelFine = 30;

		lpWndUNO32->xPolStage = 1.0;
		lpWndUNO32->yPolStage = 1.0;
		lpWndUNO32->zPolStage = 1.0;
		lpWndUNO32->xPolStick = 1.0;
		lpWndUNO32->yPolStick = 1.0;
		lpWndUNO32->zPolStick = 1.0;

		Edit_SetFloat(lpWndUNO32->hEditXVelFine, lpWndUNO32->xVelFine);
		Edit_SetFloat(lpWndUNO32->hEditYVelFine, lpWndUNO32->yVelFine);
		Edit_SetFloat(lpWndUNO32->hEditZVelFine, lpWndUNO32->zVelFine);
		Edit_SetFloat(lpWndUNO32->hEditXVelCoarse, lpWndUNO32->xVelCoarse);
		Edit_SetFloat(lpWndUNO32->hEditYVelCoarse, lpWndUNO32->yVelCoarse);
		Edit_SetFloat(lpWndUNO32->hEditZVelCoarse, lpWndUNO32->zVelCoarse);

		Edit_SetFloat(lpWndUNO32->hEditSampleRate, lpWndUNO32->sampleRate);

		Button_SetCheck(lpWndUNO32->hCheckZDelay, BST_CHECKED);

		lpWndUNO32->flgBPosition = TRUE;
		lpWndUNO32->flgThread = TRUE;

		CreateThread(NULL, NULL, A3200Thread, (LPVOID) lpWndUNO32, NULL, NULL);


		break;
	case WM_NOTIFY:
		{
			//NMHDR *nmhdr = (NMHDR *) lParam;
			//if(nmhdr->hwndFrom == lpWndUNO32->hSpinRoiDiameter){
			//	NMUPDOWN *nmupdown = (NMUPDOWN *) lParam;
			//	//if(nmupdown->
			//}
		}
		break;
	case WM_COMMAND:


		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_BTN_ZERO){
				lpWndUNO32->flgZero = TRUE;
				return TRUE;
			}
			if(LOWORD(wParam) == IDC_CHECK_ZDELAY){
				LRESULT result = Button_GetCheck(lpWndUNO32->hCheckZDelay);
				if(BST_CHECKED == result){
					lpWndUNO32->flgZDelay = TRUE;
				}else if(BST_UNCHECKED == result){
					lpWndUNO32->flgZDelay = FALSE;
				}else{}
			}
			if(LOWORD(wParam) == IDC_BTN_ZSTACK){
				if(GetFolderPath(hwnd, lpWndUNO32->strFolder)){
					lpWndUNO32->flgZStack = TRUE;
					lpWndUNO32->slice = 0;
				}
			}
			if(LOWORD(wParam) == IDC_CHECK_XAXIS_STAGE){
				lpWndUNO32->xPolStage = -1.0*lpWndUNO32->xPolStage;
			}
			if(LOWORD(wParam) == IDC_CHECK_YAXIS_STAGE){
				lpWndUNO32->yPolStage = -1.0*lpWndUNO32->yPolStage;
			}
			if(LOWORD(wParam) == IDC_CHECK_ZAXIS_STAGE){
				lpWndUNO32->zPolStage = -1.0*lpWndUNO32->zPolStage;
			}
			if(LOWORD(wParam) == IDC_CHECK_XAXIS_STICK){
				//lpWndUNO32->xPolStick = -1.0*lpWndUNO32->xPolStick;
			}
			if(LOWORD(wParam) == IDC_CHECK_YAXIS_STICK){
				//lpWndUNO32->yPolStick = -1.0*lpWndUNO32->yPolStick;
			}
			if(LOWORD(wParam) == IDC_CHECK_ZAXIS_STICK){
				//lpWndUNO32->zPolStick = -1.0*lpWndUNO32->zPolStick;
			}

			return FALSE;
		case EN_CHANGE:
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_SAMPLERATE:
				{
					INT msTime;
					Edit_GetText(lpWndUNO32->hEditSampleRate, str, MAX_STRING);
					sscanf(str, "%f", &lpWndUNO32->sampleRate);
					lpWndUNO32->sampleRate = lpWndUNO32->sampleRate;
					msTime = ((FLOAT)1.0 / (FLOAT)lpWndUNO32->sampleRate);
				}
				break;
			case IDC_EDIT_XVEL_FINE:
				{
					lpWndUNO32->xVelFine = Edit_GetFloat(lpWndUNO32->hEditXVelFine);
					lpWndUNO32->xVelFine = (FLOAT)lpWndUNO32->xVelFine;
					UpdateUNO32Velocity(lpWndUNO32);
				}
				break;
			case IDC_EDIT_YVEL_FINE:
				{
					lpWndUNO32->yVelFine = Edit_GetFloat(lpWndUNO32->hEditYVelFine);
					lpWndUNO32->yVelFine = (FLOAT)lpWndUNO32->yVelFine;
					UpdateUNO32Velocity(lpWndUNO32);
				}
				break;
			case IDC_EDIT_ZVEL_FINE:
				{
					lpWndUNO32->zVelFine = Edit_GetFloat(lpWndUNO32->hEditZVelFine);
					lpWndUNO32->zVelFine = (FLOAT)lpWndUNO32->zVelFine;
					UpdateUNO32Velocity(lpWndUNO32);
				}
				break;
			case IDC_EDIT_XVEL_COARSE:
				{
					lpWndUNO32->xVelCoarse = Edit_GetFloat(lpWndUNO32->hEditXVelCoarse);
					lpWndUNO32->xVelCoarse = (FLOAT)lpWndUNO32->xVelCoarse;
					UpdateUNO32Velocity(lpWndUNO32);
				}
				break;
			case IDC_EDIT_YVEL_COARSE:
				{
					lpWndUNO32->yVelCoarse = Edit_GetFloat(lpWndUNO32->hEditYVelCoarse);
					lpWndUNO32->yVelCoarse = (FLOAT)lpWndUNO32->yVelCoarse;
					UpdateUNO32Velocity(lpWndUNO32);
				}
				break;
			case IDC_EDIT_ZVEL_COARSE:
				{
					lpWndUNO32->zVelCoarse = Edit_GetFloat(lpWndUNO32->hEditZVelCoarse);
					lpWndUNO32->zVelCoarse = (FLOAT)lpWndUNO32->zVelCoarse;
					UpdateUNO32Velocity(lpWndUNO32);
				}
				break;
			case IDC_EDIT_ROIDIAMETER:
				{
					lpWndUNO32->roiDiameter = Edit_GetFloat(lpWndUNO32->hEditRoiDiameter);
				}
				break;
			case IDC_EDIT_NSLICE:
				{
					lpWndUNO32->nSlice = Edit_GetInt(lpWndUNO32->hEditNSlice);
				}
				break;
			case IDC_EDIT_STEPSIZE:
				{
					lpWndUNO32->stepSize = Edit_GetFloat(lpWndUNO32->hEditStepSize);
				}
				break;
			case IDC_EDIT_PIXPERUM:
				{
					lpWndUNO32->pixPerUm = Edit_GetFloat(lpWndUNO32->hEditPixPerUm);
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		//Control Identifier
		switch(LOWORD(wParam))
		{
		//case IDSET:
		//	return TRUE;
		default:
			break;
		}

		break;
	case WM_GETLONGPTR:
		return (INT_PTR) DialogReturn(hwnd, (LRESULT)lpWndUNO32);
	case WM_SETHWND:

		switch(wParam)
		{
		case HWND_OWNER:
			lpWndUNO32->hOwner = (HWND) lParam;
			lpWndUNO32->hCamProp = (HWND) SendMessage(lpWndUNO32->hOwner, WM_GETHWND, HWND_CAMERAPROPERTIES, NULL);
			SendMessage(lpWndUNO32->hCamProp, WM_CREATELINK, (WPARAM) hwnd, NULL);
			return TRUE;
		case HWND_SHUTTER_SC10:
			lpWndUNO32->hShutter = (HWND) lParam;
			SendMessage(lpWndUNO32->hShutter, WM_SETHWND, HWND_LINK, (LPARAM)hwnd);
			return TRUE;
		case HWND_DATAVIEW:
			lpWndUNO32->hDataView = (HWND) lParam;
			SendMessage(lpWndUNO32->hDataView, WM_GETFRAMESIZE, (WPARAM) &lpWndUNO32->wFrame, (LPARAM) &lpWndUNO32->hFrame);
			lpWndUNO32->fov.wFOV = (1.0/lpWndUNO32->pixPerUm*lpWndUNO32->wFrame);
			lpWndUNO32->fov.hFOV = (1.0/lpWndUNO32->pixPerUm*lpWndUNO32->hFrame);
			lpWndUNO32->xFOV = (1.0/lpWndUNO32->pixPerUm)*lpWndUNO32->wFrame;
			lpWndUNO32->yFOV = (1.0/lpWndUNO32->pixPerUm)*lpWndUNO32->hFrame;
			return TRUE;
		default:
			return TRUE;
		}
	
		return TRUE;
	case WM_GETHWND:
		switch(wParam){
		case HWND_FRAME:
			return (LRESULT) lpWndUNO32->hOwner;
		default:
			return FALSE;
		}

		return FALSE;
	case WM_SETHANDLE:

		if(wParam == HANDLE_DEVICE){
			lpWndUNO32->hDevice = (HANDLE) lParam;
			InitUNO32Device(lpWndUNO32);
		}
		return TRUE;
	case WM_XBINPUT:
		{
			return XBoxInputUNO32(lpWndUNO32, wParam, lParam);
		}
	case WM_CLOSE:

		//SendMessage(lpPropertyManager->hOwner, WM_COMMAND, LOWORD(ID_VIEW_PROPERTYMANAGER), NULL);
		//lpWndUNO32->flgThread = FALSE;
		//Sleep(100);
		//if(lpWndUNO32->hAero){
		//	if(!A3200MotionDisable(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00 | AXISMASK_01 | AXISMASK_02)){
		//		MessageBox(NULL, "Drivers Not Enabled!", "Drivers Not Enabled!", MB_OK);
		//	}
		//}
		return TRUE;
	case WM_TIMER:

		return TRUE;
	case WM_DATAIN:
		if(!lpWndUNO32->flgZStack)
			return FALSE;

		PDMFRAME *pdmf;
		pdmf = (PDMFRAME *) wParam;

		if(!lpWndUNO32->pdmIn.lpvoid){
			AllocPDMFrame(&lpWndUNO32->pdmIn, pdmf->pdmfh.width, pdmf->pdmfh.height, pdmf->pdmfh.typeData);
		}

		CopyPDMFrame(&lpWndUNO32->pdmIn, (PDMFRAME *)wParam);

		lpWndUNO32->flgFrameIn = TRUE;

		return TRUE;
	case WM_INPUT:
		if((HWND)wParam == lpWndUNO32->hShutter){
			//lParam = TRUE if shutter open
			//lParam = FALSE if shutter closed
			lpWndUNO32->flgShutter = lParam;
		}
		return TRUE;
	case WM_DESTROY:
		SendMessage(lpWndUNO32->hDataView, WM_DESTROY, NULL, NULL);
		ShowWindow(hwnd, SW_HIDE);
		lpWndUNO32->flgThread = FALSE;
		if(lpWndUNO32->hAero){
			if(!A3200MotionDisable(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00 | AXISMASK_01 | AXISMASK_02)){
				MessageBox(NULL, "Drivers Not Enabled!", "Drivers Not Enabled!", MB_OK);
			}
		}
		Sleep(300);
		PostQuitMessage(0);
		return FALSE;
	default:
		return FALSE;
	}


	return FALSE;

}


DWORD UpdateDriveDirection(HANDLE hDevice, FLOAT velocity, UDRIVE *uDrive)
{

	INT dir = 0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));

	if(velocity < 0){
		dir = -1;
	}else{
		dir = 1;
	}

	if(dir != uDrive->dir){
		uDrive->dir = dir;

		
		sprintf(str, "!SETRATE%c%f", uDrive->cDrive, (FLOAT)0.0);
		SerialSend(hDevice, str, NULL); 

		if(dir < 0){
			sprintf(str, "!SETDIR%cCW", uDrive->cDrive);
		}else{
			sprintf(str, "!SETDIR%cCCW", uDrive->cDrive);
		}
		SerialSend(hDevice, str, NULL);

		return TRUE;
	}

	return FALSE;
}



DWORD XBoxInputUNO32(WNDUNO32 *lpWndUNO32, WPARAM wParam, LPARAM lParam)
{

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

			//xJog = CalculateStickPositionXBOX(*x, OFFSET, 1);
			xJog = CalculateStickPositionXBOX(*x, OFFSET, lpWndUNO32->xPolStick);

			lpWndUNO32->xStick = xJog;

			lpWndUNO32->flgXInput = TRUE;
			return TRUE;
		case XB_LEFTSTICK_Y:
			y = (SHORT *)lParam;

			//yJog = CalculateStickPositionXBOX(*y, OFFSET, -1);
			yJog = CalculateStickPositionXBOX(*y, OFFSET, lpWndUNO32->yPolStick);

			lpWndUNO32->yStick = yJog;

			lpWndUNO32->flgYInput = TRUE;
			return TRUE;
		case XB_RIGHTSTICK_Y:
			z = (SHORT *)lParam;

			//zJog = CalculateStickPositionXBOX(*z, OFFSET, 1);
			zJog = CalculateStickPositionXBOX(*z, OFFSET, lpWndUNO32->zPolStick);

			lpWndUNO32->zStick = zJog;

			lpWndUNO32->flgZInput = TRUE;
			return TRUE;
		case XB_LEFTTRIGGER_DOWN:
			lpWndUNO32->flgShutter = !lpWndUNO32->flgShutter;
			lpWndUNO32->flgLeftTrigger = TRUE;

			if(!lpWndUNO32->flgShutter){
				PostMessage(lpWndUNO32->hShutter, WM_TODO, (WPARAM) SHUTTER_CLOSE, NULL);
				UpdateUNO32Velocity(lpWndUNO32);
			}else{
				PostMessage(lpWndUNO32->hShutter, WM_TODO, (WPARAM) SHUTTER_OPEN, NULL);
				UpdateUNO32Velocity(lpWndUNO32);
			}
			return TRUE;
		case XB_BUTTON_START:
			lpWndUNO32->flgStartButton = lParam;
			return TRUE;
		case XB_BUTTON_A:
			lpWndUNO32->flgAButton = lParam;
			return TRUE;
		case XB_BUTTON_B:
			lpWndUNO32->flgBButton = lParam;
			return TRUE;
		case XB_BUTTON_X:
			lpWndUNO32->flgXButton = lParam;
			return TRUE;
		case XB_BUTTON_Y:
			lpWndUNO32->flgYButton = lParam;
			return TRUE;
		case XB_LEFTTRIGGER_UP:
			return TRUE;
		case XB_LEFTSHOULDER:
			lpWndUNO32->flgLeftShoulder = lParam;
			return TRUE;
		case XB_RIGHTSHOULDER:
			lpWndUNO32->flgRightShoulder = lParam;
			return TRUE;
		case XB_RIGHTTRIGGER:
			lpWndUNO32->flgRightTrigger = lParam;
		case XB_BUTTON_BACK:
			lpWndUNO32->flgBackButton = lParam;
			return TRUE;
		case XB_DPAD_DOWN:
			Edit_SetFloat(lpWndUNO32->hEditZVelFine, lpWndUNO32->zVelFine-5);
			lpWndUNO32->flgDPadDown = TRUE;
			//Edit_SetFloat(lpWndUNO32->hEditYVelFine, lpWndUNO32->yVelFine-5);
			return TRUE;
		case XB_DPAD_UP:
			Edit_SetFloat(lpWndUNO32->hEditZVelFine, lpWndUNO32->zVelFine+5);
			lpWndUNO32->flgDPadUp = TRUE;
			//Edit_SetFloat(lpWndUNO32->hEditYVelFine, lpWndUNO32->yVelFine+5);
			return TRUE;
		case XB_DPAD_LEFT:
			Edit_SetFloat(lpWndUNO32->hEditXVelFine, lpWndUNO32->xVelFine-5);
			Edit_SetFloat(lpWndUNO32->hEditYVelFine, lpWndUNO32->yVelFine-5);
			lpWndUNO32->flgDPadLeft = TRUE;
			return TRUE;
		case XB_DPAD_RIGHT:
			Edit_SetFloat(lpWndUNO32->hEditXVelFine, lpWndUNO32->xVelFine+5);
			Edit_SetFloat(lpWndUNO32->hEditYVelFine, lpWndUNO32->yVelFine+5);
			lpWndUNO32->flgDPadRight = TRUE;
			return TRUE;
		default:
			return TRUE;
	}

	return TRUE;
}


DWORD InitUNO32Device(WNDUNO32 *lpWndUNO32)
{
	if(!lpWndUNO32->hDevice)
		return FALSE;

	HANDLE hDevice = lpWndUNO32->hDevice;
	CHAR str[MAX_STRING];
	//sprintf(str, "!SETHOMEA");
	//SerialSend(hDevice, str, NULL);

	//sprintf(str, "!SETHOMEB");
	//SerialSend(hDevice, str, NULL);

	//sprintf(str, "!SETHOMEC");
	//SerialSend(hDevice, str, NULL);

	return TRUE;
}

DWORD GetUNO32DevicePosition(WNDUNO32 *lpWndUNO32)
{
	if(!lpWndUNO32->hDevice){
		return FALSE;
	}

	CHAR str[MAX_STRING];
	HANDLE  hDevice = lpWndUNO32->hDevice;

	long long int pos;
	float p = 0;

	pos = GetUNO32AxisPosition(hDevice, 'A');
	p = pos *lpWndUNO32->xScale;
	Edit_SetFloat(lpWndUNO32->hEditXPos, p);

	pos = GetUNO32AxisPosition(hDevice, 'B');
	p = pos * lpWndUNO32->yScale;
	Edit_SetFloat(lpWndUNO32->hEditYPos, p);

	pos = GetUNO32AxisPosition(hDevice, 'C');
	p = pos * lpWndUNO32->zScale;
	Edit_SetFloat(lpWndUNO32->hEditZPos, p);

	return TRUE;
}

DWORD UpdateUNO32PositionDisplay(UNO32_3DPOINT pt, WNDUNO32 *lpWndUNO32)
{
	float p = 0;

	p = pt.x*lpWndUNO32->xScale;
	Edit_SetFloat(lpWndUNO32->hEditXPos, p);
	p = pt.y*lpWndUNO32->yScale;
	Edit_SetFloat(lpWndUNO32->hEditYPos, p);
	p = pt.z*lpWndUNO32->zScale;
	Edit_SetFloat(lpWndUNO32->hEditZPos, p);

	return TRUE;
}

DWORD UpdateUNO32PositionDisplay(DOUBLE_3DPOINT pt, WNDUNO32 *lpWndUNO32)
{
	float p = 0;

	p = pt.x*1000;
	//p = pt.x*lpWndUNO32->xScale;
	Edit_SetFloat(lpWndUNO32->hEditXPos, p);
	p = pt.y*1000;
	//p = pt.y*lpWndUNO32->yScale;
	Edit_SetFloat(lpWndUNO32->hEditYPos, p);
	p = pt.z*1000;
	//p = pt.z*lpWndUNO32->zScale;
	Edit_SetFloat(lpWndUNO32->hEditZPos, p);

	return TRUE;
}


DWORD GetUNO32DevicePosition(HANDLE hDevice, long long int *x, long long int *y, long long int *z)
{

	CHAR str[MAX_STRING];

	long long int pos;
	float p = 0;

	*x = GetUNO32AxisPosition(hDevice, 'A');
	*y = GetUNO32AxisPosition(hDevice, 'B');
	*z= GetUNO32AxisPosition(hDevice, 'C');

	return TRUE;
}

DWORD GetUNO32DevicePosition(HANDLE hDevice, UNO32_3DPOINT *pt)
{
	return GetUNO32DevicePosition(hDevice, &pt->x, &pt->y, &pt->z);
}

DWORD GetUNO32DevicePositionX(HANDLE hDevice, UNO32_3DPOINT *pt)
{
	pt->x = GetUNO32AxisPosition(hDevice, 'A');

	return TRUE;
}

DWORD GetUNO32DevicePositionY(HANDLE hDevice, UNO32_3DPOINT *pt)
{
	pt->y = GetUNO32AxisPosition(hDevice, 'B');

	return TRUE;
}

DWORD GetUNO32DevicePositionZ(HANDLE hDevice, UNO32_3DPOINT *pt)
{
	pt->z = GetUNO32AxisPosition(hDevice, 'C');

	return TRUE;
}

long long int GetUNO32AxisPosition(HANDLE hDevice, CHAR axis)
{
	CHAR str[MAX_STRING];
	CHAR strPos[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strPos, NULL, MAX_STRING*sizeof(CHAR));

	long long int pos = 0;

	sprintf(str, "?GETPOS%c", axis);
	SerialSendEx(hDevice, sizeof(long long int), str, strPos);
	memcpy(&pos, strPos, sizeof(long long int));

	return pos;

}

DWORD SetUNO32AxisPosition(HANDLE hDevice, CHAR axis, long long int pos)
{
	CHAR str[MAX_STRING];
	CHAR strPos[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(str, "!MOVE%c%d", axis, pos);
	SerialSend(hDevice, str, NULL);

	return TRUE;
}

DWORD SetUNO32DevicePosition(HANDLE hDevice, UNO32_3DPOINT pt)
{
	SetUNO32AxisPosition(hDevice, 'A', pt.x);
	SetUNO32AxisPosition(hDevice, 'B', pt.y);
	SetUNO32AxisPosition(hDevice, 'C', pt.z);

	return TRUE;
}

DWORD SetUNO32DevicePositionXY(HANDLE hDevice, UNO32_3DPOINT pt)
{
	SetUNO32AxisPosition(hDevice, 'A', pt.x);
	SetUNO32AxisPosition(hDevice, 'B', pt.y);
	//SetUNO32AxisPosition(hDevice, 'C', pt.z);

	return TRUE;
}

DWORD SetUNO32DevicePositionZ(HANDLE hDevice, UNO32_3DPOINT pt)
{
	//SetUNO32AxisPosition(hDevice, 'A', pt.x);
	//SetUNO32AxisPosition(hDevice, 'B', pt.y);
	SetUNO32AxisPosition(hDevice, 'C', pt.z);

	return TRUE;
}


DWORD SetUNO32DevicePosition(HANDLE hDevice, long long int x, long long int y, long long int z)
{
	SetUNO32AxisPosition(hDevice, 'A', x);
	SetUNO32AxisPosition(hDevice, 'B', y);
	SetUNO32AxisPosition(hDevice, 'C', z);

	return TRUE;
}

DWORD SetUNO32DevicePositionXY(HANDLE hDevice, long long int x, long long int y)
{
	SetUNO32AxisPosition(hDevice, 'A', x);
	SetUNO32AxisPosition(hDevice, 'B', y);

	return TRUE;
}

DWORD SetUNO32DevicePositionZ(HANDLE hDevice, long long int z)
{
	SetUNO32AxisPosition(hDevice, 'C', z);

	return TRUE;
}

DWORD SetUNO32DeviceVelocity(HANDLE hDevice, float xVel, float yVel, float zVel)
{
	SetUNO32AxisVelocity(hDevice, 'A', xVel);
	SetUNO32AxisVelocity(hDevice, 'B', yVel);
	SetUNO32AxisVelocity(hDevice, 'C', zVel);

	return TRUE;
}

DWORD SetUNO32DeviceVelocityXY(HANDLE hDevice, float xVel, float yVel)
{
	SetUNO32AxisVelocity(hDevice, 'A', xVel);
	SetUNO32AxisVelocity(hDevice, 'B', yVel);

	return TRUE;
}

DWORD SetUNO32DeviceVelocityZ(HANDLE hDevice, float zVel)
{
	SetUNO32AxisVelocity(hDevice, 'C', zVel);

	return TRUE;
}

DWORD SetUNO32AxisVelocity(HANDLE hDevice, CHAR axis, float velocity)
{
	CHAR str[MAX_STRING];

	sprintf(str, "!SETRATE%c%.0f", axis, abs(velocity));
	SerialSend(hDevice, str, NULL);

	return TRUE;
}

//'Compare' returns true if the two points are the same
DWORD Compare(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2)
{
	if(pt1.x == pt2.x)
		if(pt1.y == pt2.y)
			if(pt1.z == pt2.z)
				return TRUE;

	return FALSE;
}

DWORD CompareXY(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2)
{
	if(pt1.x == pt2.x){
		if(pt1.y == pt2.y){
			return TRUE;
		}
	}

	return FALSE;
}

DWORD CompareZ(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2)
{
	if(pt1.z == pt2.z){
			return TRUE;
		}

	return FALSE;
}

DWORD Compare(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2)
{
	if(Truncate3(pt1.x) == Truncate3(pt2.x))
		if(Truncate3(pt1.y) == Truncate3(pt2.y))
			if(Truncate3(pt1.z) == Truncate3(pt2.z))
				return TRUE;

	return FALSE;
}

DWORD CompareXY(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2)
{

	if(Truncate3(pt1.x) == Truncate3(pt2.x)){
		if(Truncate3(pt1.y) == Truncate3(pt2.y)){
			return TRUE;
		}
	}

	return FALSE;
}

FLOAT GetZDistance(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2)
{
	return pt1.z-pt2.z;
}


DWORD CompareZ(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2)
{
	if(Truncate3(pt1.z) == Truncate3(pt2.z)){
			return TRUE;
		}

	return FALSE;
}





DWORD UpdateUNO32Velocity(WNDUNO32 *lpWndUNO32)
{
	if(lpWndUNO32->flgShutter){
		lpWndUNO32->xVelMax = lpWndUNO32->xVelFine;
		lpWndUNO32->yVelMax = lpWndUNO32->yVelFine;
		lpWndUNO32->zVelMax = lpWndUNO32->zVelFine;
	}else{
		lpWndUNO32->xVelMax = lpWndUNO32->xVelCoarse;
		lpWndUNO32->yVelMax = lpWndUNO32->yVelCoarse;
		lpWndUNO32->zVelMax = lpWndUNO32->zVelCoarse;
	}

	return TRUE;
}

DWORD GetUNO32NavigationVector(UNO32_3DPOINT ptCurr, UNO32_3DPOINT ptDest, FPOINT center, VECTOR *vector)
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

DWORD GetUNO32NavigationVector(DOUBLE_3DPOINT ptCurr, DOUBLE_3DPOINT ptDest, FPOINT center, VECTOR *vector)
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

//DWORD GetUNO32Vector(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2, VECTOR *v)
//{
//	//VECTOR v;
//	FPOINT fpt1;
//	FPOINT fpt2;
//
//	fpt1.x = pt1.x;
//	fpt1.y = pt1.y;
//
//	fpt2.x = pt2.x;
//	fpt2.y = pt2.y;
//
//	GetVector(pt1, pt2, v);
//
//	return TRUE;
//}

DWORD GetUNO32VelocityVector(WNDUNO32 *lpWndUNO32, FLOAT velX, FLOAT velY, VECTOR *vector, TEXTR *text)
{
	if(!velX && !velY)
		return FALSE;

	VECTOR v = {0};
	FPOINT ptTail = {0};
	FPOINT ptHead;
	ptHead.x = velX;
	ptHead.y = velY;

	FLOAT tailPos = lpWndUNO32->roiDiameter*0.5*lpWndUNO32->pixPerUm;

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

DWORD GetUNO32ZAxisIndicator(WNDUNO32 *lpWndUNO32, CIRCLE *circle, TEXTR *text)
{
	INT velZ = (INT) lpWndUNO32->zVel;

	if(!velZ)
		return FALSE;

	INT max = 15;

	circle->x = lpWndUNO32->wFrame/2;
	circle->y = lpWndUNO32->hFrame/2;

	if(lpWndUNO32->driveC.dir)
		circle->r = 25 - ((FLOAT)max*((FLOAT)lpWndUNO32->zVel/(FLOAT)lpWndUNO32->zVelMax));

	if(!lpWndUNO32->driveC.dir)
		circle->r = 25 + ((FLOAT)(max+10)*((FLOAT)lpWndUNO32->zVel/(FLOAT)lpWndUNO32->zVelMax));

	return TRUE;
}

DWORD CreateUNO32Objects(WNDUNO32 *lpWndUNO32)
{

	Clean(&lpWndUNO32->os);

	CIRCLE c;
	c.r = 200;
	c.x = lpWndUNO32->wFrame/2;
	c.y = lpWndUNO32->hFrame/2; 

	
	Append(&lpWndUNO32->os, c, 2, ColorR(0, 0, 255), 0.25, NULL);

	c.r = lpWndUNO32->roiDiameter*lpWndUNO32->pixPerUm*0.5;
	//c.r = 25;
	c.x = lpWndUNO32->wFrame/2;
	c.y = lpWndUNO32->hFrame/2;

	if(lpWndUNO32->flgShutter){
		Append(&lpWndUNO32->os, c, 4, ColorR(255, 0, 0), 0.25, NULL);
	}else{
		Append(&lpWndUNO32->os, c, 4, ColorR(0, 0, 255), 0.25, NULL);
	}

	FLOAT xPol = lpWndUNO32->xPolStage;
	FLOAT yPol = -lpWndUNO32->yPolStage;
	FLOAT zPol = lpWndUNO32->zPolStage;

	FLOAT delZ;
	FLOAT delXY;
	FLOAT norm;
	VECTOR v;
	TEXTR t = {0};
	FPOINT ptCentroid;
	ptCentroid.x = c.x;
	ptCentroid.y = c.y;


	//MUST INVERT X-AXIS FOR CORRECT NAVIGATION VECTORS
	DOUBLE_3DPOINT dptCurrent;
	DOUBLE_3DPOINT dptX;
	DOUBLE_3DPOINT dptY;
	DOUBLE_3DPOINT dptA;
	DOUBLE_3DPOINT dptB;

	memcpy(&dptX, &lpWndUNO32->dptX, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptY, &lpWndUNO32->dptY, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptA, &lpWndUNO32->dptA, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptB, &lpWndUNO32->dptB, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptCurrent, &lpWndUNO32->dptCurrent, sizeof(DOUBLE_3DPOINT));

	dptX.x = xPol*dptX.x;
	dptY.x = xPol*dptY.x;
	dptA.x = xPol*dptA.x;
	dptB.x = xPol*dptB.x;
	dptCurrent.x = xPol*dptCurrent.x;

	dptX.y = yPol*dptX.y;
	dptY.y = yPol*dptY.y;
	dptA.y = yPol*dptA.y;
	dptB.y = yPol*dptB.y;
	dptCurrent.y = yPol*dptCurrent.y;

	dptX.z = yPol*dptX.z;
	dptY.z = yPol*dptY.z;
	dptA.z = yPol*dptA.z;
	dptB.z = yPol*dptB.z;
	dptCurrent.z = yPol*dptCurrent.z;


	Scale(dptX, &dptX, 1000);
	Scale(dptY, &dptY, 1000);
	Scale(dptA, &dptA, 1000);
	Scale(dptB, &dptB, 1000);
	Scale(dptCurrent, &dptCurrent, 1000);

	Truncate(dptX, &dptX);
	Truncate(dptY, &dptY);
	Truncate(dptA, &dptA);
	Truncate(dptB, &dptB);
	Truncate(dptCurrent, &dptCurrent);

	//FLOAT xVel = -1.0*lpWndUNO32->xVel;
	//FLOAT yVel = 1.0*lpWndUNO32->yVel;

	FLOAT xVel = lpWndUNO32->xPolStick*lpWndUNO32->xVel;
	FLOAT yVel = lpWndUNO32->yPolStick*lpWndUNO32->yVel;

	if(lpWndUNO32->flgXPosition){


		GetUNO32NavigationVector(dptCurrent, dptX, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptX);
		delZ = GetZDisplacement(dptCurrent, dptX);

		sprintf(t.str, "X(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, v, 2, ColorR(0, 0, 255), 0.5, NULL);
		Append(&lpWndUNO32->os, t, 2, ColorR(0, 0, 255), 0.5, NULL);
	}

	if(lpWndUNO32->flgYPosition){
		GetUNO32NavigationVector(dptCurrent, dptY, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptY);
		delZ = GetZDisplacement(dptCurrent, dptY);

		sprintf(t.str, "Y(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, t, 2, ColorR(255, 255, 0), 1.0, NULL);
		Append(&lpWndUNO32->os, v, 2, ColorR(255, 255, 0), 0.5, NULL);
	}

	if(lpWndUNO32->flgAPosition){
		GetUNO32NavigationVector(dptCurrent, dptA, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptA);
		delZ = GetZDisplacement(dptCurrent, dptA);

		sprintf(t.str, "A(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, t, 2, ColorR(0, 255, 0), 1.0, NULL);
		Append(&lpWndUNO32->os, v, 2, ColorR(0, 255, 0), 0.5, NULL);
	}

	if(lpWndUNO32->flgBPosition){
		GetUNO32NavigationVector(dptCurrent, dptB, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptB);
		delZ = GetZDisplacement(dptCurrent, dptB);

		sprintf(t.str, "B(%d,%d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, v, 2, ColorR(255, 0, 0), 0.5, NULL);
		Append(&lpWndUNO32->os, t, 2, ColorR(255, 0, 0), 0.5, NULL);
	}

	if(GetUNO32VelocityVector(lpWndUNO32, xVel, -yVel, &v, &t)){
		ShiftVector(ptCentroid, &v);
		Append(&lpWndUNO32->os, v, 2, ColorR(255, 255, 255), 0.5, NULL);
		t.pt.x+=ptCentroid.x;
		t.pt.y+=ptCentroid.y;
		Append(&lpWndUNO32->os, t, 2, ColorR(255, 255, 255), 1.0, NULL);
	}

	if(GetUNO32ZAxisIndicator(lpWndUNO32, &c, &t)){
		//ShiftVector(ptCentroid, &v);
		Append(&lpWndUNO32->os, c, 2, ColorR(255, 255, 255), 0.5, NULL);
		//t.pt.x+=ptCentroid.x;
		//t.pt.y+=ptCentroid.y;
		//Append(&lpWndUNO32->os, t, 2, ColorR(255, 255, 255), 1.0, NULL);
	}


	return TRUE;
}

DWORD GetVector(UNO32_3DPOINT ptHead, UNO32_3DPOINT ptTail, VECTOR *v)
{
	FPOINT fptHead;
	FPOINT fptTail;

	fptHead.x = ptHead.x;
	fptHead.y = ptHead.y;

	fptTail.x = ptTail.x;
	fptTail.y = ptTail.y;

	GetVector(fptHead, fptTail, v);

	return TRUE;
}

DWORD GetVector(DOUBLE_3DPOINT ptHead, DOUBLE_3DPOINT ptTail, VECTOR *v)
{
	FPOINT fptHead;
	FPOINT fptTail;

	fptHead.x = ptHead.x;
	fptHead.y = ptHead.y;

	fptTail.x = ptTail.x;
	fptTail.y = ptTail.y;

	GetVector(fptHead, fptTail, v);

	return TRUE;
}

INT GetXYDisplacement(UNO32_3DPOINT ptHead, UNO32_3DPOINT ptTail)
{
	VECTOR v = {0};

	GetVector(ptHead, ptTail, &v);

	return (INT)v.mag;
}

INT GetZDisplacement(UNO32_3DPOINT ptHead, UNO32_3DPOINT ptTail)
{

	return (INT)(ptHead.z-ptTail.z);
}

FLOAT GetXYDisplacement(DOUBLE_3DPOINT ptHead, DOUBLE_3DPOINT ptTail)
{
	VECTOR v = {0};

	GetVector(ptHead, ptTail, &v);

	return v.mag;
}

FLOAT GetZDisplacement(DOUBLE_3DPOINT ptHead, DOUBLE_3DPOINT ptTail)
{

	return (ptHead.z-ptTail.z);
}

FLOAT GetXYDisplacement(P3D ptHead, P3D ptTail)
{
	VECTOR v = {0};

	GetVector(ptHead, ptTail, &v);

	return v.mag;
}

FLOAT GetZDisplacement(P3D ptHead, P3D ptTail)
{

	return (ptHead.z-ptTail.z);
}

//FLOAT CalculateStickPositionXBOX(INT xInput, INT offset, INT sign)
//{
//	//FLOAT pos = 0.00;
//	FLOAT coef = (abs((FLOAT)(xInput))-OFFSET)/(FLOAT)32767;
//
//	coef = coef*sign;
//
//	//pos = ((FLOAT)(*x)*coef/((FLOAT)32767-OFFSET));
//
//	return ((FLOAT)(xInput)*coef/((FLOAT)32767-OFFSET));
//	//return pos;
//}



DWORD Truncate(double d, float *f)
{

	CHAR str[256];
	//CHAR strOut[256];
	//memset(strOut, NULL, 256*sizeof(CHAR));
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.1f", d);
	sscanf(str, "%f", f);

	return TRUE;
}

FLOAT Truncate(DOUBLE d)
{
	FLOAT f = 0.0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.0f", d);
	sscanf(str, "%f", &f);

	return f;
}

FLOAT Truncate1(DOUBLE d)
{
	FLOAT f = 0.0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.1f", d);
	sscanf(str, "%f", &f);

	return f;
}

FLOAT Truncate2(DOUBLE d)
{
	FLOAT f = 0.0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.2f", d);
	sscanf(str, "%f", &f);

	return f;
}

FLOAT Truncate3(DOUBLE d)
{
	FLOAT f = 0.0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.3f", d);
	sscanf(str, "%f", &f);

	return f;
}

FLOAT Truncate4(DOUBLE d)
{
	FLOAT f = 0.0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.4f", d);
	sscanf(str, "%f", &f);

	return f;
}

FLOAT Truncate5(DOUBLE d)
{
	FLOAT f = 0.0;
	CHAR str[256];
	memset(str, NULL, 256*sizeof(CHAR));
	sprintf(str, "%.5f", d);
	sscanf(str, "%f", &f);

	return f;
}

DWORD Truncate(DOUBLE_3DPOINT dptIn, DOUBLE_3DPOINT *dptOut)
{
	if(!dptOut)
		return FALSE;

	dptOut->x = Truncate(dptIn.x);
	dptOut->y = Truncate(dptIn.y);
	dptOut->z = Truncate(dptIn.z);

	return TRUE;
}

DWORD Scale(DOUBLE_3DPOINT dptIn, DOUBLE_3DPOINT *dptOut, FLOAT scale)
{
	if(!dptOut)
		return FALSE;

	dptOut->x = dptIn.x*scale;
	dptOut->y = dptIn.y*scale;
	dptOut->z = dptIn.z*scale;

	return TRUE;
}
