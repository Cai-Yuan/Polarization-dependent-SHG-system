#include "stdafx.h"

#include "ExperimentOptions.h"
#include "GetFolderPath.h"
#include "ZeissMicroscope.h"


INT_PTR CALLBACK ExperimentOptionsProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EXPERIMENTOPTIONS *lpExperimentOptions = (EXPERIMENTOPTIONS *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		lpExperimentOptions = (EXPERIMENTOPTIONS *)calloc(1, sizeof(EXPERIMENTOPTIONS));
		memset(lpExperimentOptions, 0x00, sizeof(EXPERIMENTOPTIONS));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpExperimentOptions);
		lpExperimentOptions = (EXPERIMENTOPTIONS *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpExperimentOptions == NULL){
			ThrowMessage(hwnd, "WndExperimentOptions: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndExperimentOptions: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpExperimentOptions->hOwner = (HWND) psp.lParam;

		memset(lpExperimentOptions->strPath, NULL, MAX_STRING*sizeof(CHAR));
		
		lpExperimentOptions->hButtonStart = GetDlgItem(hwnd, IDC_BTN_START);
		lpExperimentOptions->hCheckAutoFocus = GetDlgItem(hwnd, IDC_CHECK_AUTOFOCUS);
		lpExperimentOptions->hCheckMosaic = GetDlgItem(hwnd, IDC_CHECK_MOSAIC);
		lpExperimentOptions->hCheckZStack = GetDlgItem(hwnd, IDC_CHECK_ZSTACK);
		lpExperimentOptions->hCheckMultiChannel = GetDlgItem(hwnd, IDC_CHECK_MULTICHANNEL);
		lpExperimentOptions->hCheckTimeLapse = GetDlgItem(hwnd, IDC_CHECK_TIMELAPSE);

		lpExperimentOptions->hEditPath = GetDlgItem(hwnd, IDC_EDIT_PATH);

		Button_Enable(lpExperimentOptions->hCheckTimeLapse, FALSE);

		lpExperimentOptions->hZeissMicroscope = (HWND) SendMessage(lpExperimentOptions->hOwner, WM_GETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, NULL);
		SendMessage(lpExperimentOptions->hZeissMicroscope, WM_SETHWND, HWND_EXPERIMENT, (LPARAM) hwnd);

		break;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			{
				DWORD state = (DWORD) SendMessage(lpExperimentOptions->hCheckMosaic, BM_GETCHECK, NULL, NULL);
				state = state;
			}
			break;
		case IDC_EDIT_PATH:
			{
				if(LOWORD(wParam) == IDC_EDIT_PATH){
					Edit_GetText(lpExperimentOptions->hEditPath, lpExperimentOptions->strPath, MAX_STRING);
					SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_SETFILEPATH, MAKEWPARAM(MAX_STRING, NULL), (LPARAM) lpExperimentOptions->strPath);
				}

			}
		default:
			break;
		}
		//Control Identifier
		switch(LOWORD(wParam))
		{
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
					memcpy(lpExperimentOptions->strPath, str, MAX_STRING*sizeof(CHAR));
					Edit_SetText(lpExperimentOptions->hEditPath, lpExperimentOptions->strPath);
					SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_SETFILEPATH, MAKEWPARAM(MAX_STRING, NULL), (LPARAM) lpExperimentOptions->strPath);
				}else{
					memset(lpExperimentOptions->strPath, NULL, MAX_STRING*sizeof(CHAR));
				}

				Edit_SetText(lpExperimentOptions->hEditPath, lpExperimentOptions->strPath);
			}
			break;
		case IDC_BTN_START:
			{

				SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_ACQUISITION, (WPARAM) NULL, NULL);
				return TRUE;

				if(!lpExperimentOptions->strPath[0]){
					MessageBox(hwnd, "Please Select a folder path", "Error!", MB_OK);
				}
				else{
					//SendMessage to AxioVert to being acquisition
					
					if(!lpExperimentOptions->ze.flgRun){
						Button_SetText(lpExperimentOptions->hButtonStart, "Stop");
						SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_STARTEXPERIMENT, NULL, NULL);
					}
					else{
						Button_SetText(lpExperimentOptions->hButtonStart, "Start");
						SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_STOPEXPERIMENT, NULL, NULL);
					}
					lpExperimentOptions->ze.flgRun = !lpExperimentOptions->ze.flgRun;
				}

			}
			break;
		case IDC_CHECK_MOSAIC:
			{
				//DWORD state = (DWORD) SendMessage(lpExperimentOptions->hCheckMosaic, BM_GETCHECK, NULL, NULL);

				if(Button_GetCheck(lpExperimentOptions->hCheckMosaic)){
					lpExperimentOptions->ze.flgMosaic = TRUE;
				}
				else
					lpExperimentOptions->ze.flgMosaic = FALSE;
			
				SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_SETEXPERIMENT, (WPARAM) &lpExperimentOptions->ze, NULL);
			}
			break;
		case IDC_CHECK_AUTOFOCUS:
			{
				if(SendMessage(lpExperimentOptions->hCheckAutoFocus, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					SendMessage(lpExperimentOptions->hCheckZStack, BM_SETCHECK, BST_UNCHECKED, NULL);

				if(Button_GetCheck(lpExperimentOptions->hCheckAutoFocus)){
					lpExperimentOptions->ze.flgAutoFocus = TRUE;
					lpExperimentOptions->ze.flgZStack = FALSE;
				}
				else{
					lpExperimentOptions->ze.flgAutoFocus = FALSE;
				}

				SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_SETEXPERIMENT, (WPARAM) &lpExperimentOptions->ze, NULL);
			}
			break;
		case IDC_CHECK_ZSTACK:
			{
				if(SendMessage(lpExperimentOptions->hCheckZStack, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					SendMessage(lpExperimentOptions->hCheckAutoFocus, BM_SETCHECK, BST_UNCHECKED, NULL);

				if(Button_GetCheck(lpExperimentOptions->hCheckZStack)){
					lpExperimentOptions->ze.flgZStack = TRUE;
					lpExperimentOptions->ze.flgAutoFocus = FALSE;
				}
				else
					lpExperimentOptions->ze.flgZStack = FALSE;

				SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_SETEXPERIMENT, (WPARAM) &lpExperimentOptions->ze, NULL);
			}
			break;
		case IDC_CHECK_MULTICHANNEL:
			{
				if(Button_GetCheck(lpExperimentOptions->hCheckMultiChannel)){
					lpExperimentOptions->ze.flgMultiChannel = TRUE;
				}
				else
					lpExperimentOptions->ze.flgMultiChannel = FALSE;
			
				SendMessage(lpExperimentOptions->hZeissMicroscope, ZSM_SETEXPERIMENT, (WPARAM) &lpExperimentOptions->ze, NULL);
			}
			break;
		default:
			break;
		}

		break;
	case ZSM_STOPEXPERIMENT:
		lpExperimentOptions->ze.flgRun = FALSE;
		Button_SetText(lpExperimentOptions->hButtonStart, "Start");
		break;
	case WM_SETHWND:

		if(wParam == HWND_OWNER)
			lpExperimentOptions->hOwner = (HWND) lParam;

		return TRUE;
	case WM_CLOSE:

		SendMessage(lpExperimentOptions->hOwner, WM_COMMAND, LOWORD(ID_VIEW_PROPERTYMANAGER), NULL);

		return TRUE;
	case WM_NOTIFY:
		{
			PSHNOTIFY pshNotify = {0};
			memcpy(&pshNotify, (VOID *)lParam, sizeof(PSHNOTIFY));

			INT i = 0;
			switch(pshNotify.hdr.code)
			{
			case PSN_APPLY:
				return DialogReturn(hwnd, PSNRET_INVALID_NOCHANGEPAGE); /*PSNRET_NOERROR*/
			case PSN_QUERYCANCEL:
					SendMessage(lpExperimentOptions->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_ZEISSMICROSCOPE), NULL);

					RECT rc;
					GetWindowRect(pshNotify.hdr.hwndFrom, &rc);
					rc.left = rc.left;
					//ShowWindow(pshNotify.hdr.hwndFrom, SW_HIDE);
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
	default:
		return FALSE;
	}


	return FALSE;

}