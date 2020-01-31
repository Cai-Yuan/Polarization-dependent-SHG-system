#pragma once

#include "stdafx.h"
#include "Point.h"

#define SEND_DATA	10
#define POST_DATA	11

typedef long FLAG;

typedef struct CTW_{

	WNDCLASSEX wc;
	HWND wndCreator;
	CHAR wndClass[MAX_STRING];

}CTW;

typedef struct CREATEDATAMANAGER_{

	WNDCLASSEX wc;
	HWND hOwner;
	HWND hDataManager;
	CHAR strClass[MAX_STRING];
	UINT wndID;

	DWORD sourceData;

}CREATEDATAMANAGER;

typedef struct _DATAMANAGER{
	
	HWND hOwner;
	HWND hProgressBar;
	HWND hDataView;
	HWND hTextWindow;
	HWND hMDIClient;
	HWND hProgressView;

	INT i;
	UINT idWnd;
	UINT nFrame;
	INT wStack;
	INT hStack;
	INT fStart;
	INT fStop;

	CHAR strSrcPath[MAX_PATH];
	CHAR strArchivePath[MAX_PATH];

	BOOL isArchiving;
	BOOL isEndTransmission;
	BOOL flgData;

	DWORD sourceData;

	FILEPATHSET fps;
	FILEPATHSET fpsArchive;
	STRINGSET ss;

	PDMFRAMESET pdmfs;
	
}DATAMANAGER;

DWORD DiscoverDevices(HWND hFrame, HWND hManifest);
DWORD DiscoverXBox(HWND hParent);

DWORD WINAPI CreateDataManager(VOID *lpvoid);
DWORD WINAPI CreateThreadedWindow(VOID *lpvoid);
HWND CreateDataManager(HWND hOwner, DWORD sourceData);

LRESULT CALLBACK DataManagerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT GetFrame(HWND hDataManager, DWORD w, DWORD h, DWORD frame, FLOAT *img);


//DataManager Functions
DWORD DataManager_Archive(HWND hwnd, BOOL state, LPVOID mask);
DWORD DataManager_GetArchivePath(HWND hwnd, CHAR *strPath);
DWORD DataManager_SetArchivePath(HWND hwnd, CHAR *strPath);
DWORD DataManager_SetFrameSize(HWND hwnd, LONG w, LONG h);
DWORD DataManager_SetStartFrame(HWND hwnd, LONG fStart);
DWORD DataManager_SendData(HWND hwnd, PDMFRAME pdmf, CHAR *str, FLAG flgInputMethod);