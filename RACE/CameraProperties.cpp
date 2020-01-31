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
#include "String.h"
#include "vfw.h"
#include "C/FlyCapture2_C.h"
#include "Chameleon CMLN-13S2M.h"


INT_PTR CALLBACK CameraPropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	//CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:
		OnInitDialogCameraProperties(lpCamProp, hwnd, msg, wParam, lParam);
		break;
	case WM_COMMAND:
		OnCameraPropertiesCommand(hwnd, msg, wParam, lParam);
		return FALSE;
	case WM_SETHWND:
		OnCameraPropertiesSetHWND(hwnd, wParam, lParam);
		return TRUE;
	case WM_GETHWND:
		OnCameraPropertiesGetHWND(hwnd, wParam, lParam);
		return TRUE;
	case CPM_CAMINIT:
		OnCameraPropertiesInitializeCamera(hwnd, wParam, lParam); 
		return FALSE;
	case CPM_SETWIDTH:
		CHAR strDim[MAX_STRING];
		sprintf_s(strDim, "%d", wParam);
		Edit_SetText(lpCamProp->hEditWidth, strDim);
		return FALSE;
	case CPM_SETHEIGHT:
		sprintf_s(strDim, "%d", wParam);
		Edit_SetText(lpCamProp->hEditHeight, strDim);
		return FALSE;
	case CPM_CAMCLOSE:
		return FALSE; //OnCameraPropertiesClose(hwnd, wParam, lParam);
	case CPM_CAPTURE:
		return OnCameraPropertiesCapture(lpCamProp, hwnd, msg, wParam, lParam);
	case WM_CREATELINK:
		AddWndLink(&lpCamProp->cActive->wls, (HWND) wParam);
		return FALSE;
	case WM_STREAMCLOSED:
		return OnCameraPropertiesStreamClosed(hwnd, wParam, lParam);
	case WM_CLOSING:
		return OnCameraPropertiesClosing(hwnd, wParam, lParam);
	case WM_CLOSE:
		return OnCameraPropertiesClose(hwnd, wParam, lParam);
	case WM_DESTROY:
		//CameraProperties_CloseAllStreams()
		PostQuitMessage(0);
		return FALSE;
	default:
		return FALSE;
	}

	return FALSE;

}


DWORD CameraProperties_CloseStream(HWND hwnd, CHAR *strID)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	//TODO: Add
	

	return TRUE;
}
LRESULT OnCameraPropertiesStreamClosed(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAPTURE *c = (CAPTURE *)wParam;
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if(!c)return FALSE;

	//Delete CAPTURE from the CAPTURESET
	DWORD i = 0;
	for(i = 0; i < lpCamProp->cs.nCapture; i++){
		if(c == &lpCamProp->cs.c[i]){
			memset(&lpCamProp->cs.c[i], NULL, sizeof(CAPTURE));
		}
	}

	return FALSE;
}
LRESULT OnCameraPropertiesClosing(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	DWORD i;
	HWND hClosing = (HWND) lParam;

	if(Find(&lpCamProp->cs, hClosing, &i)){
		//if(MessageBox(NULL, "Stop Stream?", "Camera Properties", MB_OKCANCEL|MB_ICONQUESTION) == IDOK){
			lpCamProp->cs.c[i].flgStream = FALSE;
			return DialogReturn(hwnd, TRUE);
		//}
	}else{
		return DialogReturn(hwnd, FALSE);
	}

	//return FALSE;
}


LRESULT OnCameraPropertiesCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	//Control-Defined Notifiation Code
	switch(HIWORD(wParam))
	{
	case BN_CLICKED:
		return OnCameraPropertiesButtonClicked(hwnd, wParam, lParam);
	case EN_CHANGE:
		return OnCameraPropertiesEditChanged(hwnd, wParam, lParam);
	case CBN_SELCHANGE:
		return OnCameraPropertiesSelChanged(hwnd, wParam, lParam);
	default:
		return TRUE;
	}
}

LRESULT OnCameraPropertiesCapture(CAMERAPROPERTIES *lpCamProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HANDLE hThread = NULL;
	CAPTURE *pc = NULL;
	pc = lpCamProp->cActive;

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
	case CAMERA_CHAMELEON:
		hThread = CreateThread(NULL, NULL, ChameleonThread, (VOID *)pc, NULL, NULL);
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

	SetHWND(lpCamProp->hOwner, hwnd, HWND_CAMERAPROPERTIES);

	CHAR str[MAX_STRING];
	Alloc(&lpCamProp->cs, 20);

	lpCamProp->cActive = &lpCamProp->cs.c[0];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(lpCamProp->strPath, NULL, MAX_STRING*sizeof(CHAR));

	lpCamProp->hComboCamera = GetDlgItem(hwnd, IDC_COMBO_CAMERA);
	lpCamProp->hListStream = GetDlgItem(hwnd, IDC_LIST_STREAM);

	lpCamProp->hBtnPath = GetDlgItem(hwnd, IDC_BTN_PATH);
	lpCamProp->hBtnStream = GetDlgItem(hwnd, IDC_BTN_STREAM);
	lpCamProp->hBtnSave = GetDlgItem(hwnd, IDC_BTN_SAVE);
	lpCamProp->hBtnSnap = GetDlgItem(hwnd, IDC_BTN_SNAP);
	lpCamProp->hBtnAutoExposure = GetDlgItem(hwnd, IDC_BTN_AUTOEXPOSURE);
	lpCamProp->hBtnStop = GetDlgItem(hwnd, IDC_BTN_CLOSE);

	lpCamProp->hEditPath = GetDlgItem(hwnd, IDC_EDIT_PATH);
	lpCamProp->hEditExposure = GetDlgItem(hwnd, IDC_EDIT_EXPOSURE);
	lpCamProp->hEditHeight = GetDlgItem(hwnd, IDC_EDIT_HEIGHT);
	lpCamProp->hEditWidth = GetDlgItem(hwnd, IDC_EDIT_WIDTH);
	lpCamProp->hEditFrameRate = GetDlgItem(hwnd, IDC_EDIT_FRAMERATE);
	lpCamProp->hEditSampleRate = GetDlgItem(hwnd, IDC_EDIT_SAMPLERATE);
	
	Button_Enable(lpCamProp->hBtnStream, FALSE);
	Button_Enable(lpCamProp->hBtnSave, FALSE);
	Button_Enable(lpCamProp->hBtnSnap, TRUE);
	Button_Enable(lpCamProp->hBtnStop, FALSE);

	lpCamProp->hCheckFrameRate = GetDlgItem(hwnd, IDC_CHECK_FRAMERATE);

	Edit_SetText(lpCamProp->hEditHeight, "N/A");
	Edit_SetText(lpCamProp->hEditWidth, "N/A");

	lpCamProp->frameRate = 20.0;
	lpCamProp->prevFrameRate = lpCamProp->frameRate;
	lpCamProp->cActive->frameRate = lpCamProp->frameRate;
	lpCamProp->flgPause = TRUE;
	sprintf(str, "%.1f", lpCamProp->frameRate);
	Edit_SetText(lpCamProp->hEditFrameRate, str);

	lpCamProp->cActive->sampleRate = 1;
	sprintf(str, "%d", lpCamProp->cActive->sampleRate);
	Edit_SetText(lpCamProp->hEditSampleRate, str);

	ComboBox_AddString(lpCamProp->hComboCamera, STR_EMULATE_CAM);
	//ComboBox_AddString(lpCamProp->hComboCamera, STR_DESKTOP);

	//Discover Camera Devices
	OnCameraPropertiesDiscoverDevices(lpCamProp);
	ComboBox_SetCurSel(lpCamProp->hComboCamera, 0);
	lpCamProp->camera = CAMERA_EMULATE;
	CameraProperties_EnableStreamControls(hwnd, FALSE);

	/*
	if(0){	//Testing Webcam
	lpCamProp->hCamCapture = capCreateCaptureWindow("Webcam Capture Window",
													WS_CHILD|WS_VISIBLE,
													400,0,200,150,
													(HWND) hwnd,
													(INT)0);
	capDriverConnect(lpCamProp->hCamCapture, 0);
	//capDlgVideoSource(lpCamProp->hCamCapture);
	capPreviewRate(lpCamProp->hCamCapture, 33);
	capPreviewScale(lpCamProp->hCamCapture, TRUE);
	capPreview(lpCamProp->hCamCapture, TRUE);
	ShowWindow(lpCamProp->hCamCapture, SW_SHOW);
	}*/

	//SetHWND(lpCamProp->hOwner, hwnd, HWND_CAMERAPROPERTIES);

	return TRUE;
}

//Searches for connected cameras
LRESULT OnCameraPropertiesDiscoverDevices(CAMERAPROPERTIES *lpCamProp)
{
	HWND hwnd = lpCamProp->hwnd;
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
		BITFLOW bf;
		if(InitPhotonFocusCam(&bf)){
			ComboBox_AddString(lpCamProp->hComboCamera, STR_PHOTONFOCUS);
			CloseBitFlowBoard(&bf);
		}
	#endif

	//#ifdef RACE_CHAMELEON
		if(1){
		INT w, h;
		UINT nCameras;
		fc2Error fcError;
		fc2CameraInfo fcCamInfo;
		fc2Context fcContext;

		nCameras = 0;
		fcError = fc2CreateContext(&fcContext);
		fcError = fc2GetNumOfCameras(fcContext, &nCameras);
		fcError = fc2GetResolution(fcContext, &w, &h);
		//fcError = fc2GetCameraInfo(fcContext, &fcCamInfo);
		if(nCameras){
			ComboBox_InsertString(lpCamProp->hComboCamera, -1, STR_CHAMELEON_CMLN_13S2M);
			//ComboBox_AddString(lpCamProp->hComboCamera, STR_CHAMELEON_CMLN_13S2M);
		}



		fc2DestroyContext(fcContext);
		}
	//#endif

	//Enumerate Capture Drivers

	/*
	DWORD i = 0;
	CHAR strDeviceName[MAX_STRING];
	CHAR strDeviceVersion[MAX_STRING];
	for(i = 0; i < 10; i++){
		if(capGetDriverDescription(i, strDeviceName, sizeof(strDeviceName), strDeviceVersion, sizeof(strDeviceVersion))){
			ComboBox_AddString(lpCamProp->hComboCamera, strDeviceName);
		}
	}*/
	

	lpCamProp->cActive->hRACE = lpCamProp->hOwner;

	return TRUE;
}

LRESULT OnCameraPropertiesSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam)){
		case IDC_COMBO_CAMERA:
			return OnCameraPropertiesComboSelChanged(hwnd, wParam, lParam);
		case IDC_LIST_STREAM:
			return OnCameraPropertiesListSelChanged(hwnd, wParam, lParam);
		default:
			return TRUE;
	}

}

LRESULT OnCameraPropertiesListSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	INT pos = 0;
	DWORD i = 0;
	CHAR strSel[MAX_STRING];
	pos = ListBox_GetCurSel(lpCamProp->hListStream);
	ListBox_GetText(lpCamProp->hListStream, pos, strSel);

	if(Find(&lpCamProp->cs, strSel, &i)){
		lpCamProp->cActive = &lpCamProp->cs.c[i];
	}

	return FALSE;
}

LRESULT OnCameraPropertiesComboSelChanged(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowUserData(hwnd);
	INT sel;
	CHAR strCamera[MAX_STRING];

	sel = ComboBox_GetCurSel(lpCamProp->hComboCamera);
	ComboBox_GetLBText(lpCamProp->hComboCamera, sel, strCamera);

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
	else if(!strcmp(strCamera, STR_CHAMELEON_CMLN_13S2M))
		lpCamProp->camera = CAMERA_CHAMELEON;
	else
		lpCamProp->camera = NULL;
	
	return FALSE;

}

LRESULT OnCameraPropertiesButtonClicked(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowUserData(hwnd);
	CAPTURE *cActive = lpCamProp->cActive;
	CHAR str[MAX_STRING];

	switch(LOWORD(wParam))
	{
	case IDC_BTN_OPEN:
		{
			//Opens the selected camera from the ComboBox
			//Puts the selected camera into the ListBox
			//Removes the selected camera from the ComboBox (only if it's not EMULATED)
			//Begins the streaming at the default frame rate

			INT pos = 0;
			CHAR strSource[MAX_STRING];
			pos = ComboBox_GetCurSel(lpCamProp->hComboCamera);
			ComboBox_GetLBText(lpCamProp->hComboCamera, pos, str);
			if(pos){
				//Removes the camera from the ComboBox if it's not emulated
				ComboBox_DeleteString(lpCamProp->hComboCamera, pos);
			}else{
				//Get the directory folder
				CHAR strDirectory[MAX_STRING];
				if(!GetFolderPath(lpCamProp->hOwner, strSource)){
					return FALSE;
				}
				DirectoryFromStringEx(strSource, strDirectory);
				sprintf(str, "Emulate: %s", strDirectory);
			}
			ListBox_AddString(lpCamProp->hListStream, str);
			pos = ListBox_SelectString(lpCamProp->hListStream, 0, str);
			ListBox_SetCurSel(lpCamProp->hListStream, pos);

			//Enable the stream controls
			CameraProperties_EnableStreamControls(hwnd, TRUE);
			SetWindowText(lpCamProp->hBtnStream, "Pause");

			//Start the selected stream
			CAPTURE c = {0};
			c.hOwner = hwnd;
			c.flgOpen = TRUE;
			c.flgClose = FALSE;
			c.flgStream = TRUE;
			c.flgPause = FALSE;
			c.exposure = lpCamProp->exposure;
			c.sampleRate = 1;
			c.frameRate = lpCamProp->frameRate;
			c.baseFrameRate = c.frameRate;
			c.hDataManager = CreateDataManager(lpCamProp->hOwner, DS_FRAMEGRABBER);
			sprintf(c.strID, "%s", str);
			sprintf(c.strSource, "%s", strSource);
			Append(&lpCamProp->cs, c);
			lpCamProp->cActive = &lpCamProp->cs.c[lpCamProp->cs.nCapture-1];
			SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);

			return FALSE;
		}
	case IDC_BTN_STREAM:
		{
			cActive->flgPause = !cActive->flgPause;
			if(!cActive->flgPause){
				//Stream
				Button_Enable(lpCamProp->hBtnSave, TRUE);
				SetWindowText(lpCamProp->hBtnStream, "Pause");
				cActive->frameRate = cActive->baseFrameRate;
			}
			else{
				//Pause
				SetWindowText(lpCamProp->hBtnStream, "Stream");
				cActive->frameRate = NULL;
			}

			Edit_SetFloat(lpCamProp->hEditFrameRate, cActive->baseFrameRate);
		}
		return FALSE;
	case IDC_BTN_CLOSE:
		{
			INT pos = 0;
			pos = ListBox_GetCurSel(lpCamProp->hListStream);
			ListBox_GetText(lpCamProp->hListStream, pos, str);
			ListBox_DeleteString(lpCamProp->hListStream, pos);
			if(!strncmp(str, "Emulate", strlen("Emulate"))){
				//Do not add this string back to the ComboBox
			}else{
				//ComboBox_AddString(lpCamProp->hComboCamera, str);
				ComboBox_InsertString(lpCamProp->hComboCamera, -1, str);
			}

			if(!ListBox_GetCount(lpCamProp->hListStream)){
				CameraProperties_EnableStreamControls(hwnd, FALSE);
			}

			cActive->flgStream = FALSE;
			
		}
		return FALSE;
	case IDC_BTN_SAVE:
		{
			lpCamProp->flgSave = !lpCamProp->flgSave;
			if(lpCamProp->flgSave){
				CHAR strPath[MAX_STRING];
				strcpy(strPath, lpCamProp->strPath);
				MakeValidPath(strPath, lpCamProp->strPath);
				SendMessage(cActive->hDataManager, WM_SETARCHIVEDIR, (WPARAM) lpCamProp->strPath, NULL);
				SendMessage(cActive->hDataManager, WM_SETARCHIVE, (WPARAM) TRUE, NULL);
				lpCamProp->cActive->flgSave = TRUE;
				SetWindowText(lpCamProp->hBtnSave, "Stop Save");
				Button_Enable(lpCamProp->hBtnPath, FALSE);
			}
			else{
				SendMessage(cActive->hDataManager, WM_SETARCHIVE, (WPARAM) FALSE, NULL);
				lpCamProp->cActive->flgSave = FALSE;
				SetWindowText(lpCamProp->hBtnSave, "Save");
				Button_Enable(lpCamProp->hBtnPath, TRUE);
			}

			SendMessage(lpCamProp->hDataManager, WM_SETARCHIVE, lpCamProp->flgSave, NULL);
				
		}
		return FALSE;
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
		return FALSE;
	case IDC_BTN_SNAP:
		{
			//HWND hActive;
			//hActive = (HWND) SendMessage(lpCamProp->hOwner, WM_GETHWND, (WPARAM) HWND_ACTIVE, NULL);
			HWND hActive = lpCamProp->cActive->hDataView;
			DataView_SnapFrame(hActive);
			ShowWindow(hActive, SW_SHOW);
		}
		return FALSE;
	default:
		return TRUE;
	}

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
		}return FALSE;
	case IDC_EDIT_EXPOSURE:
		{
			lpCamProp->exposure = GetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, NULL, FALSE);
			SendMessage(lpCamProp->hSlider, 
						TBM_SETPOS, 
						(WPARAM) TRUE, 
						(LPARAM) lpCamProp->exposure);
		}return FALSE;
	case IDC_EDIT_FRAMERATE:
		{
			Edit_GetText(lpCamProp->hEditFrameRate, str, MAX_STRING);
			sscanf(str, "%f", &lpCamProp->cActive->baseFrameRate);

			if(!lpCamProp->cActive->flgPause){
				lpCamProp->cActive->frameRate = lpCamProp->cActive->baseFrameRate;
			}

			if(!str[0]){
				lpCamProp->cActive->frameRate = NULL;
			}
		}return FALSE;
	case IDC_EDIT_SAMPLERATE:
		{
				Edit_GetText(lpCamProp->hEditSampleRate, str, MAX_STRING);
				sscanf(str, "%d", &lpCamProp->cActive->sampleRate);
				if(!str[0]){
					lpCamProp->cActive->sampleRate = 1;
				}
		}return FALSE;
	default:
		return TRUE;
	}

}

LRESULT OnCameraPropertiesChangeActive(CAMERAPROPERTIES *lpCamProp)
{
	if(!lpCamProp)
		return FALSE;

	CAPTURE *c = lpCamProp->cActive;

	if(!c->flgPause){
		Button_Enable(lpCamProp->hBtnSave, TRUE);
		SetWindowText(lpCamProp->hBtnStream, "Pause");
	}else{
		SetWindowText(lpCamProp->hBtnStream, "Stream");
	}

	if(c->flgSave){
		SetWindowText(lpCamProp->hBtnSave, "Stop Save");
	}else{
		SetWindowText(lpCamProp->hBtnSave, "Save");
	}

	ListBox_SelectString(lpCamProp->hListStream, 0, c->strID);

	Edit_SetFloat(lpCamProp->hEditFrameRate, c->frameRate);
	
	return FALSE;
}

LRESULT OnCameraPropertiesChangeCapture(CAMERAPROPERTIES *lpCamProp)
{
	if(!lpCamProp)
		return FALSE;

	CAPTURE *c = lpCamProp->cActive;

	if(!c->flgPause){
		Button_Enable(lpCamProp->hBtnSave, TRUE);
		SetWindowText(lpCamProp->hBtnStream, "Pause");
	}else{
		SetWindowText(lpCamProp->hBtnStream, "Stream");
	}

	if(c->flgSave){
		SetWindowText(lpCamProp->hBtnSave, "Stop Save");
		Button_Enable(lpCamProp->hBtnPath, FALSE);
	}else{
		SetWindowText(lpCamProp->hBtnSave, "Save");
		Button_Enable(lpCamProp->hBtnPath, TRUE);
	}

	if(c->flgClose){

	}

	Edit_SetFloat(lpCamProp->hEditFrameRate, c->baseFrameRate);

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
				Button_Enable(lpCamProp->hBtnStream, TRUE);

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
				Button_Enable(lpCamProp->hBtnStream, TRUE);
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
				Button_Enable(lpCamProp->hBtnStream, TRUE);
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
					Button_Enable(lpCamProp->hBtnStream, TRUE);
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
				Button_Enable(lpCamProp->hBtnStream, TRUE);
				#endif
			}
			return TRUE;
		case CAMERA_EMULATE:
			{
				Button_Enable(lpCamProp->hBtnStream, TRUE);
			}
			return TRUE;
		case CAMERA_CHAMELEON:
			{
				UINT nCameras;
				INT w, h;
				fc2Error fcError;
				fc2Context fcContext;

				fcError = fc2CreateContext(&fcContext);
				if(fc2GetResolution(fcContext, &w, &h)){
					ThrowMessage(lpCamProp->hOwner, "Connected to Chameleon");
					SendMessage(hwnd, CPM_SETWIDTH, (WPARAM)w, NULL);
					SendMessage(hwnd, CPM_SETHEIGHT, (WPARAM)h, NULL);
					Button_Enable(lpCamProp->hBtnStream, TRUE);
				}else{
					ThrowMessage(lpCamProp->hOwner, "Failed to Connect to Chameleon");
				}

				fc2DestroyContext(fcContext);
			}
			return TRUE;
		default:
			return TRUE;
		}

		return TRUE;
}


DWORD CameraProperties_EnableStreamControls(HWND hwnd, DWORD state)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowUserData(hwnd);

	if(!lpCamProp)
		return FALSE;

	Button_Enable(lpCamProp->hBtnStream, state);
	Button_Enable(lpCamProp->hBtnStop, state);
	Button_Enable(lpCamProp->hBtnSnap, state);
	Button_Enable(lpCamProp->hBtnSave, state);
	EnableWindow(lpCamProp->hEditFrameRate, state);
	EnableWindow(lpCamProp->hEditSampleRate, state);
	EnableWindow(lpCamProp->hCheckFrameRate, FALSE);
	//EnableWindow(lpCamProp->hCheckSampleRate, FALSE);
	EnableWindow(lpCamProp->hEditPath, state);

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
		}
		return TRUE;
	case HWND_ACTIVE:
		{
			HWND hFocus = (HWND) lParam;
			DWORD i = 0;
			if(Find(&lpCamProp->cs, hFocus, &i)){
				lpCamProp->cActive = &lpCamProp->cs.c[i];
				OnCameraPropertiesChangeActive(lpCamProp);
			}
		}
		return TRUE;
	case HWND_DATAMANAGER:
		{
			lpCamProp->hDataManager = (HWND) lParam;
			SendMessage(hwnd, CPM_CAPTURE, NULL, NULL);
		}
		return TRUE;
	default:
		return TRUE;
	}

		return TRUE;
}

LRESULT OnCameraPropertiesCloseStream(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	//NEW
	lpCamProp->cActive->flgSave = FALSE;
	lpCamProp->cActive->flgStream = FALSE;
	lpCamProp->cActive->flgOpen = FALSE;
	//NEW
	switch(lpCamProp->camera)
	{
	case CAMERA_ZEISS_HSM:
		//lpCamProp->zeissProc.CloseCamera();
		//ThrowMessage(lpCamProp->hOwner, "Disconnected from Zeiss HSm");
		//lpCamProp->cActive->flgClose = TRUE;
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
		lpCamProp->flgStream = FALSE;
		Button_Enable(lpCamProp->hBtnSave, FALSE);
		SetWindowText(lpCamProp->hBtnStream, "Stream");
		lpCamProp->flgSave = FALSE;
		return TRUE;
	default:
		return TRUE;
	}
}

LRESULT OnCameraPropertiesClose(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	CAMERAPROPERTIES *lpCamProp = (CAMERAPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	SendMessage(lpCamProp->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_CAMERAPROPERTIES), NULL);

	if(lpCamProp->camera == CAMERA_ZEISS_HSM)
		lpCamProp->zeissProc.CloseCamera();

	return FALSE;
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

DWORD Find(CAPTURESET *cs, CHAR *strID, DWORD *index)
{
	if(!cs)return FALSE;
	if(!index)return FALSE;
	if(!cs->c)return FALSE;
	if(!strID)return FALSE;

	DWORD i = 0;
	for(i = 0; i < cs->nCapture; i++){
		if(!strcmp(strID, cs->c[i].strID)){
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


		////NEW
		//lpCamProp->cActive->flgSave = FALSE;
		//lpCamProp->cActive->flgStream = FALSE;
		//lpCamProp->cActive->flgOpen = FALSE;
		////NEW
		//switch(lpCamProp->camera)
		//{
		//case CAMERA_ZEISS_HSM:
		//	//lpCamProp->zeissProc.CloseCamera();
		//	//ThrowMessage(lpCamProp->hOwner, "Disconnected from Zeiss HSm");
		//	//lpCamProp->cActive->flgClose = TRUE;
		//	return TRUE;
		//case CAMERA_PIXELINK:
		//	PxLUninitialize(lpCamProp->hCamera);
		//	ThrowMessage(lpCamProp->hOwner, "Disconnected from PixeLINK-A781");
		//	return TRUE;
		//case CAMERA_PHOTONFOCUS:
		//	ThrowMessage(lpCamProp->hOwner, "Disconnected from PhotonFocus");
		//	return TRUE;
		//case CAMERA_EMULATE:
		//	ThrowMessage(lpCamProp->hOwner, "Disconnected from Emulate Camera");
		//	lpCamProp->flgStream = FALSE;
		//	Button_Enable(lpCamProp->hBtnSave, FALSE);
		//	SetWindowText(lpCamProp->hBtnStream, "Stream");
		//	lpCamProp->flgSave = FALSE;
		//	return TRUE;
		//default:
		//	return TRUE;
		//}
		//return TRUE;

//REMOVED: Slider Initialization
	//lpCamProp->hSlider = GetDlgItem(hwnd, IDC_SLIDER_EXPOSURE);
	//SendMessage(lpCamProp->hSlider, TBM_SETRANGEMIN, TRUE, 0);
	//SendMessage(lpCamProp->hSlider, TBM_SETRANGEMAX, TRUE, 500000);
	//lpCamProp->exposure = 15000;
	//SetDlgItemInt(hwnd, IDC_EDIT_EXPOSURE, lpCamProp->exposure, TRUE);
	//SendMessage(lpCamProp->hSlider, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) lpCamProp->exposure);

	//LRESULT OnCameraPropertiesCapture(CAMERAPROPERTIES *lpCamProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	//{
	//	HANDLE hThread = NULL;
	//	CAPTURE c = {0};
	//	CAPTURE *pc = NULL;
	//	c.hOwner = hwnd;
	//	c.flgStream = TRUE;
	//	c.exposure = lpCamProp->exposure;
	//	c.sampleRate = 1;
	//	c.frameRate = lpCamProp->frameRate;
	//	c.baseFrameRate = c.frameRate;
	//	c.flgOpen = TRUE;
	//	c.hDataManager = CreateDataManager(lpCamProp->hOwner, DS_FRAMEGRABBER);
	//
	//	Append(&lpCamProp->cs, c);
	//	pc = &lpCamProp->cs.c[lpCamProp->cs.nCapture-1];
	//
	//	switch(lpCamProp->camera)
	//	{
	//	case CAMERA_PHOTONFOCUS:
	//		hThread = CreateThread(NULL, NULL, PhotonFocusThread, (VOID *)pc, NULL, NULL);
	//		break;
	//	case CAMERA_EMULATE:
	//		hThread = CreateThread(NULL, NULL, EmulateCaptureStream, (VOID *)pc, NULL, NULL);
	//		break;
	//	case CAMERA_PIXELINK:
	//		hThread = CreateThread(NULL, NULL, PixeLINKThread, (VOID *)pc, NULL, NULL);
	//		break;
	//	case CAMERA_DESKTOP:
	//		hThread = CreateThread(NULL, NULL, DesktopCaptureThread, (VOID *)pc, NULL, NULL);
	//		break;
	//	case CAMERA_RACE:
	//		//hThread = CreateThread(NULL, NULL, RACECaptureThread, (VOID *)pc, NULL, NULL);
	//		break;
	//	default:
	//		break;
	//	}
	//	
	//	pc->hThread = hThread;
	//	
	//
	//		return TRUE;
	//}