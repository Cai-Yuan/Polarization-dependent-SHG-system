#include "stdafx.h"

#include "RACE.h"
#include "PropertyManager.h"


//Positions in ComboBox
#define EP_NONE				0
#define EP_PARTICLETRACK	1
#define EP_IMAGESUBTRACT	2
#define EP_GRADIENTSUBTRACT	3
#define EP_LOGOPERATOR1X	4
#define EP_LOGOPERATOR2X	5
#define EP_LOGOPERATOR3X	6
#define EP_GRADIENTPLOW		7
#define EP_GRADIENTPLOWINV	8
#define EP_BLOBLOG1			9
#define EP_BLOBLOG3			10
#define EP_BLOBLOG5			11
#define EP_BLOBLOG7			12
#define EP_STRETCH			13

DWORD SendAlgorithmSelection(PROPERTYMANAGER *lpPropertyManager, HWND hwnd);
DWORD InitializeAlgorithmCombo(PROPERTYMANAGER *lpPropertyManager, HWND hwnd);
DWORD UpdateFieldOfViewDimensions(PROPERTYMANAGER *lpPropertyManager, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnKeyDownPropertyManager(PROPERTYMANAGER *lpPropertyManager, HWND hwnd, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK PropertyManagerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PROPERTYMANAGER *lpPropertyManager = (PROPERTYMANAGER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		lpPropertyManager = (PROPERTYMANAGER *)calloc(1, sizeof(PROPERTYMANAGER));
		memset(lpPropertyManager, 0x00, sizeof(PROPERTYMANAGER));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpPropertyManager);
		lpPropertyManager = (PROPERTYMANAGER *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpPropertyManager == NULL){
			ThrowMessage(hwnd, "WndPropertyManager: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndPropertyManager: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpPropertyManager->xFieldOfView = 700;
		lpPropertyManager->yFieldOfView = 575;
		lpPropertyManager->zFieldOfView = 0;
		
		lpPropertyManager->hEditX = GetDlgItem(hwnd, IDC_XFOV);
		lpPropertyManager->hEditY = GetDlgItem(hwnd, IDC_YFOV);
		lpPropertyManager->hEditZ = GetDlgItem(hwnd, IDC_ZFOV);

		lpPropertyManager->hEditLoop = GetDlgItem(hwnd, IDC_EDIT_LOOP);
		lpPropertyManager->hButtonLoop = GetDlgItem(hwnd, IDC_BUTTON_LOOP);

		lpPropertyManager->hAlgorithm = GetDlgItem(hwnd, IDC_COMBO_ALGORITHM);
		
		lpPropertyManager->fps = 1.0;

		InitializeAlgorithmCombo(lpPropertyManager, hwnd);

		//Innitialize AutoPlay Edit Box
		INT success;
		SetDlgItemInt(hwnd, IDC_EDIT_LOOP, lpPropertyManager->fps, TRUE);

		break;
	//Handles all Keyboard input to a control item
	//wParam -- The Virtual Key Pressed by User
	//lParam -- A Pointer to an MSG Structure
	case WM_GETDLGCODE:

		OnKeyDownPropertyManager(lpPropertyManager, hwnd, wParam, lParam);

		break;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case EN_CHANGE:
			return UpdateFieldOfViewDimensions(lpPropertyManager, hwnd, wParam, lParam);
		case CBN_SELCHANGE:
			//INT selection;
			//selection = (INT) ManageAlgorithmSelection(lpPropertyManager, hwnd);
			break;
		default:
			break;
		}
		//Control Identifier
		switch(LOWORD(wParam))
		{
		case IDSET:
			WPARAM wPar;
			//lpPropertyManager->hFocus = (HWND) SendMessage(lpPropertyManager->hOwner, WM_GETHWND, HWND_ACTIVE, NULL);
			SendMessage(lpPropertyManager->hFocus, WM_COMMAND, DVN_SETXFOV, (LPARAM) lpPropertyManager->xFieldOfView);
			SendMessage(lpPropertyManager->hFocus, WM_COMMAND, DVN_SETYFOV, (LPARAM) lpPropertyManager->yFieldOfView);
			SendMessage(lpPropertyManager->hFocus, WM_COMMAND, DVN_SETZFOV, (LPARAM) lpPropertyManager->zFieldOfView);
			SendAlgorithmSelection(lpPropertyManager, hwnd);
			return TRUE;
		case IDC_BUTTON_LOOP:
			SendMessage(lpPropertyManager->hFocus, WM_COMMAND, DVN_LOOP, (LPARAM) (1000/lpPropertyManager->fps));
			return TRUE;
		default:
			break;
		}

		break;
	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpPropertyManager->hOwner = (HWND) lParam;
		if(wParam == HWND_ACTIVE)
			lpPropertyManager->hFocus = (HWND) lParam;

		return TRUE;
	case WM_CLOSE:

		SendMessage(lpPropertyManager->hOwner, WM_COMMAND, LOWORD(ID_VIEW_PROPERTYMANAGER), NULL);
		//ShowWindow(hwnd, SW_HIDE);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD UpdateFieldOfViewDimensions(PROPERTYMANAGER *lpPropertyManager, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	DWORD dw = NULL;
	BOOL status = NULL;
	HWND hEdit = (HWND) lParam;
	CHAR str[MAX_STRING] = {0};

	dw = (DWORD) GetDlgItemInt(hwnd, LOWORD(wParam), &status, TRUE);

	switch(LOWORD(wParam))
	{
	case IDC_XFOV:
		lpPropertyManager->xFieldOfView = dw;
		return TRUE;
	case IDC_YFOV:
		lpPropertyManager->yFieldOfView = dw;
		return TRUE;
	case IDC_ZFOV:
		lpPropertyManager->zFieldOfView = dw;
		return TRUE;
	case IDC_EDIT_LOOP:
		lpPropertyManager->fps = dw;
		return TRUE;
	default:
		return FALSE;
	}

	return dw;
}

DWORD InitializeAlgorithmCombo(PROPERTYMANAGER *lpPropertyManager, HWND hwnd)
{
	DWORD dw = NULL;

	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_NONE, (LPARAM) "NONE");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_PARTICLETRACK, (LPARAM) "ParticleTrack");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_PARTICLETRACK, (LPARAM) "ImageSubtract");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_PARTICLETRACK, (LPARAM) "GradientSubtract");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_PARTICLETRACK, (LPARAM) "LogOperator(1x)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_PARTICLETRACK, (LPARAM) "LogOperator(2x)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_PARTICLETRACK, (LPARAM) "LogOperator(3x)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_GRADIENTPLOW,	(LPARAM) "GradientPlow");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_GRADIENTPLOW,	(LPARAM) "GradientPlow(-)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_GRADIENTPLOW,	(LPARAM) "BlobDetect(LoG-1)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_GRADIENTPLOW,	(LPARAM) "BlobDetect(LoG-3)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_GRADIENTPLOW,	(LPARAM) "BlobDetect(LoG-5)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_GRADIENTPLOW,	(LPARAM) "BlobDetect(LoG-7)");
	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_ADDSTRING, EP_STRETCH,	(LPARAM) "Stretch(0, 255)");

	dw = SendMessage(lpPropertyManager->hAlgorithm, CB_SETCURSEL, (WPARAM) 2, NULL); 

	CHAR str[MAX_STRING];
	//Initialize x
	sprintf(str, "%d", lpPropertyManager->xFieldOfView);
	Edit_SetText(lpPropertyManager->hEditX, str);

	//Initialize y
	sprintf(str, "%d", lpPropertyManager->yFieldOfView);
	Edit_SetText(lpPropertyManager->hEditY, str);

	//Initialize z
	sprintf(str, "%d", lpPropertyManager->zFieldOfView);
	Edit_SetText(lpPropertyManager->hEditZ, str);


	return dw;
}

DWORD SendAlgorithmSelection(PROPERTYMANAGER *lpPropertyManager, HWND hwnd)
{
	DWORD dw = NULL;
	WPARAM wParam = NULL;

	wParam = LOWORD(DVN_SETALGORITHM);

	lpPropertyManager->selAlgorithm = SendMessage(lpPropertyManager->hAlgorithm, CB_GETCURSEL, NULL, NULL);

	switch(lpPropertyManager->selAlgorithm)
	{
	case EP_PARTICLETRACK:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_PARTICLETRACK);
		break;
	case EP_IMAGESUBTRACT:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_IMAGESUBTRACT);
		break;
	case EP_GRADIENTSUBTRACT:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_GRADIENTSUBTRACT);
		break;
	case EP_LOGOPERATOR1X:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_LOG1X);
		break;
	case EP_LOGOPERATOR2X:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_LOG2X);
		break;
	case EP_LOGOPERATOR3X:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_LOG3X);
		break;
	case EP_GRADIENTPLOW:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_GRADIENTPLOW);
		break;
	case EP_GRADIENTPLOWINV:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_GRADIENTPLOWINV);
		break;
	case EP_BLOBLOG1:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_BLOBLOG1);
		break;
	case EP_BLOBLOG3:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_BLOBLOG3);
		break;
	case EP_BLOBLOG5:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_BLOBLOG5);
		break;
	case EP_BLOBLOG7:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_BLOBLOG7);
		break;
	case EP_STRETCH:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_STRETCH);
		break;
	default:
		SendMessage(lpPropertyManager->hFocus, WM_COMMAND, wParam, IDA_NONE);
		break;
	}


	SendMessage(lpPropertyManager->hFocus, WM_COMMAND, (WPARAM) LOWORD(DVN_UPDATE), NULL);
	dw = (DWORD) lpPropertyManager->selAlgorithm;

	return dw;
}

LRESULT OnKeyDownPropertyManager(PROPERTYMANAGER *lpPropertyManager, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	switch(wParam)
	{
	case VK_RETURN:
		SendMessage(hwnd, WM_COMMAND, (WPARAM) IDSET, (LPARAM) NULL);
		return TRUE;
	default:
		break;
	}
	
	return lResult;

}