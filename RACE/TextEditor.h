#pragma once

#include "stdafx.h"
#include "Richedit.h"

#define WM_GETHWNDEDIT		0x8002
#define WM_APPENDTEXT		0x8003
#define WM_APPENDTEXTLINE	0x8004
#define WM_NEWLINE			0x8005

#define TN_GETTEXTLENGTH	0x8006

#define MAX_DOC  5000
#define MAX_EDITLENGTH		500000

typedef struct TEXTEDITOR_{

	UINT iWnd;
	HANDLE hText;
	VOID *lpvoidText;

	HWND hEdit;
	HWND hOwner;
	
}TEXTEDITOR;

LRESULT CALLBACK WndTextEditorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

VOID InitializeOutputMemory(HWND hEdit, TEXTEDITOR *lpWndText);
VOID Edit_AppendText(HWND hOutput, CHAR *text);