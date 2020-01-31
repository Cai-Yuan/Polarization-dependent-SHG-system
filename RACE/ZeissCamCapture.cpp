#include "stdafx.h"

#include "ZeissCam.h"
#include "time.h"
#include "CameraProperties.h"
#include "CaptureThreads.h"

#define MAX_CAPTURE 500

DWORD WINAPI ZeissHSmCapture(LPVOID param)
{
	CAPTURE *cs = (CAPTURE *)param; 

	HWND hDataView = NULL;
	HWND hFrame = NULL;

	HMODULE dllZeiss = NULL;
	ZEISSFRAMEINFO zfm = {0};
	ZEISSPROC zp = {0};

	INT i = NULL;
	INT p = NULL;
	INT d = NULL;

	INT w = NULL;
	INT h = NULL;
	INT max = NULL;

	INT temp = NULL;
	FLOAT ratio = NULL;

	INT numCamera = NULL;
	INT camInfo[256];
	INT bitsPerPixel = NULL;
	INT imgSize = NULL;
	INT fastAcquisition = NULL;
	INT available = NULL;
	INT result = NULL;
	INT resolution = NULL;
	INT resultInit = NULL;
	INT exposure = NULL;

	clock_t cTime = NULL;
	INT del = NULL;

	//UINT *data = NULL;
	UCHAR *data = NULL;
	UCHAR *img = NULL;
	UCHAR *dump = NULL;

	memset(camInfo, NULL, 256*sizeof(INT));

	//Handle DLL Loading and Function Pointer Initialization
	dllZeiss = LoadZeissLibrary(hDataView, ZEISS_HSM);
	if(!ImportZeissFunctions(dllZeiss, &zp))
		return FALSE;

	//Get Camera Properties
	result = zp.InitCamera();
	numCamera = zp.GetNumCamera();
	resolution = zp.GetResolution();
	available = zp.CamIsAvailable();
	//fastAcquisition = zp.StartFastAcquisition();
	resultInit = zp.InitializeAcquisition(4);

	ZEISSCAMINFO zci = {0};
	zp.GetCameraInfo((INT *)&zci);
	bitsPerPixel = zp.GetCurrentBitsPerPixel();

	zp.GetCurrentFrameSize((INT *)&zfm);
	w = zfm.width;
	h = zfm.height;
	max = w*h;
	imgSize = 2*w*h;

	exposure = zp.SetExposure(7000);

	if(w < 0 || h < 0)
		return FALSE;

	data = (UCHAR *) calloc(2*w*h, sizeof(UCHAR));
	img = (UCHAR *) calloc(max, sizeof(UCHAR));

	memset(data, 10, 2*w*h*sizeof(UCHAR));

	if(!data)
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

	ShowWindow(hDataView, SW_SHOW);

	Sleep(1000);
	SendMessage(cs->hDataManager, WM_SETFRAMESIZE, (WPARAM) w, (LPARAM) h);
	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	hDataView = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_DATAVIEW, NULL);
	hFrame = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_FRAME, NULL);

	ShowWindow(hDataView, SW_SHOW);

	while(cs->flgStream){

		//Error 6 - Internal Memory Error

		//ShowWindow(hDataView, SW_SHOW);
		cTime = clock();
		//zp.GetFrame((INT *) data, &imgSize, 0);
		//zp.GetFrame(imgSize, (INT *) data, 0);
		INT error = 0;

		//Error Code 22: Internal Error in the AxioCam Driver
		//AxioCamIC SDK: Loaded from  C:\PROGRA~2\COMMON~1\CARLZE~2\System\Zipl\3rdParty\AxioCamIC\UniControl.dll failed!

		//error = zp.GetFrame(imgSize, imgSize, (INT *) data);		//Error 22
		//error = zp.GetFrame(imgSize, 0, (INT *) data);			//Error 6
		//error = zp.GetFrame(w*h, 0, (INT *) data);				//Error 6
		//error = zp.GetFrame(0, 0, (INT *) data);					//Error 6
		//error = zp.GetFrame(imgSize, 0, (INT *) data);			//Error 6
		//error = zp.GetFrame(4*w*h, 4*w*h, (INT *) data);			//Error 22
		//error = zp.GetFrame(4*w*h, 0, (INT *) data);				//Error 6
		//error = zp.GetFrame(0, 4*w*h, (INT *) data);				//Error 22
		//error = zp.GetFrame(0, 2*w*h, (INT *) data);				//Error 22
		//error = zp.GetFrame(0, w*h, (INT *) data);				//Error 6
		//error = zp.GetFrame(0, w*h*12, (INT *) data);				//Error 22
		//error = zp.GetFrame((INT *)data, w*h*2, NULL);			//Error 22
		//error = zp.GetFrame((INT *) data, (INT)w*h*2, 0);			//Error 22
		//error = zp.NextFrame(NULL, (INT *)data, w*h*2, NULL);		//Error 8

		error = zp.GetFrame((INT *) data, imgSize, NULL);

		del = clock()-cTime;
		GetSystemTime(&pdmfh.timeStamp);

		for(p = 0, d = 0; p < max; p++, d+=2){
			temp = (data[d+1]<<8) + data[d];
			//temp = temp + data[d];
			ratio = (FLOAT) ((FLOAT) temp / (FLOAT) 4096.0);
			temp = ratio * 255;
			if(temp < 255)
				img[p] = temp;
			else
				img[p] = 255;
		}

		del = clock() - cTime;

		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
		pdmFrame.lpvoid = img;

		if(cs->flgSave){
			
			pdmfh.frame = i;
			i++;
			dump = (UCHAR *) malloc(max*sizeof(UCHAR));
			memcpy(dump, img, max*sizeof(UCHAR));
			
			pdmFrameDrop = (PDMFRAME *)calloc(1, sizeof(PDMFRAME));
			memcpy(&pdmFrameDrop->pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
			
			pdmFrameDrop->lpvoid = (VOID *)dump;

			PostMessage(cs->hDataManager, WM_DATADROP, (WPARAM) pdmFrameDrop, (LPARAM) "RAW");
			SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);

			del = clock()-cTime;
			CHAR str[MAX_STRING];
			sprintf(str, "%d", d);
			ThrowMessage(hFrame, str);
		}
		else
			SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);



	}

	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
	SendMessage(cs->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);

	zp.CloseCamera();

	return TRUE;

}

DWORD WINAPI ZeissMRmCapture(LPVOID param)
{
	CAPTURE *cs = (CAPTURE *)param; 

	HWND hDataView = NULL;
	HWND hFrame = NULL;
	HWND hLinked = NULL;

	HMODULE dllZeiss = NULL;
	ZEISSFRAMEINFO zfm = {0};
	ZEISSPROC zp = {0};

	BOOL *flgLinkRequest = NULL;

	INT i = NULL;
	INT p = NULL;
	INT d = NULL;

	INT w = NULL;
	INT h = NULL;
	INT max = NULL;

	INT temp = NULL;
	FLOAT ratio = NULL;

	INT numCamera = NULL;
	INT camInfo[256];
	INT bitsPerPixel = NULL;
	INT imgSize = NULL;

	INT result = NULL;
	INT resolution = NULL;

	clock_t cTime = NULL;
	INT del = NULL;

	UCHAR *data = NULL;
	UCHAR *img = NULL;
	UCHAR *dump = NULL;

	//Handle DLL Loading and Function Pointer Initialization
	dllZeiss = LoadZeissLibrary(hDataView, ZEISS_MRM);
	if(!ImportZeissFunctions(dllZeiss, &zp))
		return FALSE;

	//Get Camera Properties
	result = zp.InitCamera();
	numCamera = zp.GetNumCamera();
	resolution = zp.GetResolution();
	zp.GetCameraInfo(camInfo);
	bitsPerPixel = zp.GetCurrentBitsPerPixel();

	zp.GetCurrentFrameSize((INT *)&zfm);
	w = zfm.width;
	h = zfm.height;
	max = w*h;

	zp.GetCurrentImageSize((INT *)&zfm);
	//zp.GetCurrentImageSize((INT *)&imgSize);
	imgSize = 2*w*h;

	zp.SetExposure(INIT_EXPOSURE);

	if(w <= 0 || h <= 0)
		return FALSE;

	data = (UCHAR *) calloc(2*w*h, sizeof(UCHAR));
	img = (UCHAR *) calloc(max, sizeof(UCHAR));

	if(!data)
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

	ShowWindow(hDataView, SW_SHOW);

	Sleep(1000);
	SendMessage(cs->hDataManager, WM_SETFRAMESIZE, (WPARAM) w, (LPARAM) h);
	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	hDataView = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_DATAVIEW, NULL);
	hFrame = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_FRAME, NULL);

	hLinked = GetLinkedProc(cs->hOwner, flgLinkRequest);
	flgLinkRequest = (BOOL *)SendMessage(hLinked, WM_GETLINKFLAG, NULL, (LPARAM) NULL);
	SendMessage(hLinked, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM) h);

	ShowWindow(hDataView, SW_SHOW);

	//while(cs->flagOpen){
	while(cs->flgStream){
		
		//if(cs->flagStream){
		while(cs->flgExposure){
			zp.SetExposure(cs->exposure);
			cs->flgExposure = FALSE;
			cs->exposure = NULL;
		}
		

			//ShowWindow(hDataView, SW_SHOW);
			cTime = clock();
			//zp.GetFrame((INT *) data, imgSize, NULL);

			del = clock()-cTime;
			GetSystemTime(&pdmfh.timeStamp);

			for(p = 0, d = 0; p < max; p++, d+=2){
				temp = (data[d+1]<<8) + data[d];
				ratio = (FLOAT) ((FLOAT) temp / (FLOAT) 4096.0);
				temp = ratio * 255;
				if(temp < 255)
					img[p] = temp;
				else
					img[p] = 255;
			}

			del = clock() - cTime;

			memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
			pdmFrame.lpvoid = img;

			if(*flgLinkRequest){
				ReleaseLinkedData(hLinked, &pdmFrame);
				*flgLinkRequest = FALSE;
			}

			if(cs->flgSave){
				
				pdmfh.frame = i;
				i++;
				dump = (UCHAR *) malloc(max*sizeof(UCHAR));
				memcpy(dump, img, max*sizeof(UCHAR));
				
				pdmFrameDrop = (PDMFRAME *)calloc(1, sizeof(PDMFRAME));
				memcpy(&pdmFrameDrop->pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
				
				pdmFrameDrop->lpvoid = (VOID *)dump;

				PostMessage(cs->hDataManager, WM_DATADROP, (WPARAM) pdmFrameDrop, (LPARAM) "RAW");
				SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);

				del = clock()-cTime;
				CHAR str[MAX_STRING];
				sprintf(str, "%d", d);
				ThrowMessage(hFrame, str);
			}
			else{
				SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);
			}

		//}


	}

	zp.CloseCamera();

	Sleep(10000);
	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
	SendMessage(cs->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);

	

	return TRUE;

}

//DWORD WINAPI ZeissMRmCapture(LPVOID param)
//{
//	CAPTURE *cs = (CAPTURE *)param; 
//
//	HWND hDataView = NULL;
//	HWND hFrame = NULL;
//
//	HMODULE dllZeiss = NULL;
//	ZEISSFRAMEINFO zfm = {0};
//	//ZEISSPROC zp = {0};
//	ZEISSPROC *zp = (ZEISSPROC *) cs->lpVoid;
//
//
//	//memcpy(&zp, cs->lpVoid, sizeof(ZEISSPROC));
//
//	INT i = NULL;
//	INT p = NULL;
//	INT d = NULL;
//
//	INT w = NULL;
//	INT h = NULL;
//	INT max = NULL;
//
//	INT temp = NULL;
//	FLOAT ratio = NULL;
//
//	INT numCamera = NULL;
//	INT camInfo[256];
//	INT bitsPerPixel = NULL;
//	INT imgSize = NULL;
//
//	INT result = NULL;
//	INT resolution = NULL;
//
//	clock_t cTime = NULL;
//	INT del = NULL;
//
//	UCHAR *data = NULL;
//	UCHAR *img = NULL;
//	UCHAR *dump = NULL;
//
//	//Handle DLL Loading and Function Pointer Initialization
//	//dllZeiss = LoadZeissLibrary(hDataView, ZEISS_MRM);
//	//if(!ImportZeissFunctions(dllZeiss, &zp))
//	//	return FALSE;
//
//	//Get Camera Properties
//	result = zp->InitCamera();
//	numCamera = zp->GetNumCamera();
//	resolution = zp->GetResolution();
//	zp->GetCameraInfo(camInfo);
//	bitsPerPixel = zp->GetCurrentBitsPerPixel();
//
//	zp->GetCurrentFrameSize((INT *)&zfm);
//	w = zfm.width;
//	h = zfm.height;
//	max = w*h;
//
//	zp->GetCurrentImageSize((INT *)&zfm);
//	//zp.GetCurrentImageSize((INT *)&imgSize);
//	imgSize = 2*w*h;
//
//	zp->SetExposure(700);
//
//	if(w < 0 || h < 0)
//		return FALSE;
//
//	data = (UCHAR *) calloc(2*w*h, sizeof(UCHAR));
//	img = (UCHAR *) calloc(max, sizeof(UCHAR));
//
//	if(!data)
//		return FALSE;
//
//	//Begin Data Acquisition
//	//Send to DataManager
//	PDMFRAME pdmFrame = {0};
//	PDMFRAMEHEADER pdmfh = {0};
//	PDMFRAME *pdmFrameDrop = NULL;
//
//	pdmfh.width = w;
//	pdmfh.height = h;
//	pdmfh.typeMetaData = MD_FRAME;
//	pdmfh.typeData = DT_UCHAR;
//	pdmfh.countMaps = 1;
//
//	ShowWindow(hDataView, SW_SHOW);
//
//	Sleep(1000);
//	SendMessage(cs->hDataManager, WM_SETFRAMESIZE, (WPARAM) w, (LPARAM) h);
//	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);
//
//	hDataView = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_DATAVIEW, NULL);
//	hFrame = (HWND) SendMessage(cs->hDataManager, WM_GETHWND, (WPARAM) HWND_FRAME, NULL);
//
//	ShowWindow(hDataView, SW_SHOW);
//
//	while(cs->flagStream){
//
//		//ShowWindow(hDataView, SW_SHOW);
//		cTime = clock();
//		zp->GetFrame((INT *) data, imgSize, 0);
//
//		del = clock()-cTime;
//		GetSystemTime(&pdmfh.timeStamp);
//
//		for(p = 0, d = 0; p < max; p++, d+=2){
//			temp = (data[d+1]<<8) + data[d];
//			//temp = temp + data[d];
//			ratio = (FLOAT) ((FLOAT) temp / (FLOAT) 4096.0);
//			temp = ratio * 255;
//			if(temp < 255)
//				img[p] = temp;
//			else
//				img[p] = 255;
//		}
//
//		del = clock() - cTime;
//
//		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
//		pdmFrame.lpvoid = img;
//
//		if(cs->flagSave){
//			
//			pdmfh.frame = i;
//			i++;
//			dump = (UCHAR *) malloc(max*sizeof(UCHAR));
//			memcpy(dump, img, max*sizeof(UCHAR));
//			
//			pdmFrameDrop = (PDMFRAME *)calloc(1, sizeof(PDMFRAME));
//			memcpy(&pdmFrameDrop->pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
//			
//			pdmFrameDrop->lpvoid = (VOID *)dump;
//
//			PostMessage(cs->hDataManager, WM_DATADROP, (WPARAM) pdmFrameDrop, (LPARAM) "RAW");
//			SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);
//
//			del = clock()-cTime;
//			CHAR str[MAX_STRING];
//			sprintf(str, "%d", d);
//			ThrowMessage(hFrame, str);
//		}
//		else
//			SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) &pdmFrame, (LPARAM) img);
//
//
//
//	}
//
//	zp->CloseCamera();
//
//	Sleep(10000);
//	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
//	SendMessage(cs->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);
//
//	
//
//	return TRUE;
//
//}





//DWORD WINAPI ZeissHSmCapture(LPVOID param)
//{
//	CAPTURE *cs = (CAPTURE *)param; 
//
//
//	//HWND hOwner = *hwnd;
//
//	//if(hwnd)free(hwnd);
//
//	HMODULE dllZeiss = NULL;
//	ZEISSFRAMEINFO zfm = {0};
//	zeissHSm zp = {0};
//
//	INT i = NULL;
//	INT p = NULL;
//	INT d = NULL;
//
//	INT w = NULL;
//	INT h = NULL;
//	INT max = NULL;
//
//	INT temp = NULL;
//	FLOAT ratio = NULL;
//
//	INT numCamera = NULL;
//	INT camInfo[256];
//	INT bitsPerPixel = NULL;
//	INT imgSize = NULL;
//
//	INT result = NULL;
//	INT resolution = NULL;
//
//	UCHAR *data = NULL;
//	UCHAR *img = NULL;
//
//	//Handle DLL Loading and Function Pointer Initialization
//	dllZeiss = LoadZeissLibrary();
//	if(!ImportZeissFunctions(dllZeiss, &zp))
//		return FALSE;
//
//	//Get Camera Properties
//	result = zp.InitHSm();
//	numCamera = zp.GetNumCamera();
//	resolution = zp.GetResolution();
//	zp.GetCameraInfo(camInfo);
//	bitsPerPixel = zp.GetCurrentBitsPerPixel();
//
//	zp.GetCurrentFrameSize((INT *)&zfm);
//	w = zfm.width;
//	h = zfm.height;
//	max = w*h;
//
//	zp.GetCurrentImageSize((INT *)&zfm);
//	//zp.GetCurrentImageSize((INT *)&imgSize);
//	imgSize = 2*w*h;
//
//	zp.SetExposure(700);
//
//	if(w < 0 || h < 0)
//		return FALSE;
//
//	data = (UCHAR *) calloc(2*w*h, sizeof(UCHAR));
//	img = (UCHAR *) calloc(max, sizeof(UCHAR));
//
//	if(!data)
//		return FALSE;
//
//	//Begin Data Acquisition
//	//Send to DataManager
//	PDMFRAME pdmFrame = {0};
//	PDMFRAMEHEADER pdmfh = {0};
//
//	pdmfh.width = w;
//	pdmfh.height = h;
//	pdmfh.typeMetaData = MD_FRAME;
//	pdmfh.typeData = DT_UCHAR;
//	pdmfh.countMaps = 1;
//
//	Sleep(1000);
//	SendMessage(cs->hDataManager, WM_SETFRAMESIZE, (WPARAM) w, (LPARAM) h);
//	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);
//
//	while(cs->flagStream){
//
//		zp.GetFrame((INT *) data, imgSize, 0);
//
//		GetSystemTime(&pdmfh.timeStamp);
//
//		for(p = 0, d = 0; p < max; p++, d+=2){
//			//temp = (BYTE)(256.0 *(((data[p]) + (data[p+1]*256.0))/4096.0));
//			temp = (data[d+1]<<8);
//			temp = temp + data[d];
//			ratio = (FLOAT) ((FLOAT) temp / (FLOAT) 4096.0);
//			temp = ratio * 255;
//			if(temp < 255)
//				img[p] = temp;
//			else
//				img[p] = 255;
//			//img[p] = data[p];
//		}
//		
//		if(cs->flagSave){
//			pdmfh.frame = i;
//			i++;
//		}
//
//		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
//		pdmFrame.lpvoid = (VOID *)img;
//
//		SendMessage(cs->hDataManager, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");
//
//	}
//
//	SendMessage(cs->hDataManager, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) i-1);
//	SendMessage(cs->hDataManager, WM_ENDTRANSMISSION, NULL, NULL);
//
//	zp.CloseHSm();
//
//	return TRUE;
//
//}