#pragma once

#include "stdafx.h"

#include "FilePath.h"
#include "CommDlg.h"

//Supported File Extension
#define EXT_TIFF	1
#define EXT_BMP		2
#define EXT_PDM		4
#define EXT_RAW		8
#define EXT_PGM		16
#define EXT_ALL		255

//Supported File Filter Order
#define TIF		2
#define PDM		1
#define BMP		3
#define RAW		4
#define PGM		5

//File Extension Identifiers
#define STR_TIFF	"TIFF(*.tif)\0*.tif\0"
#define STR_BMP		"BMP(*.bmp)\0*.bmp\0"
#define STR_PDM		"PDM(*.pdm)\0*.pdm\0"
#define STR_RAW		"RAW(*.raw)\0*.raw\0"
#define STR_PGM		"PGM(*.pgm)\0*.pgm\0"

typedef struct OPENFILETHREAD_{
	HWND hOwner;
	UINT typeFile;
	FILEPATHSET *fps;
	STRINGSET *ss;
}OPENFILETHREAD;

typedef struct ARCHIVEFILETHREAD_{
	HWND hOwner;
	UINT iOwner;
	UINT typeFile;
	FILEPATHSET *fps;
}ARCHIVEFILETHREAD;

DWORD WINAPI OpenFileThreadTIFF(LPVOID param);
DWORD WINAPI OpenFileThreadRAW(LPVOID param);
DWORD WINAPI OpenFileThreadPDM(LPVOID param);
DWORD WINAPI OpenFileThreadBMP(LPVOID param);

UINT GetOpenFilePaths(HWND hParent, UINT filter, FILEPATHSET *fps);
UINT GetFilePaths(OPENFILENAME ofn, FILEPATHSET *filePathSet);
HANDLE CreateOpenFileThread(HWND hOwner, UINT typeFile, FILEPATHSET *fps);
HANDLE CreateOpenFileThread(HWND hOwner, UINT typeFile, STRINGSET *ss);
UINT GetOpenFileProfile(HWND hParent, FILEPATHSET *fps);

UINT GetFilePaths(OPENFILENAME ofn, STRINGSET *ss);
DWORD GetOpenFilePaths(HWND hParent, UINT filter, STRINGSET *ss);
//INT OpenFile(UINT *w, UINT *h, UINT *uType, LPVOID **lpvoid);
//
//UINT GetOpenFilePaths(HWND hParent, UINT filter, FILEPATHSET *fps);
//
//VOID GetExtension(UINT ext, CHAR *str);
//VOID GetFilterString(UINT filter, UINT nBytesString, CHAR *str);
//
//
//
//
//HANDLE CreateOpenFileThread(HWND hChild, UINT typeFile, FILEPATHSET *fps);
//
////One Thread for Each Supported Type
//DWORD WINAPI OpenFileThreadPDM(LPVOID param);
//DWORD WINAPI OpenFileThreadRAW(LPVOID param);
//DWORD WINAPI OpenFileThreadTIFF(LPVOID param);