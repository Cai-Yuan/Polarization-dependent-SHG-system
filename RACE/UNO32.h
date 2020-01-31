#pragma once
#include "stdafx.h"

//#include "CircularBuffer.h"
#include "Object.h"
#include "Microscope.h"

#include "A3200.h"

typedef struct UNO32DRIVE_
{
	CHAR cDrive;
	FLOAT hz;
	INT dir;
}UDRIVE;

typedef struct UNO32_3DPOINT{
	long long int x;
	long long int y;
	long long int z;
}UNO32_3DPOINT;



typedef struct WNDUNO32_
{

	HWND hwnd;
	HWND hOwner;
	HWND hShutter;
	HWND hDataView;
	HWND hCamProp;
	HANDLE hDevice;
	HANDLE hAero;

	UDRIVE driveA;
	UDRIVE driveB;
	UDRIVE driveC;

	//FLOATCB bufferA;
	//FLOATCB bufferB;
	//FLOATCB bufferC;

	FOV fov;

	DWORD wFrame;
	DWORD hFrame;

	FLOAT xFOV;
	FLOAT yFOV;

	FLOAT xStick;
	FLOAT yStick;
	FLOAT zStick;

	FLOAT xVelFine;
	FLOAT xVelCoarse;
	FLOAT yVelFine;
	FLOAT yVelCoarse;
	FLOAT zVelFine;
	FLOAT zVelCoarse;

	FLOAT xVelMax;
	FLOAT yVelMax;
	FLOAT zVelMax;

	FLOAT xVel;
	FLOAT yVel;
	FLOAT zVel;
	FLOAT xVelPrev;
	FLOAT yVelPrev;
	FLOAT zVelPrev;

	FLOAT pixPerUm;

	FLOAT roiDiameter;

	DWORD slice;
	DWORD nSlice;
	FLOAT stepSize;

	HWND hBtnZero;

	HWND hEditXPos;
	HWND hEditYPos;
	HWND hEditZPos;
	HWND hEditXVel;
	HWND hEditYVel;
	HWND hEditZVel;

	HWND hEditXVelFine;
	HWND hEditXVelCoarse;
	HWND hEditYVelFine;
	HWND hEditYVelCoarse;
	HWND hEditZVelFine;
	HWND hEditZVelCoarse;

	HWND hEditXVelAuto;
	HWND hEditYVelAuto;
	HWND hEditZVelAuto;

	HWND hBtnCapture;
	HWND hEditNSlice;
	HWND hEditStepSize;

	HWND hCheckXAxis;
	HWND hCheckYAxis;
	HWND hCheckZAxis;

	HWND hEditPixPerUm;

	HWND hEditRoiDiameter;
	//HWND hSpinRoiDiameter;

	HWND hCheckZDelay;

	HWND hEditSampleRate;

	FLOAT sampleRate;
	FLOAT xScale;
	FLOAT yScale;
	FLOAT zScale;
	FLOAT xPolStage;
	FLOAT yPolStage;
	FLOAT zPolStage;
	FLOAT xPolStick;
	FLOAT yPolStick;
	FLOAT zPolStick;

	DWORD flgThread;
	DWORD flgShutter;
	DWORD flgLeftShoulder;
	DWORD flgRightShoulder;
	DWORD flgAButton;
	DWORD flgBButton;
	DWORD flgXButton;
	DWORD flgYButton;
	DWORD flgStartButton;
	DWORD flgBackButton;
	DWORD flgLeftTrigger;
	DWORD flgRightTrigger;

	DWORD flgZero;
	DWORD flgXInput;
	DWORD flgYInput;
	DWORD flgZInput;
	DWORD flgInitMoveTo;
	DWORD flgMoveTo;
	DWORD flgMoveToZ;
	DWORD flgZDelay;
	DWORD flgAuto;
	DWORD flgAPosition;
	DWORD flgBPosition;
	DWORD flgXPosition;
	DWORD flgYPosition;
	DWORD flgVelUpdate;
	DWORD flgDPadLeft;
	DWORD flgDPadRight;
	DWORD flgDPadUp;
	DWORD flgDPadDown;
	DWORD flgZStack;
	DWORD flgFrameIn;

	WPARAM wParam;
	LPARAM lParam;

	UNO32_3DPOINT ptCurrent;
	UNO32_3DPOINT ptMoveTo;
	UNO32_3DPOINT ptA;
	UNO32_3DPOINT ptB;
	UNO32_3DPOINT ptX;
	UNO32_3DPOINT ptY;

	DOUBLE_3DPOINT dptCurrent;
	DOUBLE_3DPOINT dptMoveTo;
	DOUBLE_3DPOINT dptA;
	DOUBLE_3DPOINT dptB;
	DOUBLE_3DPOINT dptX;
	DOUBLE_3DPOINT dptY;
	DOUBLE_3DPOINT *dptSet;

	OBJECTSET os;

	PDMFRAME pdmIn;
	CHAR strFolder[256];
	UNO32_3DPOINT ptZRef;


}WNDUNO32;



DWORD UpdateDriveDirection(HANDLE hDevice, FLOAT velocity, UDRIVE *uDrive);


INT_PTR CALLBACK UNO32WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD XBoxInputUNO32(WNDUNO32 *lpWndUNO32, WPARAM wParam, LPARAM lParam);
DWORD InitUNO32Device(WNDUNO32 *lpWndUNO32);
DWORD GetUNO32DevicePosition(WNDUNO32 *lpWndUNO32);
DWORD GetUNO32DevicePosition(HANDLE hDevice, long long int *x, long long int *y, long long int *z);
DWORD GetUNO32DevicePosition(HANDLE hDevice, UNO32_3DPOINT *pt);
DWORD WINAPI A3200Thread(LPVOID param);
DWORD WINAPI UNO32Thread(LPVOID param);
DWORD UpdateUNO32Velocity(WNDUNO32 *lpWndUNO32);


DWORD GetUNO32DevicePositionX(HANDLE hDevice, UNO32_3DPOINT *pt);
DWORD GetUNO32DevicePositionY(HANDLE hDevice, UNO32_3DPOINT *pt);
DWORD GetUNO32DevicePositionZ(HANDLE hDevice, UNO32_3DPOINT *pt);

long long int GetUNO32AxisPosition(HANDLE hDevice, CHAR axis);
DWORD UpdateUNO32PositionDisplay(UNO32_3DPOINT pt, WNDUNO32 *lpWndUNO32);
DWORD UpdateUNO32PositionDisplay(DOUBLE_3DPOINT pt, WNDUNO32 *lpWndUNO32);
DWORD SetUNO32AxisVelocity(HANDLE hDevice, CHAR axis, float velocity);
DWORD SetUNO32DeviceVelocity(HANDLE hDevice, float xVel, float yVel, float zVel);


DWORD SetUNO32AxisPosition(HANDLE hDevice, CHAR axis, long long int pos);

DWORD SetUNO32DevicePositionZ(HANDLE hDevice, long long int z);
DWORD SetUNO32DevicePositionXY(HANDLE hDevice, long long int x, long long int y);
DWORD SetUNO32DevicePosition(HANDLE hDevice, long long int x, long long int y, long long int z);

DWORD SetUNO32DevicePosition(HANDLE hDevice, UNO32_3DPOINT pt);
DWORD SetUNO32DevicePositionXY(HANDLE hDevice, UNO32_3DPOINT pt);
DWORD SetUNO32DevicePositionZ(HANDLE hDevice, UNO32_3DPOINT pt);

DWORD SetUNO32DeviceVelocityZ(HANDLE hDevice, float zVel);
DWORD SetUNO32DeviceVelocityXY(HANDLE hDevice, float xVel, float yVel);

FLOAT GetZDistance(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2);
DWORD Compare(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2);
DWORD CompareXY(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2);
DWORD CompareZ(UNO32_3DPOINT pt1, UNO32_3DPOINT pt2);
DWORD GetUNO32NavigationVector(UNO32_3DPOINT ptCurr, UNO32_3DPOINT ptDest, FPOINT center, VECTOR *vector);
DWORD GetUNO32VelocityVector(FLOAT velX, FLOAT velY, VECTOR *vector, TEXTR *text);
DWORD GetUNO32VelocityVector(WNDUNO32 lpWndUNO32, FLOAT velX, FLOAT velY, VECTOR *vector, TEXTR *text);
DWORD GetUNO32ZAxisIndicator(WNDUNO32 *lpWndUNO32, CIRCLE *circle, TEXTR *text);
DWORD CreateUNO32Objects(WNDUNO32 *lpWndUNO32);



DWORD GetVector(UNO32_3DPOINT ptHead, UNO32_3DPOINT ptTail, VECTOR *v);
INT GetXYDisplacement(UNO32_3DPOINT ptHead, UNO32_3DPOINT ptTail);
INT GetZDisplacement(UNO32_3DPOINT ptHead, UNO32_3DPOINT ptTail);

FLOAT CalculateStickPositionXBOX(INT xInput, INT offset, INT sign);




//A3200 Wrapper Functions

//DWORD GetA3200Position(A3200Handle handle, DOUBLE_3DPOINT *dpt);

DWORD Compare(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2);
DWORD CompareXY(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2);
DWORD CompareZ(DOUBLE_3DPOINT pt1, DOUBLE_3DPOINT pt2);

DWORD GetUNO32NavigationVector(DOUBLE_3DPOINT ptCurr, DOUBLE_3DPOINT ptDest, FPOINT center, VECTOR *vector);

FLOAT GetXYDisplacement(DOUBLE_3DPOINT ptHead, DOUBLE_3DPOINT ptTail);
FLOAT GetZDisplacement(DOUBLE_3DPOINT ptHead, DOUBLE_3DPOINT ptTail);

DWORD GetVector(DOUBLE_3DPOINT ptHead, DOUBLE_3DPOINT ptTail, VECTOR *v);

DWORD Truncate(double d, float *f);
FLOAT Truncate(DOUBLE d);
DWORD Truncate(DOUBLE_3DPOINT dptIn, DOUBLE_3DPOINT *dptOut);

DWORD Scale(DOUBLE_3DPOINT dptIn, DOUBLE_3DPOINT *dptOut, FLOAT scale);

FLOAT Truncate1(DOUBLE d);
FLOAT Truncate2(DOUBLE d);
FLOAT Truncate3(DOUBLE d);
FLOAT Truncate4(DOUBLE d);
FLOAT Truncate5(DOUBLE d);
