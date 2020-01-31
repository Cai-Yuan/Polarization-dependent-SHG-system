#include "stdafx.h"

#include "ToolsWIN32.h"
#include "Winuser.h"
//#include "Windows.h"

#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <windows.h>

DWORD PositionDialog(HWND hDialog, HWND hReference, DWORD flag)
{
	RECT rcDialog;
	RECT rcReference;

	GetWindowRect(hDialog, &rcDialog);
	GetWindowRect(hReference, &rcReference);

	INT wR = rcReference.right-rcReference.left;
	INT hR = rcReference.bottom - rcReference.top;
	INT wD = rcDialog.right-rcDialog.left;
	INT hD = rcDialog.bottom - rcDialog.top;

	INT x = (wR/2)-(wD/2);
	INT y = (hR/2)-(hD/2);

	SetWindowPos(hDialog, HWND_TOPMOST, x, y, SWP_SHOWWINDOW);

	return TRUE;
}

BOOL SetWindowPos(HWND hwnd, HWND hWndInsertAfter, INT x, INT y, UINT uFlags)
{
	RECT r;
	GetWindowRect(hwnd, &r);


	return SetWindowPos(hwnd, hWndInsertAfter, x, y, r.right-r.left, r.bottom-r.top, uFlags);
}

RECT ScreenToClient(HWND hClientRef, RECT rScreen)
{
	RECT rClient;
	POINT ptUL;
	POINT ptUR;
	POINT ptLL;
	POINT ptLR;

	ptUL.x = rScreen.left;
	ptUL.y = rScreen.top;
	ptLR.x = rScreen.right;
	ptLR.y = rScreen.bottom;

	ScreenToClient(hClientRef, &ptUL);
	ScreenToClient(hClientRef, &ptLR);

	rClient.left = ptUL.x;
	rClient.top = ptUL.y;
	rClient.right = ptLR.x;
	rClient.bottom = ptLR.y;

	return rClient;
}

DWORD GetWindowHeight(HWND hwnd)
{
	RECT rc = {0};

	GetWindowRect(hwnd, &rc);
	
	return (DWORD) (rc.bottom - rc.top);
}

DWORD GetWindowWidth(HWND hwnd)
{
	RECT rc = {0};

	GetWindowRect(hwnd, &rc);
	
	return (DWORD) (rc.right - rc.left);
}

DWORD GetClientHeight(HWND hwnd)
{
	RECT rc = {0};

	GetClientRect(hwnd, &rc);

	return (DWORD) (rc.bottom - rc.top);
}

DWORD GetClientWidth(HWND hwnd)
{

	RECT rc = {0};

	GetClientRect(hwnd, &rc);

	return (DWORD) (rc.right - rc.left);

}

//Returns the rect of a child relative to it's parent

DWORD GetChildRect(HWND hParent, HWND hChild, RECT *rcChild)
{
	RECT rcParent = {0};
	RECT rc = {0};
	RECT rcMaped = {0};
	POINT ptChild = {0};
	
	if(rcChild){

		//Returns Screen Coordinates
		GetWindowRect(hChild, rcChild);
		GetWindowRect(hParent, &rcParent);

		//memcpy(&rcMaped, &rcChild, sizeof(RECT));

		//MapWindowPoints(HWND_DESKTOP, hParent, (POINT *) &rcMaped, 2);

		rcMaped.left = rcChild->left-rcParent.left;
		rcMaped.right = rcMaped.left+GetRectWidth(*rcChild);
		rcMaped.top = rcChild->top-rcParent.top;
		rcMaped.bottom = rcMaped.top+GetRectHeight(*rcChild);

		memcpy(rcChild, &rcMaped, sizeof(RECT));

	}
	else
		return FALSE;

	return TRUE;

}

//DWORD GetChildRect(HWND hParent, HWND hChild, RECT *rcChild)
//{
//	RECT rcParent = {0};
//	RECT rc = {0};
//	POINT ptChild = {0};
//	
//	if(rcChild){
//
//		//Returns Screen Coordinates
//		GetWindowRect(hParent, &rcParent);
//
//		GetWindowRect(hChild, rcChild);
//
//		ptChild.x = rcChild->left;
//		ptChild.y = rcChild->top;
//		ScreenToClient(hParent, &ptChild);
//
//		rc.right = rcChild->right - ptChild.x;
//		rc.left = rcChild->left - ptChild.y;
//
//		rc.bottom = rcChild->bottom - ptChild.y;
//		rc.top = rcChild->top - ptChild.y;
//
//
//		memcpy(rcChild, &rc, sizeof(RECT));
//
//	}
//	else
//		return FALSE;
//
//	return TRUE;
//
//}

DWORD GetClientDimensions(HWND hwnd, INT *width, INT *height)
{

	if(width){
		*width = GetClientWidth(hwnd);
	}

	if(height){
		*height = GetClientHeight(hwnd);
	}

	return TRUE;
}

DWORD DialogReturn(HWND hwnd, LRESULT lReturn)
{

	SetWindowLong(hwnd, NULL, lReturn);

	return TRUE;
}

DWORD Button_GetEnable(HWND hButton)
{
	DWORD fEnable = FALSE;
	
	//Set the button state to 'TRUE'
	//fEnable holds the current state
	//	0 - Disabled
	//	1 - Enabled
	fEnable = Button_Enable(hButton, TRUE);

	//Reset the button to previous state
	Button_Enable(hButton, fEnable);

	return fEnable;
}

DWORD SetDialogParent(HWND hDialog, HWND hParent)
{

	if(!hDialog || !hParent)
		return FALSE;

	LONG_PTR lpStyle = NULL;
	HWND hPreviousParent = NULL;

	hPreviousParent = SetParent(hDialog, hParent);
	lpStyle = GetWindowLongPtr(hDialog, GWL_STYLE);
	//lpStyle = lpStyle&(!WS_POPUP);
	lpStyle = lpStyle|WS_CHILD;
	SetWindowLongPtr(hDialog, GWL_STYLE, lpStyle);


	return TRUE;
}

//Updates a filter using the specified flag based on the state of a check box
DWORD UpdateCheckBoxFilter(HWND hCheck, DWORD filter, DWORD flag)
{
	DWORD check = NULL;
	check = Button_GetCheck(hCheck);

	if(check){
		filter = filter|flag;
	}
	else{
		check = !flag;
		filter = filter&(~flag);
	}

	return filter;
}

INT CALLBACK PropSheetProc(HWND hwnd, UINT msg, LPARAM lParam)
{
	INT i = NULL;

	switch(msg){
		case PSCB_BUTTONPRESSED:
			i = 0;
			i = 0;
			break;
		case PSCB_INITIALIZED:
			i = 0;
			HWND hTabControl;
			hTabControl = PropSheet_GetTabControl(hwnd);
			hTabControl = hTabControl;
			break;
		case PSCB_PRECREATE:
			i = 0;
			break;
		case WM_COMMAND:
			i = 0;
			i = 0;
			break;
		default:
			break;
	}

	switch(lParam){
		case PSBTN_OK:
			i = 0;
			i = 0;
			break;
		case PSBTN_CANCEL:
			i = 0;
			i = 0;
			break;
		default:
			break;
	}


	return FALSE;
}

DWORD GetClassName(HWND hwnd, CHAR *strClassName)
{
	DWORD lastError = NULL;

	if(!strClassName){
		return FALSE;
	}

	WNDCLASSEX wcex = {0};
	HINSTANCE hInstance = (HINSTANCE) GetWindowLongPtr(hwnd, (-6)/*GWL_HINSTANCE*/);

	if(!GetClassInfoEx(hInstance, strClassName, &wcex)){
		lastError = GetLastError();
	}

	return TRUE;
}

//Provides Basic Window Linking
//Sends a 'WM_SETHWND' message to 'hwnd'
//The LPARAM of the message is the 'hSet' value
DWORD SetHWND(HWND hwnd, HWND hSet, DWORD typeHWND)
{
	return SendMessage(hwnd, WM_SETHWND, (WPARAM) typeHWND, (LPARAM) hSet);

}

//Gets Basic Window Linking
//Sends a 'WM_GETHWND' message to hwnd
HWND GetHWND(HWND hwnd, DWORD typeHWND)
{
	return (HWND) SendMessage(hwnd, WM_GETHWND, (WPARAM) typeHWND, NULL);
}

LONG_PTR InitWindowLong(HWND hwnd, DWORD size)
{
	LONG_PTR lp = NULL;

	lp = (LONG_PTR) malloc(size);
	
	if(!lp){
		return FALSE;
	}

	memset((LPVOID)lp, NULL, size);
	SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lp);
	lp = GetWindowUserData(hwnd);
	//lp = GetWindowLongPtr(hwnd, (-21));

	if(lp == NULL){
		return FALSE;
	}

	return lp;
}

LONG_PTR InitWindowLongRACE(HWND hwnd, DWORD size)
{
	LONG_PTR lp = NULL;

	lp = InitWindowLong(hwnd, size);
	
	if(lp == NULL){
		CHAR str[MAX_STRING];
		CHAR strOut[MAX_STRING];

		GetWindowText(hwnd, str, MAX_STRING);
		sprintf(strOut, "Error Getting Long Ptr ('%s')");
		ThrowMessage(hwnd, strOut);
		SendMessage(hwnd, WM_CLOSE, NULL, NULL);
		return FALSE;
	}

	return lp;
}

FLOAT Edit_GetFloat(HWND hEdit)
{
	FLOAT value;
	CHAR str[MAX_STRING];
	Edit_GetText(hEdit, str, 256);
	sscanf(str, "%f", &value);
	return value;
}

INT Edit_GetInt(HWND hEdit)
{
	INT value;
	CHAR str[MAX_STRING];
	Edit_GetText(hEdit, str, 256);
	sscanf(str, "%d", &value);
	return value;
}

DWORD Edit_SetInt(HWND hEdit, INT value)
{
	CHAR str[MAX_STRING];
	sprintf(str, "%d", value);
	Edit_SetText(hEdit, str);

	return TRUE;
}

DWORD Edit_SetFloat(HWND hEdit, FLOAT value)
{
	//CHAR str[MAX_STRING];
	//sprintf(str, "%.2f", value);
	//Edit_SetText(hEdit, str);
	Edit_SetFloat(hEdit, value, 2);
	return TRUE;
}

DWORD Edit_SetFloat3(HWND hEdit, FLOAT value)
{
	//CHAR str[MAX_STRING];
	//sprintf(str, "%.3f", value);
	//Edit_SetText(hEdit, str);
	Edit_SetFloat(hEdit, value, 3);

	return TRUE;
}

DWORD Edit_SetFloat4(HWND hEdit, FLOAT value)
{
	//CHAR str[MAX_STRING];
	//sprintf(str, "%.4f", value);
	//Edit_SetText(hEdit, str);
	Edit_SetFloat(hEdit, value, 4);

	return TRUE;
}

DWORD Edit_SetFloat5(HWND hEdit, FLOAT value)
{
	//CHAR str[MAX_STRING];
	//sprintf(str, "%.5f", value);
	//Edit_SetText(hEdit, str);
	Edit_SetFloat(hEdit, value, 5);

	return TRUE;
}

DWORD Edit_SetFloat6(HWND hEdit, FLOAT value)
{
	//CHAR str[MAX_STRING];
	//sprintf(str, "%.6f", value);
	//Edit_SetText(hEdit, str);
	Edit_SetFloat(hEdit, value, 6);

	return TRUE;
}

DWORD Edit_SetFloat7(HWND hEdit, FLOAT value)
{
	//CHAR str[MAX_STRING];
	//sprintf(str, "%.7f", value);
	//Edit_SetText(hEdit, str);
	Edit_SetFloat(hEdit, value, 7);

	return TRUE;
}

DWORD Edit_SetFloat(HWND hEdit, FLOAT value, INT precision)
{
	CHAR str[MAX_STRING];
	switch(precision){
		case 1:
			sprintf(str, "%.1f", value);
			break;
		case 2:
			sprintf(str, "%.2f", value);
			break;
		case 3:
			sprintf(str, "%.3f", value);
			break;
		case 4:
			sprintf(str, "%.4f", value);
			break;
		case 5:
			sprintf(str, "%.5f", value);
			break;
		case 6:
			sprintf(str, "%.6f", value);
			break;
		case 7:
			sprintf(str, "%.7f", value);
			break;
		default:
			sprintf(str, "%f", value);
			break;
	}

	Edit_SetText(hEdit, str);

	return TRUE;
}

BOOL Button_Disable(HWND hwnd)
{
	Button_Enable(hwnd, FALSE);
	return TRUE;
}

LONG_PTR WINAPI GetWindowUserData(HWND hwnd)
{
	return GetWindowLongPtr(hwnd, (-21));
}

INT GetRectWidth(RECT r)
{
	return r.right-r.left;
}

INT GetRectHeight(RECT r)
{
	return r.bottom-r.top;
}

BOOL MoveWindow(HWND hwnd, RECT rect, BOOL bRepaint)
{
	return MoveWindow(hwnd, rect.left, rect.top, GetRectWidth(rect), GetRectHeight(rect), bRepaint);
}


DWORD AppendWindowText(HWND hwnd, CHAR *strAppend)
{
	CHAR str[MAX_STRING];

	GetWindowText(hwnd, str, MAX_STRING);
	sprintf(str, "%s - %s", str, strAppend);
	SetWindowText(hwnd, str);

	return TRUE;

}


DWORD UpdateCheckFlag(HWND hButton, DWORD *flag)
{
	if(!flag)
		return FALSE;

	LRESULT lResult = Button_GetCheck(hButton);

	if(lResult == BST_CHECKED){
		*flag = 1;
	}
	if(lResult == BST_UNCHECKED){
		*flag = 0;
	}

	return TRUE;
}

DWORD CheckButton(HWND hButton, DWORD *flag)
{
	Button_SetCheck(hButton, BST_CHECKED);
	*flag = TRUE;

	return TRUE;
}

DWORD UnCheckButton(HWND hButton, DWORD *flag)
{
	Button_SetCheck(hButton, BST_UNCHECKED);
	*flag = FALSE;

	return TRUE;
}

DWORD GetDesktopDimensions(LONG *w, LONG *h)
{

	if(!w||!h)
		return FALSE;

	BITMAP bitmapHeader = {0};
	HWND hDesktop = GetDesktopWindow();
	HDC hdc = GetDC(hDesktop);
	HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
	GetObject(hBitmap, sizeof(BITMAP), &bitmapHeader);
	*w = bitmapHeader.bmWidth;
	*h = bitmapHeader.bmHeight;
	
	return TRUE;
}

DWORD GetBitmapDimensions(HDC hdc, LONG *w, LONG *h)
{
	BITMAP bitmapHeader = {0};
	HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
	GetObject(hBitmap, sizeof(BITMAP), &bitmapHeader);
	*w = bitmapHeader.bmWidth;
	*h = bitmapHeader.bmHeight;
	

	return TRUE;
}

DWORD GetBitmapDimensions(HWND hwnd, LONG *w, LONG *h)
{
	BITMAP bitmapHeader = {0};
	HDC hdc = GetDC(hwnd);
	HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
	GetObject(hBitmap, sizeof(BITMAP), &bitmapHeader);
	*w = bitmapHeader.bmWidth;
	*h = bitmapHeader.bmHeight;

	return TRUE;
}

DWORD GetBitmap(HWND hwnd, LONG w, LONG h, PIXEL *img)
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

	LONG wBitmap;
	LONG hBitmap;

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hwnd);
	
	GetBitmapDimensions(hwnd, &wBitmap, &hBitmap);

    // Create a compatible DC which is used in a BitBlt from the window DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 

    if(!hdcMemDC)
    {
        MessageBox(hwnd, (LPCSTR)"CreateCompatibleDC has failed", (LPCSTR)"Failed", MB_OK);
        goto done;
    }

    // Get the client area for size calculation
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);

    //This is the best stretch mode
    SetStretchBltMode(hdcWindow,HALFTONE);

	if(0){
		//The source DC is the entire screen and the destination DC is the current window (HWND)
		if(!StretchBlt(hdcWindow, 
				   0,0, 
				   w/*rcClient.right*/, h/*rcClient.bottom*/, 
				   hdcScreen, 
				   0,0,
				   w/*GetSystemMetrics (SM_CXSCREEN)*/,
				   h/*GetSystemMetrics (SM_CYSCREEN)*/,
				   SRCCOPY))
		{
			MessageBox(hwnd, (LPCSTR)"StretchBlt has failed", (LPCSTR)"Failed", MB_OK);
			goto done;
		}
	}
    
    // Create a compatible bitmap from the Window DC
    hbmScreen = CreateCompatibleBitmap(hdcWindow, wBitmap/*rcClient.right-rcClient.left*/, hBitmap/*rcClient.bottom-rcClient.top*/);
    
    if(!hbmScreen)
    {
        MessageBox(hwnd, (LPCSTR)"CreateCompatibleBitmap Failed", (LPCSTR)"Failed", MB_OK);
        goto done;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC,hbmScreen);
    
    // Bit block transfer into our compatible memory DC.
    if(!BitBlt(hdcMemDC, 
               0,0, 
               wBitmap/*rcClient.right-rcClient.left*/, hBitmap/*rcClient.bottom-rcClient.top*/, 
               hdcWindow, 
               0,0,
               SRCCOPY))
    {
        MessageBox(hwnd, (LPCSTR)"BitBlt has failed", (LPCSTR)"Failed", MB_OK);
        goto done;
    }

    // Get the BITMAP from the HBITMAP
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);
     
    BITMAPFILEHEADER   bmfHeader;    
    BITMAPINFOHEADER   bi;
     
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bmpScreen.bmWidth;    
    bi.biHeight = bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
    char *lpbitmap = (char *)GlobalLock(hDIB);    

    // Gets the "bits" from the bitmap and copies them into a buffer 
    // which is pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO *)&bi, DIB_RGB_COLORS);

	if(lpbitmap){

		if(w == wBitmap && h == hBitmap){
			memcpy(img, lpbitmap, w*h*sizeof(PIXEL));
		}
	}

    // A file is created, this is where we will save the screen capture.
/*    HANDLE hFile = CreateFile((LPCSTR)"captureqwsx.bmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);  */ 
    
    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
    
    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB; 
    
    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM   
 
    //DWORD dwBytesWritten = 0;
    //WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    //WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    //WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    
    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    //CloseHandle(hFile);
       
    //Clean up
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL,hdcScreen);
    ReleaseDC(hwnd,hdcWindow);

    return 0;
}

DWORD GetBitmap(HWND hwnd, LONG w, LONG h, POINT ptUL, POINT ptLR, PIXEL *img)
{
    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

	LONG wBitmap;
	LONG hBitmap;

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hwnd);

	//Convert Screen coordinates to logical coordinates
	LONG wRect = ptLR.x-ptUL.x;
	LONG hRect = ptLR.y-ptUL.y;

	PhysicalToLogicalPoint(hwnd, &ptLR);
	PhysicalToLogicalPoint(hwnd, &ptUL);
	
	//GetBitmapDimensions(hwnd, &wBitmap, &hBitmap);
	wBitmap = wRect;
	hBitmap = hRect;

    // Create a compatible DC which is used in a BitBlt from the window DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 

    if(!hdcMemDC)
    {
        MessageBox(hwnd, (LPCSTR)"CreateCompatibleDC has failed", (LPCSTR)"Failed", MB_OK);
        goto done;
    }

    // Get the client area for size calculation
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);

    //This is the best stretch mode
   // SetStretchBltMode(hdcWindow,HALFTONE);

    
    // Create a compatible bitmap from the Window DC
    hbmScreen = CreateCompatibleBitmap(hdcWindow, wBitmap/*rcClient.right-rcClient.left*/, hBitmap/*rcClient.bottom-rcClient.top*/);
    
    if(!hbmScreen)
    {
        MessageBox(hwnd, (LPCSTR)"CreateCompatibleBitmap Failed", (LPCSTR)"Failed", MB_OK);
        goto done;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC,hbmScreen);
    
    // Bit block transfer into our compatible memory DC.
    if(!BitBlt(hdcMemDC, 
               0, 0, 
              wRect, hRect, 
               hdcWindow, 
               ptUL.x, ptLR.y,
               SRCCOPY))
    {
        MessageBox(hwnd, (LPCSTR)"BitBlt has failed", (LPCSTR)"Failed", MB_OK);
        goto done;
    }

    //if(!BitBlt(hdcMemDC, 
    //           ptUL.x, ptLR.y, 
    //          wRect/* wBitmap*//*rcClient.right-rcClient.left*/, hRect/*hBitmap*//*rcClient.bottom-rcClient.top*/, 
    //           hdcWindow, 
    //           0, 0,
    //           SRCCOPY))
    //{
    //    MessageBox(hwnd, (LPCSTR)"BitBlt has failed", (LPCSTR)"Failed", MB_OK);
    //    goto done;
    //}

    // Get the BITMAP from the HBITMAP
    GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);
     
    BITMAPFILEHEADER   bmfHeader;    
    BITMAPINFOHEADER   bi;
     
    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = wRect;    
    bi.biHeight = hRect;  
    //bi.biWidth = bmpScreen.bmWidth;    
    //bi.biHeight = bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
    char *lpbitmap = (char *)GlobalLock(hDIB);    

    // Gets the "bits" from the bitmap and copies them into a buffer 
    // which is pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO *)&bi, DIB_RGB_COLORS);

	if(lpbitmap){

		if(w == wBitmap && h == hBitmap){
			memcpy(img, lpbitmap, w*h*sizeof(PIXEL));
		}
	}

    // A file is created, this is where we will save the screen capture.
/*    HANDLE hFile = CreateFile((LPCSTR)"captureqwsx.bmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);  */ 
    
    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
    
    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB; 
    
    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM   
 
    //DWORD dwBytesWritten = 0;
    //WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    //WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    //WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    
    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    //CloseHandle(hFile);
       
    //Clean up
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL,hdcScreen);
    ReleaseDC(hwnd,hdcWindow);

    return 0;
}

DWORD KillProcess(CHAR *strProcess)
{
	if(!strProcess)
		return FALSE;

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;

	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);

	while(hRes)
	{
		if(strcmp(pEntry.szExeFile, strProcess) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD) pEntry.th32ProcessID);

			if(hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}

		}

		hRes = Process32Next(hSnapShot, &pEntry);
	}

	CloseHandle(hSnapShot);

	return TRUE;
}

//LONG GetMicroSeconds(VOID)
//{
//	LONG tSec;
//	LONG tuSec;
//	static const LONG EPOCH = ((LONG) 116444736000000000ULL);
//
//	SYSTEMTIME	sysTime;
//	FILETIME	fileTime;
//	LONG		time;
//
//	GetSystemTime(&sysTime);
//	SystemTimeToFileTime(&sysTime, &fileTime);
//    time =  ((LONG)fileTime.dwLowDateTime );
//    //time += ((LONG)fileTime.dwHighDateTime) << 32;
//
//    tSec  = (LONG) ((time - EPOCH) / 10000000L);
//    tuSec = (LONG) (sysTime.wMilliseconds * 1000);
//
//	return tuSec;
//}

ULONGLONG GetSyncedMicroSeconds(VOID)
{
	FILETIME fileTime;

	//Bad Practice, but works
	ULONGLONG uSeconds;			//Time in uSeconds since 1601
	
	GetSystemTimeAsFileTime((FILETIME *)&uSeconds);
	//GetSystemTimePreciseAsFileTime(&fileTime);

	return uSeconds;
}

//LRESULT CALLBACK MouseHookProc(INT nCode, WPARAM wParam, LPARAM lParam)
//{
//
//	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
//	PMOUSEHOOKSTRUCT m = (PMOUSEHOOKSTRUCT)(lParam);
//	POINT p;
//
//	if(wParam == WM_RBUTTONDOWN)
//	{
//		//right button clicked
//		GetCursorPos(&p);
//		p.x = p.x;
//		p.y = p.y;
//	}
//	if(wParam == WM_LBUTTONDOWN)
//	{
//		gg_hwnd = m->hwnd;
//	}
//
//
//	return TRUE;
//}





//DWORD GetBitmap(HWND hwnd, LONG w, LONG h, PIXEL *img)
//{
//	if(!img)
//		return FALSE;
//
//	LONG wBit = NULL;
//	LONG hBit = NULL;
//
//	BITMAP bitmapHeader = {0};
//	bitmapHeader.bmWidth = w;
//	bitmapHeader.bmHeight = h;
//	bitmapHeader.bmWidthBytes = (w*4);
//	bitmapHeader.bmBits = (PIXEL *)malloc(w*h*4);
//	bitmapHeader.bmType = 0;
//	bitmapHeader.bmPlanes = 1;
//	bitmapHeader.bmBitsPixel = 32;
//	HDC hdc = GetDC(hwnd);
//	HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
//	GetObject(hBitmap, sizeof(BITMAP)+(w*h*4), &bitmapHeader);
//	
//	wBit = bitmapHeader.bmWidth;
//	hBit = bitmapHeader.bmHeight;
//
//	if(wBit != w || hBit != h)
//		return FALSE;
//
//	if(bitmapHeader.bmBits){
//		memcpy(img, bitmapHeader.bmBits, w*h*sizeof(PIXEL));
//	}
//
//	return TRUE;
//}

//DWORD GetBitmap(HWND hwnd, LONG w, LONG h, PIXEL *img)
//{
//	if(!img)
//		return FALSE;
//
//	LONG wBit = NULL;
//	LONG hBit = NULL;
//
//	BITMAP bitmapHeader = {0};
//	bitmapHeader.bmWidth = w*h*4;
//	bitmapHeader.bmBits = (PIXEL *)malloc(w*h*4);
//	HDC hdc = GetDC(hwnd);
//	HBITMAP = hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
//	//HGDIOBJ hBitmap = GetCurrentObject(hdc, OBJ_BITMAP);
//	GetObject(hBitmap, sizeof(BITMAP), &bitmapHeader);
//	
//	wBit = bitmapHeader.bmWidth;
//	hBit = bitmapHeader.bmHeight;
//
//	if(wBit != w || hBit != h)
//		return FALSE;
//
//	if(bitmapHeader.bmBits){
//		memcpy(img, bitmapHeader.bmBits, w*h*sizeof(PIXEL));
//	}
//
//	return TRUE;
//}


