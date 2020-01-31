#pragma once

#include "stdafx.h"

DWORD FileNameFromPath(UINT cMax, CHAR *strFilePath, CHAR *strFileName);
DWORD FilePathFromPath(UINT n, CHAR *strFilePath, CHAR *strPath);
DWORD FileNameSetFromFilePathSet(FILEPATHSET *fps, FILEPATHSET *fns);
DWORD DirectoryFromString(CHAR *str, CHAR *strDirectory);
DWORD DirectoryFromStringEx(CHAR *str, CHAR *strDirectory);
DWORD MakeValidPath(CHAR *str, CHAR *strPath);