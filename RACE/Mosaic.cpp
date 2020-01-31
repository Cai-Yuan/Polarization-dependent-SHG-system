#include "stdafx.h"

#include "Mosaic.h"
#include "String.h"
#include "Stack.h"
#include "StringFxns.h"
#include "ZeissMicroscope.h"

#include "stdio.h"
#include <iostream>

#ifdef RACE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/stitching/stitcher.hpp"
#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#endif

DWORD WINAPI MosaicThread(LPVOID param)
{

	HWND *hwnd = (HWND *) param;
	HWND hDataView = NULL;
	HWND hDataManager = NULL;

	//Stores File Paths return from 
	//FILEPATHSET	fps;	
	//memset((VOID *) &fps, NULL, sizeof(FILEPATHSET));	

	FILEPATHSET fns;
	memset((VOID *) &fns, NULL, sizeof(FILEPATHSET));

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

	//Get File Names
	//Row-Column values within parenthesis dictate where their approximate 
	//position will be in the final projection
	VOID *lpFilePathSet = (VOID *) SendMessage(hDataManager, WM_GETFILELIST, NULL, NULL);
	FileNameSetFromFilePathSet((FILEPATHSET *)lpFilePathSet, &fns);

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
	DWORD nFrame = Max(fStart, fStop) - Min(fStart, fStop);

	FLOAT **imgSet = CallocImageStack(w, h, nFrame);

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


	//Get Row-Col Indicies from File Name Set (fns)	
	INT nRow = NULL;
	INT nCol = NULL;
	POINT *ptRowCol = (POINT *) calloc(fns.nPaths, sizeof(POINT));
	
	for(i = 0; i < fns.nPaths; i++){

		CHAR str[MAX_STRING];
		memset(str, NULL, MAX_STRING*sizeof(CHAR));

		ReadStringWithin(fns.filePath[i].lpstr, str, '(', ')');
		sscanf(str, "%d, %d", &ptRowCol[i].y, &ptRowCol[i].x);


		//Keep Track of Matrix Dimensions
		if(ptRowCol[i].y > nRow){
			nRow = ptRowCol[i].y;
		}

		if(ptRowCol[i].x > nCol){
			nCol = ptRowCol[i].x;
		}
	}

	//Apply offset form zero-based indexing system
	nRow++;
	nCol++;


	INT p = NULL;
	FLOAT **imgMatrix = CallocImageStack(w, h, nFrame);
	FLOAT *iMatrix = NULL;

	for(i = 0; i < fns.nPaths-1; i++){

		p = (ptRowCol[i].y*nCol) + ptRowCol[i].x;

		memcpy(imgMatrix[p], imgSet[i], w*h*sizeof(FLOAT));
		
	}

	UINT wMosaic = NULL;
	UINT hMosaic = NULL;
	UCHAR *mosaic = NULL;// = (UCHAR *)calloc(w*h, sizeof(UCHAR));

	//SurfFeatureDetector detector(4000);

	//DirectCompareMosaic(w, h, &wMosaic, &hMosaic, nRow, nCol, 0.1, imgMatrix, &mosaic);
	//ZStack(w, h, nFrame, imgSet, zStack);
	//
	//UCHAR *img = (UCHAR *)malloc(w*h*sizeof(UCHAR));

	//FLOATtoUCHAR(w, h, zStack, img);

	//FILEPATHSET *fps = (FILEPATHSET *) SendMessage(hDataManager, WM_GETFILELIST, NULL, NULL);
	//CHAR strPath[MAX_STRING];
	//CHAR str[MAX_STRING];
	//
	//memset(str, NULL, MAX_STRING*sizeof(CHAR));
	//memset(strPath, NULL, MAX_STRING*sizeof(CHAR));
	//FilePathFromPath(MAX_STRING, fps->filePath[0].lpstr, strPath);

	//sprintf(str, "%s%s", strPath, "ZStackTest.bmp");

	//SaveBMP(w, h, str, img);
	SaveBMP(wMosaic, hMosaic, "C:\\Mosaic.bmp", mosaic);


	//if(img)free(img);
	FreeImageStack(nFrame, imgSet);

	//MessageBox(NULL, "ZStack Complete", "Complete", MB_OK);

	return TRUE;

}

