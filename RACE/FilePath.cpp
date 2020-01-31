#include "stdafx.h"

#include "FilePath.h"


DWORD AllocFilePathSet(FILEPATHSETEX *fps, DWORD nPath)
{

	DWORD i = NULL;
	if(fps->filePath){
		return FALSE;
	}

	fps->filePath = (FILEPATH *) malloc(nPath*sizeof(FILEPATH));

	if(!fps->filePath){
		fps->nPaths = NULL;
		return FALSE;
	}

	memset(fps->filePath, NULL, nPath*sizeof(FILEPATH));
	fps->nPaths = nPath;

	return nPath;
}

//Implements a bubble sort on the strings within 'fps'
DWORD SortFilePathSet(FILEPATHSETEX *fps, DWORD flgSortType)
{
	if(!fps)
		return FALSE;

	INT i = NULL;
	INT j = NULL;

	INT num = NULL;

	DWORD nString = fps->nPaths;
	CHAR strTemp[MAX_STRING]; 
	memset(strTemp, NULL, MAX_STRING);

	for(i = NULL; i < nString; i++){
		for(j = NULL; j < nString-1; j++){

			if(strcmp(fps->filePath[j].lpstr, fps->filePath[j+1].lpstr) > 0){
				strcpy(strTemp, fps->filePath[j].lpstr);
				strcpy(fps->filePath[j].lpstr, fps->filePath[j+1].lpstr);
				strcpy(fps->filePath[j+1].lpstr, strTemp);
				memset(strTemp, NULL, MAX_STRING*sizeof(CHAR));
			}
		}
	}


	return TRUE;
}

DWORD SortPDMFilePathSet(FILEPATHSETEX *fps, DWORD flgSortType)
{
	if(!fps)
		return FALSE;

	INT i = NULL;
	INT j = NULL;

	INT ni = NULL;
	INT nj = NULL;

	DWORD nString = fps->nPaths;
	CHAR strTemp[MAX_STRING]; 
	memset(strTemp, NULL, MAX_STRING);

	for(i = NULL; i < nString; i++){
		for(j = NULL; j < nString-1; j++){

			ni = GetPDMFrameNumber(fps->filePath[j].lpstr);
			nj = GetPDMFrameNumber(fps->filePath[i].lpstr);
			if(ni > nj){
			//if(strcmp(fps->filePath[j].lpstr, fps->filePath[j+1].lpstr) > 0){
				strcpy(strTemp, fps->filePath[j].lpstr);
				strcpy(fps->filePath[j].lpstr, fps->filePath[j+1].lpstr);
				strcpy(fps->filePath[j+1].lpstr, strTemp);
				memset(strTemp, NULL, MAX_STRING*sizeof(CHAR));
			}
		}
	}


	return TRUE;
}

DWORD GetPDMFrameNumber(CHAR *str)
{
	INT nStream;
	INT nFrame;

	CHAR strFile[MAX_STRING];

	sscanf(str, "%*s %d %d", &nStream, &nFrame);

	return nFrame;

}

DWORD Free(FILEPATHSETEX *fps)
{
	if(!fps)
		return FALSE;
	else
		free(fps->filePath);

	fps->nPaths = NULL;

	return TRUE;
}