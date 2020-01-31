#include "stdafx.h"

#include "DisplayProc.h"
#include "ToolsWIN32.h"

INT_PTR CALLBACK DisplayProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int flgInit = FALSE;
	LM_DISPLAY *lpWnd = (LM_DISPLAY *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

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

		lpWnd = (LM_DISPLAY *) InitWindowLongRACE(hwnd, sizeof(LM_DISPLAY));

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpWnd->hOwner = (HWND) psp.lParam;

		Alloc(&lpWnd->os, 100);

		lpWnd->hEditDiameter = GetDlgItem(hwnd, IDC_EDIT_DIAMETER);
		lpWnd->hEditPixPerUm = GetDlgItem(hwnd, IDC_EDIT_PIXPERUM);
		lpWnd->hEditX = GetDlgItem(hwnd, IDC_EDIT_X);
		lpWnd->hEditY = GetDlgItem(hwnd, IDC_EDIT_Y);
		lpWnd->hCheckA = GetDlgItem(hwnd, IDC_CHECK_A);
		lpWnd->hCheckB = GetDlgItem(hwnd, IDC_CHECK_B);
		lpWnd->hCheckX = GetDlgItem(hwnd, IDC_CHECK_X);
		lpWnd->hCheckY = GetDlgItem(hwnd, IDC_CHECK_Y);

		Edit_SetFloat(lpWnd->hEditPixPerUm, 4.8);
		Edit_SetFloat(lpWnd->hEditDiameter, 10);

		//Button_SetCheck(lpWnd->hCheckA, BST_CHECKED);

		SendMessage(lpWnd->hOwner, WM_SETHWND, (WPARAM)HWND_DISPLAY, (LPARAM)hwnd);

		return FALSE;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
				switch(LOWORD(wParam))
				{
				case IDC_CHECK_A:
					UpdateCheckFlag(lpWnd->hCheckA, &lpWnd->flgAPosition);
					break;
				case IDC_CHECK_B:
					UpdateCheckFlag(lpWnd->hCheckB, &lpWnd->flgBPosition);
					break;
				case IDC_CHECK_X:
					UpdateCheckFlag(lpWnd->hCheckX, &lpWnd->flgXPosition);
					break;
				case IDC_CHECK_Y:
					UpdateCheckFlag(lpWnd->hCheckY, &lpWnd->flgYPosition);
					break;
				default:
					break;
				}
			break;
		case EN_CHANGE:
			{
				switch(LOWORD(wParam))
				{
				case IDC_EDIT_DIAMETER:
					lpWnd->roiDiameter = Edit_GetFloat(lpWnd->hEditDiameter);
					break;
				case IDC_EDIT_PIXPERUM:
					lpWnd->pixPerUm = Edit_GetFloat(lpWnd->hEditPixPerUm);
					UpdateFieldOfView(lpWnd);
					break;
				default:
					break;
				}
			}
		  break;
			default:
				break;
		}
		return TRUE;
	case WM_DATAIN:
		//if(!lpWnd->flgZStack)
		//	return FALSE;

		PDMFRAME *pdmf;
		pdmf = (PDMFRAME *) wParam;

		if(!lpWnd->pdmIn.lpvoid){
			AllocPDMFrame(&lpWnd->pdmIn, pdmf->pdmfh.width, pdmf->pdmfh.height, pdmf->pdmfh.typeData);
		}

		CopyPDMFrame(&lpWnd->pdmIn, (PDMFRAME *)wParam);

		lpWnd->flgFrameIn = TRUE;

		return TRUE;
	case WM_SETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			lpWnd->hOwner = (HWND) lParam;
			return TRUE;
		case HWND_DATAVIEW:
			lpWnd->hDataView = (HWND) lParam;
			SendMessage(lpWnd->hDataView, WM_GETFRAMESIZE, (WPARAM) &lpWnd->wDisplay, (LPARAM) &lpWnd->hDisplay);
			AllocPDMFrame(&lpWnd->pdmIn, lpWnd->wDisplay, lpWnd->hDisplay, DT_UCHAR);
			UpdateFieldOfView(lpWnd);
			CreateLink(lpWnd->hDataView, hwnd, FALSE);
			//SendMessage(lpWnd->hDataView, WM_CREATELINK, (WPARAM)hwnd, NULL);
			return TRUE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}


	return FALSE;

}

DWORD UpdateFieldOfView(LM_DISPLAY *lpDisplay)
{
	lpDisplay->fov.wFOV = (1.0/lpDisplay->pixPerUm*lpDisplay->wDisplay);
	lpDisplay->fov.hFOV = (1.0/lpDisplay->pixPerUm*lpDisplay->hDisplay);
	lpDisplay->xFOV = (1.0/lpDisplay->pixPerUm)*lpDisplay->wDisplay;
	lpDisplay->yFOV = (1.0/lpDisplay->pixPerUm)*lpDisplay->hDisplay;
	Edit_SetFloat(lpDisplay->hEditX, lpDisplay->xFOV);
	Edit_SetFloat(lpDisplay->hEditY, lpDisplay->yFOV);

	return TRUE;
}


		////lpWndUNO32->pixPerUm = 6.56; //Pixels per um
		//lpWnd->lpDisplay->pixPerUm = 4.8; //Pixels per um
		//lpWnd->lpSystem->pixPerUm = 4.8;
		//lpWnd->lpDisplay->roiDiameter = 10;



		//case HWND_DATAVIEW:
		//	lpWnd->hDataView = (HWND) lParam;
		//	SendMessage(lpWnd->hDataView, WM_GETFRAMESIZE, (WPARAM) &lpWnd->wFrame, (LPARAM) &lpWnd->hFrame);
		//	UpdateFieldOfView(lpWnd);
			//lpWnd->fov.wFOV = (1.0/lpWnd->pixPerUm*lpWnd->wFrame);
			//lpWnd->fov.hFOV = (1.0/lpWnd->pixPerUm*lpWnd->hFrame);
			//lpWnd->xFOV = (1.0/lpWnd->pixPerUm)*lpWnd->wFrame;
			//lpWnd->yFOV = (1.0/lpWnd->pixPerUm)*lpWnd->hFrame;
			//Edit_SetFloat(lpWnd->hEditX, lpWnd->xFOV);
			//Edit_SetFloat(lpWnd->hEditY, lpWnd->yFOV);