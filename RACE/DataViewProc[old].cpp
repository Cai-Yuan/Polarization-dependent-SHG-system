#include "stdafx.h"


#include "DataView[old].h"


LRESULT CALLBACK MDIChildImageViewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPTRIMAGEVIEW *lpImageView = (LPTRIMAGEVIEW *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

	static HBITMAP hbmp;
	static CHAR cWndTitle[MAX_STRING];
	UINT		i;
	UINT		w;
	UINT		h;
	UINT		dispState;
	UINT		vScrollMax;
	UINT		hScrollMax;
	UINT		vScrollPos;
	UINT		hScrollPos;
	UINT		typeMask;
	HMENU		hMenu;
	HMENU		hPopUpMenu;
	SCROLLINFO	vScrollInfo;
	SCROLLINFO	hScrollInfo;
	RECT		rcClient;
	INT			wClient;
	INT			hClient;
	HWND		hFrameSlider;
	INT			iFrame;
	INT			iImage;
	UINT		iWnd;
	UINT		iFrameAdd;
	UINT		nFramesMax;
	POINT		ptCurGrab;
	TOOL		tool;
	
	if(lpImageView != NULL){
		w = lpImageView->w;
		h = lpImageView->h;
		lpImageView->hwnd = hwnd;
		dispState = lpImageView->dispState;
		vScrollMax = lpImageView->vScrollMax;
		hScrollMax = lpImageView->hScrollMax;
		vScrollPos = lpImageView->vScrollPos;
		hScrollPos = lpImageView->hScrollPos;
		typeMask = lpImageView->typeMask;
		hMenu = lpImageView->hMenu;
		hPopUpMenu = lpImageView->hPopUpMenu;
		vScrollInfo = lpImageView->vScrollInfo;
		hScrollInfo = lpImageView->hScrollInfo;
		rcClient = lpImageView->rcClient;
		wClient = lpImageView->wClient;
		hClient = lpImageView->hClient;
		hFrameSlider = lpImageView->hFrameSlider;
		iFrame = lpImageView->iFrame;
		iImage = lpImageView->iImage;
		iWnd = lpImageView->iWnd;
		iFrameAdd = lpImageView->iFrameAdd;
		nFramesMax = lpImageView->nFramesMax;
		tool = lpImageView->tool;

		RECT rc;
		GetWindowRect(hwnd, &rc);
		lpImageView->xWin = rc.left;
		lpImageView->yWin = rc.top;
		lpImageView->wWin = rc.right;
		lpImageView->hWin = rc.bottom;

		ptCurGrab = lpImageView->ptCurGrab;
	}
	else{
		//memset(lpImageView, 0x00, sizeof(LPTRIMAGEVIEW));
		w = NULL;
		h = NULL;
		dispState = NULL;
		vScrollMax = NULL;
		hScrollMax = NULL;
		vScrollPos = NULL;
		hScrollPos = NULL;
		typeMask = NULL;
		hMenu = NULL;
		hPopUpMenu = NULL;
		memset(&vScrollInfo, 0x00, sizeof(SCROLLINFO));
		memset(&hScrollInfo, 0x00, sizeof(SCROLLINFO));
		memset(&rcClient, 0x00, sizeof(RECT));
		memset(&ptCurGrab, 0x00, sizeof(POINT));
		wClient = NULL;
		hClient = NULL;
		hFrameSlider = NULL;
		iFrame = NULL;
		iImage = NULL;
		iFrameAdd = NULL;
		nFramesMax = NULL;
		tool = NULL;

		
	}

	HMENU hFileMenu, hSubMenu;
	
	
	HDC hdc = NULL;
	PAINTSTRUCT ps;

	UINT length = 0;
	LONG_PTR lp;

	INT hPos = 0;
	INT vPos = 0;
	INT bottom = 0;
	INT right = 0;

	INT vMin, vMax;
	INT hMin, hMax;

	INT rMin, rMax;
	INT xOff, yOff;

	
	CHAR curVal[256];
	CHAR curPos[256];
	CHAR frmPos[256];
	CHAR frmTim[256];
	CHAR str[256];

	UINT typIdentifier;



    switch(msg)
	{
	case WM_PAINT:
		{
			HDC hdc;
			RECT rect;
			hdc = BeginPaint(hwnd, &ps);
			hPos = hScrollInfo.nPos;
			vPos = vScrollInfo.nPos;
			bottom = rcClient.bottom;
			right = rcClient.right;
			if(lpImageView){
				if(lpImageView->hdcMem){		
					hdc = GetDC(hwnd);
					GetClientRect(hwnd, &rect);
					BitBlt(hdc, -hPos, -vPos, right+hPos, bottom+vPos, lpImageView->hdcMem, 0, 0, SRCCOPY);
					SendMessage(hFrameSlider, WM_PAINT, NULL, NULL);
					SendMessage(hFrameSlider, WM_NCPAINT, NULL, NULL);
				}
			}
			EndPaint(hwnd, &ps);
			SendMessage(hwnd, WM_NCPAINT, NULL, NULL);
			
		}
		break;
	case WM_DISPLAYIMG:
		if(lpImageView->nFramesAdded <= lpImageView->nFramesMax){

			CHAR typeData;
			CHAR typeMetaData;
			//PDMFRAME pdmFrame;
			//GetFilePathPDM(lpImageView->iWnd, lpImageView->iFrameAdd, pdmFrameHeader->typeData, (CHAR *)lpImageView->strDirectory, (CHAR *)lpImageView->strPath);
			//ReadPDMFrame(&typeData, &typeMetaData, &pdmFrame, lpImageView->strPath, lpImageView->lpvoid);

			switch(dispState)
			{
			case TI_CHAR:
				if(lpImageView->imgCHAR){
					if(nFramesMax > 1){
						if(lpImageView->nFramesMax < lpImageView->iFrameAdd){
							GetArchiveRawTYPE(lpImageView->iWnd, iFrame, dispState, w*h*sizeof(CHAR),(LPVOID *)lpImageView->imgCHAR);

						}
					}
					DrawChannels(lpImageView);
					CHARtoPIXEL(w, h, lpImageView->imgCHAR, lpImageView->dispPIXEL);
					//if(lpImageView->tool = TL_POLYGON)
					//DrawChannels(lpImageView);
					}
			break;
			case TI_UCHAR:
				if(lpImageView->imgUCHAR){
					if(nFramesMax > 1){
						if(lpImageView->nFramesMax < lpImageView->iFrameAdd){
							GetArchiveRawTYPE(lpImageView->iWnd, iFrame, dispState, w*h*sizeof(UCHAR),(LPVOID *)lpImageView->imgUCHAR);
						}
					}
					DrawChannels(lpImageView);
					UCHARtoPIXEL(w, h, lpImageView->imgUCHAR, lpImageView->dispPIXEL);
					//if(lpImageView->tool = TL_POLYGON)
					//DrawChannels(lpImageView);
					}
			break;
			case TI_INT:
				if(lpImageView->imgINT){
					if(nFramesMax > 1){
						if(lpImageView->nFramesMax < lpImageView->iFrameAdd){
							GetArchiveRawTYPE(lpImageView->iWnd, iFrame, dispState, w*h*sizeof(INT),(LPVOID *)lpImageView->imgINT);
						}
					}
					DrawChannels(lpImageView);
					INTtoPIXEL(w, h, lpImageView->imgINT, lpImageView->dispPIXEL);
					///if(lpImageView->tool = TL_POLYGON)
					//DrawChannels(lpImageView);
					}
			break;
			case TI_UINT:
				if(lpImageView->imgUINT){
					if(nFramesMax > 1){
						if(lpImageView->nFramesMax < lpImageView->iFrameAdd){
							GetArchiveRawTYPE(lpImageView->iWnd, iFrame, dispState, w*h*sizeof(UINT),(LPVOID *)lpImageView->imgUINT);
						}
					}
					DrawChannels(lpImageView);
					UINTtoPIXEL(w, h, lpImageView->imgUINT, lpImageView->dispPIXEL);
					//if(lpImageView->tool = TL_POLYGON)
					//DrawChannels(lpImageView);
				}
			break;
			case TI_PIXEL:
				if(lpImageView->imgPIXEL){
					if(nFramesMax > 1){
						if(lpImageView->nFramesMax < lpImageView->iFrameAdd){
							GetArchiveRawTYPE(lpImageView->iWnd, iFrame, dispState, w*h*sizeof(PIXEL),(LPVOID *)lpImageView->imgPIXEL);
						}
					}
					memcpy(lpImageView->dispPIXEL, lpImageView->imgPIXEL, w*h*sizeof(PIXEL));
					//if(lpImageView->tool = TL_POLYGON)
					DrawChannels(lpImageView);
				}
			break;
			case TI_FLOAT:
				if(lpImageView->imgFLOAT){
					if(nFramesMax > 1){
						if(lpImageView->nFramesMax < lpImageView->iFrameAdd){
							GetArchiveRawTYPE(lpImageView->iWnd, iFrame, dispState, w*h*sizeof(FLOAT), (LPVOID *)lpImageView->imgFLOAT);
						}
					}
					FLOATtoPIXEL(w, h, lpImageView->imgFLOAT, lpImageView->dispPIXEL);
				}
			break;
			default:
				break;
			}
		}
		//PostMessage(hwnd, WM_PAINT, NULL, NULL);
		SendMessage(hwnd, WM_PAINT, NULL, NULL);
		return 0;

	/********************************************UPDATE********************************************/
	/********************************************6/12/12*******************************************/
	case WM_SHOWFRAMEDATA:
		if(lpImageView->nFramesAdded <= lpImageView->nFramesMax){

			CHAR typeData;
			CHAR typeMetaData;
			PDMFRAME pdmFrame;

			if(lpImageView->statusSlider == TS_FLOATING){
				GetFilePathPDM(lpImageView->iWnd, lpImageView->iFrame, lpImageView->dataDisplay, (CHAR *)lpImageView->strDirectory, (CHAR *)lpImageView->strPath);
				if(!ReadPDMFrame(&typeData, &typeMetaData, &pdmFrame, (CHAR *) lpImageView->strPath, &lpImageView->lpvoid))
				{
					INT in;
					in = 0;
					return TRUE;
				}
					
			}
			

			switch(lpImageView->dataDisplay)
			{
			case DT_CHAR:
				if(lpImageView->imgCHAR){
					if(lpImageView->lpvoid){
						memcpy(lpImageView->imgCHAR, lpImageView->lpvoid, w*h*sizeof(CHAR));
					}
					CHARtoPIXEL(w, h, lpImageView->imgCHAR, lpImageView->dispPIXEL);
				}
			break;
			case DT_UCHAR:
				if(lpImageView->imgUCHAR){
					if(lpImageView->lpvoid){
						memcpy(lpImageView->imgUCHAR, lpImageView->lpvoid, w*h*sizeof(UCHAR));
					}
					UCHARtoPIXEL(w, h, lpImageView->imgUCHAR, lpImageView->dispPIXEL);
					
				}
			break;

			//lpImageView->typeMask = typeMask|TM_INT;


			case DT_INT:
				if(lpImageView->imgINT){
					if(lpImageView->lpvoid){
						memcpy(lpImageView->imgINT, lpImageView->lpvoid, w*h*sizeof(INT));
					}
					INTtoPIXEL(w, h, lpImageView->imgINT, lpImageView->dispPIXEL);
					
				}
			break;
			case DT_UINT:
				if(lpImageView->imgUINT){
					if(lpImageView->lpvoid){
						memcpy(lpImageView->imgUINT, lpImageView->lpvoid, w*h*sizeof(UINT));
					}
					UINTtoPIXEL(w, h, lpImageView->imgUINT, lpImageView->dispPIXEL);
					
				}
			break;
			case DT_PIXEL:
				if(lpImageView->imgPIXEL && lpImageView->lpvoid){
					if(lpImageView->lpvoid){
						memcpy(lpImageView->dispPIXEL, lpImageView->lpvoid, w*h*sizeof(PIXEL));
					}
				}
			break;
			case DT_FLOAT:
				if(lpImageView->imgFLOAT && lpImageView->lpvoid){
					if(lpImageView->lpvoid){
						memcpy(lpImageView->imgFLOAT, lpImageView->lpvoid, w*h*sizeof(FLOAT));
					}
					FLOATtoPIXEL(w, h, lpImageView->imgFLOAT, lpImageView->dispPIXEL);
				}
			break;
			default:
				break;
			}


		DrawChannels(lpImageView);

		//PostMessage(hwnd, WM_PAINT, NULL, NULL);
		SendMessage(hwnd, WM_PAINT, NULL, NULL);

		}
		return 0;
	case WM_GETHDC:
		if(lpImageView->hdcMem)
			return (LRESULT) lpImageView->hdcMem;
		else
			return 0;
		break;
	case WM_SIZE:
		//UINT w, h;
		UINT hOff;
		UINT vOff;
		GetClientRect(hwnd, &lpImageView->rcClient);
		SendMessage(g_hStatusBar, WM_SIZE, NULL, NULL);
		
		SendMessage(lpImageView->hFrameSlider, WM_SIZE, NULL, NULL);

		if(wParam == SIZE_MAXIMIZED){
			SetMDIChildClientArea(g_hMDIClient, hwnd, 0, 0, w, h, SW_SHOW/*NORMAL*/);
			SetScrollPos(hwnd, SB_HORZ, 0, TRUE);
			SetScrollPos(hwnd, SB_VERT, 0, TRUE);
			lpImageView->vScrollInfo.nPos = 0;
			lpImageView->hScrollInfo.nPos = 0;
			return 0;
		}

		if(lpImageView){

			RECT rect;
			w = lpImageView->rcClient.right - lpImageView->rcClient.left;
			h = lpImageView->rcClient.bottom - lpImageView->rcClient.top;
			GetWindowRect(hwnd, &rect);

			if(w > lpImageView->w){
				rect.right += lpImageView->w - w;
			}
			if(h > lpImageView->h){
				rect.bottom += lpImageView->h - h;
			}

		}


		SendMessage(hwnd, WM_UPDATESCROLL, NULL, NULL);

		return DefMDIChildProc(hwnd, msg, wParam, lParam);
		break;
    case WM_COMMAND:
		INT p;
		p = 0;
		INT loWord;
		INT hiWord;
		loWord = LOWORD(wParam);
		hiWord = HIWORD(wParam);					
		
		if(wParam == SC_RESTORE){
			SendMessage(hwnd, WM_SIZE, SIZE_RESTORED, LOWORD(400)|HIWORD(400));
		}
        switch(LOWORD(wParam))
        {
            case ID_FILE_OPEN:
                //DoFileOpen(hwnd);
				break;
            case ID_FILE_SAVEAS:
				//SaveFileTYPE();
				switch(dispState)
				{
				case TI_CHAR:
					SaveFileTYPE(w, h, dispState, (LPVOID *) lpImageView->imgCHAR);
					break;
				case TI_UCHAR:
					SaveFileTYPE(w, h, dispState, (LPVOID *) lpImageView->imgUCHAR);
					break;
				case TI_INT:
					SaveFileTYPE(w, h, dispState, (LPVOID *) lpImageView->imgINT);
					break;
				case TI_UINT:
					SaveFileTYPE(w, h, dispState, (LPVOID *) lpImageView->imgUINT);
					break;
				case TI_PIXEL:
					SaveFileTYPE(w, h, dispState, (LPVOID *) lpImageView->imgPIXEL);
					break;
				case TI_FLOAT:
					SaveFileTYPE(w, h, dispState, (LPVOID *) lpImageView->imgFLOAT);
					break;
				default:
					break;
				}
                //DoFileSave(hwnd);
				break;
			case ID_FILE_NEW:
				//CreateNewMDIChild(g_hMDIClient, "MDIChild");
				break;
            case ID_EDIT_CUT:
                SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_CUT, 0, 0);
				break;
            case ID_EDIT_COPY:
                SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_COPY, 0, 0);
				break;
            case ID_EDIT_PASTE:
                SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_PASTE, 0, 0);
				break;
			case ID_EXPORT_BMP:
				SaveBMP(w, h, lpImageView->dispPIXEL);
				break;
			case ID_TOOL_CURSOR:
				lpImageView->tool = TL_CURSOR;
				break;
			case ID_TOOL_POLYGON:
				if((MessageBox(hwnd, "Make a new polygon?", "Careful!", MB_OKCANCEL)) == IDOK){
					lpImageView->tool = TL_POLYGON;
					lpImageView->polygon->nPoint = 0;
					lpImageView->polygon->nVertex = 0;
					lpImageView->ptFill.nPoints = 0;
				}
				break;
			case ID_TOOL_FILL:
				if((MessageBox(hwnd, "Make a new fill pattern?", "Careful!", MB_OKCANCEL)) == IDOK){
					lpImageView->tool = TL_FILL;
					lpImageView->ptFill.nPoints = 0;
					SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
				}
				break;

		}
		break;
	case WM_SYSCOMMAND:
		WPARAM	wparam;
		wparam = wParam&0xFFF0;
		switch(wParam)
		{
			case ID_SHOWCHAR:
				SendMessage(hwnd, WM_SETSHOWTYPE, TI_CHAR, NULL);
				break;
			case ID_SHOWUCHAR:
				SendMessage(hwnd, WM_SETSHOWTYPE, TI_UCHAR, NULL);
				break;
			case ID_SHOWINT:
				SendMessage(hwnd, WM_SETSHOWTYPE, TI_INT, NULL);
				break;
			case ID_SHOWPIXEL:
				SendMessage(hwnd, WM_SETSHOWTYPE, TI_PIXEL, NULL);
				break;
			case ID_SHOWFLOAT:
				SendMessage(hwnd, WM_SETSHOWTYPE, TI_FLOAT, NULL);
				break;
			case SC_MAXIMIZE:
				RECT rc;
				GetWindowRect(hwnd, &rc);
				POINT ptCursor;
				if(lpImageView->SWMODE == DS_RESTORED){
					lpImageView->SWMODE = DS_FDATAWH;


					lpImageView->xWin = rc.left;
					lpImageView->yWin = rc.top;
					lpImageView->wWin = rc.right;
					lpImageView->hWin = rc.bottom;

					//Set Window Pos to Full Screen;
					UINT wScreen;
					UINT hScreen;
					RECT rcDesktop;
					SendMessage(hwnd, WM_SIZE, (WPARAM) SIZE_MAXIMIZED, NULL);
					GetWindowRect(GetDesktopWindow(), &rcDesktop);
					wScreen = rcDesktop.right;
					hScreen = rcDesktop.bottom;
					hScreen = hScreen - GetTaskBarHeight();

					GetCursorPos(&ptCursor);
					if(ptCursor.x > (wScreen - GetSystemMetrics(SM_CXBORDER))){
						wScreen = wScreen;
						SetMDIChildClientArea(g_hMDIClient, hwnd, wScreen - GetSystemMetrics(SM_CXBORDER), 0, w, h, SW_SHOW/*NORMAL*/);
					}
					SendMessage(hwnd, WM_UPDATESCROLL, NULL, NULL);
					
				}
				else if(lpImageView->SWMODE == DS_FDATAWH){
					lpImageView->SWMODE = DS_RESTORED;
					
					MoveWindow(hwnd, lpImageView->xWin, lpImageView->yWin, lpImageView->wWin, lpImageView->hWin, TRUE);
					ShowWindow(hwnd, SW_SHOW);
				}

				return 0;
			default:
				return DefMDIChildProc(hwnd, msg, wParam, lParam);
		}
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
		break;
	case WM_SETSHOWTYPE:	//SETTYPEDISP
		
		if(lpImageView){

			lpImageView->dispState = wParam;
			SetMenuItem(lpImageView->hMenu, dispState, MF_ENABLED|MF_UNCHECKED);
			dispState = lpImageView->dispState;
			SetMenuItem(lpImageView->hMenu, dispState, MF_ENABLED|MF_CHECKED);
			//SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
			

			switch(lpImageView->dispState)
			{
			case TI_CHAR:
				lpImageView->dataDisplay = DT_CHAR;
			break;
			case TI_UCHAR:
				lpImageView->dataDisplay = DT_UCHAR;
			break;
			case TI_INT:
				lpImageView->dataDisplay = DT_INT;
			break;
			case TI_UINT:
				lpImageView->dataDisplay = DT_UINT;
			break;
			case TI_PIXEL:
				lpImageView->dataDisplay = DT_PIXEL;
			break;
			case TI_FLOAT:
				lpImageView->dataDisplay = DT_FLOAT;
				break;
			default:
				break;
			}
			
			SendMessage(hwnd, WM_SHOWFRAMEDATA, NULL, NULL);
		}
		return 0;
		break;
	case WM_SETWH:
		INT vMax;
		INT hMax;
		if(lpImageView != NULL){
			lpImageView->w = (UINT)wParam;
			lpImageView->h = (UINT)lParam;
			w = lpImageView->w;
			h = lpImageView->h;
		}

		SendMessage(hwnd, WM_ALCIMGTYPE, NULL, NULL);
		SendMessage(hwnd, WM_SIZE, SIZE_MAXIMIZED, NULL);
		SendMessage(hwnd, WM_UPDATESCROLL, NULL, NULL);

		return 0;
		break;
	case WM_UPDATESCROLL:
		GetClientRect(hwnd, &rcClient);
		if(lpImageView){
			lpImageView->vScrollMax = (h - rcClient.bottom);
			lpImageView->hScrollMax = (w - rcClient.right);

			if(lpImageView->hScrollMax < 0)
				lpImageView->hScrollMax = 0;
			if(lpImageView->vScrollMax < 0)
				lpImageView->vScrollMax = 0;

			SetScrollRange(hwnd, SB_VERT, 0, vScrollMax, TRUE);
			SetScrollRange(hwnd, SB_HORZ, 0, hScrollMax, TRUE);

			
		}
		INT xhOff;
		INT yvOff;

		xhOff = 0;
		yvOff = 0;

		if(lpImageView){
			w = lpImageView->rcClient.right - lpImageView->rcClient.left;
			h = lpImageView->rcClient.bottom - lpImageView->rcClient.top;

			if(lpImageView->w){
				if((lpImageView->hScrollInfo.nPos + w) > lpImageView->w)
				{
					xhOff = (INT) lpImageView->w - (INT) (lpImageView->hScrollInfo.nPos + w);
					lpImageView->hScrollInfo.nPos += xhOff;
				}

				xhOff = (INT) labs((INT)(lpImageView->hScrollInfo.nPos + w) - (INT)lpImageView->w);
				if(xhOff < 0)
				{
					lpImageView->hScrollInfo.nPos = 0;
				}

			}

			if(lpImageView->h){
				if((lpImageView->vScrollInfo.nPos + h) > lpImageView->h)
				{
					yvOff = lpImageView->h - (lpImageView->vScrollInfo.nPos + h);
					lpImageView->vScrollInfo.nPos += yvOff;
				}
				yvOff = (INT)labs((lpImageView->vScrollInfo.nPos + h) - lpImageView->h);
				if(yvOff < 0)
				{
					lpImageView->vScrollInfo.nPos = 0;
				}

			}

		}
		
		SendMessage(hwnd, WM_PAINT, NULL, NULL);

		return 0;
		break;
	case WM_SETFRAMENUM:
		UINT numFrame;
		numFrame = (UINT)wParam;
		lpImageView->nFramesMax = (UINT)wParam-1;

		SendMessage(hFrameSlider, WM_SETFRAMENUM, wParam, lParam);
		if(lpImageView){
			lpImageView->iFrame = 0;
			lpImageView->iImage = 0;
			lpImageView->tCalcFrame = (UINT *)calloc(numFrame, sizeof(UINT));

			//10.18.12	Consider Removing -- Doesn't Make Sense
			lpImageView->cFileName[MAX_STRING] = (CHAR *)calloc(numFrame, sizeof(CHAR));

		}
		return 0;
		break;
	case WM_SETFRAMEPOS:
		lpImageView->iFrame = (UINT)wParam;
		lpImageView->iImage = (UINT)wParam;

		if(lpImageView->iFrame < lpImageView->iFrameAdd){
			lpImageView->statusSlider = TS_FLOATING;
		}
		else if(lpImageView->iFrame == (lpImageView->nFramesMax)){
			lpImageView->endOfTransmission = TRUE;
			lpImageView->statusSlider = TS_FLOATING;
		}
		else{
			if(!lpImageView->endOfTransmission)
				lpImageView->statusSlider = TS_LATCHED;
		}

		SetScrollPos(lpImageView->hFrameSlider, SB_HORZ, lpImageView->iFrame, TRUE);

		//SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
		PostMessage(hwnd, WM_SHOWFRAMEDATA, NULL, NULL);
		//SendMessage(hwnd, WM_SHOWFRAMEDATA, NULL, NULL);

		if(hwnd == GetCapture()){
			sprintf(frmPos, "Frame: %d", lpImageView->tCalcFrame[iFrame]);
			UpdatePartText(g_hStatusBar, 2, frmPos);
		}
		
		return 0;
		break;
	case WM_SETFRAMEADD:

		lpImageView->iFrameAdd = (UINT)wParam;
		return 0;//Remove

		break;

	//case WM_SETFRMNAME:
	//	//sprintf(lpImageView->cFileName[iFrame], "%s", (CHAR)wParam);
	//	break;
	//case WM_GETFRMNAME:
	//	//return (WPARAM)lpImageView->cFileName[iFrame];
	//	break;
	case WM_SETTITLE:
		strcpy(cWndTitle, (CHAR *)wParam); 
		SetWindowText(hwnd, cWndTitle);
		return 0;
	case WM_SETFRMTIME:
		lpImageView->tCalcFrame[iFrameAdd] = (UINT)wParam;
		if(hwnd == GetCapture()){
			sprintf(frmTim, "Time: %d", lpImageView->tCalcFrame[iFrameAdd]);
			UpdatePartText(g_hStatusBar, 3, frmTim);
		}
		break;
	case WM_GETFRMTIME:
		return lpImageView->tCalcFrame[iFrame];
	case WM_ADDTYPE:

		HANDLE hThread;
		TYPEINFO *typeInfo;
		typeInfo = (TYPEINFO *) wParam;

		UINT nBytes;
		UINT idType;
		idType = typeInfo->uType;
		nBytes = typeInfo->nBytes;
		hThread = GetCurrentThread();

		SetThreadPriorityBoost(hThread, TRUE);
		SetThreadPriority(hThread, 15);

		
		if(lpImageView){

			if(lpImageView->iFrameAdd > lpImageView->nFramesAdded){
				lpImageView->nFramesAdded = lpImageView->iFrameAdd;
				iFrameAdd = lpImageView->iFrameAdd;
			}

			if(nFramesMax > 1){
				ArchiveRawTYPE(lpImageView->iWnd, iFrameAdd, idType, nBytes, typeInfo->lpvoid);
			}

			
			switch(idType)
			{
			case TI_INT:
				if(lpImageView->imgINT){
					if(!(typeMask&TM_INT))
						SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
					lpImageView->typeMask = typeMask|TM_INT;
					//if(nFramesMax == 1)
					memcpy(lpImageView->imgINT, typeInfo->lpvoid, w*h*sizeof(INT));
				}
				break;
			case TI_UINT:
				if(lpImageView->imgUINT){
					if(!(typeMask&TM_UINT))
						SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
					lpImageView->typeMask = typeMask|TM_UINT;
					//if(nFramesMax == 1)
					memcpy(lpImageView->imgUINT, typeInfo->lpvoid, w*h*sizeof(UINT));
				}
				break;
			case TI_UCHAR:
				if(lpImageView->imgUCHAR){
					if(!(typeMask&TM_UCHAR))
						SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
					lpImageView->typeMask = typeMask|TM_UCHAR;
					//if(nFramesMax == 1)
					memcpy(lpImageView->imgUCHAR, typeInfo->lpvoid, w*h*sizeof(UCHAR));
				}
				break;
			case TI_CHAR:
				if(lpImageView->imgCHAR){
					if(!(typeMask&TM_CHAR))
						SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
					lpImageView->typeMask = typeMask|TM_CHAR;
					//if(nFramesMax == 1)
					memcpy(lpImageView->imgCHAR, typeInfo->lpvoid, w*h*sizeof(CHAR));
				}
				break;
			case TI_FLOAT:
				if(lpImageView->imgFLOAT){
					if(!(typeMask&TM_FLOAT))
						SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
					lpImageView->typeMask = typeMask|TM_FLOAT;
					//if(nFramesMax == 1)
					memcpy(lpImageView->imgFLOAT, typeInfo->lpvoid, w*h*sizeof(FLOAT));
				}
				break;
			case TI_PIXEL:
				if(lpImageView->imgPIXEL){
					if(!(typeMask&TM_PIXEL))
						SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
					lpImageView->typeMask = typeMask|TM_PIXEL;
					//if(nFramesMax == 1){
					memcpy(lpImageView->imgPIXEL, typeInfo->lpvoid, w*h*sizeof(PIXEL));
					memcpy(lpImageView->dispPIXEL, typeInfo->lpvoid, w*h*sizeof(PIXEL));
					//}

				}
				break;
			//case TI_POINT:
			//	if(lpImageView->point){
			//	}
			//	return 0;
			//case TI_EDGE:
			//	if(lpImageView->edge){
			//	}
			//	return 0;
			default:
				break;
			}
		}

		if(lpImageView->statusSlider == TS_LATCHED)
			SendMessage(hwnd, WM_SETFRAMEPOS, (WPARAM)lpImageView->iFrameAdd, NULL);

		return 0;
	break;


	/********************************************UPDATE********************************************/
	/********************************************6/12/12*******************************************/
	case WM_INSERTFRAME:

		SetThreadPriority(GetModuleHandle(NULL), 12);

		PDMFRAMEHEADER *pdmFrameHeader;
		LPVOID			lpvoid;

		pdmFrameHeader = (PDMFRAMEHEADER *) wParam;
		lpvoid = (LPVOID) lParam;

		
		if(lParam && wParam){

			CHAR strPath[MAX_STRING];
			GetFilePathPDM(lpImageView->iWnd, lpImageView->iFrameAdd, pdmFrameHeader->typeData, (CHAR *)lpImageView->strDirectory, (CHAR *)lpImageView->strPath);
	
			switch(pdmFrameHeader->typeData){
				case DT_INT:
					if(lpImageView->imgINT){
						if(!(typeMask&TM_INT))
							SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
						lpImageView->typeMask = typeMask|TM_INT;
						//if(nFramesMax == 1)
						memcpy(lpImageView->imgINT, lpvoid, w*h*sizeof(INT));
					}
					break;
				case DT_UINT:
					if(lpImageView->imgUINT){
						if(!(typeMask&TM_UINT))
							SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
						lpImageView->typeMask = typeMask|TM_UINT;
						//if(nFramesMax == 1)
						memcpy(lpImageView->imgUINT, lpvoid, w*h*sizeof(UINT));
					}
					break;
				case DT_UCHAR:
					if(lpImageView->imgUCHAR){
						if(!(typeMask&TM_UCHAR))
							SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
						lpImageView->typeMask = typeMask|TM_UCHAR;
						//if(nFramesMax == 1)
						memcpy(lpImageView->imgUCHAR, lpvoid, w*h*sizeof(UCHAR));
					}
					break;
				case DT_CHAR:
					if(lpImageView->imgCHAR){
						if(!(typeMask&TM_CHAR))
							SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
						lpImageView->typeMask = typeMask|TM_CHAR;
						//if(nFramesMax == 1)
						memcpy(lpImageView->imgCHAR, lpvoid, w*h*sizeof(CHAR));
					}
					break;
				case DT_FLOAT:
					if(lpImageView->imgFLOAT){
						if(!(typeMask&TM_FLOAT))
							SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
						lpImageView->typeMask = typeMask|TM_FLOAT;
						//if(nFramesMax == 1)
						memcpy(lpImageView->imgFLOAT, lpvoid, w*h*sizeof(FLOAT));
					}
					break;
				case DT_PIXEL:
					if(lpImageView->imgPIXEL){
						if(!(typeMask&TM_PIXEL))
							SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
						lpImageView->typeMask = typeMask|TM_PIXEL;
						//if(nFramesMax == 1){
						memcpy(lpImageView->imgPIXEL, lpvoid, w*h*sizeof(PIXEL));
						memcpy(lpImageView->dispPIXEL, lpvoid, w*h*sizeof(PIXEL));
						//}
					}
				//case DT_FLOAT:
				//	if(lpImageView->imgPIXEL){
				//		if(!(typeMask&TM_FLOAT))
				//			SetMenuItem(lpImageView->hMenu, idType, MF_ENABLED|MF_UNCHECKED);
				//		lpImageView->typeMask = typeMask|TM_FLOAT;
				//		memcpy(lpImageView->imgFLOAT, lpvoid, w*h*sizeof(FLOAT));
				//	}
				//	break;
			}

		}

		//Archive Frame
		//Opens a seperate thread to write to the hard drive
		ArchivePDMFRAME((CHAR *)lpImageView->strPath, (PDMFRAMEHEADER)*pdmFrameHeader, (LPVOID) lParam);

		if(lpImageView->statusSlider == TS_LATCHED)
				SendMessage(hwnd, WM_SETFRAMEPOS, (WPARAM)lpImageView->iFrameAdd, NULL);

		return 0;


/***********************************************MOUSE INPUT****************************************************/
/***********************************************MOUSE INPUT****************************************************/
/***********************************************MOUSE INPUT****************************************************/
/***********************************************MOUSE INPUT****************************************************/
	case WM_KEYDOWN:
			POINT pt;
			GetCursorPos(&pt);
            switch (wParam) 
            { 
                case VK_LEFT: 
                    // Process the LEFT ARROW key. 
					SetCursorPos(pt.x-1, pt.y);
                    break; 
                 case VK_RIGHT: 
                    // Process the RIGHT ARROW key. 
					 SetCursorPos(pt.x+1, pt.y);
                    break; 
                case VK_UP:   
                    // Process the UP ARROW key. 
					SetCursorPos(pt.x, pt.y-1);
                    break; 
                case VK_DOWN:   
                    // Process the DOWN ARROW key.  
					SetCursorPos(pt.x, pt.y+1);
                    break;
               
                default: 
                    break; 
            } 
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		INT x, y, i;
		INT value;
		xOff = lpImageView->hScrollInfo.nPos;
		yOff = lpImageView->vScrollInfo.nPos;

		x = LOWORD(lParam) + xOff;
		y = HIWORD(lParam) + yOff;
		i = y*w + x;
		
		if(lpImageView != NULL){
			if(range(w, h, x, y)){	
				i = y*w + x;
				switch(dispState)
				{
				case TI_CHAR:
						if(lpImageView->imgCHAR){ 
							value = lpImageView->imgCHAR[i];
							sprintf(curVal, "Value: %d", value);
						}
					break;
				case TI_UCHAR:
						if(lpImageView->imgUCHAR){
							value = lpImageView->imgUCHAR[i];
							sprintf(curVal, "Value: %d", value);
						}
					break;
				case TI_INT:
						if(lpImageView->imgINT){
							value = lpImageView->imgINT[i];
							sprintf(curVal, "Value: %d", value);
						}
					break;
				case TI_PIXEL:
						UINT r, g, b;
						if(lpImageView->imgPIXEL){
							r = lpImageView->imgPIXEL[i].r;
							g = lpImageView->imgPIXEL[i].g;
							b = lpImageView->imgPIXEL[i].b;
							sprintf(curVal, "RGB(%d, %d, %d)", r, g, b);
						}
					break;
				case TI_UINT:
						if(lpImageView->imgUINT){
							value = lpImageView->imgUINT[i];
							sprintf(curVal, "Value: %d", value);
						}
				case TI_FLOAT:
						if(lpImageView->imgFLOAT){
							FLOAT fValue;
								fValue = lpImageView->imgFLOAT[i];
								sprintf(curVal, "Value: %.2f", fValue);
						}
				default:
					break;
				}
				sprintf(frmTim, "Time: %d", lpImageView->tCalcFrame[iFrame]);
				sprintf(frmPos, "Frame: %d", iFrame);
				sprintf(curPos, "Cursor: (%d, %d)", x, y);
				UpdatePartText(g_hStatusBar, 0, curVal);
				UpdatePartText(g_hStatusBar, 1, curPos);
				UpdatePartText(g_hStatusBar, 2, frmPos);
				UpdatePartText(g_hStatusBar, 3, frmTim);
			}


		}

		
		if(wParam&MK_LBUTTON)
		{	
			if(lpImageView->tool == TL_CURSOR){
				POINT pt;
				UINT hPos;
				UINT vPos;
				memcpy(&pt, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
				pt.x = pt.x;

				hPos = lpImageView->ptCurGrab.x - pt.x + lpImageView->hGrabPos;
				vPos = lpImageView->ptCurGrab.y - pt.y + lpImageView->vGrabPos;
				if(hPos > 0 && hPos < hScrollMax){
					lpImageView->hScrollInfo.nPos = hPos;
					lpImageView->hScrollPos = hPos;
					SetScrollPos(hwnd, SB_HORZ, hPos, TRUE);
				}
				if(vPos > 0 && vPos < vScrollMax){
					lpImageView->vScrollInfo.nPos = vPos;
					lpImageView->vScrollPos = vPos;
					SetScrollPos(hwnd, SB_VERT, vPos, TRUE);
				}
				SendMessage(hwnd, WM_PAINT, NULL, NULL);

			}

		}
		/*return DefMDIChildProc(hwnd, msg, wParam, lParam);*/
		break;
	case WM_HSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_THUMBTRACK:
			INT pos;
			pos = HIWORD(wParam);
			lpImageView->hScrollInfo.nPos = pos;
			SetScrollPos(hwnd, SB_HORZ, pos, TRUE);
			SendMessage(hwnd, WM_PAINT, NULL, NULL);
			break;		
		case SB_LINERIGHT:
			pos = GetScrollPos(hwnd, SB_HORZ);
			GetScrollRange(hwnd, SB_HORZ, &rMin, &rMax);
			if(pos < rMax){
				pos=pos+10;
				lpImageView->hScrollInfo.nPos = pos;
				SetScrollPos(hwnd, SB_HORZ, pos, TRUE);
				SendMessage(hwnd, WM_PAINT, NULL, NULL);
			}
			break;
		case SB_LINELEFT:
			pos = GetScrollPos(hwnd, SB_HORZ);
			GetScrollRange(hwnd, SB_HORZ, &rMin, &rMax);
			if(pos > 0){
				pos=pos-10;
				lpImageView->hScrollInfo.nPos = pos;
				SetScrollPos(hwnd, SB_HORZ, pos, TRUE);
				SendMessage(hwnd, WM_PAINT, NULL, NULL);
			}
			break;
		default:
			break;
		}
		break;
	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_THUMBTRACK:
			INT pos;
			pos = HIWORD(wParam);
			lpImageView->vScrollInfo.nPos = pos;
			SetScrollPos(hwnd, SB_VERT, pos, TRUE);
			SendMessage(hwnd, WM_PAINT, NULL, NULL);
			break;
		case SB_LINERIGHT:
			pos = GetScrollPos(hwnd, SB_VERT);
			GetScrollRange(hwnd, SB_VERT, &rMin, &rMax);
			if(pos < rMax){
				pos=pos+10;
				lpImageView->vScrollInfo.nPos = pos;
				SetScrollPos(hwnd, SB_VERT, pos, TRUE);
				SendMessage(hwnd, WM_PAINT, NULL, NULL);
			}
			break;
		case SB_LINELEFT:
			pos = GetScrollPos(hwnd, SB_VERT);
			GetScrollRange(hwnd, SB_VERT, &rMin, &rMax);
			if(pos > 0){
				pos=pos-10;
				lpImageView->vScrollInfo.nPos = pos;
				SetScrollPos(hwnd, SB_VERT, pos, TRUE);
				SendMessage(hwnd, WM_PAINT, NULL, NULL);
			}
			break;
		default:
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		BOOL selection;
		GetCursorPos(&pt);
		selection = TrackPopupMenuEx(lpImageView->hPopUpMenu,	TPM_LEFTALIGN|
																TPM_TOPALIGN|
																TPM_RETURNCMD|
																TPM_LEFTBUTTON|
																TPM_VERPOSANIMATION|
																TPM_VERTICAL|
																TPM_RETURNCMD, pt.x, pt.y, hwnd, NULL);
		switch(selection)
		{
		case ID_SHOWUCHAR:
			SendMessage(hwnd, WM_SETSHOWTYPE, (WPARAM)TI_UCHAR, NULL);
			return 0;
		case ID_SHOWINT:
			SendMessage(hwnd, WM_SETSHOWTYPE, (WPARAM)TI_INT, NULL);
			return 0;
		case ID_SHOWPIXEL:
			SendMessage(hwnd, WM_SETSHOWTYPE, (WPARAM)TI_PIXEL, NULL);
			return 0;
		case ID_SHOWFLOAT:
			SendMessage(hwnd, WM_SETSHOWTYPE, (WPARAM)TI_FLOAT, NULL);
			return 0;
		default:
			return 0;
		}

		break;
	case WM_LBUTTONDOWN:

		switch(lpImageView->tool)
		{
		case TL_CURSOR:
			/*lpImageView->tool == TL_CURSOR){*/
			lpImageView->vGrabPos = lpImageView->vScrollInfo.nPos;
			lpImageView->hGrabPos = lpImageView->hScrollInfo.nPos;
			memcpy(&lpImageView->ptCurGrab, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
		break;
		case TL_POLYGON:
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			xOff = lpImageView->hScrollInfo.nPos;
			yOff = lpImageView->vScrollInfo.nPos;
			if(lpImageView->polygon->nPoint < MAX_POLYGONPOINT){
				lpImageView->polygon->point[lpImageView->polygon->nPoint] = Point(pt.x+xOff, pt.y+yOff);
				lpImageView->polygon->vertex[lpImageView->polygon->nVertex] = Point(pt.x+xOff, pt.y+yOff);
				lpImageView->polygon->nPoint++;
				lpImageView->polygon->nVertex++;
				SendMessage(hwnd, WM_SHOWFRAMEDATA, NULL, NULL);
				//SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
			}
			//lpImageView->
			break;
		case TL_FILL:
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);
			xOff = lpImageView->hScrollInfo.nPos;
			yOff = lpImageView->vScrollInfo.nPos;
			if(lpImageView->ptFill.point){
				UINT nPoint = lpImageView->ptFill.nPoints;
				lpImageView->ptFill.point[nPoint] = Point(pt.x+xOff, pt.y+yOff);
				lpImageView->ptFill.nPoints++;
				SendMessage(hwnd, WM_SHOWFRAMEDATA, NULL, NULL);
				//SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
			}
			break;
		default:
			break;
		}

		//switch(lpImageView->tool)
		//	{
		//	case TL_CURSOR:
		//		/*lpImageView->tool == TL_CURSOR){*/
		//		lpImageView->vGrabPos = lpImageView->vScrollInfo.nPos;
		//		lpImageView->hGrabPos = lpImageView->hScrollInfo.nPos;
		//		memcpy(&lpImageView->ptCurGrab, &Point(LOWORD(lParam), HIWORD(lParam)), sizeof(POINT));
		//	break;
		//	case TL_POLYGON:
		//		GetCursorPos(&pt);
		//		if(lpImageView->polygon->nPoint < MAX_POLYGONPOINT){
		//			lpImageView->polygon->point[lpImageView->polygon->nPoint] = Point(x, y);
		//			lpImageView->polygon->nPoint++;
		//			SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
		//		}
		//		//lpImageView->
		//		break;
		//	default:
		//		break;
		//	}
		//
		
		//lpImageView->ptCurGrab
		break;
	case WM_MOUSEWHEEL:
		//if(LOWORD(wParam)&MK_LBUTTON){
			INT frame;
			INT wheelPos;

			wheelPos = (short) HIWORD(wParam);
			//if(lpImageView->iFrame <= (lpImageView->nFramesMax+1) && lpImageView->iFrame > 0){
				if(wheelPos > 0){
					if(lpImageView->iFrame < lpImageView->nFramesMax){
						frame = lpImageView->iFrame + 1;
						//lpImageView->iFrame = lpImageView->iFrame + 1;
					}
					else{
						lpImageView->iFrame = lpImageView->nFramesMax;
						frame = lpImageView->nFramesMax;
					}
				}
				else{
					if(lpImageView->iFrame > 0){
						frame = lpImageView->iFrame - 1;
						//lpImageView->iFrame = lpImageView->iFrame - 1;
					}
					else{
						lpImageView->iFrame = 0;
						frame = 0;
					}
				}
			//}
			//if(frame > 0 && frame < lpImageView->nFramesMax){
			SendMessage(hwnd, WM_SETFRAMEPOS, frame, NULL);
			//}

		//}
		break;
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^MOUSE INPUT^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/***********************************************MOUSE INPUT****************************************************/
/***********************************************MOUSE INPUT****************************************************/
/***********************************************MOUSE INPUT****************************************************/




	case WM_SETSLIDERSTATUS:
		lpImageView->statusSlider = (CHAR) wParam;
		return 0;
	case WM_MOVE:
		SendMessage(g_hStatusBar, WM_SIZE, NULL, NULL);
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
	case WM_SETCHILDID:
		lpImageView->iWnd = (UINT)wParam;
		break;
	case WM_NCCREATE:
		lpImageView = (LPTRIMAGEVIEW *)malloc(sizeof(LPTRIMAGEVIEW));
		memset(lpImageView, 0x00, sizeof(LPTRIMAGEVIEW));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpImageView);
		lpImageView = (LPTRIMAGEVIEW *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);
		if(lpImageView == NULL){
			MessageBox(hwnd, (LPCSTR)"GetWindowLongPtr Failed", (LPCSTR)"Error!", MB_OK);
		}

		return DefMDIChildProc(hwnd, msg, wParam, lParam);
	case WM_CREATE:
			HFONT hfDefault;
			HWND hControl;
			HWND hSlider;
			HMENU hSubMenu;


			//Create Image View Static Control
			hControl = CreateWindowEx(WS_EX_CLIENTEDGE, "#32769"/*"Static"*/, "", 
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 
				CW_USEDEFAULT, 
				CW_USEDEFAULT, 
				CW_USEDEFAULT, 
				CW_USEDEFAULT, 
				hwnd, 
				(HMENU)IDC_CHILD_EDIT, 
				GetModuleHandle(NULL), 
				NULL);

			//Create Frame Slider Control
			hSlider = CreateWindowEx(WS_EX_STATICEDGE|WS_EX_TRANSPARENT, "FrameSlider", "",
				WS_HSCROLL|WS_CHILD|WS_VISIBLE,
				10,
				100,
				300,
				40,
				hwnd, 
				(HMENU)IDC_CHILD_EDIT,
				GetModuleHandle(NULL),
				NULL);

			HWND hProgressBar;

			//hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD|WS_VISIBLE, 20, 20, 260, 17, hwnd, NULL, GetModuleHandle(NULL), NULL);
			//ShowWindow(hProgressBar, SW_SHOW);

			ShowWindow(hControl, SW_SHOW);
			ShowWindow(hSlider, SW_SHOW);

			if(hControl == NULL)
				MessageBox(hwnd, "Could not create Image View.", "Error", MB_OK | MB_ICONERROR);
			else
				lpImageView->hwnd = hControl;

			if(hSlider == NULL)
				MessageBox(hwnd, "Could not create Frame Slider (2)", "Error", MB_OK);
			else
				lpImageView->hFrameSlider = hSlider;

			//Initialize 'endOfTransmission' Flag to Be False
			lpImageView->endOfTransmission = FALSE;

			hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hControl, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			lpImageView->hMenu = GetSystemMenu(hwnd, FALSE);
			if(lpImageView->hMenu){
				InsertMenu(lpImageView->hMenu, TI_CHAR, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWCHAR, (LPCSTR) STR_CHAR);
				InsertMenu(lpImageView->hMenu, TI_UCHAR, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWUCHAR, (LPCSTR) STR_UCHAR);
				InsertMenu(lpImageView->hMenu, TI_INT, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWINT, (LPCSTR) STR_INT);
				InsertMenu(lpImageView->hMenu, TI_UINT, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWUINT, (LPCSTR) STR_UINT);
				InsertMenu(lpImageView->hMenu, TI_FLOAT, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWFLOAT, (LPCSTR) STR_FLOAT);
				InsertMenu(lpImageView->hMenu, TI_PIXEL, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWPIXEL, (LPCSTR) STR_PIXEL);
				InsertMenu(lpImageView->hMenu, TI_POINT, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWPOINT, (LPCSTR) STR_POINT);
				InsertMenu(lpImageView->hMenu, TI_EDGE, MF_BYPOSITION | MF_STRING | MF_GRAYED, ID_SHOWEDGE, (LPCSTR) STR_EDGE);
			}

			lpImageView->hPopUpMenu = CreatePopupMenu();
			MENUITEMINFO mii;
			memset(&mii, 0x00, sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_ID|MIIM_STRING|MIIM_DATA;


			mii.wID = ID_ZOOM;
			mii.dwTypeData = STR_ZOOM;
			mii.cch = strlen(STR_ZOOM);
			InsertMenuItem(lpImageView->hPopUpMenu, MP_ZOOM, TRUE, &mii);


			mii.wID = ID_SHOWUCHAR;
			mii.dwTypeData = STR_UCHAR;
			mii.cch = strlen(STR_UCHAR);
			InsertMenuItem(lpImageView->hPopUpMenu, MP_SHOWUCHAR, TRUE, &mii);

			mii.wID = ID_SHOWINT;
			mii.dwTypeData = STR_INT;
			mii.cch = strlen(mii.dwTypeData);
			InsertMenuItem(lpImageView->hPopUpMenu, MP_SHOWINT, TRUE, &mii);

			mii.wID = ID_SHOWPIXEL;
			mii.dwTypeData = STR_PIXEL;
			mii.cch = strlen(mii.dwTypeData);
			InsertMenuItem(lpImageView->hPopUpMenu, MP_SHOWPIXEL, TRUE, &mii);

			mii.wID = ID_SHOWFLOAT;
			mii.dwTypeData = STR_FLOAT;
			mii.cch = strlen(mii.dwTypeData);
			InsertMenuItem(lpImageView->hPopUpMenu, MP_SHOWFLOAT, TRUE, &mii);


			GetClientRect(hwnd, &lpImageView->rcClient);

			SendMessage(hSlider, WM_SETNUMFRAME, (WPARAM)0, (LPARAM)10);

			//Initialize Status Bar
			INT statusRegion[256];
			statusRegion[0] = W_VALUEPART;
			statusRegion[1] = statusRegion[0] + W_COORDPART;
			statusRegion[2] = statusRegion[1] + W_FRAMEPART;
			statusRegion[3] = statusRegion[2] + W_FRAMETIME;
			statusRegion[4] = -1;
			InitializeStatusBarParts(g_hStatusBar, 4, statusRegion);

			//Initialize Window Parameters
			lpImageView->xWin = 0;
			lpImageView->yWin = 0;
			lpImageView->wWin = 0;
			lpImageView->hWin = 0;

			//Initialize the starting Tool to be a cursor
			lpImageView->tool = TL_CURSOR;
			lpImageView->nptClick = NULL;

			//Ensure that scroll positions are Zeroed
			lpImageView->hScrollInfo.nPos = 0;
			lpImageView->vScrollInfo.nPos = 0;


			lpImageView->SWMODE = DS_RESTORED;

			//Latch Slider Bar so that each new frame is added sequentially
			lpImageView->statusSlider = TS_LATCHED;

			//Allocate Strings
			lpImageView->strPath = (CHAR *)calloc(MAX_STRING, sizeof(CHAR));
			lpImageView->strDirectory = (CHAR *)calloc(MAX_STRING, sizeof(CHAR));

			//Request the Archive Directory from it's self
			SendMessage(hwnd, WM_GETARCHIVEDIR, (WPARAM)lpImageView->strDirectory, NULL);

			break;
	case WM_GETARCHIVEDIR:
		sprintf((CHAR *)wParam, "%s", /*"C:\\Save"*/DIR_C);

		return 0;

	case WM_SETARCHIVEDIR:
		//sprintf((CHAR *)wParam, "%s", /*"C:\\Save"*/DIR_C);
		sprintf((CHAR *)lpImageView->strDirectory, "%s", (CHAR *)wParam);
		return 0;
	case WM_MDIACTIVATE:
		UINT EnableFlag;
		HMENU hFileMenu;
		hMenu = GetMenu(g_hMainWindow);
		if(hwnd == (HWND)lParam)
		{   
			//being activated, enable the menus
			EnableFlag = MF_ENABLED;
		}
		else
		{   
			//being de-activated, gray the menus
			EnableFlag = MF_GRAYED;
		}

		//EnableMenuItem(hMenu, MP_WINDOW , MF_BYPOSITION | EnableFlag);
		//EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);

		hFileMenu = GetSubMenu(hMenu, MP_FILE);
		EnableMenuItem(hFileMenu, ID_FILE_SAVEAS, MF_BYCOMMAND | EnableFlag);
		//EnableMenuItem(hFileMenu, ID_EXPORT_BMP, MF_BYCOMMAND | EnableFlag);
		EnableMenuItem(hFileMenu, MP_FILE_EXPORT, MF_BYPOSITION|EnableFlag);
		
		//Force Menu Update
		DrawMenuBar(g_hMainWindow);
		break;
	case WM_CLOSE:
		if(MessageBox(hwnd, (LPCSTR) "Close Image View?", (LPCSTR) "Warning!", MB_OKCANCEL) == IDOK){
			if(lpImageView){
				if(lpImageView->hbmp != NULL)		DeleteObject(lpImageView->hbmp);
				if(lpImageView->hdcMem != NULL)		ReleaseDC(hwnd, lpImageView->hdcMem);
				if(lpImageView->imgCHAR != NULL)	free(lpImageView->imgCHAR);
				if(lpImageView->imgUCHAR != NULL)	free(lpImageView->imgUCHAR);
				if(lpImageView->imgINT != NULL)		free(lpImageView->imgINT);
				if(lpImageView->imgUINT != NULL)	free(lpImageView->imgUINT);
				if(lpImageView->imgFLOAT != NULL)	free(lpImageView->imgFLOAT);
				if(lpImageView->imgINT64 != NULL)	free(lpImageView->imgINT64);
				if(lpImageView->imgUINT64 != NULL)	free(lpImageView->imgUINT64);
				if(lpImageView->imgBGR != NULL)		free(lpImageView->imgBGR);
				
				for(i = 0; i < iFrame+1; i++){
					sprintf(str, "%s%d_%d_%s.raw", DIR_ARCHIVE, iWnd, i, STR_CHAR);
					DeleteFile(str);
					sprintf(str, "%s%d_%d_%s.raw", DIR_ARCHIVE, iWnd, i, STR_UCHAR);
					DeleteFile(str);
					sprintf(str, "%s%d_%d_%s.raw", DIR_ARCHIVE, iWnd, i, STR_INT);
					DeleteFile(str);
					sprintf(str, "%s%d_%d_%s.raw", DIR_ARCHIVE, iWnd, i, STR_UINT);
					DeleteFile(str);
					sprintf(str, "%s%d_%d_%s.raw", DIR_ARCHIVE, iWnd, i, STR_FLOAT);
					DeleteFile(str);
					sprintf(str, "%s%d_%d_%s.raw", DIR_ARCHIVE, iWnd, i, STR_PIXEL);
					DeleteFile(str);
				}

				/*************************MEMORY LEAK HERE*************************/
				//Cannot Relase lpImageView
				//if(lpImageView)						free(lpImageView);
			}
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
		}
		else
			return 0;
		break;
	case WM_ALCIMGTYPE:
		if(lpImageView->imgCHAR)	free(lpImageView->imgCHAR);
		if(lpImageView->imgUCHAR)	free(lpImageView->imgUCHAR);
		if(lpImageView->imgINT)		free(lpImageView->imgINT);
		if(lpImageView->imgUINT)	free(lpImageView->imgUINT);
		if(lpImageView->imgFLOAT)	free(lpImageView->imgFLOAT);
		if(lpImageView->imgPIXEL)	free(lpImageView->imgPIXEL);
		if(lpImageView->dispPIXEL)	free(lpImageView->dispPIXEL);

		lpImageView->imgCHAR = (CHAR *) malloc(w*h*sizeof(CHAR));
		lpImageView->imgUCHAR = (UCHAR *) malloc(w*h*sizeof(UCHAR));
		lpImageView->imgINT = (INT *) malloc(w*h*sizeof(INT));
		lpImageView->imgUINT = (UINT *) malloc(w*h*sizeof(UINT));
		lpImageView->imgFLOAT = (FLOAT *) malloc(w*h*sizeof(FLOAT));
		lpImageView->imgPIXEL = (PIXEL *) malloc(w*h*sizeof(PIXEL));
		lpImageView->polygon = (POLYGON *) calloc(1, sizeof(POLYGON));
		lpImageView->polygon->point = (POINT *) calloc(MAX_POLYGONPOINT, sizeof(POINT));
		lpImageView->polygon->vertex = (VERTEX *)calloc(MAX_POLYGONPOINT, sizeof(POINT));
		lpImageView->ptFill.point = (POINT *)calloc(MAX_FILLPOINT, sizeof(POINT));
		//lpImageView->dispPIXEL = (PIXEL *)calloc(w*h, sizeof(PIXEL));

		if(lpImageView->hdc)					
			ReleaseDC(hwnd, lpImageView->hdc);
		if(lpImageView->hbmp/* || lpImageView->hbmp != ERROR_INVALID_PARAMETER*/)	
			DeleteObject(lpImageView->hbmp);
		if(lpImageView->hdcMem)
			ReleaseDC(hwnd, lpImageView->hdcMem);

		lpImageView->dispSurface.w = w;
		lpImageView->dispSurface.h = h;
		lpImageView->dispSurface.hwnd = hwnd;
		lpImageView->hbmp = CreateDisplaySurface(hwnd, &lpImageView->hdcMem, &lpImageView->hdc, w, h, &lpImageView->dispPIXEL);

		lpImageView->ptClick = 0;

		//******TEST******TEST********TEST*******
		//******TEST******TEST********TEST*******
		//******TEST******TEST********TEST*******
		SendMessage(hwnd, WM_UPDATESCROLL, NULL, NULL);
		ShowWindow(hwnd, SW_SHOW);
		//TestInitializeDrawChannles(lpImageView);
		break;
	default:
		return DefMDIChildProc(hwnd, msg, wParam, lParam);

	}
	return 0;//DefMDIChildProc(hwnd, msg, wParam, lParam);
}

VOID InsertFrameTYPE(HWND hwnd, UINT frame, TYPEINFO typeInfo)
{
	if(frame < 2)
		ShowWindow(hwnd, SW_SHOW);

	SendMessage(hwnd, WM_SETFRAMEADD, (WPARAM)frame, NULL);
	SendMessage(hwnd, WM_ADDTYPE, (WPARAM)&typeInfo, NULL);
	//SendMessage(hwnd, WM_SETFRAMEPOS, (WPARAM)frame, NULL);
	//SendMessage(hwnd, WM_UPDATESCROLL, NULL, NULL);
	
	//SendMessage(hwnd, WM_DISPLAYIMG, NULL, NULL);
}