#include "stdafx.h"

#include "RACE.h"
#include "RebarRACE.h"

HWND BuildRebar(HWND hOwner, HWND hToolBar)
{	
	LRESULT lResult = NULL;

	HWND	hRebar = NULL;
	HWND	hCombo = NULL;	
	HWND	hComboBox = NULL;

	UINT toolBarSize;
	RECT rcCombo;
	RECT rcComboBox;
	RECT rcOwner;

	InitializeCommonControlsRACE(NULL);

	hRebar = CreateRebar(hOwner);

	if(!hRebar){
		ThrowError(hOwner, "Create Rebar Failed");
		ThrowLastError(hOwner);
	}


	//if(!BuildCameraRebarBand(hOwner, hRebar)){
	//	ThrowError(hOwner, "Build Camera Bar Failed");
	//	ThrowLastError(hOwner);
	//}

	BuildFileRebarBand(hOwner, hRebar);


	GetWindowRect(hOwner, &rcOwner);

	MoveWindow(hRebar, 
				0, 0, 
				rcOwner.right/*-rcOwner.left*/, 32,
				TRUE);

	ShowWindow(hRebar, SW_SHOW);


	return hRebar;

}

DWORD BuildCameraRebarBand(HWND hOwner, HWND hRebar)
{
	INT i = NULL;
	CHAR str[MAX_STRING];
	LRESULT lResult = NULL;
	HWND hComboBox = NULL;
	RECT rcCombo = {0};

	InitializeCommonControlsRACE(NULL);

	hComboBox = CreateWindowEx(NULL,
							WC_COMBOBOX,
							TEXT("ComboBox"),
							CBS_DROPDOWN|
							CBS_HASSTRINGS|
							CBS_AUTOHSCROLL|
							WS_CHILD|
							WS_OVERLAPPED|
							WS_VISIBLE,
							20, 5, 300, 100,
							hRebar,
							(HMENU) IDC_COMBOBOX,
							GetModuleHandle(NULL),
							NULL);
	if(!hComboBox)
		ThrowLastError(hOwner);

	GetWindowRect(hComboBox, &rcCombo);

	REBARBANDINFO rbBandInfo = {0};
	rbBandInfo.cbSize = sizeof(REBARBANDINFO);
	rbBandInfo.fMask		= RBBIM_SIZE  
							//| RBBIM_ID 
							| RBBIM_STYLE 
							| RBBIM_TEXT  
							| RBBIM_CHILD;

	rbBandInfo.cx           = 5000;
	rbBandInfo.fStyle       = RBBS_GRIPPERALWAYS 
							| RBBS_VARIABLEHEIGHT
							| RBBS_CHILDEDGE;

	rbBandInfo.wID          = IDC_SELECTDEVICE;
	rbBandInfo.hwndChild    = hComboBox;
	rbBandInfo.lpText       = "ComboBox";
	rbBandInfo.cch          = MAX_STRING;

	SetLastError(NULL);
	lResult = SendMessage(hRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)(LPREBARBANDINFO)&rbBandInfo);
	
	SendMessage(hComboBox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	
	for(i = 0; i < 5; i++){
		sprintf(str, "Device %d", i+1);
		if(SendMessage(hComboBox, CB_ADDSTRING, NULL, (LPARAM)str) == CB_ERR)
		{
			ThrowLastError(hOwner);
		}
		else
			ThrowMessage(hOwner, str);
	}
	SendMessage(hComboBox, CB_SETCURSEL, 2, NULL);

	return lResult;
}

DWORD BuildFileRebarBand(HWND hOwner, HWND hRebar)
{
	RECT rcToolBar = {0};
	HWND hToolBar = NULL;
	
	REBARBANDINFO rbbi = {0};

	LRESULT lResult = NULL;
	CHAR str[MAX_STRING];

	InitializeCommonControlsRACE(NULL);

	hToolBar = CreateToolBar(hRebar);
	SendMessage(hToolBar, WM_SIZE, NULL, NULL);
	GetWindowRect(hToolBar, &rcToolBar);
	sprintf(str, "%s", "FileBand");

	memset(&rbbi, NULL, sizeof(REBARBANDINFO));
	rbbi.cbSize = sizeof(REBARBANDINFO);
	rbbi.fMask =  RBBIM_SIZE
				| RBBIM_CHILD
				| RBBIM_CHILDSIZE
				//| RBBIM_ID
				| RBBIM_STYLE
				//| RBBIM_TEXT
				;

	DWORD buttonSize = SendMessage(hToolBar, TB_GETBUTTONSIZE, NULL, NULL);
	DWORD wButton = LOWORD(buttonSize);
	DWORD hButton = HIWORD(buttonSize);
	DWORD nButton = SendMessage(hToolBar, TB_BUTTONCOUNT, NULL, NULL);
	rbbi.hwndChild = hToolBar;
	rbbi.cx = (nButton+1)*wButton;
	rbbi.cyMaxChild = hButton + (hButton*0.2);
	rbbi.cxMinChild = (nButton+2)*wButton;
	rbbi.cyMinChild = hButton;
	rbbi.fStyle = RBBS_CHILDEDGE
				//| RBBS_NOGRIPPER
				| RBBS_GRIPPERALWAYS
				| RBBS_BREAK
				//| RBBS_USECHEVRON
				//| RBBS_VARIABLEHEIGHT
				;
	rbbi.lpText = str;
	rbbi.cch = MAX_STRING;

	rbbi.cyChild = rcToolBar.bottom-rcToolBar.top;

	SetLastError(NULL);
	lResult = SendMessage(hRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbbi);

	if(!lResult){
		ThrowError(hOwner, "Build File Bar Failed");
		ThrowLastError(hOwner);
	}
	
	return lResult;
}



HWND CreateRebar(HWND hOwner)
{

	REBARINFO rbInfo = {0};//Contains information that describes rebar control characteristics.

	HWND hRebar = NULL;
	DWORD rebarStyles = NULL;
	
	rbInfo.cbSize = sizeof(REBARINFO);
	rbInfo.fMask = 

	//rebarStyles = 	WS_VISIBLE | 
	//WS_BORDER |
	//WS_CHILD | 
	//WS_CLIPCHILDREN |
	//WS_CLIPSIBLINGS | 
	//RBS_VARHEIGHT | 
	//RBS_BANDBORDERS |
	//CCS_NODIVIDER | 
	//CCS_NOPARENTALIGN |
	//CCS_TOP| //g_wSide is odd if this is a vertical bar
	//0;

	rebarStyles = WS_VISIBLE  
				| WS_BORDER 
				| WS_CHILD
				//| WS_CLIPCHILDREN
				//| WS_CLIPSIBLINGS
				//| RBS_VARHEIGHT
				| RBS_BANDBORDERS
				| CCS_NORESIZE
				//| CCS_NODIVIDER
				//| RBS_AUTOSIZE
				;

	hRebar = CreateWindowEx(WS_EX_TOOLWINDOW, 
							REBARCLASSNAME, 
							NULL, 
							rebarStyles,
							0,0,350,50, 
							hOwner, 
							(HMENU)IDC_REBAR, 
							GetModuleHandle(0), 
							NULL );

	//SendMessage(hRebar, RB_SETBKCOLOR, NULL, (LPARAM) COLORREF(RGB(255, 100, 0)));
	HIMAGELIST himlRebar;
	HICON		hIcon;

	himlRebar = ImageList_Create(16, 6, ILC_COLORDDB | ILC_MASK, 1, 0); //Creates a new image list.
	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PLAY));

	ImageList_AddIcon(himlRebar, hIcon);

	rbInfo.cbSize = sizeof(REBARINFO);
	rbInfo.fMask = RBIM_IMAGELIST;
	rbInfo.himl = himlRebar;
	if(!SendMessage(hRebar, RB_SETBARINFO, 0, (LPARAM) &rbInfo))
		ThrowLastError(hOwner);


	return hRebar;
}
