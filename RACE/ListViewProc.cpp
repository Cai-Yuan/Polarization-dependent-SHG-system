#include "stdafx.h"

#include "ListView.h"

LRESULT CALLBACK ListViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	LISTVIEW *lpListView = (LISTVIEW *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch(msg)
    {
        case WM_CREATE:
			{
				HFONT hfDefault;
				HWND hListView;
				LONG_PTR lpEdit;

				// Create Edit Control
				hListView = CreateWindow(WC_LISTVIEW, "", 
					WS_CHILD 
					| WS_VISIBLE 
					| WS_VSCROLL 
					| WS_HSCROLL 
					| ES_MULTILINE 
					| LVS_REPORT 
					| LVS_EDITLABELS 
					| WS_SYSMENU
					| LVS_EX_AUTOSIZECOLUMNS
					| LVS_EX_GRIDLINES
					| LVS_EX_FULLROWSELECT
					, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					CW_USEDEFAULT, 
					hwnd, 
					(HMENU)IDC_CHILD_EDIT, 
					GetModuleHandle(NULL), 
					NULL);

				lpListView->hListView = hListView;

				if(hListView == NULL){
					MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
				}

				SendMessage(hListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);

				LVCOLUMN lvColumn;
				memset(&lvColumn, NULL, sizeof(LVCOLUMN));

				lvColumn.mask = LVCF_TEXT|LVCF_SUBITEM|LVCF_WIDTH;
				lvColumn.iSubItem = 0;
				lvColumn.cx = 100;

				lvColumn.pszText = "Particle ID";
				lvColumn.cchTextMax = strlen(lvColumn.pszText);
				ListView_InsertColumn(hListView, NULL, &lvColumn);

				lvColumn.pszText = "Ellipse (a)";
				lvColumn.cchTextMax = strlen(lvColumn.pszText);
				ListView_InsertColumn(hListView, 1, &lvColumn);

				lvColumn.pszText = "Ellipse (b)";
				lvColumn.cchTextMax = strlen(lvColumn.pszText);
				ListView_InsertColumn(hListView, 2, &lvColumn);

				lvColumn.pszText = "Diameter (um)";
				lvColumn.cchTextMax = strlen(lvColumn.pszText);
				ListView_InsertColumn(hListView, 3, &lvColumn);

				lvColumn.pszText = "Velocity (um/s)";
				lvColumn.cchTextMax = strlen(lvColumn.pszText);
				ListView_InsertColumn(hListView, 4, &lvColumn);

				lvColumn.pszText = "Frames";
				lvColumn.cchTextMax = strlen(lvColumn.pszText);
				ListView_InsertColumn(hListView, 5, &lvColumn);

				LVITEM lvItem;
				memset(&lvItem, NULL, sizeof(LVITEM));

				INT index;
				INT p;
				INT i; 
				CHAR str[MAX_STRING];
				for(p = 0; p < 10; p++){

					memset(str, NULL, MAX_STRING*sizeof(CHAR));
					lvItem.mask = LVIF_TEXT;
					lvItem.iSubItem = NULL;

					lvItem.iItem = NULL;

					sprintf(str, "%d", p);
					lvItem.pszText = str;
					lvItem.cchTextMax = strlen(lvItem.pszText);
					index = SendMessage(hListView, LVM_INSERTITEM, 0, (LPARAM) &lvItem);
					if( index == -1){
						
						p = p;
						p = p;
					}
					//ListView_InsertItem(hListView, &lvItem);

					for(i = 1; i < 6; i++){

						memset(str, NULL, MAX_STRING*sizeof(CHAR));
						lvItem.iSubItem = i;
						sprintf(str, "SubItem %d", i);
						lvItem.pszText = str;

						//Both 'ListView_SetItem' and 'LVM_SETITEM' work
						//SendMessage(hListView, LVM_SETITEM, 0, (LPARAM) &lvItem);
						index = ListView_SetItem(hListView, &lvItem);
						if( index == -1){
							i = i;
							i = i;
						}
						
					}
				}



			}
			break;
		case WM_NCCREATE:
			{
				lpListView = (LISTVIEW *)malloc(sizeof(LISTVIEW));
				memset(lpListView, 0x00, sizeof(LISTVIEW));
				SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpListView);
				lpListView = (LISTVIEW *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
				if(lpListView == NULL){
					MessageBox(hwnd, (LPCSTR)"GetWindowLongPtr Failed", (LPCSTR)"Error!", MB_OK);
				}

				return DefMDIChildProc(hwnd, msg, wParam, lParam); 
			}
			break;
		case WM_GETHWND:
			//if(wParam == HWND_EDIT)
			//	return (LRESULT) lpListView->hEdit;
			break;
		case WM_SETHWND:
			if(wParam == HWND_OWNER)
				lpListView->hOwner = (HWND) lParam;
			break;
		case WM_NOTIFY:
			//HandleWM_NOTIFY(lParam);
			break;
		case WM_SIZE:
			{
				HWND hListView = lpListView->hListView;
				RECT rcClient;

				// Calculate remaining height and size edit
				GetClientRect(hwnd, &rcClient);

				SetWindowPos(hListView, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
			}
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
		case WM_CLOSE:
			{
				CHAR str[MAX_STRING];
				GetWindowText(hwnd, str, MAX_STRING);

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