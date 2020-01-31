#include "stdafx.h"

#include "MDIProc.h"


LRESULT CALLBACK MDISubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	MDI *lpMDI = (MDI *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch(msg)
	{

	case WM_INITSUBCLASS:
		{

			lpMDI = (MDI *)calloc(1, sizeof(MDI));
			SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpMDI);
			if(!lpMDI){
				ThrowMessage(hwnd, (CHAR *) "Error MIDSubclassProc: GetWindowLongPtr Failed");
			}

			lpMDI->wpOriginal = (WNDPROC) wParam;
			lpMDI->hFrame = (HWND) lParam;

			SetFocus(hwnd);
		
			return TRUE;
		}
	case WM_KEYDOWN:
		{
			INT i = 0;

			i = 0;
			i = 0;

			if(wParam == VK_F2){

				MessageBox(hwnd, "F2 Down", "!!!", MB_OK);
				return FALSE;
			}
		}
		break;
	case WM_MENUCHAR:
		{
			UCHAR i;
			i = 0;
			i = 0;

		}
		break;
	case WM_GETMODULEHANDLE:
		return (LRESULT) GetModuleHandle(0);
	default:
		{
			if(lpMDI)
				return CallWindowProc(lpMDI->wpOriginal, hwnd, msg, wParam, lParam);
			else
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	if(lpMDI)
		return CallWindowProc(lpMDI->wpOriginal, hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}
