#include "stdafx.h"


#include "BitFlow.h"
#include "PhotonFocus.h"


#include "PixeLINKApi.h"
#include "PixeLINK.h"
#include "CameraProperties.h"
#include "GetFolderPath.h"
#include "ZeissCam.h"
#include "EmulateCamera.h"
#include "WndLink.h"


INT_PTR CALLBACK CameraPropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpCamProp = (CAMERAPROPERTIES *) InitWindowLongRACE(hwnd, sizeof(CAMERAPROPERTIES));
		

		if(lParam){
			HWND hOwner;
			PROPSHEETPAGE psp;
			memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
			hOwner = (HWND) psp.lParam;
			lpCamProp->hOwner = hOwner;
			PostMessage(hwnd, WM_SETHWND, HWND_OWNER, (LPARAM) hOwner);
		}
		
		InitializeCameraProperties(hwnd, lpCamProp);

		break;
	case WM_GETDLGCODE:
		//OnKeyDownPropertyManager(lpPropertyManager, hwnd, wParam, lParam);
		break;
	case WM_CREATELINK:
		AddWndLink(&lpCamProp->cs.wls, (HWND) wParam);
		return TRUE;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			OnCameraPropertiesButtonClicked(hwnd, wParam, lParam);
			break;
		case EN_CHANGE:
			OnCameraPropertiesEditChanged(hwnd, wParam, lParam);
			break;
		case CBN_SELCHANGE:
			INT sel;
			CHAR strCamera[MAX_STRING];

			sel = ComboBox_GetCurSel(lpCamProp->hComboCamera);
			ComboBox_GetLBText(lpCamProp->hComboCamera, sel, strCamera);

			SendMessage(hwnd, CPM_CAMCLOSE, NULL, NULL);

			if(!strcmp(strCamera, STR_ZEISS_HSM))
				lpCamProp->camera = CAMERA_ZEISS_HSM;	
			else if(!strcmp(strCamera, STR_ZEISS_MRM))
				lpCamProp->camera = CAMERA_ZEISS_MRM;
			else if(!strcmp(strCamera, STR_PHOTONFOCUS))
				lpCamProp->camera = CAMERA_PHOTONFOCUS;
			else if(!strcmp(strCamera, STR_PIXELINK_A781))
				lpCamProp->camera = CAMERA_PIXELINK;
			else if(!strcmp(strCamera, STR_EMULATE_CAM))
				lpCamProp->camera = CAMERA_EMULATE;
			else if(!strcmp(strCamera, STR_DESKTOP))
				lpCamProp->camera = CAMERA_DESKTOP;
			else
				lpCamProp->camera = NULL;

			SendMessage(hwnd, CPM_CAMINIT, NULL, NULL);
			break;
		default:
			break;
		}
		//Control Identifier
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_STREAM:
			
			return TRUE;
		default:
			break;
		}
		break;
	case WM_HSCROLL:
		
		lpCamProp->exposure = SendMessage(lpCamProp->hSlider, TBM_GETPOS, NULL, NULL);

		SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, lpCamProp->exposure, TRUE);

		if(lpCamProp->camera == CAMERA_ZEISS_HSM){
			INT result;
			result = SetExposure(&lpCamProp->zeissProc, lpCamProp->exposure);
		}

		if(lpCamProp->camera == CAMERA_ZEISS_MRM){
			INT result;
			result = SetExposure(&lpCamProp->zeissProc, lpCamProp->exposure);
		}
		break;
	case WM_SETHWND:
		OnCameraPropertiesSetHWND(hwnd, wParam, lParam);
		return TRUE;
	case WM_GETHWND:
		OnCameraPropertiesGetHWND(hwnd, wParam, lParam);
		return TRUE;
	case CPM_GETEXPOSURE:
		if(wParam == CAMERA_ZEISS_MRM){
			DWORD exposure = NULL;
			exposure = (DWORD) GetExposure(&lpCamProp->zeissProc);
			return DialogReturn(hwnd, exposure);
			//return (LRESULT) exposure;
		}
		break;
	case CPM_SETEXPOSURE:
		if(wParam == CAMERA_ZEISS_MRM){
			INT result;
			lpCamProp->cs.flagExposure = TRUE;
			lpCamProp->cs.exposure = (INT) lParam;
		}
		break;
	case CPM_CAMINIT:
		OnCameraPropertiesInitializeCamera(hwnd, wParam, lParam); 
//		switch(lpCamProp->camera)
//		{
//		case NULL:
//			{
//				SendMessage(hwnd, CPM_SETWIDTH, NULL, NULL);
//				SendMessage(hwnd, CPM_SETHEIGHT, NULL, NULL);
//			}
//			break;
//		case CAMERA_DESKTOP:
//			{
//				LONG w;
//				LONG h;
//
//			}
//			return TRUE;
//		case CAMERA_ZEISS_HSM:
//			{
//				INT i;
//				INT nCam;
//				HMODULE dllZeiss;
//				ZEISSFRAMEINFO zfm;
//				INT width;
//				INT height;
//				dllZeiss = LoadZeissLibrary(hwnd, ZEISS_HSM);
//				ImportZeissFunctions(dllZeiss, &lpCamProp->zeissProcHSm);
//				nCam = GetNumCamera(&lpCamProp->zeissProcHSm);
//				i = InitCamera(&lpCamProp->zeissProcHSm);
//				GetCurrentImageSize(&lpCamProp->zeissProcHSm, &width, &height);
//				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) width, NULL);
//				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) height, NULL);
//				Button_Enable(lpCamProp->hButtonStream, TRUE);
//				lpCamProp->cs.lpVoid = &lpCamProp->zeissProcHSm;
//			}
//			return TRUE;
//		case CAMERA_ZEISS_MRM:
//			{
//				HMODULE dllZeiss;
//				ZEISSFRAMEINFO zfm;
//				INT width;
//				INT height;
//				dllZeiss = LoadZeissLibrary(hwnd, ZEISS_MRM);
//				ImportZeissFunctions(dllZeiss, &lpCamProp->zeissProc);
//				InitCamera(&lpCamProp->zeissProc);
//				GetCurrentImageSize(&lpCamProp->zeissProc, &width, &height);
//				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) width, NULL);
//				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) height, NULL);
//				Button_Enable(lpCamProp->hButtonStream, TRUE);
//				lpCamProp->cs.lpVoid = &lpCamProp->zeissProc;
//				//CloseCamera(&lpCamProp->zeissProc);
//
//				//if(dllZeiss)
//				//	FreeLibrary(dllZeiss);
//			}
//			return TRUE;
//		case CAMERA_PIXELINK:
//			{
//				HANDLE hCamera;
//				PXL_RETURN_CODE pxlReturn;
//				pxlReturn = PxLInitialize(NULL/*7810391*/, &lpCamProp->hCamera);
//				if(!pxlReturn){
//					U32 sizeCamFeatures;
//					CAMERA_FEATURES *camFeatures;
//					ThrowMessage(lpCamProp->hOwner, "Connected to PixeLINK-A781");
//					PxLGetCameraFeatures(lpCamProp->hCamera, FEATURE_ALL, NULL, &sizeCamFeatures);
//					camFeatures = (CAMERA_FEATURES *) calloc(1, sizeCamFeatures);
//					PxLGetCameraFeatures(lpCamProp->hCamera, FEATURE_ALL, camFeatures, &sizeCamFeatures);
//					sizeCamFeatures = sizeCamFeatures;
//					SendMessage(hwnd, CPM_SETWIDTH, (WPARAM)2208, NULL);
//					SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM)3000, NULL);
//					PxLUninitialize(lpCamProp->hCamera);
//					Button_Enable(lpCamProp->hButtonStream, TRUE);
//				}
//				else{
//					ThrowMessage(lpCamProp->hOwner, "Failed to Connect to PixeLINK-A781");
//				}
//			}
//			return TRUE;
//		case CAMERA_PHOTONFOCUS:
//			{
//#ifdef RACE_BITFLOW
//
//				BITFLOW bf;
//				InitPhotonFocusCam(&bf);
//				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) bf.wFrame, NULL);
//				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) bf.hFrame, NULL);
//				CloseBitFlowBoard(&bf);
//				Button_Enable(lpCamProp->hButtonStream, TRUE);
//#endif
//			}
//			return TRUE;
//		case CAMERA_EMULATE:
//			{
//				Button_Enable(lpCamProp->hButtonStream, TRUE);
//			}
//			return TRUE;
//		default:
//			return TRUE;
//		}

		return TRUE;
	case CPM_SETWIDTH:
		CHAR strDim[MAX_STRING];
		sprintf(strDim, "%d", wParam);
		Edit_SetText(lpCamProp->hEditWidth, strDim);
		return TRUE;
	case CPM_SETHEIGHT:
		sprintf(strDim, "%d", wParam);
		Edit_SetText(lpCamProp->hEditHeight, strDim);
		return TRUE;
	case CPM_CAMCLOSE:
		//NEW
		lpCamProp->cs.flagSave = FALSE;
		lpCamProp->cs.flagStream = FALSE;
		lpCamProp->cs.flagOpen = FALSE;
		//NEW
		switch(lpCamProp->camera)
		{
		case CAMERA_ZEISS_HSM:
			//lpCamProp->zeissProc.CloseCamera();
			//ThrowMessage(lpCamProp->hOwner, "Disconnected from Zeiss HSm");
			//lpCamProp->cs.flagClose = TRUE;
			return TRUE;
		case CAMERA_PIXELINK:
			PxLUninitialize(lpCamProp->hCamera);
			ThrowMessage(lpCamProp->hOwner, "Disconnected from PixeLINK-A781");
			return TRUE;
		case CAMERA_PHOTONFOCUS:
			ThrowMessage(lpCamProp->hOwner, "Disconnected from PhotonFocus");
			return TRUE;
		case CAMERA_EMULATE:
			ThrowMessage(lpCamProp->hOwner, "Disconnected from Emulate Camera");
			lpCamProp->flagStream = FALSE;
			Button_Enable(lpCamProp->hButtonSave, FALSE);
			SetWindowText(lpCamProp->hButtonStream, "Stream");
			lpCamProp->flagSave = FALSE;
			return TRUE;
		default:
			return TRUE;
		}
		return TRUE;
	case CPM_CAPTURE:
		if(!lpCamProp->flagStream){
			lpCamProp->cs.flagStream = FALSE;
			lpCamProp->cs.flagOpen = FALSE;
			lpCamProp->hDataManager = NULL;
			Sleep(400);
		}
		else
		{
			if(!lpCamProp->hDataManager){
				lpCamProp->cs.flagStream = FALSE;
				lpCamProp->cs.flagOpen = FALSE;
				SendMessage(lpCamProp->hOwner, WM_COMMAND, LOWORD(ID_OPEN_STREAM), (LPARAM) hwnd);
				Sleep(100);
			}
			else{
				lpCamProp->cs.hDataManager = lpCamProp->hDataManager;
				lpCamProp->cs.hOwner = hwnd;
				lpCamProp->cs.flagStream = TRUE;
				lpCamProp->cs.flagSave = FALSE;
				lpCamProp->cs.flagClose = FALSE;
				lpCamProp->cs.flagOpen = TRUE;
			
		
				switch(lpCamProp->camera)
				{
				case CAMERA_ZEISS_HSM:
					CreateThread(NULL, NULL, ZeissHSmCapture, &lpCamProp->cs, NULL, NULL);
					return TRUE;
				case CAMERA_ZEISS_MRM:
					CreateThread(NULL, NULL, ZeissMRmCapture, &lpCamProp->cs, NULL, NULL);
					return TRUE;
				case CAMERA_PIXELINK:
					CreateThread(NULL, NULL, PixeLINKThread, &lpCamProp->cs, NULL, NULL);
					return TRUE;
				case CAMERA_PHOTONFOCUS:
					HANDLE hThread;
					hThread = CreateThread(NULL, NULL, PhotonFocusThread, &lpCamProp->cs, NULL, NULL);
					SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
					return TRUE;
				case CAMERA_EMULATE:
					CreateThread(NULL, NULL, EmulateCaptureStream, &lpCamProp->cs, NULL, NULL);
					return TRUE;
				default:
					return TRUE;
				}
			}
		}
		return TRUE;
	case WM_CLOSING:
		{
			HWND hClosing;
			hClosing = (HWND) lParam;

			GetWindowText(hClosing, str, MAX_STRING);

			if(MessageBox(NULL, "Stop Stream?", "Camera Properties", MB_OKCANCEL|MB_ICONQUESTION) == IDOK){
				SendMessage(hwnd, CPM_CAMCLOSE, NULL, NULL);
				return DialogReturn(hwnd, TRUE);
			}else{
				return DialogReturn(hwnd, FALSE);
			}
		}
		return TRUE;
	case WM_CLOSE:

		
		SendMessage(lpCamProp->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_CAMERAPROPERTIES), NULL);

		if(lpCamProp->camera == CAMERA_ZEISS_HSM)
			lpCamProp->zeissProc.CloseCamera();

		return TRUE;
	case WM_DESTROY:

		SendMessage(hwnd, CPM_CAMCLOSE, NULL, NULL);
		PostQuitMessage(0);
		return FALSE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD InitializeCameraProperties(HWND hwnd, CAMERAPROPERTIES *lpCamProp)
{

	CHAR str[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(lpCamProp->strPath, NULL, MAX_STRING*sizeof(CHAR));

	lpCamProp->hButtonPath = GetDlgItem(hwnd, IDC_BTN_PATH);
	lpCamProp->hButtonStream = GetDlgItem(hwnd, IDC_BUTTON_STREAM);
	lpCamProp->hButtonSave = GetDlgItem(hwnd, IDC_BUTTON_SAVE);
	lpCamProp->hButtonSnap = GetDlgItem(hwnd, IDC_BUTTON_SNAP);
	lpCamProp->hButtonAutoExposure = GetDlgItem(hwnd, IDC_BTN_AUTOEXPOSURE);
	lpCamProp->hBtnStop = GetDlgItem(hwnd, IDC_BTN_STOP);

	lpCamProp->hComboCamera = GetDlgItem(hwnd, IDC_COMBO_CAMERA);

	lpCamProp->hEditPath = GetDlgItem(hwnd, IDC_EDIT_PATH);
	lpCamProp->hEditExposure = GetDlgItem(hwnd, IDC_EDIT_EXPOSURE);
	lpCamProp->hEditHeight = GetDlgItem(hwnd, IDC_EDIT_HEIGHT);
	lpCamProp->hEditWidth = GetDlgItem(hwnd, IDC_EDIT_WIDTH);
	lpCamProp->hEditFrameRate = GetDlgItem(hwnd, IDC_EDIT_FRAMERATE);
	lpCamProp->hEditSampleRate = GetDlgItem(hwnd, IDC_EDIT_SAMPLERATE);

	lpCamProp->hSlider = GetDlgItem(hwnd, IDC_SLIDER_EXPOSURE);
	SendMessage(lpCamProp->hSlider, TBM_SETRANGEMIN, TRUE, 0);
	SendMessage(lpCamProp->hSlider, TBM_SETRANGEMAX, TRUE, 500000);
	
	Button_Enable(lpCamProp->hButtonStream, FALSE);
	Button_Enable(lpCamProp->hButtonSave, FALSE);
	Button_Enable(lpCamProp->hButtonSnap, TRUE);
	Button_Enable(lpCamProp->hBtnStop, FALSE);

	lpCamProp->hRadioBit2 = GetDlgItem(hwnd, IDC_RADIO_BIT2);
	lpCamProp->hRadioBit4 = GetDlgItem(hwnd, IDC_RADIO_BIT4);
	lpCamProp->hRadioBit8 = GetDlgItem(hwnd, IDC_RADIO_BIT8);
	lpCamProp->hRadioBit12 = GetDlgItem(hwnd, IDC_RADIO_BIT12);

	lpCamProp->hRadioBin1 = GetDlgItem(hwnd, IDC_RADIO_BIN1);
	lpCamProp->hRadioBin2 = GetDlgItem(hwnd, IDC_RADIO_BIN2);
	lpCamProp->hRadioBin3 = GetDlgItem(hwnd, IDC_RADIO_BIN3);
	lpCamProp->hRadioBin4 = GetDlgItem(hwnd, IDC_RADIO_BIN4);

	lpCamProp->hRadioMonochrome = GetDlgItem(hwnd, IDC_RADIO_MONOCHROME);
	lpCamProp->hRadioRGB = GetDlgItem(hwnd, IDC_RADIO_RGB);
	lpCamProp->hRadioCMYK = GetDlgItem(hwnd, IDC_RADIO_CMYK);

	lpCamProp->hCheckFrameRate = GetDlgItem(hwnd, IDC_CHECK_FRAMERATE);

	lpCamProp->exposure = 15000;
	SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, lpCamProp->exposure, TRUE);
	SendMessage(lpCamProp->hSlider, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) lpCamProp->exposure);

	Button_SetCheck(lpCamProp->hRadioMonochrome, BST_CHECKED);
	Button_SetCheck(lpCamProp->hRadioBit8, BST_CHECKED);
	Button_SetCheck(lpCamProp->hRadioBin1, BST_CHECKED);

	Edit_SetText(lpCamProp->hEditHeight, "N/A");
	Edit_SetText(lpCamProp->hEditWidth, "N/A");

	lpCamProp->nBit = 8;
	lpCamProp->nBin = 1;

	lpCamProp->frameRate = 30.0;
	lpCamProp->prevFrameRate = lpCamProp->frameRate;
	lpCamProp->cs.frameRate = lpCamProp->frameRate;
	lpCamProp->flagPause = TRUE;
	sprintf(str, "%.1f", lpCamProp->frameRate);
	Edit_SetText(lpCamProp->hEditFrameRate, str);

	lpCamProp->cs.sampleRate = 1;
	sprintf(str, "%d", lpCamProp->cs.sampleRate);
	Edit_SetText(lpCamProp->hEditSampleRate, str);

	ComboBox_AddString(lpCamProp->hComboCamera, STR_NOSELECTION);
	ComboBox_AddString(lpCamProp->hComboCamera, STR_EMULATE_CAM);

	INT error;
	INT numCam;
	HMODULE dllZeissHSm;// = NULL;
	dllZeissHSm = LoadZeissLibrary(hwnd, ZEISS_HSM);
	ImportZeissFunctions(dllZeissHSm, &lpCamProp->zeissProcHSm);
	error = InitCamera(&lpCamProp->zeissProcHSm);

	numCam = GetNumCamera(&lpCamProp->zeissProcHSm);
	if(!error)
		ComboBox_AddString(lpCamProp->hComboCamera, STR_ZEISS_HSM);

	CloseCamera(&lpCamProp->zeissProcHSm);

	if(dllZeissHSm)
		FreeLibrary(dllZeissHSm);

	
	HMODULE dllZeissMRm;
	dllZeissMRm = LoadZeissLibrary(hwnd, ZEISS_MRM);
	ImportZeissFunctions(dllZeissMRm, &lpCamProp->zeissProc);
	error = InitCamera(&lpCamProp->zeissProc);
	
	numCam = GetNumCamera(&lpCamProp->zeissProc);
	if(!error)
	//if(GetNumCamera(&lpCamProp->zeissProc))
		ComboBox_AddString(lpCamProp->hComboCamera, STR_ZEISS_MRM);

	CloseCamera(&lpCamProp->zeissProc);

	if(dllZeissMRm)
		FreeLibrary(dllZeissMRm);


	U32 nPixeLink;
	PxLGetNumberCameras(NULL, &nPixeLink);
	if(nPixeLink)
		ComboBox_AddString(lpCamProp->hComboCamera, STR_PIXELINK_A781);

	ComboBox_SetCurSel(lpCamProp->hComboCamera, 1);

	lpCamProp->camera = NULL;
	SendMessage(hwnd, CPM_CAMINIT, NULL, NULL);

#ifdef RACE_BITFLOW
//#ifdef _WIN64
		BITFLOW bf;
		if(InitPhotonFocusCam(&bf)){
			ComboBox_AddString(lpCamProp->hComboCamera, STR_PHOTONFOCUS);
			CloseBitFlowBoard(&bf);
		}
#endif

	lpCamProp->cs.hRACE = lpCamProp->hOwner;
	AllocWndLink(&lpCamProp->cs.wls, 5);

	SetHWND(lpCamProp->hOwner, hwnd, HWND_CAMERAPROPERTIES);

	return TRUE;
}

LRESULT OnCameraPropertiesButtonClicked(/*CAMERAPROPERTIES *lpCamProp, */HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowUserData(hwnd);
	
	HWND hActive = GetHWND(lpCamProp->hOwner, HWND_ACTIVE);

	switch(LOWORD(wParam))
	{
	case IDC_BUTTON_STREAM:
		{
			if(LOWORD(wParam) == IDC_BUTTON_STREAM){
				//Begin Sream using specified camera parameters
				//Active IDC_BUTTON_SAVE
				lpCamProp->capture.flgStream = TRUE;
				lpCamProp->flagStream = TRUE;
				
				lpCamProp->capture.flgPause = !lpCamProp->capture.flgPause;
				lpCamProp->flagPause = !lpCamProp->flagPause;
				if(!lpCamProp->cs.flagPause);
				//if(!lpCamProp->flagPause){
					
					Button_Enable(lpCamProp->hButtonSave, TRUE);
					SetWindowText(lpCamProp->hButtonStream, "Pause");
					lpCamProp->cs.frameRate = lpCamProp->prevFrameRate;
				}
				else{
					//Pause
					SetWindowText(lpCamProp->hButtonStream, "Start");
					lpCamProp->prevFrameRate = lpCamProp->cs.frameRate;
					lpCamProp->cs.frameRate = NULL;
					
				}

				if(!lpCamProp->cs.flagOpen)
				{
					SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
				}
				lpCamProp->cs.flagOpen = TRUE;
			}
		}
		return TRUE;
	case IDC_BTN_STOP:
		{
			Button_Disable(lpCamProp->hButtonSave);
			SetWindowText(lpCamProp->hButtonStream, "Start");
			SendMessage(hwnd, CPM_CAMCLOSE, NULL, NULL);
			//lpCamProp->cs.flagSave = FALSE;
			//lpCamProp->cs.flagStream = FALSE;
			//lpCamProp->cs.flagOpen = FALSE;
			
		}
		return TRUE;
	case IDC_BTN_SAVE:
		{
			lpCamProp->flagSave = !lpCamProp->flagSave;
			if(lpCamProp->flagSave){
				SendMessage(lpCamProp->hDataManager, WM_SETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
				lpCamProp->cs.flagSave = TRUE;
				SetWindowText(lpCamProp->hButtonSave, "Stop Save");
				Button_Enable(lpCamProp->hButtonPath, FALSE);
			}
			else{
				lpCamProp->cs.flagSave = FALSE;
				SetWindowText(lpCamProp->hButtonSave, "Save");
				Button_Enable(lpCamProp->hButtonPath, TRUE);
				SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_STREAM, BN_CLICKED), NULL);
			}

			SendMessage(lpCamProp->hDataManager, WM_SETARCHIVE, lpCamProp->flagSave, NULL);
				
		}
		return TRUE;
	case IDC_BTN_PATH:
		{
			CHAR str[MAX_STRING];
			OPENFILENAME ofn;
			memset(&ofn, NULL, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = (CHAR *) str;
			ofn.hwndOwner = hwnd;
			ofn.nMaxFile = MAX_STRING;
			ofn.Flags = OFN_EXPLORER;
			
			if(GetFolderPath(hwnd, str)){
				memcpy(lpCamProp->strPath, str, MAX_STRING*sizeof(CHAR));
				Edit_SetText(lpCamProp->hEditPath, lpCamProp->strPath);
			}

		}
		return TRUE;
	case IDC_BTN_SNAP:
		{
			HWND hActive;
			hActive = (HWND) SendMessage(lpCamProp->hOwner, WM_GETHWND, (WPARAM) HWND_ACTIVE, NULL);
			DataView_SnapFrame(hActive);
			ShowWindow(hActive, SW_SHOW);
		}
		return TRUE;
	default:
		return FALSE;
	}

	return FALSE;
}

LRESULT OnCameraPropertiesEditChanged(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowUserData(hwnd);
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	switch(LOWORD(wParam))
	{
	case IDC_EDIT_PATH:
		{

			Edit_GetText(lpCamProp->hEditPath, lpCamProp->strPath, MAX_STRING);

		}return TRUE;
	case IDC_EDIT_EXPOSURE:
		{
			lpCamProp->exposure = GetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, NULL, FALSE);
			SendMessage(lpCamProp->hSlider, 
						TBM_SETPOS, 
						(WPARAM) TRUE, 
						(LPARAM) lpCamProp->exposure);
		}return TRUE;
	case IDC_EDIT_FRAMERATE:
		{
			Edit_GetText(lpCamProp->hEditFrameRate, str, MAX_STRING);
			sscanf(str, "%f", &lpCamProp->prevFrameRate);

			if(!lpCamProp->flagPause){
				lpCamProp->cs.frameRate = lpCamProp->prevFrameRate;
			}

			if(!str[0]){
				lpCamProp->cs.frameRate = NULL;
			}
		}return TRUE;
	case IDC_EDIT_SAMPLERATE:
		{
				Edit_GetText(lpCamProp->hEditSampleRate, str, MAX_STRING);
				sscanf(str, "%d", &lpCamProp->cs.sampleRate);
				if(!str[0]){
					lpCamProp->cs.sampleRate = 1;
				}
		}return TRUE;
	default:
		return FALSE;
	}

			return FALSE;
}

LRESULT OnCameraPropertiesSetHWND(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *)GetWindowUserData(hwnd);

	switch(wParam)
	{
	case HWND_OWNER:
		{//
			//HWND hwnd = (HWND) lParam;
			//hwnd = GetAncestor(hwnd, GA_ROOT);
			//lpCamProp->hOwner = (HWND) hwnd;
			lpCamProp->hOwner = (HWND) lParam;
			lpCamProp->hFrame =  GetHWND(lpCamProp->hOwner, HWND_FRAME);
			SendMessage(lpCamProp->hFrame, WM_GETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
			Edit_SetText(lpCamProp->hEditPath, lpCamProp->strPath);	
			SendMessage(lpCamProp->hOwner, WM_SETHWND, (WPARAM) HWND_CAMERAPROPERTIES, (LPARAM) hwnd);
		}return TRUE;
	case HWND_ACTIVE:
		{
			lpCamProp->hFocus = (HWND) lParam;
			DWORD i;
			if(!Find(&lpCamProp->cs, lpCamProp->hFocus, &i)){
				lpCamProp->cActive = NULL;
				return FALSE;
			}

			lpCamProp->cActive = &lpCamProp->cs.c[i];

			OnUpdateActiveCameraProperties(lpCamProp);

		}return TRUE;
	case HWND_DATAMANAGER:
		{
			lpCamProp->hDataManager = (HWND) lParam;
			SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
		}return TRUE;
	default:
		return TRUE;
	}

		return TRUE;
}


LRESULT OnCameraPropertiesGetHWND(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *)GetWindowUserData(hwnd);

	switch(wParam)
	{
	case HWND_STAGECONTROLS:
		{
			HWND hStageControls;
			hStageControls = (HWND) SendMessage(lpCamProp->hOwner, WM_GETHWND, HWND_STAGECONTROLS, NULL);
			SetWindowLong(hwnd, NULL/*DWL_MSGRESULT*/, (LONG)hStageControls);
			return (INT_PTR)hStageControls;
		}
	case HWND_FRAME:
		return DialogReturn(hwnd, (LRESULT)lpCamProp->hOwner);
	case HWND_OWNER:
		return DialogReturn(hwnd, (LRESULT)lpCamProp->hOwner);
	case HWND_DATAMANAGER:
		return DialogReturn(hwnd, (LRESULT)lpCamProp->hDataManager);
	//case HWND_ACTIVE:
		//return DialogReturn(hwnd, (LRESULT)lpCamProp->hActive);
	default:
		return TRUE;
	}
	

	return TRUE;
}

LRESULT OnCameraPropertiesInitializeCamera(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *)GetWindowUserData(hwnd);

		switch(lpCamProp->camera)
		{
		case NULL:
			{
				SendMessage(hwnd, CPM_SETWIDTH, NULL, NULL);
				SendMessage(hwnd, CPM_SETHEIGHT, NULL, NULL);
			}
			break;
		case CAMERA_DESKTOP:
			{
				LONG w;
				LONG h;

			}
			return TRUE;
		case CAMERA_ZEISS_HSM:
			{
				INT i;
				INT nCam;
				HMODULE dllZeiss;
				ZEISSFRAMEINFO zfm;
				INT width;
				INT height;
				dllZeiss = LoadZeissLibrary(hwnd, ZEISS_HSM);
				ImportZeissFunctions(dllZeiss, &lpCamProp->zeissProcHSm);
				nCam = GetNumCamera(&lpCamProp->zeissProcHSm);
				i = InitCamera(&lpCamProp->zeissProcHSm);
				GetCurrentImageSize(&lpCamProp->zeissProcHSm, &width, &height);
				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) width, NULL);
				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) height, NULL);
				Button_Enable(lpCamProp->hButtonStream, TRUE);
				lpCamProp->cs.lpVoid = &lpCamProp->zeissProcHSm;
			}
			return TRUE;
		case CAMERA_ZEISS_MRM:
			{
				HMODULE dllZeiss;
				ZEISSFRAMEINFO zfm;
				INT width;
				INT height;
				dllZeiss = LoadZeissLibrary(hwnd, ZEISS_MRM);
				ImportZeissFunctions(dllZeiss, &lpCamProp->zeissProc);
				InitCamera(&lpCamProp->zeissProc);
				GetCurrentImageSize(&lpCamProp->zeissProc, &width, &height);
				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) width, NULL);
				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) height, NULL);
				Button_Enable(lpCamProp->hButtonStream, TRUE);
				lpCamProp->cs.lpVoid = &lpCamProp->zeissProc;
				//CloseCamera(&lpCamProp->zeissProc);

				//if(dllZeiss)
				//	FreeLibrary(dllZeiss);
			}
			return TRUE;
		case CAMERA_PIXELINK:
			{
				HANDLE hCamera;
				PXL_RETURN_CODE pxlReturn;
				pxlReturn = PxLInitialize(NULL/*7810391*/, &lpCamProp->hCamera);
				if(!pxlReturn){
					U32 sizeCamFeatures;
					CAMERA_FEATURES *camFeatures;
					ThrowMessage(lpCamProp->hOwner, "Connected to PixeLINK-A781");
					PxLGetCameraFeatures(lpCamProp->hCamera, FEATURE_ALL, NULL, &sizeCamFeatures);
					camFeatures = (CAMERA_FEATURES *) calloc(1, sizeCamFeatures);
					PxLGetCameraFeatures(lpCamProp->hCamera, FEATURE_ALL, camFeatures, &sizeCamFeatures);
					sizeCamFeatures = sizeCamFeatures;
					SendMessage(hwnd, CPM_SETWIDTH, (WPARAM)2208, NULL);
					SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM)3000, NULL);
					PxLUninitialize(lpCamProp->hCamera);
					Button_Enable(lpCamProp->hButtonStream, TRUE);
				}
				else{
					ThrowMessage(lpCamProp->hOwner, "Failed to Connect to PixeLINK-A781");
				}
			}
			return TRUE;
		case CAMERA_PHOTONFOCUS:
			{
#ifdef RACE_BITFLOW

				BITFLOW bf;
				InitPhotonFocusCam(&bf);
				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) bf.wFrame, NULL);
				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) bf.hFrame, NULL);
				CloseBitFlowBoard(&bf);
				Button_Enable(lpCamProp->hButtonStream, TRUE);
#endif
			}
			return TRUE;
		case CAMERA_EMULATE:
			{
				Button_Enable(lpCamProp->hButtonStream, TRUE);
			}
			return TRUE;
		default:
			return TRUE;
		}
}


///////////////////////
//WORKS -Supressed 7.2.14
//Supressed to design multiple stream control
///////////////////////
//INT_PTR CALLBACK CameraPropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
//
//	CHAR str[MAX_STRING];
//
//	switch(msg)
//	{
//	case WM_INITDIALOG:
//
//		lpCamProp = (CAMERAPROPERTIES *)calloc(1, sizeof(CAMERAPROPERTIES));
//		memset(lpCamProp, 0x00, sizeof(CAMERAPROPERTIES));
//		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpCamProp);
//		lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
//
//		if(lpCamProp == NULL){
//			ThrowMessage(hwnd, "WndCameraProperties: Error Getting Long Ptr");
//			ThrowMessage(hwnd, "WndCameraProperties: Exiting Initialization");
//			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
//			return TRUE;
//		}
//
//		memset(lpCamProp->strPath, NULL, MAX_STRING*sizeof(CHAR));
//
//		lpCamProp->hButtonPath = GetDlgItem(hwnd, IDC_BUTTON_PATH);
//		lpCamProp->hButtonStream = GetDlgItem(hwnd, IDC_BUTTON_STREAM);
//		lpCamProp->hButtonSave = GetDlgItem(hwnd, IDC_BUTTON_SAVE);
//		lpCamProp->hButtonSnap = GetDlgItem(hwnd, IDC_BUTTON_SNAP);
//		lpCamProp->hButtonAutoExposure = GetDlgItem(hwnd, IDC_BTN_AUTOEXPOSURE);
//		lpCamProp->hBtnStop = GetDlgItem(hwnd, IDC_BTN_STOP);
//
//		lpCamProp->hComboCamera = GetDlgItem(hwnd, IDC_COMBO_CAMERA);
//
//		lpCamProp->hEditPath = GetDlgItem(hwnd, IDC_EDIT_PATH);
//		lpCamProp->hEditExposure = GetDlgItem(hwnd, IDC_EDIT_EXPOSURE);
//		lpCamProp->hEditHeight = GetDlgItem(hwnd, IDC_EDIT_HEIGHT);
//		lpCamProp->hEditWidth = GetDlgItem(hwnd, IDC_EDIT_WIDTH);
//		lpCamProp->hEditFrameRate = GetDlgItem(hwnd, IDC_EDIT_FRAMERATE);
//		lpCamProp->hEditSampleRate = GetDlgItem(hwnd, IDC_EDIT_SAMPLERATE);
//
//		lpCamProp->hSlider = GetDlgItem(hwnd, IDC_SLIDER_EXPOSURE);
//		SendMessage(lpCamProp->hSlider, TBM_SETRANGEMIN, TRUE, 0);
//		SendMessage(lpCamProp->hSlider, TBM_SETRANGEMAX, TRUE, 500000);
//		
//		Button_Enable(lpCamProp->hButtonStream, FALSE);
//		Button_Enable(lpCamProp->hButtonSave, FALSE);
//		Button_Enable(lpCamProp->hButtonSnap, FALSE);
//		Button_Enable(lpCamProp->hBtnStop, FALSE);
//
//		lpCamProp->hRadioBit2 = GetDlgItem(hwnd, IDC_RADIO_BIT2);
//		lpCamProp->hRadioBit4 = GetDlgItem(hwnd, IDC_RADIO_BIT4);
//		lpCamProp->hRadioBit8 = GetDlgItem(hwnd, IDC_RADIO_BIT8);
//		lpCamProp->hRadioBit12 = GetDlgItem(hwnd, IDC_RADIO_BIT12);
//
//		lpCamProp->hRadioBin1 = GetDlgItem(hwnd, IDC_RADIO_BIN1);
//		lpCamProp->hRadioBin2 = GetDlgItem(hwnd, IDC_RADIO_BIN2);
//		lpCamProp->hRadioBin3 = GetDlgItem(hwnd, IDC_RADIO_BIN3);
//		lpCamProp->hRadioBin4 = GetDlgItem(hwnd, IDC_RADIO_BIN4);
//
//		lpCamProp->hRadioMonochrome = GetDlgItem(hwnd, IDC_RADIO_MONOCHROME);
//		lpCamProp->hRadioRGB = GetDlgItem(hwnd, IDC_RADIO_RGB);
//		lpCamProp->hRadioCMYK = GetDlgItem(hwnd, IDC_RADIO_CMYK);
//
//		lpCamProp->hCheckFrameRate = GetDlgItem(hwnd, IDC_CHECK_FRAMERATE);
//
//		lpCamProp->exposure = 15000;
//		SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, lpCamProp->exposure, TRUE);
//		SendMessage(lpCamProp->hSlider, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) lpCamProp->exposure);
//
//		Button_SetCheck(lpCamProp->hRadioMonochrome, BST_CHECKED);
//		Button_SetCheck(lpCamProp->hRadioBit8, BST_CHECKED);
//		Button_SetCheck(lpCamProp->hRadioBin1, BST_CHECKED);
//
//		Edit_SetText(lpCamProp->hEditHeight, "N/A");
//		Edit_SetText(lpCamProp->hEditWidth, "N/A");
//
//		lpCamProp->nBit = 8;
//		lpCamProp->nBin = 1;
//
//		lpCamProp->frameRate = 30.0;
//		lpCamProp->prevFrameRate = lpCamProp->frameRate;
//		lpCamProp->cs.frameRate = lpCamProp->frameRate;
//		lpCamProp->flagPause = TRUE;
//		sprintf(str, "%.1f", lpCamProp->frameRate);
//		Edit_SetText(lpCamProp->hEditFrameRate, str);
//
//		lpCamProp->cs.sampleRate = 1;
//		sprintf(str, "%d", lpCamProp->cs.sampleRate);
//		Edit_SetText(lpCamProp->hEditSampleRate, str);
//
//		ComboBox_AddString(lpCamProp->hComboCamera, STR_NOSELECTION);
//		ComboBox_AddString(lpCamProp->hComboCamera, STR_EMULATE_CAM);
//
//		INT error;
//		INT numCam;
//		HMODULE dllZeissHSm;// = NULL;
//		dllZeissHSm = LoadZeissLibrary(hwnd, ZEISS_HSM);
//		ImportZeissFunctions(dllZeissHSm, &lpCamProp->zeissProcHSm);
//		error = InitCamera(&lpCamProp->zeissProcHSm);
//
//		numCam = GetNumCamera(&lpCamProp->zeissProcHSm);
//		if(!error)
//			ComboBox_AddString(lpCamProp->hComboCamera, STR_ZEISS_HSM);
//
//		CloseCamera(&lpCamProp->zeissProcHSm);
//
//		if(dllZeissHSm)
//			FreeLibrary(dllZeissHSm);
//
//		
//		HMODULE dllZeissMRm;
//		dllZeissMRm = LoadZeissLibrary(hwnd, ZEISS_MRM);
//		ImportZeissFunctions(dllZeissMRm, &lpCamProp->zeissProc);
//		error = InitCamera(&lpCamProp->zeissProc);
//		
//		numCam = GetNumCamera(&lpCamProp->zeissProc);
//		if(!error)
//		//if(GetNumCamera(&lpCamProp->zeissProc))
//			ComboBox_AddString(lpCamProp->hComboCamera, STR_ZEISS_MRM);
//
//		CloseCamera(&lpCamProp->zeissProc);
//
//		if(dllZeissMRm)
//			FreeLibrary(dllZeissMRm);
//
//
//		U32 nPixeLink;
//		PxLGetNumberCameras(NULL, &nPixeLink);
//		if(nPixeLink)
//			ComboBox_AddString(lpCamProp->hComboCamera, STR_PIXELINK_A781);
//
//		ComboBox_SetCurSel(lpCamProp->hComboCamera, 1);
//
//		lpCamProp->camera = NULL;
//		SendMessage(hwnd, CPM_CAMINIT, NULL, NULL);
//
//#ifdef RACE_BITFLOW
////#ifdef _WIN64
//		BITFLOW bf;
//		if(InitPhotonFocusCam(&bf)){
//			ComboBox_AddString(lpCamProp->hComboCamera, STR_PHOTONFOCUS);
//			CloseBitFlowBoard(&bf);
//		}
//#endif
//		break;
//	//Handles all Keyboard input to a control item
//	//wParam -- The Virtual Key Pressed by User
//	//lParam -- A Pointer to an MSG Structure
//	case WM_GETDLGCODE:
//
//		//OnKeyDownPropertyManager(lpPropertyManager, hwnd, wParam, lParam);
//
//		break;
//	//case BM_SETCHECK:
//	//	return TRUE;
//	case WM_COMMAND:
//
//		//Control-Defined Notifiation Code
//		switch(HIWORD(wParam))
//		{
//		case BN_CLICKED:
//			
//			if(LOWORD(wParam) == IDC_BUTTON_STREAM){
//				//Begin Sream using specified camera parameters
//				//Active IDC_BUTTON_SAVE
//
//				lpCamProp->flagStream = TRUE;
//				
//				lpCamProp->flagPause = !lpCamProp->flagPause;
//				//lpCamProp->flagStream = !lpCamProp->flagStream;
//				//lpCamProp->cs.flagOpen = !lpCamProp->cs.flagOpen;
//				if(!lpCamProp->flagPause){
//					
//					Button_Enable(lpCamProp->hButtonSave, TRUE);
//					SetWindowText(lpCamProp->hButtonStream, "Pause");
//					lpCamProp->cs.frameRate = lpCamProp->prevFrameRate;
//					//SetWindowText(lpCamProp->hButtonStream, "Stop Stream");
//				}
//				else{
//					//Pause
//					SetWindowText(lpCamProp->hButtonStream, "Start");
//					lpCamProp->prevFrameRate = lpCamProp->cs.frameRate;
//					lpCamProp->cs.frameRate = NULL;
//
//					//Button_Enable(lpCamProp->hButtonSave, FALSE);
//					//lpCamProp->flagSave = FALSE;
//					//SetWindowText(lpCamProp->hButtonSave, "Save");
//					
//				}
//
//				if(!lpCamProp->cs.flagOpen)
//				{
//					SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
//				}
//				lpCamProp->cs.flagOpen = TRUE;
//			}
//
//			if(LOWORD(wParam) == IDC_BTN_STOP){
//				Button_Enable(lpCamProp->hButtonSave, FALSE);
//				lpCamProp->cs.flagSave = FALSE;
//				lpCamProp->cs.flagStream = FALSE;
//				lpCamProp->cs.flagOpen = FALSE;
//				SetWindowText(lpCamProp->hButtonStream, "Start");
//			}
//
//
//			if(LOWORD(wParam) == IDC_BUTTON_SAVE){
//
//				
//				lpCamProp->flagSave = !lpCamProp->flagSave;
//				if(lpCamProp->flagSave){
//					SendMessage(lpCamProp->hDataManager, WM_SETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
//					lpCamProp->cs.flagSave = TRUE;
//					SetWindowText(lpCamProp->hButtonSave, "Stop Save");
//					Button_Enable(lpCamProp->hButtonPath, FALSE);
//				}
//				else{
//					lpCamProp->cs.flagSave = FALSE;
//					SetWindowText(lpCamProp->hButtonSave, "Save");
//					Button_Enable(lpCamProp->hButtonPath, TRUE);
//					SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_STREAM, BN_CLICKED), NULL);
//				}
//
//				SendMessage(lpCamProp->hDataManager, WM_SETARCHIVE, lpCamProp->flagSave, NULL);
//				
//			}
//
//			if(LOWORD(wParam) == IDC_BUTTON_PATH){
//				CHAR str[MAX_STRING];
//				OPENFILENAME ofn;
//				memset(&ofn, NULL, sizeof(OPENFILENAME));
//				ofn.lStructSize = sizeof(ofn);
//				ofn.lpstrFile = (CHAR *) str;
//				ofn.hwndOwner = hwnd;
//				ofn.nMaxFile = MAX_STRING;
//				ofn.Flags = OFN_EXPLORER;
//
//				
//				if(GetFolderPath(hwnd, str)){
//					memcpy(lpCamProp->strPath, str, MAX_STRING*sizeof(CHAR));
//					Edit_SetText(lpCamProp->hEditPath, lpCamProp->strPath);
//				}
//
//			}
//			break;
//		case EN_CHANGE:
//
//			if(LOWORD(wParam) == IDC_EDIT_PATH){
//
//				Edit_GetText(lpCamProp->hEditPath, lpCamProp->strPath, MAX_STRING);
//
//			}
//			if(LOWORD(wParam) == IDC_EDIT_EXPOSURE){
//				lpCamProp->exposure = GetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, NULL, FALSE);
//				SendMessage(lpCamProp->hSlider, 
//							TBM_SETPOS, 
//							(WPARAM) TRUE, 
//							(LPARAM) lpCamProp->exposure);
//			}
//			if(LOWORD(wParam) == IDC_EDIT_FRAMERATE){
//				Edit_GetText(lpCamProp->hEditFrameRate, str, MAX_STRING);
//				sscanf(str, "%f", &lpCamProp->prevFrameRate);
//				//sscanf(str, "%f", &lpCamProp->frameRate);
//
//				if(!lpCamProp->flagPause){
//					lpCamProp->cs.frameRate = lpCamProp->prevFrameRate;
//				}
//
//				if(!str[0]){
//					lpCamProp->cs.frameRate = NULL;
//				}
//				//lpCamProp->cs.frameRate = lpCamProp->frameRate;
//			}
//			if(LOWORD(wParam) == IDC_EDIT_SAMPLERATE){
//				Edit_GetText(lpCamProp->hEditSampleRate, str, MAX_STRING);
//				sscanf(str, "%d", &lpCamProp->cs.sampleRate);
//				if(!str[0]){
//					lpCamProp->cs.sampleRate = 1;
//				}
//			}
//			break;
//		case CBN_SELCHANGE:
//			INT sel;
//			CHAR strCamera[MAX_STRING];
//
//			sel = ComboBox_GetCurSel(lpCamProp->hComboCamera);
//			ComboBox_GetLBText(lpCamProp->hComboCamera, sel, strCamera);
//
//			SendMessage(hwnd, CPM_CAMCLOSE, NULL, NULL);
//
//			if(!strcmp(strCamera, STR_ZEISS_HSM))
//				lpCamProp->camera = CAMERA_ZEISS_HSM;	
//			else if(!strcmp(strCamera, STR_ZEISS_MRM))
//				lpCamProp->camera = CAMERA_ZEISS_MRM;
//			else if(!strcmp(strCamera, STR_PHOTONFOCUS))
//				lpCamProp->camera = CAMERA_PHOTONFOCUS;
//			else if(!strcmp(strCamera, STR_PIXELINK_A781))
//				lpCamProp->camera = CAMERA_PIXELINK;
//			else if(!strcmp(strCamera, STR_EMULATE_CAM))
//				lpCamProp->camera = CAMERA_EMULATE;
//			else
//				lpCamProp->camera = NULL;
//
//			SendMessage(hwnd, CPM_CAMINIT, NULL, NULL);
//			break;
//		default:
//			break;
//		}
//		//Control Identifier
//		switch(LOWORD(wParam))
//		{
//		case IDC_BUTTON_STREAM:
//			
//			return TRUE;
//		default:
//			break;
//		}
//		break;
//	//case WM_GETHANDLE:
//	//	if(wParam == HANDLE_ZEISS_MRM){
//	//		if(lpCamProp->camera == CAMERA_ZEISS_MRM){
//	//			return (LRESULT) 
//	case WM_HSCROLL:
//		
//		lpCamProp->exposure = SendMessage(lpCamProp->hSlider, TBM_GETPOS, NULL, NULL);
//
//		SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, lpCamProp->exposure, TRUE);
//
//		if(lpCamProp->camera == CAMERA_ZEISS_HSM){
//			INT result;
//			result = SetExposure(&lpCamProp->zeissProc, lpCamProp->exposure);
//			//result = GetExposure(&lpCamProp->zeissProc);
//			//SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, result, TRUE);
//		}
//
//		if(lpCamProp->camera == CAMERA_ZEISS_MRM){
//			INT result;
//			result = SetExposure(&lpCamProp->zeissProc, lpCamProp->exposure);
//			//result = GetExposure(&lpCamProp->zeissProc);
//			//SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, result, TRUE);
//		}
//		break;
//	case WM_SETHWND:
//
//		if(wParam == HWND_OWNER){
//			lpCamProp->hOwner = (HWND) lParam;
//			SendMessage(lpCamProp->hOwner, WM_GETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
//			Edit_SetText(lpCamProp->hEditPath, lpCamProp->strPath);
//		}	
//		if(wParam == HWND_ACTIVE)
//			lpCamProp->hFocus = (HWND) lParam;
//		if(wParam == HWND_DATAMANAGER){
//			lpCamProp->hDataManager = (HWND) lParam;
//			SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
//		}
//
//		return TRUE;
//	case WM_GETHWND:
//
//		if(wParam == HWND_STAGECONTROLS){
//			HWND hStageControls;
//			hStageControls = (HWND) SendMessage(lpCamProp->hOwner, WM_GETHWND, HWND_STAGECONTROLS, NULL);
//			SetWindowLong(hwnd, NULL/*DWL_MSGRESULT*/, (LONG)hStageControls);
//			return (INT_PTR)hStageControls;
//		}
//		break;
//	case CPM_GETEXPOSURE:
//		if(wParam == CAMERA_ZEISS_MRM){
//			DWORD exposure = NULL;
//			exposure = (DWORD) GetExposure(&lpCamProp->zeissProc);
//			return DialogReturn(hwnd, exposure);
//			//return (LRESULT) exposure;
//		}
//		break;
//	case CPM_SETEXPOSURE:
//		if(wParam == CAMERA_ZEISS_MRM){
//			INT result;
//			lpCamProp->cs.flagExposure = TRUE;
//			lpCamProp->cs.exposure = (INT) lParam;
//			//Sleep(250);
//			//result = SetExposure(&lpCamProp->zeissProc, (INT) lParam);
//			//lpCamProp->cs.flagIdle = FALSE;
//		}
//		break;
//	case CPM_CAMINIT:
//		
//		switch(lpCamProp->camera)
//		{
//		case NULL:
//			SendMessage(hwnd, CPM_SETWIDTH, NULL, NULL);
//			SendMessage(hwnd, CPM_SETHEIGHT, NULL, NULL);
//			
//			break;
//		case CAMERA_ZEISS_HSM:
//			{
//				INT i;
//				INT nCam;
//				HMODULE dllZeiss;
//				ZEISSFRAMEINFO zfm;
//				INT width;
//				INT height;
//				dllZeiss = LoadZeissLibrary(hwnd, ZEISS_HSM);
//				ImportZeissFunctions(dllZeiss, &lpCamProp->zeissProcHSm);
//				nCam = GetNumCamera(&lpCamProp->zeissProcHSm);
//				i = InitCamera(&lpCamProp->zeissProcHSm);
//				GetCurrentImageSize(&lpCamProp->zeissProcHSm, &width, &height);
//				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) width, NULL);
//				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) height, NULL);
//				Button_Enable(lpCamProp->hButtonStream, TRUE);
//				lpCamProp->cs.lpVoid = &lpCamProp->zeissProcHSm;
//			}
//			return TRUE;
//		case CAMERA_ZEISS_MRM:
//			HMODULE dllZeiss;
//			ZEISSFRAMEINFO zfm;
//			INT width;
//			INT height;
//			dllZeiss = LoadZeissLibrary(hwnd, ZEISS_MRM);
//			ImportZeissFunctions(dllZeiss, &lpCamProp->zeissProc);
//			InitCamera(&lpCamProp->zeissProc);
//			GetCurrentImageSize(&lpCamProp->zeissProc, &width, &height);
//			SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) width, NULL);
//			SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) height, NULL);
//			Button_Enable(lpCamProp->hButtonStream, TRUE);
//			lpCamProp->cs.lpVoid = &lpCamProp->zeissProc;
//			//CloseCamera(&lpCamProp->zeissProc);
//
//			//if(dllZeiss)
//			//	FreeLibrary(dllZeiss);
//
//			return TRUE;
//		case CAMERA_PIXELINK:
//			HANDLE hCamera;
//			PXL_RETURN_CODE pxlReturn;
//			pxlReturn = PxLInitialize(NULL/*7810391*/, &lpCamProp->hCamera);
//			if(!pxlReturn){
//				U32 sizeCamFeatures;
//				CAMERA_FEATURES *camFeatures;
//				ThrowMessage(lpCamProp->hOwner, "Connected to PixeLINK-A781");
//				PxLGetCameraFeatures(lpCamProp->hCamera, FEATURE_ALL, NULL, &sizeCamFeatures);
//				camFeatures = (CAMERA_FEATURES *) calloc(1, sizeCamFeatures);
//				PxLGetCameraFeatures(lpCamProp->hCamera, FEATURE_ALL, camFeatures, &sizeCamFeatures);
//				sizeCamFeatures = sizeCamFeatures;
//				SendMessage(hwnd, CPM_SETWIDTH, (WPARAM)2208, NULL);
//				SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM)3000, NULL);
//				PxLUninitialize(lpCamProp->hCamera);
//				Button_Enable(lpCamProp->hButtonStream, TRUE);
//			}
//			else{
//				ThrowMessage(lpCamProp->hOwner, "Failed to Connect to PixeLINK-A781");
//			}
//		case CAMERA_PHOTONFOCUS:
//#ifdef RACE_BITFLOW
//
//			BITFLOW bf;
//			InitPhotonFocusCam(&bf);
//			SendMessage(hwnd, CPM_SETWIDTH, (WPARAM) bf.wFrame, NULL);
//			SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM) bf.hFrame, NULL);
//			CloseBitFlowBoard(&bf);
//			Button_Enable(lpCamProp->hButtonStream, TRUE);
//#endif
//
//			return TRUE;
//		case CAMERA_EMULATE:
//			Button_Enable(lpCamProp->hButtonStream, TRUE);
//			return TRUE;
//		default:
//			return TRUE;
//		}
//
//		return TRUE;
//	case CPM_SETWIDTH:
//		CHAR strDim[MAX_STRING];
//		sprintf(strDim, "%d", wParam);
//		Edit_SetText(lpCamProp->hEditWidth, strDim);
//		return TRUE;
//	case CPM_SETHEIGHT:
//		sprintf(strDim, "%d", wParam);
//		Edit_SetText(lpCamProp->hEditHeight, strDim);
//		return TRUE;
//	case CPM_CAMCLOSE:
//		switch(lpCamProp->camera)
//		{
//		case CAMERA_ZEISS_HSM:
//			//lpCamProp->zeissProc.CloseCamera();
//			//ThrowMessage(lpCamProp->hOwner, "Disconnected from Zeiss HSm");
//			//lpCamProp->cs.flagClose = TRUE;
//			return TRUE;
//		case CAMERA_PIXELINK:
//			PxLUninitialize(lpCamProp->hCamera);
//			ThrowMessage(lpCamProp->hOwner, "Disconnected from PixeLINK-A781");
//			return TRUE;
//		case CAMERA_PHOTONFOCUS:
//			ThrowMessage(lpCamProp->hOwner, "Disconnected from PhotonFocus");
//			return TRUE;
//		case CAMERA_EMULATE:
//			ThrowMessage(lpCamProp->hOwner, "Disconnected from Emulate Camera");
//			lpCamProp->flagStream = FALSE;
//			Button_Enable(lpCamProp->hButtonSave, FALSE);
//			SetWindowText(lpCamProp->hButtonStream, "Stream");
//			lpCamProp->flagSave = FALSE;
//			return TRUE;
//		default:
//			return TRUE;
//		}
//		return TRUE;
//	case CPM_CAPTURE:
//		if(!lpCamProp->flagStream){
//			lpCamProp->cs.flagStream = FALSE;
//			lpCamProp->cs.flagOpen = FALSE;
//			lpCamProp->hDataManager = NULL;
//			Sleep(400);
//		}
//		else
//		{
//			if(!lpCamProp->hDataManager){
//				lpCamProp->cs.flagStream = FALSE;
//				lpCamProp->cs.flagOpen = FALSE;
//				SendMessage(lpCamProp->hOwner, WM_COMMAND, LOWORD(ID_OPEN_STREAM), (LPARAM) hwnd);
//				Sleep(100);
//			}
//			else{
//				lpCamProp->cs.hDataManager = lpCamProp->hDataManager;
//				lpCamProp->cs.hReference = hwnd;
//				lpCamProp->cs.flagStream = TRUE;
//				lpCamProp->cs.flagSave = FALSE;
//				lpCamProp->cs.flagClose = FALSE;
//				lpCamProp->cs.flagOpen = TRUE;
//			
//		
//				switch(lpCamProp->camera)
//				{
//				case CAMERA_ZEISS_HSM:
//					CreateThread(NULL, NULL, ZeissHSmCapture, &lpCamProp->cs, NULL, NULL);
//					return TRUE;
//				case CAMERA_ZEISS_MRM:
//					CreateThread(NULL, NULL, ZeissMRmCapture, &lpCamProp->cs, NULL, NULL);
//					return TRUE;
//				case CAMERA_PIXELINK:
//					CreateThread(NULL, NULL, PixeLINKThread, &lpCamProp->cs, NULL, NULL);
//					return TRUE;
//				case CAMERA_PHOTONFOCUS:
//					CreateThread(NULL, NULL, PhotonFocusThread, &lpCamProp->cs, NULL, NULL);
//					return TRUE;
//				case CAMERA_EMULATE:
//					CreateThread(NULL, NULL, EmulateCaptureStream, &lpCamProp->cs, NULL, NULL);
//					return TRUE;
//				default:
//					return TRUE;
//				}
//			}
//		}
//		return TRUE;
//	case WM_CLOSE:
//
//		SendMessage(lpCamProp->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_CAMERAPROPERTIES), NULL);
//
//		if(lpCamProp->camera == CAMERA_ZEISS_HSM)
//			lpCamProp->zeissProc.CloseCamera();
//
//		return TRUE;
//	default:
//		return FALSE;
//	}
//
//
//	return FALSE;
//
//}


//Removed from BN_CLICKED:
			//if(LOWORD(wParam) == IDC_BUTTON_STREAM){
			//	//Begin Sream using specified camera parameters
			//	//Active IDC_BUTTON_SAVE

			//	lpCamProp->flagStream = TRUE;
			//	
			//	lpCamProp->flagPause = !lpCamProp->flagPause;
			//	//lpCamProp->flagStream = !lpCamProp->flagStream;
			//	//lpCamProp->cs.flagOpen = !lpCamProp->cs.flagOpen;
			//	if(!lpCamProp->flagPause){
			//		
			//		Button_Enable(lpCamProp->hButtonSave, TRUE);
			//		SetWindowText(lpCamProp->hButtonStream, "Pause");
			//		lpCamProp->cs.frameRate = lpCamProp->prevFrameRate;
			//		//SetWindowText(lpCamProp->hButtonStream, "Stop Stream");
			//	}
			//	else{
			//		//Pause
			//		SetWindowText(lpCamProp->hButtonStream, "Start");
			//		lpCamProp->prevFrameRate = lpCamProp->cs.frameRate;
			//		lpCamProp->cs.frameRate = NULL;

			//		//Button_Enable(lpCamProp->hButtonSave, FALSE);
			//		//lpCamProp->flagSave = FALSE;
			//		//SetWindowText(lpCamProp->hButtonSave, "Save");
			//		
			//	}

			//	if(!lpCamProp->cs.flagOpen)
			//	{
			//		SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
			//	}
			//	lpCamProp->cs.flagOpen = TRUE;
			//}

			//if(LOWORD(wParam) == IDC_BTN_STOP){
			//	Button_Disable(lpCamProp->hButtonSave);
			//	lpCamProp->cs.flagSave = FALSE;
			//	lpCamProp->cs.flagStream = FALSE;
			//	lpCamProp->cs.flagOpen = FALSE;
			//	SetWindowText(lpCamProp->hButtonStream, "Start");
			//}


			//if(LOWORD(wParam) == IDC_BUTTON_SAVE){

			//	
			//	lpCamProp->flagSave = !lpCamProp->flagSave;
			//	if(lpCamProp->flagSave){
			//		SendMessage(lpCamProp->hDataManager, WM_SETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
			//		lpCamProp->cs.flagSave = TRUE;
			//		SetWindowText(lpCamProp->hButtonSave, "Stop Save");
			//		Button_Enable(lpCamProp->hButtonPath, FALSE);
			//	}
			//	else{
			//		lpCamProp->cs.flagSave = FALSE;
			//		SetWindowText(lpCamProp->hButtonSave, "Save");
			//		Button_Enable(lpCamProp->hButtonPath, TRUE);
			//		SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_STREAM, BN_CLICKED), NULL);
			//	}

			//	SendMessage(lpCamProp->hDataManager, WM_SETARCHIVE, lpCamProp->flagSave, NULL);
			//	
			//}

			//if(LOWORD(wParam) == IDC_BUTTON_PATH){
			//	CHAR str[MAX_STRING];
			//	OPENFILENAME ofn;
			//	memset(&ofn, NULL, sizeof(OPENFILENAME));
			//	ofn.lStructSize = sizeof(ofn);
			//	ofn.lpstrFile = (CHAR *) str;
			//	ofn.hwndOwner = hwnd;
			//	ofn.nMaxFile = MAX_STRING;
			//	ofn.Flags = OFN_EXPLORER;

			//	
			//	if(GetFolderPath(hwnd, str)){
			//		memcpy(lpCamProp->strPath, str, MAX_STRING*sizeof(CHAR));
			//		Edit_SetText(lpCamProp->hEditPath, lpCamProp->strPath);
			//	}

			//}

//Removed From EN_CHANGED:
			//if(LOWORD(wParam) == IDC_EDIT_PATH){

			//	Edit_GetText(lpCamProp->hEditPath, lpCamProp->strPath, MAX_STRING);

			//}
			//if(LOWORD(wParam) == IDC_EDIT_EXPOSURE){
			//	lpCamProp->exposure = GetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, NULL, FALSE);
			//	SendMessage(lpCamProp->hSlider, 
			//				TBM_SETPOS, 
			//				(WPARAM) TRUE, 
			//				(LPARAM) lpCamProp->exposure);
			//}
			//if(LOWORD(wParam) == IDC_EDIT_FRAMERATE){
			//	Edit_GetText(lpCamProp->hEditFrameRate, str, MAX_STRING);
			//	sscanf(str, "%f", &lpCamProp->prevFrameRate);
			//	//sscanf(str, "%f", &lpCamProp->frameRate);

			//	if(!lpCamProp->flagPause){
			//		lpCamProp->cs.frameRate = lpCamProp->prevFrameRate;
			//	}

			//	if(!str[0]){
			//		lpCamProp->cs.frameRate = NULL;
			//	}
			//	//lpCamProp->cs.frameRate = lpCamProp->frameRate;
			//}
			//if(LOWORD(wParam) == IDC_EDIT_SAMPLERATE){
			//	Edit_GetText(lpCamProp->hEditSampleRate, str, MAX_STRING);
			//	sscanf(str, "%d", &lpCamProp->cs.sampleRate);
			//	if(!str[0]){
			//		lpCamProp->cs.sampleRate = 1;
			//	}
			//}
//Removed From WM_GETHWND
		//if(wParam == HWND_STAGECONTROLS){
		//	HWND hStageControls;
		//	hStageControls = (HWND) SendMessage(lpCamProp->hOwner, WM_GETHWND, HWND_STAGECONTROLS, NULL);
		//	SetWindowLong(hwnd, NULL/*DWL_MSGRESULT*/, (LONG)hStageControls);
		//	return (INT_PTR)hStageControls;
		//}