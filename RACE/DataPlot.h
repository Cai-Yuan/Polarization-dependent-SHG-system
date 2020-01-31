#pragma once
#include "stdafx.h"

typedef struct DATAPLOT_
{
	HWND hOwner;
	INT wPlot;
	INT hPlot;

}DATAPLOT;

LRESULT OnSizeDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCreateDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCloseDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnDestroyDataPlot(DATAPLOT *lphwnd, HWND hwnd, WPARAM wParam, LPARAM lParam);