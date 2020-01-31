#include "stdafx.h"
#include "ToolBar.h"
#include "Commctrl.h"

#define NUM 5

/*
TODO:
Fix bug that generates icons continuously
-DONE
*/

HWND CreateToolBar(HWND hwndOwner)
{
	HWND hToolBar = NULL;

	hToolBar = CreateWindowEx(NULL, 
								TOOLBARCLASSNAME,
								NULL,
								WS_CHILD|WS_VISIBLE,
								NULL, NULL, NULL, NULL,
								hwndOwner,
								(HMENU) IDC_MAIN_TOOL,
								GetModuleHandle(NULL),
								NULL);


	InitializeToolBar(hToolBar);

	return hToolBar;
}

HWND CreateToolBar(HWND hwndOwner, DWORD idControl, DWORD dwStyle)
{
	HWND hToolBar = NULL;

	hToolBar = CreateWindowEx(NULL, 
								TOOLBARCLASSNAME,
								NULL,
								WS_CHILD|WS_VISIBLE|dwStyle,
								NULL, NULL, NULL, NULL,
								hwndOwner,
								(HMENU) idControl,
								GetModuleHandle(NULL),
								NULL);


	InitializeToolBar(hToolBar);

	return hToolBar;
}

DWORD InitializeToolBar(HWND hToolBar)
{

	//Tell ToolBar which version of library controls you are using
	SendMessage(hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	
	//Add Controls to the ToolBar
	TBBUTTON	tbb[NUM];
	TBADDBITMAP	tbab;

	SendMessage(hToolBar, TB_SETBUTTONSIZE, NULL, MAKELPARAM(16, 16));

	memset(&tbab, 0x00, sizeof(TBADDBITMAP));
	tbab.hInst = HINST_COMMCTRL/*GetModuleHandle(NULL)*/;
	//tbab.nID = IDB_STD_LARGE_COLOR;
	tbab.nID = IDB_STD_SMALL_COLOR;
	SendMessage(hToolBar, TB_ADDBITMAP, 0, (LPARAM) &tbab);


	memset(&tbb, 0x00, NUM*sizeof(TBBUTTON));
	tbb[0].iBitmap = STD_FILENEW;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].idCommand = ID_FILE_NEW;

	tbb[1].iBitmap = STD_FILESAVE;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].idCommand = ID_FILE_SAVEAS;
	
	tbb[2].iBitmap = STD_FILEOPEN;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;
	tbb[2].idCommand = ID_OPEN_FILE;

	tbb[3].iBitmap = 0;
	tbb[3].fsState = TBSTATE_ENABLED;
	tbb[3].fsStyle = TBSTYLE_SEP;
	tbb[3].idCommand = 0;

	tbb[4].iBitmap = STD_DELETE;
	tbb[4].fsState = TBSTATE_ENABLED;
	tbb[4].fsStyle = TBSTYLE_BUTTON;
	tbb[4].idCommand = ID_FILE_CLOSE;

	SendMessage(hToolBar, TB_SETEXTENDEDSTYLE, NULL, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);
	SendMessage(hToolBar, TB_ADDBUTTONS, NUM, (LPARAM)&tbb);
	

	return TRUE;
}