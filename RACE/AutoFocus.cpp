#include "stdafx.h"

#include "LaserMicroscope.h"
#include "SystemProc.h"
#include "DeviceProc.h"
#include "StageProc.h"
#include "AutoFocusProc.h"
#include "A3200.h"
#include "A3200Stage.h"
#include "Magnitude.h"
#include "Norm.h"
#include "Sum.h"
#include "Power.h"
#include "Statistics.h"



DWORD AutoFocus(LASERMICROSCOPE *laserMicroscope)
{
	if(!laserMicroscope)
		return FALSE;
	
	LM_STAGE *lpStage = laserMicroscope->lpStage;
	LM_DISPLAY *lpDisplay = laserMicroscope->lpDisplay;
	LM_AUTOFOCUS *lpAutoFocus = laserMicroscope->lpAutoFocus;
	LM_DEVICE *lpDevice = laserMicroscope->lpDevice;


	DOUBLE_3DPOINT pos;
	GetA3200Position(lpDevice->hAero, &pos);


	INT i = NULL;
	INT nStep = lpAutoFocus->af.nStep;
	INT stepSize = lpAutoFocus->af.stepDist;
	INT fltrFocus = lpAutoFocus->af.filter;
	INT ref = pos.z;
	INT current = ref - ((nStep/2)*stepSize);

	DOUBLE rate = 1000;

	FLOAT max = NULL;
	INT maxPos = current;


	FLOAT *measure = (FLOAT *)calloc(nStep, sizeof(FLOAT));

	if(!measure)
		return FALSE;

	UINT w = lpDisplay->wDisplay;
	UINT h = lpDisplay->hDisplay;

	UCHAR *img = (UCHAR *)calloc(w*h, sizeof(UCHAR));
	FLOAT *fimg = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	if(!img || !fimg)
		return FALSE;

	memset(measure, NULL, nStep*sizeof(FLOAT));

	MoveToA3200Z(lpDevice->hAero, current, rate);
	//SetFocalPos(hObjective, current, AF_WAIT);

	for(i = 0; i < nStep; i++)
	{
		//WaitForFocalPos(hObjective, current);

		GetStreamImage(laserMicroscope, w, h, img);
		//GetStreamImage(lpZeissScope, w, h, img);

		//Set Focal Plane to the next position
		MoveToA3200Z(lpDevice->hAero, current+stepSize, rate);
		//SetFocalPos(hObjective, current+stepSize, AF_WAIT);

		UCHARtoFLOAT(w, h, img, fimg);
		//measure[i] = GetFocalMeasure(w, h, fimg);
		measure[i] = CalculateFocalMeasure(w, h, fimg, fltrFocus);

		if(measure[i] > max){
			max = measure[i];
			maxPos = current;
		}
		
		current = current+stepSize;

		if(!laserMicroscope->lpSystem->flgThread){
			maxPos = ref;
			i = nStep;
		}

		//CHAR str[MAX_STRING];
		//memset(str, NULL, MAX_STRING*sizeof(CHAR));
		//sprintf(str, "C:\\%d.bmp", i);
		//SaveBMP(w, h, str, img);

	}

	MoveToA3200Z(lpDevice->hAero, maxPos, rate);
	//SetFocalPos(hObjective, maxPos, AF_WAIT);

	laserMicroscope->lpSystem->flgAutoFocus = FALSE;

	if(img)free(img);
	if(fimg)free(fimg);
	if(measure)free(measure);


	return TRUE;
}

FLOAT CalculateFocalMeasure(UINT w, UINT h, FLOAT *img, DWORD filter)
{
	
	INT n = 3;
	FLOAT sigma = 1.0;
	FLOAT measure = NULL;
	FLOAT mLaplace = NULL;
	FLOAT mSobel = NULL;
	FLOAT mNormVar = NULL;

	FLOAT *imgLaplace = NULL;
	FLOAT *imgSobel = NULL;
	FLOAT *imgSqr = NULL;

	imgSqr = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	if((filter&AF_SOBEL)){
		imgSobel = (FLOAT *)calloc(w*h, sizeof(FLOAT));
		GradientSobel(w, h, 1, img, imgSobel);
		Square(w*h, imgSobel, imgSqr);
		measure += Sum(w*h, imgSqr);
	}
	if((filter&AF_LAPLACIAN)){
		imgLaplace = (FLOAT *)calloc(w*h, sizeof(FLOAT));
		Laplacian(w, h, 1, img, imgLaplace);
		Square(w*h, imgLaplace, imgSqr);
		measure += Sum(w*h, imgSqr);
	}
	if((filter&AF_VARIANCE)){
		Square(w*h, img, imgSqr);
		measure += NormalizedVariance(w*h, imgSqr, NULL);
	}

	if(imgLaplace)free(imgLaplace);
	if(imgSobel)free(imgSobel);
	if(imgSqr)free(imgSqr);

	return (FLOAT) measure;
}

DWORD GetStreamImage(LASERMICROSCOPE *laserMicroscope, DWORD w, DWORD h, UCHAR *img)
{
	if(!img || !laserMicroscope)
		return FALSE;

	LM_DISPLAY *lpDisplay = laserMicroscope->lpDisplay;

	laserMicroscope->flgGetFrame = TRUE;

	while(!laserMicroscope->flgFrameIn){
	}

	if(lpDisplay->wDisplay == w && lpDisplay->hDisplay == h){
		if(laserMicroscope->pdmf.lpvoid){
			memcpy(img, laserMicroscope->pdmf.lpvoid, w*h*sizeof(UCHAR));
		}
	}

	return TRUE;
}

DWORD GetStreamImage(LASERMICROSCOPE *laserMicroscope, DWORD w, DWORD h, FLOAT *img)
{

	UCHAR *imgUCHAR = (UCHAR *)malloc(w*h*sizeof(UCHAR));

	if(!imgUCHAR)
		return 0;

	GetStreamImage(laserMicroscope, w, h, imgUCHAR);
	UCHARtoFLOAT(w, h, imgUCHAR, img);

	free(imgUCHAR);

	return TRUE;
}

DWORD InitAutoFocus(LASERMICROSCOPE *lpMicroscope)
{
	LM_AUTOFOCUS *lpAutoFocus = lpMicroscope->lpAutoFocus;

	lpAutoFocus->flgInitAutoFocus = FALSE;



	return TRUE;
}

FLOAT GetFocalMeasure(LASERMICROSCOPE *lpMicroscope)
{

	if(!lpMicroscope)
		return FALSE;

	FLOAT measure = 0.0;
	INT w = lpMicroscope->pdmf.pdmfh.width;
	INT h = lpMicroscope->pdmf.pdmfh.height;

	UCHAR *imgUCHAR  = (UCHAR *)malloc(w*h*sizeof(UCHAR));
	FLOAT *imgFLOAT = (FLOAT *)malloc(w*h*sizeof(FLOAT));

	if(!imgUCHAR){
		if(imgFLOAT){
			free(imgFLOAT);
		}
		return FALSE;
	}

	if(!imgFLOAT){
		if(imgUCHAR){
			free(imgUCHAR);
		}
		return 0;
	}

	GetStreamImage(lpMicroscope, w, h, imgUCHAR);
	UCHARtoFLOAT(w, h, imgUCHAR, imgFLOAT);
	measure = CalculateFocalMeasure(w, h, imgFLOAT, lpMicroscope->lpAutoFocus->af.filter);

	free(imgUCHAR);
	free(imgFLOAT);

	return measure;

}