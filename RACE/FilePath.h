#pragma once

#include "StringFxns.h"

#define MAX_STRING	256
#define MAX_PATHS	3000

//File Structures
typedef struct FILEPATH_{
	CHAR lpstr[MAX_STRING];
	UINT lenPath;
}FILEPATH;


typedef struct FILEPATHSET2_{
	DWORD size;
	DWORD nPath;
	FILEPATH *fp;
}FILEPATHSET2;

typedef struct FILEPATHSET_{
	FILEPATH filePath[MAX_PATHS];
	UINT nPaths;
}FILEPATHSET;

typedef struct FILEPATHSETEX_{
	FILEPATH *filePath;
	UINT nPaths;
}FILEPATHSETEX;

DWORD Free(FILEPATHSETEX *fps);
DWORD AllocFilePathSet(FILEPATHSETEX *fps, DWORD nPath);
DWORD SortFilePathSet(FILEPATHSETEX *fps, DWORD flgSortType);

DWORD SortPDMFilePathSet(FILEPATHSETEX *fps, DWORD flgSortType);
DWORD GetPDMFrameNumber(CHAR *str);

//typedef struct OPENFILETHREAD_{
//	HWND hChild;
//	UINT typeFile;
//	FILEPATHSET *fps;
//}OPENFILETHREAD;

