#include "stdafx.h"
#include "ThreadParticleTrack.h"

DWORD WINAPI ThreadParticleTrack(VOID *lpvoid)
{

	DATAMANAGER *dm = (DATAMANAGER *) lpvoid;

	DATAMANAGER *dataManager = (DATAMANAGER *) calloc(1, sizeof(DATAMANAGER));

	UINT w = dataManager->wStack;
	UINT h = dataManager->hStack;
	UINT nFrames = dataManager->nFrame;

	UINT nSub = NULL;
	UINT ext = NULL;
	HWND hwnd = dataManager->hOwner;

	//Allocate Subtraction Frame Memory
	FILEPATHSET fps;
	UCHAR *subFrame = (UCHAR *)calloc(w*h, sizeof(UCHAR));

	//Prompt user to load in a Subtraciton Frame(s)
	if(MessageBox(dataManager->hOwner, "Please Select a Subtraction Frame", "RACE: Input", MB_OKCANCEL) != IDCANCEL){
	
		ext = GetOpenFilePaths(dataManager->hOwner, EXT_ALL, &fps);
		nSub = fps.nPaths;
	}


	//Load and Average all of the subtraction frames
	
	//Load and Subtract all of the dataManager Frames
	return TRUE;
}