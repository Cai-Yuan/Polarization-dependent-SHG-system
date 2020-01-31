#pragma once

#include "stdafx.h"

#define MEGABYTE 1000000

VOID ArchivePDMFRAME(CHAR *strPath, PDMFRAMEHEADER pdmfh, LPVOID lpvoid);
DWORD WINAPI ProcArchivePDMFRAME(LPVOID lpvoid);
VOID WritePDMFrame(CHAR typeData, CHAR typeMetaData, PDMFRAME pdmFrame, LPSTR lpsFilePath, LPVOID lpData);

WORD GenerateArchivePathPDM(INT iWnd, INT iFrame, CHAR *strDirectory, CHAR *strFilter, CHAR *strPath);
WORD GenerateArchivePathPDM(INT iWnd, INT iFrame, INT typeData, CHAR *strDirectory, CHAR *strPath);
WORD GenerateArchivePathPDM(INT iWnd, INT iFrame, INT typeData, CHAR *strDirectory, CHAR *strClass, CHAR *strPath);

LONG ReadPDMFrame(CHAR *typeData, CHAR *typeMetaData, PDMFRAME *pdmFrame, LPSTR lpsFilePath, VOID **lpvoid);
//LONG ReadPDMFrame(CHAR *typeData, CHAR *typeMetaData, PDMFRAME *pdmFrame, LPSTR lpsFilePath, LPVOID *lpvoid);

LONG ReadPDMWindow(CHAR *typeData, CHAR *typeMetaData, PDMWINDOW *pdmWindow, LPSTR lpsFilePath, VOID **lpvoid);