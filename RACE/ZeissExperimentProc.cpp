#include "stdafx.h"
#include "ZeissMicroscope.h"
#include "GetFolderPath.h"
#include "CameraProperties.h"
#include "StageProperties.h"


DWORD ZeissChannelExposureFromMask(ZEISSMICROSCOPE *lpZeissScope, DWORD mask);

DWORD WINAPI ZeissExperimentThread(LPVOID param)
{
	INT i = NULL;
	INT w = NULL;
	INT h = NULL;
	INT nChannel = 1;

	HWND hwnd;

	HANDLE hStage;
	HANDLE hMicroscope;

	ZEISSMICROSCOPE *lpZeissScope;
	ZEISSPOSITION zp = {0};
	ZEISSPOSITION zpBegin = {0};

	CHAR str[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strChannel[MAX_STRING];
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strFile[MAX_STRING];
	memset(strFile, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strPath[MAX_STRING];
	memset(strPath, NULL, MAX_STRING*sizeof(CHAR));

	UCHAR *img = NULL;

	INT row = NULL;
	INT col = NULL;
	INT z = NULL;	//Z-Stack Index
	INT c = NULL;	//Channel Index

	INT nSlice = NULL;

	if(param){
		memcpy(&hwnd, (VOID *)param, sizeof(HWND));
		free(param);
		param = NULL;
	}
	else
		return NULL;

	lpZeissScope = (ZEISSMICROSCOPE *)SendMessage(hwnd, WM_GETUSERDATA, NULL, NULL);

	w = lpZeissScope->w;
	h = lpZeissScope->h;
	hStage = lpZeissScope->hStage;
	hMicroscope = lpZeissScope->hMicroscope;

	img = (UCHAR *)malloc(w*h*sizeof(UCHAR));
	memset(img, NULL, w*h*sizeof(UCHAR));

	DWORD flgMosaic = lpZeissScope->ze.flgMosaic;
	DWORD flgZStack = lpZeissScope->ze.flgZStack;
	DWORD flgAutoFocus = lpZeissScope->ze.flgAutoFocus;

	zp.x = GetXAxisPos(hStage)/STEPPERUM;
	zp.y = GetYAxisPos(hStage)/STEPPERUM;
	zp.z = GetFocalPos(hMicroscope);

	zpBegin.x = zp.x;
	zpBegin.y = zp.y;
	zpBegin.z = zp.z;

	memcpy(&zpBegin, &zp, sizeof(ZEISSPOSITION));

	SetAxisSpeed(hStage, 500, XAXIS|YAXIS);

	//if(!flgZStack){
	//	nSlice = 1;
	//	flgZStack = TRUE;
	//}else{
	//	nSlice = lpZeissScope->zs.nSlice;
	//}


	while(lpZeissScope->ze.flgRun){

		if(flgMosaic){
			//Update XY-Position
			memcpy(&zp, &zpBegin, sizeof(ZEISSPOSITION));
			flgMosaic = UpdateFieldOfView(hStage, lpZeissScope->fov, &zp, &row, &col); 
			memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
			sprintf(strPath, "%s\(%d, %d)", lpZeissScope->strRootPath, zp.x, zp.y);
			CreateValidDirectory(strPath);
		}

		for(c = 1; c < 32; c = c*2){

			//Set Channel
			//Set Exposure

			if(c&lpZeissScope->mc.filter){

				SendMessage(lpZeissScope->hwnd, ZSM_SETCHANNEL, ZeissChannelFromMask(c), NULL);
				SendMessage(lpZeissScope->hCameraProperties, 
								CPM_SETEXPOSURE, 
								CAMERA_ZEISS_MRM,
								(WPARAM)ZeissChannelExposureFromMask(lpZeissScope, c));
				Sleep(2000);
			
				ZeissChannelStringFromMask(c, strChannel);
				memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(strPath, "%s\(%d, %d)\\%s", lpZeissScope->strRootPath, zp.x, zp.y, strChannel);
				CreateValidDirectory(strPath);

				if(flgZStack){
					//Update Z-Position
					//flgZStack = UpdateFieldOfView(hObjective, 
					for(z = 0; z < lpZeissScope->zs.nSlice; z++){
						
						SetFocalPos(hMicroscope, zpBegin.z + (z*lpZeissScope->zs.distance));
						WaitForFocalPos(hMicroscope, zpBegin.z + (z*lpZeissScope->zs.distance));
						GetStreamImage(lpZeissScope, w, h, img);
						sprintf(str, "%s\%d.bmp", strPath, zpBegin.z + (z*lpZeissScope->zs.distance));
						SaveBMP(w, h, str, img);

						if(!lpZeissScope->ze.flgRun)
							break;
					}
				}
				else if(flgAutoFocus){
					//Autofocus
					AutoFocus(lpZeissScope);
					Sleep(250);
					GetStreamImage(lpZeissScope, w, h, img);
					sprintf(str, "%s\%d.bmp", strPath, zpBegin.z + (z*lpZeissScope->zs.distance));
					SaveBMP(w, h, str, img);
				}
				else{
					//Do Nothing
				}
			}

			if(!lpZeissScope->ze.flgRun)
				break;
		}

		//for(/*nChannel*/){
			//Apply Channel
			//Set Channel Exposure
			//Capture Image
			//Save to Folder
		//}
		
		if(flgAutoFocus){
			//AutoFocus
			//Capture Image
			//Save to Folder
		}

		if(!flgMosaic){
			lpZeissScope->ze.flgRun = FALSE;
		}

		if(!lpZeissScope->ze.flgRun){
			//GoTo Initial Position
			//Select First Channel
			//Set Channel Exposure

			SetXAxisPos(hStage, zpBegin.x*STEPPERUM);
			SetYAxisPos(hStage, zpBegin.y*STEPPERUM);
			SetFocalPos(hMicroscope, zpBegin.z);
			break;
		}


	}


	SendMessage(lpZeissScope->hExperiment, ZSM_STOPEXPERIMENT, NULL, NULL);

	return TRUE;
}

DWORD UpdateFieldOfView(HANDLE hStage, FIELDOFVIEW fov, ZEISSPOSITION *zp, INT *row, INT *col)
{
	INT r = *row;
	INT c = *col;

	INT xGoTo = NULL;
	INT yGoTo = NULL;

	//Get Field of View from Objective
	INT wUM = FOV_X_40x;
	INT hUM = FOV_Y_40x;

	INT xStep = wUM - (wUM*fov.overlap);
	INT yStep = hUM - (hUM*fov.overlap);

	xGoTo = (zp->x-(c*xStep));
	yGoTo = (zp->y+(r*yStep));

	SetXAxisPos(hStage, xGoTo, SP_MICRON);
	SetYAxisPos(hStage, yGoTo, SP_MICRON);

	WaitForPosition(hStage, xGoTo, yGoTo, SP_MICRON);

	zp->x = xGoTo;
	zp->y = yGoTo;

	(*row)++;


	if(*row >= fov.nRow){
		*row = NULL;
		(*col)++;
	}

	if(*col >= fov.nCol){
		*col = NULL;
		return FALSE;
	}
	
	return TRUE;
}

//DWORD UpdateFieldOfView(HANDLE hStage, FIELDOFVIEW fov, ZEISSPOSITION *zp, INT *row, INT *col)
//{
//	INT r = *row;
//	INT c = *col;
//
//	INT xGoTo = NULL;
//	INT yGoTo = NULL;
//
//	//Get Field of View from Objective
//	INT wUM = FOV_X_40x;
//	INT hUM = FOV_Y_40x;
//
//	INT xStep = (1*wUM) - (wUM*fov.overlap);
//	INT yStep = (1*hUM) - (hUM*fov.overlap);
//
//	xGoTo = ((zp->x-(c*xStep))*STEPPERUM);
//	yGoTo = ((zp->y+(r*yStep))*STEPPERUM);
//
//	SetXAxisPos(hStage, xGoTo);
//	SetYAxisPos(hStage, yGoTo);
//
//	WaitForPosition(hStage, xGoTo, yGoTo);
//
//	zp->x = xGoTo;
//	zp->y = yGoTo;
//
//	(*row)++;
//
//
//	if(*row >= fov.nRow){
//		*row = NULL;
//		(*col)++;
//	}
//
//	if(*col >= fov.nCol){
//		*col = NULL;
//		return FALSE;
//	}
//	
//	return TRUE;
//}

DWORD UpdateZStackPosition(HANDLE hObjective, ZSTACK zs, ZEISSPOSITION zpBegin, INT *z)
{
	INT zGoTo = NULL;

	zGoTo = zpBegin.z + (*z)*zs.distance;
	SetFocalPos(hObjective, zGoTo);

	*z++;
	if(*z >= zs.nSlice){
		*z = NULL;
	}

	return TRUE;
}

DWORD ZeissChannelFromMask(DWORD mask)
{
	switch(mask){
		case MC_DAPI:
			return CHNL_DAPI;
		case MC_FITC:
			return CHNL_FITC;
		case MC_RHODAMINE:
			return CHNL_RHODAMINE;
		case MC_CY5:
			return CHNL_CY5;
		case MC_TRANSLIGHT:
			return CHNL_BRIGHTFIELD;
		default:
			return NULL;
	}

}

DWORD ZeissChannelExposureFromMask(ZEISSMICROSCOPE *lpZeissScope, DWORD mask)
{
	switch(mask){
		case MC_DAPI:
			return (DWORD) lpZeissScope->mc.expDAPI;
		case MC_FITC:
			return (DWORD) lpZeissScope->mc.expFITC;
		case MC_RHODAMINE:
			return (DWORD) lpZeissScope->mc.expRhodamine;
		case MC_CY5:
			return (DWORD) lpZeissScope->mc.expCY5;
		case MC_TRANSLIGHT:
			return (DWORD) lpZeissScope->mc.expBrightField;
		default:
			return NULL;
	}
}

DWORD ZeissChannelStringFromMask(DWORD mask, CHAR *str)
{
	if(!str)
		return NULL;

	switch(mask){
		case MC_DAPI:
			sprintf(str, "%s", "DAPI");
			return CHNL_DAPI;
		case MC_FITC:
			sprintf(str, "%s", "FITC");
			return CHNL_FITC;
		case MC_RHODAMINE:
			sprintf(str, "%s", "RHODAMINE");
			return CHNL_RHODAMINE;
		case MC_CY5:
			sprintf(str, "%s", "CY5");
			return CHNL_CY5;
		case MC_TRANSLIGHT:
			sprintf(str, "%s", "BRIGHTFIELD");
			return CHNL_BRIGHTFIELD;
		default:
			return NULL;
	}

}


//DWORD CaptureZStack(HANDLE hObjective, ZSTACK zs, ZEISSPOSITION zpBegin, UCHAR **imgStack)
//{
//	INT z = NULL;	//Z-Stack Index
//
//	for(z = 0; z < zs.nSlice; z++){
//		SetFocalPos(
//
//	}
//
//
//}









//while(lpZeissScope->ze.flgRun){

//	if(1/*mosiac*/){
//		//Update XY-Position
//	}

//	if(1/*flgZStack*/){
//		//Update Z-Position
//	}

//	//for(/*nChannel*/){
//		//Apply Channel
//		//Set Channel Exposure
//		//Capture Image
//		//Save to Folder
//	//}
//	
//	if(1/*AutoFocus*/){
//		//AutoFocus
//		//Capture Image
//		//Save to Folder
//	}

//	if(!lpZeissScope->ze.flgRun){
//		//GoTo Initial Position
//		//Select First Channel
//		//Set Channel Exposure
//		break;
//	}


//}