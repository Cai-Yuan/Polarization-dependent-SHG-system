#include "stdafx.h"

#include "TrackerProc.h"

#include "Limit.h"
#include "ParticleTracker.h"


//SORTING CRITERIA
//1)Max Velcity
//2)Velocity at specific window
//3)Window Average
//4)Curve Fitting

INT_PTR CALLBACK TrackerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int flgInit = FALSE;
	LM_TRACKER *lpWnd = (LM_TRACKER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		if(lpWnd){
			return FALSE;
		}

		//Multiple 'WM_INITDIALOG' messages were being recieved
		//To stop multple copies of 'DeviceProc' from running, a static 'flgInit' flag
		//is used to force only a single instance is generated
		if(flgInit){
			return TRUE;
		}
		flgInit = TRUE;

		lpWnd = (LM_TRACKER *) InitWindowLongRACE(hwnd, sizeof(LM_TRACKER));

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpWnd->hOwner = (HWND) psp.lParam;

		lpWnd->hListResources = GetDlgItem(hwnd, IDC_LIST_RESOURCES);
		lpWnd->hEditUpperDim = GetDlgItem(hwnd, IDC_EDIT_UPPER_DIM);
		lpWnd->hEditLowerDim = GetDlgItem(hwnd, IDC_EDIT_LOWER_DIM);
		lpWnd->hEditUpperVel = GetDlgItem(hwnd, IDC_EDIT_UPPER_VEL);
		lpWnd->hEditLowerVel = GetDlgItem(hwnd, IDC_EDIT_LOWER_VEL);
		lpWnd->hBtnCapture = GetDlgItem(hwnd, IDC_BTN_CAPTURE);
		lpWnd->hBtnReference = GetDlgItem(hwnd, IDC_BTN_CALCREFERENCE);
		lpWnd->hEditNumReference = GetDlgItem(hwnd, IDC_EDIT_NUMREFERENCE);

		lpWnd->hEditMaxAngle = GetDlgItem(hwnd, IDC_EDIT_MAXANGLE);
		lpWnd->hEditMaxDelta = GetDlgItem(hwnd, IDC_EDIT_MAXDELTA);
		lpWnd->hEditMaxVelocity = GetDlgItem(hwnd, IDC_EDIT_MAXVELOCITY);

		lpWnd->hCheckEnable = GetDlgItem(hwnd, IDC_CHECK_ENABLE);

		HWND hList;
		hList = lpWnd->hListResources;
		SendMessage(hList, LB_INSERTSTRING, 0, (LPARAM) STR_STREAM);
		SendMessage(hList, LB_INSERTSTRING, 1, (LPARAM) STR_FILTER);
		SendMessage(hList, LB_INSERTSTRING, 2, (LPARAM) STR_TRACKING);
		SendMessage(hList, LB_INSERTSTRING, 3, (LPARAM) STR_REFFRAME);
		SendMessage(hList, LB_INSERTSTRING, 4, (LPARAM) STR_MASK_LATGUIDE);
		SendMessage(hList, LB_INSERTSTRING, 5, (LPARAM) STR_MASK_LONGUIDE);
		SendMessage(hList, LB_INSERTSTRING, 6, (LPARAM) STR_MASK_CHANNEL);
		SendMessage(hList, LB_INSERTSTRING, 7, (LPARAM) STR_MASK_FLOW);
		
		
		lpWnd->maxAngle = 30;
		lpWnd->maxDelta = 15;
		lpWnd->maxVelocity = 200;

		Set(8.0, 25.0, &lpWnd->limDiameter);

		Edit_SetFloat(lpWnd->hEditMaxAngle, lpWnd->maxAngle, 2);
		Edit_SetFloat(lpWnd->hEditMaxDelta, lpWnd->maxDelta, 2);
		Edit_SetFloat(lpWnd->hEditMaxVelocity, lpWnd->maxVelocity, 2);
		Edit_SetFloat(lpWnd->hEditLowerDim, lpWnd->limDiameter.lower, 2);
		Edit_SetFloat(lpWnd->hEditUpperDim, lpWnd->limDiameter.upper, 2);
		

		SendMessage(lpWnd->hOwner, WM_SETHWND, (WPARAM)HWND_TRACKER, (LPARAM)hwnd);

		Tracker_Enable(lpWnd, FALSE);

		lpWnd->hParticleTracker = CreateParticleTracker(hwnd, "ParticleTracker - Laser Microscope");
		SendMessage(lpWnd->hParticleTracker, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM) hwnd);
		
		HWND hFrame;
		HWND hLaserMicroscope;
		hLaserMicroscope = lpWnd->hOwner;
		hFrame = (HWND) SendMessage(hLaserMicroscope, WM_GETHWND, (WPARAM)HWND_OWNER, NULL);
		lpWnd->hDataView = CreateMDIDataView(hFrame, "Data View - ParticleTracker");
		SendMessage(lpWnd->hParticleTracker, WM_SETHWND, (WPARAM)HWND_DATAVIEW, (LPARAM)lpWnd->hDataView);
		//ShowWindow(lpWnd->hDataView, SW_SHOW);
		lpWnd->lpParticleTracker = (PARTICLETRACKER *) GetWindowUserData(lpWnd->hParticleTracker);

		lpWnd->lpParticleTracker->nReference = 20;
		Edit_SetInt(lpWnd->hEditNumReference, lpWnd->lpParticleTracker->nReference);

		return FALSE;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
				switch(LOWORD(wParam))
				{
				case IDC_CHECK_ENABLE:
					UpdateCheckFlag(lpWnd->hCheckEnable, &lpWnd->flgEnable);
					if(lpWnd->flgEnable){
						Tracker_Enable(lpWnd, TRUE);
					}else{
						Tracker_Enable(lpWnd, FALSE);
					}
					break;
				case IDC_BTN_CAPTURE:
					SendMessage(lpWnd->hParticleTracker, WM_GETMASK, NULL, (LPARAM)lpWnd->flgResource);
					return TRUE;
				case IDC_BTN_CALCREFERENCE:
					if(lpWnd->flgResource == PTS_REFERENCE){
						lpWnd->lpParticleTracker->flgGetRef = TRUE;
					}else if(lpWnd->flgResource == PTS_MASK_FLOW){
						lpWnd->lpParticleTracker->flgGetFlow = TRUE;
					}
					return TRUE;
				default:
					break;
				}
			break;
		case EN_CHANGE:
			{
				switch(LOWORD(wParam))
				{
				case IDC_EDIT_MAXANGLE:
					{
						lpWnd->maxAngle = Edit_GetFloat(lpWnd->hEditMaxAngle);
						LIMIT lim;
						lim.lower = 0;
						lim.upper = lpWnd->maxAngle;
						SendLimit(lpWnd->hParticleTracker, ANGLE, lim);
					}
					break;
				case IDC_EDIT_MAXVELOCITY:
					{
						lpWnd->maxVelocity = Edit_GetFloat(lpWnd->hEditMaxVelocity);
						LIMIT lim;
						lim.lower = 0;
						lim.upper = lpWnd->maxVelocity;
						SendLimit(lpWnd->hParticleTracker, VELOCITY, lim);
					}
					break;
				case IDC_EDIT_MAXDELTA:
					{
						lpWnd->maxDelta = Edit_GetFloat(lpWnd->hEditMaxDelta);
						LIMIT lim;
						lim.lower = 0;
						lim.upper = lpWnd->maxDelta;
						SendLimit(lpWnd->hParticleTracker, DVELOCITY, lim);
					}
					break;
				case IDC_EDIT_LOWER_DIM:
					{
						lpWnd->limDiameter.lower = Edit_GetFloat(lpWnd->hEditLowerDim);
						//lpWnd->limDiameter.lower = Edit_GetFloat(lpWnd->hEditMaxDelta);
						SendLimit(lpWnd->hParticleTracker, DIAMETER, lpWnd->limDiameter);
					}
					break;
				case IDC_EDIT_UPPER_DIM:
					{
						lpWnd->limDiameter.upper = Edit_GetFloat(lpWnd->hEditUpperDim);
						SendLimit(lpWnd->hParticleTracker, DIAMETER, lpWnd->limDiameter);
					}
					break;
				case IDC_EDIT_NUMREFERENCE:
					{
						lpWnd->lpParticleTracker->nReference = Edit_GetInt(lpWnd->hEditNumReference);
					}
					break;
				default:
					break;
				}
			}
		  break;
		case LBN_SELCHANGE:
			if(HIWORD(wParam) == LBN_SELCHANGE){
				INT sel = NULL;
				sel = SendMessage(lpWnd->hListResources, LB_GETCURSEL, NULL, NULL);
				SendMessage(lpWnd->hListResources, LB_GETTEXT, (WPARAM) sel, (LPARAM) str);
				
				if(!strcmp(str, STR_STREAM)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_STREAM, NULL);
					lpWnd->flgResource = PTS_STREAM;
				}else if(!strcmp(str, STR_FILTER)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_FILTER, NULL);
					lpWnd->flgResource = PTS_FILTER;
				}else if(!strcmp(str, STR_REFFRAME)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_REFERENCE, NULL);
					lpWnd->flgResource = PTS_REFERENCE;
				}else if(!strcmp(str, STR_MASK_LONGUIDE)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_MASK_LON, NULL);
					lpWnd->flgResource = PTS_MASK_LON;
				}else if(!strcmp(str, STR_MASK_LATGUIDE)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_MASK_LAT, NULL);
					lpWnd->flgResource = PTS_MASK_LAT;
				}else if(!strcmp(str, STR_MASK_CHANNEL)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_MASK_CHN, NULL);
					lpWnd->flgResource = PTS_MASK_CHN;
				}else if(!strcmp(str, STR_TRACKING)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_TRACKING, NULL);
					lpWnd->flgResource = PTS_TRACKING;
				}else if(!strcmp(str, STR_MASK_FLOW)){
					SendMessage(lpWnd->hParticleTracker, WM_SHOW, PTS_MASK_FLOW, NULL);
					lpWnd->flgResource = PTS_MASK_FLOW;
				}else{
				}
			}
			break;

		default:
				break;
		}
		return TRUE;
	case WM_SETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			lpWnd->hOwner = (HWND) lParam;
			return TRUE;
		case HWND_DATAVIEW:
			UINT w, h;
			HWND hDataView;
			hDataView = (HWND) lParam;
			//lpWnd->hDataView = (HWND) lParam;

			DataView_GetWidthHeight(hDataView, &w, &h);
			ParticleTracker_SetFrameSize(lpWnd->hParticleTracker, w, h);

			return TRUE;
		default:
			return FALSE;
		}
	case WM_GETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			return (LRESULT) lpWnd->hOwner;
		case HWND_DATAVIEW:
			return (LRESULT) lpWnd->hDataView;
		case HWND_PARTICLETRACKER:
			return (LRESULT) lpWnd->hParticleTracker;
		default:
			return FALSE;
		}
		return FALSE;
	case WM_DATAIN:
		SendMessage(lpWnd->hParticleTracker, msg, wParam, lParam);
		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD Tracker_Enable(LM_TRACKER *lpTracker, BOOL bState)
{
	if(!lpTracker)
		return FALSE;

	if(bState){
		ShowWindow(lpTracker->hDataView, SW_SHOW);
	}


	lpTracker->flgEnable = bState;
	Button_Enable(lpTracker->hBtnCapture, bState);
	Button_Enable(lpTracker->hBtnReference, bState);
	Edit_Enable(lpTracker->hEditLowerDim, bState);
	Edit_Enable(lpTracker->hEditUpperDim, bState);
	Edit_Enable(lpTracker->hEditLowerVel, bState);
	Edit_Enable(lpTracker->hEditUpperVel, bState);
	Edit_Enable(lpTracker->hEditMaxAngle, bState);
	Edit_Enable(lpTracker->hEditMaxDelta, bState);
	Edit_Enable(lpTracker->hEditMaxVelocity, bState);

	EnableWindow(lpTracker->hListResources, bState);

	ParticleTracker_Enable(lpTracker->hParticleTracker, bState);

	return TRUE;
}
