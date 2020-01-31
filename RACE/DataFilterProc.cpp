#include "stdafx.h"

#include "DataFilter.h"

LRESULT CALLBACK DataFilterProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATAFILTER *lpDataFilter = (DATAFILTER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	CHAR str[MAX_STRING];

	switch(msg){
	case DF_INITFILTER:
		{
			OnInitFilter(lpDataFilter, hwnd, wParam, lParam);
		}
		return TRUE;
	case WM_CREATE:
		{
			return OnCreateDataFilter(lpDataFilter, hwnd, wParam, lParam);
		}
	case WM_NCCREATE:
		{
			//lpDataView = (DATAVIEW *)calloc(1, sizeof(DATAVIEW));
			lpDataFilter = (DATAFILTER *)malloc(sizeof(DATAFILTER));
			memset(lpDataFilter, NULL, sizeof(DATAFILTER));
			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpDataFilter);
			if(!lpDataFilter){
				MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "DataFilter Error!", MB_OK);
			}
		}
		return TRUE;

	}

	return TRUE;
}

DWORD OnInitFilter(DATAFILTER *lpDataFilter, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	DWORD selFilterProc = (DWORD) wParam;
	STRINGSET *ss = (STRINGSET *)lParam;

	if(ss->nStr){
		FreeStringSet(&lpDataFilter->ssFilter);
		CallocStringSet(ss->nStr, ss->string[0].nChar, &lpDataFilter->ssFilter);
		MemcpyStringSet(&lpDataFilter->ssFilter, ss);
		lpDataFilter->selFilterProc = selFilterProc;
		return TRUE;
	}else{
		return FALSE;
	}

}

DWORD OnCreateDataFilter(DATAFILTER *lpDataFilter, HWND hwnd, WPARAM wParam, LPARAM lParam)
{

	lpDataFilter->hwnd = hwnd;

	return TRUE;
}

HWND CreateDataFilter(HWND hOwner, CHAR *strTitle)
{
	HWND hDataFilter = NULL;
	BOOL flgFree = NULL;
	CHAR *str = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "DataFilter");
		flgFree = TRUE;
	}

	hDataFilter = (HWND) CreateWindow("DataFilter", 
									strTitle, 
									WS_OVERLAPPEDWINDOW,
									NULL,
									NULL,
									200,
									200,
									hOwner,
									NULL,
									GetModuleHandle(NULL),
									NULL);
	
	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hDataFilter;
}
