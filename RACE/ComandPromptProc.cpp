#include "stdafx.h"

#include "CommandPrompt.h"

#define H_COMBO		25
#define W_SEND		50
#define H_SEND		25
#define W_CONN		75
#define H_CONN		25

LRESULT CALLBACK CommandPromptProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	COMMANDPROMPT *lpCommandPrompt = (COMMANDPROMPT *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	RECT rcClient;
	RECT rcCombo;

	switch(msg)
	{
	case WM_SIZE:

		GetClientRect(hwnd, &rcClient);

		GetClientRect(lpCommandPrompt->hCommand, &rcCombo);

		SetWindowPos(lpCommandPrompt->hConnect,
					NULL,
					rcClient.right-W_CONN, 0, W_CONN, rcClient.top+H_CONN, 
					SWP_NOZORDER);
		SetWindowPos(lpCommandPrompt->hSend,
					NULL,
					rcClient.right-W_SEND, rcClient.bottom-H_SEND, W_SEND, H_SEND/*rcClient.bottom*/,
					SWP_NOZORDER);

		SetWindowPos(lpCommandPrompt->hCommand/*hCombo*/,
					NULL,
					rcClient.left, rcClient.bottom - H_COMBO, rcClient.right-W_SEND, rcClient.bottom,
					SWP_NOZORDER);

		GetClientRect(lpCommandPrompt->hCommand, &rcCombo);

		SetWindowPos(lpCommandPrompt->hEdit, 
					NULL, 
					rcClient.left, rcClient.top+H_CONN, rcClient.right, /*rcCombo.top*/rcClient.bottom - H_SEND-H_CONN, 
					SWP_NOZORDER);

		break;
	case WM_SETHWND:
		
		switch(wParam)
		{
		case HWND_OWNER:
			lpCommandPrompt->hOwner = (HWND) lParam;
			break;
		default:
			return TRUE;
		}

		return TRUE;
	case WM_CREATE:

		lpCommandPrompt->hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
					WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_SYSMENU, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					hwnd, 
					(HMENU)IDC_CHILD_EDIT, 
					GetModuleHandle(NULL), 
					NULL);

		lpCommandPrompt->hCommand = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
					WS_CHILD | WS_VISIBLE | ES_LEFT | WS_SYSMENU,
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					hwnd, 
					(HMENU)IDC_CHILD_COMMAND, 
					GetModuleHandle(NULL), 
					NULL);

		lpCommandPrompt->hConnect = CreateWindow("BUTTON",
												"Connect",
												WS_TABSTOP|
												WS_VISIBLE|
												WS_CHILD|	
												BS_DEFPUSHBUTTON,
												10, 10, 100, 100,
												hwnd,
												NULL,
												NULL,
												NULL);

		lpCommandPrompt->hSend = CreateWindow("BUTTON",
												"Send",
												WS_TABSTOP|
												WS_VISIBLE|
												WS_CHILD|	
												BS_DEFPUSHBUTTON,
												10, 10, 100, 100,
												hwnd,
												NULL,
												NULL,
												NULL);

		if(lpCommandPrompt->hEdit)
			ShowWindow(lpCommandPrompt->hEdit, SW_SHOW);
		else
			ThrowMessage(hwnd, "Command Prompt: Edit Failed to Create");

		GetClientRect(hwnd, &rcClient);

		lpCommandPrompt->hCombo = CreateWindow(WC_COMBOBOX,
												"",
												CBS_DROPDOWN|
												CBS_HASSTRINGS|
												WS_CHILD|
												WS_OVERLAPPED|
												WS_VISIBLE,
												0,
												rcClient.bottom-H_COMBO,
												rcClient.right-rcClient.left,
												H_COMBO,
												hwnd,
												(HMENU)IDC_CHILD_COMBO,
												GetModuleHandle(0),
												NULL);

		if(lpCommandPrompt->hCombo)
			ShowWindow(lpCommandPrompt->hCombo, SW_HIDE);
		else
			ThrowMessage(hwnd, "Command Prompt: Combo Failed to Create");

		if(lpCommandPrompt->hCommand)
			ShowWindow(lpCommandPrompt->hCommand, SW_SHOW);
		else
			ThrowMessage(hwnd, "Command Prompt: Command Edit Failed to Create");

		//Removes Resizing Flicker
		LONG_PTR lptr;
		lptr = GetWindowLongPtr(hwnd, GWL_STYLE);
		lptr = lptr|WS_CLIPCHILDREN;

		//SetTimer(hwnd, NULL, 1000, NULL);
		
		
		if(!SetWindowLongPtr(hwnd, GWL_STYLE, lptr))
			ThrowMessage(hwnd, "CommandPrompt Error: SetWindowLongPtr(CommandPrompt)!!");

		SetWindowText(lpCommandPrompt->hConnect, "Connect");
		SetWindowText(lpCommandPrompt->hSend, "Send");
		break;
	case WM_NCCREATE:
		lpCommandPrompt = (COMMANDPROMPT *)calloc(1, sizeof(COMMANDPROMPT));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpCommandPrompt);
		if(!lpCommandPrompt){
			MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "CommandPrompt Error!", MB_OK);
		}
		break;
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return TRUE;
	default:
		break;
	}


	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}