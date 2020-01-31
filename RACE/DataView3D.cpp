#include "stdafx.h"

#include "DataView3D.h"
//#include "d3d11.h"
//#include "d3dx11.h"
//#include "d3dx10.h"

LRESULT CALLBACK DataView3DProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DATAVIEW3D *lpDataView3D = (DATAVIEW3D *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	CHAR str[MAX_STRING];

	switch(msg){
		case WM_SETHWND:
			if(wParam == HWND_OWNER)
				lpDataView3D->hOwner = (HWND) lParam;

			return TRUE;
		case WM_CREATE:

			InitD3D(hwnd, lpDataView3D);

			break;;
		case WM_NCCREATE:
			{
				lpDataView3D = (DATAVIEW3D *)malloc(sizeof(DATAVIEW3D));
				if(lpDataView3D){
					memset(lpDataView3D, NULL, sizeof(DATAVIEW));
					SetWindowLongPtr(hwnd, (-21), (LONG_PTR) lpDataView3D);
				}
				if(!lpDataView3D)
					MessageBox(hwnd, (LPCSTR) "GetWindowLongPtr Failed", (LPCSTR) "DataView3D Error!", MB_OK);
			}
			break;
		default:
			DefMDIChildProc(hwnd, msg, wParam, lParam);
	}


	return DefMDIChildProc(hwnd, msg, wParam, lParam);
}

HWND CreateMDIDataView3D(HWND hMDI, CHAR *strTitle)
{
	HWND hDataView = NULL;
	BOOL flgFree = NULL;
	CHAR *str = (CHAR *)malloc(MAX_STRING*sizeof(CHAR));

	if(str)
		memset(str, NULL, MAX_STRING*sizeof(CHAR));

	if(!strTitle){
		sprintf(str, "DataView3D");
	}else{
		sprintf(str, "%s", strTitle);
	}

	hDataView = (HWND) SendMessage(hMDI, WM_CREATEMDICHILD, (WPARAM) "DataView3D", (LPARAM) str);
	//hDataView = (HWND) CreateWindow("DataView",
	//								str,
	//								WS_OVERLAPPEDWINDOW,
	//								NULL,
	//								NULL,
	//								200,
	//								200,
	//								hOwner,
	//								NULL,
	//								GetModuleHandle(NULL),
	//								NULL);

	SendMessage(hDataView, WM_MDIACTIVATE, TRUE, (LPARAM) hDataView);

	return (HWND) hDataView;
}

VOID InitD3D(HWND hwnd, DATAVIEW3D *lpDataView3D)
{
	DXGI_SWAP_CHAIN_DESC scd;


	memset(&scd, NULL, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(	NULL,
									D3D_DRIVER_TYPE_HARDWARE,
									NULL,
									NULL,
									NULL,
									NULL,
									D3D11_SDK_VERSION,
									&scd,
									&lpDataView3D->swapChain,
									&lpDataView3D->d,
									NULL,
									&lpDataView3D->dc);

}