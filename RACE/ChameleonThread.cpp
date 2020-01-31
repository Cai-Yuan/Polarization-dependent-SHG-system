#include "stdafx.h"
#include "C/FlyCapture2_C.h"
#include "Chameleon CMLN-13S2M.h"

#include "CameraProperties.h"
#include "DataView.h"
#include "PDM.h"

DWORD WINAPI ChameleonThread(LPVOID param)
{
	
	CAPTURE *cs = (CAPTURE *)param; 

	INT i = NULL;
	INT p = NULL;
	INT d = NULL;

	UINT nCameras;
	INT w, h;
	fc2Error fcError;
	fc2CameraInfo fcCamInfo;
	fc2Context fcContext;
	fc2TimeStamp ts;
	
	fcError = fc2CreateContext(&fcContext);
	if(!fc2GetResolution(fcContext, &w, &h)){
		return FALSE;
	}

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
	fc2Image fcImage;
	fc2PGRGuid guid;

	fcError = fc2GetCameraFromIndex(fcContext, 0, &guid);
	fcError = fc2Connect(fcContext, &guid);
	//SetTimeStamping(fcContext, TRUE);

	fcError = fc2GetCameraInfo(fcContext, &fcCamInfo);
	fcError = fc2StartCapture(fcContext);
	fcError = fc2CreateImage(&fcImage);


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

		fcError = fc2RetrieveBuffer(fcContext, &fcImage);
		memcpy(img, fcImage.pData, max*sizeof(CHAR));

		//ts = fc2GetTimeStamp(&fcImage);
		//GetSystemTime(&pdmfh.timeStamp);
		
		if(cs->flgSave){
			pdmfh.frame = i;
			i++;
		}

		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
		pdmFrame.lpvoid = (VOID *)img;

		SendMessage(cs->hDataManager, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");

	}

	fcError = fc2StopCapture(fcContext);
	fcError = fc2DestroyImage(&fcImage);
	fcError = fc2DestroyContext(fcContext);
	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
	SendMessage(cs->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);
	
	return TRUE;

}