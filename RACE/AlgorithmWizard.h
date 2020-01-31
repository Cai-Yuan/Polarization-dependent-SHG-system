#pragma once

#include "stdafx.h"

#define MAX_TREEITEM 500

typedef struct ALGORITHMWIZARD_{

	HWND hList;
	HWND hTree;
	HWND hOwner;
	HWND hDataBar;
	HWND hButtonAdd;

	HTREEITEM htDrag;
	HTREEITEM hTreeRoot;
	HTREEITEM hTreeItem[MAX_TREEITEM];

}ALGORITHMWIZARD;

LRESULT FillListAlgWizard(HWND hParent, HWND hList);


INT_PTR CALLBACK AlgorithmWizardProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM);