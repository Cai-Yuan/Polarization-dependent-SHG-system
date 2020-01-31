#pragma once

#include "stdafx.h"
#include "RACE.h"

//'fRate' [s]

typedef struct _TIMELAPSETHREAD{
	HANDLE hThread;
	HWND hOwner;
	HWND hSourceDataView;
	HWND hDestDataView;

	FLOAT fRate;
	CHAR strID[MAX_STRING];
	CHAR strSource[MAX_STRING];
	CHAR strDest[MAX_STRING];
	CHAR strArchive[MAX_STRING];

	DWORD flgRaw;
	DWORD flgRendering;
	DWORD flgTimeLapse;
	DWORD flgEnd;

	DWORD id;

}TIMELAPSETHREAD;

typedef struct TIMELAPSETHREADSET{
	DWORD size;
	DWORD nTimeLapse;
	TIMELAPSETHREAD *tlt;
}TIMELAPSETHREADSET;

typedef struct _TIMELAPSE{

	HWND hOwner;
	HWND hActive;
	HWND hBtnCreate;
	HWND hBtnDelete;
	HWND hListStudies;
	HWND hTimeLapseProp;
	HWND hCheckRaw;
	HWND hCheckRendering;

	TIMELAPSETHREADSET tlts;

	DWORD nTimeLapse;
	DWORD flgRaw;
	DWORD flgRendering;


}TIMELAPSE;

typedef struct _TIMELAPSEPROPERTIES{
	HWND hOwner;
	HWND hSource;		//Stores the DataView handle to recieve data from
	HWND hTimeLapse;
	HWND hEditRate;
	HWND hEditSource;
	
	FLOAT fRate;
	

}TIMELAPSEPROPERTIES;

DWORD WINAPI TimeLapseThreadProc(LPVOID param);


INT_PTR CALLBACK TimeLapseProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK TimeLapsePropertiesProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD OnInitDialogTimeLapse(HWND hwnd, WPARAM wParam, LPARAM lParam);
DWORD OnInitDialogTimeLapseProperties(HWND hwnd, WPARAM wParam, LPARAM lParam);
DWORD OnCommandTimeLapse(TIMELAPSE *lpTimeLapse, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD OnCommandTimeLapseProperties(TIMELAPSEPROPERTIES *lpTimeLapseProp, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

DWORD Alloc(TIMELAPSETHREADSET *tlts, DWORD size);
DWORD Append(TIMELAPSETHREADSET *tlts, TIMELAPSETHREAD tlt);
DWORD Delete(TIMELAPSETHREADSET *tlts, DWORD index);
DWORD Find(TIMELAPSETHREADSET *tlts, CHAR *strID, DWORD *index);