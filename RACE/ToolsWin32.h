#pragma once

#include "stdafx.h"



//Window Border Tools
DWORD GetWindowHeight(HWND hwnd);
DWORD GetWindowWidth(HWND hwnd);
DWORD GetClientHeight(HWND hwnd);
DWORD GetClientWidth(HWND hwnd);
DWORD GetChildRect(HWND hParent, HWND hChild, RECT *rcChild);
DWORD GetClientDimensions(HWND hwnd, INT *width, INT *height);
RECT ScreenToClient(HWND hwnd, RECT rScreen);
BOOL SetWindowPos(HWND hwnd, HWND hWndInsertAfter, INT x, INT y, UINT uFlags);
DWORD PositionDialog(HWND hDialog, HWND hReference, DWORD flag);

//Dialog Tools
DWORD DialogReturn(HWND hwnd, LRESULT lReturn);
DWORD SetDialogParent(HWND hDialog, HWND hParent);

//Button Tools
DWORD Button_GetEnable(HWND hButton);
BOOL Button_Disable(HWND hwnd);
DWORD CheckButton(HWND hButton, DWORD *flag);
DWORD UnCheckButton(HWND hButton, DWORD *flag);
DWORD UpdateCheckFlag(HWND hButton, DWORD *flag);

//CheckBox Tools
DWORD UpdateCheckBoxFilter(HWND hCheck, DWORD filter, DWORD flag);


//Property Sheet Tools
INT CALLBACK PropSheetProc(HWND hwnd, UINT msg, LPARAM lParam);

//All Windows
HWND GetHWND(HWND hwnd, DWORD typeHWND);
DWORD GetClassName(HWND hwnd, CHAR *strClassName);
DWORD SetHWND(HWND hwnd, HWND hSet, DWORD typeHWND);
LONG_PTR WINAPI GetWindowUserData(HWND hwnd);
LPVOID GetWindowLongPtr(HWND hwnd);

LONG_PTR InitWindowLong(HWND hwnd, DWORD size);
LONG_PTR InitWindowLongRACE(HWND hwnd, DWORD size);

//EditBox Tools
FLOAT Edit_GetFloat(HWND hEdit);
DWORD Edit_SetFloat(HWND hEdit, FLOAT value, INT precision);
DWORD Edit_SetFloat(HWND hEdit, FLOAT value);
DWORD Edit_SetFloat3(HWND hEdit, FLOAT value);
DWORD Edit_SetFloat4(HWND hEdit, FLOAT value);
DWORD Edit_SetFloat5(HWND hEdit, FLOAT value);
DWORD Edit_SetFloat6(HWND hEdit, FLOAT value);
DWORD Edit_SetFloat7(HWND hEdit, FLOAT value);
INT Edit_GetInt(HWND hEdit);
DWORD Edit_SetInt(HWND hEdit, INT value);

//RECT Tools
INT GetRectWidth(RECT r);
INT GetRectHeight(RECT r);

//General Window Tools
BOOL MoveWindow(HWND hwnd, RECT rect, BOOL bRepaint);
DWORD AppendWindowText(HWND hwnd, CHAR *strAppend);

DWORD GetDesktopDimensions(LONG *w, LONG *h);
DWORD GetBitmapDimensions(HDC hdc, LONG *w, LONG *h);
DWORD GetBitmapDimensions(HWND hwnd, LONG *w, LONG *h);
DWORD GetBitmap(HWND hwnd, LONG w, LONG h, PIXEL *img);

LRESULT CALLBACK MouseHookProc(INT nCode, WPARAM wParam, LPARAM lParam);

DWORD GetBitmap(HWND hwnd, LONG w, LONG h, POINT ptUL, POINT ptLR, PIXEL *img);

DWORD KillProcess(CHAR *strProcess);

LONG GetMicroSeconds(VOID);
ULONGLONG GetSyncedMicroSeconds(VOID);