#pragma once

#include "stdafx.h"

DWORD GetAlgorithmParams(CHAR *strIn, CHAR *strRoot, FLOAT *paramList);
DWORD GetAlgorithmInputs(CHAR *strIn, DWORD nChar, FLOAT *inputList);

DWORD GetAlgorithmProcedures(HWND hwnd, HTREEITEM hTreeItem, DWORD *selProc, STRINGSET *ss);
DWORD SortAlgorithmProcedures(DWORD activeProc, STRINGSET *ss);