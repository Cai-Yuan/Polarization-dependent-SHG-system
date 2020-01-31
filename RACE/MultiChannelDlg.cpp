#include "stdafx.h"

#include "MultiChannelDlg.h"
#include "ToolsWin32.h"
#include "ZeissMicroscope.h"
#include "ToolsWin32.h"
#include "CameraProperties.h"

INT_PTR CALLBACK MultiChannelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MULTICHANNELDLG *lpMultiChannelDlg = (MULTICHANNELDLG *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpMultiChannelDlg = (MULTICHANNELDLG *)calloc(1, sizeof(MULTICHANNELDLG));
		memset(lpMultiChannelDlg, 0x00, sizeof(MULTICHANNELDLG));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpMultiChannelDlg);
		lpMultiChannelDlg = (MULTICHANNELDLG *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpMultiChannelDlg == NULL){
			ThrowMessage(hwnd, "WndAUTOFOCUSTAB: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndAUTOFOCUSTAB: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpMultiChannelDlg->hOwner = (HWND) psp.lParam;

		lpMultiChannelDlg->hBtnBrightField = GetDlgItem(hwnd, IDC_BTN_BRIGHTFIELD);
		lpMultiChannelDlg->hBtnCY5 = GetDlgItem(hwnd, IDC_BTN_CY5);
		lpMultiChannelDlg->hBtnDapi = GetDlgItem(hwnd, IDC_BTN_DAPI);
		lpMultiChannelDlg->hBtnFitc = GetDlgItem(hwnd, IDC_BTN_FITC);
		lpMultiChannelDlg->hBtnRhodamine = GetDlgItem(hwnd, IDC_BTN_RHODAMINE);
		lpMultiChannelDlg->hBtnMeasure = GetDlgItem(hwnd, IDC_BTN_MEASURE);

		lpMultiChannelDlg->hCheckBrightField = GetDlgItem(hwnd, IDC_CHECK_BRIGHTFIELD);
		lpMultiChannelDlg->hCheckCY5 = GetDlgItem(hwnd, IDC_CHECK_CY5);
		lpMultiChannelDlg->hCheckDapi = GetDlgItem(hwnd, IDC_CHECK_DAPI);
		lpMultiChannelDlg->hCheckFitc = GetDlgItem(hwnd, IDC_CHECK_FITC);
		lpMultiChannelDlg->hCheckRhodamine = GetDlgItem(hwnd, IDC_CHECK_RHODAMINE);

		lpMultiChannelDlg->hEditExposure = GetDlgItem(hwnd, IDC_EDIT_EXPOSURE);

		//Initialize Bright Field as selected filter
		lpMultiChannelDlg->hBtnSelect = lpMultiChannelDlg->hBtnBrightField;

		//Apply 'Pushed' Visual Effects to Selected Fitler
		Button_SetState(lpMultiChannelDlg->hBtnSelect, TRUE);

		Button_SetCheck(lpMultiChannelDlg->hCheckBrightField, TRUE);

		Button_SetStyle(lpMultiChannelDlg->hBtnBrightField, BS_DEFPUSHBUTTON, TRUE);

		Button_Enable(lpMultiChannelDlg->hBtnCY5, FALSE);
		Button_Enable(lpMultiChannelDlg->hBtnDapi, FALSE);
		Button_Enable(lpMultiChannelDlg->hBtnFitc, FALSE);
		Button_Enable(lpMultiChannelDlg->hBtnRhodamine, FALSE);

		lpMultiChannelDlg->fBrightField = TRUE;
		lpMultiChannelDlg->fCY5 = FALSE;
		lpMultiChannelDlg->fDapi = FALSE;
		lpMultiChannelDlg->fFitc = FALSE;
		lpMultiChannelDlg->fRhodamine = FALSE;

		lpMultiChannelDlg->hZeissMicroscope = (HWND) SendMessage(lpMultiChannelDlg->hOwner, WM_GETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, NULL);
		SendMessage(lpMultiChannelDlg->hZeissMicroscope, WM_SETHWND, HWND_MULTICHANNEL, (LPARAM) hwnd);

		//Select Brightfield as light path
		SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNEL, CHNL_BRIGHTFIELD, NULL);
		lpMultiChannelDlg->dwCurrentChannel = CHNL_BRIGHTFIELD;

		//Initialize Channel Masks
		memset(&lpMultiChannelDlg->mc, NULL, sizeof(MULTICHANNEL));
		lpMultiChannelDlg->mc.filter = MC_TRANSLIGHT;
		SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);

		//Get HWND to CameraProperties Dialog Box
		lpMultiChannelDlg->hCameraProperties = (HWND) SendMessage(lpMultiChannelDlg->hOwner,
															WM_GETHWND,
															HWND_CAMERAPROPERTIES,
															NULL);
		break;
	case WM_COMMAND:

		switch(HIWORD(wParam))
		{
		case BN_CLICKED:

			switch(LOWORD(wParam))
			{
			case IDC_BTN_BRIGHTFIELD:
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNEL, CHNL_BRIGHTFIELD, NULL);

				Button_SetState( (HWND) lParam, TRUE);

				if(lpMultiChannelDlg->hBtnSelect != (HWND) lParam)
					Button_SetState(lpMultiChannelDlg->hBtnSelect, FALSE);

				if(lpMultiChannelDlg->mc.expBrightField){
					SendMessage(lpMultiChannelDlg->hCameraProperties, 
								CPM_SETEXPOSURE, 
								CAMERA_ZEISS_MRM, 
								(LPARAM)lpMultiChannelDlg->mc.expBrightField);
					sprintf(str, "%d", lpMultiChannelDlg->mc.expBrightField);
					Edit_SetText(lpMultiChannelDlg->hEditExposure, str);
				}

				lpMultiChannelDlg->hBtnSelect = (HWND) lParam;
				lpMultiChannelDlg->dwCurrentChannel = CHNL_BRIGHTFIELD;

				break;
			case IDC_BTN_CY5:
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNEL, CHNL_CY5, NULL);

				Button_SetState( (HWND) lParam, TRUE);

				if(lpMultiChannelDlg->hBtnSelect != (HWND) lParam)
					Button_SetState(lpMultiChannelDlg->hBtnSelect, FALSE);

				if(lpMultiChannelDlg->mc.expCY5){
					SendMessage(lpMultiChannelDlg->hCameraProperties, 
								CPM_SETEXPOSURE, 
								CAMERA_ZEISS_MRM, 
								(LPARAM)lpMultiChannelDlg->mc.expCY5);
					sprintf(str, "%d", lpMultiChannelDlg->mc.expCY5);
					Edit_SetText(lpMultiChannelDlg->hEditExposure, str);
				}

				lpMultiChannelDlg->hBtnSelect = (HWND) lParam;
				lpMultiChannelDlg->dwCurrentChannel = CHNL_CY5;

				break;
			case IDC_BTN_DAPI:
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNEL, CHNL_DAPI, NULL);

				Button_SetState( (HWND) lParam, TRUE);

				if(lpMultiChannelDlg->hBtnSelect != (HWND) lParam)
					Button_SetState(lpMultiChannelDlg->hBtnSelect, FALSE);

				if(lpMultiChannelDlg->mc.expDAPI){
					SendMessage(lpMultiChannelDlg->hCameraProperties, 
								CPM_SETEXPOSURE, 
								CAMERA_ZEISS_MRM,
								(WPARAM)lpMultiChannelDlg->mc.expDAPI);
					sprintf(str, "%d", lpMultiChannelDlg->mc.expDAPI);
					Edit_SetText(lpMultiChannelDlg->hEditExposure, str);
				}


				lpMultiChannelDlg->hBtnSelect = (HWND) lParam;
				lpMultiChannelDlg->dwCurrentChannel = CHNL_DAPI;

				break;
			case IDC_BTN_FITC:
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNEL, CHNL_FITC, NULL);

				Button_SetState( (HWND) lParam, TRUE);

				if(lpMultiChannelDlg->hBtnSelect != (HWND) lParam)
					Button_SetState(lpMultiChannelDlg->hBtnSelect, FALSE);

				if(lpMultiChannelDlg->mc.expFITC){
					SendMessage(lpMultiChannelDlg->hCameraProperties, 
								CPM_SETEXPOSURE, 
								CAMERA_ZEISS_MRM, 
								(LPARAM)lpMultiChannelDlg->mc.expFITC);
					sprintf(str, "%d", lpMultiChannelDlg->mc.expFITC);
					Edit_SetText(lpMultiChannelDlg->hEditExposure, str);
				}

				lpMultiChannelDlg->hBtnSelect = (HWND) lParam;
				lpMultiChannelDlg->dwCurrentChannel = CHNL_FITC;

				break;
			case IDC_BTN_RHODAMINE:
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNEL, CHNL_RHODAMINE, NULL);

				Button_SetState( (HWND) lParam, TRUE);

				if(lpMultiChannelDlg->hBtnSelect != (HWND) lParam)
					Button_SetState(lpMultiChannelDlg->hBtnSelect, FALSE);

				if(lpMultiChannelDlg->mc.expRhodamine){
					SendMessage(lpMultiChannelDlg->hCameraProperties, 
								CPM_SETEXPOSURE, 
								CAMERA_ZEISS_MRM, 
								(LPARAM)lpMultiChannelDlg->mc.expRhodamine);
					sprintf(str, "%d", lpMultiChannelDlg->mc.expRhodamine);
					Edit_SetText(lpMultiChannelDlg->hEditExposure, str);
				}

				lpMultiChannelDlg->hBtnSelect = (HWND) lParam;
				lpMultiChannelDlg->dwCurrentChannel = CHNL_RHODAMINE;

				break;
			case IDC_CHECK_BRIGHTFIELD:

				lpMultiChannelDlg->fBrightField = !lpMultiChannelDlg->fBrightField;
				Button_Enable(lpMultiChannelDlg->hBtnBrightField, lpMultiChannelDlg->fBrightField);
				Button_SetState(lpMultiChannelDlg->hBtnBrightField, FALSE);
				lpMultiChannelDlg->mc.filter = UpdateCheckBoxFilter(lpMultiChannelDlg->hCheckBrightField, 
																	lpMultiChannelDlg->mc.filter, 
																	MC_TRANSLIGHT);
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);
				break;
			case IDC_CHECK_CY5:

				lpMultiChannelDlg->fCY5 = !lpMultiChannelDlg->fCY5;
				Button_Enable(lpMultiChannelDlg->hBtnCY5, lpMultiChannelDlg->fCY5);
				Button_SetState(lpMultiChannelDlg->hBtnCY5, FALSE);
				lpMultiChannelDlg->mc.filter = UpdateCheckBoxFilter(lpMultiChannelDlg->hCheckCY5, 
																	lpMultiChannelDlg->mc.filter, 
																	MC_CY5);
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);
				break;
			case IDC_CHECK_DAPI:

				lpMultiChannelDlg->fDapi = !lpMultiChannelDlg->fDapi;
				Button_Enable(lpMultiChannelDlg->hBtnDapi, lpMultiChannelDlg->fDapi);
				Button_SetState(lpMultiChannelDlg->hBtnDapi, FALSE);
				lpMultiChannelDlg->mc.filter = UpdateCheckBoxFilter(lpMultiChannelDlg->hCheckDapi, 
																	lpMultiChannelDlg->mc.filter, 
																	MC_DAPI);
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);
				//UpdateChannelExposure(lpMultiChannelDlg);
				break;
			case IDC_CHECK_FITC:

				lpMultiChannelDlg->fFitc = !lpMultiChannelDlg->fFitc;
				Button_Enable(lpMultiChannelDlg->hBtnFitc, lpMultiChannelDlg->fFitc);
				Button_SetState(lpMultiChannelDlg->hBtnFitc, FALSE);
				lpMultiChannelDlg->mc.filter = UpdateCheckBoxFilter(lpMultiChannelDlg->hCheckFitc, 
																	lpMultiChannelDlg->mc.filter, 
																	MC_FITC);
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);
				//UpdateChannelExposure(lpMultiChannelDlg);
				break;
			case IDC_CHECK_RHODAMINE:

				lpMultiChannelDlg->fRhodamine = !lpMultiChannelDlg->fRhodamine;
				Button_Enable(lpMultiChannelDlg->hBtnRhodamine, lpMultiChannelDlg->fRhodamine);
				Button_SetState(lpMultiChannelDlg->hBtnRhodamine, FALSE);
				lpMultiChannelDlg->mc.filter = UpdateCheckBoxFilter(lpMultiChannelDlg->hCheckRhodamine, 
																	lpMultiChannelDlg->mc.filter, 
																	MC_RHODAMINE);
				SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);
				//UpdateChannelExposure(lpMultiChannelDlg);
				break;
			case IDC_BTN_MEASURE:
				{
					//DWORD exposure = NULL;
					//exposure = (DWORD) SendMessage(lpMultiChannelDlg->hCameraProperties, CPM_GETEXPOSURE, CAMERA_ZEISS_MRM, NULL);
					//sprintf(str, "%dus", exposure);
					//Edit_SetText(lpMultiChannelDlg->hEditExposure, str);

					//switch(lpMultiChannelDlg->dwCurrentChannel){
					//case CHNL_BRIGHTFIELD:
					//	lpMultiChannelDlg->mc.expBrightField = exposure;
					//	break;
					//case CHNL_CY5:
					//	lpMultiChannelDlg->mc.expCY5 = exposure;
					//	break;
					//case CHNL_DAPI:
					//	lpMultiChannelDlg->mc.expDAPI = exposure;
					//	break;
					//case CHNL_FITC:
					//	lpMultiChannelDlg->mc.expFITC = exposure;
					//	break;
					//case CHNL_RHODAMINE:
					//	lpMultiChannelDlg->mc.expRhodamine = exposure;
					//default:
					//	break;
					//}

					UpdateChannelExposure(lpMultiChannelDlg);
					SendMessage(lpMultiChannelDlg->hZeissMicroscope, ZSM_SETCHANNELMASK, NULL, (LPARAM) &lpMultiChannelDlg->mc);
				}
				break;
			default:
				break;
			}

			break;
		default:
			break;
		}
		break;


	case WM_NOTIFY:
		{
			PSHNOTIFY pshNotify = {0};
			memcpy(&pshNotify, (VOID *)lParam, sizeof(PSHNOTIFY));

			INT i = 0;
			switch(pshNotify.hdr.code)
			{
			case PSN_APPLY:
				return DialogReturn(hwnd, PSNRET_INVALID_NOCHANGEPAGE);
			case PSN_QUERYCANCEL:
					SendMessage(lpMultiChannelDlg->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);
				return DialogReturn(hwnd, PSNRET_INVALID);
			case PSN_KILLACTIVE:
				return DialogReturn(hwnd, FALSE);
			case PSN_SETACTIVE:
				return TRUE;
			default:
				break;
			}

		}
		return (LRESULT) FALSE;
	case WM_SETHWND:
		if(wParam == HWND_OWNER){
			lpMultiChannelDlg->hOwner = (HWND) lParam;
		}
		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD UpdateChannelExposure(MULTICHANNELDLG *lpMultiChannelDlg)
{
	DWORD exposure = NULL;
	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	exposure = (DWORD) SendMessage(lpMultiChannelDlg->hCameraProperties, CPM_GETEXPOSURE, CAMERA_ZEISS_MRM, NULL);
	sprintf(str, "%d", exposure);
	Edit_SetText(lpMultiChannelDlg->hEditExposure, str);

	switch(lpMultiChannelDlg->dwCurrentChannel){
		case CHNL_BRIGHTFIELD:
			lpMultiChannelDlg->mc.expBrightField = exposure;
			break;
		case CHNL_CY5:
			lpMultiChannelDlg->mc.expCY5 = exposure;
			break;
		case CHNL_DAPI:
			lpMultiChannelDlg->mc.expDAPI = exposure;
			break;
		case CHNL_FITC:
			lpMultiChannelDlg->mc.expFITC = exposure;
			break;
		case CHNL_RHODAMINE:
			lpMultiChannelDlg->mc.expRhodamine = exposure;
			break;
		default:
			break;
	}
	
	return exposure;
}