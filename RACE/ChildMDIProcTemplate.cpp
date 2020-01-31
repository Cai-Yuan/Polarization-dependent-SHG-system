#include "stdafx.h"

//#define IDC_CHILD_EDIT	0x100

LRESULT CALLBACK MDIChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //switch(msg)
    //{
    //    case WM_CREATE:
    //    {
    //        HFONT hfDefault;
    //        HWND hEdit;

    //        // Create Edit Control

    //        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
    //            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
    //            0, 0, 100, 100, hwnd, (HMENU)IDC_CHILD_EDIT, GetModuleHandle(NULL), NULL);
    //        if(hEdit == NULL)
    //            MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

    //        hfDefault = GetStockObject(DEFAULT_GUI_FONT);
    //        SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
    //    }
    //    break;
    //    case WM_MDIACTIVATE:
    //    {
    //        HMENU hMenu, hFileMenu;
    //        UINT EnableFlag;

    //        hMenu = GetMenu(g_hMainWindow);
    //        if(hwnd == (HWND)lParam)
    //        {      //being activated, enable the menus
    //            EnableFlag = MF_ENABLED;
    //        }
    //        else
    //        {                          //being de-activated, gray the menus
    //            EnableFlag = MF_GRAYED;
    //        }

    //        EnableMenuItem(hMenu, 1, MF_BYPOSITION | EnableFlag);
    //        EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);

    //        hFileMenu = GetSubMenu(hMenu, 0);
    //        EnableMenuItem(hFileMenu, ID_FILE_SAVEAS, MF_BYCOMMAND | EnableFlag);

    //        EnableMenuItem(hFileMenu, ID_FILE_CLOSE, MF_BYCOMMAND | EnableFlag);
    //        EnableMenuItem(hFileMenu, ID_FILE_CLOSEALL, MF_BYCOMMAND | EnableFlag);

    //        DrawMenuBar(g_hMainWindow);
    //    }
    //    break;
    //    case WM_COMMAND:
    //        switch(LOWORD(wParam))
    //        {
    //            case ID_FILE_OPEN:
    //                DoFileOpen(hwnd);
    //            break;
    //            case ID_FILE_SAVEAS:
    //                DoFileSave(hwnd);
    //            break;
    //            case ID_EDIT_CUT:
    //                SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_CUT, 0, 0);
    //            break;
    //            case ID_EDIT_COPY:
    //                SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_COPY, 0, 0);
    //            break;
    //            case ID_EDIT_PASTE:
    //                SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_PASTE, 0, 0);
    //            break;
    //        }
    //    break;
    //    case WM_SIZE:
    //    {
    //        HWND hEdit;
    //        RECT rcClient;

    //        // Calculate remaining height and size edit

    //        GetClientRect(hwnd, &rcClient);

    //        hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
    //        SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
    //    }
    //    return DefMDIChildProc(hwnd, msg, wParam, lParam);
    //    default:
    //        return DefMDIChildProc(hwnd, msg, wParam, lParam);
    //
    //}
    return 0;
}