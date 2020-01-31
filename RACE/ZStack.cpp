#include "stdafx.h"

#include "Stack.h"
#include "ZStack.h"
#include "MaxMin.h"
#include "DataManager.h"
#include "DerivativeOperator.h"
#include "Sum.h"
#include "String.h"
#include "ImageSubtract.h"
#include "GaussianBlurr.h"
#include "Statistics.h"
#include "Power.h"


DWORD WINAPI ZStackAlgorithm(LPVOID param)
{

	HWND *hwnd = (HWND *) param;
	HWND hDataView = NULL;
	HWND hDataManager = NULL;


	if(hwnd){
		memcpy(&hDataView, hwnd, sizeof(HWND));
		free(hwnd);
	}

	if(!hDataView){
		MessageBox(NULL, "No Data Loaded!", "ZStack Algorithm: Exiting...", MB_OK);
		return FALSE;
	}

	hDataManager = (HWND) SendMessage(hDataView, WM_GETHWND, HWND_DATAMANAGER, NULL);

	if(!hDataManager){
		MessageBox(NULL, "DataManager Not Found!", "ZStack Algorithm: Exiting...", MB_OK);
		return FALSE;
	}

	LPARAM fBounds;

	SendMessage(hDataManager, WM_COMMAND, LOWORD(DVN_GETFRAMEBOUNDS), (LPARAM) &fBounds);

	INT f = NULL;
	INT fStart = LOWORD(fBounds);
	INT fStop = HIWORD(fBounds);

	WPARAM wparam = NULL;

	SendMessage(hDataManager, WM_GETSTACKDIM, (WPARAM) &wparam, NULL);

	INT w = (INT) LOWORD(wparam);
	INT h = (INT) HIWORD(wparam);
	DWORD max = w*h;

	DWORD i = NULL;
	INT nFrame = Max(fStart, fStop) - Min(fStart, fStop);

	FLOAT **imgSet = CallocImageStack(w, h, nFrame);
	FLOAT *zStack = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	PDMFRAMEHEADER pdmfh;

	SendMessage(hDataManager, WM_GETFRAMEHEADER, (WPARAM) fStart, (LPARAM) &pdmfh);

	//Load in Frames
	for(i = 0; i < nFrame; i++){
		
		if(pdmfh.typeData == DT_PIXEL){
			PIXEL *p = (PIXEL *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
			PIXELtoFLOAT(w, h, p, imgSet[i], NULL); 
			if(p)free(p);
			
		}
		else{
			UCHAR *im = (UCHAR *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
			UCHARtoFLOAT(w, h, im, imgSet[i]);
			if(im)free(im);
		}
	}


	ZStack(w, h, nFrame, imgSet, zStack);
	
	UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));

	FLOATtoUCHAR(w, h, zStack, img);

	FILEPATHSET *fps = (FILEPATHSET *) SendMessage(hDataManager, WM_GETFILELIST, NULL, NULL);
	CHAR strPath[MAX_STRING];
	CHAR str[MAX_STRING];
	
	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
	FilePathFromPath(MAX_STRING, fps->filePath[0].lpstr, strPath);

	sprintf(str, "%s%s", strPath, "ZStackTest.bmp");

	SaveBMP(w, h, str, img);
	SaveBMP(w, h, "C:\\ZSTACK.bmp", img);


	if(img)free(img);
	if(zStack)free(zStack);
	FreeImageStack(nFrame, imgSet);

	MessageBox(NULL, "ZStack Complete", "Complete", MB_OK);

	return TRUE;

}




DWORD ZStack(DWORD width, DWORD height, DWORD nFrame, UCHAR **stack, UCHAR *imgZStack)
{

	




	return TRUE;
}

//5.29.13	
//Lucas Schmidt
//Reference Paper I: On The Extedned Depth of Focus Algorithms for Bring Field Microscopy
//Variable used adhere closely (not completely) to RefPaper I

//'p'			- The pth stack slice
//'stackRef'	- The most 'In-Focus' stack slice

DWORD ZStack(DWORD width, DWORD height, DWORD nSlice, FLOAT **stack, FLOAT *imgZStack)
{

	DWORD i = NULL;
	DWORD x = NULL;
	DWORD y = NULL;

	DWORD w = width;
	DWORD h = height;
	DWORD max = w*h;

	DWORD p = NULL;
	DWORD pFocus = NULL;

	FLOAT fMeasure = NULL;
	FLOAT fMax = NULL;
	FLOAT fMean = NULL;

	FLOAT *stackRef = NULL;
	FLOAT *blur = (FLOAT *)calloc(w*h, sizeof(FLOAT));
	
	FLOAT **edgeStack = CallocImageStack(w, h, nSlice);
	//FLOAT **edgeSobel = CallocImageStack(w, h, nSlice);
	//FLOAT **edgeLaplace	 = CallocImageStack(w, h, nSlice);

	//Find Reference Slice (Most 'In-Focus' stack slice)
	for(p = 0; p < nSlice; p++){
		GaussianBlur(w, h, 3, 1.0, stack[p], blur);
		//GradientSobel(w, h, 1, blur, edgeStack[p]);
		Laplacian(w, h, 4, blur, edgeStack[p]);
		
		//fMeasure = Variance(w*h, stack[p], NULL);
		fMeasure = Sum(w*h, edgeStack[p]);

		if(fMeasure > fMax){
			fMax = fMeasure;
			pFocus = p;
		}

	}

	//Initialize Stack Reference Slice
	stackRef = stack[pFocus];

	//Initialize Z-Stack Image 
	memcpy(imgZStack, stackRef, w*h*sizeof(FLOAT));

	FLOAT *pSlice;
	FLOAT *pMax;
	DWORD mMax;
	//Find Measure of Focus for each pixel in every pth slice
	for(i = 0; i < max; i++){
	
		pMax = edgeStack[pFocus];
		mMax = pFocus;
		for(p = 0; p < nSlice; p++){

			pSlice = stack[p];
			if(pSlice[i] > pMax[i]){
				pMax = edgeStack[p];
				mMax = p;
			}

		}

		pSlice = stack[mMax];
		imgZStack[i] = (imgZStack[i] + pSlice[i]) / 2;
		//imgZStack[i] = pSlice[i];
	}




	FreeImageStack(nSlice, edgeStack);
	//FreeImageStack(nSlice, edgeSobel);
	//FreeImageStack(nSlice, edgeLaplace);
	if(blur)free(blur);


	return TRUE;
}




















//DWORD WINAPI ZStackAlgorithm(LPVOID param)
//{
//
//	HWND *hwnd = (HWND *) param;
//	HWND hDataView = NULL;
//	HWND hDataManager = NULL;
//
//	if(hwnd){
//		memcpy(&hDataView, hwnd, sizeof(HWND));
//		free(hwnd);
//	}
//
//	if(!hDataView){
//		MessageBox(NULL, "No Data Loaded!", "ZStack Algorithm: Exiting...", MB_OK);
//		return FALSE;
//	}
//
//	hDataManager = (HWND) SendMessage(hDataView, WM_GETHWND, HWND_DATAMANAGER, NULL);
//
//	if(!hDataManager){
//		MessageBox(NULL, "DataManager Not Found!", "ZStack Algorithm: Exiting...", MB_OK);
//		return FALSE;
//	}
//
//	LPARAM fBounds;
//
//	SendMessage(hDataManager, WM_COMMAND, LOWORD(DVN_GETFRAMEBOUNDS), (LPARAM) &fBounds);
//
//	INT f = NULL;
//	INT fStart = LOWORD(fBounds);
//	INT fStop = HIWORD(fBounds);
//
//	WPARAM wparam = NULL;
//
//	SendMessage(hDataManager, WM_GETSTACKDIM, (WPARAM) &wparam, NULL);
//
//	INT w = (INT) LOWORD(wparam);
//	INT h = (INT) HIWORD(wparam);
//	DWORD max = w*h;
//
//	DWORD i = NULL;
//	DWORD nFrame = Max(fStart, fStop) - Min(fStart, fStop);
//
//	FLOAT **imgSet = (FLOAT **)calloc(nFrame, sizeof(FLOAT *));
//
//	for(i = 0; i < nFrame; i++){
//		imgSet[i] = (FLOAT *)calloc(w*h, sizeof(FLOAT));
//	}
//
//	PDMFRAMEHEADER pdmfh;
//
//	SendMessage(hDataManager, WM_GETFRAMEHEADER, (WPARAM) fStart, (LPARAM) &pdmfh);
//
//	FLOAT totalSum = NULL;
//	FLOAT totalAvg = NULL;
//
//	//Load in Frames
//	for(i = 0; i < nFrame; i++){
//		
//		if(pdmfh.typeData == DT_PIXEL){
//			PIXEL *p = (PIXEL *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
//			PIXELtoFLOAT(w, h, p, imgSet[i], NULL); 
//			totalSum += Sum(w*h, imgSet[i]);
//			if(p)free(p);
//			
//		}
//		else{
//			UCHAR *im = (UCHAR *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
//			UCHARtoFLOAT(w, h, im, imgSet[i]);
//			totalSum += Sum(w*h, imgSet[i]);
//			if(im)free(im);
//		}
//	}
//
//	totalAvg = (totalSum / (nFrame*w*h));
//	
//
//	FLOAT **edgeSet = (FLOAT **)malloc(nFrame*sizeof(FLOAT *));
//
//	INT iMax = NULL;
//	INT edgeMax = NULL;
//	INT edgeCnt = NULL;
//	
//	for(i = 0; i < nFrame; i++){
//		edgeSet[i] = (FLOAT *)calloc(w*h,sizeof(FLOAT));
//		
//		GradientSobel(w, h, 1, imgSet[i], edgeSet[i]);
//		//Laplacian(w, h, 3, imgSet[i], edgeSet[i]);
//		
//		edgeCnt = Sum(w*h, edgeSet[i]);
//
//		ConstSubtract(w, h, totalAvg, imgSet[i], edgeSet[i]);
//
//		if(edgeCnt > edgeMax){
//			edgeMax = edgeCnt;
//			iMax = i;
//		}
//			
//	}
//
//	FLOAT *zStack = (FLOAT *)malloc(w*h*sizeof(FLOAT));
//	memcpy(zStack, imgSet[iMax], w*h*sizeof(FLOAT));
//
//	INT fMax = NULL;
//
//	FLOAT *edgeRef = edgeSet[iMax];
//	FLOAT *edgeCmp;
//
//	for(i = 0; i < max; i++){
//
//		fMax = iMax;
//		edgeRef = edgeSet[iMax];
//		for(f = 0; f < nFrame; f++){
//
//			edgeCmp = edgeSet[f];
//			if(Magnitude(edgeCmp[i]) > Magnitude(edgeRef[i])){
//				edgeRef = edgeCmp/*edgeSet[f]*/;
//				fMax = f;
//			}
//			
//		}
//
//		//if(fMax != iMax){
//			edgeCmp = imgSet[fMax];
//			zStack[i] = (FLOAT) edgeCmp[i]/*edgeCmp[fMax]*/;/*(edgeCmp[i] + zStack[i]) / 2.0;*///*(imgSet + fMax)[i];
//		//}
//
//	}
//	
//	UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));
//
//	FLOATtoUCHAR(w, h, zStack, img);
//
//	FILEPATHSET *fps = (FILEPATHSET *) SendMessage(hDataManager, WM_GETFILELIST, NULL, NULL);
//	CHAR strPath[MAX_STRING];
//	CHAR str[MAX_STRING];
//	
//	memset(str, NULL, MAX_STRING*sizeof(CHAR));
//	memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
//	FilePathFromPath(MAX_STRING, fps->filePath[0].lpstr, strPath);
//
//	sprintf(str, "%s%s", strPath, "ZStackTest.bmp");
//
//	SaveBMP(w, h, str, img);
//	SaveBMP(w, h, "C:\\ZSTACK.bmp", img);
//
//
//	if(img)free(img);
//
//	if(zStack)free(zStack);
//
//	//Free Memory
//	for(i = 0; i < nFrame; i++){
//		if(imgSet[i])free(imgSet[i]);
//	}
//	if(imgSet)free(imgSet);
//
//	//Free Memory
//	for(i = 0; i < nFrame; i++){
//		if(edgeSet[i])free(edgeSet[i]);
//	}
//	if(edgeSet)free(edgeSet);
//
//	MessageBox(NULL, "ZStack Complete", "Complete", MB_OK);
//
//	return TRUE;
//
//}

//DWORD WINAPI ZStackAlgorithm(LPVOID param)
//{
//
//	HWND *hwnd = (HWND *) param;
//	HWND hDataView = NULL;
//	HWND hDataManager = NULL;
//
//	if(hwnd){
//		memcpy(&hDataView, hwnd, sizeof(HWND));
//		free(hwnd);
//	}
//
//	if(!hDataView){
//		MessageBox(NULL, "No Data Loaded!", "ZStack Algorithm: Exiting...", MB_OK);
//		return FALSE;
//	}
//
//	hDataManager = (HWND) SendMessage(hDataView, WM_GETHWND, HWND_DATAMANAGER, NULL);
//
//	if(!hDataManager){
//		MessageBox(NULL, "DataManager Not Found!", "ZStack Algorithm: Exiting...", MB_OK);
//		return FALSE;
//	}
//
//	LPARAM fBounds;
//
//	SendMessage(hDataManager, WM_COMMAND, LOWORD(DVN_GETFRAMEBOUNDS), (LPARAM) &fBounds);
//
//	INT f = NULL;
//	INT fStart = LOWORD(fBounds);
//	INT fStop = HIWORD(fBounds);
//
//	WPARAM wparam = NULL;
//
//	SendMessage(hDataManager, WM_GETSTACKDIM, (WPARAM) &wparam, NULL);
//
//	INT w = (INT) LOWORD(wparam);
//	INT h = (INT) HIWORD(wparam);
//	DWORD max = w*h;
//
//	DWORD i = NULL;
//	DWORD nFrame = Max(fStart, fStop) - Min(fStart, fStop);
//
//	FLOAT **imgSet = (FLOAT **)calloc(nFrame, sizeof(FLOAT *));
//
//	for(i = 0; i < nFrame; i++){
//		imgSet[i] = (FLOAT *)calloc(w*h, sizeof(FLOAT));
//	}
//
//	PDMFRAMEHEADER pdmfh;
//
//	SendMessage(hDataManager, WM_GETFRAMEHEADER, (WPARAM) fStart, (LPARAM) &pdmfh);
//
//
//	//Load in Frames
//	for(i = 0; i < nFrame; i++){
//		
//		if(pdmfh.typeData == DT_PIXEL){
//			PIXEL *p = (PIXEL *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
//			PIXELtoFLOAT(w, h, p, imgSet[i], NULL); 
//			if(p)free(p);
//			
//		}
//		else{
//			UCHAR *im = (UCHAR *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
//			UCHARtoFLOAT(w, h, im, imgSet[i]);
//			if(im)free(im);
//		}
//	}
//
//	FLOAT **edgeSet = (FLOAT **)malloc(nFrame*sizeof(FLOAT *));
//
//	INT iMax = NULL;
//	INT edgeMax = NULL;
//	INT edgeCnt = NULL;
//	for(i = 0; i < nFrame; i++){
//		edgeSet[i] = (FLOAT *)calloc(w*h,sizeof(FLOAT));
//		GradientSobel(w, h, 1, imgSet[i], edgeSet[i]);
//		
//		edgeCnt = Sum(w*h, edgeSet[i]);
//
//		if(edgeCnt > edgeMax){
//			edgeMax = edgeCnt;
//			iMax = i;
//		}
//			
//	}
//
//	FLOAT *zStack = (FLOAT *)malloc(w*h*sizeof(FLOAT));
//	memcpy(zStack, imgSet[iMax], w*h*sizeof(FLOAT));
//
//	INT fMax = NULL;
//
//	{
//		FLOAT *edgeRef = edgeSet[iMax];
//		FLOAT *edgeCmp = NULL;
//		FLOAT *edge = NULL;
//		FLOAT *img = NULL;
//
//		FLOAT sum = NULL;
//		FLOAT edgeSum = NULL;
//
//		FLOAT *edgeMap = (FLOAT *) malloc(nFrame*sizeof(FLOAT));
//		FLOAT *imgMap = (FLOAT *) malloc(nFrame*sizeof(FLOAT));
//
//
//		for(i = 0; i < max; i++){
//
//			sum = NULL;
//			edgeSum = NULL;
//
//			fMax = iMax;
//			edgeRef = edgeSet[iMax];
//			for(f = 0; f < nFrame; f++){
//
//				img = imgSet[f];
//				edge = edgeSet[f];
//
//				edgeMap[f] = edge[i];
//				imgMap[f] = img[i];
//
//				//sum = sum + sum[i];
//				edgeSum = edgeSum + edge[i];
//				
//				
//			}
//
//			zStack[i] = (sum + zStack[i])/2;
//
//		}
//
//		if(edgeMap)free(edgeMap);
//		if(imgMap)free(imgMap);
//	}
//	
//	UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));
//
//	FLOATtoUCHAR(w, h, zStack, img);
//
//	FILEPATHSET *fps = (FILEPATHSET *) SendMessage(hDataManager, WM_GETFILELIST, NULL, NULL);
//	CHAR strPath[MAX_STRING];
//	CHAR str[MAX_STRING];
//	
//	memset(str, NULL, MAX_STRING*sizeof(CHAR));
//	memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
//	FilePathFromPath(MAX_STRING, fps->filePath[0].lpstr, strPath);
//
//	sprintf(str, "%s%s", strPath, "ZStack Test.bmp");
//
//	SaveBMP(w, h, str, img);
//	SaveBMP(w, h, "C:\\ZSTACK.bmp", img);
//
//
//	if(img)free(img);
//
//	if(zStack)free(zStack);
//
//	//Free Memory
//	for(i = 0; i < nFrame; i++){
//		if(imgSet[i])free(imgSet[i]);
//	}
//	if(imgSet)free(imgSet);
//
//	//Free Memory
//	for(i = 0; i < nFrame; i++){
//		if(edgeSet[i])free(edgeSet[i]);
//	}
//	if(edgeSet)free(edgeSet);
//
//	MessageBox(NULL, "ZStack Complete", "Complete", MB_OK);
//
//	return TRUE;
//
//}

//DWORD WINAPI ZStackAlgorithm(LPVOID param)
//{
//
//	HWND *hwnd = (HWND *) param;
//	HWND hDataView = NULL;
//	HWND hDataManager = NULL;
//
//	if(hwnd){
//		memcpy(&hDataView, hwnd, sizeof(HWND));
//		free(hwnd);
//	}
//
//	if(!hDataView){
//		MessageBox(NULL, "No Data Loaded!", "ZStack Algorithm: Exiting...", MB_OK);
//		return FALSE;
//	}
//
//	hDataManager = (HWND) SendMessage(hDataView, WM_GETHWND, HWND_DATAMANAGER, NULL);
//
//	if(!hDataManager){
//		MessageBox(NULL, "DataManager Not Found!", "ZStack Algorithm: Exiting...", MB_OK);
//		return FALSE;
//	}
//
//	LPARAM fBounds;
//
//	SendMessage(hDataManager, WM_COMMAND, LOWORD(DVN_GETFRAMEBOUNDS), (LPARAM) &fBounds);
//
//	INT f = NULL;
//	INT fStart = LOWORD(fBounds);
//	INT fStop = HIWORD(fBounds);
//
//	WPARAM wparam = NULL;
//
//	SendMessage(hDataManager, WM_GETSTACKDIM, (WPARAM) &wparam, NULL);
//
//	INT w = (INT) LOWORD(wparam);
//	INT h = (INT) HIWORD(wparam);
//	DWORD max = w*h;
//
//	DWORD i = NULL;
//	DWORD nFrame = Max(fStart, fStop) - Min(fStart, fStop);
//
//	FLOAT **imgSet = (FLOAT **)calloc(nFrame, sizeof(FLOAT *));
//
//	for(i = 0; i < nFrame; i++){
//		imgSet[i] = (FLOAT *)calloc(w*h, sizeof(FLOAT));
//	}
//
//	PDMFRAMEHEADER pdmfh;
//
//	SendMessage(hDataManager, WM_GETFRAMEHEADER, (WPARAM) fStart, (LPARAM) &pdmfh);
//
//
//	//Load in Frames
//	for(i = 0; i < nFrame; i++){
//		
//		if(pdmfh.typeData == DT_PIXEL){
//			PIXEL *p = (PIXEL *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
//			PIXELtoFLOAT(w, h, p, imgSet[i], NULL); 
//			if(p)free(p);
//			
//		}
//		else{
//			UCHAR *im = (UCHAR *) SendMessage(hDataManager, WM_RETURNFRAME, (WPARAM) i + fStart, (LPARAM) FS_RAW);
//			UCHARtoFLOAT(w, h, im, imgSet[i]);
//			if(im)free(im);
//		}
//	}
//
//	FLOAT **edgeSet = (FLOAT **)malloc(nFrame*sizeof(FLOAT *));
//
//	INT iMax = NULL;
//	INT edgeMax = NULL;
//	INT edgeCnt = NULL;
//
//	FLOAT *blur = (FLOAT *)malloc(w*h*sizeof(FLOAT));
//	for(i = 0; i < nFrame; i++){
//		edgeSet[i] = (FLOAT *)calloc(w*h,sizeof(FLOAT));
//		GaussianBlur(w, h, 3, 1.0, imgSet[i], blur);
//		GradientSobel(w, h, 1, blur/*imgSet[i]*/, edgeSet[i]);
//		//Laplacian(w, h, 1, imgSet[i], edgeSet[i]);
//		
//		edgeCnt = Sum(w*h, edgeSet[i]);
//
//		if(edgeCnt > edgeMax){
//			edgeMax = edgeCnt;
//			iMax = i;
//		}
//			
//	}
//
//	if(blur)free(blur);
//
//	FLOAT *zStack = (FLOAT *)malloc(w*h*sizeof(FLOAT));
//	memcpy(zStack, imgSet[iMax], w*h*sizeof(FLOAT));
//
//	INT fMax = NULL;
//
//	FLOAT *edgeRef = edgeSet[iMax];
//	FLOAT *edgeCmp;
//
//	for(i = 0; i < max; i++){
//
//		fMax = iMax;
//		edgeRef = edgeSet[iMax];
//		for(f = 0; f < nFrame; f++){
//
//			edgeCmp = edgeSet[f];
//			if(Magnitude(edgeCmp[i]) > Magnitude(edgeRef[i])){
//				edgeRef = edgeCmp/*edgeSet[f]*/;
//				fMax = f;
//			}
//			
//		}
//
//		//if(fMax != iMax){
//			edgeCmp = imgSet[fMax];
//			zStack[i] = (FLOAT) edgeCmp[i]/*edgeCmp[fMax]*/;/*(edgeCmp[i] + zStack[i]) / 2.0;*///*(imgSet + fMax)[i];
//		//}
//
//	}
//	
//	UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));
//
//	FLOATtoUCHAR(w, h, zStack, img);
//
//	FILEPATHSET *fps = (FILEPATHSET *) SendMessage(hDataManager, WM_GETFILELIST, NULL, NULL);
//	CHAR strPath[MAX_STRING];
//	CHAR str[MAX_STRING];
//	
//	memset(str, NULL, MAX_STRING*sizeof(CHAR));
//	memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
//	FilePathFromPath(MAX_STRING, fps->filePath[0].lpstr, strPath);
//
//	sprintf(str, "%s%s", strPath, "ZStackTest.bmp");
//
//	SaveBMP(w, h, str, img);
//	SaveBMP(w, h, "C:\\ZSTACK.bmp", img);
//
//
//	if(img)free(img);
//
//	if(zStack)free(zStack);
//
//	//Free Memory
//	for(i = 0; i < nFrame; i++){
//		if(imgSet[i])free(imgSet[i]);
//	}
//	if(imgSet)free(imgSet);
//
//	//Free Memory
//	for(i = 0; i < nFrame; i++){
//		if(edgeSet[i])free(edgeSet[i]);
//	}
//	if(edgeSet)free(edgeSet);
//
//	MessageBox(NULL, "ZStack Complete", "Complete", MB_OK);
//
//	return TRUE;
//
//}