#include "stdafx.h"
#include "XBox.h"
#include "XBoxLinker.h"
#include "DataView.h"
#include "DataManager.h"

VOID RemoveStickDeadZone(XINPUT_STATE *xInput)
{

	  // Controller is connected
	  // Check to make sure we are not moving during the dead zone
	  if( (xInput->Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			xInput->Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(xInput->Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			xInput->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
			{    
				  xInput->Gamepad.sThumbLX = 0;
				  xInput->Gamepad.sThumbLY = 0;
			}
 
	  if( (xInput->Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			xInput->Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(xInput->Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			xInput->Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
			{
				  xInput->Gamepad.sThumbRX = 0;
				  xInput->Gamepad.sThumbRY = 0;
			}


}

VOID RemoveStickDeadZone(DWORD value, XINPUT_STATE *xInput) 
{

	  // Controller is connected
	  // Check to make sure we are not moving during the dead zone

	if(RangeInt((INT)xInput->Gamepad.sThumbLX, (INT)-value, (INT)value)){
		xInput->Gamepad.sThumbLX = 0;
	}

	if(RangeInt((INT)xInput->Gamepad.sThumbLY, (INT)-value, (INT)value)){
		xInput->Gamepad.sThumbLY = 0;
	}

	if(RangeInt((INT)xInput->Gamepad.sThumbRX, (INT)-value, (INT)value)){
		xInput->Gamepad.sThumbRX = 0;
	}

	if(RangeInt((INT)xInput->Gamepad.sThumbRY, (INT)-value, (INT)value)){
		xInput->Gamepad.sThumbRY = 0;
	}


}

VOID GetLeftStickPosition(INT cnt, FLOAT *x, FLOAT *y)
{
	DWORD xPos = 0;
	DWORD yPos = 0;

	XINPUT_STATE xInput;

	for(INT i = 0; i < cnt; i++)
	{
		XInputGetState(0, &xInput);
		xPos += xInput.Gamepad.sThumbLX;
		yPos += xInput.Gamepad.sThumbLY;	

	}

	xPos = (xPos / cnt);
	yPos = (yPos / cnt);

}

VOID LogStick(XINPUT_STATE *xInput)
{
	FLOAT xLog = 0;
	FLOAT yLog = 0;

	xLog = Magnitude((FLOAT)xInput->Gamepad.sThumbLX);
	yLog = Magnitude((FLOAT)xInput->Gamepad.sThumbLY);

	

	xLog = log((FLOAT) xLog);
	yLog = log((FLOAT) yLog);

	xInput->Gamepad.sThumbLX = xLog * Sign(xInput->Gamepad.sThumbLX);
	xInput->Gamepad.sThumbLY = yLog * Sign(xInput->Gamepad.sThumbLY);

}

FLOAT CalculateStickPositionXBOX(INT xInput, INT offset, INT sign)
{
	//FLOAT pos = 0.00;
	FLOAT coef = (abs((FLOAT)(xInput))-OFFSET)/(FLOAT)32767;

	coef = coef*sign;

	//pos = ((FLOAT)(*x)*coef/((FLOAT)32767-OFFSET));

	return ((FLOAT)(xInput)*coef/((FLOAT)32767-OFFSET));
	//return pos;
}

DWORD WINAPI XBoxCaptureThread(LPVOID param)
{
	//HWND *hWND = (HWND *) param;

	HWND hwnd = (HWND) param;

	//hwnd = *hWND;
	HWND hXBoxLinker = (HWND) SendMessage(hwnd, WM_GETHWND, (WPARAM)HWND_XBOXLINKER, NULL);
	HWND hActive = NULL;
	//memcpy(&hwnd, (VOID *)hWND, sizeof(HWND));
	INT count = 0;

	//[Hz] - Rate
	INT rate = 20;
	INT delay = (1000/rate);

	FLOAT xMove = 0;
	FLOAT yMove = 0;

	POINT ptMove;
	POINT ptCursor;
	FLOAT xLeftStick;
	FLOAT yLeftStick;
	DWORD dwResult;
	XINPUT_STATE xInput;

	BOOL flagCursor = FALSE;
	BOOL flagData = !flagCursor;

	//Initialize Vibration Parameters
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 65535;
	vibration.wRightMotorSpeed = 65535;


	//Initialize Stop-Vibration Parameters
	XINPUT_VIBRATION stopvibration;
	ZeroMemory(&stopvibration, sizeof(XINPUT_VIBRATION));
	stopvibration.wLeftMotorSpeed = 0;
	stopvibration.wRightMotorSpeed = 0;

	delay = (1000/rate);

	while(1){

		ZeroMemory(&xInput, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(0, &xInput);
	
		RemoveStickDeadZone(7000, &xInput);

		SendMessage(hwnd, XB_INPUT, (WPARAM) &xInput, NULL);

		Sleep(delay);
	}

	return TRUE;

}

