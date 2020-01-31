#include "stdafx.h"
#include "TimeLapse.h"
#include "ToolsWIN32.h"
#include "DataManager.h"
#include "PDM.h"
#include "String.h"

INT_PTR CALLBACK TimeLapseProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	TIMELAPSE *lpTimeLapse = (TIMELAPSE *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:
		OnInitDialogTimeLapse(hwnd, wParam, lParam);
		break;
	case WM_COMMAND:
		OnCommandTimeLapse(lpTimeLapse, hwnd, msg, wParam, lParam);
		return FALSE;
	case WM_SETHWND:
		if(wParam == HWND_OWNER)
			lpTimeLapse->hOwner = (HWND) lParam;
		if(wParam == HWND_TIMELAPSEPROPERTIES){
			lpTimeLapse->hTimeLapseProp = (HWND) lParam;
		}
		return TRUE;
	case WM_TIMELAPSE:
		TIMELAPSEPROPERTIES *tlp;
		tlp = (TIMELAPSEPROPERTIES *)wParam;
		
		FLOAT rate;
		rate = tlp->fRate;

		//Update running timelapse count
		lpTimeLapse->nTimeLapse++;
		
		CHAR str[MAX_STRING];
		CHAR strSrc[MAX_STRING];
		CHAR strDirectory[MAX_STRING];

		GetWindowText(tlp->hSource, strSrc, MAX_STRING);
		DirectoryFromStringEx(strSrc, strDirectory);
		sprintf(str, "%s [%.3fs]", strDirectory, rate);
		ListBox_AddString(lpTimeLapse->hListStudies, str);
		//SendMessage(lpTimeLapse->hListStudies, LB_ADDSTRING, NULL, (LPARAM) str);

		TIMELAPSETHREAD tlt;
		tlt.hOwner = hwnd;
		tlt.flgTimeLapse = TRUE;
		tlt.fRate = rate;
		tlt.hSourceDataView = tlp->hSource;
		tlt.id = lpTimeLapse->nTimeLapse;
		tlt.flgRaw = lpTimeLapse->flgRaw;
		tlt.flgRendering = lpTimeLapse->flgRendering;
		memcpy(tlt.strDest, str, MAX_STRING*sizeof(CHAR));
		memcpy(tlt.strSource, strSrc, MAX_STRING*sizeof(CHAR));
		memcpy(tlt.strArchive, str, MAX_STRING*sizeof(CHAR));
		memcpy(tlt.strID, str, MAX_STRING*sizeof(CHAR));
		Append(&lpTimeLapse->tlts, tlt);
		tlt.hThread = CreateThread(NULL, NULL, TimeLapseThreadProc, (LPVOID) &lpTimeLapse->tlts.tlt[lpTimeLapse->nTimeLapse-1], NULL, NULL);

		//Sleep(300);

		return TRUE;
	case WM_CLOSETIMELAPSE:
		{
			CHAR *strID = (CHAR *)wParam;
			INT pos = 0;
			DWORD i = 0;
			if(Find(&lpTimeLapse->tlts, strID, &i)){
				pos = ListBox_FindString(lpTimeLapse->hListStudies, 0, strID);
				ListBox_DeleteString(lpTimeLapse->hListStudies, pos);
				Delete(&lpTimeLapse->tlts, i);
			}
		}
		return TRUE;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return TRUE;
	default:
		return FALSE;
	}

	return FALSE;
}

INT_PTR CALLBACK TimeLapsePropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	TIMELAPSEPROPERTIES *lpTimeLapseProp = (TIMELAPSEPROPERTIES *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:
		OnInitDialogTimeLapseProperties(hwnd, wParam, lParam);
		break;
	case WM_COMMAND:
		OnCommandTimeLapseProperties(lpTimeLapseProp, hwnd, msg, wParam, lParam);
		break;
	case WM_SETHWND:
		if(wParam == HWND_OWNER)
			lpTimeLapseProp->hOwner = (HWND) lParam;
		if(wParam == HWND_TIMELAPSE){
			lpTimeLapseProp->hTimeLapse = (HWND) lParam;
			SendMessage(lpTimeLapseProp->hTimeLapse, WM_SETHWND, (WPARAM) HWND_TIMELAPSEPROPERTIES, (LPARAM) hwnd);
		}
		if(wParam == HWND_ACTIVE){
			HWND hActive;
			CHAR strActive[MAX_STRING];
			hActive = (HWND) lParam;
			GetWindowText(hActive, strActive, MAX_STRING);
			Edit_SetText(lpTimeLapseProp->hEditSource, strActive);
		}
		if(wParam == HWND_TIMELAPSE){
			lpTimeLapseProp->hTimeLapse = (HWND) lParam;
		}
		return TRUE;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return TRUE;
	default:
		return FALSE;
	}

	return FALSE;
}

DWORD OnInitDialogTimeLapse(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	TIMELAPSE *lpTimeLapse;
	lpTimeLapse = (TIMELAPSE *)malloc(sizeof(TIMELAPSE));
	memset(lpTimeLapse, 0x0, sizeof(TIMELAPSE));
	SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lpTimeLapse);
	lpTimeLapse = (TIMELAPSE *) GetWindowLongPtr(hwnd, (-21));

	if(!lpTimeLapse){
		ThrowMessage(hwnd, "WndTimeLapse: Error Getting Long Ptr");
		ThrowMessage(hwnd, "WndTimeLapse: Exiting Initialization");
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		return TRUE;
	}

	lpTimeLapse->hBtnCreate = GetDlgItem(hwnd, IDC_BTN_CREATE);
	lpTimeLapse->hBtnDelete = GetDlgItem(hwnd, IDC_BTN_DELETE);
	lpTimeLapse->hListStudies = GetDlgItem(hwnd, IDC_LIST_STUDIES);

	lpTimeLapse->hCheckRaw = GetDlgItem(hwnd, IDC_CHECK_RAW);
	lpTimeLapse->hCheckRendering = GetDlgItem(hwnd, IDC_CHECK_RENDERING);

	Button_SetCheck(lpTimeLapse->hCheckRaw, BST_CHECKED);

	lpTimeLapse->flgRaw = TRUE;
	lpTimeLapse->flgRendering = TRUE;

	lpTimeLapse->nTimeLapse = 0;
	Alloc(&lpTimeLapse->tlts, 100);

	return FALSE;
}

DWORD OnInitDialogTimeLapseProperties(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	TIMELAPSEPROPERTIES *lpTimeLapseProp;
	lpTimeLapseProp = (TIMELAPSEPROPERTIES *)malloc(sizeof(TIMELAPSEPROPERTIES));
	memset(lpTimeLapseProp, 0x0, sizeof(TIMELAPSEPROPERTIES));
	SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lpTimeLapseProp);
	lpTimeLapseProp = (TIMELAPSEPROPERTIES *) GetWindowLongPtr(hwnd, (-21));

	if(!lpTimeLapseProp){
		ThrowMessage(hwnd, "WndTimeLapseProperties: Error Getting Long Ptr");
		ThrowMessage(hwnd, "WndTimeLapseProperties: Exiting Initialization");
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		return TRUE;
	}

	lpTimeLapseProp->hEditSource = GetDlgItem(hwnd, IDC_EDIT_SOURCE);
	lpTimeLapseProp->hEditRate = GetDlgItem(hwnd, IDC_EDIT_RATE);
	Edit_SetFloat3(lpTimeLapseProp->hEditRate, 1.0);

	return FALSE;
}

DWORD OnCommandTimeLapse(TIMELAPSE *lpTimeLapse, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(HIWORD(wParam))
	{
		case EN_CHANGE:
			if(LOWORD(wParam) == IDC_EDIT_RATE){
				//FLOAT rate;
				//HWND hActive;
				//rate = Edit_GetFloat(lpTimeLapse->hEditRate);
				//hActive = (HWND) SendMessage(lpTimeLapse->hOwner, WM_GETHWND, HWND_ACTIVE, NULL);
				//Search through the set of threads and find a matching hDataView
				//If no match, ignore
			}
			return FALSE;
		default:
			break;
	}

	switch(LOWORD(wParam))
	{
		case IDC_BTN_CREATE:
			HWND hActive;
			hActive = (HWND) SendMessage(lpTimeLapse->hOwner, WM_GETHWND, (WPARAM)HWND_ACTIVE, NULL);
			SendMessage(lpTimeLapse->hTimeLapseProp, WM_SETHWND, (WPARAM)HWND_ACTIVE, (LPARAM) hActive);
			ShowWindow(lpTimeLapse->hTimeLapseProp, SW_SHOW);
			return TRUE;
		case IDC_BTN_DELETE:
			DWORD sel;
			CHAR strID[MAX_STRING];
			sel = ListBox_GetCurSel(lpTimeLapse->hListStudies);
			ListBox_GetText(lpTimeLapse->hListStudies, sel, strID);
			//sel = SendMessage(lpTimeLapse->hListStudies, LB_GETCURSEL, NULL, NULL);
			//if(sel == LB_ERR);
			//	return TRUE;
			if(Find(&lpTimeLapse->tlts, strID, &sel)){
				lpTimeLapse->tlts.tlt[sel].flgTimeLapse = FALSE;
			}
			//while(!lpTimeLapse->tlts.tlt[sel].flgEnd){
			//	Sleep(10);
			//}
			//TerminateThread(lpTimeLapse->tlts.tlt[sel].hThread, FALSE);
				
			//Delete(&lpTimeLapse->tlts, sel);
			//SendMessage(lpTimeLapse->hListStudies, LB_DELETESTRING, (WPARAM) sel, NULL);

			return TRUE;
		case IDC_LIST_STUDIES:
			return TRUE;
		case IDC_CHECK_RAW:
			lpTimeLapse->flgRaw = !lpTimeLapse->flgRaw;
			return TRUE;
		case IDC_CHECK_RENDERING:
			lpTimeLapse->flgRendering = !lpTimeLapse->flgRendering;
			return TRUE;
		case IDOK:
			ShowWindow(hwnd, SW_HIDE);
			return TRUE;
		case IDCANCEL:
			ShowWindow(hwnd, SW_HIDE);
			return TRUE;
		default:
			break;
	}

	return FALSE;
}

DWORD OnCommandTimeLapseProperties(TIMELAPSEPROPERTIES *lpTimeLapseProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(HIWORD(wParam))
	{
	case EN_CHANGE:
		if(LOWORD(wParam) == IDC_EDIT_RATE){
			HWND hActive;
			lpTimeLapseProp->fRate = Edit_GetFloat(lpTimeLapseProp->hEditRate);
			hActive = (HWND) SendMessage(lpTimeLapseProp->hOwner, WM_GETHWND, HWND_ACTIVE, NULL);

		}
		return FALSE;
	default:
		break;
	}

	switch(LOWORD(wParam))
	{
	case IDOK:
		HWND hActive;
		hActive = (HWND) SendMessage(lpTimeLapseProp->hOwner, WM_GETHWND, HWND_ACTIVE, NULL);
		lpTimeLapseProp->hSource = hActive;
		SendMessage(lpTimeLapseProp->hTimeLapse, WM_TIMELAPSE, (WPARAM) lpTimeLapseProp, NULL);
		ShowWindow(hwnd, SW_HIDE);
		return TRUE;
	case IDCANCEL:
		ShowWindow(hwnd, SW_HIDE);
		return TRUE;
	default:
		break;
	}

	return FALSE;
}

DWORD WINAPI TimeLapseThreadProc(LPVOID param)
{
	TIMELAPSETHREAD *lptlt = (TIMELAPSETHREAD *)param;
	TIMELAPSETHREAD	tlt = {0};
	
	if(!lptlt)
		return FALSE;

	memcpy(&tlt, lptlt, sizeof(TIMELAPSETHREAD));

	HWND hOwner = NULL;
	HWND hDataViewSrc = tlt.hSourceDataView;
	HWND hDataManager = NULL;
	HWND hDataView = NULL;

	CHAR str[MAX_STRING];
	CHAR strPath[MAX_STRING];
	CHAR strSrc[MAX_STRING];
	CHAR strAppend[MAX_STRING];
	CHAR strDirectory[MAX_STRING];
	CHAR strArchive[MAX_STRING];

	DWORD i = 0;
	DWORD msDelay = ((FLOAT)1000.0 * (FLOAT)tlt.fRate);

	PDMFRAME pdmf = {0};

	hOwner = (HWND) SendMessage(hDataViewSrc, WM_GETHWND, (WPARAM) HWND_FRAME, NULL);
	hDataManager = CreateDataManager(hOwner, DS_FRAMEGRABBER);
	Sleep(500);
	hDataView = (HWND) SendMessage(hDataManager, WM_GETHWND, HWND_DATAVIEW, NULL);

	LONG w, h;
	DataView_GetDimensions(hDataViewSrc, &w, &h);
	AllocPDMFrame(&pdmf, w, h, DT_UCHAR);

	DataManager_SetFrameSize(hDataManager, w, h);

	GetWindowText(hDataViewSrc, strSrc, MAX_STRING);
	DirectoryFromString(strSrc, strDirectory);
	sprintf(strAppend, "TimeLapse %d (%s)", tlt.id, strDirectory);
	sprintf(strArchive, "C:\\Archive\\%s\\TimeLapse %d\\", strDirectory, tlt.id);
	SetWindowText(hDataView, strAppend);
	ShowWindow(hDataView, SW_SHOW);

	PDMFRAME *pdmfSource = (PDMFRAME *) DataView_GetPDMPTR(hDataViewSrc, DT_RAW);
	PDMFRAME *pdmfRendering = (PDMFRAME *) DataView_GetPDMPTR(hDataViewSrc, DT_RENDERING);

	//CHAR strPath[MAX_STRING];
	//sprintf(str, "C:\\Archive\\%s\\", strDirectory);
	DataManager_SetArchivePath(hDataManager, strArchive);
	DataManager_Archive(hDataManager, TRUE, NULL);

	ShowWindow(hDataView, SW_SHOW);

	lptlt->flgEnd = FALSE;

	sprintf(strPath, "%s\Rendering\\", strArchive);
	CreateDirectory(strPath, NULL);

	while(lptlt->flgTimeLapse){
		pdmf.pdmfh.frame = i;
		SendMessage(hDataManager, WM_DATAIN, (WPARAM) pdmfSource, (LPARAM) FS_RAW);
		if(lptlt->flgRendering){
			sprintf(strPath, "%s\Rendering\\Rend %d.bmp", strArchive, i);
			SaveBMP(w, h, strPath, (PIXEL *)pdmfRendering->lpvoid);
		}
		Sleep(msDelay);
		i++;
	}

	lptlt->flgEnd = TRUE;
	PostMessage(hDataManager, WM_ENDTRANSMISSION, NULL, NULL);
	SendMessage(lptlt->hOwner, WM_CLOSETIMELAPSE, (WPARAM) lptlt->strID, NULL);

	return TRUE;
}

DWORD Alloc(TIMELAPSETHREADSET *tlts, DWORD size)
{

	if(!tlts)
		return FALSE;

	if(tlts->tlt)
		return FALSE;

	tlts->tlt = (TIMELAPSETHREAD *)malloc(size*sizeof(TIMELAPSETHREAD));

	if(!tlts->tlt){
		memset(tlts->tlt, NULL, sizeof(TIMELAPSETHREAD));
		return FALSE;
	}

	tlts->size = size;
	tlts->nTimeLapse = NULL;

	return TRUE;
}

DWORD Append(TIMELAPSETHREADSET *tlts, TIMELAPSETHREAD tlt)
{
	if(!tlts || !tlts->tlt)
		return FALSE;

	if(tlts->nTimeLapse < tlts->size){
		memcpy(&tlts->tlt[tlts->nTimeLapse], &tlt, sizeof(TIMELAPSETHREAD));
		tlts->nTimeLapse++;
	}


	return TRUE;
}

DWORD Delete(TIMELAPSETHREADSET *tlts, DWORD index)
{
	
	if(!tlts || !tlts->tlt)
		return FALSE;

	if(index > tlts->nTimeLapse)
		return FALSE;

	//Copy the last index into the one being removed
	memcpy(&tlts->tlt[index], &tlts->tlt[tlts->nTimeLapse], sizeof(TIMELAPSETHREAD));

	tlts->nTimeLapse--;

	return TRUE;
}

DWORD Find(TIMELAPSETHREADSET *tlts, CHAR *strID, DWORD *index)
{
	if(!tlts || !strID)
		return FALSE;

	DWORD i;

	for(i = 0; i < tlts->nTimeLapse; i++){
		if(!strcmp(strID, tlts->tlt[i].strID)){
			*index = i;
			return TRUE;
		}
	}

	return FALSE;
}