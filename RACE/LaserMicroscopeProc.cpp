#include "stdafx.h"
#include "LaserMicroscope.h"
#include "AutoFocusProc.h"


LRESULT CALLBACK LaserMicroscopeProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LASERMICROSCOPE *lpWnd = (LASERMICROSCOPE *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_CREATE:
		break;
	case WM_INITMICROSCOPE:
		{
			lpWnd->flgThread = TRUE;
			CreateThread(NULL, NULL, LaserMicroscopeThread, (LPVOID) lpWnd, NULL, NULL);
			return TRUE;
		}
	case WM_NCCREATE:
		{
			lpWnd = (LASERMICROSCOPE *) InitWindowLongRACE(hwnd, sizeof(LASERMICROSCOPE));
			lpWnd->strFolder = (CHAR *)malloc(MAX_STRING*sizeof(CHAR));
			if(lpWnd->strFolder){
				sprintf(lpWnd->strFolder, "C:\\Archive\\ZStack\\");
				CreateDirectory(lpWnd->strFolder, NULL);
				lpWnd->stepSize = 1.0;//um
				lpWnd->flgZStack = FALSE;
				lpWnd->slice = 0;
				lpWnd->nSlice = 25;
			}
		}
		break;
	case WM_SETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			lpWnd->hOwner = (HWND)lParam;
			//lpWnd->hCamProp = (HWND) SendMessage(lpWnd->hOwner, WM_GETHWND, HWND_CAMERAPROPERTIES, NULL);
			//SendMessage(lpWnd->hCamProp, WM_CREATELINK, (WPARAM)hwnd, NULL);
			return TRUE;
		case HWND_SYSTEM:
			lpWnd->hSystem = (HWND)lParam;
			lpWnd->lpSystem = (LM_SYSTEM *)GetWindowLongPtr(lpWnd->hSystem, GWLP_USERDATA);
			if(!lpWnd->lpSystem){
				MessageBox(NULL, "lpLaserMicroscope->lpSystem = NULL", "Error!", MB_OK);
			}
			return TRUE;
		case HWND_STAGE:
			if(!lpWnd->hStage){
				lpWnd->hStage = (HWND)lParam;
				lpWnd->lpStage = (LM_STAGE *)GetWindowLongPtr(lpWnd->hStage, GWLP_USERDATA);
				if(!lpWnd->lpStage){
					MessageBox(NULL, "lpLaserMicroscope->lpStage = NULL", "Error!", MB_OK);
				}
			}
			return TRUE;
		case HWND_DEVICE:
			if(!lpWnd->hDevice){
				lpWnd->hDevice = (HWND)lParam;
				lpWnd->lpDevice = (LM_DEVICE *)GetWindowLongPtr(lpWnd->hDevice, GWLP_USERDATA);
				if(!lpWnd->lpDevice){
					MessageBox(NULL, "lpLaserMicroscope->lpDevice = NULL", "Error!", MB_OK);
				}
			}
			return TRUE;
		case HWND_DISPLAY:
			if(!lpWnd->hDisplay){
				lpWnd->hDisplay = (HWND) lParam;
				lpWnd->lpDisplay = (LM_DISPLAY *)GetWindowLongPtr(lpWnd->hDisplay, GWLP_USERDATA);
				if(!lpWnd->lpDisplay){
					MessageBox(NULL, "lpLaserMicroscope->lpDisplay = NULL", "Error!", MB_OK);
				}
			}
			return TRUE;
		case HWND_DATAVIEW:
			SendMessage(lpWnd->hDisplay, msg, wParam, lParam);
			SendMessage(lpWnd->hTracker, msg, wParam, lParam);
			return TRUE;
		case HWND_TRACKER:
			if(!lpWnd->hTracker){
				lpWnd->hTracker = (HWND) lParam;
				lpWnd->lpTracker = (LM_TRACKER *)GetWindowLongPtr(lpWnd->hTracker, GWLP_USERDATA);
				if(!lpWnd->lpDisplay){
					MessageBox(NULL, "lpLaserMicroscope->lpDisplay = NULL", "Error!", MB_OK);
				}
			}
			return TRUE;
		case HWND_AUTOFOCUS:
			if(!lpWnd->hAutoFocus){
				lpWnd->hAutoFocus = (HWND) lParam;
				lpWnd->lpAutoFocus = (LM_AUTOFOCUS *)GetWindowUserData(lpWnd->hAutoFocus);
				if(!lpWnd->lpAutoFocus){
					MessageBox(NULL, "lpLaserMicroscope->lpAutoFocus = NULL", "Error!", MB_OK);
				}
			}
			return TRUE;
		case HWND_CAMERAPROPERTIES:
			lpWnd->hCamProp = (HWND) SendMessage(lpWnd->hOwner, WM_GETHWND, HWND_CAMERAPROPERTIES, NULL);
			SendMessage(lpWnd->hCamProp, WM_CREATELINK, (WPARAM)hwnd, NULL);
		case HWND_FRAME:
			return SendMessage(lpWnd->hOwner, msg, wParam, lParam);
		default:
			return FALSE;
		}
	
		return FALSE;
	case WM_DATAIN:
		{
			PDMFRAME *pdmf;
			PDMFRAME *pdmfWnd;
			pdmf = (PDMFRAME *) wParam;
			pdmfWnd = (PDMFRAME *)&lpWnd->pdmf;

			SendMessage(lpWnd->hTracker, msg, wParam, lParam);

			if(!pdmfWnd->lpvoid){
				AllocPDMFrame(pdmfWnd, pdmf->pdmfh.width, pdmf->pdmfh.height, DT_CHAR);
				lpWnd->pdmf.pdmfh.typeMetaData = MD_FRAME;
			}

			if(!SameSize(pdmfWnd->pdmfh, pdmf->pdmfh)){
				//Free
				//Alloc to New Size
			}

			CopyPDMFrame(pdmfWnd, pdmf);

			lpWnd->flgFrameIn = TRUE;
		}
		return TRUE;
	case WM_DATADROP:
		{
			PDMFRAME *pdmf;
			PDMFRAME *pdmfWnd;
			pdmf = (PDMFRAME *) wParam;
			pdmfWnd = (PDMFRAME *)&lpWnd->pdmf;
			if(!lpWnd->pdmf.lpvoid){
				AllocPDMFrame(pdmfWnd, pdmf->pdmfh.width, pdmf->pdmfh.height, pdmf->pdmfh.typeData);
			}
			CopyPDMFrame(&lpWnd->pdmf, (PDMFRAME *) wParam);
			lpWnd->flgFrameIn = TRUE;
			PostMessage(lpWnd->lpTracker->hParticleTracker, msg, wParam, lParam);
			return TRUE;
		}
	case WM_ZSTACK:
		{
			lpWnd->flgZStack = !lpWnd->flgZStack;
			return TRUE;
		}
	case WM_GETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			return (LRESULT)lpWnd->hOwner;
		case HWND_CAMERAPROPERTIES:
			return (LRESULT)lpWnd->hCamProp;
		case HWND_DATAVIEW:
			return (LRESULT)lpWnd->hDataView;
		case HWND_DISPLAY:
			return (LRESULT)lpWnd->hDisplay;
		case HWND_TRACKER:
			return (LRESULT)lpWnd->hTracker;
		case HWND_AUTOFOCUS:
			return (LRESULT)lpWnd->hAutoFocus;
		case HWND_SYSTEM:
			return (LRESULT)lpWnd->hSystem;
		default:
			return (LRESULT)SendMessage(lpWnd->hOwner, msg, wParam, lParam);
		}
		return FALSE;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


	return DefWindowProc(hwnd, msg, wParam, lParam);

}
