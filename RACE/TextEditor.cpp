#include "stdafx.h"
#include "TextEditor.h"
#include "Windowsx.h"
#include "RACE.h"


//TODO:
//11.9.2012
//Enable typing in window


LRESULT CALLBACK WndTextEditorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	TEXTEDITOR *lpWndText = (TEXTEDITOR *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch(msg)
    {
        case WM_CREATE:
			{
				HFONT hfDefault;
				HWND hEdit, hListView;
				LONG_PTR lpEdit;

				// Create Edit Control
				hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
					WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE /*| ES_AUTOVSCROLL | ES_AUTOHSCROLL */| WS_SYSMENU, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					hwnd, 
					(HMENU)IDC_CHILD_EDIT, 
					GetModuleHandle(NULL), 
					NULL);

				lpWndText->hEdit = hEdit;

				if(hEdit == NULL){
					MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
				}
				else{
					if(lpWndText)
						InitializeOutputMemory(hEdit, lpWndText);
				}

				//Sets or removes the read-only style (ES_READONLY) of an edit control.
				SendMessage(hEdit, EM_SETREADONLY, (WPARAM)FALSE, (LPARAM)NULL);

				Edit_LimitText(hEdit, MAX_EDITLENGTH);


				lpEdit = GetWindowLongPtr(hEdit, GWL_ID);

				hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
				SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			}
			break;
		case WM_NCCREATE:
			{
				lpWndText = (TEXTEDITOR *)malloc(sizeof(TEXTEDITOR));
				memset(lpWndText, 0x00, sizeof(TEXTEDITOR));
				SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpWndText);
				lpWndText = (TEXTEDITOR *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
				if(lpWndText == NULL){
					MessageBox(hwnd, (LPCSTR)"GetWindowLongPtr Failed", (LPCSTR)"Error!", MB_OK);
				}

				return DefMDIChildProc(hwnd, msg, wParam, lParam); 
				case WM_MDIACTIVATE:
				{
					//ManageMDIFrameMenu(hwnd, g_hMainWindow, wParam, lParam);
				}
			}
			break;
		case WM_COMMAND:
			{
				switch(wParam)
				//switch(LOWORD(wParam))
				{
					case ID_FILE_SAVEAS:
						//DoFileSave(hwnd);
						INT temptemp;
						temptemp = 0;
						temptemp = 0;
						break;
					case ID_FILE_CLOSE:
						HWND hManifest;

						//Protect the Manifest From Being Closed Accidentally
						hManifest = (HWND) SendMessage(GetParent(GetParent(hwnd)), WM_GETMDIMANIFEST, NULL, NULL);
						if(hwnd != hManifest){
							SendMessage(hwnd, WM_CLOSE, NULL, NULL);
						}
						break;
					case TN_GETTEXTLENGTH:
						return (LRESULT) strlen((CHAR *)lpWndText->lpvoidText);
					default:
						break;
				}
			}
			break;
		case WM_MDIDESTROY:
			break;
		//case WM_GETHWNDEDIT:
		//	return (LRESULT) lpWndText->hEdit;
		case WM_GETHWND:
			if(wParam == HWND_EDIT)
				return (LRESULT) lpWndText->hEdit;
			break;
		case WM_SETHWND:
			if(wParam == HWND_OWNER)
				lpWndText->hOwner = (HWND) lParam;
			break;
		case WM_APPENDTEXT:
			{
				CHAR *pText;
				UINT lText;
				pText = (CHAR *)wParam;
				lText = Edit_GetTextLength(lpWndText->hEdit);
				SendMessage(lpWndText->hEdit, EM_SETSEL, (WPARAM)lText, (LPARAM)lText+1);
				SendMessage(lpWndText->hEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pText);
				SendMessage(lpWndText->hEdit, EM_SETMODIFY, (WPARAM)TRUE, (LPARAM)NULL);
			}
			return TRUE;
		case WM_APPENDTEXTLINE:
			break;
		case WM_NEWLINE:
			{
				SendMessage(hwnd, WM_APPENDTEXT, (WPARAM)"\r\n", NULL);
			}
			return TRUE;
		
		case WM_NOTIFY:
			//HandleWM_NOTIFY(lParam);
			break;
		case WM_SIZE:
			{
				HWND hEdit;
				RECT rcClient;

				// Calculate remaining height and size edit
				GetClientRect(hwnd, &rcClient);

				hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
				SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
				SendMessage(lpWndText->hOwner, WM_UPDATEFRAME, NULL, NULL);
			}
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
		case WM_CLOSE:
			{
				CHAR str[MAX_STRING];
				GetWindowText(hwnd, str, MAX_STRING);  //GetWindowText()是获取控件当前内容

				if(!strcmp(str, "Manifest")){
					ShowWindow(hwnd, SW_HIDE);
					//ThrowMessage(hwnd, "Manifest cannot be closed.");
					return TRUE;
				}

				return DefMDIChildProc(hwnd, msg, wParam, lParam);;
			}
		default:
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
    
    }
    return 0;
}

VOID InitializeOutputMemory(HWND hEdit, TEXTEDITOR *lpWndText)
{
	HLOCAL hLocalText;
	VOID *lpvoidText;

	//Gets a handle of the memory currently allocated for a multiline edit control's text.
	hLocalText = (HLOCAL) SendMessage(hEdit, EM_GETHANDLE, NULL, NULL);

	//If the function succeeds, the application can access the contents of the edit control 
	//by casting the return value to HLOCAL and passing it to LocalLock

	//If the function succeeds, the return value is a pointer to the first byte of the memory block.
	if((lpvoidText = LocalLock(hLocalText))){
		lpWndText->lpvoidText = lpvoidText;
	}

}

VOID Edit_AppendText(HWND hOutput, CHAR *text)
{
	SendMessage(hOutput, WM_APPENDTEXT, (WPARAM)text, (LPARAM)NULL);
}