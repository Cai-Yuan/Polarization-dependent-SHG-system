#include "stdafx.h"

#include "String.h"


DWORD FileNameFromPath(UINT cMax, CHAR *strFilePath, CHAR *strFileName)
{
	UINT i = 0;
	UINT strLength = 0;
	CHAR strTemp[MAX_STRING];

	//Zero strTemp
	memset(strTemp, NULL, MAX_STRING*sizeof(CHAR));

	//Does not include the null terminating char
	strLength = strlen(strFilePath);

	for(i = strLength; i > 0; i--)
	{
		if(strFilePath[i] == 92){
			i++;
			break;
		}
	}

	strcpy(strFileName, (strFilePath + i));
	


	return TRUE;
}

DWORD FilePathFromPath(UINT n, CHAR *strFilePath, CHAR *strPath)
{

	DWORD i = NULL;
	DWORD iStop = NULL;
	CHAR strTemp[MAX_STRING];

	if(!strFilePath || !strPath)
		return FALSE;

	DWORD strLen = strlen(strFilePath);
	
	
	memset(strTemp, NULL, MAX_STRING*sizeof(CHAR));

	for(i = strLen; i > 0; i--)
	{

		if(strFilePath[i] == 92){
			iStop = i+1;
			break;
		}
	}

	memcpy(strPath, strFilePath, iStop*sizeof(CHAR));

	return strlen(strPath);

}
	

DWORD FileNameSetFromFilePathSet(FILEPATHSET *fps, FILEPATHSET *fns)
{

	if(!fps || !fns){
		return TRUE;

	}

	DWORD i = NULL;
	DWORD nConvert = fps->nPaths;

	for(i = 0; i < nConvert; i++){

		FileNameFromPath(fps->filePath[i].lenPath, fps->filePath[i].lpstr, fns->filePath[i].lpstr);
		fns->filePath[i].lenPath = strlen(fns->filePath[i].lpstr);

	}

	fns->nPaths = i;

	return i;
}

DWORD DirectoryFromString(CHAR *str, CHAR *strDirectory)
{
	
	UINT i = 0;
	UINT strLength = 0;
	CHAR strTemp[MAX_STRING];

	//Zero strTemp
	memset(strTemp, NULL, MAX_STRING*sizeof(CHAR));

	//Does not include the null terminating char
	strLength = strlen(str);

	for(i = strLength; i > 0; i--)
	{
		if(str[i] == 92){
			i++;
			break;
		}
	}

	strcpy(strDirectory, (str + i));




	return TRUE;
}

//Updated Version: Looks for '\' as the last character in a string and skips it
DWORD DirectoryFromStringEx(CHAR *str, CHAR *strDirectory)
{
	if(!str || !strDirectory)
		return FALSE;
	
	UINT i = 0;
	UINT strLength = 0;
	CHAR strTemp[MAX_STRING];

	//Zero strTemp
	memset(strTemp, NULL, MAX_STRING*sizeof(CHAR));

	//Does not include the null terminating char
	strLength = strlen(str);

	if(str[strLength-1] == 92)
		strLength-=2;

	for(i = strLength; i > 0; i--)
	{
		if(str[i] == 92){
			i++;
			break;
		}
	}

	strcpy(strDirectory, (str + i));

	return TRUE;
}

DWORD MakeValidPath(CHAR *str, CHAR *strPath)
{
	if(!str || !strPath)
		return FALSE;

	UINT i = 0;
	UINT strLength = strlen(str);

	strcpy(strPath, str);

	if(strPath[strLength-1] == 92){
		return TRUE;
	}else{
		strPath[strLength] = 92;
		strPath[strLength+1] =  0;
		return TRUE;
	}

}