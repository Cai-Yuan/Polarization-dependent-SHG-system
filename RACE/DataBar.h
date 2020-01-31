#pragma once

#include "stdafx.h"
#include "RACE.h"
#include "DataTab.h"
#define WIDTH_SLIDER 40
#define HEIGHT_PROGRESS	25

#define DT_RESETCONTENT	4000	

typedef struct DATABAR_{

	HWND hwnd;
	HWND hBorder;
	HWND hSlider;
	HWND hTabControl;
	HWND hButtonHide;
	HWND hDataView;
	HWND hProgress;
	HWND hTree;
	HWND hFrame;

	WNDPROC wpOriginal;

	BOOL fProgress;
	BOOL fTree;

	INT fStart;
	INT fStop;

	FILEPATHSET fps;

}DATABAR;

LRESULT CALLBACK DataBarSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);