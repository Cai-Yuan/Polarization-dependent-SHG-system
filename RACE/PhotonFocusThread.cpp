
#include "stdafx.h"
#include "PhotonFocus.h"
#include "CameraProperties.h"
#include "ToolsWin32.h"
#include "Windows.h"
#include "Holography.h"

DWORD WINAPI PhotonFocusThread(LPVOID param)
{
	#ifdef RACE_BITFLOW
	CAPTURE *c = (CAPTURE *)param; 

	HWND hDataView = NULL;
	HWND hFrame = NULL;
	HWND hUNO32 = NULL;
	HWND hLaserMicroscope = NULL;

	CHAR str[MAX_STRING];

	INT i = NULL;
	INT p = NULL;
	INT d = NULL;

	INT w = NULL;
	INT h = NULL;
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
	UCHAR *dump = NULL;

	ULONGLONG uSeconds;
	ULONGLONG uSecondsPrev;
	INT uSecondsDif;
	uSeconds = 0;
	uSecondsPrev = 0;

	//Initialize Camera
	BITFLOW bf;
	InitPhotonFocusCam(&bf);
	w = bf.wFrame;
	h = bf.hFrame;

	max = w*h;

	img = (UCHAR *) calloc(max, sizeof(UCHAR));
	if(!img)
		return FALSE;

	//Begin Data Acquisition
	//Send to DataManager
	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};
	PDMFRAME *pdmFrameDrop = NULL;

	pdmfh.width = w;
	pdmfh.height = h;
	pdmfh.typeMetaData = MD_FRAME;
	pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;
	pdmFrame.lpvoid = (VOID *)img;

	Sleep(1000);
	SendMessage(c->hDataManager, WM_SETFRAMESIZE, (WPARAM) w, (LPARAM) h);
	SendMessage(c->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	hDataView = (HWND) SendMessage(c->hDataManager, WM_GETHWND, (WPARAM) HWND_DATAVIEW, NULL);
	c->hDataView = hDataView;
	AppendWindowText(hDataView, "Photon Focus");
	SendMessage(hDataView, WM_SETHWND, HWND_PERIPHERAL, (LPARAM)c->hOwner);
	ShowWindow(hDataView, SW_SHOW);

	
	hFrame = (HWND) SendMessage(c->hOwner, WM_GETHWND, (WPARAM) HWND_FRAME, NULL);
	hLaserMicroscope = (HWND) SendMessage(hFrame, WM_GETHWND, (WPARAM) HWND_LASERMICROSCOPE, NULL);
	SendMessage(hLaserMicroscope, WM_SETHWND, (WPARAM) HWND_DATAVIEW, (LPARAM) hDataView);

	FLOAT prd = (1/c->frameRate)*(1000);
	DWORD tUpdate = clock()+prd;
	clock_t cTime = NULL;

	while(c->flgStream){

		
		while(!c->frameRate){
			if(!c->flgStream){
				break;
			}
			Sleep(250);
			tUpdate = clock();
		}

		prd = (1/c->frameRate)*(1000);

		cTime = clock();

		if (CiAqCommand(bf.hBoard, CiConSnap, CiConWait, CiQTabBank0, AqEngJ))
		{
			BFErrorShow(bf.hBoard);
		}
		
		GetSystemTime(&pdmfh.timeStamp);
		memcpy(img, bf.hostBuffer, w*h*sizeof(UCHAR));

		tUpdate = tUpdate+(DWORD)prd;

		if(c->flgSave){
			pdmfh.frame = i;
			i++;		
			dump = (UCHAR *)malloc(max*sizeof(UCHAR));
			if(dump){
				memcpy(dump, img, max*sizeof(UCHAR));

				pdmFrameDrop = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
				memcpy(&pdmFrameDrop->pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));

				pdmFrameDrop->lpvoid = (VOID *)dump;
				PostMessage(c->hDataManager, WM_DATADROP, (WPARAM) pdmFrameDrop, (LPARAM) "RAW");

				memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
				DropPDMFrame(hDataView, pdmFrame);
			}
		}
		else{
			memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));				
			DropPDMFrame(hDataView, pdmFrame);
			DropPDMFrame(hLaserMicroscope, pdmFrame);
		}

	}
	
	Sleep(1000);
	CloseBitFlowBoard(&bf);
	SendMessage(c->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
	SendMessage(c->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);
#endif
	return TRUE;

}
