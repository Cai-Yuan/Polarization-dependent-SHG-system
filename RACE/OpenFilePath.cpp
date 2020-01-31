#include "stdafx.h"

#include "CommDlg.h"
#include "FilePath.h"
#include "OpenFilePath.h"
#include "StringFxns.h"

#ifdef _WIN32
#include "tiff.h"
#include "tiffio.h"
#endif

#ifdef _WIN64
#endif

#include "ReadTIFF.h"
#include "RAW.h"

#include "Archive.h"
#include "PDM.h"

#define KILOBYTE	1024
//#define MEGABYTE	1024*1024
#define GIGABYTE	1024*1024*1024

#ifdef RACE_OPENCV

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#endif

UINT GetOpenFilePaths(HWND hParent, UINT filter, FILEPATHSET *fps)
{
	HANDLE hThread;
	OPENFILENAME ofn;
	DWORD ExtenedError;
	UINT strl = 0;
	CHAR strFilter[MAX_STRING];
	//CHAR filePath[MAX_PATH*MAX_PATHS];
	CHAR fileExtension[MAX_PATH];

	CHAR *filePath = (CHAR *)malloc(MAX_PATH*MAX_PATHS*sizeof(CHAR));


	
	if(!filePath)
		return FALSE;

	memset(filePath, 0x00, MAX_PATH*MAX_PATHS*sizeof(CHAR));
	memset(fileExtension, NULL, MAX_PATH*sizeof(CHAR));
	memset(strFilter, NULL, MAX_STRING*sizeof(CHAR));

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = (LPSTR) filePath;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hParent;
	ofn.nMaxFile = MAX_PATH*MAX_PATHS*sizeof(filePath);
	ofn.lpstrFilter = TEXT("PDM(*.pdm)\0*.pdm\0TIFF(*.tif)\0*.tif\0BMP(*.bmp)\0*.bmp\0RAW(*.raw)\0*.raw\0PGM(*.pgm)\0*.pgm\0\0\0");
	//ofn.lpstrFilter = TEXT("TIFF(*.tif)\0*.tif\0PDM(*.pdm)\0*.pdm\0BMP(*.bmp)\0*.bmp\0RAW(*.raw)\0*.raw\0PGM(*.pgm)\0*.pgm\0SUPPORTED(*.tiff*.bmp*.raw)\0*.tiff;*.bmp;*.raw\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	
	if(GetOpenFileName(&ofn))
	{
		GetFilePaths(ofn, fps);

		//Return File Type
		return ofn.nFilterIndex;
	}
	else
	{
		//Error
		if((ExtenedError = CommDlgExtendedError() != 0)){
			MessageBox(NULL, (LPCSTR) "Could Not Open File!", "Error!", MB_OK | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	if(filePath)
		free(filePath);

	return TRUE;

}

DWORD GetOpenFilePaths(HWND hParent, UINT filter, STRINGSET *ss)
{

	HANDLE hThread;
	OPENFILENAME ofn;
	DWORD ExtenedError;
	UINT strl = 0;
	CHAR strFilter[MAX_STRING];
	CHAR fileExtension[MAX_PATH];

	CHAR *filePath = (CHAR *)malloc(MAX_PATH*MAX_PATHS*sizeof(CHAR));
	
	if(!filePath)
		return FALSE;

	memset(filePath, 0x00, MAX_PATH*MAX_PATHS*sizeof(CHAR));
	memset(fileExtension, NULL, MAX_PATH*sizeof(CHAR));
	memset(strFilter, NULL, MAX_STRING*sizeof(CHAR));

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = (LPSTR) filePath;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hParent;
	ofn.nMaxFile = MAX_PATH*MAX_PATHS*sizeof(filePath);
	ofn.lpstrFilter = TEXT("PDM(*.pdm)\0*.pdm\0TIFF(*.tif)\0*.tif\0BMP(*.bmp)\0*.bmp\0RAW(*.raw)\0*.raw\0PGM(*.pgm)\0*.pgm\0\0\0");
	//ofn.lpstrFilter = TEXT("TIFF(*.tif)\0*.tif\0PDM(*.pdm)\0*.pdm\0BMP(*.bmp)\0*.bmp\0RAW(*.raw)\0*.raw\0PGM(*.pgm)\0*.pgm\0SUPPORTED(*.tiff*.bmp*.raw)\0*.tiff;*.bmp;*.raw\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	
	if(GetOpenFileName(&ofn))
	{
		GetFilePaths(ofn, ss);

		//Return File Type
		return ofn.nFilterIndex;
	}
	else
	{
		//Error
		if((ExtenedError = CommDlgExtendedError() != 0)){
			MessageBox(NULL, (LPCSTR) "Could Not Open File!", "Error!", MB_OK | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	return TRUE;
}
//2.5.14 (NOT IMPLEMENTED/TESTED)
//Will allocate the the FILEPATHSET struture to the correct size before filling it
//Removes the need to know how many files the user plans to open before calling GetOpenFileName()
//Must use FILEPATHSETEX since it is allocated dynamically
//UINT GetOpenFilePaths(HWND hParent, UINT filter, FILEPATHSETEX **fps)
//{
//	HANDLE hThread;
//	OPENFILENAME ofn;
//	DWORD ExtenedError;
//	UINT strl = 0;
//	CHAR strFilter[MAX_STRING];
//	CHAR filePath[MAX_PATH*MAX_PATHS];
//	CHAR fileExtension[MAX_PATH];
//
//	memset(filePath, 0x00, MAX_PATH*MAX_PATHS*sizeof(CHAR));
//	memset(fileExtension, NULL, MAX_PATH*sizeof(CHAR));
//	memset(strFilter, NULL, MAX_STRING*sizeof(CHAR));
//
//	ZeroMemory(&ofn, sizeof(ofn));
//	ofn.lStructSize = sizeof(ofn);
//	ofn.lpstrFile = (LPSTR) filePath;
//	ofn.lpstrFile[0] = '\0';
//	ofn.hwndOwner = hParent;
//	ofn.nMaxFile = sizeof(filePath);
//	ofn.lpstrFilter = TEXT("PDM(*.pdm)\0*.pdm\0TIFF(*.tif)\0*.tif\0BMP(*.bmp)\0*.bmp\0RAW(*.raw)\0*.raw\0PGM(*.pgm)\0*.pgm\0\0\0");
//	//ofn.lpstrFilter = TEXT("TIFF(*.tif)\0*.tif\0PDM(*.pdm)\0*.pdm\0BMP(*.bmp)\0*.bmp\0RAW(*.raw)\0*.raw\0PGM(*.pgm)\0*.pgm\0SUPPORTED(*.tiff*.bmp*.raw)\0*.tiff;*.bmp;*.raw\0\0");
//	ofn.nFilterIndex = 1;
//	ofn.lpstrInitialDir = NULL;
//	ofn.lpstrFileTitle = NULL;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
//
//	
//	if(GetOpenFileName(&ofn))
//	{
//		GetFilePaths(ofn, fps);
//
//		//Return File Type
//		return ofn.nFilterIndex;
//	}
//	else
//	{
//		//Error
//		if((ExtenedError = CommDlgExtendedError() != 0)){
//			MessageBox(NULL, (LPCSTR) "Could Not Open File!", "Error!", MB_OK | MB_ICONINFORMATION);
//		}
//		return FALSE;
//	}
//
//	return TRUE;
//
//}

UINT GetOpenFileProfile(HWND hParent, FILEPATHSET *fps)
{

	HANDLE hThread;
	OPENFILENAME ofn;
	DWORD ExtenedError;
	UINT strl = 0;
	CHAR strFilter[MAX_STRING];
	CHAR filePath[MAX_PATH*MAX_PATHS];
	CHAR fileExtension[MAX_PATH];

	memset(filePath, 0x00, MAX_PATH*MAX_PATHS*sizeof(CHAR));
	memset(fileExtension, NULL, MAX_PATH*sizeof(CHAR));
	memset(strFilter, NULL, MAX_STRING*sizeof(CHAR));

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = (LPSTR) filePath;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hParent;
	ofn.nMaxFile = sizeof(filePath);
	ofn.lpstrFilter = TEXT("RPF(*.rpf)\0*.rpf\0\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	
	if(GetOpenFileName(&ofn))
	{
		GetFilePaths(ofn, fps);

		//Return File Type
		return ofn.nFilterIndex;
	}
	else
	{
		//Error
		if((ExtenedError = CommDlgExtendedError() != 0)){
			MessageBox(NULL, (LPCSTR) "Could Not Open File!", "Error!", MB_OK | MB_ICONINFORMATION);
		}
		return FALSE;
	}

	return TRUE;

}

//Modified 8.17.2012
UINT GetFilePaths(OPENFILENAME ofn, FILEPATHSET *filePathSet)
{
	FILEPATH filePathDirectory;
	BOOL MULTIFILE = FALSE;
	CHAR *fileDirectory;

	//Backslash
	CHAR s[] = {92, 0};

	UINT nChars = 0;
	UINT iChar = 0;

	UINT nFiles = 0;
	UINT iFile = 0;

	UINT strcnt = 0;
	UINT strpos = 0;

	UINT strl;
	HANDLE hFile;
	FILE *pFile;
	DWORD BytesWritten;
	//CHAR *pchr = NULL;
	UINT pos = 0;
	CHAR temp;
	UINT strPosMax = 0;

	//Initialize the number of files in the FILEPATHSET
	filePathSet->nPaths = 0;
	
	//Load File Directory String
	strncpy((CHAR *)filePathDirectory.lpstr, ofn.lpstrFile, (ofn.nFileOffset));
	strl = strlen(ofn.lpstrFile);
	nChars = strlen(ofn.lpstrFile);

	//Identify whether the user requested 'Single-' or 'Multi-'  File Load
	if(strl < ofn.nFileOffset ){
		MULTIFILE = TRUE;
		iChar = 0;
		iFile = 0;
		iChar = nChars + 1;
		strpos = strl + 1;
	}
	else{
		MULTIFILE = FALSE;
		strcnt++;
	}

	//Multi-File Loading Method
	if(MULTIFILE){
		nChars = strlen(ofn.lpstrFile);
		strcat(filePathDirectory.lpstr, s);

		//Continue as long as there is another string [file] to be stored into the FILEPATHSET
		while((strl = strlen(ofn.lpstrFile + iChar)) != 0){

			//Zero String Values
			memset(filePathSet->filePath[iFile].lpstr, 0x00, MAX_PATH);

			//Copy in the Directory (Ex. C:\Archive\) (Ex. F:\Archive)
			strcpy(filePathSet->filePath[iFile].lpstr, filePathDirectory.lpstr);

			//Append the File name to the Directory String (Ex. C:\Archive\Test.txt)
			//This Completes the File Name
			strcat(filePathSet->filePath[iFile].lpstr, (ofn.lpstrFile + iChar));

			//Keeps track of where you are in the OPENFILENAME.lpstrFile string
			iChar = iChar + strl + 1;
			
			//Increment the number of files in the FILEPATHSET
			iFile++;
			filePathSet->nPaths = iFile;
		}
	}

	//[9.26.2012]
	//Single-File Loading Method
	if(!MULTIFILE)
	{
		filePathSet->nPaths = 1;

		memset(filePathSet->filePath[0].lpstr, 0x00, MAX_PATH);
		strcpy(filePathSet->filePath[0].lpstr, ofn.lpstrFile);
		//Add Single File Loading Capability Here

	}
	

	return iFile;
}

//Modified 3.13.2015
UINT GetFilePaths(OPENFILENAME ofn, STRINGSET *ss)
{
	FILEPATH filePathDirectory;
	BOOL MULTIFILE = FALSE;
	CHAR *fileDirectory;
	CHAR str[MAX_PATH];

	//Backslash
	CHAR s[] = {92, 0};

	UINT nChars = 0;
	UINT iChar = 0;

	UINT nFiles = 0;
	UINT iFile = 0;

	UINT strcnt = 0;
	UINT strpos = 0;

	UINT strl;
	HANDLE hFile;
	FILE *pFile;
	DWORD BytesWritten;
	//CHAR *pchr = NULL;
	UINT pos = 0;
	CHAR temp;
	UINT strPosMax = 0;

	if(!ss)
		return FALSE;


	//Initialize the number of files in the FILEPATHSET
	//filePathSet->nPaths = 0;
	
	//Load File Directory String
	strncpy((CHAR *)filePathDirectory.lpstr, ofn.lpstrFile, (ofn.nFileOffset));
	strl = strlen(ofn.lpstrFile);
	nChars = strlen(ofn.lpstrFile);

	//Identify whether the user requested 'Single-' or 'Multi-'  File Load
	if(strl < ofn.nFileOffset ){
		MULTIFILE = TRUE;
		iChar = 0;
		iFile = 0;
		iChar = nChars + 1;
		strpos = strl + 1;
	}
	else{
		MULTIFILE = FALSE;
		strcnt++;
	}

	//Multi-File Loading Method
	if(MULTIFILE){
		nChars = strlen(ofn.lpstrFile);
		strcat(filePathDirectory.lpstr, s);

		//Continue as long as there is another string [file] to be stored into the FILEPATHSET
		while((strl = strlen(ofn.lpstrFile + iChar)) != 0){

			//Zero String Values
			memset(str, NULL, MAX_PATH);
			//memset(filePathSet->filePath[iFile].lpstr, 0x00, MAX_PATH);

			//Copy in the Directory (Ex. C:\Archive\) (Ex. F:\Archive)
			strcpy(str, filePathDirectory.lpstr);
			//strcpy(filePathSet->filePath[iFile].lpstr, filePathDirectory.lpstr);
			

			//Append the File name to the Directory String (Ex. C:\Archive\Test.txt)
			//This Completes the File Name
			strcat(str, (ofn.lpstrFile+iChar));
			//strcat(filePathSet->filePath[iFile].lpstr, (ofn.lpstrFile + iChar));

			//Append String to the STRINGSET 'ss'
			Append(ss, str);

			//Keeps track of where you are in the OPENFILENAME.lpstrFile string
			iChar = iChar + strl + 1;
			
			//Increment the number of files in the FILEPATHSET
			iFile++;
			//filePathSet->nPaths = iFile;
		}
	}

	//[9.26.2012]
	//Single-File Loading Method
	if(!MULTIFILE)
	{
		memset(str, NULL, MAX_PATH);
		strcpy(str, ofn.lpstrFile);
		Append(ss, str);
		//filePathSet->nPaths = 1;
		//memset(filePathSet->filePath[0].lpstr, 0x00, MAX_PATH);
		//strcpy(filePathSet->filePath[0].lpstr, ofn.lpstrFile);
		//Add Single File Loading Capability Here

	}
	

	return iFile;
}

//2.5.14 (NOT IMPLEMENTED/TESTED)
//Will allocate the the FILEPATHSET struture to the correct size before filling it
//Removes the need to know how many files the user plans to open before calling GetOpenFileName()
//Must use FILEPATHSETEX since it is allocated dynamically
//UINT GetFilePaths(OPENFILENAME ofn, FILEPATHSETEX **filePathSet)
//{
//	FILEPATH filePathDirectory;
//	BOOL MULTIFILE = FALSE;
//	CHAR *fileDirectory;
//
//	//Backslash
//	CHAR s[] = {92, 0};
//
//	UINT nChars = 0;
//	UINT iChar = 0;
//
//	UINT nFiles = 0;
//	UINT iFile = 0;
//
//	UINT strcnt = 0;
//	UINT strpos = 0;
//
//	UINT strl;
//	HANDLE hFile;
//	FILE *pFile;
//	DWORD BytesWritten;
//	//CHAR *pchr = NULL;
//	UINT pos = 0;
//	CHAR temp;
//	UINT strPosMax = 0;
//
//	//Initialize the number of files in the FILEPATHSET
//	filePathSet->nPaths = 0;
//	
//	//Load File Directory String
//	strncpy((CHAR *)filePathDirectory.lpstr, ofn.lpstrFile, (ofn.nFileOffset));
//	strl = strlen(ofn.lpstrFile);
//	nChars = strlen(ofn.lpstrFile);
//
//	//Identify whether the user requested 'Single-' or 'Multi-'  File Load
//	if(strl < ofn.nFileOffset ){
//		MULTIFILE = TRUE;
//		iChar = 0;
//		iFile = 0;
//		iChar = nChars + 1;
//		strpos = strl + 1;
//	}
//	else{
//		MULTIFILE = FALSE;
//		strcnt++;
//	}
//
//	//Multi-File Loading Method
//	if(MULTIFILE){
//		nChars = strlen(ofn.lpstrFile);
//		strcat(filePathDirectory.lpstr, s);
//
//		//Continue as long as there is another string [file] to be stored into the FILEPATHSET
//		while((strl = strlen(ofn.lpstrFile + iChar)) != 0){
//
//			//Zero String Values
//			memset(filePathSet->filePath[iFile].lpstr, 0x00, MAX_PATH);
//
//			//Copy in the Directory (Ex. C:\Archive\) (Ex. F:\Archive)
//			strcpy(filePathSet->filePath[iFile].lpstr, filePathDirectory.lpstr);
//
//			//Append the File name to the Directory String (Ex. C:\Archive\Test.txt)
//			//This Completes the File Name
//			strcat(filePathSet->filePath[iFile].lpstr, (ofn.lpstrFile + iChar));
//
//			//Keeps track of where you are in the OPENFILENAME.lpstrFile string
//			iChar = iChar + strl + 1;
//			
//			//Increment the number of files in the FILEPATHSET
//			iFile++;
//			filePathSet->nPaths = iFile;
//		}
//	}
//
//	//[9.26.2012]
//	//Single-File Loading Method
//	if(!MULTIFILE)
//	{
//		filePathSet->nPaths = 1;
//
//		memset(filePathSet->filePath[0].lpstr, 0x00, MAX_PATH);
//		strcpy(filePathSet->filePath[0].lpstr, ofn.lpstrFile);
//		//Add Single File Loading Capability Here
//
//	}
//	
//
//	return iFile;
//}

HANDLE CreateOpenFileThread(HWND hOwner, UINT typeFile, FILEPATHSET *fps)
{
	
	HANDLE hThread = NULL;

	static OPENFILETHREAD oft;
	oft.hOwner = hOwner;
	oft.typeFile = typeFile;
	oft.fps = fps;

	switch(typeFile){
		case PDM:
			hThread = CreateThread(NULL, NULL, OpenFileThreadPDM, &oft, NULL, NULL);
		break;
		case RAW:
			hThread = CreateThread(NULL, NULL, OpenFileThreadRAW, &oft, NULL, NULL);
			break;
		case TIF:	//[Works Only On x86]
			hThread = CreateThread(NULL, NULL, OpenFileThreadTIFF, &oft, NULL, NULL);
			break;
		case BMP:
			hThread = CreateThread(NULL, NULL, OpenFileThreadBMP, &oft, NULL, NULL);
		default:
			break;
	}

	return hThread;
}

//HANDLE CreateOpenFileThread(HWND hOwner, UINT typeFile, STRINGSET *ss)
//{
//	
//	HANDLE hThread = NULL;
//
//	static OPENFILETHREAD oft;
//	oft.hOwner = hOwner;
//	oft.typeFile = typeFile;
//	oft.ss = ss;
//
//	switch(typeFile){
//		case PDM:
//			//if(!VerifyStackDimensionsPDM(fps))
//			//	ThrowError(hOwner, "Mis-match stack dimensions");
//			//else
//				hThread = CreateThread(NULL, NULL, OpenFileThreadPDM, &oft, NULL, NULL);
//		break;
//		case RAW:
//			hThread = CreateThread(NULL, NULL, OpenFileThreadRAW, &oft, NULL, NULL);
//			break;
//		case TIF:	//[Works Only On x86]
//			if(!VerifyStackDimensionsTIFF(fps))
//				ThrowError(hOwner, "Mis-match stack dimensions");
//			else
//				hThread = CreateThread(NULL, NULL, OpenFileThreadTIFF, &oft, NULL, NULL);
//			break;
//		case BMP:
//			hThread = CreateThread(NULL, NULL, OpenFileThreadBMP, &oft, NULL, NULL);
//		default:
//			break;
//	}
//
//	return hThread;
//}

DWORD WINAPI OpenFileThreadTIFF(LPVOID param)
{
#ifndef _WIN64
	OPENFILETHREAD *OFT = (OPENFILETHREAD *) param;

	OPENFILETHREAD oft = *OFT;

	FILEPATHSET fps = {0};
	memcpy(&fps, oft.fps, sizeof(FILEPATHSET));

	HWND hwnd = oft.hOwner;
	INT nPaths = oft.fps->nPaths;

	INT f = 0;
	INT result = 0;
	UINT32 sizeScanLine = 0;
	UINT32 bytePerPixel = 0;
	UINT32 bufBytePerPixel = 0;


	UCHAR *buf = NULL;
	UCHAR *tiff = NULL;
	VOID *lpvoid = NULL;
	TIFF *tif = NULL;

	
	LONG w = 0;
	LONG h = 0;
	LONG row = 0;
	LONG max = 0;
	

	w = ReadTIFFWidth(fps.filePath[0].lpstr);
	h = ReadTIFFHeight(fps.filePath[0].lpstr);
	max = w*h;

	tif = TIFFOpen(fps.filePath[f].lpstr, "r");

	if(tif){

		sizeScanLine = TIFFScanlineSize(tif);
		bufBytePerPixel = sizeScanLine/w;
	}

	//w = ReadTIFFWidth(
	PDMFRAME pdmFrame = {0};
	AllocPDMFrame(&pdmFrame, w, h, DT_UCHAR);
	PDMFRAMEHEADER *pdmfh = &pdmFrame.pdmfh;
	//pdmfh.width = w;
	//pdmfh.height = h;
	//pdmfh.typeMetaData = MD_FRAME;
	//pdmfh.typeData = DT_UCHAR;
	////pdmfh.timeStamp = (LONG) clock();
	////pdmfh.typeData = DT_UCHAR;
	//pdmfh.countMaps = 1;

	

	//Sleep(500);

	DataManager_SetFrameSize(hwnd, w, h);
	DataManager_SetStartFrame(hwnd, f);
	//SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
	//SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	buf = (UCHAR *) calloc(w*h*bufBytePerPixel, sizeof(UCHAR));
	tiff = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	if(!tiff || !buf || !pdmFrame.lpvoid)return FALSE;

	for(f = 0; f < nPaths; f++){
		
		//Update Frame Count
		pdmfh->frame = f;

		//IplImage *img = cvLoadImage(fps.filePath[f].lpstr, CV_LOAD_IMAGE_UNCHANGED);

		//Open TIFF File
		tif = TIFFOpen(fps.filePath[f].lpstr, "r");

		//Read TIFF File
		if(tif){

			sizeScanLine = TIFFScanlineSize(tif);
			bytePerPixel = sizeScanLine/w;

			if(bytePerPixel != bufBytePerPixel)
				break;

			//buf = (UCHAR *) calloc(w*h*bytePerPixel, sizeof(UCHAR));
			//tiff = (UCHAR *) calloc(w*h, sizeof(UCHAR));

			//if(tiff && buf){
				//if(bytePerPixel == 1){
				//	for(row = 0; row < h; row++){

				//		TIFFReadScanline(tif, tiff + (row*w), row);

				//	}
				//}
				//else if(bytePerPixel == 2){
				//
				//	for(row = 0; row < h; row++){

				//		TIFFReadScanline(tif, buf + (row*w*bytePerPixel), row);

				//	}

				//	CompressTIFF2(w, h, buf, tiff);
				//}
				//else if(bytePerPixel == 3){

				//	for(row = 0; row < h; row++){

				//		TIFFReadScanline(tif, buf + (row*w*bytePerPixel), row);

				//	}

				//	CompressTIFF3(w, h, buf, tiff);
				//}
				//else
				//	ThrowMessage(hwnd, "TIFF BytesPerPixel Unrecognized");
			//}

			switch(bytePerPixel)
			{
			case 1:
				for(row = 0; row < h; row++){

					TIFFReadScanline(tif, tiff + (row*w), row);

				}
				break;
			case 2:
				for(row = 0; row < h; row++){
					TIFFReadScanline(tif, buf + (row*w*bytePerPixel), row);
				}
				CompressTIFF2(w, h, buf, tiff);
				break;
			case 3:
				for(row = 0; row < h; row++){
					TIFFReadScanline(tif, buf + (row*w*bytePerPixel), row);
				}
				CompressTIFF3(w, h, buf, tiff);
				break;
			default:
				ThrowMessage(hwnd, ".TIFF BytesPerPixel Unrecognized");
				break;
			}

			//Fill PDMFRAME before transfer
			//memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
			//pdmFrame.lpvoid = (VOID *) tiff;

			memcpy(pdmFrame.lpvoid, tiff, max*sizeof(UCHAR));
			

			//Transfer PDMFRAME
			DataManager_SendData(hwnd, pdmFrame, "RAW", SEND_DATA);
			//SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");
			//SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmfh, (LPARAM) tiff);


		}

	}		

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) nPaths-1);
	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);

	if(buf){free(buf);buf = NULL;}
	if(tiff){free(tiff);tiff = NULL;}

#endif
	return TRUE;
}

DWORD WINAPI OpenFileThreadRAW(LPVOID param)
{
	OPENFILETHREAD *OFT = (OPENFILETHREAD *) param;

	OPENFILETHREAD oft = *OFT;

	FILEPATHSET fps = {0};
	memcpy(&fps, oft.fps, sizeof(FILEPATHSET));

	HWND hRoot = NULL;
	HWND hwnd = oft.hOwner;
	INT nPaths = oft.fps->nPaths;

	INT f;
	UINT32 sizeScanLine = 0;
	UINT32 bytePerPixel = 0;

	CHAR str[MAX_STRING];

	UCHAR *raw = NULL;
	UCHAR *tiff = NULL;
	VOID *lpvoid = NULL;
	TIFF *tif = NULL;

	
	long w = 1312;
	long h = 1080;
	long row = 0;

	//w = ReadTIFFWidth(fps.filePath[0].lpstr);
	//h = ReadTIFFHeight(fps.filePath[0].lpstr);

	//w = ReadTIFFWidth(
	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};
	pdmfh.width = w;
	pdmfh.height = h;
	pdmfh.typeMetaData = MD_FRAME;
	//pdmfh.typeData = DT_UCHAR;
	//pdmfh.timeStamp = (LONG) clock();
	pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;

	//Sleep(500);
	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	hRoot = (HWND)SendMessage(hwnd, WM_GETOWNERHWND, NULL, NULL);

	for(f = 0; f < nPaths; f++){

		PDMFRAME *lpFramePDM = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
		
		sprintf(str, "Frame %4d", f);
		ThrowMessage(hRoot, str);
		
		//Update Frame Count
		pdmfh.frame = f;

		raw = (UCHAR *) calloc(w*h, sizeof(UCHAR));

		ReadRawImageUCHAR(w, h, raw, fps.filePath[f].lpstr);

		//Fill PDMFRAME before transfer
		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
		pdmFrame.lpvoid = (VOID *) raw;
		memcpy(lpFramePDM, &pdmFrame, sizeof(PDMFRAME));

		//Transfer PDMFRAME
		SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");
		//PostMessage(hwnd, WM_DATADROP, (WPARAM) lpFramePDM, (LPARAM) "RAW");

		//if(raw){free(raw);}
		//if(tiff){free(tiff);}

	}		

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) nPaths-1);
	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);

	return TRUE;
}

//DWORD WINAPI OpenFileThreadRAW(LPVOID param)
//{
//	OPENFILETHREAD *OFT = (OPENFILETHREAD *) param;
//
//	OPENFILETHREAD oft = *OFT;
//
//	FILEPATHSET fps = {0};
//	memcpy(&fps, oft.fps, sizeof(FILEPATHSET));
//
//	HWND hRoot = NULL;
//	HWND hwnd = oft.hOwner;
//	INT nPaths = oft.fps->nPaths;
//
//	INT f;
//	UINT32 sizeScanLine = 0;
//	UINT32 bytePerPixel = 0;
//
//	CHAR str[MAX_STRING];
//
//	UCHAR *raw = NULL;
//	UCHAR *tiff = NULL;
//	VOID *lpvoid = NULL;
//	TIFF *tif = NULL;
//
//	
//	long w = 1312;
//	long h = 1080;
//	long row = 0;
//
//	//w = ReadTIFFWidth(fps.filePath[0].lpstr);
//	//h = ReadTIFFHeight(fps.filePath[0].lpstr);
//
//	//w = ReadTIFFWidth(
//	PDMFRAME pdmFrame = {0};
//	PDMFRAMEHEADER pdmfh = {0};
//	pdmfh.width = w;
//	pdmfh.height = h;
//	pdmfh.typeMetaData = MD_FRAME;
//	//pdmfh.typeData = DT_UCHAR;
//	//pdmfh.timeStamp = (LONG) clock();
//	pdmfh.typeData = DT_UCHAR;
//	pdmfh.countMaps = 1;
//
//	//Sleep(500);
//	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
//	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);
//
//	hRoot = (HWND)SendMessage(hwnd, WM_GETOWNERHWND, NULL, NULL);
//
//	for(f = 0; f < nPaths; f++){
//
//		
//		sprintf(str, "Frame %4d", f);
//		ThrowMessage(hRoot, str);
//		
//		//Update Frame Count
//		pdmfh.frame = f;
//
//		raw = (UCHAR *) calloc(w*h, sizeof(UCHAR));
//
//		ReadRawImageUCHAR(w, h, raw, fps.filePath[f].lpstr);
//
//		//Fill PDMFRAME before transfer
//		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
//		pdmFrame.lpvoid = (VOID *) raw;
//
//		//Transfer PDMFRAME
//		SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");
//		//SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmfh, (LPARAM) tiff);
//
//		if(raw){free(raw);}
//		if(tiff){free(tiff);}
//
//	}		
//
//	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) nPaths-1);
//	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);
//
//	return TRUE;
//}


DWORD WINAPI OpenFileThreadPDM(LPVOID param)
{

	OPENFILETHREAD *OFT = (OPENFILETHREAD *) param;

	OPENFILETHREAD oft = *OFT;

	FILEPATHSET fps = {0};
	memcpy(&fps, oft.fps, sizeof(FILEPATHSET));

	CHAR str[MAX_STRING];

	HWND hRoot;
	HWND hText;
	HWND hwnd = oft.hOwner;
	INT nPaths = oft.fps->nPaths;

	INT f;
	UINT32 sizeScanLine = 0;
	UINT32 bytePerPixel = 0;

	INT fStart = NULL;
	INT fStop = NULL;

	UCHAR *raw = NULL;
	UCHAR *tiff = NULL;
	VOID *lpvoid = NULL;
	TIFF *tif = NULL;

	CHAR typeData;
	CHAR typeMetaData;
	
	long w = 1312;
	long h = 1080;
	long row = 0;

	w = GetPDMWidth(fps.filePath[0].lpstr);
	h = GetPDMHeight(fps.filePath[0].lpstr);

	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};
	pdmfh.width = w;
	pdmfh.height = h;
	pdmfh.typeMetaData = MD_FRAME;
	pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;


	//Sleep(500);
	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
	hRoot = (HWND)SendMessage(hwnd, WM_GETOWNERHWND, NULL, NULL);

	ReadPDMFrame(&typeData, &typeMetaData, &pdmFrame, (CHAR *)fps.filePath[0].lpstr, &lpvoid);
	if(lpvoid){free(lpvoid); lpvoid = NULL;}
	
	fStart = pdmFrame.pdmfh.frame;

	DataManager_Archive(hwnd, TRUE, NULL);

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) fStart);

	for(f = 0; f < nPaths; f++){
		
		//Update Frame Count

		ReadPDMFrame(&typeData, &typeMetaData, &pdmFrame, (CHAR *) fps.filePath[f].lpstr, &lpvoid);
		//pdmFrame.pdmfh.frame = f;
		//pdmfh.frame = f;

		//if(pdmFrame.pdmfh.width == w && pdmFrame.pdmfh.height == h){

		
			sprintf(str, "Frame %4d || %2d:%2d:%2d:%3d",f,
														pdmFrame.pdmfh.timeStamp.wHour, 
														pdmFrame.pdmfh.timeStamp.wMinute, 
														pdmFrame.pdmfh.timeStamp.wSecond, 
														pdmFrame.pdmfh.timeStamp.wMilliseconds);
			ThrowMessage(hRoot, str);

			if(pdmFrame.pdmfh.frame < fStart)
				fStart = pdmFrame.pdmfh.frame;
			if(pdmFrame.pdmfh.frame > fStop)
				fStop = pdmFrame.pdmfh.frame;

			//Fill PDMFRAME before transfer
			//memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
			if(lpvoid){
				pdmFrame.lpvoid = (VOID *) lpvoid/*raw*/;
			}else{
				pdmFrame.lpvoid = NULL;
			}

			//Send PDM Frame to DataManager
			//if(f == 0){
				SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) FS_RAW);
			//}

			if(lpvoid)	{free(lpvoid); lpvoid = NULL;}
			if(raw){free(raw);}
			if(tiff){free(tiff);}
		//}

	}		

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) fStart);
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) fStop);
	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);

	return TRUE;
}

DWORD WINAPI OpenFileThreadBMP(LPVOID param)
{
	OPENFILETHREAD *OFT = (OPENFILETHREAD *) param;

	OPENFILETHREAD oft = *OFT;

	FILEPATHSET fps = {0};
	memcpy(&fps, oft.fps, sizeof(FILEPATHSET));

	HWND hRoot = NULL;
	HWND hwnd = oft.hOwner;
	INT nPaths = oft.fps->nPaths;

	INT fStart;
	INT fStop;

	INT f;
	DWORD bytePerPixel = NULL;

	CHAR str[MAX_STRING];
	
	PIXEL *bmp = NULL;
	VOID *lpbmp = NULL;
	VOID *lpvoid = NULL;

	UINT w = NULL;
	UINT h = NULL;

	UINT wRead = NULL;
	UINT hRead = NULL;

	UCHAR dataType = NULL;

	bytePerPixel = GetBMPSize((CHAR *)&fps.filePath[0].lpstr, &w, &h);

	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};

	AllocPDMFrame(&pdmFrame, w, h, DT_PIXEL);
	//pdmFrame.pdmfh.width = w;
	//pdmFrame.pdmfh.height = h;
	//pdmFrame.pdmfh.typeMetaData = MD_FRAME;
	//pdmFrame.pdmfh.countMaps = 1;

	//if(bytePerPixel == 1){
	//	pdmfh.typeData = DT_UCHAR;
	//	dataType = DT_UCHAR;
	//}
	//else if(bytePerPixel == 2){
	//	pdmfh.typeData = DT_SHORT;
	//	dataType = DT_SHORT;
	//}
	//else if(bytePerPixel == 3){
		pdmFrame.pdmfh.typeData = DT_PIXEL;
		dataType = DT_PIXEL;
	//}
	//else
	//	return FALSE;

	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);
	DataManager_Archive(hwnd, TRUE, NULL);

	hRoot = (HWND)SendMessage(hwnd, WM_GETOWNERHWND, NULL, NULL);

	//pdmFrame.lpvoid = (VOID *) calloc(w*h, sizeof(PIXEL)/*bytePerPixel*/);
	//lpvoid = (VOID *)calloc(w*h, bytePerPixel);

	fStart = 0;
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) fStart);

	for(f = 0; f < nPaths; f++){

		ReadBMP((CHAR *)&fps.filePath[f], &wRead, &hRead, (VOID *)pdmFrame.lpvoid);
		//Color256toPIXEL(wRead, hRead, (UCHAR *)lpvoid, (PIXEL *)pdmFrame.lpvoid);
		//ReadBMP((CHAR *)&fps.filePath[f], &wRead, &hRead, (VOID *) pdmFrame.lpvoid);
		//ReadBMPOpenCV((CHAR *)&fps.filePath[f], &wRead, &hRead, (VOID *) pdmFrame.lpvoid);

		if(wRead == w && hRead == h){
			

			pdmFrame.pdmfh.frame = f;
			//memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
			
			SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) FS_RAW);
		}

	}

	fStop = nPaths-1;
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) fStop);
	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);

	return TRUE;
}

DWORD WINAPI StreamFileThread(LPVOID param)
{







	return TRUE;
}