#pragma once

//TODO: Add More Image loading capabilities
//--->FreeImage

//hhbtar142
//vufkh453


#include "resource.h"
#include "ThrowError.h"
#include "Subclass.h"
#include "DataBar.h"
#include "Shlwapi.h"

#define MAX_CHILD	256
#define MAX_WND		1000
#define MAX_PROC	256

#define WIDTH_BAR	25

//Status Bar Positions
#define SBP_FRAME		0
#define SBP_FRAMERATE	1
#define SBP_CURSOR		2
#define SBP_PIXELVAL	3
#define SBP_ZOOM		4
#define SBP_MAX			(SBP_ZOOM + 1)
//#define SBP_TIMESTAMP	5
//#define SBP_MAX			(SBP_TIMESTAMP + 1)

//Private Messages (WM_APP through 0xBFFF) or 
//					(32768 through 49151)

//Define All Window Messages Here
#define WM_NEWFRAMESET			39770
#define WM_ADDFRAME				39771
#define WM_SETFRAMENUM			39772
#define WM_SETDC				39773
#define WM_SETWH				39774
#define WM_SETFRAMEPOS			39775
#define WM_SETCHILDID			39776
#define WM_ALCIMGTYPE			39777
#define WM_SETFRMTIME			39778
#define WM_SETFRAMEFOCUS		39779
#define WM_GETFRMTIME			39780
#define WM_SETFRMNAME			39781
#define WM_GETFRMNAME			39782
#define WM_ADDTYPE				39783
#define WM_DISPLAYIMG			39784
#define WM_SETSHOWTYPE			39785
#define WM_GETHDC				39786
#define	WM_SETFRAMEADD			39787
#define	WM_UPDATESCROLL			39788
#define WM_INSERTFRAME			39789
#define WM_SETSLIDERSTATUS		39790
#define WM_GETARCHIVEDIR		32791
#define WM_SHOWFRAMEDATA		39792
#define	WM_GETMDIHANDLE			39793
#define	WM_SETMDIHANDLE			39794
#define WM_UPDATECHILDCOUNT		39795
#define WM_CREATECHILD			39796
#define WM_GETCHILDID			39797
#define WM_SETMDIPARENTHANDLE	39798
#define WM_REGISTERWINDOW		39799
#define WM_GETMDICLIENT			39800
#define WM_CREATEDATAVIEW		39801
#define WM_SETHWND				39802
#define WM_CREATEDATAMANAGER	39803
#define	WM_GETMDIMANIFEST		39804
#define WM_THROWERROR			39805
#define WM_GETHWND				39806
#define WM_UPDATEFRAME			39807
#define WM_GETLINKFLAG			39808
#define WM_CREATEDIALOG			39809
#define WM_GETMODULEHANDLE		39810
#define WM_CREATEMDICHILD		39811
#define WM_TODO					39812
//#define WM_INPUT				39813
#define WM_TIMELAPSE			39814
#define WM_CLOSETIMELAPSE		39815

#define WM_SETDATASOURCE		36864
#define WM_DATAIN				36865
#define WM_FILTERIN				36866
#define WM_SETID				36867
#define WM_ADDFILELIST			32768
#define WM_SETFRAMESIZE			32769
#define WM_GETSTACKDIM			32770
#define WM_SHOWFRAME			32771
#define WM_GETFRAME				32772
#define WM_INITSUBCLASS			32773
#define WM_SIZEDATABAR			32774
#define WM_GETOWNERHWND			32775
#define WM_GETHANDLE			32776
#define WM_ENDTRANSMISSION		32777	
#define WM_RETURNFRAME			32778
#define WM_GETFRAMEHEADER		32779
#define WM_DRAW					32780
#define DVM_FRAMEFORWARD		32781
#define DVM_FRAMEBACKWARD		32782
#define DVM_ZOOM				32783
#define WM_DATADROP				32784
#define WM_SETARCHIVE			32785
#define WM_SETARCHIVEDIR		32786
#define WM_GETUSERDATA			32787
#define WM_GETPOS				32788
#define WM_SETPOS				32789
#define WM_UPDATEPOS			32790
#define WM_GETFILELIST			32791
#define WM_SETHANDLE			32792
#define WM_STARTEXPERIMENT		32793
#define WM_STOPEXPERIMENT		32794
#define WM_CREATELINK			32795
#define WM_SETMASK				32796
#define WM_GETMASK				32797
#define WM_GETLONGPTR			32798
#define MW_SETLONGPTR			32799
#define WM_XBINPUT				32800
#define WM_CLOSING				32801
#define WM_SETOBJECT			32802
#define WM_GETFRAMESIZE			32803
#define WM_LINK					32804
#define WM_INITMICROSCOPE		32805
#define WM_AUTOFOCUS			32806
#define WM_TRACKERENABLE		32807
#define WM_TRACK				32808
#define WM_GETPDM				32809
#define WM_GETPDMPTR			32810
#define WM_SNAPFRAME			32811
#define WM_ZSTACK				32812
#define WM_GETSTRARCHIVEDIR		32813
#define WM_STREAMCLOSED			32814


#define DS_FILE				0x9000
#define DS_FRAMEGRABBER		0x9001

#define DB_EXPAND			1000
#define DB_COLLAPSE			1001

#define HWND_OWNER					0x1000
#define HWND_CHILD					0x1001
#define HWND_DATAMANAGER			0x1002
#define HWND_ALGORITHMMANAGER		0x1003
#define HWND_DATAVIEW				0x1004
#define HWND_ACTIVE					0x1005
#define HWND_ID						0x1006
#define HWND_TEXT					0x1007
#define HWND_MANIFEST				0x1008
#define HWND_FRAME					0x1009
#define HWND_PROPERTYMANAGER		0x1010
#define HWND_COMMANDPROMPT			0x100A
#define HWND_MDICLIENT				0x100B
#define HWND_EDIT					0x100C
#define HWND_XBOXLINKER				0x100D
#define HWND_MDI					0x100E
#define HWND_ZEISSMICROSCOPE		0x100F
#define HWND_PERIPHERAL				0x1010
#define HWND_STAGECONTROLS			4112
#define HWND_CAMERAPROPERTIES		4113
#define HWND_CELLSTRETCHER			4114
#define HWND_ACTIVECAMERA			4115
#define HWND_ACTIVESTAGE			4116
#define HWND_PROPERTYSHEET			4117
#define HWND_REEFWATCHDOG			4118
#define HWND_AUTOFOCUS				4119
#define HWND_MOSAIC					4120
#define HWND_MULTICHANNEL			4121
#define HWND_OBJECTIVE				4122
#define HWND_ZSTACK					4123
#define HWND_EXPERIMENT				4124
#define HWND_PARTICLETRACKER		4125
#define HWND_LINK					4126
#define HWND_SHUTTER_SC10			4127
#define HWND_UNO32					4128
#define HWND_LASERMICROSCOPE		4129
#define HWND_SYSTEM					4130
#define HWND_STAGE					4131
#define HWND_DEVICE					4132
#define HWND_SC10					4133
#define HWND_DISPLAY				4134
#define HWND_TRACKER				4135
#define HWND_TIMELAPSE				4136
#define HWND_TIMELAPSEPROPERTIES	4137

//#define HANDLE_ZEISS_MRM		0x1000
#define HANDLE_DEVICE			4096
#define HANDLE_DEVICE_DOSING	4097
#define HANDLE_DEVICE_LIGHTING	4098


//Data Manager Notifications
#define DMN_PARTICLETRACK	36864

//RACE Message
#define RM_SETTEXT				36922

//DataBar Message
#define DB_ADDSTRINGSET			36915
#define DB_SETFRAMEFOCUS		36916

//DataView Message
#define DV_SETFRAMEFOCUS		36917
#define DV_GETVIEWPOS			36918
#define DV_SETVIEWPOS			36919
#define DV_UPDATEVIEWPOS		36920
#define DF_INITFILTER			36922

//DataTab Message
#define DT_SELTABCHANGE			36921


//ParticleTrack Message
#define WM_SETLIMITS			36925
#define WM_SHOW					36926
		
//#define WN_GETACTIVE		1000
//#define WN_SETACTIVE		1001


//From Resources
#define IDC_MAIN_STATUS		130
#define IDC_MAIN_EDIT		131
#define IDC_CHILD_EDIT		132
#define IDC_CHILD_STATIC	133
#define IDC_TRACKBAR		134
#define IDC_REBAR			135
#define IDC_COMBOBOX		136
#define IDC_BUTTONHIDE		137
#define IDC_CHILD_COMBO		138
#define IDC_CHILD_COMMAND	139
#define IDC_PROGRESS		140
#define IDC_MANIFEST		141
#define IDC_DATABAR			142
//#define IDC_TREE			143
#define IDC_SELECTDEVICE	144

//#define ID_FILE_NEW			0x100

#define DVN_SETXFOV			100
#define DVN_SETYFOV			101
#define DVN_SETZFOV			102

//DataView Notifications
#define DVN_SETALGORITHM		400
#define DVN_UPDATE				401
#define DVN_SETTOOL				402
#define DVN_SETSTARTFRAME		403
#define DVN_SETSTOPFRAME		404
#define DVN_LOOP				405
#define DVN_SETALGORITHM2		406
#define DVN_SETALGORITHMSTRING	407
#define DVN_GETFRAMEBOUNDS		408




//DataBar Notificaitons
#define DBN_SETSTART		400
#define DBN_SETSTOP			401


//ID Algorithm
#define IDA_NONE				100
#define IDA_PARTICLETRACK		101
#define IDA_LOG1X				102
#define IDA_LOG2X				103
#define IDA_LOG3X				104
#define IDA_DX					105
#define IDA_DY					106
#define IDA_DXDY				107
#define IDA_GRADIENTPLOW		108
#define IDA_GRADIENTPLOWINV		109
#define IDA_DERIVATIVE2			110
#define IDA_IMAGESUBTRACT		111
#define IDA_GRADIENTSUBTRACT	112
#define IDA_BLOBLOG1			113
#define IDA_BLOBLOG3			114
#define IDA_BLOBLOG5			115
#define IDA_BLOBLOG7			116
#define IDA_STRETCH				117
#define IDA_WINDOW				118
#define IDA_GRADIENT2			119
#define IDA_GAUSSBLUR			120			

#define STR_ADD					"Add"
#define STR_SUBTRACT			"Subtract"
#define STR_DIVIDE				"Divide"
#define STR_MULTIPLY			"Multiply"
#define STR_MAX					"Max"
#define STR_GRAYSCALE			"Grayscale"
#define STR_IMAGESUBTRACT		"ImageSubtract"
#define STR_GRADIENT			"1stGradient"
#define STR_GRADIENT2			"2ndGradient"
#define STR_LAPLACIAN			"Laplacian"
#define STR_LAPLACIANANGLE		"LaplacianAngle"
#define STR_INVERSELAPLACIAN	"InverseLaplacian"
#define STR_GRADIENTANGLE		"GradientAngle"
#define STR_LOG					"Log"
#define STR_LOG1X				"Log(1x)"
#define STR_LOG2X				"Log(2x)"
#define STR_STRETCH				"Stretch"
#define STR_WINDOW				"Window"
#define STR_PARTICLETRACK		"ParticleTrack"
#define STR_RGB					"RGB"
#define STR_SOBEL				"Sobel"
#define STR_CENTRALDIFFERENCE	"CentralDifference"
#define STR_BOOLEAN				"Boolean"
#define STR_BOOLEANAND			"BooleanAND"
#define STR_PAVLIDISCONTOUR		"PavlidisContour"
#define STR_GAUSSBLUR			"GaussianBlur"
#define STR_NONMAXIMUM			"NonMaximum"
#define STR_NONMAXIMUMEXP		"NonMaximumExp"
#define STR_INVERT				"Invert"
#define STR_GETFRAME			"GetFrame"
#define STR_GETSTATICFRAME		"GetStaticFrame"
#define STR_AND					"AND"
#define STR_XOR					"XOR"
#define STR_FLOODFILL			"FloodFill"
#define STR_MIP					"MIP"
#define STR_CHAMFER				"Chamfer"
#define STR_SQUARE				"Square"
#define STR_MEAN				"Mean"
#define STR_STDDEV				"StdDev"
#define STR_VARIANCE			"Variance"
#define STR_NORMVARIANCE		"NormVariance"
#define STR_ERODE				"Erode"
#define STR_DILATE				"Dilate"
#define STR_DOUBLETHRESHOLD		"DoubleThreshold"
#define STR_THRESHOLD			"Threshold"
#define STR_THRESHOLDCONST		"ThresholdConst"
#define STR_CONST				"CONST"
#define STR_CONTRAST			"Contrast"
#define STR_NORMALIZE			"Normalize"
#define STR_PI					"PI"
#define STR_DEGREES				"Degrees"
#define STR_MEDIAN				"Median"
#define STR_HISTOGRAM			"Histogram"
#define STR_SQUAREROOT			"SquareRoot"
#define STR_HESSIANDXX			"HessianDxx"
#define STR_HESSIANDYY			"HessianDyy"
#define STR_HESSIANDXY			"HessianDxy"
#define STR_HESSIANDYX			"HessianDyx"
#define STR_ZEROCROSSING		"ZeroCrossing"
#define STR_MEDIANFILTER		"MedianFilter"

#define STR_RACEPROFILE			".rpf"

typedef HRESULT (__stdcall *DllGetVersionProc)(DLLVERSIONINFO *);


typedef struct WND_{

	//UINT id;
	HWND hwnd;
	CHAR strClass[MAX_STRING];
	CHAR strText[MAX_STRING];
	BOOL bVisible;
	RECT rc;
	WINDOWPLACEMENT wndPlacement;
	//CHAR className[MAX_STRING];

}WND;

//typedef struct WND_{
//
//	UINT id;
//	HWND hwnd;
//	CHAR className[MAX_STRING];
//
//}WND;

typedef struct WNDSET_{

	UINT nWnd;
	WND wnd[MAX_WND];


}WNDSET;

//Should replace WNDSET
typedef struct WNDLIST_{

	UINT nWnd;
	WND wnd[MAX_WND];

}WNDLIST;

typedef struct LPTRFRAME_{

	CHAR strArchivePath[MAX_STRING];

	DWORD nDataManagerChild;
	HWND dataManagerChild[MAX_CHILD];

	DWORD nDataViewChild;
	HWND dataViewChild[MAX_CHILD];

	HWND hWnd;
	HWND hToolBar;
	HWND hStatusBar;
	HWND hDeviceManager;
	HWND hManifest;
	HWND hRebar;
	HWND hPropertyManager;
	HWND hCommandPrompt;
	HWND hHistogramView;
	HWND hDataBar;
	HWND hXBoxLinker;
	HWND hToolLeft;
	HWND hToolRight;
	HWND hChild;
	HWND hMDIClient;	
	HWND hAlgWizard;
	HWND hCameraProperties;
	HWND hStageProperties;
	HWND hCellStretcher;
	HWND hZeissAcqusition;
	HWND hAxiovert;
	HWND hReefWatchdog;
	HWND hMotorController;
	HWND hZeissMicroscope;
	HWND hUNO32;
	HWND hSC10Linker;
	HWND hLaserMicroscope;
	HWND hSpectraPhysics;
	HWND hTimeLapse;
	HWND hTimeLapseProp;

	HWND hReturn;

	HMENU hMenu;

	RECT rcFrame;
	RECT rcManifest;
	RECT rcDataView;
	RECT rcDataBar;
	RECT rcText;

	INT wManifest;
	INT wDataBar;

	HWND hActive;

	UINT xWin;
	UINT ywin;
	UINT wWin;
	UINT hWin;

	WNDSET wndSet;

	UINT SWMODE;

	BOOL fManifest;
	BOOL fDataBar;
	BOOL fPropertyManager;
	BOOL fCommandPrompt;
	BOOL fXBoxLinker;
	BOOL fCameraProperties;
	BOOL fStageProperties;
	BOOL fCellStretcher;
	BOOL fZeissAcqusition;

}LPTRFRAME;

LRESULT OnCommandRACE(LPTRFRAME *lpFrame, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT OnSizeRACE(LPTRFRAME *lpFrame, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT OnCreateRACE(LPTRFRAME *lpFrame, HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnRegisterWindowRACE(LPTRFRAME *lpFrame, HWND hWnd, WPARAM wParam, LPARAM lParam);
DWORD OnWindowClosing(LPTRFRAME *lpFrame, HWND hwnd, WPARAM wParam, LPARAM lParam);
DWORD InitializeCommonControlsRACE(DWORD dw);
LRESULT VerifyLoadedDLLsRACE(HWND hwnd, DWORD dword);

DWORD SLMTestRACE(HWND hOwner, CHAR *strFilePath);

#ifdef WIN32
#define FRAMETITLE "RACE - Rapid Algorithm Computing Environment [x86]"
#endif
#ifdef _WIN64
#define FRAMETITLE "RACE - Rapid Algorithm Computing Environment [x64]"
#endif

#include "Boot.h"
