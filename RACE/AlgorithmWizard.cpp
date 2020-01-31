#include "stdafx.h"
#include "AlgorithmWizard.h"
#include "TreeViewFxns.h"
#include "TreeViewSubclass.h"

		

INT_PTR CALLBACK AlgorithmWizardProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM)
{
	ALGORITHMWIZARD *lpWnd = (ALGORITHMWIZARD *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{
	case WM_INITDIALOG:

		lpWnd = (ALGORITHMWIZARD *)calloc(1, sizeof(ALGORITHMWIZARD));
		memset(lpWnd, 0x00, sizeof(ALGORITHMWIZARD));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpWnd);
		lpWnd = (ALGORITHMWIZARD *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpWnd == NULL){
			ThrowMessage(hwnd, "WndPropertyManager: Error Getting Long Ptr");
			ThrowMessage(hwnd, "WndPropertyManager: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			return TRUE;
		}

		lpWnd->hList = GetDlgItem(hwnd, IDC_LIST);
		lpWnd->hTree = GetDlgItem(hwnd, IDC_TREEALG);
		lpWnd->hButtonAdd = GetDlgItem(hwnd, IDC_BUTTONADD);

		lpWnd->hTreeRoot = TreeView_AddRootItem(lpWnd->hTree, MAX_STRING, "<Algorithm Title>");
		TreeView_Select(lpWnd->hTree, lpWnd->hTreeRoot, TVGN_CARET);

		FillListAlgWizard(hwnd, lpWnd->hList);

		return TRUE;
	case WM_KILLFOCUS:

		if((HWND)wParam == lpWnd->hTree)
			MessageBox(hwnd, "TreeView Lost Focus" , "TreeView", MB_OK);

		return TRUE;
	case WM_COMMAND:
		{

		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			return TRUE;
		case BN_CLICKED:
			
			switch(LOWORD(wParam))
			{
			case IDC_BUTTONADD:
				HWND hTree;
				HTREEITEM hRoot;
				HTREEITEM hSelected;
				LRESULT selList;
				CHAR str[MAX_STRING];

				hTree = lpWnd->hTree;
				hRoot = lpWnd->hTreeRoot;
				memset(str, NULL, MAX_STRING*sizeof(CHAR));

				selList = SendMessage(lpWnd->hList, LB_GETCURSEL, NULL, NULL);
				SendMessage(lpWnd->hList, LB_GETTEXT, (WPARAM) selList, (LPARAM) str);

				if(str[0]){
					hSelected = TreeView_GetSelection(hTree);
					TreeView_AddChildItem(hTree, hRoot, hSelected, MAX_STRING, str);
					TreeView_Expand(lpWnd->hTree, lpWnd->hTreeRoot, TVM_EXPAND);
				}

				return TRUE;
			case (2):
				ShowWindow(hwnd, SW_HIDE);
				return TRUE;
			default:
				return TRUE;
			}
		default:
			return FALSE;
		}

		}
	case WM_NOTIFY:

		return FALSE;
	default:
		return FALSE;

	}

	return FALSE;


}

LRESULT OnNotifyAlgWizard(ALGORITHMWIZARD *lpWnd, HWND hwnd, MSG msg, WPARAM wParam, LPARAM lParam)
{
	CHAR str[260];

	NMHDR *nmhdr;
	NMTREEVIEW *nmtv;
	NMTVDISPINFO *nmtvd;

	nmhdr = (NMHDR *)lParam;
	nmtv = (LPNMTREEVIEW)(lParam);
	nmtvd = (NMTVDISPINFO *)(lParam);

	HTREEITEM hTreeItem;
	NMTVDISPINFO tvdi;

	INT sel = NULL;

	//HandleTreeViewDragDrop(hwnd, lpWnd->hTree, wParam, lParam, &lpWnd->htDrag);

	if( ((NMHDR *)lParam)->hwndFrom == lpWnd->hTree){
		
		switch(nmtv->hdr.code){

			case NM_CLICK:
				TVHITTESTINFO tvht;
				memset(&tvht, NULL, sizeof(tvht));

				GetCursorPos(&tvht.pt);
				MapWindowPoints(HWND_DESKTOP, ((LPNMHDR)lParam)->hwndFrom, &tvht.pt, 1);
				TreeView_HitTest(((LPNMHDR)lParam)->hwndFrom, &tvht);

				if(tvht.flags & TVHT_ONITEMSTATEICON)
				{
					NMHDR nmhdr;
					nmhdr.code = TVN_CHECKSTATECHANGE;
					nmhdr.hwndFrom = ((NMHDR *)lParam)->hwndFrom;
					nmhdr.idFrom = ((NMHDR *)lParam)->idFrom;

					SendMessage(hwnd, WM_NOTIFY, (WPARAM) NULL, (LPARAM)&nmhdr);
				}

				break;
			default:
				break;
		}
	}


	return FALSE;
}

LRESULT FillListAlgWizard(HWND hParent, HWND hList)
{

	ListBox_AddString(hList, "GetFrame([])");
	ListBox_AddString(hList, "GaussianBlur([], 1, 1)");
	ListBox_AddString(hList, "Sobel([], 1)");
	ListBox_AddString(hList, "I'([], 1)");
	ListBox_AddString(hList, "I''([], 1)");
	ListBox_AddString(hList, "Prewitt([], 1)");
	ListBox_AddString(hList, "RGB(1, 1, 1)");


	return TRUE;
}