#pragma once

#include "stdafx.h"



#include "DataViewDep.h"


typedef struct DATAVIEW3D_{
		HWND hOwner;
		IDXGISwapChain *swapChain;
		ID3D11Device *d;
		ID3D11DeviceContext *dc;

}DATAVIEW3D;

VOID InitD3D(HWND hwnd, DATAVIEW3D *lpDataView3D);
VOID CleanD3d(VOID);

HWND CreateMDIDataView3D(HWND hOwner, CHAR *strTitle);

LRESULT CALLBACK DataView3DProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


