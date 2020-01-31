#include "stdafx.h"

typedef struct MDI_{
	HWND hFrame;
	WNDPROC wpOriginal;
}MDI;

LRESULT CALLBACK MDISubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);