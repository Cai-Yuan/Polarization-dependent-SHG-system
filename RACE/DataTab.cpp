#include "stdafx.h"

#include "DataBar.h"
#include "DataTab.h"
#include "TreeViewFxns.h"
#include "RACE.h"
#include "StringFxns.h"
#include "AlgorithmFxns.h"
//TVN_FIRST = -402;

LRESULT CALLBACK DataTabSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATATAB *lpDataTab = (DATATAB *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{

	case WM_COMMAND:
		{
			OnCommandDataTab(lpDataTab, hwnd, wParam, lParam);
		}
		break;
	case WM_SIZE:
		{
		
			//OnSizeDataTab(hwnd, wParam, lParam);
			INT tab;

			tab = SendMessage(hwnd, TCM_GETCURSEL, NULL, NULL);


			RECT rcClient;
			GetClientRect(hwnd, &rcClient);

			switch(tab)
			{
			case TP_STACK:
				{
					SetWindowPos(lpDataTab->hComboBox, 
								NULL, 
								5, 30, (rcClient.right - rcClient.left)-10, (rcClient.bottom - rcClient.top)-40,
								SWP_NOZORDER);
				}
				break;
			case TP_FILTER:
				{
					SetWindowPos(lpDataTab->hTree,
								NULL,
								5, 30, (rcClient.right-rcClient.left)-10, (rcClient.bottom-rcClient.top)-40,
								SWP_NOZORDER);

				}
				break;
			case TP_DISPLAY:
				{
					SetWindowPos(lpDataTab->hEdit,
								NULL,
								5, 30, (rcClient.right-rcClient.left)-10, rcClient.bottom-rcClient.top-40,
								SWP_NOZORDER);
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_MOUSEMOVE:
			HandleTreeViewMouseMove(hwnd, lpDataTab->hTree, wParam, lParam, &lpDataTab->htDrag);
		break;
	case WM_LBUTTONUP:
			HandleTreeViewDrop(hwnd, lpDataTab->hTree, wParam, lParam, &lpDataTab->htDrag);
		break;
	case WM_NOTIFY:
		{
			OnNotifyDataTab(lpDataTab, hwnd, wParam, lParam);
		}
		break;
	case CB_SETCURSEL:
		{
			SendMessage(lpDataTab->hComboBox, msg, wParam, lParam);
		}
		return TRUE;
	case CB_ADDSTRING:
		{
			SendMessage(lpDataTab->hComboBox, msg, wParam, lParam);
		}
		return TRUE;
	case CB_DELETESTRING:
		{
			LRESULT lResult;
			lResult = SendMessage(lpDataTab->hComboBox, msg, wParam, lParam);
		}
		break;
	case DT_RESETCONTENT:
		{

			UINT i;
			UINT count;
			count = SendMessage(lpDataTab->hComboBox, CB_GETCOUNT, NULL, NULL);

			//Deletes the 0th position item 'count' times
			for(i = 0; i < count; i++)
			{
				SendMessage(lpDataTab->hComboBox, CB_DELETESTRING, NULL, NULL);
			}
		}
		break;
	case WM_INITSUBCLASS:
		{

		InitializeCommonControlsRACE(NULL);

		CHAR str[MAX_STRING];
		sprintf(str, "TVN_SELCHANGED: %d", TVN_SELCHANGED);
		ThrowMessage(hwnd, str);

		lpDataTab = (DATATAB *)calloc(1, sizeof(DATAVIEW));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpDataTab);
		if(!lpDataTab){
			ThrowMessage(hwnd, (CHAR *) "Error DataTabSublcass: GetWindowLongPtr Failed");
		}

		lpDataTab->wpOriginal = (WNDPROC) wParam;
		lpDataTab->hDataBar = (HWND) lParam;

		//Create Subcontrols
		//Create ComboBox to show loaded files
		lpDataTab->hComboBox = CreateWindow(WC_COMBOBOX, 
											TEXT(""),
											WS_VSCROLL|
											CBS_HASSTRINGS|
											WS_CHILD|
											WS_OVERLAPPED|
											WS_VISIBLE|
											CBS_NOINTEGRALHEIGHT/*||
											CBS_DROPDOWN|CBS_DROPDOWNLIST*/,	
											0, 0, 200, 200,
											hwnd,
											NULL,
											GetModuleHandle(NULL),
											NULL);

		SendMessage(lpDataTab->hComboBox, WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FONT), NULL);

		//Create EditBox
		lpDataTab->hEdit = CreateWindow("EDIT",
										"EDIT",
										WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_SYSMENU, 
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										CW_USEDEFAULT,
										hwnd, 
										(HMENU)IDC_CHILD_EDIT,
										GetModuleHandle(NULL),
										NULL);

		//Create TreeView
		lpDataTab->hTree = CreateWindowEx(NULL, 
										WC_TREEVIEW,
										"Tree View",
										WS_VISIBLE | 
										WS_CHILD | 
										WS_BORDER | 
										TVS_TRACKSELECT | 
										TVS_HASLINES | 
										TVS_HASBUTTONS | 
										TVS_LINESATROOT | 
										/*TVS_CHECKBOXES |*/ 
										TVS_EDITLABELS,
										CW_USEDEFAULT, CW_USEDEFAULT,
										CW_USEDEFAULT, CW_USEDEFAULT,
										hwnd,
										(HMENU) IDC_TREE,
										GetModuleHandle(NULL),
										NULL);

		InitializeTreeDataTab(hwnd, lpDataTab->hTree);

		ShowWindow(lpDataTab->hTree, SW_SHOW);
		ShowWindow(lpDataTab->hEdit, SW_SHOW);

		SendMessage(hwnd, WM_SIZE, NULL, NULL);

			return TRUE;
		}
	case WM_SETHWND:

		if(wParam == HWND_ACTIVE)
			lpDataTab->hActive = (HWND) lParam;

		return TRUE;
	default:
		{
			if(lpDataTab)
				return CallWindowProc(lpDataTab->wpOriginal, hwnd, msg, wParam, lParam);
			else
				return DefWindowProc(hwnd, msg, wParam, lParam);;
		}
	}

	if(lpDataTab)
		return CallWindowProc(lpDataTab->wpOriginal, hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);;
}



LRESULT OnCommandDataTab(DATATAB *lpDataTab, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch(HIWORD(wParam))
	{
	case CBN_SELCHANGE:
		{
			SendMessage(lpDataTab->hDataBar, WM_COMMAND, wParam, lParam);
		}
		return TRUE;
	case DT_SELTABCHANGE:
		{
			INT iTab;
			RECT rect;
			GetClientRect(hwnd, &rect);
			iTab = (INT) lParam;	
			if(iTab != TP_STACK)
				ShowWindow(lpDataTab->hComboBox, SW_HIDE);
			else
				ShowWindow(lpDataTab->hComboBox, SW_SHOW);
			
			if(iTab != TP_FILTER)
				ShowWindow(lpDataTab->hTree, SW_HIDE);
			else{
				ShowWindow(lpDataTab->hTree, SW_SHOW);
				SendMessage(hwnd, WM_SIZE, NULL, MAKELPARAM(rect.right-rect.left, rect.bottom-rect.top));
			}

			if(iTab != TP_DISPLAY)
				ShowWindow(lpDataTab->hEdit, SW_HIDE);
			else{
				ShowWindow(lpDataTab->hEdit, SW_SHOW);
				SendMessage(hwnd, WM_SIZE, NULL, MAKELPARAM(rect.right-rect.left, rect.bottom-rect.top));
			}

		}
		break;
	default:
		break;
	}
	return TRUE;
}

//**NOTE -- The Treeview is sending UNICODE WM_NOTIFY actions
//**Must add a 'W' to the end of TVN_XXX Messages  Ex. TVN_XXXW
LRESULT OnNotifyDataTab(DATATAB *lpDataTab, HWND hwnd, WPARAM wParam, LPARAM lParam)
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

	HandleTreeViewDragDrop(hwnd, lpDataTab->hTree, wParam, lParam, &lpDataTab->htDrag);

	if( ((NMHDR *)lParam)->hwndFrom == lpDataTab->hTree){

		switch(nmtv->hdr.code){

			case NM_SETFOCUS:
				break;
			case NM_KILLFOCUS:
				break;
			case NM_RCLICK:
				break;
			case NM_CLICK:
				
				TVHITTESTINFO tvht;
				memset(&tvht, NULL, sizeof(TVHITTESTINFO));

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
			case NM_RETURN:
				break;
			case NM_DBLCLK:
				break;	
			case TVN_CHECKSTATECHANGE:
				break;
			case TVN_BEGINLABELEDITW:
				break;
			case TVN_ENDLABELEDITW:
				TVITEM tvItem;
				nmtvd = (NMTVDISPINFO *)lParam;

				memset(&tvItem, NULL, sizeof(TVITEM));

				if(nmtvd->item.pszText){
					GetStringTreeView(nmtvd->item.cchTextMax, &nmtvd->item, str);
					SendMessage(lpDataTab->hActive, WM_COMMAND, DVN_SETALGORITHMSTRING, (LPARAM) str);

					if(!TreeView_SetItemText(lpDataTab->hTree, nmtvd->item.hItem, str))
						ThrowMessage(hwnd, "TreeView_SetItem: Failed");

					STRINGSET strSet;
					DWORD selProc;
					memset((VOID *)&strSet, NULL, sizeof(STRINGSET));
					GetAlgorithmProcedures(lpDataTab->hTree, nmtvd->item.hItem, &selProc, &strSet);
					SendMessage(lpDataTab->hActive, DF_INITFILTER, (WPARAM) selProc, (LPARAM)&strSet);
					FreeStringSet(&strSet);
				}
				
				return TRUE;
			case TVN_ITEMCHANGEDW:
				break;
			case TVN_SELCHANGINGW:
				break;
			case TVN_SELCHANGEDW:
				nmtv = (NMTREEVIEW *)(lParam);

				DWORD nChild;
				DWORD selProc;
				FILEPATHSET fps;
				STRINGSET strSet;
				STRINGSET strCpy;

				memset((VOID *)&strSet, NULL, sizeof(STRINGSET));
				memset((VOID *)&strCpy, NULL, sizeof(STRINGSET));

				GetAlgorithmProcedures(lpDataTab->hTree, nmtv->itemNew.hItem, &selProc,  &strSet);
				TreeView_GetItemText(lpDataTab->hTree, nmtv->itemNew.hItem, str);

				if(selProc == 0)
					selProc = (strSet.nStr-1);
				
				SendMessage(lpDataTab->hActive, DF_INITFILTER, (WPARAM) selProc, (LPARAM) &strSet);
				FreeStringSet(&strSet);

				break;
			case TVN_DELETEITEMW:
				break;
			case TVN_GETDISPINFOW:
				break;
			case TVN_BEGINDRAGW:

				break;
			default:
				break;	
		}
	}


	return TRUE;
}



DWORD InitializeTreeDataTab(HWND hwnd, HWND hTree)
{
	HTREEITEM hParent = NULL;

	TreeView_AddRootItem(hTree, MAX_STRING, "Grayscale");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Library");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]CONST(255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]LoadFrame(F)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]1stGradient(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]2ndGradient(0, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Sobel(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]CentralDifference(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Stretch(5, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]BooleanAND(3, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]NonMaximum(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]Threshold(0, 2)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[13]Subtract(2, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[14]ThresholdConst(13, 125)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[15]FloodFill(14)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[16]RGB(1, 2, 0)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Template");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[13]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[14]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[15]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[16]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[17]MIP(0, 1, 2, 3)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Hessian");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]HessianDxx(1, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]HessianDyy(1, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]HessianDxy(1, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]HessianDyx(1, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Add(4, 6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Add(5, 7)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]ZeroCrossing(8, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]AND(6,7)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]MIP(0, 8, 6, 9)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]RGB(2, 3, 4)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Invert Grayscale");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]Subtract(1, 0)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Contrast");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Contrast(0, 50)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Invert Contrast");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]Subtract(1, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Contrast(2, 100)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Normalized");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Normalize(0)");
	
	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Sobel of Laplace");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Sobel(3, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Stretch(4, 0, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]MIP(0, 1, 5, 1)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Laplacian Division");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Contrast(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(1, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Divide(4,3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Stretch(5, 0, 255)"); 
	//TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]MIP(0, 1, 5, 1)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Chamfer");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]Contrast(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]GaussianBlur(2, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 8)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Window(4, 75, 500)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]PavlidisContour(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Chamfer(6)");

	//TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]MIP(0, 1, 5, 1)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Depth Detection 1");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 20)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Invert(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]MIP(0, 8, 6, 5)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Depth Detection 2");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 20)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Invert(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]MIP(0, 7, 6, 5)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Depth Detection 3");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 20)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Invert(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Threshold(6, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]Threshold(5, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]Threshold(8, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]Threshold(6, 10)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[13]MIP(0, 11, 12, 10)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Depth Detection 4");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 20)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Invert(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Threshold(6, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]Threshold(5, 9)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]Threshold(8, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]PavlidisContour(9)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[13]MIP(0, 12, 9, 10)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Depth Detection 5");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.25)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 20)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Invert(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]MIP(0, 8, 6, 5)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Depth Detection 6");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Contrast(0, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(1, 3, 1.25)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 20)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Invert(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]MIP(0, 8, 6, 5)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Cell Detection");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Contrast(0, 150)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(1, 3, 1.25)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]InverseLaplacian(2, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]InverseLaplacian(2, 7)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Window(3, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Window(4, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Add(5, 6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Stretch(7, 0, 500)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Threshold(8, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]PavlidisContour(9)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]MIP(1, 10, 11, 11)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Thin Edge");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]Contrast(0, 150)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]GaussianBlur(2, 3, 1.25)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(3, 9)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Sobel(3, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Divide(4, 5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Threshold(6, 1)");


	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Morphological Edge");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]Dilate(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Erode(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Subtract(2, 3");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Laplace Scale-Space");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Laplacian(2, 5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Laplacian(2, 7)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Laplacian(2, 9)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Add(3, 4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Add(8, 5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]Add(9, 6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]Add(10, 7)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]Stretch(11, 0, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[13]RGB(3, 5, 7)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[14]MIP(0, 1, 5, 1)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Laplace Orientation");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Inverse(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]LaplacianAngle(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Inverse(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Stretch(5, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]RGB(3, 0, 7)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Gradient Orientation");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GradientAngle(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Degrees(2)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Inverse(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Window(3, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Window(4, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Stretch(5, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]RGB(5, 0, 6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]RGB(3, 0, 7)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Laplace Orientation 2");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]LaplacianAngle(0, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Degrees(2)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Inverse(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Window(3, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Window(4, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Stretch(5, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]RGB(5, 0, 6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[12]RGB(3, 0, 7)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Laplace of Sobel");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Sobel(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacican(3, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Stretch(4, 0, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]MIP(0, 1, 5, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Cell Contour");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]CONST(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2,5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]NonMaximum(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Threshold(4, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]MIP(0, 3, 1, 5)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Sarcomere Detector (ACM)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Stretch(0, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(1, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 16)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Laplacian(2, 32)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Add(3,4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Invert(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]NonMaximum(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Window(6, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Stretch(8, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]Window(5, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[11]RGB(7, 10, 9)");
	//TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]RGB(3, 1, 4)"); 
	
	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Edge Detector (ACM)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Invert(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Stretch(4, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Stretch(3, 0, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]RGB(3, 1, 4)"); 

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Edge Detector (Sobel)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Sobel(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]NonMaximum(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Stretch(4, 0, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]MIP(0, 1, 5, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Edge Detector (Laplacian)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Window(3, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]NonMaximum(4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Stretch(5, 0, 500)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]MIP(0, 1, 6, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Edge Detector (Sobel-Laplacian)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Window(3, 0, 500)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Sobel(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]RGB(4, 5, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "NonMaximal Edge Detector (Sobel-Laplacian)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0 )");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Sobel(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]NonMaximumExp(3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]NonMaximumExp(4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]RGB(5, 6, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Fluorescence Detector");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GetFrame(1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]2ndGradient(3, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Sobel(3, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]NonMaximum(5)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]Stretch(6, 0, 6000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]Stretch(0, 0, 500)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[9]Stretch(2, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[10]RGB(8, 7, 2)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "2D Model(Sobel)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Sobel(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Window(3, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Square(4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Window(5, 50, 20000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]PavlidisContour(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]MIP(0, 1, 7, 1)");



	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "2D Model(Laplacian)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Window(3, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Square(4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Window(5, 50, 20000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]PavlidisContour(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]MIP(0, 1, 7, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "2D Dilate Model(Sobel)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Sobel(2, 1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Dilate(3,2)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]Erode(4, 2)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]Window(5, 0, 255)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[7]PavlidisContour(6)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]MIP(0, 1, 7, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "2D Model(Axon)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]NULL()");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]GaussianBlur(0, 3, 1.0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Laplacian(2, 3)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Window(3, 30, 200)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]PavlidisContour(4)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[6]RGB(0, 5, 0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[8]MIP(0, 1, 5, 1)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Particle Track (Dilation)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Window(0, 0, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]Square(1)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Dilate(2)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]Window(3, 50, 50000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[5]PavlidisContour(4)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Chan-Vese Segmentation");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Square(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]DoubleThreshold(1, 50, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Window(2, 50, 50000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]PavlidisContour(3)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Particle Track (Double Thresh)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Square(0)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]DoubleThreshold(1, 50, 100)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[3]Window(2, 50, 50000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[4]PavlidisContour(3)");

	hParent = TreeView_AddRootItem(hTree, MAX_STRING, "Particle Track");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[1]Window(0, 5, 1000)");
	TreeView_AddChildItem(hTree, hParent, MAX_STRING, "[2]PavlidisContour(1)");
	SendMessage(hTree, TVM_SETBKCOLOR, NULL, RGB(200, 200, 200));

	return TRUE;
}