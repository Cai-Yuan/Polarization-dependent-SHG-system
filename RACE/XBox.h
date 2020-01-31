#pragma once

#include "stdafx.h"
#include "Math.h"

#include "XInput.h"
#include "XBox.h"
#include "Magnitude.h"
#include "Sign.h"
#pragma comment(lib, "XInput.lib")

#define OFFSET 7000

//List Control Positions
#define LP_CURSOR		0
#define LP_DATAVIEW		1
#define LP_XYZSTAGE		2

//XBox Linker Messages
#define XB_LISTSELUP			1024
#define XB_LISTSELDOWN			1025
#define XB_INPUT				1026
#define XB_LEFTSTICK_X			1027
#define XB_LEFTSTICK_Y			1028
#define XB_RIGHTSTICK_X			1029
#define XB_RIGHTSTICK_Y			1030
#define XB_LEFTTRIGGER			1031
#define XB_RIGHTTRIGGER			1032
#define XB_BUTTON_X				1033
#define XB_BUTTON_Y				1034
#define XB_BUTTON_A				1035
#define XB_BUTTON_B				1036
#define XB_BUTTON_START			1046
#define XB_DPAD					1037
#define XB_DPAD_UP				1038
#define XB_DPAD_DOWN			1039
#define XB_DPAD_RIGHT			1040
#define XB_DPAD_LEFT			1041
#define XB_LEFTTRIGGER_UP		1042
#define XB_LEFTTRIGGER_DOWN		1043
#define XB_RIGHTTRIGGER_UP		1044
#define XB_RIGHTTRIGGER_DOWN	1045
#define XB_LEFTSHOULDER			1047
#define XB_RIGHTSHOULDER		1048
#define XB_BUTTON_BACK			1049


#define XB_BUTTON_DOWN		100
#define XB_BUTTON_UP		101
#define XB_BUTTON_PRESSED	102


DWORD WINAPI XBoxCaptureThread(LPVOID param);
VOID RemoveStickDeadZone(XINPUT_STATE *xInput);
VOID RemoveStickDeadZone(DWORD value, XINPUT_STATE *xInput);
VOID GetLeftStickPosition(INT cnt, FLOAT *x, FLOAT *y);
VOID LogStick(XINPUT_STATE *xInput);

FLOAT CalculateStickPositionXBOX(INT xInput, INT offset, INT sign);
