#include "stdafx.h"

#include "TreeViewFxns.h"
#include "StringFxns.h"

DWORD GetStringTreeView(DWORD nCopy, TVITEM *tvItem, CHAR *str)
{
	INT i = NULL;
	INT p = NULL;
	
	if(tvItem->pszText && str){
		
		memset(str, NULL, nCopy*sizeof(CHAR));

		for(i = 0, p = 0; i < nCopy; i++, p+=2){

			if(tvItem->pszText[p] && !tvItem->pszText[p+1])
				str[i] = tvItem->pszText[p];
			else
				break;

		}

	}
		
	return TRUE;
}

DWORD TreeView_SetItemText(HWND hwnd, HTREEITEM hItem, CHAR *str)
{
	TVITEM tvItem = {0};
	INT state = NULL;

	if(!str)
		return FALSE;
	
	tvItem.stateMask = TVIF_HANDLE|TVIF_TEXT/*|TVIF_STATE*/;
	tvItem.mask = tvItem.stateMask;
	tvItem.hItem = hItem;
	tvItem.cchTextMax = MAX_STRING;
	tvItem.pszText = str;
	
	state =  SendMessage(hwnd, TVM_SETITEM, NULL, (LPARAM)&tvItem);
	return state;
	

	return TRUE;
}

DWORD TreeView_GetItemText(HWND hwnd, HTREEITEM hItem, CHAR *str)
{
	TVITEM tvItem = {0};

	if(!str)
		return FALSE;

	tvItem.mask = TVIF_HANDLE|TVIF_TEXT;
	tvItem.hItem = hItem;
	tvItem.cchTextMax = MAX_STRING;
	tvItem.pszText = str;
	
	SendMessage(hwnd, TVM_GETITEM, NULL, (LPARAM) &tvItem);



	return TRUE;
}

HTREEITEM TreeView_AddRootItem(HWND hwnd, UINT nChar, CHAR *str)
{
	HTREEITEM htParent = NULL;
	TVITEM tvItem = {0};
	TVINSERTSTRUCT tvis = {0};

	tvItem.mask = TVIF_TEXT;
	tvItem.cchTextMax = nChar;
	tvItem.pszText = str;

	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	memcpy(&tvis.item, &tvItem, sizeof(TVITEM));
	

	return (HTREEITEM) SendMessage(hwnd, TVM_INSERTITEM, NULL, (LPARAM) &tvis);

}

HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, UINT nChar, CHAR *str)
{

	HTREEITEM htParent = NULL;
	TVITEM tvItem = {0};
	TVINSERTSTRUCT tvis = {0};

	tvItem.mask = TVIF_TEXT;
	tvItem.cchTextMax = nChar;
	tvItem.pszText = str;

	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_LAST;
	memcpy(&tvis.item, &tvItem, sizeof(TVITEM));
	
	return (HTREEITEM) SendMessage(hwnd, TVM_INSERTITEM, NULL, (LPARAM) &tvis);

}

//HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, HTREEITEM htInsertAfter, UINT nChar, CHAR *str)
//{
//
//	HTREEITEM htParent = NULL;
//	TVITEM tvItem = {0};
//	TVINSERTSTRUCT tvis = {0};
//
//	tvItem.mask = TVIF_TEXT;
//	tvItem.cchTextMax = nChar;
//	tvItem.pszText = str;
//
//	tvis.hParent = hParent;
//	tvis.hInsertAfter = htInsertAfter;
//	memcpy(&tvis.item, &tvItem, sizeof(TVITEM));
//	
//	return (HTREEITEM) SendMessage(hwnd, TVM_INSERTITEM, NULL, (LPARAM) &tvis);
//
//}

HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, HTREEITEM hItemAbove, UINT nChar, CHAR *str)
{

	HTREEITEM htParent = NULL;
	TVITEM tvItem = {0};
	TVINSERTSTRUCT tvis = {0};

	tvItem.mask = TVIF_TEXT;
	tvItem.cchTextMax = nChar;
	tvItem.pszText = str;

	tvis.hParent = hParent;
	tvis.hInsertAfter = hItemAbove;
	memcpy(&tvis.item, &tvItem, sizeof(TVITEM));

	return (HTREEITEM) SendMessage(hwnd, TVM_INSERTITEM, NULL, (LPARAM) &tvis);

}

//Adds Child String at Specified Index
//Only handles strings
HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM htParent, DWORD index, UINT nChar, CHAR *str)
{
	DWORD i = NULL;
	DWORD nChild = NULL;

	TVINSERTSTRUCT tvis = {0};
	tvis.hParent = htParent;
	tvis.hInsertAfter = TVI_LAST;

	HTREEITEM htCurrent = NULL;
	STRINGSET ss = {0};


	nChild = TreeView_GetChildCount(hwnd, htParent);
	htCurrent = (HTREEITEM) SendMessage(hwnd, TVM_GETNEXTITEM, (WPARAM) TVGN_CHILD, (LPARAM) htParent);

	if(nChild){

		TVITEM tvItem = {0};
		HTREEITEM htItem;
		HTREEITEM *htChildren = (HTREEITEM *) calloc(nChild, sizeof(HTREEITEM));
		CallocStringSet(nChild, MAX_STRING, &ss);

		//Save All Item Strings
		for(i = 0; i < nChild; i++){
			htChildren[i] = htCurrent;

			tvItem.hItem = htCurrent;
			tvItem.mask = TVIF_TEXT;
			tvItem.pszText = ss.string[i].str;
			tvItem.cchTextMax = MAX_STRING;

			TreeView_GetItem(hwnd, &tvItem);

			htCurrent = (HTREEITEM) SendMessage(hwnd, TVM_GETNEXTITEM, (WPARAM) TVGN_NEXT, (LPARAM) htCurrent);
			
		}

		//Delete all items below 'index'
		for(i = nChild-1; i >= index; i--){

			TreeView_DeleteItem(hwnd, htChildren[i]);

		}

		//Insert New Item
		TreeView_AddChildItem(hwnd, htParent, nChar, str);

		//Append Remaining Items
		for(i = index; i < nChild; i++){
			TreeView_AddChildItem(hwnd, htParent, MAX_STRING, ss.string[i].str);
		}

		FreeStringSet(&ss);
		if(htChildren)free(htChildren);

	}
			

	
	return (HTREEITEM) TRUE;

}

DWORD TreeView_GetParentChildStrings(HWND hwnd, HTREEITEM hParent, STRINGSET *ss)
{

	if(!ss)
		return FALSE;

	DWORD nChild = 1;
	CHAR str[MAX_STRING];
	
	HTREEITEM hChild = TreeView_GetChild(hwnd, hParent);

	TreeView_GetItemText(hwnd, hParent, ss->string[0].str);
	while(hChild)
	{
		TreeView_GetItemText(hwnd, hChild, ss->string[nChild].str);
		hChild = TreeView_GetNextItem(hwnd, hChild, TVGN_NEXT);
		nChild++;
	}

	ss->nStr = nChild;

	return nChild;
}

DWORD TreeView_GetChildStrings(HWND hwnd, HTREEITEM hParent, STRINGSET *ss)
{
	DWORD nChild = NULL;
	CHAR str[MAX_STRING];
	
	HTREEITEM hChild = TreeView_GetChild(hwnd, hParent);

	while(hChild)
	{
		TreeView_GetItemText(hwnd, hChild, ss->string[nChild].str);
		hChild = TreeView_GetNextItem(hwnd, hChild, TVGN_NEXT);
		nChild++;
	}

	return nChild;
}

DWORD TreeView_GetChildCount(HWND hwnd, HTREEITEM hItem)
{

	DWORD nChild = NULL;

	HTREEITEM hChild = TreeView_GetChild(hwnd, hItem);

	while(hChild)
	{
		hChild = TreeView_GetNextItem(hwnd, hChild, TVGN_NEXT);
		nChild++;
	}

	return nChild;
}

DWORD TreeView_GetSelectedIndex(HWND hwnd, HTREEITEM hTreeItem)
{
	DWORD selChild = NULL;

	DWORD i = NULL;
	DWORD nChild = NULL;

	HTREEITEM hRoot = NULL;
	HTREEITEM hParent = NULL;
	HTREEITEM hPrevious = NULL;

	hRoot = TreeView_GetRoot(hwnd);
	hParent = TreeView_GetParent(hwnd, hTreeItem);
	hPrevious = hTreeItem;

	while((hPrevious = (HTREEITEM) SendMessage(hwnd, TVM_GETNEXTITEM, (WPARAM) TVGN_PREVIOUS,(LPARAM) hPrevious)))
	{
		selChild++;
	}


	return selChild;
}

DWORD TreeView_GetSelectedIndex(DWORD listCount, HWND hwnd, HTREEITEM hTreeItem, HTREEITEM *hTreeItemList)
{
	DWORD i = NULL;

	for(i = 0; i < listCount; i++){

		if(hTreeItem == hTreeItemList[i])
			return i;
	}

	return FALSE;
}

DWORD HandleTreeViewDragDrop(HWND hwnd, HWND hTree, WPARAM wParam, LPARAM lParam, HTREEITEM *htiDrag)
{

	NMTREEVIEW *nmtv;
	nmtv = (LPNMTREEVIEW)(lParam);

	if( ((NMHDR *)lParam)->hwndFrom == hTree ){

		switch(nmtv->hdr.code)
		{
		case NM_SETFOCUS:
			{
			INT i;
			i = 0; 
			i = 0;
			}
			break;
		case TVN_SELCHANGEDW:
			INT i;
			i = 0;
			i = 0;
			break;
		case TVN_BEGINDRAGW:
			HIMAGELIST hImgList;
			
			hImgList = TreeView_CreateDragImage(hTree, nmtv->itemNew.hItem);
			ImageList_BeginDrag(hImgList, NULL, NULL, NULL);
			ImageList_DragEnter(hTree, nmtv->ptDrag.x, nmtv->ptDrag.y);
			//ShowCursor(FALSE);
			SetCapture(hwnd);
			*htiDrag = nmtv->itemNew.hItem;
			//*flagDrag = TRUE;

			break;
		default:
			break;
		}
	}


	return TRUE;

}

DWORD HandleTreeViewMouseMove(HWND hwnd, HWND hTree, WPARAM wParam, LPARAM lParam, HTREEITEM *htiDrag)
{
	POINTS pt = {0};
	HTREEITEM htItem = NULL;

	if(*htiDrag){

		pt = MAKEPOINTS(lParam);
		//ImageList_DragMove(pos.x-32, pos.y-25);
		//ImageList_DragShowNoLock(FALSE);

		//Highlight the items at the point of drag
		TVHITTESTINFO	tvht;
		tvht.pt.x = pt.x;-20;
		tvht.pt.y = pt.y-20;
		if( (htItem = (HTREEITEM)SendMessage(hTree, TVM_HITTEST, NULL, (LPARAM) &tvht)) ){

			SendMessage(hTree, TVM_SELECTITEM, (WPARAM) TVGN_DROPHILITE, (LPARAM) htItem);

		}

		//ImageList_DragShowNoLock(TRUE);

	}

	return TRUE;
}

DWORD HandleTreeViewDrop(HWND hwnd, HWND hTree, WPARAM wParam, LPARAM lParam, HTREEITEM *htiDrag)
{
	DWORD iItem = NULL;
	DWORD iDrag = NULL;
	HTREEITEM htItem = NULL;
	HTREEITEM htParent = NULL;
	CHAR *str = (CHAR *) calloc(MAX_STRING, sizeof(CHAR));

	if(*htiDrag){

		ImageList_DragLeave(hTree);
		ImageList_EndDrag();
	
		htItem = (HTREEITEM) SendMessage(hTree, TVM_GETNEXTITEM, TVGN_DROPHILITE, NULL);

		htParent = TreeView_GetParent(hTree, htItem);

		iItem = TreeView_GetSelectedIndex(hTree, htItem);
		iDrag = TreeView_GetSelectedIndex(hTree, *htiDrag);

		TreeView_GetItemText(hTree, *htiDrag, str);

		if(iItem == 0){
			htItem = (HTREEITEM) TreeView_AddChildItem(hTree, htParent, TVI_FIRST, MAX_STRING, str);
		}
		else if(iDrag > iItem){
			htItem = (HTREEITEM) SendMessage(hTree, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM) htItem);		
			htItem = TreeView_AddChildItem(hTree, htParent, htItem, MAX_STRING, str);
		}
		else
			htItem = (HTREEITEM) TreeView_AddChildItem(hTree, htParent, htItem, MAX_STRING, str);

		TreeView_DeleteItem(hTree, *htiDrag);

		SendMessage(hTree, TVM_SELECTITEM, (WPARAM) NULL, (LPARAM) htItem);

		SendMessage(hTree, TVM_SELECTITEM, TVGN_CARET, (LPARAM) htItem);
		SendMessage(hTree, TVM_SELECTITEM, TVGN_DROPHILITE, NULL);

		ReleaseCapture();
		ShowCursor(TRUE);
		*htiDrag = NULL;
	}

	if(str) free(str);

	return TRUE;
}