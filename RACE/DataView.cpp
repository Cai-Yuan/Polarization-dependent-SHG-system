#include "stdafx.h"

#include "DataView.h"
#include "ImageScale.h"


//hOwner 
HWND CreateDataView(HWND hOwner, CHAR *strTitle)
{
	HWND hDataView = NULL;
	BOOL flgFree = NULL;
	CHAR *str = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "DataView");
		flgFree = TRUE;
	}

	hDataView = (HWND) CreateWindow("DataView", 
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

	SendMessage(hDataView, WM_MDIACTIVATE, TRUE, (LPARAM) hDataView);
	
	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hDataView;
}

HWND CreateDataView(HWND hOwner,  INT w, INT h, CHAR *strTitle)
{
	HWND hDataView = NULL;
	BOOL flgFree = NULL;
	CHAR *str = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "DataView");
		flgFree = TRUE;
	}

	hDataView = (HWND) CreateWindow("DataView", 
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

	SendMessage(hDataView, WM_MDIACTIVATE, TRUE, (LPARAM) hDataView);

	DataView_SetDimensions(hDataView, w, h);
	
	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hDataView;
}

//Should Be: HWND hFrame
HWND CreateMDIDataView(HWND hMDI, CHAR *strTitle)
{
	HWND hDataView = NULL;
	BOOL flgFree = NULL;
	CHAR *str = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "DataView");
		flgFree = TRUE;
	}

	hDataView = (HWND) SendMessage(hMDI, WM_CREATEMDICHILD, (WPARAM) "DataView", (LPARAM) strTitle);

	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hDataView;
}

HWND CreateMDIDataView(HWND hMDI, LONG w, LONG h, CHAR *strTitle)
{
	HWND hDataView = NULL;
	BOOL flgFree = NULL;
	CHAR *str = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
	memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		strTitle = (CHAR *) malloc(MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "DataView");
		flgFree = TRUE;
	}

	hDataView = (HWND) SendMessage(hMDI, WM_CREATEMDICHILD, (WPARAM) "DataView", (LPARAM) strTitle);

	DataView_SetDimensions(hDataView, w, h);

	if(flgFree){
		if(strTitle){
			free(strTitle);
		}
	}

	return (HWND) hDataView;
}

DWORD DataView_SetDimensions(HWND hDataView, LONG w, LONG h)
{
	return (DWORD) SendMessage(hDataView, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM)h);
}

DWORD DataView_GetDimensions(HWND hDataView, LONG *w, LONG *h)
{
	return (DWORD) SendMessage(hDataView, WM_GETFRAMESIZE, (WPARAM) w, (LPARAM) h);
}

DWORD DataView_SetWidthHeight(HWND hDataView, UINT w, UINT h)
{
	return (DWORD) SendMessage(hDataView, WM_SETFRAMESIZE, (WPARAM)w, (LPARAM) h);
}

DWORD DataView_GetWidthHeight(HWND hDataView, UINT *w, UINT *h)
{
	return (DWORD) SendMessage(hDataView, WM_GETFRAMESIZE, (WPARAM)w, (LPARAM) h);
}


DWORD DataView_SetStartFrame(HWND hDataView, UINT frameStart)
{


	return TRUE;
}

DWORD DataView_SetEndFrame(HWND hDataView, UINT frameEnd)
{



	return TRUE;
}

DWORD DataView_ShowFrame(HWND hDataView, UINT frame)
{


	return TRUE;
}

POINT DataView_GetCursorPixelCoordinate(DATAVIEW *lpDataView)
{

	POINT ptImage = {-1, -1};
	if(!lpDataView)
		return (POINT) ptImage;

	FLOAT xNorm;
	FLOAT yNorm;

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(lpDataView->hwnd, &ptCursor);
	
	ptImage.x = ptCursor.x + lpDataView->hPos;
	ptImage.y = ptCursor.y + lpDataView->vPos;

	//GetNormalizedCoordinate(lpDataView->wStack, lpDataView->hStack, ptImage.x, ptImage.y, &xNorm, &yNorm);

	return (POINT) ptImage;
}

LRESULT DataView_UpdateCursorPixelPosition(DATAVIEW *lpDataView, FLOAT xNorm, FLOAT yNorm)
{

	if(!lpDataView)
		return FALSE;

	UINT w = lpDataView->wStack;
	UINT h = lpDataView->hStack;

	POINT ptCursor;
	POINT ptScaled;
	INT x;
	INT y;

	INT dx;
	INT dy;

	GetPixelCoordinate(w, h, xNorm, yNorm, &x, &y);

	ptScaled.x = x;
	ptScaled.y = y;

	GetCursorPos(&ptCursor);
	ScreenToClient(lpDataView->hwnd, &ptCursor);

	
	SendMessage(lpDataView->hwnd, 
				DV_SETVIEWPOS, 
				(WPARAM)ptScaled.x-ptCursor.x, 
				(LPARAM)ptScaled.y-ptCursor.y);

	return TRUE;


}

LRESULT DataView_ShowFrame(HWND hDataView, PDMFRAME *pdmf)
{
	return SendMessage(hDataView, WM_SHOWFRAME, (WPARAM) pdmf, (LPARAM) pdmf->lpvoid);

}

LRESULT DataView_Draw(HWND hDataView)
{
	return SendMessage(hDataView, WM_DRAW, NULL, NULL);
}

LRESULT DataView_Paint(HWND hDataView)
{
	return SendMessage(hDataView, WM_PAINT, NULL, NULL);
}

LRESULT DataView_GetPDMFrame(HWND hDataView, PDMFRAME *pdm)
{
	return SendMessage(hDataView, WM_GETPDM, (WPARAM)pdm, NULL);
}

LRESULT DataView_GetPDMPTR(HWND hDataView)
{
	return SendMessage(hDataView, WM_GETPDMPTR, NULL, NULL);
}

LRESULT DataView_GetPDMPTR(HWND hDataView, DWORD source)
{
	return SendMessage(hDataView, WM_GETPDMPTR, (WPARAM) source, NULL);
}

LRESULT DataView_SnapFrame(HWND hDataView)
{
	return SendMessage(hDataView, WM_SNAPFRAME, NULL, NULL);
}