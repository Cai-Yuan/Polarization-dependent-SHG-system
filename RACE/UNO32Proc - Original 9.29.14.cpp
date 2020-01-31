#include "stdafx.h"
#include "UNO32.h"
#include "XBox.h"
#include "EnumerateDevice.h"
#include "CircularBuffer.h"
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

#include "A3200.h"

#define MAXSTICK 32767
#define OFFSET 7000


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

		lpWndUNO32->sampleRate = 5.0;
		lpWndUNO32->xScale = 0.215;
		lpWndUNO32->yScale = 0.215;
		lpWndUNO32->zScale = 0.215;
		//lpWndUNO32->xScale = 0.197;
		//lpWndUNO32->yScale = 0.197;
		//lpWndUNO32->zScale = 0.197;

		
		lpWndUNO32->pixPerUm = 6.56; //Pixels per um

		CreateBuffer(&lpWndUNO32->bufferA, 2);
		CreateBuffer(&lpWndUNO32->bufferB, 2);
		CreateBuffer(&lpWndUNO32->bufferC, 2);

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

		lpWndUNO32->xVelCoarse = 1;
		lpWndUNO32->yVelCoarse = 1;
		lpWndUNO32->zVelCoarse = 1;
		lpWndUNO32->xVelFine = 1;
		lpWndUNO32->yVelFine = 1;
		lpWndUNO32->zVelFine = 3;

		Edit_SetFloat(lpWndUNO32->hEditXVelFine, lpWndUNO32->xVelFine);
		Edit_SetFloat(lpWndUNO32->hEditYVelFine, lpWndUNO32->yVelFine);
		Edit_SetFloat(lpWndUNO32->hEditZVelFine, lpWndUNO32->zVelFine);
		Edit_SetFloat(lpWndUNO32->hEditXVelCoarse, lpWndUNO32->xVelCoarse);
		Edit_SetFloat(lpWndUNO32->hEditYVelCoarse, lpWndUNO32->yVelCoarse);
		Edit_SetFloat(lpWndUNO32->hEditZVelCoarse, lpWndUNO32->zVelCoarse);

		Edit_SetFloat(lpWndUNO32->hEditSampleRate, lpWndUNO32->sampleRate);

		Button_SetCheck(lpWndUNO32->hCheckZDelay, BST_CHECKED);

		lpWndUNO32->flgBPosition = TRUE;

		CreateThread(NULL, NULL, UNO32Thread, (LPVOID) lpWndUNO32, NULL, NULL);

		if(!A3200Connect(&lpWndUNO32->hAero)){
			MessageBox(NULL, "Drivers Not Found!", "Drivers Not Found!", MB_OK);
		}

		if(lpWndUNO32->hAero){
			A3200MotionFaultAck(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
			if(!A3200MotionEnable(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02)){
				MessageBox(NULL, "Drivers Not Enabled!", "Drivers Not Enabled!", MB_OK);
			}
		}

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

		if(lpWndUNO32->hAero){
			if(!A3200MotionDisable(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00 | AXISMASK_01 | AXISMASK_02)){
				MessageBox(NULL, "Drivers Not Enabled!", "Drivers Not Enabled!", MB_OK);
			}
		}
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

			xJog = CalculateStickPositionXBOX(*x, OFFSET, 1);

			lpWndUNO32->xStick = xJog;

			lpWndUNO32->flgXInput = TRUE;
			return TRUE;
		case XB_LEFTSTICK_Y:
			y = (SHORT *)lParam;

			yJog = CalculateStickPositionXBOX(*y, OFFSET, -1);

			lpWndUNO32->yStick = yJog;

			lpWndUNO32->flgYInput = TRUE;
			return TRUE;
		case XB_RIGHTSTICK_Y:
			z = (SHORT *)lParam;

			zJog = CalculateStickPositionXBOX(*z, OFFSET, 1);

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

DWORD WINAPI UNO32Thread(LPVOID param)
{

	WNDUNO32 *lpWndUNO32 = (WNDUNO32 *)param;

	if(!lpWndUNO32)
		return FALSE;

	CHAR str[MAX_STRING];
	FLOAT prd = ((FLOAT) 1000 / (FLOAT)lpWndUNO32->sampleRate);
	DWORD tUpdate = clock()+prd;
	clock_t cTime = NULL;

	while(1){

		prd = ((FLOAT) 1000 / (FLOAT)lpWndUNO32->sampleRate);
		cTime = clock();
		if(cTime >= tUpdate){
			tUpdate = tUpdate+prd;

			//if(lpWndUNO32->xVel){
			//	GetUNO32DevicePositionX(lpWndUNO32->hDevice, &lpWndUNO32->ptCurrent);
			//}
			//if(lpWndUNO32->yVel){
			//	GetUNO32DevicePositionY(lpWndUNO32->hDevice, &lpWndUNO32->ptCurrent);
			//}
			//if(lpWndUNO32->zVel){
			//	GetUNO32DevicePositionZ(lpWndUNO32->hDevice, &lpWndUNO32->ptCurrent);
			//}
			DOUBLE posX;
			DOUBLE posY;
			DOUBLE posZ;
			A3200StatusGetItem(lpWndUNO32->hAero, 0, STATUSITEM_PositionFeedback, 0, &posX);
			A3200StatusGetItem(lpWndUNO32->hAero, 1, STATUSITEM_PositionFeedback, 0, &posY);
			A3200StatusGetItem(lpWndUNO32->hAero, 2, STATUSITEM_PositionFeedback, 0, &posZ);
			
			//GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptCurrent);
			UpdateUNO32PositionDisplay(lpWndUNO32->ptCurrent, lpWndUNO32);

			//if(lpWndUNO32->flgMoveTo && !lpWndUNO32->flgZDelay){
			if(lpWndUNO32->flgMoveTo){
				if(!lpWndUNO32->flgZDelay){
				//Remove the MoveToFlag
					if(Compare(lpWndUNO32->ptCurrent, lpWndUNO32->ptMoveTo)){
						lpWndUNO32->driveA.dir = !lpWndUNO32->driveA.dir;
						lpWndUNO32->driveB.dir = !lpWndUNO32->driveB.dir;
						lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
						UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->xVel, &lpWndUNO32->driveA);
						UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->yVel, &lpWndUNO32->driveB);
						UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);

						lpWndUNO32->xVel = 0;
						lpWndUNO32->yVel = 0;
						lpWndUNO32->flgMoveTo = FALSE;
					}
				}

				if(lpWndUNO32->flgZDelay){
					if(!lpWndUNO32->flgMoveToZ){	
						if(CompareXY(lpWndUNO32->ptCurrent, lpWndUNO32->ptMoveTo)){
							SetUNO32DevicePositionZ(lpWndUNO32->hDevice, lpWndUNO32->ptMoveTo);
							SetUNO32DeviceVelocityZ(lpWndUNO32->hDevice, 
								(lpWndUNO32->zVelMax/lpWndUNO32->zScale));
							lpWndUNO32->driveA.dir = !lpWndUNO32->driveA.dir;
							lpWndUNO32->driveB.dir = !lpWndUNO32->driveB.dir;

							UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->xVel, &lpWndUNO32->driveA);
							UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->yVel, &lpWndUNO32->driveB);

							lpWndUNO32->xVel = 0;
							lpWndUNO32->yVel = 0;
							lpWndUNO32->flgMoveToZ = TRUE;
							lpWndUNO32->zVel = lpWndUNO32->zVelMax;
						}
					}
				}

				if(lpWndUNO32->flgMoveToZ){
					if(CompareZ(lpWndUNO32->ptCurrent, lpWndUNO32->ptMoveTo)){
						lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
						UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);
						lpWndUNO32->flgMoveTo = FALSE;
						lpWndUNO32->flgMoveToZ = FALSE;
						lpWndUNO32->zVel = 0;

						lpWndUNO32->driveA.dir = !lpWndUNO32->driveA.dir;
						lpWndUNO32->driveB.dir = !lpWndUNO32->driveB.dir;
						lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;

						//PostMessage(lpWndUNO32->hShutter, WM_TODO, (WPARAM) SHUTTER_CLOSE, NULL);
						//lpWndUNO32->flgShutter = FALSE;
						UpdateUNO32Velocity(lpWndUNO32);
					}
				}


			}	

			CreateUNO32Objects(lpWndUNO32);
 
			SendMessage(lpWndUNO32->hDataView, WM_SETOBJECT, NULL, (LPARAM)&lpWndUNO32->os);
		}

		if(lpWndUNO32->flgInitMoveTo){

			lpWndUNO32->flgMoveTo = TRUE;

			VECTOR v;
			GetVector(lpWndUNO32->ptMoveTo, lpWndUNO32->ptCurrent, &v);
			lpWndUNO32->xVel = lpWndUNO32->xVelMax*cos((v.dir*PI)/180.0);
			lpWndUNO32->yVel = lpWndUNO32->yVelMax*sin((v.dir*PI)/180.0);
				
				if(lpWndUNO32->xVelMax != lpWndUNO32->xVelCoarse){
					SetUNO32DevicePositionXY(lpWndUNO32->hDevice, lpWndUNO32->ptMoveTo);
					SetUNO32DeviceVelocityXY(lpWndUNO32->hDevice, 
						(lpWndUNO32->xVel/lpWndUNO32->xScale), 
						(lpWndUNO32->yVel/lpWndUNO32->yScale));
				}else{
					lpWndUNO32->zVel = lpWndUNO32->zVelMax;
					SetUNO32DevicePosition(lpWndUNO32->hDevice, lpWndUNO32->ptMoveTo);
					SetUNO32DeviceVelocity(lpWndUNO32->hDevice, 
						(lpWndUNO32->xVel/lpWndUNO32->xScale), 
						(lpWndUNO32->yVel/lpWndUNO32->yScale), 
						(lpWndUNO32->zVel/lpWndUNO32->zScale));
				}

			lpWndUNO32->flgInitMoveTo = FALSE;

		}


		if(!lpWndUNO32->flgMoveTo){
			if(lpWndUNO32->flgXInput || lpWndUNO32->flgYInput){

				FLOAT xStick = lpWndUNO32->xStick;
				FLOAT yStick = lpWndUNO32->yStick;
				FLOAT norm = Norm(xStick, yStick);

				VECTOR v;
				FPOINT p1;
				FPOINT p2;
				p1.x = 0;
				p1.y = 0;
				p2.x = xStick;
				p2.y = yStick;
				GetVector(p2, p1, &v);
				FLOAT anglRad = (v.dir*PI)/180.0;

				lpWndUNO32->xVel = (abs(v.i)+abs(v.j))*lpWndUNO32->xVelMax*cos(anglRad);
				lpWndUNO32->yVel = (abs(v.i)+abs(v.j))*lpWndUNO32->yVelMax*sin(anglRad);

				A3200MotionFreeRun(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, lpWndUNO32->xVel);
				A3200MotionFreeRun(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, lpWndUNO32->yVel);

				Edit_SetFloat(lpWndUNO32->hEditXVel, lpWndUNO32->xVel/**lpWndUNO32->xScale*/);
				Edit_SetFloat(lpWndUNO32->hEditYVel, lpWndUNO32->yVel/**lpWndUNO32->xScale*/);
				
				//UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->xVel, &lpWndUNO32->driveA);
				//sprintf(str, "!SETRATEA%.0f", (abs(lpWndUNO32->xVel)/lpWndUNO32->xScale));
				//SerialSend(lpWndUNO32->hDevice, str, NULL); 
				lpWndUNO32->flgXInput = FALSE;

				//UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->yVel, &lpWndUNO32->driveB);
				//sprintf(str, "!SETRATEB%.0f", (abs(lpWndUNO32->yVel)/lpWndUNO32->yScale));
				//SerialSend(lpWndUNO32->hDevice, str, NULL); 
				lpWndUNO32->flgYInput = FALSE;
			}

			if(lpWndUNO32->flgZInput){

				lpWndUNO32->zVel = lpWndUNO32->zStick*lpWndUNO32->zVelMax;
				Edit_SetFloat(lpWndUNO32->hEditZVel, lpWndUNO32->zVel/**lpWndUNO32->xScale*/);
				A3200MotionFreeRun(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->zVel);
				//UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->zVel, &lpWndUNO32->driveC);
				//sprintf(str, "!SETRATEC%.0f", (abs(lpWndUNO32->zVel)/lpWndUNO32->zScale));
				//SerialSend(lpWndUNO32->hDevice, str, NULL); 
				lpWndUNO32->flgZInput = FALSE;
			}
		}

		if(lpWndUNO32->flgAButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptA);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptA, sizeof(UNO32_3DPOINT));
				//lpWndUNO32->flgMoveTo = TRUE;
				lpWndUNO32->flgInitMoveTo = TRUE;
				lpWndUNO32->flgLeftShoulder = NULL;
			}
			lpWndUNO32->flgAButton = NULL;
			lpWndUNO32->flgAPosition = TRUE;
		}

		if(lpWndUNO32->flgBButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptB);	
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptB, sizeof(UNO32_3DPOINT));
				//lpWndUNO32->flgMoveTo = TRUE;
				lpWndUNO32->flgInitMoveTo = TRUE;
				lpWndUNO32->flgLeftShoulder = NULL;
			}
			lpWndUNO32->flgBButton = NULL;
			lpWndUNO32->flgBPosition = TRUE;
		}
		if(lpWndUNO32->flgXButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptX);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptX, sizeof(UNO32_3DPOINT));
				//lpWndUNO32->flgMoveTo = TRUE;
				lpWndUNO32->flgInitMoveTo = TRUE;
				lpWndUNO32->flgLeftShoulder = NULL;
			}
			lpWndUNO32->flgXButton = NULL;
			lpWndUNO32->flgXPosition = TRUE;
		}

		if(lpWndUNO32->flgYButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptY);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptY, sizeof(UNO32_3DPOINT));
				//lpWndUNO32->flgMoveTo = TRUE;
				lpWndUNO32->flgInitMoveTo = TRUE;
			}
			lpWndUNO32->flgYButton = NULL;
			lpWndUNO32->flgYPosition = TRUE;
		}

		if(lpWndUNO32->flgBackButton || lpWndUNO32->flgLeftTrigger){
			if(lpWndUNO32->flgMoveTo){
				SetUNO32DevicePosition(lpWndUNO32->hDevice, lpWndUNO32->ptCurrent.x, lpWndUNO32->ptCurrent.y, lpWndUNO32->ptCurrent.z);
				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptCurrent, sizeof(UNO32_3DPOINT));
				lpWndUNO32->flgBackButton = NULL;
			}
			lpWndUNO32->flgLeftTrigger = NULL;
		}

		if(lpWndUNO32->flgZero){
			sprintf(str, "!SETPOSA%d", 0);
			SerialSend(lpWndUNO32->hDevice, str, NULL);
			sprintf(str, "!SETPOSB%d", 0);
			SerialSend(lpWndUNO32->hDevice, str, NULL);
			sprintf(str, "!SETPOSC%d", 0);
			SerialSend(lpWndUNO32->hDevice, str, NULL);
			lpWndUNO32->flgZero = NULL;
		}

		if(lpWndUNO32->flgZStack){
			long long int zRef = lpWndUNO32->ptZRef.z;
			long long int zGoTo;
			INT slice = lpWndUNO32->slice;
			FLOAT stepSize = lpWndUNO32->stepSize;
			INT w = lpWndUNO32->pdmIn.pdmfh.width;
			INT h = lpWndUNO32->pdmIn.pdmfh.height;
			CHAR str[MAX_STRING];
			PDMFRAME pdmf;
			AllocPDMFrame(&pdmf, w, h, DT_PIXEL);

			sprintf(str, "%s\Slice%d.bmp", lpWndUNO32->strFolder, lpWndUNO32->slice);

			if(lpWndUNO32->slice == 0){
				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptZRef);
				//memcpy(&lpWndUNO32->ptZRef, &lpWndUNO32->ptCurrent, sizeof(UNO32_3DPOINT));
				SetUNO32DevicePosition(lpWndUNO32->hDevice, 
										(lpWndUNO32->ptCurrent.x),
										(lpWndUNO32->ptCurrent.y),
										(lpWndUNO32->ptCurrent.z));
				lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
				UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);
				SetUNO32DeviceVelocity(lpWndUNO32->hDevice, 
										(10/lpWndUNO32->xScale),
										(10/lpWndUNO32->yScale),
										(10/lpWndUNO32->zScale));
				Sleep(100);
				lpWndUNO32->flgFrameIn = FALSE;
				while(!lpWndUNO32->flgFrameIn){
					Sleep(1);
				}
				CopyPDMFrameUCHARtoPIXEL(&pdmf, &lpWndUNO32->pdmIn);
				SaveBMP(w, h, str, (PIXEL *)pdmf.lpvoid);
			}

			zGoTo = (zRef*lpWndUNO32->zScale) - (slice*stepSize);
			SetUNO32DevicePositionZ(lpWndUNO32->hDevice, (zGoTo/lpWndUNO32->zScale));
			SetUNO32DeviceVelocityZ(lpWndUNO32->hDevice, (100/lpWndUNO32->zScale));
			Sleep(100);
			lpWndUNO32->flgFrameIn = FALSE;
			while(!lpWndUNO32->flgFrameIn){
				Sleep(1);
			}
			CopyPDMFrameUCHARtoPIXEL(&pdmf, &lpWndUNO32->pdmIn);
			SaveBMP(w, h, str, (PIXEL *)pdmf.lpvoid);
			lpWndUNO32->slice++;

			if(lpWndUNO32->slice == lpWndUNO32->nSlice){
				SetUNO32DevicePosition(lpWndUNO32->hDevice, lpWndUNO32->ptZRef);
				SetUNO32DeviceVelocity(lpWndUNO32->hDevice, 
					(300/lpWndUNO32->xScale),
					(300/lpWndUNO32->yScale),
					(300/lpWndUNO32->zScale));
				lpWndUNO32->slice = 0;
				lpWndUNO32->flgZStack = FALSE;
				Sleep(200);
			}

		}

		//lpWndUNO32->flgLeftShoulder = NULL;
		//lpWndUNO32->flgRightShoulder = NULL;
		
		
		

		




	}


	return FALSE;
}

//Must thread!!!!
//GetUNO32Device Position slows everything down
//DWORD WINAPI UNO32Thread(LPVOID param)
//{
//
//	WNDUNO32 *lpWndUNO32 = (WNDUNO32 *)param;
//
//	if(!lpWndUNO32)
//		return FALSE;
//
//	CHAR str[MAX_STRING];
//	FLOAT prd = ((FLOAT) 1000 / (FLOAT)lpWndUNO32->sampleRate);
//	DWORD tUpdate = clock()+prd;
//	clock_t cTime = NULL;
//
//	while(1){
//
//		prd = ((FLOAT) 1000 / (FLOAT)lpWndUNO32->sampleRate);
//		cTime = clock();
//		if(cTime >= tUpdate){
//			tUpdate = tUpdate+prd;
//
//			GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptCurrent);
//			UpdateUNO32PositionDisplay(lpWndUNO32->ptCurrent, lpWndUNO32);
//
//			if(lpWndUNO32->flgMoveTo && !lpWndUNO32->flgZDelay){
//				//Remove the MoveToFlag
//				if(Compare(lpWndUNO32->ptCurrent, lpWndUNO32->ptMoveTo)){
//					lpWndUNO32->driveA.dir = !lpWndUNO32->driveA.dir;
//					lpWndUNO32->driveB.dir = !lpWndUNO32->driveB.dir;
//					lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
//					UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->xVel, &lpWndUNO32->driveA);
//					UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->yVel, &lpWndUNO32->driveB);
//					UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);
//
//					lpWndUNO32->xVel = 0;
//					lpWndUNO32->yVel = 0;
//					lpWndUNO32->flgMoveTo = FALSE;
//
//				}
//			}else if(lpWndUNO32->flgMoveTo && lpWndUNO32->flgZDelay && !lpWndUNO32->flgMoveToZ){	
//				if(CompareXY(lpWndUNO32->ptCurrent, lpWndUNO32->ptMoveTo)){
//					SetUNO32DevicePositionZ(lpWndUNO32->hDevice, lpWndUNO32->ptMoveTo);
//					SetUNO32DeviceVelocityZ(lpWndUNO32->hDevice, 
//						(lpWndUNO32->zVelMax/lpWndUNO32->zScale));
//					lpWndUNO32->driveA.dir = !lpWndUNO32->driveA.dir;
//					lpWndUNO32->driveB.dir = !lpWndUNO32->driveB.dir;
//					//lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
//					UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->xVel, &lpWndUNO32->driveA);
//					UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->yVel, &lpWndUNO32->driveB);
//					//UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);
//					//lpWndUNO32->flgMoveTo = FALSE;
//					lpWndUNO32->xVel = 0;
//					lpWndUNO32->yVel = 0;
//					lpWndUNO32->flgMoveToZ = TRUE;
//					lpWndUNO32->zVel = lpWndUNO32->zVelMax;
//
//
//				}
//			}else if(lpWndUNO32->flgMoveToZ){
//				if(CompareZ(lpWndUNO32->ptCurrent, lpWndUNO32->ptMoveTo)){
//					lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
//					UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);
//					lpWndUNO32->flgMoveTo = FALSE;
//					lpWndUNO32->flgMoveToZ = FALSE;
//
//					lpWndUNO32->driveA.dir = !lpWndUNO32->driveA.dir;
//					lpWndUNO32->driveB.dir = !lpWndUNO32->driveB.dir;
//					lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
//
//					lpWndUNO32->zVel = 0;
//
//					PostMessage(lpWndUNO32->hShutter, WM_TODO, (WPARAM) SHUTTER_CLOSE, NULL);
//					lpWndUNO32->flgShutter = FALSE;
//					UpdateUNO32Velocity(lpWndUNO32);
//				}
//			}else{}
//
//			CreateUNO32Objects(lpWndUNO32);
//
//			SendMessage(lpWndUNO32->hDataView, WM_SETOBJECT, NULL, (LPARAM)&lpWndUNO32->os);
//		}
//
//		if(lpWndUNO32->flgStartButton == XB_BUTTON_PRESSED){
//			//lpWndUNO32->flgZero = TRUE;
//			lpWndUNO32->flgStartButton = FALSE;
//		}
//
//		if(lpWndUNO32->flgAButton == XB_BUTTON_PRESSED){
//			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
//				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptA);
//				lpWndUNO32->flgRightShoulder = NULL;
//			}
//			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
//				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptA, sizeof(UNO32_3DPOINT));
//				lpWndUNO32->flgMoveTo = TRUE;
//				lpWndUNO32->flgInitMoveTo = TRUE;
//				lpWndUNO32->flgLeftShoulder = NULL;
//			}
//			lpWndUNO32->flgAButton = NULL;
//			lpWndUNO32->flgAPosition = TRUE;
//		}
//
//		if(lpWndUNO32->flgBButton == XB_BUTTON_PRESSED){
//			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
//				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptB);	
//				lpWndUNO32->flgRightShoulder = NULL;
//			}
//			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
//				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptB, sizeof(UNO32_3DPOINT));
//				lpWndUNO32->flgMoveTo = TRUE;
//				lpWndUNO32->flgInitMoveTo = TRUE;
//				lpWndUNO32->flgLeftShoulder = NULL;
//			}
//			lpWndUNO32->flgBButton = NULL;
//			lpWndUNO32->flgBPosition = TRUE;
//		}
//		if(lpWndUNO32->flgXButton == XB_BUTTON_PRESSED){
//			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
//				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptX);
//				lpWndUNO32->flgRightShoulder = NULL;
//			}
//			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
//				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptX, sizeof(UNO32_3DPOINT));
//				lpWndUNO32->flgMoveTo = TRUE;
//				lpWndUNO32->flgInitMoveTo = TRUE;
//				lpWndUNO32->flgLeftShoulder = NULL;
//			}
//			lpWndUNO32->flgXButton = NULL;
//			lpWndUNO32->flgXPosition = TRUE;
//		}
//
//		if(lpWndUNO32->flgYButton == XB_BUTTON_PRESSED){
//			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
//				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptY);
//				lpWndUNO32->flgRightShoulder = NULL;
//			}
//			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
//				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptY, sizeof(UNO32_3DPOINT));
//				lpWndUNO32->flgMoveTo = TRUE;
//				lpWndUNO32->flgInitMoveTo = TRUE;
//			}
//			lpWndUNO32->flgYButton = NULL;
//			lpWndUNO32->flgYPosition = TRUE;
//		}
//
//		if(lpWndUNO32->flgInitMoveTo){
//			INT flg = FALSE;
//				if(lpWndUNO32->flgZDelay){
//					flg = TRUE;
//				}
//				if(lpWndUNO32->xVel != lpWndUNO32->xVelCoarse){
//
//					VECTOR v;
//					GetVector(lpWndUNO32->ptMoveTo, lpWndUNO32->ptCurrent, &v);
//					lpWndUNO32->xVel = lpWndUNO32->xVelMax*cos((v.dir*PI)/180.0);
//					lpWndUNO32->yVel = lpWndUNO32->yVelMax*sin((v.dir*PI)/180.0);
//					SetUNO32DevicePositionXY(lpWndUNO32->hDevice, lpWndUNO32->ptMoveTo);
//					//SetUNO32DeviceVelocityXY(lpWndUNO32->hDevice, 
//						//(lpWndUNO32->xVelMax/lpWndUNO32->xScale), 
//						//(lpWndUNO32->yVelMax/lpWndUNO32->yScale));
//					SetUNO32DeviceVelocityXY(lpWndUNO32->hDevice, 
//						(lpWndUNO32->xVel/lpWndUNO32->xScale), 
//						(lpWndUNO32->yVel/lpWndUNO32->yScale));
//				}else{
//					lpWndUNO32->zVel = lpWndUNO32->zVelMax;
//					SetUNO32DevicePosition(lpWndUNO32->hDevice, lpWndUNO32->ptMoveTo);
//					SetUNO32DeviceVelocity(lpWndUNO32->hDevice, 
//						(lpWndUNO32->xVelMax/lpWndUNO32->xScale), 
//						(lpWndUNO32->yVelMax/lpWndUNO32->yScale), 
//						(lpWndUNO32->zVelMax/lpWndUNO32->zScale));
//				}
//
//			lpWndUNO32->flgInitMoveTo = FALSE;
//		}
//
//
//		if(!lpWndUNO32->flgMoveTo){
//			if(lpWndUNO32->flgXInput || lpWndUNO32->flgYInput || lpWndUNO32->flgZInput){
//
//				FLOAT xStick = lpWndUNO32->xStick;
//				FLOAT yStick = lpWndUNO32->yStick;
//				FLOAT norm = Norm(xStick, yStick);
//
//				VECTOR v;
//				FPOINT p1;
//				FPOINT p2;
//				p1.x = 0;
//				p1.y = 0;
//				p2.x = xStick;
//				p2.y = yStick;
//				GetVector(p2, p1, &v);
//				FLOAT anglRad = (v.dir*PI)/180.0;
//
//				lpWndUNO32->xVel = (abs(v.i)+abs(v.j))*lpWndUNO32->xVelMax*cos(anglRad);
//				lpWndUNO32->yVel = (abs(v.i)+abs(v.j))*lpWndUNO32->yVelMax*sin(anglRad);
//				//lpWndUNO32->xVel = lpWndUNO32->xStick*lpWndUNO32->xVelMax;
//				//lpWndUNO32->yVel = lpWndUNO32->yStick*lpWndUNO32->yVelMax;
//				lpWndUNO32->zVel = lpWndUNO32->zStick*lpWndUNO32->zVelMax;
//
//				Edit_SetFloat(lpWndUNO32->hEditXVel, lpWndUNO32->xVel/**lpWndUNO32->xScale*/);
//				Edit_SetFloat(lpWndUNO32->hEditYVel, lpWndUNO32->yVel/**lpWndUNO32->xScale*/);
//				Edit_SetFloat(lpWndUNO32->hEditZVel, lpWndUNO32->zVel/**lpWndUNO32->xScale*/);
//
//				UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->xVel, &lpWndUNO32->driveA);
//				sprintf(str, "!SETRATEA%.0f", (abs(lpWndUNO32->xVel)/lpWndUNO32->xScale));
//				SerialSend(lpWndUNO32->hDevice, str, NULL); 
//				lpWndUNO32->flgXInput = FALSE;
//
//				UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->yVel, &lpWndUNO32->driveB);
//				sprintf(str, "!SETRATEB%.0f", (abs(lpWndUNO32->yVel)/lpWndUNO32->yScale));
//				SerialSend(lpWndUNO32->hDevice, str, NULL); 
//				lpWndUNO32->flgYInput = FALSE;
//
//				UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->zVel, &lpWndUNO32->driveC);
//				sprintf(str, "!SETRATEC%.0f", (abs(lpWndUNO32->zVel)/lpWndUNO32->zScale));
//				SerialSend(lpWndUNO32->hDevice, str, NULL); 
//				lpWndUNO32->flgZInput = FALSE;
//			}
//		}
//
//		if(lpWndUNO32->flgVelUpdate){
//			//VECTOR v;
//			//GetVector(lpWndUNO32->ptMoveTo, lpWndUNO32->ptCurrent, &v);
//			//lpWndUNO32->xVel = lpWndUNO32->xVelMax*cos((v.dir*PI)/180.0);
//			//lpWndUNO32->yVel = lpWndUNO32->yVelMax*sin((v.dir*PI)/180.0);
//			//SetUNO32DeviceVelocityXY(lpWndUNO32->hDevice, 
//			//	(lpWndUNO32->xVel/lpWndUNO32->xScale), 
//			//	(lpWndUNO32->yVel/lpWndUNO32->yScale));
//			lpWndUNO32->flgVelUpdate = NULL;
//		}
//
//		if(lpWndUNO32->flgBackButton || lpWndUNO32->flgLeftTrigger){
//			if(lpWndUNO32->flgMoveTo){
//				SetUNO32DevicePosition(lpWndUNO32->hDevice, lpWndUNO32->ptCurrent.x, lpWndUNO32->ptCurrent.y, lpWndUNO32->ptCurrent.z);
//				memcpy(&lpWndUNO32->ptMoveTo, &lpWndUNO32->ptCurrent, sizeof(UNO32_3DPOINT));
//				lpWndUNO32->flgBackButton = NULL;
//			}
//			lpWndUNO32->flgLeftTrigger = NULL;
//		}
//
//		if(lpWndUNO32->flgZero){
//			sprintf(str, "!SETPOSA%d", 0);
//			SerialSend(lpWndUNO32->hDevice, str, NULL);
//			sprintf(str, "!SETPOSB%d", 0);
//			SerialSend(lpWndUNO32->hDevice, str, NULL);
//			sprintf(str, "!SETPOSC%d", 0);
//			SerialSend(lpWndUNO32->hDevice, str, NULL);
//			lpWndUNO32->flgZero = NULL;
//		}
//
//		//lpWndUNO32->flgLeftShoulder = NULL;
//		//lpWndUNO32->flgRightShoulder = NULL;
//		
//		
//		
//
//		
//
//
//
//
//	}
//
//
//	return FALSE;
//}


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

	FLOAT delZ;
	FLOAT delXY;
	FLOAT norm;
	VECTOR v;
	TEXTR t = {0};
	FPOINT ptCentroid;
	ptCentroid.x = c.x;
	ptCentroid.y = c.y;


	if(lpWndUNO32->flgXPosition){
		GetUNO32NavigationVector(lpWndUNO32->ptCurrent, lpWndUNO32->ptX, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptX);
		delZ = GetZDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptX);

		//norm = Norm(lpWndUNO32->xScale, lpWndUNO32->yScale);
		delXY = delXY*lpWndUNO32->xScale;
		delZ = delZ*lpWndUNO32->zScale;
		sprintf(t.str, "X(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, v, 2, ColorR(0, 0, 255), 0.5, NULL);
		Append(&lpWndUNO32->os, t, 2, ColorR(0, 0, 255), 0.5, NULL);
	}

	if(lpWndUNO32->flgYPosition){
		GetUNO32NavigationVector(lpWndUNO32->ptCurrent, lpWndUNO32->ptY, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptY);
		delZ = GetZDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptY);

		delXY = delXY*lpWndUNO32->xScale;
		//delXY = delXY*Norm(lpWndUNO32->xScale, lpWndUNO32->yScale);
		delZ = delZ*lpWndUNO32->zScale;
		sprintf(t.str, "Y(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, t, 2, ColorR(255, 255, 0), 1.0, NULL);
		Append(&lpWndUNO32->os, v, 2, ColorR(255, 255, 0), 0.5, NULL);
	}

	if(lpWndUNO32->flgAPosition){
		GetUNO32NavigationVector(lpWndUNO32->ptCurrent, lpWndUNO32->ptA, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptA);
		delZ = GetZDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptA);

		delXY = delXY*lpWndUNO32->xScale;
		//delXY = delXY*Norm(lpWndUNO32->xScale, lpWndUNO32->yScale);
		delZ = delZ*lpWndUNO32->zScale;
		sprintf(t.str, "A(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, t, 2, ColorR(0, 255, 0), 1.0, NULL);
		Append(&lpWndUNO32->os, v, 2, ColorR(0, 255, 0), 0.5, NULL);
	}

	if(lpWndUNO32->flgBPosition){
		GetUNO32NavigationVector(lpWndUNO32->ptCurrent, lpWndUNO32->ptB, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptB);
		delZ = GetZDisplacement(lpWndUNO32->ptCurrent, lpWndUNO32->ptB);

		delXY = delXY*lpWndUNO32->xScale;
		//delXY = delXY*Norm(lpWndUNO32->xScale, lpWndUNO32->yScale);
		delZ = delZ*lpWndUNO32->zScale;
		sprintf(t.str, "B(%d,%d)", (INT)delXY, (INT)delZ);
		Append(&lpWndUNO32->os, v, 2, ColorR(255, 0, 0), 0.5, NULL);
		Append(&lpWndUNO32->os, t, 2, ColorR(255, 0, 0), 0.5, NULL);
	}

	if(GetUNO32VelocityVector(lpWndUNO32, lpWndUNO32->xVel, lpWndUNO32->yVel, &v, &t)){

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

FLOAT CalculateStickPositionXBOX(INT xInput, INT offset, INT sign)
{
	//FLOAT pos = 0.00;
	FLOAT coef = (abs((FLOAT)(xInput))-OFFSET)/(FLOAT)32767;

	coef = coef*sign;

	//pos = ((FLOAT)(*x)*coef/((FLOAT)32767-OFFSET));

	return ((FLOAT)(xInput)*coef/((FLOAT)32767-OFFSET));
	//return pos;
}



		//if(!lpWndUNO32->flgMoveTo){
		//	if(lpWndUNO32->flgXInput){
		//		UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->xVel, &lpWndUNO32->driveA);
		//		sprintf(str, "!SETRATEA%.0f", (abs(lpWndUNO32->xVel)/lpWndUNO32->xScale));
		//		SerialSend(lpWndUNO32->hDevice, str, NULL); 
		//		lpWndUNO32->flgXInput = FALSE;
		//	}

		//	if(lpWndUNO32->flgYInput){
		//		UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->yVel, &lpWndUNO32->driveB);
		//		sprintf(str, "!SETRATEB%.0f", (abs(lpWndUNO32->yVel)/lpWndUNO32->yScale));
		//		SerialSend(lpWndUNO32->hDevice, str, NULL); 
		//		lpWndUNO32->flgYInput = FALSE;
		//	}

		//	if(lpWndUNO32->flgZInput){
		//		UpdateDriveDirection(lpWndUNO32->hDevice, lpWndUNO32->zVel, &lpWndUNO32->driveC);
		//		sprintf(str, "!SETRATEC%.0f", (abs(lpWndUNO32->zVel)/lpWndUNO32->zScale));
		//		SerialSend(lpWndUNO32->hDevice, str, NULL); 
		//		lpWndUNO32->flgZInput = FALSE;
		//	}
		//}