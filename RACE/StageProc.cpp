
#include "stdafx.h"
#include "StageProc.h"
#include "LaserMicroscope.h"




INT_PTR CALLBACK StageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int flgInit = FALSE;
	LM_STAGE *lpWnd = (LM_STAGE *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		if(lpWnd){
			return FALSE;
		}

		if(flgInit){
			return TRUE;
		}
		flgInit = TRUE;

		lpWnd = (LM_STAGE *) InitWindowLongRACE(hwnd, sizeof(LM_STAGE));

		PROPSHEETPAGE psp;
		memcpy(&psp, (VOID *)lParam, sizeof(PROPSHEETPAGE));
		lpWnd->hOwner = (HWND) psp.lParam;

		lpWnd->hCheckXAxis = GetDlgItem(hwnd, IDC_CHECK_XAXIS);
		lpWnd->hCheckYAxis = GetDlgItem(hwnd, IDC_CHECK_YAXIS);
		lpWnd->hCheckZAxis = GetDlgItem(hwnd, IDC_CHECK_ZAXIS);
		lpWnd->hCheckZDelay = GetDlgItem(hwnd, IDC_CHECK_ZDELAY);

		lpWnd->hEditXPos = GetDlgItem(hwnd, IDC_EDIT_XPOS);
		lpWnd->hEditYPos = GetDlgItem(hwnd, IDC_EDIT_YPOS);
		lpWnd->hEditZPos = GetDlgItem(hwnd, IDC_EDIT_ZPOS);
		lpWnd->hEditXVel = GetDlgItem(hwnd, IDC_EDIT_XVEL);
		lpWnd->hEditYVel = GetDlgItem(hwnd, IDC_EDIT_YVEL);
		lpWnd->hEditZVel = GetDlgItem(hwnd, IDC_EDIT_ZVEL);

		lpWnd->hEditXVelFine = GetDlgItem(hwnd, IDC_EDIT_XVEL_FINE);
		lpWnd->hEditXVelCoarse = GetDlgItem(hwnd, IDC_EDIT_XVEL_COARSE);
		lpWnd->hEditYVelFine = GetDlgItem(hwnd, IDC_EDIT_YVEL_FINE);
		lpWnd->hEditYVelCoarse = GetDlgItem(hwnd, IDC_EDIT_YVEL_COARSE);
		lpWnd->hEditZVelFine = GetDlgItem(hwnd, IDC_EDIT_ZVEL_FINE);
		lpWnd->hEditZVelCoarse = GetDlgItem(hwnd, IDC_EDIT_ZVEL_COARSE);

		lpWnd->hBtnZStack = GetDlgItem(hwnd, IDC_BTN_ZSTACK);

		lpWnd->hEditXVelAuto = NULL;
		lpWnd->hEditYVelAuto = NULL;
		lpWnd->hEditZVelAuto = NULL;

		Edit_SetFloat(lpWnd->hEditXPos, 0.0);
		Edit_SetFloat(lpWnd->hEditYPos, 0.0);
		Edit_SetFloat(lpWnd->hEditZPos, 0.0);

		lpWnd->xVelCoarse = 1000;
		lpWnd->yVelCoarse = 1000;
		lpWnd->zVelCoarse = 1000;
		lpWnd->xVelFine = 100;
		lpWnd->yVelFine = 100;
		lpWnd->zVelFine = 40;

		lpWnd->xPolStage = 1.0;
		lpWnd->yPolStage = 1.0;
		lpWnd->zPolStage = 1.0;
		lpWnd->xPolStick = 1.0;
		lpWnd->yPolStick = 1.0;
		lpWnd->zPolStick = 1.0;

		lpWnd->flgZDelay = TRUE;

		Alloc(lpWnd->pSet, 400);

		Edit_SetFloat(lpWnd->hEditXVelFine, lpWnd->xVelFine);
		Edit_SetFloat(lpWnd->hEditYVelFine, lpWnd->yVelFine);
		Edit_SetFloat(lpWnd->hEditZVelFine, lpWnd->zVelFine);
		Edit_SetFloat(lpWnd->hEditXVelCoarse, lpWnd->xVelCoarse);
		Edit_SetFloat(lpWnd->hEditYVelCoarse, lpWnd->yVelCoarse);
		Edit_SetFloat(lpWnd->hEditZVelCoarse, lpWnd->zVelCoarse);

		Button_SetCheck(lpWnd->hCheckZDelay, BST_CHECKED);

		Button_SetCheck(lpWnd->hCheckXAxis, BST_CHECKED);
		Button_SetCheck(lpWnd->hCheckYAxis, BST_CHECKED);
		lpWnd->xPolStage = lpWnd->xPolStage*-1.0;
		lpWnd->yPolStage = lpWnd->yPolStage*-1.0;

		SendMessage(lpWnd->hOwner, WM_SETHWND, (WPARAM) HWND_STAGE, (LPARAM)hwnd);

		return FALSE;
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case BN_CLICKED:
			switch(LOWORD(wParam))
			{
			case IDC_CHECK_XAXIS:
				lpWnd->xPolStage = lpWnd->xPolStage*-1.0;
				break;
			case IDC_CHECK_YAXIS:
				lpWnd->yPolStage = lpWnd->yPolStage*-1.0;
				break;
			case IDC_CHECK_ZAXIS:
				lpWnd->zPolStage = lpWnd->zPolStage*-1.0;
				break;
			case IDC_CHECK_ZDELAY:
				{
					LRESULT result;
					result = Button_GetCheck(lpWnd->hCheckZDelay);
					if(BST_CHECKED == result){
						lpWnd->flgZDelay = TRUE;
					}else if(BST_UNCHECKED == result){
						lpWnd->flgZDelay = FALSE;
					}else{}
				}
				break;
			case IDC_BTN_ZSTACK:
				{
					SendMessage(lpWnd->hOwner, WM_ZSTACK, NULL, NULL);
					return TRUE;
				}
			default:
				break;
			}
			break;
		case BN_PUSHED:
			switch(LOWORD(wParam))
			{
			case IDC_CHECK_XAXIS:
				lpWnd->xPolStage = lpWnd->xPolStage*-1.0;
				break;
			case IDC_CHECK_YAXIS:
				lpWnd->yPolStage = lpWnd->yPolStage*-1.0;
				break;
			case IDC_CHECK_ZAXIS:
				lpWnd->zPolStage = lpWnd->zPolStage*-1.0;
				break;
			default:
				break;
			}
			break;	
		case EN_CHANGE:
			switch(LOWORD(wParam))
			{
			case IDC_EDIT_XPOS:
				break;
			case IDC_EDIT_YPOS:
				break;
			case IDC_EDIT_ZPOS:
				break;
			case IDC_EDIT_XVEL_FINE:
				{
					lpWnd->xVelFine = Edit_GetFloat(lpWnd->hEditXVelFine);
					lpWnd->xVelFine = (FLOAT)lpWnd->xVelFine;
				}
				break;
			case IDC_EDIT_YVEL_FINE:
				{
					lpWnd->yVelFine = Edit_GetFloat(lpWnd->hEditYVelFine);
					lpWnd->yVelFine = (FLOAT)lpWnd->yVelFine;
				}
				break;
			case IDC_EDIT_ZVEL_FINE:
				{
					lpWnd->zVelFine = Edit_GetFloat(lpWnd->hEditZVelFine);
					lpWnd->zVelFine = (FLOAT)lpWnd->zVelFine;
				}
				break;
			case IDC_EDIT_XVEL_COARSE:
				{
					lpWnd->xVelCoarse = Edit_GetFloat(lpWnd->hEditXVelCoarse);
					lpWnd->xVelCoarse = (FLOAT)lpWnd->xVelCoarse;
				}
				break;
			case IDC_EDIT_YVEL_COARSE:
				{
					lpWnd->yVelCoarse = Edit_GetFloat(lpWnd->hEditYVelCoarse);
					lpWnd->yVelCoarse = (FLOAT)lpWnd->yVelCoarse;
				}
				break;
			case IDC_EDIT_ZVEL_COARSE:
				{
					lpWnd->zVelCoarse = Edit_GetFloat(lpWnd->hEditZVelCoarse);
					lpWnd->zVelCoarse = (FLOAT)lpWnd->zVelCoarse;
				}
				break;
			default:
				break;
			}
		}
	case WM_SETHWND:
		switch(wParam)
		{
		case HWND_OWNER:
			lpWnd->hOwner = (HWND) lParam;
			return TRUE;
		default:
			return FALSE;
		}
	default:
		return FALSE;
	}


	return FALSE;

}