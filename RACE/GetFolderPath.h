#include "stdafx.h"

DWORD GetFolderPath(HWND hOwner, CHAR *strFolderPath);
BOOL PathExists(CHAR *strDirectory);
DWORD CreateValidDirectory(CHAR *strPath);
DWORD GetNumDirectoryFiles(CHAR *strFolderPath, CHAR *strExt);
DWORD EnumerateDirectoryFilePaths(CHAR *strFolderPath, FILEPATHSETEX *fps, CHAR *strExt);
DWORD EnumerateDirectoryFileNames(CHAR *strFolderPath, FILEPATHSETEX *fps, CHAR *strExt);
