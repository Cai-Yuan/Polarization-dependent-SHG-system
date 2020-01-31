#include "stdafx.h"

//C:\Program Files (x86)\PixeLINK\include
//C:\Program Files (x86)\PixeLINK\lib

#include "PixeLINKApi.h"
#include "PixeLINK.h"
#include "CameraProperties.h"

DWORD WINAPI PixeLINKThread(LPVOID param)
{
	CAPTURE *cs = (CAPTURE *)param; 

	INT i = NULL;
	INT p = NULL;
	INT d = NULL;

	INT w = 2208;
	INT h = 3000;
	INT max = w*h;

	INT temp = NULL;
	FLOAT ratio = NULL;

	INT numCamera = NULL;
	INT camInfo[256];
	INT bitsPerPixel = NULL;
	INT imgSize = NULL;

	INT result = NULL;
	INT resolution = NULL;

	UCHAR *data = NULL;
	UCHAR *img = NULL;

	//Initialize Camera
	HANDLE hCamera = NULL;
	FRAME_DESC frameDesc;
	PxLInitialize(NULL, &hCamera);

	if(!hCamera)
		return FALSE;

	PxLSetStreamState(hCamera, START_STREAM);

	img = (UCHAR *) calloc(max, sizeof(UCHAR));
	if(!img)
		return FALSE;

	//Begin Data Acquisition
	//Send to DataManager
	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};

	pdmfh.width = w;
	pdmfh.height = h;
	pdmfh.typeMetaData = MD_FRAME;
	pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;

	Sleep(1000);
	SendMessage(cs->hDataManager, WM_SETFRAMESIZE, (WPARAM) w, (LPARAM) h);
	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	HWND hDataView = NULL;
	hDataView = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_DATAVIEW, NULL);
	cs->hDataView = hDataView;


	while(cs->flgStream){

		PxLGetNextFrame(hCamera, max, img, &frameDesc);

		GetSystemTime(&pdmfh.timeStamp);
		
		if(cs->flgSave){
			pdmfh.frame = i;
			i++;
		}

		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
		pdmFrame.lpvoid = (VOID *)img;

		SendMessage(cs->hDataManager, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");

	}

	PxLSetStreamState(hCamera, STOP_STREAM);
	PxLUninitialize(hCamera);
	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
	SendMessage(cs->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);

	return TRUE;

}