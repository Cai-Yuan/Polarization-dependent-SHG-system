#include "stdafx.h"

typedef struct AXIOVERT_{

	HWND hOwner;
	HWND hBtnOK;
	HWND hBtnCancel;
	HWND hTab;


}AXIOVERT;

//DLGTEMPLATEEX is not defined in any header file (variable size)
#pragma pack(push, 1)
typedef struct {
  WORD      dlgVer;
  WORD      signature;
  DWORD     helpID;
  DWORD     exStyle;
  DWORD     style;
  WORD      cDlgItems;
  short     x;
  short     y;
  short     cx;
  short     cy;
  //sz_Or_Ord menu;
  //sz_Or_Ord windowClass;
  WCHAR     title[MAX_STRING];
  WORD      pointsize;
  WORD      weight;
  BYTE      italic;
  BYTE      charset;
  WCHAR     typeface[MAX_STRING];
} DLGTEMPLATEEX;
#pragma pack(pop)

INT_PTR CALLBACK Axiovert200Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT OnTabbedDialogInit(HWND hwnd, HWND hTab);