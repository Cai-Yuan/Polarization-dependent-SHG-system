#include "stdafx.h"

DWORD ThrowError(HWND hOwner, CHAR *strError);
DWORD ThrowLastError(HWND hOwner);
DWORD ThrowMessage(HWND hOwner, CHAR *strError);
DWORD ThrowLastError(HWND hOwner, CHAR *strPefix);
DWORD ThrowMessage(HWND hOwner, CHAR *strMessage);

//DWORD ThrowMessage(HWND hOwner, CHAR *strFormat, ... );