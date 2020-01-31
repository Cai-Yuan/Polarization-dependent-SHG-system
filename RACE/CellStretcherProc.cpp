#include "stdafx.h"

#include "CellStretcher.h"
#include "EnumerateDevice.h"

INT_PTR CALLBACK CellStretcherProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CELLSTRETCHER *lpCellStretch = (CELLSTRETCHER *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	CHAR str[MAX_STRING];
	CHAR strRecieved[MAX_STRING];

	switch(msg)
	{
	case WM_INITDIALOG:

		lpCellStretch = (CELLSTRETCHER *)calloc(1, sizeof(CELLSTRETCHER));
		memset(lpCellStretch, 0x00, sizeof(CELLSTRETCHER));
		SetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/, (LONG_PTR) lpCellStretch);
		lpCellStretch = (CELLSTRETCHER *) GetWindowLongPtr(hwnd, (-21)/*GWL_USERDATA*/);

		if(lpCellStretch == NULL){
			ThrowMessage(hwnd, "CellStretcherDialog: Error Getting Long Ptr");
			ThrowMessage(hwnd, "CellStretcherDialog: Exiting Initialization");
			SendMessage(hwnd, WM_CLOSE, NULL, TRUE);
			return TRUE;
		}

		lpCellStretch->hStretcher = FindCellStretcher(&lpCellStretch->port);

		//Initlaize Handles
		GetCellStretcherControls(lpCellStretch, hwnd, wParam, lParam);

		//Initalize Controls
		InitCellStretcherControls(lpCellStretch, hwnd, wParam, lParam);



		break;
	case WM_COMMAND:

		switch(HIWORD(wParam))
		{
		case BN_CLICKED:

			switch(LOWORD(wParam))
			{
			case IDC_BTN_CONNECT:
				if(!lpCellStretch->hStretcher){
					lpCellStretch->hStretcher = FindCellStretcher(&lpCellStretch->port);
					if(lpCellStretch->hStretcher){
						sprintf(str, "Connected (COM%d)", lpCellStretch->port);
						ComboBox_InsertString(lpCellStretch->hComboPort, 0, str);
						ComboBox_SetCurSel(lpCellStretch->hComboPort, 0);
						Button_SetText(lpCellStretch->hBtnConnect, "Disconnect");
						lpCellStretch->idTimer = SetTimer(hwnd, NULL, 1000, NULL);
					}
					else{
						Button_SetText(lpCellStretch->hBtnConnect, "Connect");
						ComboBox_InsertString(lpCellStretch->hComboPort, 0, "Not Found");
						ComboBox_SetCurSel(lpCellStretch->hComboPort, 0);
					}
				}
				else{
					CloseHandle(lpCellStretch->hStretcher);
					lpCellStretch->hStretcher = NULL;
					lpCellStretch->port = -1;
					Button_SetText(lpCellStretch->hBtnConnect, "Connect");
					ComboBox_InsertString(lpCellStretch->hComboPort, 0, "Disconnected");
					ComboBox_SetCurSel(lpCellStretch->hComboPort, 0);
				}
				break;
			case IDC_BTN_SETHOME:
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c", "!SETHOME", lpCellStretch->motor);
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_BTN_GOHOME:
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c", "!MOVE", lpCellStretch->motor, (INT) NULL);
				SerialSend(lpCellStretch->hStretcher, str, NULL);

				if(lpCellStretch->flgJog){
					lpCellStretch->flgJog = FALSE;
					Button_SetText(lpCellStretch->hBtnJog, "Jog");
				}
				break;
			case IDC_BTN_MOVE:
				if(lpCellStretch->flgJog){
					lpCellStretch->flgJog = FALSE;
					Button_SetText(lpCellStretch->hBtnJog, "Jog");
				}
				GetWindowText(lpCellStretch->hEditGoTo, str, MAX_STRING);
				sscanf(str, "%d", &lpCellStretch->valGoTo);
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c%d", "!MOVE", lpCellStretch->motor, lpCellStretch->valGoTo);
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_BTN_JOG:

				if(lpCellStretch->flgJog){
					lpCellStretch->flgJog = FALSE;
					memset(str, NULL, MAX_STRING*sizeof(CHAR));
					sprintf(str, "%s%c", "!JOG", lpCellStretch->motor);
					SerialSend(lpCellStretch->hStretcher, str, NULL);
					Button_SetText(lpCellStretch->hBtnJog, "Jog");
				}else{
					lpCellStretch->flgJog = TRUE;
					lpCellStretch->valVelocity = GetDlgItemInt(hwnd, IDC_EDIT_VELOCITY, NULL, FALSE);
					memset(str, NULL, MAX_STRING*sizeof(CHAR));
					sprintf(str, "%s%c", "!JOG", lpCellStretch->motor);
					SerialSend(lpCellStretch->hStretcher, str, NULL);
					Button_SetText(lpCellStretch->hBtnJog, "Stop");
				}
				break;
			case IDC_RADIO_A:
				lpCellStretch->motor = 'A';
				break;
			case IDC_RADIO_B:
				lpCellStretch->motor = 'B';
				break;
			case IDC_RADIO_C:
				lpCellStretch->motor = 'C';
				break;
			case IDC_RADIO_D:
				lpCellStretch->motor = 'D';
				break;
			case IDC_RADIO_E:
				lpCellStretch->motor = 'E';
				break;
			case IDC_RADIO_F:
				lpCellStretch->motor = 'F';
				break;
			case IDC_RADIO_CW:
				sprintf(str, "%s%c%s", "!SETDIR", lpCellStretch->motor, "CW");
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_RADIO_CCW:
				sprintf(str, "%s%c%s", "!SETDIR", lpCellStretch->motor, "CCW");
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_RADIO_FULL:
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c%d", "!SETUSTEP", lpCellStretch->motor, 1);
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_RADIO_HALF:
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c%d", "!SETUSTEP", lpCellStretch->motor, 2);
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_RADIO_QUARTER:
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c%d", "!SETUSTEP", lpCellStretch->motor, 4);
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			case IDC_RADIO_EIGHTH:
				memset(str, NULL, MAX_STRING*sizeof(CHAR));
				sprintf(str, "%s%c%d", "!SETUSTEP", lpCellStretch->motor, 8);
				SerialSend(lpCellStretch->hStretcher, str, NULL);
				break;
			default:
				break;
			}
			break;
		case EN_CHANGE:
			switch(LOWORD(wParam)){
				case IDC_EDIT_VELOCITY:
					GetWindowText(lpCellStretch->hEditVelocity, str, MAX_STRING);
					sscanf(str, "%f", &lpCellStretch->valVelocity);
					sprintf(str, "%s%c%.4f", "!SETRATE", lpCellStretch->motor, lpCellStretch->valVelocity);
					SerialSend(lpCellStretch->hStretcher, str, NULL);
					break;
				case IDC_EDIT_GOTO:

					break;
				default:
					break;
			}
		default:
			break;
		}
		break;	
	case WM_SETHWND:

		if(wParam == HWND_OWNER){
			lpCellStretch->hOwner = (HWND) lParam;
			//SendMessage(lpCellStretch->hOwner, WM_GETARCHIVEDIR, (WPARAM) lpCellStretch->strPath, NULL);
			//Edit_SetText(lpCellStretch->hEditPath, lpCellStretch->strPath);
		}	

		return TRUE;
	case WM_TIMER:

		CHAR strPos[MAX_STRING];
		memset(strPos, NULL, MAX_STRING*sizeof(CHAR));
		memset(str, NULL, MAX_STRING*sizeof(CHAR));
		sprintf(str, "%s%c", "?GETPOS", lpCellStretch->motor);
		SerialSend(lpCellStretch->hStretcher, str, strPos);

		CHAR strValue[MAX_STRING];
		memset(strValue, NULL, MAX_STRING*sizeof(CHAR));
		ReadStringTo(strPos, strValue, 13); //13=='CR'
		//RemoveStringTermination(strPos, strValue, 13);
		long long int pos;
		memset(&pos, NULL, sizeof(long long int));
		memcpy(&pos, strValue, sizeof(long long int)*sizeof(char));
		sprintf(strPos, "%d", pos);
		
		Edit_SetText(lpCellStretch->hEditPosAbsolute, strPos);
		return TRUE;
	case WM_CLOSE:

		if(!lParam){
			SendMessage(lpCellStretch->hOwner, WM_COMMAND, LOWORD(ID_DEVICES_CELLSTRETCHER), NULL);
			return TRUE;
		}

		return FALSE;
	default:
		return FALSE;
	}


	return FALSE;

}

LRESULT GetCellStretcherControls(CELLSTRETCHER *lpCellStretch, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	
	//Radio Controls
	lpCellStretch->hRadioTicks = GetDlgItem(hwnd, IDC_RADIO_TICKS);
	lpCellStretch->hRadioUm = GetDlgItem(hwnd, IDC_RADIO_UM);
	lpCellStretch->hRadioInch = GetDlgItem(hwnd, IDC_RADIO_INCH);

	lpCellStretch->hRadioA = GetDlgItem(hwnd, IDC_RADIO_A);
	lpCellStretch->hRadioB = GetDlgItem(hwnd, IDC_RADIO_B);
	lpCellStretch->hRadioC = GetDlgItem(hwnd, IDC_RADIO_C);
	lpCellStretch->hRadioD = GetDlgItem(hwnd, IDC_RADIO_D);
	lpCellStretch->hRadioE = GetDlgItem(hwnd, IDC_RADIO_E);
	lpCellStretch->hRadioF = GetDlgItem(hwnd, IDC_RADIO_F);

	lpCellStretch->hRadioFull = GetDlgItem(hwnd, IDC_RADIO_FULL);
	lpCellStretch->hRadioHalf = GetDlgItem(hwnd, IDC_RADIO_HALF);
	lpCellStretch->hRadioQuarter = GetDlgItem(hwnd, IDC_RADIO_QUARTER);
	lpCellStretch->hRadioEighth = GetDlgItem(hwnd, IDC_RADIO_EIGHTH);

	lpCellStretch->hRadioCW = GetDlgItem(hwnd, IDC_RADIO_CW);
	lpCellStretch->hRadioCCW = GetDlgItem(hwnd, IDC_RADIO_CCW);

	//Edit Controls
	lpCellStretch->hEditGoTo = GetDlgItem(hwnd, IDC_EDIT_GOTO);
	lpCellStretch->hEditVelocity = GetDlgItem(hwnd, IDC_EDIT_VELOCITY);
	lpCellStretch->hEditPosAbsolute = GetDlgItem(hwnd, IDC_EDIT_POS_ABSOLUTE);
	lpCellStretch->hEditPosRelative = GetDlgItem(hwnd, IDC_EDIT_POS_RELATIVE);

	//Button Controls
	lpCellStretch->hBtnMove = GetDlgItem(hwnd, IDC_BTN_MOVE);
	lpCellStretch->hBtnConnect = GetDlgItem(hwnd, IDC_BTN_CONNECT);
	lpCellStretch->hBtnJog = GetDlgItem(hwnd, IDC_BTN_JOG);

	//ComboBox Controls
	lpCellStretch->hComboPort = GetDlgItem(hwnd, IDC_COMBO_PORT);


	return TRUE;
}

LRESULT InitCellStretcherControls(CELLSTRETCHER *lpCellStretch, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	//SetDlgItemInt(hwnd, IDC, value, TRUE);
	CHAR str[MAX_STRING];

	Button_SetCheck(lpCellStretch->hRadioTicks, BST_CHECKED);

	ComboBox_InsertString(lpCellStretch->hComboPort, 0, "Not Connected");
	ComboBox_SetCurSel(lpCellStretch->hComboPort, 0);
	
	SendMessage(lpCellStretch->hEditGoTo, WM_SETTEXT, NULL, (LPARAM) "3200");
	SendMessage(lpCellStretch->hEditVelocity, WM_SETTEXT, NULL, (LPARAM) "200");

	lpCellStretch->valGoTo = 3200;
	lpCellStretch->valVelocity = 200;
	sprintf(str, "%s%d", "SETVEL", lpCellStretch->valVelocity);
	SerialSend(lpCellStretch->hStretcher, str, NULL);

	//Load these values
	SendMessage(lpCellStretch->hEditPosAbsolute, WM_SETTEXT, NULL, (LPARAM) "0");
	SendMessage(lpCellStretch->hEditPosRelative, WM_SETTEXT, NULL, (LPARAM) "0");

	if(lpCellStretch->hStretcher){
		sprintf(str, "Connected (COM%d)", lpCellStretch->port);
		ComboBox_InsertString(lpCellStretch->hComboPort, 0, str);
		ComboBox_SetCurSel(lpCellStretch->hComboPort, 0);
		Button_SetText(lpCellStretch->hBtnConnect, "Disconnect");
		lpCellStretch->idTimer = SetTimer(hwnd, NULL, 1000, NULL);
	}
	else{
		ComboBox_InsertString(lpCellStretch->hComboPort, 0, "NotConnected");
		ComboBox_SetCurSel(lpCellStretch->hComboPort, 0);
	}

	Button_SetCheck(lpCellStretch->hRadioA, TRUE);
	lpCellStretch->motor = 'A';

	Button_SetCheck(lpCellStretch->hRadioCW, TRUE);
	Button_SetCheck(lpCellStretch->hRadioEighth, TRUE);

	lpCellStretch->flgJog = NULL;

	return TRUE;
}

HANDLE FindCellStretcher(CHAR *port)
{
	HANDLE hStretcher = NULL;
	CHAR strPort[MAX_STRING];

	DWORD nPort = NULL;
	CHAR portList[MAX_STRING];
	
	
	hStretcher = ConnectToDevice(port, "?ID", "UNO32", 115200, 3000);
	//hStretcher = ConnectToDevice(port, "?ID", "UNO32");
	//hStretcher = ConnectToDevice(port, "?ID", "CellStretcher");

	return hStretcher;
}