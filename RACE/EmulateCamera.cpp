#include "stdafx.h"

#include "EmulateCamera.h"
#include "CameraProperties.h"
#include "CaptureThreads.h"
#include "DataView.h"
#include "GetFolderPath.h"
#include "FilePath.h"
#include "PDM.h"
#include "ToolsWin32.h"
#include "ParticleTracker.h"


DWORD WINAPI EmulateCaptureStream(LPVOID param)
{
	CAPTURE *cc = (CAPTURE *)param; 

	HWND hFrame = NULL;
	HWND hDataView = NULL;
	HWND hDataFilter = NULL;
	HWND hDataManager = NULL;
	HWND hParticleTracker = NULL;

	INT i = NULL;
	INT p = NULL;
	INT d = NULL;

	INT w = NULL;
	INT h = NULL;
	INT max = NULL;

	INT result = NULL;

	clock_t cTime = NULL;
	INT del = NULL;

	UCHAR *data = NULL;
	VOID *img = NULL;
	UCHAR *dump = NULL;

	FILEPATHSETEX fps;
	memset(&fps, NULL, sizeof(FILEPATHSETEX));

	CHAR strFolderPath[MAX_STRING];
	memset(strFolderPath, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strDataView[MAX_STRING];
	memset(strDataView, NULL, MAX_STRING*sizeof(CHAR));

	strcpy((CHAR *)strFolderPath, (CHAR *)cc->strSource);

	EnumerateDirectoryFilePaths(strFolderPath, &fps, "pdm");
	SortFilePathSet(&fps, TRUE);

	hFrame = GetAncestor(cc->hOwner, GA_ROOT);
	sprintf(strDataView, "Emulated: %s", strFolderPath);
	hDataView = CreateMDIDataView(hFrame, strDataView);
	hParticleTracker = (HWND) SendMessage(hFrame, WM_GETHWND, (WPARAM) HWND_LASERMICROSCOPE, NULL);
	cc->hDataView = hDataView;
	

	for(i = NULL; i < fps.nPaths; i++){
		ThrowMessage(hFrame, fps.filePath[i].lpstr);
	}

	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};
	PDMFRAME *pdmFrameDrop = NULL;

	pdmfh.width = w;
	pdmfh.height = h;
	pdmfh.typeMetaData = MD_FRAME;
	pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;

	Getpdmfh(fps.filePath[0].lpstr, &pdmfh);
	
	w = pdmfh.width;
	h = pdmfh.height;

	max = w*h;

	if(w < 0 || h < 0)
		return FALSE;

	DataView_SetWidthHeight(hDataView, w, h);
	DataView_SetStartFrame(hDataView, pdmfh.frame);

	ParticleTracker_SetFrameSize(hParticleTracker, w, h);
	ParticleTracker_Enable(hParticleTracker, TRUE);
	ShowWindow(hParticleTracker, SW_SHOW);
	ShowWindow(hDataView, SW_SHOW);

	CHAR typeData = NULL;
	CHAR typeMetaData = NULL;
	
	FLOAT prd = (1/cc->frameRate)*(1000);
	DWORD tUpdate = clock()+prd;

	i = 0;
	while(cc->flgStream){


		while(cc->flgPause){
			if(!cc->flgStream){
				break;
			}
			Sleep(250);
			tUpdate = clock();
		}

		prd = (1/cc->frameRate)*(1000);

		cTime = clock();
		if(cTime >= tUpdate)
		{
			if(!(i%cc->sampleRate)){
				ReadPDMFrame(&typeData, &typeMetaData, &pdmFrame, fps.filePath[i].lpstr, &img);
				cTime = clock();
				tUpdate = tUpdate+prd;
				del = clock()-cTime;
				GetSystemTime(&pdmfh.timeStamp);
				del = clock() - cTime;

				if(img){
					memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
					pdmFrame.lpvoid = img;

					SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);
					SendMessage(hParticleTracker, WM_DATAIN, (WPARAM) &pdmFrame, NULL);
				}
			
				if(img){
					free(img);
					img = NULL;
				}
			}

			i++;
			if(i > fps.nPaths){
				i = NULL;
			}	
		}
	}

	Free(&fps);

	SendMessage(cc->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
	SendMessage(cc->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);

	SendMessage(cc->hDataView, WM_CLOSE, NULL, NULL);
	SendMessage(cc->hDataManager, WM_CLOSE, NULL, NULL);
	SendMessage(cc->hOwner, WM_STREAMCLOSED, (WPARAM) cc, NULL);


	return TRUE;

}

