#include "stdafx.h"
#include "DataManager.h"
#include "PDM.h"

DWORD DataManager_Archive(HWND hwnd, BOOL state, LPVOID mask)
{
	SendMessage(hwnd, WM_SETARCHIVE, (WPARAM) state, (LPARAM) NULL);
	return TRUE;
}

DWORD DataManager_SetArchivePath(HWND hwnd, CHAR *strPath)
{
	SendMessage(hwnd, WM_SETARCHIVEDIR, (WPARAM) strPath, NULL);
	return TRUE;
}

DWORD DataManager_GetArchivePath(HWND hwnd, CHAR *strPath)
{
	return SendMessage(hwnd, WM_GETSTRARCHIVEDIR, (WPARAM) strPath, NULL);
	//return TRUE;
}

DWORD DataManager_SetFrameSize(HWND hwnd, LONG w, LONG h)
{
	return SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM) w, (WPARAM) h);
	//return TRUE;
}

DWORD DataManager_SetStartFrame(HWND hwnd, LONG fStart)
{
	return SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) fStart);
}

//'flgInputMethd' - DATAIN or DATADROP
//'flgInputMethod' - SEND_DATA or POST_DATA
DWORD DataManager_SendData(HWND hwnd, PDMFRAME pdmf, CHAR *str, FLAG flgInputMethod)
{

	if(!str || !pdmf.lpvoid)return FALSE;

	if(flgInputMethod == SEND_DATA){
		SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmf, (LPARAM) str);
	}

	if(flgInputMethod == POST_DATA){

	}

	return TRUE;
}


//DS_FRAMEGRABBER
//DS_FILE
HWND CreateDataManager(HWND hOwner, DWORD sourceData)
{
	CREATEDATAMANAGER cdm = {0};
	cdm.hOwner = hOwner;
	cdm.sourceData = sourceData;
	CreateThread(NULL, NULL, CreateDataManager, &cdm, NULL, NULL);

	while(!cdm.hDataManager){
		Sleep(1);
	}

	return cdm.hDataManager;
}

DWORD WINAPI CreateDataManager(VOID *lpvoid)
{

	CREATEDATAMANAGER *createDataManager = (CREATEDATAMANAGER *) lpvoid;

	MSG msg = {0};
	HWND hwnd;
	WNDCLASSEX wc = {0};
	CREATEDATAMANAGER cdm = {0};

	if(createDataManager){
		memcpy(&cdm, createDataManager, sizeof(CREATEDATAMANAGER));
	}
	hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, 
							"DataManager", 
							NULL, 
							WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, 
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							0, 0, 
							HWND_MESSAGE, 
							NULL, 
							GetModuleHandle(0), 
							NULL);

	if(!hwnd){
		//Throw Error
		return FALSE;
	}

	createDataManager->hDataManager = hwnd;

	//Initialize Timer
	SetTimer(hwnd, 1, 50, NULL);

	//Send the Creating Window the HWND of the new data manager
	SendMessage(cdm.hOwner, WM_REGISTERWINDOW, (WPARAM)hwnd, (LPARAM)"DataManager");

	//Initialize DataManger Data Source
	//NOTE: Ownder handle must be initialized before anything else
	SendMessage(hwnd, WM_SETHWND, (WPARAM)HWND_OWNER, (LPARAM) cdm.hOwner);
	SendMessage(hwnd, WM_SETDATASOURCE, (WPARAM)cdm.sourceData, (LPARAM) NULL);
	

	while(GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}



	
//DWORD DataManager_DropData(HWND hwnd, PDMFRAME pdmf)
//{
//	PMDFRAME pdmfDrop = {0};
//	Alloc(&pdmfDrop, pdmf.pdmfh.width, pdmf.pdmfh.height);
//
//
//	return TRUE;
//}