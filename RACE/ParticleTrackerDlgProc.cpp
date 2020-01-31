#include "stdafx.h"

#include "ParticleTrackerDlg.h"

INT_PTR CALLBACK ParticleTrackerDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PARTICLETRACKERDLG *lpParticleTrackerDlg = (PARTICLETRACKERDLG *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpParticleTrackerDlg = (PARTICLETRACKERDLG *)calloc(1, sizeof(PARTICLETRACKERDLG));
		memset(lpParticleTrackerDlg, 0x00, sizeof(PARTICLETRACKERDLG));
		SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lpParticleTrackerDlg);
		lpParticleTrackerDlg = (PARTICLETRACKERDLG *) GetWindowLongPtr(hwnd, (-21));

		if(lpParticleTrackerDlg == NULL){
			ThrowMessage(hwnd, "DlgParticleTracker: Error Getting Long Ptr");
			ThrowMessage(hwnd, "DlgParticleTracker: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpParticleTrackerDlg->hwnd = hwnd;
		lpParticleTrackerDlg->hListResources = GetDlgItem(hwnd, IDC_LIST_RESOURCES);
		lpParticleTrackerDlg->hEditUpper = GetDlgItem(hwnd, IDC_EDIT_UPPER);
		lpParticleTrackerDlg->hEditLower = GetDlgItem(hwnd, IDC_EDIT_LOWER);
		lpParticleTrackerDlg->hBtnCapture = GetDlgItem(hwnd, IDC_BTN_CAPTURE);

		lpParticleTrackerDlg->hEditMaxAngle = GetDlgItem(hwnd, IDC_EDIT_MAXANGLE);
		lpParticleTrackerDlg->hEditMaxDelta = GetDlgItem(hwnd, IDC_EDIT_MAXDELTA);
		lpParticleTrackerDlg->hEditMaxVelocity = GetDlgItem(hwnd, IDC_EDIT_MAXVELOCITY);

		HWND hList;
		hList = lpParticleTrackerDlg->hListResources;
		SendMessage(hList, LB_INSERTSTRING, 0, (LPARAM) STR_STREAM);
		SendMessage(hList, LB_INSERTSTRING, 1, (LPARAM) STR_FILTER);
		SendMessage(hList, LB_INSERTSTRING, 2, (LPARAM) STR_TRACKING);
		SendMessage(hList, LB_INSERTSTRING, 3, (LPARAM) STR_REFFRAME);
		SendMessage(hList, LB_INSERTSTRING, 4, (LPARAM) STR_MASK_LATGUIDE);
		SendMessage(hList, LB_INSERTSTRING, 5, (LPARAM) STR_MASK_LONGUIDE);
		SendMessage(hList, LB_INSERTSTRING, 6, (LPARAM) STR_MASK_CHANNEL);
		
		lpParticleTrackerDlg->maxAngle = 30;
		lpParticleTrackerDlg->maxDelta = 15;
		lpParticleTrackerDlg->maxVelocity = 200;

		sprintf(str, "%.f", lpParticleTrackerDlg->maxAngle);
		Edit_SetText(lpParticleTrackerDlg->hEditMaxAngle, str);

		sprintf(str, "%.f", lpParticleTrackerDlg->maxDelta);
		Edit_SetText(lpParticleTrackerDlg->hEditMaxDelta, str);

		sprintf(str, "%.f", lpParticleTrackerDlg->maxVelocity);
		Edit_SetText(lpParticleTrackerDlg->hEditMaxVelocity, str);
	
		Set(8.0, 25.0, &lpParticleTrackerDlg->limDiameter);

		sprintf(str, "%.1f", lpParticleTrackerDlg->limDiameter.lower);
		Edit_SetText(lpParticleTrackerDlg->hEditLower, str);
		sprintf(str, "%.1f", lpParticleTrackerDlg->limDiameter.upper);
		Edit_SetText(lpParticleTrackerDlg->hEditUpper, str);

		return FALSE;
	case WM_COMMAND:

		//Control-Defined Notifiation Code
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			if(LOWORD(wParam) == IDC_BTN_CAPTURE){
				SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_GETMASK, NULL, (LPARAM)lpParticleTrackerDlg->flgResource);
				return TRUE;
			}
			if(LOWORD(wParam) == IDC_BTN_CALCREFERENCE){
				lpParticleTrackerDlg->lpParticleTracker->flgGetRef = TRUE;
			}
	
			break;
		case LBN_SELCHANGE:
			if(HIWORD(wParam) == LBN_SELCHANGE){
				INT sel = NULL;
				sel = SendMessage(lpParticleTrackerDlg->hListResources, LB_GETCURSEL, NULL, NULL);
				SendMessage(lpParticleTrackerDlg->hListResources, LB_GETTEXT, (WPARAM) sel, (LPARAM) str);
				
				if(!strcmp(str, STR_STREAM)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_STREAM, NULL);
					lpParticleTrackerDlg->flgResource = PTS_STREAM;
				}else if(!strcmp(str, STR_FILTER)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_FILTER, NULL);
					lpParticleTrackerDlg->flgResource = PTS_FILTER;
				}else if(!strcmp(str, STR_REFFRAME)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_REFERENCE, NULL);
					lpParticleTrackerDlg->flgResource = PTS_REFERENCE;
				}else if(!strcmp(str, STR_MASK_LONGUIDE)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_MASK_LON, NULL);
					lpParticleTrackerDlg->flgResource = PTS_MASK_LON;
				}else if(!strcmp(str, STR_MASK_LATGUIDE)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_MASK_LAT, NULL);
					lpParticleTrackerDlg->flgResource = PTS_MASK_LAT;
				}else if(!strcmp(str, STR_MASK_CHANNEL)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_MASK_CHN, NULL);
					lpParticleTrackerDlg->flgResource = PTS_MASK_CHN;
				}else if(!strcmp(str, STR_TRACKING)){
					SendMessage(lpParticleTrackerDlg->hParticleTracker, WM_SHOW, PTS_TRACKING, NULL);
					lpParticleTrackerDlg->flgResource = PTS_TRACKING;
				}else{
				}

			}
			break;
		case EN_CHANGE:
			if(LOWORD(wParam) == IDC_EDIT_LOWER){
				Edit_GetText(lpParticleTrackerDlg->hEditLower, str, MAX_STRING);
				sscanf(str, "%f", &lpParticleTrackerDlg->limDiameter.lower);
				SendLimit(lpParticleTrackerDlg->hParticleTracker, DIAMETER, lpParticleTrackerDlg->limDiameter);
			}
			if(LOWORD(wParam) == IDC_EDIT_UPPER){
				Edit_GetText(lpParticleTrackerDlg->hEditUpper, str, MAX_STRING);
				sscanf(str, "%f", &lpParticleTrackerDlg->limDiameter.upper);
				SendLimit(lpParticleTrackerDlg->hParticleTracker, DIAMETER, lpParticleTrackerDlg->limDiameter);
			}
			if(LOWORD(wParam) == IDC_EDIT_MAXANGLE){
				Edit_GetText(lpParticleTrackerDlg->hEditMaxAngle, str, MAX_STRING);
				sscanf(str, "%f", &lpParticleTrackerDlg->maxAngle);
				LIMIT lim = {0};
				lim.upper = lpParticleTrackerDlg->maxAngle;
				SendLimit(lpParticleTrackerDlg->hParticleTracker, ANGLE, lim);
			}
			if(LOWORD(wParam) == IDC_EDIT_MAXDELTA){
				Edit_GetText(lpParticleTrackerDlg->hEditMaxDelta, str, MAX_STRING);
				sscanf(str, "%f", &lpParticleTrackerDlg->maxDelta);
				LIMIT lim = {0};
				lim.upper = lpParticleTrackerDlg->maxDelta;
				SendLimit(lpParticleTrackerDlg->hParticleTracker, DVELOCITY, lim);
			}
			if(LOWORD(wParam) == IDC_EDIT_MAXDISTANCE){
				Edit_GetText(lpParticleTrackerDlg->hEditMaxVelocity, str, MAX_STRING);
				sscanf(str, "%f", &lpParticleTrackerDlg->maxVelocity);
				LIMIT lim = {0};
				lim.upper = lpParticleTrackerDlg->maxVelocity;
				SendLimit(lpParticleTrackerDlg->hParticleTracker, VELOCITY, lim);
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
	case WM_SETHWND:

		switch(wParam){
			case HWND_OWNER:
				lpParticleTrackerDlg->hOwner = (HWND) lParam;
			return TRUE;
			case HWND_PARTICLETRACKER:
				lpParticleTrackerDlg->hParticleTracker = (HWND) lParam;
				lpParticleTrackerDlg->lpParticleTracker = (PARTICLETRACKER *) GetWindowLongPtr((HWND) lParam, GWLP_USERDATA);
				SendLimit(lpParticleTrackerDlg->hParticleTracker, DIAMETER, lpParticleTrackerDlg->limDiameter);
			return TRUE;
		default:
			break;
		}

		return TRUE;
	case WM_SETHANDLE:

		if(wParam == HANDLE_DEVICE){
		}
		return TRUE;
	case WM_CLOSE:

		//SendMessage(lpPropertyManager->hOwner, WM_COMMAND, LOWORD(ID_VIEW_PROPERTYMANAGER), NULL);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}