#pragma once

#include "stdafx.h"

#define TVM_INISERTITEM 

typedef struct TREEVIEWSUBCLASS_{

	WNDPROC wpOriginal;

	HWND hOwner;

}TREEVIEWSUBCLASS;

LRESULT CALLBACK TreeViewSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



