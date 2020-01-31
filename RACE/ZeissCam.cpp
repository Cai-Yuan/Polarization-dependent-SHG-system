#include "stdafx.h"

#include "ZeissCam.h"

DWORD InitializeZeissCamera(HWND hwnd, DWORD flgCam, ZEISSPROC *zp)
{

	ZEISSPROC zeissHSm;

	HMODULE dll = LoadZeissLibrary(hwnd, flgCam);

	if(ImportZeissFunctions(dll, &zeissHSm))
		return TRUE;

	return FALSE;
}

HMODULE LoadZeissLibrary(HWND hwnd, DWORD flgCam)
{

	HMODULE dll = NULL;

	switch(flgCam)
	{
	case ZEISS_HSM:
		dll = LoadLibrary((CHAR *)"mcamhs.dll");
		break;
	case ZEISS_MRM:
		dll = LoadLibrary((CHAR *)"mcammr.dll");
		break;
	case ZEISS_MRC:
		dll = LoadLibrary((CHAR *)"mcammc.dll");
		break;
	default:
		dll = NULL;
		break;
	}

	if(!dll)
		ThrowMessage(hwnd, "Load Zeiss Library Failed");

	return dll;
}

HMODULE LoadZeissLibrary(HWND hwnd)
{

	HMODULE dll = NULL;
	CHAR strPath[MAX_STRING];
	memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
	sprintf(strPath, "%s", "C:\\\mcamhs.dll");

	dll = LoadLibrary((CHAR *)/*strPath*/"mcamhs.dll");

	if(!dll)
		ThrowMessage(hwnd, "Load Zeiss Library Failed");
	

	return dll;
}

//Names always being with Underscore
//End with:
	//@(number of bytes as input arguments)
DWORD ImportZeissFunctions(HMODULE dll, ZEISSPROC *zp)
{
	BOOL flag = NULL;
	DWORD i = NULL;

	memset(zp, NULL, sizeof(ZEISSPROC));

	zp->InitCamera = (PROC_Void) GetProcAddress(dll, "_McamInit@0");
	if(!zp->InitCamera)
		flag = TRUE;

	zp->CloseCamera = (PROC_Void) GetProcAddress(dll, "_McamClose@0");
	if(!zp->CloseCamera)
		flag = TRUE;

	zp->GetResolution = (Sgn_PROC_Void) GetProcAddress(dll, "_McamGetCurrentResolution@0");
	if(!zp->GetResolution)
		flag = TRUE;

	zp->GetCameraInfo = (PROC_Int_Ptr) GetProcAddress(dll, "_McamInfo@4");
	if(!zp->GetCameraInfo)
		flag = TRUE;

	zp->GetNumCamera = (Int_PROC_Void) GetProcAddress(dll, "_McamGetNumberofCameras@0");
	if(!zp->GetNumCamera)
		flag = TRUE;

	zp->GetCurrentBitsPerPixel = (Int_PROC_Void) GetProcAddress(dll, "_McamGetCurrentBitsPerPixel@0");
	if(!zp->GetCurrentBitsPerPixel)
		flag = TRUE;

	zp->GetCurrentImageSize = (PROC_Int_Ptr) GetProcAddress(dll, "_McamGetCurrentImageSize@4");
	if(!zp->GetCurrentImageSize)
		flag = TRUE;

	zp->GetCurrentFrameSize = (PROC_Int_Ptr) GetProcAddress(dll, "_McamGetCurrentFrameSize@4");
	if(!zp->GetCurrentFrameSize)
		flag = TRUE;

	zp->GetFrame = (SgnInt_PROC_Acquisition) GetProcAddress(dll, "_McamAcquisition@12");
	if(!zp->GetFrame)
		flag = TRUE;

	zp->SetExposure = (PROC_Int_Val) GetProcAddress(dll, "_McamSetExposure@4");
	if(!zp->SetExposure)
		flag = TRUE;

	zp->GetCurrentExposure = (PROC_IntVal_IntPtr) GetProcAddress(dll, "_McammGetCurrentExposure@8");
	if(!zp->GetCurrentExposure)
		flag = TRUE;

	zp->NextFrame = (SgnInt_PROC_NextAcquisition) GetProcAddress(dll, "_McammNextAcquisition@16");
	if(!zp->NextFrame)
		flag = TRUE;

	zp->CamIsAvailable = (Int_PROC_Void)GetProcAddress(dll, "_MCamIsAvailable@0");
	if(!zp->CamIsAvailable)
		flag = TRUE;

	zp->StartFastAcquisition = (Int_PROC_Void)GetProcAddress(dll, "_McamStartFastAcquisition@0");
	if(!zp->StartFastAcquisition)
		flag = TRUE;
	//HSm Only
	zp->InitializeAcquisition = (SgnInt_PROC_SgnInt) GetProcAddress(dll, "_McammInitializeAcquisition@4");
	if(!zp->InitializeAcquisition){
		flag = TRUE;
	}


	if(flag){
		//if(zp)free(zp);
		//zp = NULL;
		return FALSE;
	}

	return TRUE;
}

DWORD InitCamera(ZEISSPROC *zp)
{
	INT result = NULL;
	if(zp->InitCamera){
		result = zp->InitCamera();
		return result;
	}

	return FALSE;
}

DWORD CloseCamera(ZEISSPROC *zp)
{
	if(zp->CloseCamera){
		zp->CloseCamera();
		memset(zp, NULL, sizeof(ZEISSPROC));
		return TRUE;
	}

	return FALSE;
}

DWORD SetExposure(ZEISSPROC *zp, INT exposure)
{
	DWORD i = NULL;
	DWORD exp = NULL;
	DWORD result = NULL;
	if(zp->SetExposure){
		for(i = 0; i < 50; i++){
			result = zp->SetExposure(exposure);
			//exp = GetExposure(zp);
			if(!result){
				return result;
			}
		}
		
	}
	return result;
}



DWORD GetExposure(ZEISSPROC *zp)
{
	DWORD exposure = NULL;
	INT t1[256];
	INT t2[256];
	memset(t1, NULL, 256*sizeof(INT));
	memset(t2, NULL, 256*sizeof(INT));
	if(zp->GetCurrentExposure){
		exposure = zp->GetCurrentExposure(NULL, t1);
		exposure = t1[0];
		return exposure;
	}
	return FALSE;
}

DWORD GetCurrentImageSize(ZEISSPROC *zp, INT *width, INT *height)
{
	ZEISSFRAMEINFO zfm;
	*width = NULL;
	*height = NULL;

	if(zp->GetCurrentImageSize){
		zp->GetCurrentImageSize((INT *)&zfm);
		*width = zfm.width;
		*height = zfm.height;
		return TRUE;
	}

	return FALSE;
}


DWORD GetNumCamera(ZEISSPROC *zp)
{
	if(zp->GetNumCamera){
		return zp->GetNumCamera();
	}

	return FALSE;
}