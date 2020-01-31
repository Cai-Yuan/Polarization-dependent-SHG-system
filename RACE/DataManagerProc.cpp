#include "stdafx.h"

#include "DataManager.h"
#include "DataView.h"
#include "Archive.h"
#include "ImageConversion.h"
#include "TextWindow.h"
#include "GetFolderPath.h"

#include "ParticleTrack.h"
#include "ThreadParticleTrack.h"
#include "BMP.h"
#include "ZeissCam.h"
#include "String.h"

//Should be in charge of opening a thread which streams data to it's self from file or framegrabber

//Input [Before Opening Thread]
//Frame Grabber or File Paths

//In Charge of Data Consistency
//Skip: Non-consistent data

//In Charge of Archiving All Data


//typedef struct _CreateThreadedWindow{
//
//	WNDCLASSEX wc;
//	HWND wndCreator;
//	CHAR wndClass[MAX_STRING];
//
//}CreateThreadedWindow;
//
//typedef struct _LPTRDATAMANAGER{
//
//	INT i;
//
//}DATAMANAGER


LRESULT OnGetHwndDataManager(DATAMANAGER *lpDataManager, HWND hwnd, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK DataManagerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATAMANAGER *lpDataManager = (DATAMANAGER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	/*
	PROVIDE GUI:

	1. Adjustable Archive File Location
		WM_GETARCHIVEPATH Returns: The Current Archive Directory

		Child Windows Need WM_SETARCHIVEPATH

	2. Adjustable Data Flow Rate
		-Pause
		-Stop
		-Continue
		-Rate

	3. Data Input Type Detection
		-This Ensures the window knows which type of data acquisition thread to create
	*/

	switch(msg)
	{
	//case WMSETARCHIVEPATH:
		//{
		//Should be called initially before any data transmission occurs
		//Should generate a new 'unique' folder for every datamanager created
			//Can initially be based on yy:mm:dd:tt of when generated

			//return TRUE;
		//}
	//case WMGETARCHIVEPATH:

	//This message comes from the parent window and identifies if the data manager needs
	//to call the OpenFile function or open a data stream from a frame grabber
	case WM_SETDATASOURCE:
		{

			lpDataManager->sourceData = (DWORD) wParam;

			switch(lpDataManager->sourceData)
			{
			case DS_FILE:
				{
					//Crate OpenFile Menu
					UINT ext;	

					if(&lpDataManager->fps){
						HWND hChild;
						HANDLE hThread;
						ext = GetOpenFilePaths(hwnd, EXT_ALL, &lpDataManager->fps);

						if(lpDataManager->fps.nPaths > 0){
							//Open Data Pipe (Thread)
							hThread = CreateOpenFileThread(hwnd, ext, &lpDataManager->fps);

							GetRootPath(lpDataManager->fps.filePath[0].lpstr, lpDataManager->strSrcPath, MAX_STRING);

							CHAR str[MAX_STRING];
							sprintf(str, "Image Set %d:", 1);
							SetWindowText(lpDataManager->hDataView, str);
							AppendWindowText(lpDataManager->hDataView, lpDataManager->strSrcPath);

							ShowWindow(lpDataManager->hDataView, SW_HIDE);
							ShowWindow(lpDataManager->hTextWindow, SW_HIDE);

							SendMessage(lpDataManager->hDataView, WM_ADDFILELIST, (WPARAM) (&lpDataManager->fps), NULL);
						}

						return TRUE;
					}
				break;
				}
			case DS_FRAMEGRABBER:
				{
					DWORD sourceFrameGrabber = (DWORD)lParam;
					lpDataManager->isArchiving = FALSE;

					return TRUE;
				}
			default:
				{
					return FALSE;
				}
			}
			break;
		}
	case WM_GETFILELIST:
		{
			return (LRESULT) &lpDataManager->fps;
		}
	case WM_COMMAND:
		{

			switch(LOWORD(wParam))
			{
				case ID_SAVEAS_BMP:
					CreateThread(NULL, NULL, SaveSequenceBMP, lpDataManager, NULL, NULL);
					return TRUE;
				case DMN_PARTICLETRACK:
					CreateThread(NULL, NULL, ThreadParticleTrack, lpDataManager, NULL, NULL);
					return TRUE;
				case DVN_SETSTARTFRAME:
					INT upper;
					INT lower;
					lpDataManager->fStart = lParam;
					SendMessage(lpDataManager->hDataView, msg, wParam, lParam);

					lower = lParam;
					upper = lParam+lpDataManager->fps.nPaths;
					lParam = MAKELONG(lower, upper);
					if(!SendMessage(lpDataManager->hOwner, PBM_SETRANGE, NULL, MAKELONG(lParam, (lParam+lpDataManager->fps.nPaths))))
						ThrowMessage(hwnd, "DataView[PBM_SETRANGE] Failed!");

					lower = SendMessage(lpDataManager->hOwner, PBM_GETRANGE, TRUE, NULL);
					upper = SendMessage(lpDataManager->hOwner, PBM_GETRANGE, FALSE, NULL);
					lower = SendMessage(lpDataManager->hOwner, PBM_SETPOS, LOWORD(lParam), NULL);
					return TRUE;
				case DVN_SETSTOPFRAME:
					lpDataManager->fStop = lParam;
					SendMessage(lpDataManager->hDataView, msg, wParam, lParam);
					return TRUE;
				case DVN_GETFRAMEBOUNDS:
					{
						LPARAM bounds = NULL;
						bounds = MAKELPARAM(lpDataManager->fStart, lpDataManager->fStop);
						memcpy((VOID *)lParam, &bounds, sizeof(LPARAM));
						return TRUE;
					}
				default:
					break;
			}
		}
		break;
	//wParam -- Frame to Load
	//lParam -- String Specifying Data Type Filter
	case WM_GETFRAME:
		{
			VOID *lpvoid;
			CHAR typeData;
			CHAR typeMetaData;
			PDMFRAME *pdmFrame;
			CHAR strArchive[MAX_STRING];

			lpvoid = NULL;
			//pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
			pdmFrame = (PDMFRAME *)malloc(sizeof(PDMFRAME));
			memset(pdmFrame, NULL, sizeof(PDMFRAME));
			memset(&strArchive, NULL, sizeof(strArchive));

			GenerateArchivePathPDM(lpDataManager->idWnd, 
									(UINT) wParam, 
									(CHAR *)lpDataManager->strArchivePath, 
									(CHAR *)lParam, 
									strArchive);

			if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
				ThrowError(lpDataManager->hOwner, "Error Reading Archive");
				ThrowLastError(lpDataManager->hOwner);
				return NULL;
			}
			else{
				if(lpvoid){

					CHAR str[MAX_STRING];

					pdmFrame->lpvoid = lpvoid;

					if(pdmFrame->pdmfh.width == lpDataManager->wStack
						&& pdmFrame->pdmfh.height == lpDataManager->hStack){

						SendMessage(lpDataManager->hDataView, WM_SHOWFRAME, (WPARAM) pdmFrame, (LPARAM) lpvoid);
						if(lpvoid){free(lpvoid); lpvoid = NULL;}
					}

					//free(lpvoid);
					return (LRESULT) pdmFrame;
				}
				
			}

			if(pdmFrame) free(pdmFrame);
		}
		break;
	case WM_GETHWND:
		{
			return OnGetHwndDataManager(lpDataManager, hwnd, wParam, lParam);
		}
	case WM_GETFRAMEHEADER:
		//wParam = frame to get
		//lParam = pdmfh pointer

		VOID *lpvoid;
		CHAR typeData;
		CHAR typeMetaData;
		PDMFRAME *pdmFrame;
		CHAR strArchive[MAX_STRING];

		lpvoid = NULL;
		//pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
		pdmFrame = (PDMFRAME *) malloc(sizeof(PDMFRAME));
		memset(pdmFrame, NULL, sizeof(PDMFRAME));
		memset(&strArchive, NULL, sizeof(strArchive));

		GenerateArchivePathPDM(lpDataManager->idWnd, 
								(UINT) wParam, 
								(CHAR *)lpDataManager->strArchivePath, 
								(CHAR *)FS_RAW, 
								strArchive);

		if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
			ThrowError(lpDataManager->hOwner, "Error Reading Archive");
			ThrowLastError(lpDataManager->hOwner);
			return NULL;
		}
		else{

			memcpy((VOID *)lParam, &pdmFrame->pdmfh, sizeof(PDMFRAMEHEADER));
			
		}

		if(lpvoid)free(lpvoid);
		if(pdmFrame)free(pdmFrame);

		return NULL;
	case WM_RETURNFRAME:
		{

			//wParam = fStack
			//lParam = Filter String
			VOID *lpvoid;
			CHAR typeData;
			CHAR typeMetaData;
			PDMFRAME *pdmFrame;
			CHAR strArchive[MAX_STRING];

			lpvoid = NULL;
			//pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
			pdmFrame = (PDMFRAME *) malloc(sizeof(PDMFRAME));
			memset(pdmFrame, NULL, sizeof(PDMFRAME));
			//memset(&strArchive, NULL, sizeof(strArchive));

			GenerateArchivePathPDM(lpDataManager->idWnd, 
									(UINT) wParam, 
									(CHAR *)lpDataManager->strArchivePath, 
									(CHAR *)lParam, 
									strArchive);

			if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
				ThrowError(lpDataManager->hOwner, "Error Reading Archive");
				ThrowLastError(lpDataManager->hOwner);
				if(pdmFrame) free(pdmFrame);
				return NULL;
			}
			else{
				if(pdmFrame->pdmfh.width != lpDataManager->wStack ||
					pdmFrame->pdmfh.height != lpDataManager->hStack){
						if(lpvoid)free(lpvoid);
						lpvoid = NULL;
				}
				if(pdmFrame)free(pdmFrame);
				return (LRESULT) lpvoid;
			}

			if(pdmFrame) free(pdmFrame);
		}
		break;
	case WM_GETHANDLE:
		{
			switch(wParam)
			{
			case HWND_DATAVIEW:
				return (LRESULT) lpDataManager->hDataView;
			case HWND_OWNER:
				return (LRESULT) lpDataManager->hOwner;
			case HWND_TEXT:
				return (LRESULT) lpDataManager->hTextWindow;
			default:
				return NULL;
			}

			break;
		}
	case WM_SETHWND:
		{
			if(wParam == HWND_OWNER){
				LRESULT lResult;
				lpDataManager->hOwner = (HWND) lParam;

				lpDataManager->hDataView = (HWND) SendMessage(hwnd, WM_CREATEDATAVIEW, NULL, NULL);

				lResult = SendMessage(lpDataManager->hOwner, WM_GETARCHIVEDIR, (WPARAM) lpDataManager->strArchivePath, NULL);
				lpDataManager->hMDIClient = (HWND) SendMessage(lpDataManager->hOwner, WM_GETMDICLIENT, NULL, NULL);
				lpDataManager->hTextWindow = CreateMDIChild(lpDataManager->hMDIClient, "TextEditor", "Particle Data");

				CHAR str[MAX_STRING];
				
				sprintf(str, "Position\t\tVelocity(um/s)\tDisplacement(um)");
				InsertNewLine(lpDataManager->hTextWindow, str);
				InsertNewLine(lpDataManager->hTextWindow, "");
				InsertNewLine(lpDataManager->hTextWindow, "**********Particle 1**********");

			}
			if(wParam == HWND_ACTIVE){

				//Update Progress Bar
				if(!lpDataManager->isEndTransmission){
					SendMessage(lpDataManager->hOwner, PBM_SETRANGE, NULL, MAKELONG(lParam, lParam+lpDataManager->fps.nPaths));
					SendMessage(lpDataManager->hOwner, PBM_SETPOS, lpDataManager->nFrame, NULL);
				}
			}
			return TRUE;
		}
		break;
	case WM_SETID:
		{
			lpDataManager->idWnd = (UINT) wParam;
			return TRUE;
		}
	case WM_CREATEDATAVIEW:
		{
			if(lpDataManager->hDataView)
				return FALSE;
			//return CreateMDIDataView(lpDataManager->hOwner, "DataView");
			lpDataManager->hDataView = (HWND) SendMessage(lpDataManager->hOwner, WM_CREATEMDICHILD, (WPARAM) "DataView", (LPARAM) "DataView");
			SendMessage(lpDataManager->hDataView, WM_SETHWND, (WPARAM) HWND_DATAMANAGER, (LPARAM) hwnd);
			return (LRESULT) lpDataManager->hDataView;
		}
	case WM_SETFRAMESIZE:
		{
			lpDataManager->wStack = (INT) wParam;
			lpDataManager->hStack = (INT) lParam;

			SendMessage(lpDataManager->hDataView, WM_SETFRAMESIZE, wParam, lParam);
			return TRUE;
		}
	case WM_GETSTACKDIM:
		{
			WPARAM wparam = MAKEWPARAM(lpDataManager->wStack, lpDataManager->hStack);
			memcpy((VOID *)wParam, &wparam, sizeof(WPARAM));
			return lpDataManager->nFrame;
		}
	case WM_GETOWNERHWND:
		{
			return (LRESULT) lpDataManager->hOwner;
		}
	case WM_DATAIN:
		{

			CHAR strFilter[MAX_STRING];
			CHAR strArchive[MAX_PATH];
			VOID *lpvoid;
			CHAR *img;

			PDMFRAME pdmFrame;

			memcpy(&pdmFrame, (PDMFRAME *)wParam, sizeof(PDMFRAME));
			sprintf(strFilter, "%s", lParam);

			//img = (CHAR *)pdmFrame.lpvoid;

			if(!lpDataManager->flgData){
				lpDataManager->flgData = TRUE;
				lpDataManager->fStart = pdmFrame.pdmfh.frame;
			}

			if(pdmFrame.pdmfh.frame < lpDataManager->fStart)
				lpDataManager->fStart = pdmFrame.pdmfh.frame;

			if(pdmFrame.pdmfh.frame > lpDataManager->fStop)
				lpDataManager->fStop = pdmFrame.pdmfh.frame;

			if(pdmFrame.pdmfh.frame == lpDataManager->fStart){
				ShowWindow(lpDataManager->hDataView, SW_SHOW);
			}



			lpDataManager->nFrame++;

			if(lpDataManager->nFrame > lpDataManager->fStop){
				lpDataManager->fStop = lpDataManager->nFrame;
				pdmFrame.pdmfh.frame = lpDataManager->nFrame;
			}

			//Display Data Through DataView
			DataView_ShowFrame(lpDataManager->hDataView, &pdmFrame);

			if(lpDataManager->isArchiving){

				INT pos;
				CHAR strPos[MAX_STRING];

				//if(1){
				//lpDataManager->nFrame++;

				//pdmFrame.pdmfh.frame = lpDataManager->nFrame;
				//}
				pdmFrame.pdmfh.typeMetaData = MD_FRAME;

				//if(pdmFrame.pdmfh.frame > lpDataManager->nFrame)
				//	lpDataManager->nFrame = pdmFrame.pdmfh.frame;

				PostMessage(lpDataManager->hOwner, PBM_SETPOS,(WPARAM) (INT)(lpDataManager->nFrame), NULL);

				//Archive
				GenerateArchivePathPDM(lpDataManager->idWnd, 
							pdmFrame.pdmfh.frame, 
							(CHAR *)lpDataManager->strArchivePath, 
							FS_RAW, 
							strArchive);

				WritePDMFrame(pdmFrame.pdmfh.typeData, pdmFrame.pdmfh.typeMetaData, pdmFrame, strArchive, pdmFrame.lpvoid);
			
			}

			return TRUE;
		}
	case WM_DATADROP:
		{

			CHAR strFilter[MAX_STRING];
			CHAR strArchive[MAX_PATH];
			VOID *lpvoid;
			PDMFRAME pdmFrame;

			memcpy(&pdmFrame, (PDMFRAME *)wParam, sizeof(PDMFRAME));
			sprintf(strFilter, "%s", lParam);

			if(!lpDataManager->flgData){
				lpDataManager->flgData = TRUE;
				lpDataManager->fStart = pdmFrame.pdmfh.frame;
			}
			
			if(pdmFrame.pdmfh.frame == lpDataManager->fStart){
				
				ShowWindow(lpDataManager->hDataView, SW_SHOW);
			}

			if(pdmFrame.pdmfh.frame < lpDataManager->fStart)
				lpDataManager->fStart = pdmFrame.pdmfh.frame;

			if(pdmFrame.pdmfh.frame > lpDataManager->fStop)
				lpDataManager->fStop = pdmFrame.pdmfh.frame;

			lpDataManager->nFrame++;

			if(lpDataManager->nFrame > lpDataManager->fStop)
				lpDataManager->fStop = lpDataManager->nFrame;
			
			if(pdmFrame.pdmfh.frame == lpDataManager->fStart){
				
				ShowWindow(lpDataManager->hDataView, SW_SHOW);
			}

			if(lpDataManager->isArchiving){

				INT pos;
				CHAR strPos[MAX_STRING];

				if(pdmFrame.pdmfh.frame > lpDataManager->nFrame)
					lpDataManager->nFrame = pdmFrame.pdmfh.frame;

				//Archive
				GenerateArchivePathPDM(lpDataManager->idWnd, 
							pdmFrame.pdmfh.frame, 
							(CHAR *)lpDataManager->strArchivePath, 
							FS_RAW, 
							strArchive);

				WritePDMFrame(pdmFrame.pdmfh.typeData, pdmFrame.pdmfh.typeMetaData, pdmFrame, strArchive, pdmFrame.lpvoid);
			
			}

			if(pdmFrame.lpvoid) free(pdmFrame.lpvoid);
			if((VOID *)wParam)free((VOID *)wParam);

			return TRUE;
		}
	case WM_ENDTRANSMISSION:
		{
			lpDataManager->isEndTransmission = TRUE;

			SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) lpDataManager->fStart);
			SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) lpDataManager->fStop);

			//Create Particle Tracking Thread
			SendMessage(lpDataManager->hOwner, WM_ENDTRANSMISSION, NULL, NULL);
		}
	case WM_SETARCHIVE:
		{
			if(wParam)
				lpDataManager->isArchiving = TRUE;
			else
				lpDataManager->isArchiving = FALSE;
		}
		return TRUE;
	case WM_SETARCHIVEDIR:
		{
			//'\' = 92
			if(wParam){
				INT lenStr;
				memcpy(lpDataManager->strArchivePath, (VOID *)wParam, MAX_STRING*sizeof(CHAR));
				lenStr = strlen(lpDataManager->strArchivePath);
				if(lpDataManager->strArchivePath[lenStr-1] != 92){
					lpDataManager->strArchivePath[lenStr-1]  = 92;
					lpDataManager->strArchivePath[lenStr/*+1*/] = 0;
				}
				if(!PathExists(lpDataManager->strArchivePath)){
					if(!CreateValidDirectory(lpDataManager->strArchivePath)){
						ThrowError(lpDataManager->hOwner, "Could Not Create Directory!");
					}
				}
			}
		
			return TRUE;
		}
	case WM_GETSTRARCHIVEDIR:
		{
			if(!wParam)
				return FALSE;

			//memcpy((VOID *) wParam, lpDataManager->strArchive, MAX_STRING*sizeof(CHAR));

			return TRUE;
		}
	case WM_CREATE:
		{			
			ShowWindow(lpDataManager->hProgressView, SW_SHOW);

			lpDataManager->isArchiving = TRUE;
			Alloc(&lpDataManager->ss, MAX_PATH, MAX_PATHS);

			return TRUE;
		}
	case WM_NCCREATE:
		{
			lpDataManager = (DATAMANAGER *) InitWindowLongRACE(hwnd, sizeof(DATAMANAGER));
			memset(lpDataManager, NULL, sizeof(DATAMANAGER));
			break;
		}
	case WM_CLOSE:
		{
			ThrowMessage(lpDataManager->hOwner, "Window Destroyed: DataManager");
			SendMessage(lpDataManager->hTextWindow, WM_CLOSE, NULL, NULL);
			
			if(lpDataManager)free(lpDataManager);
			break;
		}
	default:
		{
			return TRUE;
		}
	}

	return TRUE;

}




DWORD WINAPI CreateThreadedWindow(VOID *lpvoid)
//DWORD WINAPI CreateDataManager(VOID *lpvoid)
{

	CTW *ctw = (CTW *) lpvoid;
	MSG msg = {0};
	HWND hwnd;
	WNDCLASSEX wc = {0};

	//RegisterMDIChild(GetModuleHandle(NULL), MDIDataManagerProc, "MDIDataManager");

	hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, 
							"MDIDataManager", 
							NULL, 
							WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, 
							CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
							HWND_MESSAGE, 
							NULL, 
							GetModuleHandle(0), 
							NULL);

	if(!hwnd){
		//Throw Error
		return FALSE;
	}

	//Initialize Timer
	SetTimer(hwnd, 1, 5000, NULL);

	//Send the Createing Window the HWND of the new data manager
	SendMessage(ctw->wndCreator, WM_REGISTERWINDOW, (WPARAM)hwnd, (LPARAM)NULL);

	//Message Pump
	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}

LRESULT OnGetHwndDataManager(DATAMANAGER *lpDataManager, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = NULL;

	switch(wParam)
	{
		case HWND_OWNER:
			return (LRESULT) lpDataManager->hOwner;
		case HWND_TEXT:
			return (LRESULT) lpDataManager->hTextWindow;
		case HWND_ID:
			return (LRESULT) lpDataManager->idWnd;
		case HWND_DATAVIEW:
			return (LRESULT) lpDataManager->hDataView;
		default:
			break;
	}

	return lResult;
}


//Description:
//
LRESULT GetFrame(HWND hDataManager, DWORD w, DWORD h, DWORD frame, FLOAT *img)
{

	VOID *lpvoid = NULL;

	lpvoid = (VOID *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) frame, (LPARAM) "RAW");

	UCHARtoFLOAT(w, h, (UCHAR *)lpvoid, img);
	if(lpvoid)free(lpvoid);


	return TRUE;
}