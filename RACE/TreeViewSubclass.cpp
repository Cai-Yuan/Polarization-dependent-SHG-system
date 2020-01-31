#include "stdafx.h"

#include "DataManager.h"
#include "TreeViewSubclass.h"
#include "TreeViewFxns.h"

LRESULT CALLBACK TreeViewSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TREEVIEWSUBCLASS *lpTreeView = (TREEVIEWSUBCLASS *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_NOTIFY:
		{
			NMTREEVIEW *nmtv;
			nmtv = (LPNMTREEVIEW)(lParam);

			if( ((NMHDR *)lParam)->hwndFrom == hwnd){

				switch(nmtv->hdr.code)
				{
				case NM_SETFOCUS:
					INT i;
					i = 0;
					i = 0;
					break;
				
				default:
					break;
				}
			}

		}
		break;
	case WM_INITSUBCLASS:
		{

			lpTreeView = (TREEVIEWSUBCLASS *) calloc(1, sizeof(TREEVIEWSUBCLASS));
			SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lpTreeView);
			if(!lpTreeView){
				ThrowMessage(hwnd, (CHAR *) "Error TreeViewSubClass: SetWindowLongPtr Failed");
			}

			lpTreeView->wpOriginal = (WNDPROC) wParam;

		}
		return TRUE;
	default:
		return CallWindowProc(lpTreeView->wpOriginal, hwnd, msg, wParam, lParam);
	}

	return CallWindowProc(lpTreeView->wpOriginal, hwnd, msg, wParam, lParam);
}