#include "stdafx.h"

#include "ThrowError.h"

DWORD ThrowError(HWND hOwner, CHAR *strError)
{
	HWND hRoot = NULL;

	hRoot = GetAncestor(hOwner, GA_ROOT);
	SendMessage(hRoot, WM_THROWERROR, (WPARAM) strError, (LPARAM) NULL);

	return TRUE;

}

DWORD ThrowMessage(HWND hOwner, CHAR *strMessage)
{
	HWND hRoot = NULL;

	hRoot = GetAncestor(hOwner, GA_ROOT);
	SendMessage(hRoot, WM_THROWERROR, (WPARAM) strMessage, (LPARAM) NULL);

	return TRUE;

}

DWORD ThrowLastError(HWND hOwner)
{
	HWND hRoot;
	DWORD lastError;
	CHAR strError[MAX_STRING];
	CHAR *strErrorSystem = NULL;

	memset(strError, NULL, MAX_STRING*sizeof(CHAR));
	hRoot = GetAncestor(hOwner, GA_ROOT);

	//Get System Error Value
	lastError = GetLastError();
	

	//Get System Error String
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL, 
					lastError, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					(CHAR *) &strErrorSystem, 
					MAX_STRING, NULL); 

	sprintf(strError, "Last Error Code: %d -- %s", lastError, strErrorSystem);
	SendMessage(hRoot, WM_THROWERROR, (WPARAM) strError, (LPARAM) NULL);

	//Use 'LocalFree' because FormatMessage uses 'LocalAlloc'
	if(strErrorSystem)
		LocalFree(strErrorSystem);

	return TRUE;

}

DWORD ThrowLastError(HWND hOwner, CHAR *strPefix)
{
	HWND hRoot;
	DWORD lastError;
	CHAR strError[MAX_STRING];
	CHAR *strErrorSystem = NULL;

	memset(strError, NULL, MAX_STRING*sizeof(CHAR));
	hRoot = GetAncestor(hOwner, GA_ROOT);

	//Get System Error Value
	lastError = GetLastError();
	
	//Get System Error String
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL, 
					lastError, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					(CHAR *) &strErrorSystem, 
					MAX_STRING, NULL); 

	sprintf(strError, "Last Error Code: %d -- %s", lastError, strErrorSystem);
	SendMessage(hRoot, WM_THROWERROR, (WPARAM) strError, (LPARAM) NULL);

	if(strErrorSystem)
		LocalFree(strErrorSystem);

	return TRUE;

}


//DWORD ThrowMessage(HWND hOwner, CHAR *strError)
//{
//	HWND hRoot;
//
//	hRoot = GetAncestor(hOwner, GA_ROOT);
//	//PostMessage(hRoot, WM_THROWERROR, (WPARAM) strError, (LPARAM) NULL);
//	SendMessage(hRoot, WM_THROWERROR, (WPARAM) strError, (LPARAM) NULL);
//
//	return TRUE;
//
//
//}

//Variable Argument Function call
//Reference: http://www.cprogramming.com/tutorial/c/lesson17.html

DWORD ThrowMessage(HWND hOwner, CHAR *strFormat, ... )
{

	HWND hRoot;
	CHAR str[MAX_STRING];

	//sprintf(str, strFormat, 

	hRoot = GetAncestor(hOwner, GA_ROOT);

	SendMessage(hRoot, WM_THROWERROR, (WPARAM) str, NULL);

	return TRUE;

}