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
		OnInitDialogCameraProperties(lpCamProp, hwnd, msg, wParam, lParam);
		break;
	case WM_CREATELINK:
		AddWndLink(&lpCamProp->cActive->wls, (HWND) wParam);
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
	//case WM_HSCROLL:
	//	
	//	lpCamProp->exposure = SendMessage(lpCamProp->hSlider, TBM_GETPOS, NULL, NULL);

	//	SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, lpCamProp->exposure, TRUE);

	//	if(lpCamProp->camera == CAMERA_ZEISS_HSM){
	//		INT result;
	//		result = SetExposure(&lpCamProp->zeissProc, lpCamProp->exposure);
	//	}

	//	if(lpCamProp->camera == CAMERA_ZEISS_MRM){
	//		INT result;
	//		result = SetExposure(&lpCamProp->zeissProc, lpCamProp->exposure);
	//	}
	//	break;
	case WM_SETHWND:
		OnCameraPropertiesSetHWND(hwnd, wParam, lParam);
		return TRUE;
	case WM_GETHWND:
		OnCameraPropertiesGetHWND(hwnd, wParam, lParam);
		return TRUE;
	//case CPM_GETEXPOSURE:
	//	if(wParam == CAMERA_ZEISS_MRM){
	//		DWORD exposure = NULL;
	//		exposure = (DWORD) GetExposure(&lpCamProp->zeissProc);
	//		return DialogReturn(hwnd, exposure);
	//		//return (LRESULT) exposure;
	//	}
	//	break;
	//case CPM_SETEXPOSURE:
	//	if(wParam == CAMERA_ZEISS_MRM){
	//		INT result;
	//		lpCamProp->cActive->flagExposure = TRUE;
	//		lpCamProp->cActive->exposure = (INT) lParam;
	//	}
	//	break;
	case CPM_CAMINIT:
		OnCameraPropertiesInitializeCamera(hwnd, wParam, lParam); 
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
		lpCamProp->cActive->flagSave = FALSE;
		lpCamProp->cActive->flagStream = FALSE;
		lpCamProp->cActive->flagOpen = FALSE;
		//NEW
		switch(lpCamProp->camera)
		{
		case CAMERA_ZEISS_HSM:
			//lpCamProp->zeissProc.CloseCamera();
			//ThrowMessage(lpCamProp->hOwner, "Disconnected from Zeiss HSm");
			//lpCamProp->cActive->flagClose = TRUE;
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
		return OnCaptureCameraProperties(lpCamProp, hwnd, msg, wParam, lParam);
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

LRESULT OnCaptureCameraProperties(CAMERAPROPERTIES *lpCamProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HANDLE hThread;
	CAPTURE c = {0};
	CAPTURE *pc = NULL;
	c.hOwner = hwnd;
	c.flagStream = TRUE;
	c.exposure = lpCamProp->exposure;
	c.sampleRate = 1;//lpCamProp->sampleRate;
	c.frameRate = lpCamProp->frameRate;
	c.prevFrameRate = c.frameRate;
	c.flagOpen = TRUE;
	c.hDataManager = CreateDataManager(lpCamProp->hOwner, DS_FRAMEGRABBER);

	Append(&lpCamProp->cs, c);
	pc = &lpCamProp->cs.c[lpCamProp->cs.nCapture-1];

	switch(lpCamProp->camera)
	{
	case CAMERA_PHOTONFOCUS:
		hThread = CreateThread(NULL, NULL, PhotonFocusThread, (VOID *)pc, NULL, NULL);
		break;
	case CAMERA_EMULATE:
		hThread = CreateThread(NULL, NULL, EmulateCaptureStream, (VOID *)pc, NULL, NULL);
		break;
	case CAMERA_PIXELINK:
		hThread = CreateThread(NULL, NULL, PixeLINKThread, (VOID *)pc, NULL, NULL);
		break;
	case CAMERA_DESKTOP:
		hThread = CreateThread(NULL, NULL, DesktopCaptureThread, (VOID *)pc, NULL, NULL);
		break;
	case CAMERA_RACE:
		//hThread = CreateThread(NULL, NULL, RACECaptureThread, (VOID *)pc, NULL, NULL);
		break;
	default:
		break;
	}
	
	pc->hThread = hThread;
	

		return TRUE;
}

DWORD OnUpdateActiveCameraProperteis(CAMERAPROPERTIES *lpCamProp)
{
	if(!lpCamProp)return FALSE;
	if(!lpCamProp->cActive)return FALSE;

	return TRUE;
}

LRESULT OnInitDialogCameraProperties(CAMERAPROPERTIES *lpCamProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	lpCamProp = (CAMERAPROPERTIES *) InitWindowLongRACE(hwnd, sizeof(CAMERAPROPERTIES));
	
	if(!lParam)return FALSE;

	HWND hOwner;
	PROPSHEETPAGE psp;
	memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
	hOwner = (HWND) psp.lParam;
	lpCamProp->hOwner = hOwner;

	PostMessage(hwnd, WM_SETHWND, HWND_OWNER, (LPARAM) hOwner);
	
	InitializeCameraProperties(hwnd, lpCamProp);

	return TRUE;
}

DWORD InitializeCameraProperties(HWND hwnd, CAMERAPROPERTIES *lpCamProp)
{

	CHAR str[MAX_STRING];
	Alloc(&lpCamProp->cs, 20);

	lpCamProp->cActive = &lpCamProp->capture;

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

	lpCamProp->frameRate = 20.0;
	lpCamProp->prevFrameRate = lpCamProp->frameRate;
	lpCamProp->cActive->frameRate = lpCamProp->frameRate;
	lpCamProp->flagPause = TRUE;
	sprintf(str, "%.1f", lpCamProp->frameRate);
	Edit_SetText(lpCamProp->hEditFrameRate, str);

	lpCamProp->cActive->sampleRate = 1;
	sprintf(str, "%d", lpCamProp->cActive->sampleRate);
	Edit_SetText(lpCamProp->hEditSampleRate, str);

	ComboBox_AddString(lpCamProp->hComboCamera, STR_NOSELECTION);
	ComboBox_AddString(lpCamProp->hComboCamera, STR_EMULATE_CAM);
	//ComboBox_AddString(lpCamProp->hComboCamera, STR_DESKTOP);

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

	lpCamProp->cActive->hRACE = lpCamProp->hOwner;
	AllocWndLink(&lpCamProp->cActive->wls, 5);

	SetHWND(lpCamProp->hOwner, hwnd, HWND_CAMERAPROPERTIES);

	return TRUE;
}

LRESULT OnCameraPropertiesButtonClicked(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowUserData(hwnd);

	CAPTURE *cActive = lpCamProp->cActive;

	switch(LOWORD(wParam))
	{
	case IDC_BUTTON_STREAM:
		{
			if(LOWORD(wParam) == IDC_BUTTON_STREAM){
				//Begin Sream using specified camera parameters
				//Active IDC_BUTTON_SAVE

				//lpCamProp->flagStream = TRUE;
				if(cActive)cActive->flagStream = TRUE;

				
				//lpCamProp->flagPause = !lpCamProp->flagPause;
				cActive->flagPause = !cActive->flagPause;
				if(!cActive->flagPause){
				//if(!lpCamProp->flagPause){
					
					Button_Enable(lpCamProp->hButtonSave, TRUE);
					SetWindowText(lpCamProp->hButtonStream, "Pause");
					cActive->frameRate = cActive->prevFrameRate;
					//lpCamProp->cActive->frameRate = lpCamProp->prevFrameRate;
					//if(cActive){cActive->frameRate = lpCamProp->prevFrameRate;}
				}
				else{
					//Pause
					SetWindowText(lpCamProp->hButtonStream, "Start");
					cActive->frameRate = NULL;
					
				}

				Edit_SetFloat(lpCamProp->hEditFrameRate, cActive->prevFrameRate);
				
				if(!cActive->flagOpen)
				{
					SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
					cActive->flagOpen = TRUE;
				}
			}
		}
		return TRUE;
	case IDC_BTN_STOP:
		{
			Button_Disable(lpCamProp->hButtonSave);
			SetWindowText(lpCamProp->hButtonStream, "Start");
			SendMessage(hwnd, CPM_CAMCLOSE, NULL, NULL);
			//lpCamProp->cActive->flagSave = FALSE;
			//lpCamProp->cActive->flagStream = FALSE;
			//lpCamProp->cActive->flagOpen = FALSE;
			
		}
		return TRUE;
	case IDC_BTN_SAVE:
		{
			lpCamProp->flagSave = !lpCamProp->flagSave;
			if(lpCamProp->flagSave){
				SendMessage(lpCamProp->hDataManager, WM_SETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
				lpCamProp->cActive->flagSave = TRUE;
				SetWindowText(lpCamProp->hButtonSave, "Stop Save");
				Button_Enable(lpCamProp->hButtonPath, FALSE);
			}
			else{
				lpCamProp->cActive->flagSave = FALSE;
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
			sscanf(str, "%f", &lpCamProp->cActive->prevFrameRate);

			if(!lpCamProp->cActive->flagPause){
				lpCamProp->cActive->frameRate = lpCamProp->cActive->prevFrameRate;
			}

			if(!str[0]){
				lpCamProp->cActive->frameRate = NULL;
			}
		}return TRUE;
	case IDC_EDIT_SAMPLERATE:
		{
				Edit_GetText(lpCamProp->hEditSampleRate, str, MAX_STRING);
				sscanf(str, "%d", &lpCamProp->cActive->sampleRate);
				if(!str[0]){
					lpCamProp->cActive->sampleRate = 1;
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
		{
			lpCamProp->hOwner = (HWND) lParam;
			lpCamProp->hFrame =  GetHWND(lpCamProp->hOwner, HWND_FRAME);
			SendMessage(lpCamProp->hFrame, WM_GETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
			Edit_SetText(lpCamProp->hEditPath, lpCamProp->strPath);	
			SendMessage(lpCamProp->hOwner, WM_SETHWND, (WPARAM) HWND_CAMERAPROPERTIES, (LPARAM) hwnd);
		}return TRUE;
	case HWND_ACTIVE:
		{
			DWORD i;
			lpCamProp->hFocus = (HWND) lParam;
			if(Find(&lpCamProp->cs, lpCamProp->hFocus, &i)){
				lpCamProp->cActive = &lpCamProp->cs.c[i];
			}
			else{
				memset(&lpCamProp->capture, NULL, sizeof(CAPTURE));
				lpCamProp->cActive = &lpCamProp->capture;
				lpCamProp->cActive->frameRate = 25.0;
			}

			OnChangeCaptureCameraProperties(lpCamProp);

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

LRESULT OnChangeCaptureCameraProperties(CAMERAPROPERTIES *lpCamProp)
{
	if(!lpCamProp)
		return FALSE;

	CAPTURE *c = lpCamProp->cActive;

	if(!c->flagPause){
		Button_Enable(lpCamProp->hButtonSave, TRUE);
		SetWindowText(lpCamProp->hButtonStream, "Pause");
	}else{
		SetWindowText(lpCamProp->hButtonStream, "Stream");
	}

	if(c->flagSave){
		SetWindowText(lpCamProp->hButtonSave, "Stop Save");
		Button_Enable(lpCamProp->hButtonPath, FALSE);
	}else{
		SetWindowText(lpCamProp->hButtonSave, "Save");
		Button_Enable(lpCamProp->hButtonPath, TRUE);
	}

	if(c->flagClose){

	}

	Edit_SetFloat(lpCamProp->hEditFrameRate, c->prevFrameRate);

	



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
				Button_Enable(lpCamProp->hButtonStream, TRUE);

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
				lpCamProp->cActive->lpVoid = &lpCamProp->zeissProcHSm;
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
				lpCamProp->cActive->lpVoid = &lpCamProp->zeissProc;
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

		return TRUE;
}


DWORD Alloc(CAPTURESET *cs, DWORD size)
{
	
	if(!cs)
		return FALSE;

	if(cs->c)
		return FALSE;

	cs->c = (CAPTURE *) malloc(size*sizeof(CAPTURE));
	cs->nCapture = 0;
	cs->size = size;

	if(!cs->c)
		return FALSE;


	return TRUE;
}

DWORD Append(CAPTURESET *cs, CAPTURE c)
{
	
	if(!cs)
		return FALSE;

	if(!cs->c)
		return FALSE;

	if(cs->nCapture >= cs->size)
		return FALSE;

	memcpy(&cs->c[cs->nCapture], &c, sizeof(CAPTURE));

	cs->nCapture++;

	return TRUE;
}

DWORD Delete(CAPTURESET *cs, DWORD index)
{
	
	if(!cs)
		return FALSE;

	if(!cs->c)
		return FALSE;

	if(index > cs->nCapture)
		return FALSE;

	//Copy the last index into the one being removed
	memcpy(&cs->c[index], &cs->c[cs->nCapture], sizeof(CAPTURE));

	cs->nCapture--;

	return TRUE;
}

DWORD Find(CAPTURESET *cs, HWND hwnd, DWORD *index)
{
	if(!cs)return FALSE;
	if(!index)return FALSE;
	if(!cs->c)return FALSE;

	DWORD i = 0;
	for(i = 0; i < cs->nCapture; i++){
		if(hwnd == cs->c[i].hDataView){
			*index = i;
			return TRUE;
		}
	}


	return FALSE; 
}

DWORD Alloc(VOIDSET *vs, DWORD size, DWORD szType)
{
	if(!vs)
		return FALSE;

	if(vs->ptr)
		return FALSE;

	vs->ptr = malloc(size*vs->szType);
	
	if(!vs->ptr){
		vs->n = 0;
		vs->szPtr = 0;
		vs->szType = 0;
		return FALSE;
	}

	vs->n = 0;
	vs->szPtr = size;
	vs->szType = szType;

	return TRUE;
}

DWORD Append(VOIDSET *vs, CAPTURE c)
{
	if(!vs)
		return FALSE;

	if(!vs->ptr)
		return FALSE;

	CAPTURE *capture = (CAPTURE *)vs->ptr;
	
	if(vs->n >= vs->szPtr)
		return FALSE;

	memcpy(&capture[vs->n], &c, sizeof(CAPTURE));
	vs->n++;

	return TRUE;
}

DWORD Delete(VOIDSET *vs, DWORD index, CAPTURE c)
{
	
	if(!vs)
		return FALSE;

	if(!vs->ptr)
		return FALSE;

	if(index > vs->n)
		return FALSE;

	//Copy the last index into the one being removed
	//memcpy((VOID *)vs->ptr + (index*vs->szType), (VOID *)vs->ptr+(vs->n*vs->szType), vs->szType);

	vs->n--;

	return TRUE;
}

