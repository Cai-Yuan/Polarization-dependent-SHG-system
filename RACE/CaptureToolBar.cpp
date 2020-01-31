#include "stdafx.h"

#define NUM 6

#include "CaptureToolBar.h"

HWND CreateCaptureToolBar(HWND hParent)
{

	HWND hToolBar = NULL;

	//Declare and Initialize Local Constants
	INT ImageListID = 0;
	INT numButtons = 3;
	INT bitmapSize = 16;

	DWORD buttonStyles = BTNS_AUTOSIZE;

	//Create Toolbar
	hToolBar = CreateWindowEx(NULL,
								TOOLBARCLASSNAME,
								NULL,
								WS_CHILD|TBSTYLE_WRAPABLE,
								0, 0, 0, 0,
								hParent,
								NULL,
								GetModuleHandle(0),
								NULL);

	if(!hToolBar)
		return NULL;

	HIMAGELIST hImageList = NULL;

	hImageList = ImageList_Create(bitmapSize, bitmapSize,
									ILC_COLOR16|ILC_MASK,
									numButtons, 0);



	return hToolBar;

}
