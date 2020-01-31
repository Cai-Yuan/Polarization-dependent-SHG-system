#pragma once

#include "stdafx.h"
#include "StringFxns.h"

#define TVN_CHECKSTATECHANGE	400

//typedef struct NODE{
//	DWORD nNodeChild;
//	NODE *nodeChild;
//	CHAR str[MAX_STRING];
//}NODE;


DWORD GetStringTreeView(DWORD nCopy, TVITEM *tvItem, CHAR *str);
DWORD TreeView_SetItemText(HWND hwnd, HTREEITEM hItem, CHAR *str);
DWORD TreeView_GetItemText(HWND hwnd, HTREEITEM hItem, CHAR *str);
DWORD TreeView_GetChildCount(HWND hwnd, HTREEITEM hItem);
HTREEITEM TreeView_AddRootItem(HWND hwnd, UINT nChar, CHAR *str);
HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, UINT nChar, CHAR *str);
HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, DWORD index, UINT nChar, CHAR *str);
HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, HTREEITEM hItemAbove, UINT nChar, CHAR *str);
HTREEITEM TreeView_AddChildItem(HWND hwnd, HTREEITEM hParent, HTREEITEM htInsertAfter, UINT nChar, CHAR *str);


DWORD TreeView_GetChildStrings(HWND hwnd, HTREEITEM hParent, STRINGSET *ss);
DWORD TreeView_GetParentChildStrings(HWND hwnd, HTREEITEM hParent, STRINGSET *ss);
DWORD TreeView_GetSelectedIndex(HWND hwnd, HTREEITEM hTreeItem);
DWORD TreeView_GetSelectedIndex(DWORD listCount, HWND hwnd, HTREEITEM hTreeItem, HTREEITEM *hTreeItemList);

DWORD HandleTreeViewDragDrop(HWND hwnd, HWND hTree, WPARAM wParam, LPARAM lParam, HTREEITEM *htiDrag);
DWORD HandleTreeViewMouseMove(HWND hwnd, HWND hTree, WPARAM wParam, LPARAM lParam, HTREEITEM *htiDrag);
DWORD HandleTreeViewDrop(HWND hwnd, HWND hTree, WPARAM wParam, LPARAM lParam, HTREEITEM *htiDrag);