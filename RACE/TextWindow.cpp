
#include "stdafx.h"
#include "Textwindow.h"
#include "TextEditor.h"
#include "Richedit.h"


DWORD InsertNewLine(HWND hTextWindow, CHAR *strNewLine)
{
	CHAR str[MAX_STRING];
	CHAR strSystemTime[MAX_STRING];
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	HWND hTextFrame = hTextWindow;
	HWND hTextEdit = NULL;

	hTextEdit = GetDlgItem(hTextFrame, IDC_CHILD_EDIT);

	SendMessage(hTextFrame, WM_NEWLINE, NULL, NULL);

	SendMessage(hTextFrame, WM_APPENDTEXT, (WPARAM)strNewLine, (LPARAM) NULL);

	return TRUE;
}


//Does not work!!!
DWORD DeleteLastLine(HWND hTextWindow)
{
	DWORD dw = NULL;

	HWND hTextFrame = hTextWindow;
	HWND hTextEdit = NULL;

	DWORD nLine = NULL;
	DWORD lLine = NULL;
	DWORD lText = NULL;

	DWORD start = NULL;
	DWORD end = NULL;

	GETTEXTLENGTHEX gtl = {0};
	gtl.flags = GTL_DEFAULT;
	gtl.codepage = CP_ACP;
	gtl.codepage = 1200;

	hTextEdit = GetDlgItem(hTextFrame, IDC_CHILD_EDIT);

	nLine = SendMessage(hTextEdit, EM_GETLINECOUNT, NULL, NULL);
	lLine = SendMessage(hTextEdit, EM_LINELENGTH, nLine, NULL);
	lText = SendMessage(hTextEdit, EM_GETTEXTLENGTHEX, (WPARAM) &gtl, NULL);


	SendMessage(hTextEdit, EM_SETSEL, (WPARAM) 0/*(lText - lLine)*/, (LPARAM) lLine/*lText*/);
	SendMessage(hTextEdit, EM_REPLACESEL, (WPARAM) FALSE, (LPARAM) "");


	return dw;
}