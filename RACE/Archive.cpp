#include "stdafx.h"
#include "Archive.h"

VOID ArchivePDMFRAME(CHAR *strPath, PDMFRAMEHEADER pdmfh, LPVOID lpvoid)
{
	HANDLE hThread;
	PDMFRAME pdmFrame;
	PDMARCHIVEFRAME *pdmArchiveFrame = (PDMARCHIVEFRAME *) calloc(1, sizeof(PDMARCHIVEFRAME));
	//pdmArchiveFrame->strPath = (CHAR *)calloc(MAX_STRING, sizeof(CHAR));

	if(!pdmArchiveFrame){
		MessageBox(NULL, "Error!", "Could Not Archive PDM Frame", MB_OK);
		return;
	}

	memcpy(&pdmArchiveFrame->pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));

	UCHAR *buf = (UCHAR *) calloc(pdmfh.width*pdmfh.height, sizeof(UCHAR));
	memcpy(buf, lpvoid, pdmfh.width*pdmfh.height*sizeof(UCHAR));
	pdmArchiveFrame->pdmFrame.lpvoid = buf;
	sprintf((CHAR *)&pdmArchiveFrame->strPath, "%s", strPath);

	hThread = CreateThread(NULL, (SIZE_T)MEGABYTE, ProcArchivePDMFRAME, pdmArchiveFrame, NULL, NULL);

}

DWORD WINAPI ProcArchivePDMFRAME(LPVOID lpvoid)
{
	PDMARCHIVEFRAME *pdmArchiveFrame = (PDMARCHIVEFRAME *) lpvoid;

	CHAR str[MAX_STRING];
	CHAR lpsFilePath[MAX_STRING];
	LONG iWnd = pdmArchiveFrame->iWin;
	LONG iFrame = pdmArchiveFrame->iFrame; 
	UCHAR typeData = pdmArchiveFrame->pdmFrame.pdmfh.typeData;
	UCHAR typeMetaData = pdmArchiveFrame->pdmFrame.pdmfh.typeMetaData;

	WritePDMFrame(typeData, typeMetaData, pdmArchiveFrame->pdmFrame, (CHAR *)pdmArchiveFrame->strPath, pdmArchiveFrame->pdmFrame.lpvoid);

	//if(pdmArchiveFrame->pdmFrame.lpvoid)	free(pdmArchiveFrame->pdmFrame.lpvoid);
	//if(pdmArchiveFrame)	free(pdmArchiveFrame);

	return NULL;
}

VOID WritePDMFrame(CHAR typeData, CHAR typeMetaData, PDMFRAME pdmFrame, LPSTR lpsFilePath, LPVOID lpData)
{
	CHAR flag = NULL;
	CHAR idPDM[]={'P', '7'};
	HANDLE hFile;
	DWORD nBytesWritten = 0;
	LONG w = pdmFrame.pdmfh.width;
	LONG h = pdmFrame.pdmfh.height;

	//Calculate Number of Bytes Needed For Data
	LONG nBytesData = w*h*GetDataTypeSize(typeData);

	//Calculate Number of Bytes Needed for Meta-Data
	LONG nBytesMetaData = sizeof(PDMFRAMEHEADER);

	if(nBytesData && lpData){

		if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

			//Append 'P7' to File (First Two Bytes of File)
			if(!WriteFile(hFile, &idPDM, sizeof(idPDM), &nBytesWritten, NULL))
				flag = TRUE;

			//Append Meta-Data Size to File
			if(!WriteFile(hFile, &nBytesMetaData, sizeof(nBytesMetaData), &nBytesWritten, NULL))
				flag = TRUE;

			//Append Meta-Data to File
			if(!WriteFile(hFile, &pdmFrame.pdmfh, sizeof(PDMFRAMEHEADER), &nBytesWritten, NULL))
				flag = TRUE;

			//Append Data to File
			if(!WriteFile(hFile, pdmFrame.lpvoid, nBytesData, &nBytesWritten, NULL))
				flag = TRUE;

			if(flag)
				MessageBox(NULL, "Error Archiving Data", "Error!", MB_OK);

			CloseHandle(hFile);
		}
	}

}

WORD GenerateArchivePathPDM(INT iWnd, INT iFrame, CHAR *strDirectory, CHAR *strFilter, CHAR *strPath)
{

	if(!strDirectory || !strPath)
		return NULL;

	sprintf(strPath, "%s%d_%06d_%s.pdm", strDirectory, iWnd, iFrame, strFilter);

	return TRUE;

}

//Obselete
WORD GenerateArchivePathPDM(INT iWnd, INT iFrame, INT typeData, CHAR *strDirectory, CHAR *strPath)
{

	if(!strDirectory || !strPath)
		return NULL;

	switch(typeData)
	{
	case DT_CHAR:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_CHAR);
		break;
	case DT_UCHAR:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_UCHAR);
		break;
	case DT_INT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_INT);
		break;
	case DT_UINT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_UINT);
		break;
	case DT_FLOAT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_FLOAT);
		break;
	case DT_PIXEL:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_PIXEL);
		break;
	case DT_POINT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_POINT);
		break;
	case DT_CIRCLE:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_CIRCLE);
		break;
	case DT_ELLIPSE:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_ELLIPSE);
		break;
	}


	return TRUE;

}
//Obselete
WORD GenerateArchivePathPDM(INT iWnd, INT iFrame, INT typeData, CHAR *strDirectory, CHAR *strClass, CHAR *strPath)
{

	if(!strDirectory || !strPath)
		return NULL;


	switch(typeData)
	{
	case DT_CHAR:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_CHAR, strClass);
		break;
	case DT_UCHAR:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_UCHAR, strClass);
		break;
	case DT_INT:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_INT, strClass);
		break;
	case DT_UINT:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_UINT, strClass);
		break;
	case DT_FLOAT:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_FLOAT, strClass);
		break;
	case DT_PIXEL:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_PIXEL, strClass);
		break;
	case DT_POINT:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_POINT, strClass);
		break;
	case DT_CIRCLE:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_CIRCLE, strClass);
		break;
	case DT_ELLIPSE:
		sprintf(strPath, "%s%d_%d_%s_%s.pdm", strDirectory, iWnd, iFrame, STR_ELLIPSE, strClass);
		break;
	}


	return TRUE;

}


LONG ReadPDMFrame(CHAR *typeData, CHAR *typeMetaData, PDMFRAME *pdmFrame, LPSTR lpsFilePath, VOID **lpvoid)
{
	CHAR id[2];
	CHAR idPDM[]={'P', '7'};
	HANDLE hFile = NULL;
	LONG sizeData = NULL;
	LONG sizeMetaData = NULL;
	LONG nBytesData = NULL;

	LONG sMetaData = sizeof(PDMFRAMEHEADER);
	DWORD nBytesRead = 0;

	LONG nBytesMetaData = sizeof(PDMFRAMEHEADER);

	PDMFRAMEHEADER pdmfh;

	if(*lpvoid)
		return FALSE;
	//Ensure lpvoid is free
	//if(*lpvoid){
	//	free(*lpvoid);
	//}


	memset(id, 0x00, sizeof(id));
	if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		
		//Get Magic Number 'P7'+'/0'
		ReadFile(hFile, &id, 2*sizeof(CHAR), &nBytesRead, NULL);
		
		//Get Size of Meta-Data
		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);

		if(sMetaData == sizeMetaData){

			//Read Meta-Data
			ReadFile(hFile, &pdmfh, sizeMetaData, &nBytesRead, NULL);

			memcpy(&pdmFrame->pdmfh, &pdmfh, sizeMetaData);
			//Check Meta-Data Type
			//(MD_FRAME)
			if(pdmfh.typeMetaData == MD_FRAME){

				sizeData = GetDataTypeSize(pdmfh.typeData);

				//TEMP
				sizeData = pdmfh.width*pdmfh.height*GetDataTypeSize(pdmfh.typeData);
				pdmFrame->pdmfh.sizeData = sizeData;
				//*lpvoid = (VOID *) calloc(sizeData, sizeof(UCHAR));
				*lpvoid = (VOID *) malloc(sizeData*sizeof(UCHAR));

				if(*lpvoid){
					nBytesData =  sizeData;
					ReadFile(hFile, *lpvoid, nBytesData, &nBytesRead, NULL);
				}
				else
					return FALSE;
			}
			else
				return FALSE;
		}
		else
			return FALSE;


		CloseHandle(hFile);
		return TRUE;
	}
	else
		return FALSE;


}

LONG ReadPDMWindow(CHAR *typeData, CHAR *typeMetaData, PDMWINDOW *pdmWindow, LPSTR lpsFilePath, VOID **lpvoid)
{
	CHAR id[2];
	CHAR idPDM[]={'P', '7'};
	HANDLE hFile = NULL;
	LONG sizeData = NULL;
	LONG sizeMetaData = NULL;
	LONG nBytesData = NULL;

	LONG sMetaData = sizeof(PDMWINDOWHEADER);
	DWORD nBytesRead = 0;

	LONG nBytesMetaData = sizeof(PDMWINDOWHEADER);

	PDMWINDOWHEADER pdmWindowHeader;


	memset(id, 0x00, sizeof(id));
	if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		
		//Get Magic Number 'P7'+'/0'
		ReadFile(hFile, &id, 2*sizeof(CHAR), &nBytesRead, NULL);
		
		//Get Size of Meta-Data
		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);

		if(sMetaData == sizeMetaData){

			//Read Meta-Data
			ReadFile(hFile, &pdmWindowHeader, sizeMetaData, &nBytesRead, NULL);

			memcpy(&pdmWindow->pdmWindowHeader, &pdmWindowHeader, sizeMetaData);
			//Check Meta-Data Type
			//(MD_FRAME)
			if(pdmWindowHeader.typeMetaData == MD_WINDOW){

				//sizeData = GetDataTypeSize(pdmWindowHeader.typeData);

				//TEMP
				//sizeData = pdmWindowHeader.width*pdmfh.height;
				// = sizeData;
				sizeData = pdmWindow->pdmWindowHeader.sizeData;
				*lpvoid = (VOID *) calloc(sizeData, sizeof(PDMWINDOWDATA));

				if(*lpvoid){
					nBytesData =  sizeData;
					ReadFile(hFile, *lpvoid, nBytesData, &nBytesRead, NULL);
				}
				else{
					CloseHandle(hFile);
					return FALSE;
				}
			}
			else{
				CloseHandle(hFile);
				return FALSE;
			}
		}
		else{
			CloseHandle(hFile);
			return FALSE;
		}


		CloseHandle(hFile);
		return TRUE;
	}
	else
		return FALSE;


}

//VOID ArchivePDMFRAME(CHAR *strPath, PDMFRAMEHEADER pdmfh, LPVOID lpvoid)
//{
//	HANDLE hThread;
//	PDMFRAME pdmFrame;
//	PDMARCHIVEFRAME *pdmArchiveFrame = (PDMARCHIVEFRAME *) calloc(1, sizeof(PDMARCHIVEFRAME));
//	//pdmArchiveFrame->strPath = (CHAR *)calloc(MAX_STRING, sizeof(CHAR));
//
//	if(!pdmArchiveFrame){
//		MessageBox(NULL, "Error!", "Could Not Archive PDM Frame", MB_OK);
//		return;
//	}
//
//	memcpy(&pdmArchiveFrame->pdmFrame.pdmfh, &pdmfh, sizeof(PDMFRAMEHEADER));
//
//	UCHAR *buf = (UCHAR *) calloc(pdmfh.width*pdmfh.height, sizeof(UCHAR));
//	memcpy(buf, lpvoid, pdmfh.width*pdmfh.height*sizeof(UCHAR));
//	pdmArchiveFrame->pdmFrame.lpvoid = buf;
//	sprintf((CHAR *)&pdmArchiveFrame->strPath, "%s", strPath);
//
//	hThread = CreateThread(NULL, (SIZE_T)MEGABYTE, ProcArchivePDMFRAME, pdmArchiveFrame, NULL, NULL);
//
//}
//
//DWORD WINAPI ProcArchivePDMFRAME(LPVOID lpvoid)
//{
//	PDMARCHIVEFRAME *pdmArchiveFrame = (PDMARCHIVEFRAME *) lpvoid;
//
//	CHAR str[MAX_STRING];
//	CHAR lpsFilePath[MAX_STRING];
//	LONG iWnd = pdmArchiveFrame->iWin;
//	LONG iFrame = pdmArchiveFrame->iFrame; 
//	UCHAR typeData = pdmArchiveFrame->pdmFrame.pdmfh.typeData;
//	UCHAR typeMetaData = pdmArchiveFrame->pdmFrame.pdmfh.typeMetaData;
//
//	WritePDMFrame(typeData, typeMetaData, pdmArchiveFrame->pdmFrame, (CHAR *)pdmArchiveFrame->strPath, pdmArchiveFrame->pdmFrame.lpvoid);
//
//	//if(pdmArchiveFrame->pdmFrame.lpvoid)	free(pdmArchiveFrame->pdmFrame.lpvoid);
//	//if(pdmArchiveFrame)	free(pdmArchiveFrame);
//
//	return NULL;
//}

//LONG ReadPDMFrame(CHAR *typeData, CHAR *typeMetaData, PDMFRAME *pdmFrame, LPSTR lpsFilePath, LPVOID *lpvoid)
//{
//	CHAR id[2];
//	CHAR idPDM[]={'P', '7'};
//	HANDLE hFile;
//	LONG sizeData;
//	LONG sizeMetaData;
//
//	LONG sMetaData = sizeof(PDMFRAMEHEADER);
//	DWORD nBytesRead = 0;
//
//	LONG nBytesMetaData = sizeof(PDMFRAMEHEADER);
//
//	PDMFRAMEHEADER pdmfh;
//
//	//Ensure lpvoid is free
//	if(*lpvoid){
//		free(*lpvoid);
//	}
//
//
//	memset(id, 0x00, sizeof(id));
//	if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
//		
//		//Get Magic Number 'P7'+'/0'
//		ReadFile(hFile, &id, 2*sizeof(CHAR), &nBytesRead, NULL);
//		
//		//Get Size of Meta-Data
//		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);
//
//		if(sMetaData == sizeMetaData){
//			//Read Meta-Data
//			ReadFile(hFile, &pdmfh, sizeMetaData, &nBytesRead, NULL);
//
//			//Check Meta-Data Type
//			//(MD_FRAME)
//			if(pdmfh.typeMetaData == MD_FRAME){
//
//				sizeData = GetDataTypeSize(pdmfh.typeData);
//
//				//TEMP
//				//sizeData = W_RAW*H_RAW*sizeof(UCHAR);
//				pdmFrame->pdmfh.sizeData = sizeData*pdmfh.width*pdmfh.height;
//				*lpvoid = calloc(pdmfh.width*pdmfh.height, sizeData);
//				/**lpvoid = calloc(pdmfh.width*pdmfh.height, sizeof(UCHAR));*/
//				if(*lpvoid){
//					LONG nBytesData =  pdmfh.width*pdmfh.height*sizeData/*sizeof(UCHAR)*/;
//					ReadFile(hFile, *lpvoid, /*sizeData*/nBytesData, &nBytesRead, NULL);
//				}
//				else
//					return FALSE;
//			}
//			else
//				return FALSE;
//		}
//		else
//			return FALSE;
//
//
//		CloseHandle(hFile);
//		return TRUE;
//	}
//	else
//		return FALSE;
//
//
//}