#include "stdafx.h"

DWORD WINAPI OpenFileThreadTIFF(LPVOID param)
{
#ifdef _WIN64
	return FALSE;

#endif
#ifndef _WIN64
	OPENFILETHREAD *OFT = (OPENFILETHREAD *) param;

	OPENFILETHREAD oft = *OFT;

	FILEPATHSET fps = {0};
	memcpy(&fps, oft.fps, sizeof(FILEPATHSET));

	HWND hwnd = oft.hOwner;
	INT nPaths = oft.fps->nPaths;

	INT f;
	UINT32 sizeScanLine = 0;
	UINT32 bytePerPixel = 0;


	UCHAR *buf = NULL;
	UCHAR *tiff = NULL;
	VOID *lpvoid = NULL;
	TIFF *tif = NULL;

	
	long w = 0;
	long h = 0;
	long row = 0;

	w = ReadTIFFWidth(fps.filePath[0].lpstr);
	h = ReadTIFFHeight(fps.filePath[0].lpstr);

	//w = ReadTIFFWidth(
	PDMFRAME pdmFrame = {0};
	PDMFRAMEHEADER pdmfh = {0};
	pdmfh.width = w;
	pdmfh.height = h;
	pdmfh.typeMetaData = MD_FRAME;
	pdmfh.typeData = DT_UCHAR;
	//pdmfh.timeStamp = (LONG) clock();
	//pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;

	//Sleep(500);
	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	for(f = 0; f < nPaths; f++){
		
		//Update Frame Count
		pdmfh.frame = f;

		//Open TIFF File
		tif = TIFFOpen(fps.filePath[f].lpstr, "r");

		//Read TIFF File
		if(tif){

			sizeScanLine = TIFFScanlineSize(tif);
			bytePerPixel = sizeScanLine/w;

			buf = (UCHAR *) calloc(w*h*bytePerPixel, sizeof(UCHAR));
			tiff = (UCHAR *) calloc(w*h, sizeof(UCHAR));

			if(tiff && buf){
				if(bytePerPixel == 1){
					for(row = 0; row < h; row++){

						TIFFReadScanline(tif, tiff + (row*w), row);

					}
				}
				else if(bytePerPixel == 2){
				
					for(row = 0; row < h; row++){

						TIFFReadScanline(tif, buf + (row*w*bytePerPixel), row);

					}

					CompressTIFF2(w, h, buf, tiff);
				}
				else if(bytePerPixel == 3){

					for(row = 0; row < h; row++){

						TIFFReadScanline(tif, buf + (row*w*bytePerPixel), row);

					}

					CompressTIFF3(w, h, buf, tiff);
				}
				else
					ThrowMessage(hwnd, "TIFF BytesPerPixel Unrecognized");
			}

			//Fill PDMFRAME before transfer
			memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
			pdmFrame.lpvoid = (VOID *) tiff;

			//Transfer PDMFRAME
			SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");
			//SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmfh, (LPARAM) tiff);

			if(buf){free(buf);}
			if(tiff){free(tiff);}
		}

	}		

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) nPaths-1);
	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);



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
	pdmfh.typeData = DT_UCHAR;
	pdmfh.countMaps = 1;

	//Sleep(500);
	SendMessage(hwnd, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTARTFRAME), (LPARAM) 0);

	hRoot = (HWND)SendMessage(hwnd, WM_GETOWNERHWND, NULL, NULL);

	for(f = 0; f < nPaths; f++){

		
		sprintf(str, "Frame %4d", f);
		ThrowMessage(hRoot, str);
		
		//Update Frame Count
		pdmfh.frame = f;

		raw = (UCHAR *) calloc(w*h, sizeof(UCHAR));

		ReadRawImageUCHAR(w, h, raw, fps.filePath[f].lpstr);

		//Fill PDMFRAME before transfer
		memcpy(&pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
		pdmFrame.lpvoid = (VOID *) raw;

		//Transfer PDMFRAME
		SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmFrame, (LPARAM) "RAW");
		//SendMessage(hwnd, WM_DATAIN, (WPARAM) &pdmfh, (LPARAM) tiff);

		if(raw){free(raw);}
		if(tiff){free(tiff);}

	}		

	SendMessage(hwnd, WM_COMMAND, LOWORD(DVN_SETSTOPFRAME), (LPARAM) nPaths-1);
	SendMessage(hwnd, WM_ENDTRANSMISSION, NULL, NULL);

	return TRUE;
}