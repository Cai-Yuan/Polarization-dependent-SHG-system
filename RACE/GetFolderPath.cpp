#include "stdafx.h"
#include "Shlobj.h"
#include "GetFolderPath.h"
#include "StringFxns.h"
#include "FilePath.h"

//SHGetPathFromIDList()

//Returns 'TRUE' if folder path selected, OTHERWISE 'FALSE'
//
DWORD GetFolderPath(HWND hOwner, CHAR *strFolderPath)
{
	DWORD dw = TRUE;

	BROWSEINFO bi = {0};
	PIDLIST_ABSOLUTE pidl = {0};
	bi.ulFlags = BIF_USENEWUI|BIF_NEWDIALOGSTYLE;
	//bi.pidlRoot = (PCIDLIST_ABSOLUTE) "C:\Archive";

	

	pidl = SHBrowseForFolder(&bi);

	if(!pidl) //User canceled operaiton
		return NULL;

	SHGetPathFromIDList(pidl, strFolderPath);

	if(!strFolderPath[0]){
		return FALSE;
	}
	
	//Append 
	sprintf(strFolderPath, "%s%s", strFolderPath, "\\");

	return dw;

}

BOOL PathExists(CHAR *strDirectory)
{

	DWORD dw = NULL;

	dw = GetFileAttributes(strDirectory);

	if(dw != INVALID_FILE_ATTRIBUTES)
		return TRUE;
	else
		return FALSE;

}

DWORD CreateValidDirectory(CHAR *strPath)
{

	DWORD strLen = strlen(strPath);
	if(strPath[strLen-1] != '\\'){
		strPath[strLen] = '\\';
	}
	return (DWORD) SHCreateDirectoryEx(NULL, strPath, NULL);


}

DWORD GetNumDirectoryFiles(CHAR *strFolderPath, CHAR *strExt)
{
	DWORD nFiles = NULL;
	HANDLE hFind = NULL;

	WIN32_FIND_DATA fd;
	memset(&fd, NULL, sizeof(WIN32_FIND_DATA));

	CHAR strDirectory[MAX_STRING];
	memset(strDirectory, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strExtension[MAX_STRING];
	memset(strExtension, NULL, MAX_STRING*sizeof(CHAR));

	sprintf(strDirectory, "%s%s", strFolderPath, "\\*");

	hFind = FindFirstFile(strDirectory, &fd);

	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	while(FindNextFile(hFind, &fd) != NULL){
	
		ReadStringWithin(fd.cFileName, strExtension, '.', NULL);
		if(!strcmp(strExtension, strExt)){
			nFiles++;
		}

		nFiles = nFiles;

	}

	FindClose(hFind);

	return nFiles;
}

DWORD EnumerateDirectoryFilePaths(CHAR *strFolderPath, FILEPATHSETEX *fps, CHAR *strExt)
{
	DWORD i = NULL;
	DWORD nFiles = NULL;
	HANDLE hFind = NULL;

	CHAR strExtension[MAX_STRING];
	memset(strExtension, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strDirectory[MAX_STRING];
	memset(strDirectory, NULL, MAX_STRING*sizeof(CHAR));

	if(!fps){}

	WIN32_FIND_DATA fd;
	memset(&fd, NULL, sizeof(WIN32_FIND_DATA));

	nFiles = GetNumDirectoryFiles(strFolderPath, strExt);

	if(!nFiles)
		return FALSE;

	sprintf(strDirectory, "%s%s", strFolderPath, "\\*");

	hFind = FindFirstFile(strDirectory, &fd);

	if(hFind == INVALID_HANDLE_VALUE){

		return FALSE;
	}

	AllocFilePathSet(fps, nFiles);

	while(FindNextFile(hFind, &fd) != NULL){
	
		ReadStringWithin(fd.cFileName, strExtension, '.', NULL);

		if(!strcmp(strExtension, strExt)){
			sprintf(fps->filePath[i].lpstr, "%s\%s", strFolderPath, fd.cFileName);
			i++;
		}

	}

	FindClose(hFind);

	return nFiles;
}

DWORD EnumerateDirectoryFileNames(CHAR *strFolderPath, FILEPATHSETEX *fps, CHAR *strExt)
{
	DWORD i = NULL;
	DWORD nFiles = NULL;
	HANDLE hFind = NULL;

	CHAR strExtension[MAX_STRING];
	memset(strExtension, NULL, MAX_STRING*sizeof(CHAR));

	CHAR strDirectory[MAX_STRING];
	memset(strDirectory, NULL, MAX_STRING*sizeof(CHAR));

	if(!fps){

	}

	WIN32_FIND_DATA fd;
	memset(&fd, NULL, sizeof(WIN32_FIND_DATA));

	nFiles = GetNumDirectoryFiles(strFolderPath, strExt);

	if(!nFiles)
		return FALSE;

	sprintf(strDirectory, "%s%s", strFolderPath, "\\*");

	hFind = FindFirstFile(strDirectory, &fd);

	if(hFind == INVALID_HANDLE_VALUE){

		return FALSE;
	}

	AllocFilePathSet(fps, nFiles);

	while(FindNextFile(hFind, &fd) != NULL){
	
		ReadStringWithin(fd.cFileName, strExtension, '.', NULL);
		if(!strcmp(strExtension, strExt)){
			sprintf(fps->filePath[i].lpstr, "%s", fd.cFileName);
			i++;
		}

	}

	FindClose(hFind);

	return nFiles;
}