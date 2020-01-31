// RACE.cpp : Defines the entry point for the application.
//	RACE - Rapid Algorithm Computing Environement

/*
R-apid
A-lgorithm
C-omputing
E-nvionment
*/

//x32
//Inlucde
//C:\Program Files (x86)\GnuWin32\include

//Library
//C:\Program Files (x86)\GnuWin32\lib

//Source
//C:\Program Files (x86)\GnuWin32\bin

//SetWindowSubclass to capture MDI Client cursor events

#include "stdafx.h"
#include "Shlwapi.h"
#include "Shellapi.h"
#include "RACE.h"
#include "Menu.h"
#include "CommCtrl.h"
#include "Manifest.h"
#include "RegisterWnd.h"

#include "ToolsWIN32.h"
#include "GetFolderPath.h"
#include "PropertyManager.h"
#include "CommandPrompt.h"
#include "time.h"
#include "Iterations.h"
#include "ProgressView.h"
#include "HistogramView.h"
#include "String.h"
#include "WinUser.h"
#include "XBoxLinker.h"
#include "GaussianBlurr.h"
#include "AlgorithmWizard.h"
#include "AlgorithmTree.h"
#include "ZeissCam.h"
#include "CameraProperties.h"
#include "ReBarRACE.h"
#include "ZeissMicroscope.h"
#include "StageProperties.h"
#include "CellStretcher.h"
#include "ZStack.h"
#include "Mosaic.h"
#include "MDIProc.h"
#include "ListView.h"
#include "DataFilter.h"
#include "ParticleTracker.h"

#include "ParticleTrackerDlg.h"
#include "LaserMicroscope.h"

//#include "CUDAFunDll.h"

#include "ExperimentOptions.h"
#include "Axiovert200Dlg.h"
//#include "ReefWatchdogDlg.h"
//#include "ReefWatchdog.h"
#include "MosaicDlg.h"
#include "MultiChannelDlg.h"
#include "ZStackDlg.h"
#include "AutoFocusDlg.h"
#include "ZeissObjectiveDlg.h"

#include "ReefWatchdog.h"
#include "ReefWatchdogDevice.h"
#include "ReefWatchdogLighting.h"

#include "UNO32.h"
#include "EnumerateDevice.h"
#include "SC10Shutter.h"

//#include "A3200.h"

#include "FloodFill.h"
#include "Region.h"
#include "TimeLapse.h"

#include "MotorWatchdog.h"

//#include "DataView3D.h"

#define MAX_LOADSTRING 100
#define NUMBUTTONS 3

HWND CreateMDIDataView3D(HWND hOwner, CHAR *strTitle);
LRESULT CALLBACK DataView3DProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


HWND CreateRebar(HWND hwndOwner, HWND hwndToolbar, HWND hwndCombo);
//HWND BuildRebar(HWND hwndParent);
HWND BuildRebar(HWND hOwner, HWND hToolBar);
HWND CreateRebar(HWND hOwner);
LRESULT AddRebarBand(HWND hRebar, HWND hChild, UINT wID, CHAR *lpText);
LRESULT SaveWindowPositions(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK DataTabSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DataBarSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Temp. -- MoveToHeader
HWND CreatePropertySheet(HWND hOwner);
INT CALLBACK PropSheetProc(HWND hwnd, UINT msg, LPARAM lParam);
HWND CreateLaserMicroscopePropertySheet(HWND hOwner);

// Global Variables:
HINSTANCE hInst;								// current instance
HWND gg_hwnd;
HWND g_hMDIClient;								// MDI Client Handle
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


////////////////////////////////////////////////////////////////////////////////


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	INT argcp;
	LPWSTR *argv;

	argv = CommandLineToArgvW((LPWSTR)lpCmdLine, &argcp); //用于接收命令行参数的

	InitializeCommonControlsRACE(NULL); //注册一些扩展控件

	RegisterMDIChild(hInstance, WndTextEditorProc, "TextEditor");//MDI 窗口类的注册
	RegisterMDIChild(hInstance, DataManagerProc, "DataManager");
	RegisterMDIChild(hInstance, DataViewProc, "DataView");
	RegisterMDIChild(hInstance, DataView3DProc, "DataView3D");
	RegisterMDIChild(hInstance, CommandPromptProc, "CommandPrompt");
	RegisterMDIChild(hInstance, ProgressViewProc, "ProgressView");
	RegisterMDIChild(hInstance, HistogramViewProc, "HistogramView");
	RegisterMDIChild(hInstance, ZeissMicroscopeProc, "ZeissMicroscope");
	RegisterMDIChild(hInstance, ListViewProc, "ListView");
	RegisterMDIChild(hInstance, ReefWatchdogProc, "ReefWatchdog");
	RegisterMDIChild(hInstance, DataFilterProc, "DataFilter");
	RegisterMDIChild(hInstance, ParticleTrackerProc, "ParticleTracker");
	RegisterMDIChild(hInstance, LaserMicroscopeProc, "LaserMicroscope");


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);//载入一些全球通用的字符
	LoadString(hInstance, IDC_RACE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);//注册父窗口的窗口类函数

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) //生成主窗口，并且验证是否成功
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RACE));//装在加速键表

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateMDISysAccel(g_hMDIClient, &msg))
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (INT) msg.wParam;

}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:

//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NEURON2/*IDI_RACE*/));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RACE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_NEURON2/*IDI_SMALL*/));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   //hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);


   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, SW_HIDE);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//GetWindowPointer
	LPTRFRAME *lpFrame = (LPTRFRAME *) GetWindowLongPtr(hWnd, GWLP_USERDATA);

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HANDLE hThread;
	CREATEDATAMANAGER cdm;
	HWND hChild;

	void *temp;
	temp = malloc(sizeof(LPTRFRAME));
	temp = (LPTRFRAME *)temp;

	
	switch (message)
	{

	/*
	******************************************CUSTOM-MESSAGE HANDLING******************************************
	*/
	//wParam = ClassName
	//lParam = WindowName
	case WM_CREATEDIALOG:
		{
			HWND hDlg = NULL;
			hDlg = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE((INT)wParam), hWnd, (DLGPROC)lParam);
			SetDialogParent(hDlg, lpFrame->hMDIClient);
			return (LRESULT)hDlg;
		}
	case WM_GETMODULEHANDLE:
		return (LRESULT) GetModuleHandle(NULL);
	case WM_SETTEXT:
		DefFrameProc(hWnd, lpFrame->hMDIClient, message, wParam, lParam);
		SendMessage(hWnd, WM_REGISTERWINDOW, (WPARAM) hWnd, lParam);
		break;
	case WM_CREATEMDICHILD:
		hChild = CreateMDIChild(lpFrame->hMDIClient, (CHAR *) wParam, (CHAR *) lParam);
		if(!hChild){
			CHAR str[MAX_STRING];
			sprintf(str, "Failed to Create %s['%s']", wParam, lParam);
			ThrowError(hWnd, str);
		}
		else
			SendMessage(hChild, WM_SETHWND, (WPARAM) HWND_OWNER, (LPARAM) hWnd);

		return (LRESULT) hChild;
	case WM_REGISTERWINDOW:
		{
			OnRegisterWindowRACE(lpFrame, hWnd, wParam, lParam);
		}
		return TRUE;
	case WM_GETMDIMANIFEST:
		return (LRESULT) lpFrame->hManifest;
	case WM_GETMDICLIENT:
		{
			//Using the Child-Window Identifier
			HWND hMDIClient;
			hMDIClient = (HWND) GetWindowLong(hWnd, (-8)/*GWL_HWNDPARENT*/);
		}
		return (LRESULT) lpFrame->hMDIClient;
	case WM_THROWERROR:
		{
			InsertNewLineManifest(lpFrame->hManifest, (CHAR *)wParam);
			return (LRESULT) TRUE;
		}
	case WM_GETARCHIVEDIR:
		{
			sprintf((CHAR *)wParam, "%s", lpFrame->strArchivePath);
			return TRUE;
		}
	case WM_SETHWND:
		{
		switch(wParam)
		{
			case HWND_ACTIVE:
				lpFrame->hActive = (HWND) lParam;
				SendMessage(lpFrame->hDataBar, WM_SETHWND, (WPARAM) HWND_ACTIVE, lParam);
				SendMessage(lpFrame->hPropertyManager, WM_SETHWND, (WPARAM) HWND_ACTIVE, lParam);
				SendMessage(lpFrame->hXBoxLinker, WM_SETHWND, (WPARAM) HWND_ACTIVE, lParam);
				SendMessage(lpFrame->hCameraProperties, WM_SETHWND, (WPARAM) HWND_ACTIVE, lParam);
				return (LRESULT) lpFrame->hActive;
			case HWND_ZEISSMICROSCOPE:
				lpFrame->hZeissMicroscope = (HWND) lParam;
				SendMessage(lpFrame->hStageProperties, message, wParam, lParam);
				return (LRESULT) lParam;
			case HWND_REEFWATCHDOG:
				lpFrame->hReefWatchdog = (HWND) lParam;
				return (LRESULT) lParam;
			case HWND_SC10:
				lpFrame->hSC10Linker = (HWND) lParam;
				return (LRESULT) lParam;
			case HWND_CAMERAPROPERTIES:
				lpFrame->hCameraProperties = (HWND) lParam;
				return (LRESULT)lParam;
			default:
				return FALSE;
			}
		}
		return FALSE;
	case WM_GETHWND:
		{
			switch(wParam)
			{			
			case HWND_FRAME:
				return (LRESULT) hWnd;
			case HWND_ACTIVE:
				return (LRESULT) lpFrame->hActive;
			case HWND_XBOXLINKER:
				return (LRESULT) lpFrame->hXBoxLinker;
			case HWND_MDI:
				return (LRESULT) lpFrame->hMDIClient;
			case HWND_CAMERAPROPERTIES:
				return (LRESULT) lpFrame->hCameraProperties;
			case HWND_STAGECONTROLS:
				return (LRESULT) lpFrame->hStageProperties;
			case HWND_CELLSTRETCHER:
				return (LRESULT) lpFrame->hCellStretcher;
			case HWND_ZEISSMICROSCOPE:
				return (LRESULT) lpFrame->hZeissMicroscope;
			case HWND_REEFWATCHDOG:
				return (LRESULT) lpFrame->hReefWatchdog;
			case HWND_UNO32:
				return (LRESULT) lpFrame->hUNO32;
			case HWND_SC10:
				return (LRESULT) lpFrame->hSC10Linker;
			case HWND_LASERMICROSCOPE:
				return (LRESULT) lpFrame->hLaserMicroscope;
			case HWND_TIMELAPSE:
				return (LRESULT) lpFrame->hTimeLapse;
			case HWND_TIMELAPSEPROPERTIES:
				return (LRESULT) lpFrame->hTimeLapseProp;
			default:
				break;
			}
		}
		return NULL;
	case WM_SETFRAMEFOCUS:
		{
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			return TRUE;
		}
	case DV_SETFRAMEFOCUS:
		{
			SendMessage(lpFrame->hActive, WM_SETFRAMEFOCUS, wParam, lParam);
			return TRUE;
		}
	case CB_SETCURSEL:
		{
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			break;
		}
	//WPARAM = *FILEPATHSET
	case DB_ADDSTRINGSET:
		{
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			return TRUE;
		}
	/*
	******************************************BASIC-MESSAGE HANDLING******************************************
	*/
	case WM_KEYDOWN:
		{
			INT p = 0;
			p = 0; 
			p = 0;
			if(wParam == VK_F2){
			
				INT i  = NULL;
				i = 0;
				i = 0;
				return FALSE;
			}
			if(wParam == VK_S){
				INT i = NULL;
				i = 0;
				i = 0;
				return FALSE;
			}
		}
		break;
	case WM_COMMAND:
		return OnCommandRACE(lpFrame, hWnd, message, wParam, lParam);
	case PBM_SETPOS:
		{
			INT prevPos;
			prevPos = (INT) SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			return prevPos;
		}
	case PBM_SETRANGE:
		{
			INT rLower;
			INT rUpper;
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			rLower = LOWORD(lParam);
			rUpper = HIWORD(lParam);
			rUpper = rUpper;
		}
		return TRUE;
	case PBM_GETRANGE:
		{
			return SendMessage(lpFrame->hDataBar, message, wParam, lParam);
		}
	case PBM_SETSTEP:
		{
			return SendMessage(lpFrame->hDataBar, message, wParam, lParam);
		}
	case PBM_STEPIT:
		{
			return SendMessage(lpFrame->hDataBar, message, wParam, lParam);
		}
	case WM_NOTIFY:
		{
			break;
		}
	case WM_ENDTRANSMISSION:
		{
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
		}
		break;
	case RM_SETTEXT:
		{
			SendMessage(lpFrame->hStatusBar, SB_SETTEXT, wParam, lParam);
		}
		return TRUE;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_SIZE:
		{
			OnSizeRACE(lpFrame, hWnd, message, wParam, lParam);
		}
		break;
	case WM_UPDATEFRAME:
		{
			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			SendMessage(hWnd, WM_SIZE, NULL, MAKELPARAM((rcClient.right-rcClient.left), (rcClient.bottom-rcClient.top)));
		}
		break;
	case WM_CREATE:
		{
			//ShowWindow(hWnd, SW_HIDE);
			//UpdateWindow(hWnd);
			OnCreateRACE(lpFrame, hWnd, wParam, lParam);
			break;
		}
	case WM_NCCREATE:
		{
			//Allocate memory for the window pointer
			lpFrame = (LPTRFRAME *)calloc(1, sizeof(LPTRFRAME));//分配一个内存
			SetWindowLongPtr(hWnd, (-21)/*GWL_USERDATA*/, (LONG_PTR)lpFrame);//将这个空间与窗口句柄关联，也可以改变窗口的属性
			lpFrame = (LPTRFRAME *) GetWindowLongPtr(hWnd, (-21)/*GWL_USERDATA*/);//获取窗口的某个属性，或者是相关联的空间

			//Validate that setting the Window Long Ptr worked
			if(lpFrame == NULL){
				MessageBox(hWnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "Error!", MB_OK);
			}
		}
		if(lpFrame)
			return DefFrameProc(hWnd, g_hMDIClient/*lpFrame->hMDIClient*/, message, wParam, lParam); //该函数对任何多文档接口（MDI）框架窗口的窗口过程不处理的窗口消息提供缺省处理。
		else
			return NULL;


	case WM_DESTROY:
		{
			//SAVEWINDOWPOSITIONS is reading null pointer
			//SaveWindowPositions(lpFrame, hWnd, wParam, lParam);
			SendMessage(lpFrame->hCameraProperties, WM_DESTROY, NULL, NULL);
			SendMessage(lpFrame->hUNO32, WM_DESTROY, NULL, NULL);
			SendMessage(lpFrame->hXBoxLinker, WM_DESTROY, NULL, NULL);
			PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		{
			if(MessageBox(hWnd, "Close RACE?", "RACE", MB_OKCANCEL) == IDOK){//关闭主窗口时的过程
				PostMessage(hWnd, WM_DESTROY, NULL, NULL);
				return FALSE;
			}
			//SendMessage(lpFrame->hCameraProperties, WM_DESTROY, NULL, NULL);
			
			//SendMessage(lpFrame->hUNO32, WM_DESTROY, NULL, NULL);
			//SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}
	case WM_CLOSING:
		{
			OnWindowClosing(lpFrame, hWnd, wParam, lParam);//关闭窗口时，记录窗口的文字及位置等
		}
		return TRUE;
	default:
		//Use 'DefFrameProc' Instead of 'DefWindowProc' when using MDI format
		if(lpFrame)
			return DefFrameProc(hWnd, lpFrame->hMDIClient, message, wParam, lParam);
		else                            //该函数对任何多文档接口（MDI）框架窗口的窗口过程不处理的窗口消息提供缺省处理。
			                            //窗口过程不能明确处理的所有窗口消息必须通过DefFrameProc函数处理，而不是通过DefWindowProc函数。
			return NULL;
		
		
	}
	return 0;
}

LRESULT OnSizeRACE(LPTRFRAME *lpFrame, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HWND hClient;
	HWND hReBar;
	HWND hStatusBar;

	RECT rcStatusBar;
	RECT rcClient;
	RECT rcManifest;

	INT heightReBar;
	INT heightStatusBar;
	INT heightClient;
	INT widthClient;
	INT wManifest;
	INT hManifest;
	INT hDataBar;
	INT wDataBar;

	INT wTool = 30;

	GetClientRect(hWnd, &rcClient);

	//hReBar = lpFrame->hRebar;
	//hStatusBar = lpFrame->hStatusBar;

	//Tool Bar
	//SendMessage(lpFrame->hToolBar, WM_SIZE, NULL, NULL);
	RECT rcRebar  = {0};
	GetWindowRect(lpFrame->hRebar, &rcRebar);
	MoveWindow(lpFrame->hRebar, 
				0, 0, 
				rcClient.right-rcClient.left, rcRebar.bottom-rcRebar.top,
				TRUE);

	//SendMessage(lpFrame->hRebar, WM_SIZE, NULL, NULL);
	heightReBar = GetWindowHeight(lpFrame->hToolBar);

	INT parts[5];
	INT xPerPart = (INT) ((FLOAT)(rcClient.right-rcClient.left)/(FLOAT)SBP_MAX);
	parts[0] = xPerPart;
	parts[1] = parts[0]+xPerPart;
	parts[2] = parts[1]+xPerPart;
	parts[3] = parts[2]+xPerPart;
	parts[4] = parts[3]+xPerPart;
	//parts[0] = -1;
	//parts[1] = -1;

	SendMessage(lpFrame->hStatusBar, SB_SETPARTS, (WPARAM) 5, (LPARAM) parts);

	//Status Bar
	//hStatusBar = GetDlgItem(hWnd, IDC_MAIN_STATUS);
	SendMessage(lpFrame->hStatusBar, WM_SIZE, NULL, NULL);
	heightStatusBar = GetWindowHeight(lpFrame->hStatusBar);
	
	heightClient = GetClientHeight(hWnd) - heightReBar - heightStatusBar;	

	SetWindowPos(lpFrame->hToolLeft, 
				HWND_TOP, 
				/*rcClient.left*/0, 
				heightReBar,
				wTool,
				heightClient,
				SWP_NOZORDER);
	
	SetWindowPos(lpFrame->hToolRight,
				HWND_TOP,
				rcClient.right-wTool,
				heightReBar,
				wTool,
				heightClient,
				SWP_NOZORDER);


	if(lpFrame->fManifest){	
		ShowWindow(lpFrame->hManifest, SW_SHOW);
		GetWindowRect(lpFrame->hManifest, &lpFrame->rcManifest);
		lpFrame->wManifest = lpFrame->rcManifest.right-lpFrame->rcManifest.left;
		wManifest = lpFrame->wManifest;		
		
		SetWindowPos(lpFrame->hManifest, 
				HWND_TOP, 
				(rcClient.right - wManifest/* - wTool*/), 
				heightReBar, 
				wManifest, 
				heightClient, 
				SWP_NOZORDER);
	}
	else{
		wManifest = 0;
		ShowWindow(lpFrame->hManifest, SW_HIDE);
	}

	if(lpFrame->fDataBar){
		ShowWindow(lpFrame->hDataBar, SW_SHOW);
		GetWindowRect(lpFrame->hDataBar, &lpFrame->rcDataBar);
		lpFrame->wDataBar = lpFrame->rcDataBar.right-lpFrame->rcDataBar.left;
		wDataBar = lpFrame->wDataBar;

		SetWindowPos(lpFrame->hDataBar,
					HWND_TOP,
					rcClient.left/* + wTool*/,
					heightReBar,
					wDataBar,
					heightClient,
					SWP_NOZORDER);
	}
	else{
		wDataBar = 0;
		ShowWindow(lpFrame->hDataBar, SW_HIDE);
	}

	if(lpFrame->fManifest)
		GetWindowRect(lpFrame->hManifest, &lpFrame->rcManifest);

	if(lpFrame->fDataBar)
		GetWindowRect(lpFrame->hDataBar, &lpFrame->rcDataBar);



	widthClient = GetClientWidth(hWnd) - wManifest - wDataBar;

	if(!SetWindowPos(g_hMDIClient, NULL, wDataBar/*+wTool*/, heightReBar, widthClient/*-(2*wTool)*/, heightClient, SWP_NOZORDER))
		ThrowMessage(hWnd, "Error Sizing MDI Client!");

	return NULL;

}

LRESULT CALLBACK MouseHookProc(INT nCode, WPARAM wParam, LPARAM lParam)
{

	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
	PMOUSEHOOKSTRUCT m = (PMOUSEHOOKSTRUCT)(lParam);
	POINT p;

	if(nCode < 0)	//Do no process message
		return CallNextHookEx(NULL, nCode, wParam, lParam);

	if(wParam == WM_RBUTTONDOWN)
	{
		//right button clicked
		GetCursorPos(&p);
		p.x = p.x;
		p.y = p.y;
	}
	if(wParam == WM_LBUTTONDOWN)
	{
		gg_hwnd = m->hwnd;
	}


	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD WINAPI DesktopCaptureThread(LPVOID param)
{
	if(!param)return FALSE;
	CAPTURE *c = (CAPTURE *)param;

	HWND hDesktop = GetDesktopWindow();
	HWND hOwner = c->hOwner;
	HWND hFrame = GetAncestor(hOwner, GA_ROOT);

	LONG w;
	LONG h;
	DWORD i;
	INT flgState = NULL;
	POINT ptUL;
	POINT ptLR;
	RECT rect;

	if(1){
		MessageBox(NULL, "Select Window", "Select Window", MB_OK);
		//MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(0), NULL);
	
		//Get Uppper Left Corner
		flgState = GetAsyncKeyState(VK_LBUTTON);
		while(!flgState){
			flgState = GetAsyncKeyState(VK_LBUTTON);
		}
		//flgState = NULL;
		GetCursorPos(&ptUL);

		//Get Lower Right Corner
		while(flgState){
			flgState = GetAsyncKeyState(VK_LBUTTON);
		}
		GetCursorPos(&ptLR);

	}

	HWND hPoint = GetDesktopWindow();

	//GetBitmapDimensions(hPoint, &w, &h);
	w = ptLR.x-ptUL.x;
	h = ptLR.y-ptUL.y;

	while(w%4){
		w++;
		ptLR.x++;
	}	
	
	PDMFRAME pdmf = {0};
	PDMFRAME pdmfSend = {0};
	AllocPDMFrame(&pdmf, w, h, DT_LONGPIXEL);
	AllocPDMFrame(&pdmfSend, w, h, DT_PIXEL);

	GetBitmap(hPoint, w, h, (PIXEL *)pdmf.lpvoid);
	HWND hDataView = CreateMDIDataView(/*(HWND)param*/hFrame, "Desktop View");
	DataView_SetDimensions(hDataView, w, h);
	DataView_ShowFrame(hDataView, &pdmfSend);
	ShowWindow(hDataView, SW_SHOW);


	for(i = 0; i < 20000; i++){
		GetBitmap(hPoint, w, h, ptUL, ptLR, (PIXEL *)pdmf.lpvoid);
		LONGtoPIXEL(w*h, (LONGPIXEL *)pdmf.lpvoid, (PIXEL *)pdmfSend.lpvoid);
		//FlipVertical(w, h, (PIXEL *)pdmfSend.lpvoid);

		DataView_ShowFrame(hDataView, &pdmfSend);
	}

	FreePDM(&pdmf);
	FreePDM(&pdmfSend);

	gg_hwnd = NULL;

	return TRUE;
}

DWORD WINAPI DekstopTestThread(LPVOID param)
{
	DWORD i = 0;
	LONG w;
	LONG h;
	//HHOOK MouseHook;
	INT flgState;
	POINT ptUL;
	POINT ptLR;
	RECT rect;

	if(1){
		MessageBox(NULL, "Select Window", "Select Window", MB_OK);
		//MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(0), NULL);
	
		//Get Uppper Left Corner
		flgState = GetAsyncKeyState(VK_LBUTTON);
		while(!flgState){
			flgState = GetAsyncKeyState(VK_LBUTTON);
		}
		GetCursorPos(&ptUL);

		//Get Lower Right Corner
		while(flgState){
			flgState = GetAsyncKeyState(VK_LBUTTON);
		}
		GetCursorPos(&ptLR);

	}

	HWND hPoint = GetDesktopWindow();

	//GetBitmapDimensions(hPoint, &w, &h);
	w = ptLR.x-ptUL.x;
	h = ptLR.y-ptUL.y;

	while(w%4){
		w++;
		ptLR.x++;
	}
		
	PDMFRAME pdmf = {0};
	PDMFRAME pdmfSend = {0};
	AllocPDMFrame(&pdmf, w, h, DT_LONGPIXEL);
	AllocPDMFrame(&pdmfSend, w, h, DT_PIXEL);


	GetBitmap(hPoint, w, h, (PIXEL *)pdmf.lpvoid);
	HWND hDataView = CreateMDIDataView((HWND)param, "Desktop View");
	DataView_SetDimensions(hDataView, w, h);
	DataView_ShowFrame(hDataView, &pdmfSend);
	ShowWindow(hDataView, SW_SHOW);


	for(i = 0; i < 20000; i++){
		GetBitmap(hPoint, w, h, ptUL, ptLR, (PIXEL *)pdmf.lpvoid);
		LONGtoPIXEL(w*h, (LONGPIXEL *)pdmf.lpvoid, (PIXEL *)pdmfSend.lpvoid);
		//FlipVertical(w, h, (PIXEL *)pdmfSend.lpvoid);

		DataView_ShowFrame(hDataView, &pdmfSend);
	}

	FreePDM(&pdmf);
	FreePDM(&pdmfSend);

	//UnhookWindowsHookEx(MouseHook);
	gg_hwnd = NULL;


	return TRUE;
}

LRESULT OnCommandRACE(LPTRFRAME *lpFrame, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wmId;
	INT wmEvent;

	PAINTSTRUCT ps;
	HDC hdc;
	HANDLE hThread;
	CREATEDATAMANAGER cdm;
	HWND hChild;

	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	// Parse the menu selections:
	switch (LOWORD(wParam))
	{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_NEW:

			if(0){
				//Create New "EDIT" Window
				HWND hManifest;
				hManifest = (HWND) SendMessage(hWnd, WM_CREATEMDICHILD, (WPARAM) "ListView"/*"TextEditor"*/, (LPARAM) "Text");
				ShowWindow(hManifest, SW_SHOW);
			}

			//Causes a crash
			if(0){
				HWND hDataView3D;
				hDataView3D = (HWND) SendMessage(hWnd, WM_CREATEMDICHILD, (WPARAM) "DataView3D", (LPARAM) "DataView3D - Test");
				//hDataView3D = CreateMDIDataView3D(hWnd, "DataView3D - Test");
				ShowWindow(hDataView3D, SW_SHOW);
			}
			break;
		case ID_FILE_SAVEAS:
			HWND hChild;
			hChild = (HWND) SendMessage(g_hMDIClient, WM_MDIGETACTIVE, NULL, NULL);
			SendMessage(hChild, WM_COMMAND, wParam, lParam);
			break;
		case ID_OPEN_FILE:
			//Create DataManager
			cdm.hOwner = hWnd;
			//cdm.hCallback = NULL;
			cdm.sourceData = DS_FILE;
			SendMessage(hWnd, WM_COMMAND, LOWORD(ID_VIEW_DATABAR), NULL);
			CreateThread(NULL, NULL, CreateDataManager, &cdm, NULL, NULL);
			Sleep(10);
			break;
		case ID_OPEN_STREAM:

			cdm.hOwner = hWnd;
			//cdm.hCallback = NULL;
			cdm.sourceData = DS_FRAMEGRABBER;
			CreateThread(NULL, NULL, CreateDataManager, &cdm, NULL, NULL);

			//lParam is a hWindow which needs the hDataManager handle
			//hDataManager hanlde is sent to hWindow after registration
			if(lParam)
				lpFrame->hReturn = (HWND) lParam;

			//Sleep(100);
			//SendMessage(hWnd, WM_COMMAND, LOWORD(ID_VIEW_CAMERAPROPERTIES), NULL);
			break;
		case ID_TOOLS_PARTICLETRACK:
			//HWND hDataManager;
			//hDataManager = (HWND) SendMessage(lpFrame->hActive, WM_GETHWND, HWND_DATAMANAGER, NULL);
			//SendMessage(hDataManager, WM_COMMAND, DMN_PARTICLETRACK, NULL);
			MessageBox(hWnd, "Tool Disabled!", "Tool Disabled", MB_OK);
			return TRUE;
		case ID_TOOLS_LOOP:
			//SendMessage(lpFrame->hActive, WM_COMMAND, DVN_LOOP, 500);
			MessageBox(hWnd, "Tool Disabled!", "Tool Disabled", MB_OK);
			return TRUE;
		case ID_TOOLS_CROP:
			SendMessage(lpFrame->hActive, WM_COMMAND, LOWORD(DVN_SETTOOL), TOOL_CROP);
			return TRUE;
		//case ID_TOOLS_ROIPOLY:
		//	SendMessage(lpFrame->hActive, WM_COMMAND, LOWORD(DVN_SETTOOL), NULL);
		//	return TRUE;
		case ID_FILE_PATHARCHIVE:
			{
				CHAR strArchive[MAX_STRING];
				CHAR strMessage[MAX_STRING*5];
				memset(strArchive, NULL, MAX_STRING*sizeof(CHAR));

				if(MessageBox(hWnd, "Please Select an Archive Directory", "Setup", MB_OKCANCEL) == IDOK){
					if(GetFolderPath(hWnd, strArchive)){
						sprintf(lpFrame->strArchivePath, "%s", strArchive);

						sprintf(strMessage, "%s%s", "Current Archive Folder: ", lpFrame->strArchivePath);
						ThrowMessage(hWnd, strMessage);
					}
					else{
						sprintf(strMessage, "%s%s", "Current Archive Folder: ", lpFrame->strArchivePath);
						ThrowMessage(hWnd, strMessage);
					}
				}

			}
			break;
		case ID_WINDOW_TILE_HORZ:
			SendMessage(lpFrame->hMDIClient, WM_MDITILE, MDITILE_HORIZONTAL, NULL);
			break;
		case ID_WINDOW_TILE_VERT:
			SendMessage(lpFrame->hMDIClient, WM_MDITILE, MDITILE_VERTICAL, NULL);
			break;
		case ID_WINDOW_CASCADE:
			SendMessage(lpFrame->hMDIClient, WM_MDICASCADE, NULL, NULL);
			break;
		case ID_VIEW_MANIFEST:
		//	{
				lpFrame->fManifest = !lpFrame->fManifest;
				ShowWindow(lpFrame->hManifest, SW_SHOW);
				SendMessage(hWnd, WM_UPDATEFRAME, NULL, NULL);

				//If Visible, check the menu item
				//else, uncheck the menu item
				HMENU hSubMenu;
				hSubMenu = NULL;
				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_VIEW);
				if(lpFrame->fManifest)
					CheckMenuItem(hSubMenu, MP_VIEW_MANIFEST, MF_BYPOSITION|MF_CHECKED);
				else
					CheckMenuItem(hSubMenu, MP_VIEW_MANIFEST, MF_BYPOSITION|MF_UNCHECKED);
		//	}
			break;		
		case ID_VIEW_ALGORITHMEDITOR:
			{
				ShowWindow(lpFrame->hAlgWizard, SW_SHOW);
			}
			break;
		case ID_VIEW_DATABAR:
			//{
				lpFrame->fDataBar = !lpFrame->fDataBar;
				
				//If Visible, check the menu item
				//else, uncheck the menu item
				hSubMenu = NULL;
				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_VIEW);
				if(lpFrame->fDataBar){
					CheckMenuItem(hSubMenu, MP_VIEW_DATABAR, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hDataBar, SW_SHOW);
					SendMessage(hWnd, WM_UPDATEFRAME, NULL, NULL);
				}
				else{
					CheckMenuItem(hSubMenu, MP_VIEW_DATABAR, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hDataBar, SW_HIDE);
					SendMessage(hWnd, WM_UPDATEFRAME, NULL, NULL);
				}
			//}
			break;
		case ID_VIEW_XBOXLINKER://简单
		//{
				lpFrame->fXBoxLinker = !lpFrame->fXBoxLinker;

				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_VIEW);
				if(lpFrame->fXBoxLinker){
					CheckMenuItem(hSubMenu, MP_VIEW_XBOXLINKER, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hXBoxLinker, SW_SHOW);
				}
				else{
					CheckMenuItem(hSubMenu, MP_VIEW_XBOXLINKER, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hXBoxLinker, SW_HIDE);
				}
		//	}
			break;
		case ID_VIEW_PROPERTYMANAGER://简单
		//	{
				lpFrame->fPropertyManager = !lpFrame->fPropertyManager;

				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_VIEW);
				if(lpFrame->fPropertyManager){
					CheckMenuItem(hSubMenu, MP_VIEW_PROPERTYMANAGER, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hPropertyManager, SW_SHOW);
				}
				else{
					CheckMenuItem(hSubMenu, MP_VIEW_PROPERTYMANAGER, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hPropertyManager, SW_HIDE);
				}
		//	}
			break;
		case ID_DEVICES_ZEISSMICROSCOPE:
			{
				lpFrame->fZeissAcqusition = !lpFrame->fZeissAcqusition;

				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_DEVICES);
				if(lpFrame->fZeissAcqusition){
					CheckMenuItem(hSubMenu, MP_DEVICES_ZEISSMICROSCOPE, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hZeissAcqusition, SW_SHOW);
				}
				else{
					CheckMenuItem(hSubMenu, MP_DEVICES_ZEISSMICROSCOPE, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hZeissAcqusition, SW_HIDE);
				}
			}
			break;
		case ID_VIEW_COMMANDPROMPT:
			ShowWindow(lpFrame->hCommandPrompt, SW_SHOW);
			break;
		case ID_VIEW_HISTOGRAMVIEW:
			ShowWindow(lpFrame->hHistogramView, SW_SHOW);
			break;
		case DBN_SETSTART:
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			break;
		case DBN_SETSTOP:
			SendMessage(lpFrame->hDataBar, message, wParam, lParam);
			break;
		case DVN_SETALGORITHM2:
			SendMessage(lpFrame->hActive, message, wParam, lParam);
			break;
		case ID_DEVICES_CAMERAPROPERTIES:

			{
				lpFrame->fCameraProperties = !lpFrame->fCameraProperties;

				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_DEVICES);
				if(lpFrame->fCameraProperties){
					CheckMenuItem(hSubMenu, MP_DEVICES_CAMERAPROPERTIES, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hCameraProperties, SW_SHOW);
				}
				else{
					CheckMenuItem(hSubMenu, MP_DEVICES_CAMERAPROPERTIES, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hCameraProperties, SW_HIDE);
				}
			}
			return TRUE;
		case ID_DEVICES_STAGEPROPERTIES:
			{
				lpFrame->fStageProperties = !lpFrame->fStageProperties;

				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_DEVICES);
				if(lpFrame->fStageProperties){
					CheckMenuItem(hSubMenu, MP_DEVICES_STAGEPROPERTIES, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hStageProperties, SW_SHOW);
				}
				else{
					CheckMenuItem(hSubMenu, MP_DEVICES_STAGEPROPERTIES, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hStageProperties, SW_HIDE);
				}

			}
			break;

		case ID_DEVICES_CELLSTRETCHER:
			{
				lpFrame->fCellStretcher = !lpFrame->fCellStretcher;

				hSubMenu = GetSubMenu(lpFrame->hMenu, MP_ROOT_DEVICES);
				if(lpFrame->fCellStretcher){
					CheckMenuItem(hSubMenu, MP_DEVICES_CELLSTRETCHER, MF_BYPOSITION|MF_CHECKED);
					ShowWindow(lpFrame->hCellStretcher, SW_SHOW);
				}
				else{
					CheckMenuItem(hSubMenu, MP_DEVICES_CELLSTRETCHER, MF_BYPOSITION|MF_UNCHECKED);
					ShowWindow(lpFrame->hCellStretcher, SW_HIDE);
				}

			}
			break;
		case ID_DEVICES_CAMERA_ZEISS:
			{
				return SendMessage(hWnd, WM_COMMAND, LOWORD(ID_VIEW_CAMERAPROPERTIES), NULL);
			}
		case ID_TOOLS_TIMELAPSE:
			{
				PositionDialog(lpFrame->hTimeLapse, lpFrame->hActive, NULL);
				ShowWindow(lpFrame->hTimeLapse, SW_SHOW);
				return TRUE;
			}
		case ID_TOOLS_ZSTACK:
			{
				//HWND *hActiveView;
				//hActiveView = (HWND *) malloc(sizeof(HWND));
				//*hActiveView = (HWND) SendMessage(hWnd, WM_GETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) NULL);
				//CreateThread(NULL, NULL, ZStackAlgorithm, hActiveView, NULL, NULL);
				MessageBox(hWnd, "Tool Disabled!", "Tool Disabled", MB_OK);
				return TRUE;
			}
		case ID_TOOLS_MOSAIC:
			{
				//HWND *hActiveView;
				//hActiveView = (HWND *) malloc(sizeof(HWND));
				//*hActiveView = (HWND) SendMessage(hWnd, WM_GETHWND, (WPARAM) HWND_ACTIVE, (LPARAM) NULL);
				//CreateThread(NULL, NULL, MosaicThread, hActiveView, NULL, NULL);
				MessageBox(hWnd, "Tool Disabled!", "Tool Disabled", MB_OK);
				return TRUE;
			}
		case ID_TOOLS_ROIPOLY:
			{
				SendMessage(lpFrame->hActive, WM_COMMAND, MAKEWPARAM(DVN_SETTOOL, TOOL_ROIPOLY), NULL);
				//SendMessage(lpFrame->hActive, WM_COMMAND, LOWORD(DVN_SETTOOL), TOOL_ROIPOLY);
				return TRUE;
			}

		case ID_TOOLS_ALGORITHM_FLOODFILL:
			{
				UINT w;
				UINT h;

				//Get hDataView
				//Ensure the Active Window is a 'DataView' Child
				if(!DataView_GetWidthHeight(lpFrame->hActive, &w, &h))
					return FALSE;


				if(w && h){
					HWND hDataView = lpFrame->hActive;
					INT i;
					INT n;
					i = 0;
					i = 0;
					//Get PDM
					PDMFRAME pdmIn;
					AllocPDMFrame(&pdmIn, w, h, DT_UCHAR);
					DataView_GetPDMFrame(hDataView, &pdmIn);
					hDataView = CreateMDIDataView(hWnd, "Flood-Fill");
					DataView_SetWidthHeight(hDataView, w, h);
					UCHAR *img = (UCHAR *)calloc(w*h, sizeof(UCHAR));
					FloodFill(w, h, (UCHAR *)pdmIn.lpvoid, (UCHAR *)img);

					FPOINTSET **ptSet;
					ptSet = (FPOINTSET **)calloc(w, sizeof(FPOINTSET *));
					for(i = 0; i < w; i++){
						ptSet[i] = (FPOINTSET *)calloc(1, sizeof(FPOINTSET));
						Alloc(ptSet[i], w*h);
					}
					DWORD nRegion;
					nRegion = Region(w, h, img, w, ptSet);

					FELLIPSE *e = (FELLIPSE *)calloc(nRegion, sizeof(FELLIPSE));
					POINT *pt = (POINT *)malloc(w+h*sizeof(POINT));
					for(i = 0; i < nRegion; i++){
						for(n = 0; n < ptSet[i]->nPoint; n++){
							pt[n].x = ptSet[i]->point[n].x;
							pt[n].y = ptSet[i]->point[n].y;
						}
						GetEllipseFromPoints(ptSet[i]->nPoint, pt, &e[i]);
						DrawEllipse(w, h, img, e[i], 255);
					}
					memcpy(pdmIn.lpvoid, img, w*h*sizeof(UCHAR));
					DropPDMFrame(hDataView, pdmIn);
					ShowWindow(hDataView, SW_SHOW);
					
					if(e)free(e);
					if(pt)free(pt);
					if(img)free(img);

					for(i = 0; i < w; i++){
						if(ptSet[i]){
							Free(ptSet[i]);
						}
					}
					if(ptSet)free(ptSet);
				
					//Execute FloodFill
					//Create hNewDataView
					//Send FloodFill output to hNewDataView
				}
				return TRUE;
			}
		default:
			{
				WPARAM wLow = LOWORD(wParam);
				WPARAM wHigh = HIWORD(wParam);


				
				if(LOWORD(wParam) >= ID_MDI_FIRSTCHILD){

					DefFrameProc(hWnd, g_hMDIClient, message, wParam, lParam);
				}

				HWND hChild = (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE, NULL, NULL);
				if(hChild){
					SendMessage(hChild, WM_COMMAND, wParam, lParam);
				}
			}
			}
			
	return FALSE;
}

				
LRESULT OnCreateRACE(LPTRFRAME *lpFrame, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//int result = callFun();

	HWND hFind;
	hFind = FindWindow("RACE", NULL); //FindWindow这个函数检索处理顶级窗口的类名和窗口名称匹配指定的字符串。只检索顶级窗口
	ThrowMessage(hFind, "Save Bitmap Failed");

	if(0){
		CreateThread(NULL, NULL, DekstopTestThread, (LPVOID) hWnd, NULL, NULL);
	}


	lpFrame->hWnd = hWnd;

	AddWindow(&lpFrame->wndSet, hWnd);//对窗口进行管理，按照顺序，记录窗口的父窗口，文字，位置，最大最小话的位置
	lpFrame->hMenu = GetMenu(hWnd);

	//Create the MDI Window (within the Frame Window [hWnd])
	g_hMDIClient = CreateMDIClient(hWnd, MP_ROOT_WINDOW); //创建MDI子窗口，客户窗口
	lpFrame->hMDIClient = g_hMDIClient;

	//MDI Subclassing works
	//Removed because the MDI Proc does not capture the MW_KEYDOWN message
	SubclassWnd(hWnd, lpFrame->hMDIClient, MDISubclassProc);

	ShowWindow(g_hMDIClient, SW_SHOW);

	//Create Manifest Window
	lpFrame->hManifest = CreateManifest(hWnd, lpFrame->hMDIClient);	
	GetWindowRect(lpFrame->hManifest, &lpFrame->rcManifest);

	VerifyLoadedDLLsRACE(hWnd, TRUE);//确定Shell32.dll已成功载入，并显示版本信息

	//Create DataBar Window
	lpFrame->hDataBar = CreateWindow("Static",
										"Data Bar",
										WS_CHILD|WS_VISIBLE|WS_BORDER|WS_SIZEBOX,
										300,
										300,
										300,
										300,
										hWnd,
										(HMENU)IDC_DATABAR,
										GetModuleHandle(NULL),
										NULL);

	SubclassWnd(hWnd, lpFrame->hDataBar, DataBarSubclassProc);
	SendMessage(hWnd, WM_COMMAND, LOWORD(ID_VIEW_DATABAR), NULL);

	InsertNewLineManifest(lpFrame->hManifest, "Initializing...");

	//Create Window [Status Bar]
	lpFrame->hStatusBar = CreateWindowEx(NULL,
										STATUSCLASSNAME,
										NULL,
										WS_CHILD|WS_VISIBLE|SBT_TOOLTIPS|WS_BORDER,
										NULL, NULL, NULL, NULL,
										hWnd,
										(HMENU) IDC_MAIN_STATUS,
										GetModuleHandle(NULL),
										NULL);//You can create a status bar by using the CreateStatusWindow function or by using the CreateWindowEx function and specifying the STATUSCLASSNAME window class.
								

	lpFrame->hRebar = BuildRebar(hWnd, lpFrame->hToolBar);
	lpFrame->hToolBar = lpFrame->hRebar;

	lpFrame->hAlgWizard = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ALGWIZARD), hWnd, AlgorithmWizardProc);
	if(!lpFrame->hAlgWizard) //创建一个向导树
		ThrowLastError(hWnd);
	else
		ShowWindow(lpFrame->hAlgWizard, SW_HIDE);



	RECT rcToolLeft;
	GetWindowRect(lpFrame->hToolLeft, &rcToolLeft);//这个不知道是怎么回事，没有创建窗口就开始找窗口的大小

	//lpFrame->hToolBar = CreateToolBar(hWnd);

	InitializeMenuBar(hWnd);//就是对菜单有一些初始化，那些可用那些不可用

	INT heightToolBar;
	heightToolBar = SendMessage(lpFrame->hToolBar, TB_GETBUTTONSIZE, NULL, NULL);

	//Initialize the Archive Directory
	sprintf(lpFrame->strArchivePath, "%s", DIR_C);//初始化文件夹地址

	//If Default Path does not exists, then have user select a new one
	if(!PathExists(lpFrame->strArchivePath)){
		if(!CreateDirectory(DIR_C, NULL)){
			PostMessage(hWnd, WM_COMMAND, LOWORD(ID_SETUP_ARCHIVE), NULL);
		}
	}
	else{
		CHAR strMessage[MAX_PATH];
		sprintf(strMessage, "%s%s", "Current Archive Folder: ", lpFrame->strArchivePath); //既可以输出到终端，也可以写入一个字符串
		ThrowMessage(hWnd, strMessage);
	}

	if(!PathExists("C:\\Archive\\Snap\\")){    //Determines whether a path to a file system object such as a file or folder is valid.
		CreateDirectory("C:\\Archive\\Snap\\", NULL);
	}

	INT nParts; 
	INT wPart;
	nParts = 5;

	wPart = (GetWindowWidth(hWnd)/(nParts+1));
	INT parts[5];
	parts[0] = wPart;
	parts[1] = parts[0]+wPart;
	parts[2] = parts[1]+wPart;
	parts[3] = parts[2]+wPart;
	parts[4] = parts[3]+wPart;

	SendMessage(lpFrame->hStatusBar, SB_SETPARTS, (WPARAM) nParts, (LPARAM) parts);//这是一个专门发个状态条的消息，分成几个部分，每个部分右边的坐标分别是什么

	DiscoverDevices(hWnd, lpFrame->hManifest);//找寻电脑上的dos设备
	InsertNewLineManifest(lpFrame->hManifest, "Done.");

	SetWindowText(hWnd, FRAMETITLE);
	ShowWindow(hWnd, SW_MAXIMIZE);

	if(1){
		lpFrame->hTimeLapse = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_TIMELAPSE), hWnd, TimeLapseProc);

		if(lpFrame->hTimeLapse){
			SendMessage(lpFrame->hTimeLapse, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);
			SetParent(lpFrame->hTimeLapse, lpFrame->hMDIClient);
		}
	}

	if(1){
		lpFrame->hTimeLapseProp = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_TIMELAPSEPROPERTIES), hWnd, TimeLapsePropertiesProc);
		
		if(lpFrame->hTimeLapseProp){
			SendMessage(lpFrame->hTimeLapseProp, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);
			SendMessage(lpFrame->hTimeLapseProp, WM_SETHWND, HWND_TIMELAPSE, (LPARAM) lpFrame->hTimeLapse);
			SetParent(lpFrame->hTimeLapseProp, lpFrame->hMDIClient);
		}

		if(lpFrame->hTimeLapse){
			SendMessage(lpFrame->hTimeLapse, WM_SETHWND, HWND_TIMELAPSEPROPERTIES, (LPARAM) lpFrame->hTimeLapseProp);
		}
	}

	lpFrame->hPropertyManager = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_PROPERTYMANAGER), hWnd, PropertyManagerProc); 

	if(!lpFrame->hPropertyManager)
		ThrowMessage(hWnd, "WndFrame: Error Creating Property Manager Dialog");
	else{
		SendMessage(lpFrame->hPropertyManager, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);
		SetParent(lpFrame->hPropertyManager, lpFrame->hMDIClient);
	}

	if(0){
		lpFrame->hCameraProperties = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_CAMERAPROPERTIES), hWnd, CameraPropertiesProc);

		if(!lpFrame->hCameraProperties)

			ThrowMessage(hWnd, "WndFrame: Error Creating CameraProperties Dialog");
		else{
			SendMessage(lpFrame->hCameraProperties, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);
			SetParent(lpFrame->hCameraProperties, lpFrame->hMDIClient);
		}
	}

	if(1){
		CHAR strID[MAX_STRING];
		memset(strID, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(strID, "Spectra Physics Lasers,Millennia Xs,5.30,0");

		CHAR strSend[MAX_STRING];
		memset(strSend, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(strSend, "?IDN");
		//strSend[2] = 13;
		ConnectToDeviceThreaded(lpFrame->hSpectraPhysics, strSend, strID, 9600);
		Sleep(500);
	}

	if(1){
		lpFrame->hSC10Linker = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SC10LINKER), hWnd, SC10LinkerProc); 

		if(!lpFrame->hSC10Linker)
			ThrowMessage(hWnd, "WndFrame: Error Creating SC10Linker Dialog");
		else
			SendMessage(lpFrame->hSC10Linker, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);

		CHAR port = NULL;

		CHAR strID[MAX_STRING];
		memset(strID, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(strID, "id?\rTHORLABS SC10 VERSION 1.0\r> ");

		ConnectToDeviceThreaded(lpFrame->hSC10Linker, "id?", strID, 9600);
		Sleep(500);
	}

	if(0){
		lpFrame->hUNO32 = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_UNO32), hWnd, UNO32WndProc); 

		if(!lpFrame->hUNO32)
			ThrowMessage(hWnd, "WndFrame: Error Creating UNO32 Dialog");
		else
			SendMessage(lpFrame->hUNO32, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);

		//ConnectToDeviceThreaded(lpFrame->hUNO32, "?ID", "UNO32", 115200, 3000);
		SendMessage(lpFrame->hUNO32, WM_SETHWND, HWND_SHUTTER_SC10, (LPARAM)lpFrame->hSC10Linker);

		SetParent(lpFrame->hUNO32, lpFrame->hMDIClient);
		ShowWindow(lpFrame->hUNO32, SW_SHOW);
	}

	if(1){
		lpFrame->hXBoxLinker = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_XBOXLINKER), hWnd, XBoxLinkerProc);

		if(!lpFrame->hXBoxLinker)
			ThrowMessage(hWnd, "WndFrame: Error Creating XBox Linker Dialog");
		else
			SendMessage(lpFrame->hXBoxLinker, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);

		SendMessage(lpFrame->hXBoxLinker, WM_SETHWND, HWND_LINK, (LPARAM) lpFrame->hUNO32);
	}

	/*if(0){
		lpFrame->hCellStretcher = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_CELLSTRETCHER), hWnd, CellStretcherProc);

		if(!lpFrame->hCellStretcher)
			ThrowMessage(hWnd, "WndFrame: Error Creating CellStretcher Dialog");
		else
			SendMessage(lpFrame->hCellStretcher, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);
	}*/

	if(0){
		//lpFrame->hReefWatchdog = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_RW_LIGHTING), hWnd, ReefWatchdogProc);
	
		if(!lpFrame->hReefWatchdog)
			ThrowMessage(hWnd, "WndFrame: Error Creating ReefWatchdog Dialog");
		else{
			SendMessage(lpFrame->hReefWatchdog, WM_SETHWND, HWND_OWNER, (LPARAM) hWnd);
			SetDialogParent(lpFrame->hReefWatchdog, lpFrame->hMDIClient);
		}
	}

	if(1){
		lpFrame->hMotorController = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MOTORWATCHDOG), hWnd, MotorWatchdogProc);
		
		if(!lpFrame->hMotorController )
			ThrowMessage(hWnd, "WndFrame: Error Creating MotorController Dialog");
		else{
			ShowWindow(lpFrame->hMotorController , SW_HIDE);
			SetDialogParent(lpFrame->hReefWatchdog, lpFrame->hMDIClient);
		}
	}

	ThrowMessage(hWnd, "");
	ThrowMessage(hWnd, "*********************************************");
	ThrowMessage(hWnd, "********************Input********************");
	ThrowMessage(hWnd, "D-Key Enables Measurement Tool");
	ThrowMessage(hWnd, "T-Key Enables Tracking Tool");
	ThrowMessage(hWnd, "Mouse Wheel Changes Frames");
	ThrowMessage(hWnd, "View->Property Manager Calibrates Field of View");
	ThrowMessage(hWnd, "********************Input********************");
	ThrowMessage(hWnd, "*********************************************");
	ThrowMessage(hWnd, "");

	lpFrame->hCommandPrompt = (HWND) SendMessage(hWnd, WM_CREATEMDICHILD, (WPARAM) "CommandPrompt", (LPARAM) "Command Prompt");
	lpFrame->hHistogramView = (HWND) SendMessage(hWnd, WM_CREATEMDICHILD, (WPARAM) "HistogramView", (LPARAM) "Histogram View");
	
	//Removes Resizing Flicker
	LONG_PTR lptr;
	lptr = GetWindowLongPtr(lpFrame->hMDIClient, GWL_STYLE);
	lptr = lptr|WS_CLIPCHILDREN;
	
	if(!SetWindowLongPtr(lpFrame->hMDIClient, GWL_STYLE, lptr))
		ThrowMessage(hWnd, "MDI Error: SetWindowLongPtr(MDI)!!");
	
	SendMessage(hWnd, WM_COMMAND, LOWORD(ID_VIEW_DATABAR), NULL);


	//HWND hPropertySheet;
	if(0){
		lpFrame->hZeissAcqusition = CreatePropertySheet(hWnd);
		SetDialogParent(lpFrame->hZeissAcqusition, lpFrame->hMDIClient);
	}
	if(0){
		//CreateMessage Window
		lpFrame->hReefWatchdog = CreateWindowEx(WS_EX_WINDOWEDGE, 
										"ReefWatchdog", 
										NULL, 
										WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, 
										CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
										HWND_MESSAGE, 
										NULL, 
										GetModuleHandle(0), 
										NULL);

		SendMessage(hWnd, WM_REGISTERWINDOW, (WPARAM) lpFrame->hReefWatchdog, (LPARAM) "ReefWatchdog");
		SendMessage(lpFrame->hReefWatchdog, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM)hWnd);
	}

	if(1){
		lpFrame->hLaserMicroscope = CreateLaserMicroscopePropertySheet(hWnd);	//创建一个属性页，这个比较复杂
	}


	ShowWindow(hWnd, SW_SHOW);


	
	return TRUE;
}

DWORD SLMTestRACE(HWND hOwner, CHAR *strFilePath)
{
	UINT w = NULL;
	UINT h = NULL;
	DWORD bytePerPixel = NULL;
	PDMFRAME pdmf = {0};

	bytePerPixel = GetBMPSize(strFilePath, &w, &h);

	if(bytePerPixel == 1){
		pdmf.pdmfh.typeData = DT_UCHAR;
		//dataType = DT_UCHAR;
	}else if(bytePerPixel == 3){
		pdmf.pdmfh.typeData = DT_PIXEL;
	}

	AllocPDMFrame(&pdmf, w, h, pdmf.pdmfh.typeData);

	ReadBMP(strFilePath, &w, &h, pdmf.lpvoid);


	HWND hDataView = CreateDataView(/*hOwner*/GetDesktopWindow(), "DataView-Test");
	//SendMessage(hDataView, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM)hOwner);
	ShowWindow(hDataView, SW_SHOW);
	DataView_SetWidthHeight(hDataView, w, h);
	SendPDMFrame(hDataView, &pdmf);

	HWND hDataViewMDI = CreateMDIDataView(hOwner, "DataView-MDI");
	ShowWindow(hDataViewMDI, SW_SHOW);
	DataView_SetWidthHeight(hDataViewMDI, w, h);
	SendPDMFrame(hDataViewMDI, &pdmf);

	
	return TRUE;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

DWORD InitializeCommonControlsRACE(DWORD dw)
{

    INITCOMMONCONTROLSEX icex;
	memset(&icex, NULL, sizeof(INITCOMMONCONTROLSEX));
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC   = ICC_COOL_CLASSES | //注册Rebar类。
					ICC_BAR_CLASSES | //注册工具栏、状态栏、Trackbar和Tooltip类。
					ICC_TREEVIEW_CLASSES | //注册TreeView和Tooltip类。
					ICC_PROGRESS_CLASS;//注册Progress Bar类。

	return (DWORD) InitCommonControlsEx(&icex);

}

LRESULT AddRebarBand(HWND hRebar, HWND hChild, UINT wID, CHAR *lpText)
{
	LRESULT lResult;
	REBARBANDINFO rbBand = {0};

	rbBand.fMask		= RBBIM_SIZE | RBBIM_CHILD | RBBIM_CHILDSIZE |
                          RBBIM_ID | RBBIM_STYLE | RBBIM_TEXT |
						  RBBIM_BACKGROUND | RBBIM_IMAGE | 0;
	rbBand.cxMinChild   = GetWindowWidth(hChild);
	rbBand.cyMinChild   = GetWindowHeight(hChild);
	rbBand.cx           = 100;
	rbBand.fStyle       = RBBS_CHILDEDGE | RBBS_FIXEDBMP | RBBS_USECHEVRON |
							RBBS_GRIPPERALWAYS | 0;

	rbBand.wID          = wID/*IDC_COMBOBOX*/;
	rbBand.hwndChild    = hChild/*hComboBox*/;
	rbBand.lpText       = lpText;
	rbBand.cch          = MAX_STRING;
	rbBand.hbmBack      = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
	rbBand.iImage       = 0;

	lResult = SendMessage(hRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)(LPREBARBANDINFO)&rbBand);

	return lResult;

}


	

LRESULT OnRegisterWindowRACE(LPTRFRAME *lpFrame, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd;
	CHAR str[MAX_STRING];
	CHAR strClass[MAX_STRING];
	CHAR strText[MAX_STRING];

	hwnd = (HWND)wParam;
	//memcpy(className, (VOID *)lParam, MAX_STRING*sizeof(CHAR));

	if(hwnd){

		INT id = GetWndID(lpFrame, hwnd);
		if(id){

			lpFrame->wndSet.wnd[id-1].rc.left = lpFrame->wndSet.wnd[id-1].rc.left;
			//MoveWindow(lpFrame->wndSet.wnd[id-1].hwnd, lpFrame->wndSet.wnd[id-1].rc, TRUE);
			UpdateWindowMetrics(hWnd, hwnd, &lpFrame->wndSet);
			return TRUE;
		}
		//CHAR temp[MAX_STRING];
		//CHAR str[MAX_STRING];

		//memset(temp, NULL, MAX_STRING*sizeof(CHAR));
		//memset(str, NULL, MAX_STRING*sizeof(CHAR));
		lpFrame->wndSet.wnd[lpFrame->wndSet.nWnd].hwnd = hwnd;
		GetClassName(hwnd, strClass, MAX_STRING);
		GetWindowText(hwnd, strText, MAX_STRING);
		//lpFrame->wndSet.wnd->id = lpFrame->wndSet.nWnd;

		//SendMessage(hwnd, WM_SETID, (WPARAM) lpFrame->wndSet.wnd->id, NULL);

		//sprintf(temp, "Window Created: %s(%d) '%s'", className, lpFrame->wndSet.nWnd, str);
		//InsertNewLineManifest(lpFrame->hManifest, (CHAR *)temp);

		if(lpFrame->hReturn){
			if(!strcmp(strClass, "DataManager")){
				SendMessage(lpFrame->hReturn, WM_SETHWND, (WPARAM)HWND_DATAMANAGER, (LPARAM)hwnd);
				lpFrame->hReturn = NULL;
			}
		}

		lpFrame->wndSet.nWnd++;
		
	}


	return TRUE;
}

LRESULT VerifyLoadedDLLsRACE(HWND hwnd, DWORD dword)
{
	HRESULT hResult = NULL;
	HMODULE hShell = NULL;

	//Load Function Pointer from DLL
	DllGetVersionProc DllGetVersion;
	hShell = LoadLibrary("Shell32.dll");
	DllGetVersion = (DllGetVersionProc) GetProcAddress(hShell, "DllGetVersion");

	//Call Function
	DLLVERSIONINFO dllVerInfo = {0};
	dllVerInfo.cbSize = sizeof(DLLVERSIONINFO);
	if(DllGetVersion)
		hResult = (HRESULT)DllGetVersion(&dllVerInfo);

	//Output Version Properties
	CHAR str[MAX_STRING];
	sprintf(str, "Major Version: %d", dllVerInfo.dwMajorVersion);
	ThrowError(hwnd, str);
	sprintf(str, "Minor Version: %d", dllVerInfo.dwMinorVersion);
	ThrowError(hwnd, str);
	sprintf(str, "Build Number : %d", dllVerInfo.dwBuildNumber);
	ThrowError(hwnd, str);
	sprintf(str, "PlatformID   : %d", dllVerInfo.dwPlatformID);
	ThrowError(hwnd, str);
	ThrowError(hwnd, "");


	return TRUE;
}

HWND CreatePropertySheet(HWND hOwner)
{
	//NOTE:
	//PROPSHEETPAGE.lParam is sent to each tab during WM_INITDIALOG

	HWND hZeissMicroscope = NULL;
	HWND hSheet = NULL;
	HWND hPage = NULL;
	
	//CreateMessage Window
	hZeissMicroscope = CreateWindowEx(WS_EX_WINDOWEDGE, 
										"ZeissMicroscope", 
										NULL, 
										WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, 
										CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
										HWND_MESSAGE, 
										NULL, 
										GetModuleHandle(0), 
										NULL);

	SendMessage(hOwner, WM_REGISTERWINDOW, (WPARAM) hZeissMicroscope, (LPARAM) "ZeissMicroscope");
	SendMessage(hOwner, WM_SETHWND, (WPARAM) HWND_ZEISSMICROSCOPE, (LPARAM)(HWND)hZeissMicroscope);
	SendMessage(hZeissMicroscope, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM)hOwner);

	PROPSHEETPAGE psp;
	memset(&psp, NULL, sizeof(PROPSHEETPAGE));
	psp.dwSize = sizeof(PROPSHEETPAGE);
	psp.dwFlags = PSP_DEFAULT
						| PSP_PREMATURE
						| PSP_USETITLE
						;
	psp.hInstance = GetModuleHandle(NULL);
	psp.pszTemplate = MAKEINTRESOURCE(IDD_EXPERIMENT);
	psp.pszTitle = "Experiment";
	psp.pfnDlgProc = ExperimentOptionsProc;
	psp.lParam = (LPARAM) hOwner;

	PROPSHEETHEADER psh;
	memset(&psh, NULL, sizeof(PROPSHEETHEADER));

	psh.dwSize = sizeof(PROPSHEETHEADER);
	psh.dwFlags = PSH_PROPSHEETPAGE
				| PSH_MODELESS
				| PSH_NOAPPLYNOW
				| PSH_USECALLBACK
				//| PSH_RESIZABLE
				| PSH_DEFAULT
				;
	psh.hwndParent = hOwner;
	psh.hInstance = GetModuleHandle(NULL);
	psh.pszCaption = "AxioVert200: Multi-Dimensional Acquisition";
	psh.nPages = 1;
	psh.ppsp = &psp;
	psh.nStartPage = 0;
	psh.pfnCallback = PropSheetProc;
	
	hSheet = (HWND) PropertySheet(&psh);

	SendMessage(hZeissMicroscope, WM_SETHWND, (WPARAM)HWND_PROPERTYSHEET, (LPARAM) hSheet);

	HPROPSHEETPAGE hpsp = NULL;

	psp.pszTitle = "AutoFocus";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_AUTOFOCUS);
	psp.pfnDlgProc = AutoFocusProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "Mosaic";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_MOSAIC);
	psp.pfnDlgProc = MosaicProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "MultiChannel";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_MULTICHANNEL);
	psp.pfnDlgProc = MultiChannelProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "Light Path";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_OBJECTIVE);
	psp.pfnDlgProc = ZeissObjectiveProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	psp.pszTitle = "Z-Stack";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_ZSTACK);
	psp.pfnDlgProc = ZStackProc;
	hpsp = CreatePropertySheetPage(&psp);
	SendMessage(hSheet, PSM_ADDPAGE, NULL, (LPARAM) hpsp);

	PropSheet_RecalcPageSizes(hSheet);

	return hSheet;
}

DWORD OnWindowClosing(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	HWND hClosing = (HWND) lParam;
	//if(SendMessage(lpFrame->hCameraProperties, WM_CLOSING, NULL, (LPARAM) hClosing))
	//	return TRUE;

	UpdateWindowMetrics(hwnd, hClosing, &lpFrame->wndSet);//有窗口要关闭，检查窗口的编号，记录窗口的文字以及位置

	return FALSE;
}


//Removed 7.4.14
//LRESULT OnRegisterWindowRACE(LPTRFRAME *lpFrame, HWND hWnd, WPARAM wParam, LPARAM lParam)
//{
//
//	CHAR className[MAX_STRING];
//	HWND hWindow;
//
//	hWindow = (HWND)wParam;
//	memcpy(className, (VOID *)lParam, MAX_STRING*sizeof(CHAR));
//
//	if(hWindow){
//		CHAR temp[MAX_STRING];
//		CHAR str[MAX_STRING];
//
//		memset(temp, NULL, MAX_STRING*sizeof(CHAR));
//		memset(str, NULL, MAX_STRING*sizeof(CHAR));
//		lpFrame->wndSet.wnd->hwnd = hWindow;
//		//memcpy(lpFrame->wndSet.wnd->className, className, MAX_STRING*sizeof(CHAR));
//		lpFrame->wndSet.wnd->id = lpFrame->wndSet.nWnd;
//
//		SendMessage(hWindow, WM_SETID, (WPARAM) lpFrame->wndSet.wnd->id, NULL);
//		//GetWindowText(hWindow, str, MAX_STRING);
//
//		sprintf(temp, "Window Created: %s(%d) '%s'", className, lpFrame->wndSet.nWnd, str);
//		InsertNewLineManifest(lpFrame->hManifest, (CHAR *)temp);
//
//		if(lpFrame->hReturn){
//			if(!strcmp(className, "DataManager")){
//				SendMessage(lpFrame->hReturn, WM_SETHWND, (WPARAM)HWND_DATAMANAGER, (LPARAM)hWindow);
//				lpFrame->hReturn = NULL;
//			}
//		}
//
//		lpFrame->wndSet.nWnd++;
//	}
//
//
//	return TRUE;
//}

//
//DWORD WINAPI DekstopTestThread(LPVOID param)
//{
//	DWORD i = 0;
//	LONG w;
//	LONG h;
//	HHOOK MouseHook;
//	INT flgState;
//	POINT ptUL;
//	POINT ptLL;
//
//	if(1){
//		MessageBox(NULL, "Select Window", "Select Window", MB_OK);
//		//MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(0), NULL);
//	
//		//Get Uppper Left Corner
//		flgState = GetAsyncKeyState(VK_LBUTTON);
//		while(!flgState){
//			flgState = GetAsyncKeyState(VK_LBUTTON);
//		}
//		GetCursorPos(&ptUL);
//
//		//Get Lower Right Corner
//		while(flgState){
//			flgState = GetAsnycKeyState(VK_LBUTTON);
//		}
//		GetCursorPos(&ptLR);
//		//Sleep(5000);
//		//while(!gg_hwnd){
//		//	Sleep(1);
//		//}
//	}
//
//	POINT pt;
//	GetCursorPos(&pt);
//	HWND hPoint = WindowFromPoint(pt);
//	HWND hPointChild = ChildWindowFromPoint(hPoint, pt);
//
//	if(hPointChild)
//		hPoint = hPointChild;
//
//	GetBitmapDimensions(hPoint, &w, &h);
//	PDMFRAME pdmf = {0};
//	PDMFRAME pdmfSend = {0};
//	AllocPDMFrame(&pdmf, w, h, DT_LONGPIXEL);
//	AllocPDMFrame(&pdmfSend, w, h, DT_PIXEL);
//
//
//	GetBitmap(hPoint, w, h, (PIXEL *)pdmf.lpvoid);
//	HWND hDataView = CreateMDIDataView((HWND)param, "Desktop");
//	DataView_SetDimensions(hDataView, w, h);
//	DataView_ShowFrame(hDataView, &pdmfSend);
//	ShowWindow(hDataView, SW_SHOW);
//
//
//	for(i = 0; i < 20000; i++){
//		GetBitmap(hPoint, w, h, (PIXEL *)pdmf.lpvoid);
//		LONGtoPIXEL(w*h, (LONGPIXEL *)pdmf.lpvoid, (PIXEL *)pdmfSend.lpvoid);
//		//FlipVertical(w, h, (PIXEL *)pdmfSend.lpvoid);
//
//		DataView_ShowFrame(hDataView, &pdmfSend);
//	}
//
//	FreePDM(&pdmf);
//	FreePDM(&pdmfSend);
//
//	UnhookWindowsHookEx(MouseHook);
//	gg_hwnd = NULL;
//
//
//	return TRUE;
//}