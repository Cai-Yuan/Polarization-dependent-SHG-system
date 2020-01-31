#include "stdafx.h"
#include "RAW.h"

VOID ReadRawImageUCHAR(UINT w, UINT h, UCHAR *imgUCHAR, LPSTR lpsFilePath)
{
	HANDLE hFile;
	DWORD nBytesRead = 0;
	if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		ReadFile(hFile, (LPVOID) imgUCHAR, w*h*sizeof(UCHAR), &nBytesRead, NULL);
		CloseHandle(hFile);
	}
}