#include "stdafx.h"

#include "StageProperties.h"
#include "ZeissMicroscope.h"
#include "GetFolderPath.h"
#include "Point.h"

INT_PTR CALLBACK StagePropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	STAGEPROPERTIES *lpStageProp = (STAGEPROPERTIES *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpStageProp = (STAGEPROPERTIES *)calloc(1, sizeof(STAGEPROPERTIES));
		memset(lpStageProp, 0x00, sizeof(STAGEPROPERTIES));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpStageProp);
		lpStageProp = (STAGEPROPERTIES *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(!lpStageProp){
			ThrowMessage(hwnd, "DlgStageProperties: Error Getting Long Ptr");
			ThrowMessage(hwnd, "DlgStageProperties: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpStageProp->hButtonZUp = GetDlgItem(hwnd, IDC_BUTTON_ZUP);
		lpStageProp->hButtonZDown = GetDlgItem(hwnd, IDC_BUTTON_ZDOWN);
		lpStageProp->hButtonAutoFocus = GetDlgItem(hwnd, IDC_BTN_AUTOFOCUS);		
		lpStageProp->hButtonGoTo = GetDlgItem(hwnd, IDC_BTN_GOTO);
		lpStageProp->hButtonGoToZ = GetDlgItem(hwnd, IDC_BTN_GOTOZ);
		lpStageProp->hButtonStart = GetDlgItem(hwnd, IDC_BTN_START);
		lpStageProp->hButtonBrowse = GetDlgItem(hwnd, IDC_BTN_BROWSE);

		lpStageProp->hEditGoTo = GetDlgItem(hwnd, IDC_EDIT_ZPOS);
		lpStageProp->hEditVelocity = GetDlgItem(hwnd, IDC_EDIT_VELOCITY);
		lpStageProp->hEditDelay = GetDlgItem(hwnd, IDC_EDIT_DELAY);
		lpStageProp->hEditXPos = GetDlgItem(hwnd, IDC_EDIT_XPOS);
		lpStageProp->hEditYPos = GetDlgItem(hwnd, IDC_EDIT_YPOS);
		lpStageProp->hEditZPos = GetDlgItem(hwnd, IDC_EDIT_ZPOS);
		lpStageProp->hEditFNStep = GetDlgItem(hwnd, IDC_EDIT_FNSTEP);
		lpStageProp->hEditFStep = GetDlgItem(hwnd, IDC_EDIT_FSTEP);
		lpStageProp->hEditZStep = GetDlgItem(hwnd, IDC_EDIT_ZSTEP);
		lpStageProp->hEditRow = GetDlgItem(hwnd, IDC_EDIT_ROW);
		lpStageProp->hEditCol = GetDlgItem(hwnd, IDC_EDIT_COL);
		lpStageProp->hEditOverlap = GetDlgItem(hwnd, IDC_EDIT_OVERLAP);
		lpStageProp->hEditXYVelocity = GetDlgItem(hwnd, IDC_EDIT_XYVELOCITY);
		lpStageProp->hEditPath = GetDlgItem(hwnd, IDC_EDIT_PATH);

		lpStageProp->hRadioRecall = GetDlgItem(hwnd, IDC_RADIO_RECALL);

		lpStageProp->zAxisVelocity = 20000;
		lpStageProp->zAxisDelay = 50;

		lpStageProp->xyAxisVelocity = 10000;

		lpStageProp->xAxisPos = NULL;
		lpStageProp->yAxisPos = NULL;
		lpStageProp->zAxisPos = NULL;

		lpStageProp->xAxisGoTo = NULL;
		lpStageProp->yAxisGoTo = NULL;
		lpStageProp->zAxisGoTo = NULL;

		lpStageProp->fNumStep = 10;
		lpStageProp->fStep = 50;

		lpStageProp->zStep = 100;

		lpStageProp->mfov.nRow = 2;
		lpStageProp->mfov.nCol = 2;
		lpStageProp->mfov.overlap = 0.1;

		lpStageProp->flgFocusRecall = TRUE;

		sprintf(lpStageProp->strPath, "C:\\Documents and Settings\\204\\My Documents\\RACE\\");
		Edit_SetText(lpStageProp->hEditPath, lpStageProp->strPath);

		SetDlgItemInt(hwnd, IDC_EDIT_VELOCITY, lpStageProp->zAxisVelocity, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_DELAY, lpStageProp->zAxisDelay, TRUE);

		SetDlgItemInt(hwnd, IDC_EDIT_XPOS, lpStageProp->xAxisPos, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_YPOS, lpStageProp->yAxisPos, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_ZPOS, lpStageProp->zAxisPos, TRUE);

		SetDlgItemInt(hwnd, IDC_EDIT_XGOTO, lpStageProp->xAxisGoTo, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_YGOTO, lpStageProp->yAxisGoTo, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_ZGOTO, lpStageProp->zAxisGoTo, TRUE);

		SetDlgItemInt(hwnd, IDC_EDIT_ZSTEP, lpStageProp->zStep, TRUE);

		SetDlgItemInt(hwnd, IDC_EDIT_FNSTEP, lpStageProp->fNumStep, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_FSTEP, lpStageProp->fStep, TRUE);

		SetDlgItemInt(hwnd, IDC_EDIT_ROW, lpStageProp->mfov.nRow, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_COL, lpStageProp->mfov.nCol, TRUE);

		SetDlgItemInt(hwnd, IDC_EDIT_XYVELOCITY, lpStageProp->xyAxisVelocity, TRUE);

		Button_SetCheck(lpStageProp->hRadioRecall, BST_CHECKED);
		
		sprintf(str, "%2.2f", lpStageProp->mfov.overlap);
		Edit_SetText(lpStageProp->hEditOverlap, str);


		break;

	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			
			if(LOWORD(wParam) == IDC_BUTTON_ZUP){
				//Objective Up
				PostMessage(lpStageProp->hZeissScope, ZSM_SETPOS, ZAXIS, lpStageProp->zAxisPos+lpStageProp->zStep);
				lpStageProp->zAxisPos = lpStageProp->zAxisPos+lpStageProp->zStep;
				return TRUE;
			}
			
			if(LOWORD(wParam) == IDC_BUTTON_ZDOWN){
				//Objective Down
				PostMessage(lpStageProp->hZeissScope, ZSM_SETPOS, ZAXIS, lpStageProp->zAxisPos-lpStageProp->zStep);
				lpStageProp->zAxisPos = lpStageProp->zAxisPos-lpStageProp->zStep;
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_BUTTON_INIT){
				//Does Not Work Correctly (But Close)
				//PostMessage(lpStageProp->hZeissScope, ZSM_BEGINBOOT, NULL, NULL);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_BTN_AUTOFOCUS){
				INT nStep;
				INT sStep;

				nStep = lpStageProp->fNumStep;
				sStep = lpStageProp->fStep;
				PostMessage(lpStageProp->hZeissScope, ZSM_AUTOFOCUS, MAKEWPARAM(nStep, sStep), NULL);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_BTN_GOTO){
				SendMessage(lpStageProp->hZeissScope, ZSM_SETPOS, XAXIS|YAXIS|ZAXIS, (LPARAM)&Point3D(lpStageProp->xAxisGoTo,
																						lpStageProp->yAxisGoTo,
																						lpStageProp->zAxisGoTo));
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_BTN_GOTOZ){
				SendMessage(lpStageProp->hZeissScope, ZSM_SETPOS, ZAXIS, (LPARAM) lpStageProp->zAxisGoTo);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_BTN_UPDATE){
				POINT3D p3D;
				SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, XAXIS|YAXIS|ZAXIS, (LPARAM) &p3D);

				lpStageProp->xAxisPos = p3D.x;
				lpStageProp->yAxisPos = p3D.y;
				lpStageProp->zAxisPos = p3D.z;

				SetDlgItemInt(hwnd, IDC_EDIT_XPOS, lpStageProp->xAxisPos, TRUE);
				SetDlgItemInt(hwnd, IDC_EDIT_YPOS, lpStageProp->yAxisPos, TRUE);
				SetDlgItemInt(hwnd, IDC_EDIT_ZPOS, lpStageProp->zAxisPos, TRUE);
				
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_BTN_START){
				CHAR str[MAX_STRING];
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				Button_GetText(lpStageProp->hButtonStart, str, MAX_STRING);

				if(!strncmp("Start", str, 5)){
					SendMessage(lpStageProp->hZeissScope, ZSM_ACQUISITION, (WPARAM) &lpStageProp->mfov, NULL);
				
					Edit_GetText(lpStageProp->hEditPath, str, MAX_STRING);
					CreateValidDirectory(str);
					//Button_SetText(lpStageProp->hButtonStart, "Stop");
				}
				else{
					SendMessage(lpStageProp->hZeissScope, ZSM_EXIT, NULL, NULL);
					Button_SetText(lpStageProp->hButtonStart, "Start");
				}

				return TRUE;
			}

			if(LOWORD(wParam) == IDC_RADIO_RECALL){

				lpStageProp->flgFocusRecall = !lpStageProp->flgFocusRecall;

			}


			if(LOWORD(wParam == IDC_BTN_BROWSE)){

				GetFolderPath(hwnd, lpStageProp->strPath);
				Edit_SetText(lpStageProp->hEditPath, lpStageProp->strPath);

			}
			
			return TRUE;
		case EN_CHANGE:
			if(LOWORD(wParam) == IDC_EDIT_VELOCITY){
				lpStageProp->zAxisVelocity = GetDlgItemInt(hwnd, IDC_EDIT_VELOCITY, NULL, FALSE);
				SendMessage(lpStageProp->hZeissScope, ZSM_SETAXIS, ZAXIS_VELOCITY, (LPARAM) lpStageProp->zAxisVelocity);
			}

			if(LOWORD(wParam) == IDC_EDIT_DELAY){
				lpStageProp->zAxisDelay = GetDlgItemInt(hwnd, IDC_EDIT_DELAY, NULL, FALSE);
				SendMessage(lpStageProp->hZeissScope, ZSM_SETAXIS, ZAXIS_DELAY, (LPARAM) lpStageProp->zAxisDelay);
			}

			if(LOWORD(wParam) == IDC_EDIT_XGOTO){
				lpStageProp->xAxisGoTo = GetDlgItemInt(hwnd, IDC_EDIT_XGOTO, NULL, TRUE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_YGOTO){
				lpStageProp->yAxisGoTo = GetDlgItemInt(hwnd, IDC_EDIT_YGOTO, NULL, TRUE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_ZGOTO){
				lpStageProp->zAxisGoTo = GetDlgItemInt(hwnd, IDC_EDIT_ZGOTO, NULL, TRUE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_FNSTEP){
				lpStageProp->fNumStep = GetDlgItemInt(hwnd, IDC_EDIT_FNSTEP, NULL, FALSE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_FSTEP){
				lpStageProp->fStep = GetDlgItemInt(hwnd, IDC_EDIT_FSTEP, NULL, FALSE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_ZSTEP){
				lpStageProp->zStep = GetDlgItemInt(hwnd, IDC_EDIT_ZSTEP, NULL, FALSE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_COL){
				lpStageProp->mfov.nCol = GetDlgItemInt(hwnd, IDC_EDIT_COL, NULL, FALSE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_ROW){
				lpStageProp->mfov.nRow = GetDlgItemInt(hwnd, IDC_EDIT_ROW, NULL, FALSE);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_XYVELOCITY){
				lpStageProp->xyAxisVelocity = GetDlgItemInt(hwnd, IDC_EDIT_XYVELOCITY, NULL, FALSE);
				SendMessage(lpStageProp->hZeissScope, ZSM_SETSPEED, (WPARAM) XAXIS|YAXIS, (LPARAM) lpStageProp->xyAxisVelocity);
				return TRUE;
			}

			if(LOWORD(wParam) == IDC_EDIT_OVERLAP){
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				Edit_GetText(lpStageProp->hEditOverlap, str, MAX_STRING);
				sscanf(str, "%f", &lpStageProp->mfov.overlap);
				return TRUE;
			}

		default:
			break;
		}
		break;
	case WM_SETPOS:
		if(wParam == XAXIS){
			lpStageProp->xAxisPos = (INT) lParam;
			SetDlgItemInt(hwnd, IDC_EDIT_XPOS, lpStageProp->xAxisPos, TRUE);
			break;
		}

		if(wParam == YAXIS){
			lpStageProp->yAxisPos = (INT) lParam;
			SetDlgItemInt(hwnd, IDC_EDIT_YPOS, lpStageProp->yAxisPos, TRUE);
			break;
		}

		if(wParam == ZAXIS){
			lpStageProp->zAxisPos = (INT) lParam;
			SetDlgItemInt(hwnd, IDC_EDIT_ZPOS, lpStageProp->zAxisPos, TRUE);
			break;
		}

		break;
	case WM_GETPOS:
		if(wParam == XAXIS){
			break;
		}

		if(wParam == YAXIS){
			break;
		}

		if(wParam == ZAXIS){
			break;
		}

		break;
	case WM_UPDATEPOS:
		if(wParam == XAXIS){
			lpStageProp->xAxisPos = (INT) lParam;
			break;
		}

		if(wParam == YAXIS){
			lpStageProp->yAxisPos = (INT) lParam;
			break;
		}

		if(wParam == ZAXIS){
			lpStageProp->zAxisPos = (INT) lParam;
			break;
		}

		break;
	case WM_SETHWND:

		if(wParam == HWND_OWNER){
			lpStageProp->hOwner = (HWND) lParam;
		}	
		if(wParam == HWND_ZEISSMICROSCOPE){
			lpStageProp->hZeissScope = (HWND) lParam;
			SendMessage(lpStageProp->hZeissScope, ZSM_SETAXIS, ZAXIS_VELOCITY, (LPARAM) lpStageProp->zAxisVelocity);
			SendMessage(lpStageProp->hZeissScope, ZSM_SETAXIS, ZAXIS_DELAY, (LPARAM) lpStageProp->zAxisDelay);\
			SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, XAXIS, NULL);
			lpStageProp->xAxisPos = SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, XAXIS, NULL);
			lpStageProp->yAxisPos = SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, YAXIS, NULL);
			lpStageProp->zAxisPos = SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, ZAXIS, NULL);

			SetDlgItemInt(hwnd, IDC_EDIT_XPOS, lpStageProp->xAxisPos, TRUE);
			SetDlgItemInt(hwnd, IDC_EDIT_YPOS, lpStageProp->yAxisPos, TRUE);
			SetDlgItemInt(hwnd, IDC_EDIT_ZPOS, lpStageProp->zAxisPos, TRUE);

		}

		return TRUE;
	case WM_GETHWND:
		if(wParam == HWND_ACTIVESTAGE){
			SetWindowLong(hwnd, NULL/*DWL_MSGRESULT*/, (LONG)lpStageProp->hZeissScope);
			return (LRESULT) lpStageProp->hZeissScope;
		}
		return NULL;
	case WM_CLOSE:

		SendMessage(lpStageProp->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_STAGEPROPERTIES), NULL);

		return TRUE;
	case WM_TIMER:
		lpStageProp->xAxisPos = SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, XAXIS, NULL);
		lpStageProp->yAxisPos = SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, YAXIS, NULL);
		lpStageProp->zAxisPos = SendMessage(lpStageProp->hZeissScope, ZSM_GETPOS, ZAXIS, NULL);

		SetDlgItemInt(hwnd, IDC_EDIT_XPOS, lpStageProp->xAxisPos, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_YPOS, lpStageProp->yAxisPos, TRUE);
		SetDlgItemInt(hwnd, IDC_EDIT_ZPOS, lpStageProp->zAxisPos, TRUE);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}
