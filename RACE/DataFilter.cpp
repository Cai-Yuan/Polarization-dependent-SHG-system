#include "stdafx.h"

#include "DataFilter.h"

#include "String.h"
#include "ImageConversion.h"
#include "DataPreview.h"
#include "Subclass.h"
#include "LogOperator.h"
#include "DerivativeOperator.h"
#include "Draw.h"
#include "Vector.h"
#include "Suppression.h"
#include "ImageSubtract.h"
#include "DataBar.h"
#include "AlgorithmFxns.h"
#include "GaussianBlurr.h"
#include "StringFxns.h"
#include "Boolean.h"
#include "PavlidisContour.h"
#include "Invert.h"
#include "AlgorithmThread.h"
#include "Chamfer.h"
#include "Power.h"
#include "Statistics.h"
#include "Addition.h"
#include "ImageScale.h"
#include "MorphOperators.h"

LRESULT Filter(UINT w, UINT h, STRINGSET *ss, DWORD selFilterProc, UCHAR *in, UCHAR *out)
{

	INT i = NULL;
	INT iProc = NULL;
	DWORD selProc = selFilterProc;
	//UINT w = lpDataView->wSource;
	//UINT h = lpDataView->hSource;
	UINT iSrc = NULL;
	DWORD nProc = TRUE;
	DWORD nChar = NULL;
	FLOAT pList[MAX_PARAM];

	DWORD fRender = NULL;
	DWORD procRender = NULL;

	CHAR str[MAX_STRING];
	CHAR strIn[MAX_STRING];
	CHAR strAlgorithm[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));

	UCHAR *img = (UCHAR *) malloc(w*h*sizeof(UCHAR));

	nProc = ss->nStr;

	if(!nProc)
		nProc = TRUE;

	FLOAT **imgProc = (FLOAT **)malloc(nProc*sizeof(FLOAT*));
	memset(imgProc, NULL, nProc*sizeof(FLOAT));

	for(i = 0; i < nProc; i++){
		imgProc[i] = (FLOAT *)malloc(w*h*sizeof(FLOAT));
		if(imgProc[i]){
			memset(imgProc[i], NULL, w*h*sizeof(FLOAT));
		}
	}

	UCHARtoFLOAT(w, h, in, (FLOAT *)imgProc[0]);

	for(i = 1; i <= selProc/*nProc*/; i++){	

		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
		memset(strAlgorithm, NULL, MAX_STRING*sizeof(CHAR));
		
		nChar = ss->string[i].nChar;
		memcpy(str, ss->string[i].str, nChar*sizeof(CHAR));
		ReadStringWithin(str, strIn, '[', ']');
		iProc = atoi(strIn);
		RemoveStringWithin(str, MAX_STRING, '[', ']');
		
		memset(pList, NULL, MAX_PARAM*sizeof(DWORD));
		GetAlgorithmParams(str, strAlgorithm, pList);

		iSrc = (UINT)pList[0];

		if(!strcmp(strAlgorithm, STR_GRADIENT)){
			GradientFirstOrder(w, h, pList[1], (FLOAT *)imgProc[iSrc], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_LOG)){
			LogOperator(w, h, imgProc[(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_LAPLACIAN)){
			Laplacian(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_RGB)){
			fRender = TRUE;
			procRender = i;
		}
		else if(!strcmp(strAlgorithm, STR_SOBEL)){
			GradientSobel(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CENTRALDIFFERENCE)){
			GradientCentralDifference(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STRETCH)){
			Stretch(w, h, pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_BOOLEANAND)){
			BooleanAND(w, h, imgProc[iSrc], imgProc[(UINT)pList[1]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_BOOLEAN)){
			Boolean(w, h, pList[1], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_PAVLIDISCONTOUR)){
			PavlidisContour(w, h, imgProc[/*iSrc*/(UINT)pList[0]], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_WINDOW)){
			ClampData(w*h, pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_GAUSSBLUR)){
			GaussianBlur(w, h, (UINT)pList[1], pList[2], imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NONMAXIMUM)){
			NonMaximumSuppression(w, h, imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NONMAXIMUMEXP)){
			NonMaximumSuppressionExp(w, h, imgProc[iSrc], imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_INVERT)){
			InvertData(w*h,  imgProc[iSrc], imgProc[iProc]);
		}
		//else if(!strcmp(strAlgorithm, STR_GETFRAME)){
		//	GetFrame(lpDataView->hDataManager, w, h, (DWORD)(lpDataView->fStack + pList[0]), (FLOAT *)imgProc[iProc]);
		//}
		//else if(!strcmp(strAlgorithm, STR_GETSTATICFRAME)){
			//GetFrame(lpDataView->hDataManager, w, h, (DWORD)(pList[0]+lpDataView->fStart), (FLOAT *)imgProc[iProc]);
		//}
		else if(!strcmp(strAlgorithm, STR_SUBTRACT)){
			ImageSubtract(w, h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ADD)){
			Addition(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_SQUARE)){
			Square(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_ERODE)){
			Erode(w, h, (UINT)pList[1], (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_DILATE)){
			Dilate(w, h, (UINT)pList[1], (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_MIP)){
			fRender = TRUE;
			procRender = i;
		}
		else if(!strcmp(strAlgorithm, STR_MEAN)){
			Mean(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_STDDEV)){
			StdDev(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_VARIANCE)){
			Variance(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_NORMVARIANCE)){
			NormalizedVariance(w*h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[iProc]);
		}
		else if(!strcmp(strAlgorithm, STR_CHAMFER)){
			Chamfer(w, h, imgProc[(UINT)pList[0]], imgProc[iProc]);
		}
		else
			i = i;

	}

	if(fRender){
		UINT nParam;
		memset(str, NULL, MAX_STRING*sizeof(UCHAR));
		memset(strIn, NULL, MAX_STRING*sizeof(UCHAR));
		nChar = ss->string[procRender].nChar;
		memcpy(str, ss->string[procRender].str, nChar*sizeof(CHAR));
		nParam = GetAlgorithmParams(str, strAlgorithm, pList);
		if(nParam == 3)
			//FLOATtoPIXEL(w, h, (FLOAT *)imgProc[(UINT)pList[0]], (FLOAT *)imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], lpDataView->pixSrc);
		if(nParam == 4){
			//MaximumIntensityProjection(w, h, (FLOAT *)imgProc[(UINT)pList[0]],(FLOAT *) imgProc[(UINT)pList[1]], (FLOAT *)imgProc[(UINT)pList[2]], (FLOAT *)imgProc[(UINT)pList[3]], lpDataView->pixSrc);
		}
	}
	else{
		ClampData(w*h, 0, 255, (FLOAT *)imgProc[selProc], img);
		//UCHARtoPIXEL(w, h, (UCHAR *) img, lpDataView->pixSrc);
	}

	for(i = 0; i < nProc; i++){
		if(imgProc[i])free(imgProc[i]);
	}

	if(imgProc) free(imgProc);
	if(img) free(img);

	return TRUE;
}