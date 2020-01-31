#include "stdafx.h"

#include "XBoxLinker.h"
#include "XBox.h"

//XBoxLinker provides functions for translating XINPUT_STATE variables in to windows messages
//

DWORD TranslateXBoxState(HWND hwnd, XINPUT_STATE xStatePrev, XINPUT_STATE xState)
{


	INT btnStart = xState.Gamepad.wButtons&XINPUT_GAMEPAD_START;
	INT btnStartPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_START;
	INT btnA = xState.Gamepad.wButtons&XINPUT_GAMEPAD_A;
	INT btnAPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_A;
	INT btnX = xState.Gamepad.wButtons&XINPUT_GAMEPAD_X;
	INT btnXPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_X;
	INT btnY = xState.Gamepad.wButtons&XINPUT_GAMEPAD_Y;
	INT btnYPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_Y;
	INT btnB = xState.Gamepad.wButtons&XINPUT_GAMEPAD_B;
	INT btnBPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_B;
	INT bLeftTrigger = xState.Gamepad.bLeftTrigger;
	INT bLeftTriggerPrev = xStatePrev.Gamepad.bLeftTrigger;	
	INT bRightTrigger = xState.Gamepad.bRightTrigger;
	INT bRightTriggerPrev = xStatePrev.Gamepad.bRightTrigger;
	INT bLeftShoulder = xState.Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER;
	INT bLeftShoulderPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER;
	INT bRightShoulder = xState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER;
	INT bRightShoulderPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER;
	INT btnBack = xState.Gamepad.wButtons&XINPUT_GAMEPAD_BACK;
	INT btnBackPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_BACK;
	INT btnDPadUp = xState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP;
	INT btnDPadUpPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP;
	INT btnDPadDown = xState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN;
	INT btnDPadDownPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN;
	INT btnDPadLeft = xState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT;
	INT btnDPadLeftPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT;
	INT btnDPadRight = xState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT;
	INT btnDPadRightPrev = xStatePrev.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT;


		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_LEFTSTICK_X, (LPARAM) &xState.Gamepad.sThumbLX);

		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_LEFTSTICK_Y, (LPARAM) &xState.Gamepad.sThumbLY);

		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_RIGHTSTICK_Y, (LPARAM) &xState.Gamepad.sThumbRY);

	
	if(bLeftTrigger != bLeftTriggerPrev){
		if(bLeftTrigger < 150 && bLeftTriggerPrev > 150){
			SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_LEFTTRIGGER_UP, NULL);
		}else if(bLeftTrigger > 150 && bLeftTriggerPrev < 150){
			SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_LEFTTRIGGER_DOWN, NULL);
		}else{
			//Otherwise
		}
	}
	if(bRightTrigger != bRightTriggerPrev){
		if(bRightTrigger < 150 && bRightTriggerPrev > 150){
			SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_RIGHTTRIGGER_UP, NULL);
		}else if(bRightTrigger > 150 && bRightTriggerPrev < 150){
			SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_RIGHTTRIGGER_DOWN, NULL);
		}else{
			//Otherwise
		}
	}

	if(!btnStartPrev && btnStart){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_BUTTON_START, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnAPrev && btnA){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_BUTTON_A, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnXPrev && btnX){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_BUTTON_X, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnYPrev && btnY){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_BUTTON_Y, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnBPrev && btnB){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_BUTTON_B, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnBackPrev && btnBack){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_BUTTON_BACK, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnDPadUpPrev && btnDPadUp){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_DPAD_UP, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnDPadDownPrev && btnDPadDown){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_DPAD_DOWN, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnDPadLeftPrev && btnDPadLeft){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_DPAD_LEFT, (LPARAM) XB_BUTTON_PRESSED);
	}
	if(!btnDPadRightPrev && btnDPadRight){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_DPAD_RIGHT, (LPARAM) XB_BUTTON_PRESSED);
	}

	if(bLeftShoulder){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_LEFTSHOULDER, (LPARAM) XB_BUTTON_DOWN);
	}else{
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_LEFTSHOULDER, (LPARAM) XB_BUTTON_UP);
	}

	if(bRightShoulder){
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_RIGHTSHOULDER, (LPARAM) XB_BUTTON_DOWN);
	}else{
		SendMessage(hwnd, WM_XBINPUT, (WPARAM)XB_RIGHTSHOULDER, (LPARAM) XB_BUTTON_UP);
	}

	return TRUE;
}

//DWORD ComboDetection(XINPUT_STATE