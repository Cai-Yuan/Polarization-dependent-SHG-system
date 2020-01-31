#include "stdafx.h"

#include "Axiovert200Dlg.h"
#include "winuser.h"
#include "resource.h"
#include "Winbase.h"

INT_PTR CALLBACK Axiovert200Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	AXIOVERT *lpAxiovert = (AXIOVERT *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpAxiovert = (AXIOVERT *)calloc(1, sizeof(AXIOVERT));
		memset(lpAxiovert, 0x00, sizeof(AXIOVERT));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpAxiovert);
		lpAxiovert = (AXIOVERT *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(!lpAxiovert){
			ThrowMessage(hwnd, "DlgAxiovert200: Error Getting Long Ptr");
			ThrowMessage(hwnd, "DlgAxiovert200: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpAxiovert->hBtnOK = GetDlgItem(hwnd, IDC_BTN_OK);
		lpAxiovert->hBtnCancel = GetDlgItem(hwnd, IDC_BTN_CANCEL);

		lpAxiovert->hTab = CreateWindow(WC_TABCONTROL, "",
										  WS_CHILD
										| WS_CLIPSIBLINGS
										| WS_VISIBLE,
										0, 0, 100, 100,
										hwnd,
										NULL, 
										GetModuleHandle(0), 
										NULL);

		lpAxiovert->hTab = lpAxiovert->hTab;

		OnTabbedDialogInit(hwnd, lpAxiovert->hTab);

		break;

	case WM_SETHWND:

		if(wParam == HWND_OWNER){
			lpAxiovert->hOwner = (HWND) lParam;
		}	
		return TRUE;
	case WM_CLOSE:

		//SendMessage(lpStageProp->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_STAGEPROPERTIES), NULL);

		return TRUE;
	default:
		return FALSE;
	}


	return FALSE;

}

HRESULT OnTabbedDialogInit(HWND hwnd, HWND hTab)
{

	DWORD dwDlgBase = GetDialogBaseUnits();

	INT cxMargin = LOWORD(dwDlgBase)/4;
	INT xyMargin = HIWORD(dwDlgBase)/8;

	TCITEM tabItem = {0};
	tabItem.mask = TCIF_TEXT|TCIF_IMAGE;
	tabItem.iImage = -1; //No Image

	tabItem.pszText = "Experiment";
	TabCtrl_InsertItem(hTab, 0, &tabItem);
	tabItem.pszText = "AutoFocus";
	TabCtrl_InsertItem(hTab, 1, &tabItem);


	return TRUE;
}

DLGTEMPLATEEX DoLockDlgResource(CHAR *strResourceName)
{
	DLGTEMPLATEEX dlgTemplateEx = {0};
	//HRSCR hrscr = FindResource(NULL, strResrouceName, RT_DIALOG);

	//HGLOBAL hGlobal = LoadResrouce(GetModuleHandle(0), hrscr);

	//return (DLGTEMPLATEEX ) LockResrouce(hglb);
	return (DLGTEMPLATEEX) dlgTemplateEx;
}