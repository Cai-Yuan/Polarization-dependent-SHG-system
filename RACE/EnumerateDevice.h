#pragma once

#include "stdafx.h"

//Struct
//Connect To Device Threaded [CTDT]
typedef struct CTDT_{
	HWND hwnd;
	DWORD deviceType;
	CHAR strRequest[MAX_STRING];
	CHAR strID[MAX_STRING];
	DWORD baudRate;
	DWORD msWait;
}CTDT;

ULONG GetOSVersion();
UINT EnumPorts(CHAR *portList);
HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID);
//HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID, INT msDelay);
HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID, DWORD baudRate);
HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait);
//DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate);
//DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait);

HANDLE SerialConfigure(HANDLE hCom);
HANDLE SerialConfigure(HANDLE hCom, DWORD baudRate);
DWORD SerialSend(HANDLE hDevice, CHAR *strMessage, CHAR *strRecieved);
DWORD SerialSend(HANDLE hDevice, CHAR *strSend, CHAR *strRecieved, DWORD tSleep);
DWORD SerialSendEx(HANDLE hDevice, DWORD lenAnswer, CHAR *strSend, CHAR *strRecieved);

DWORD WINAPI ConnectToDeviceThreadedProc(LPVOID param);

DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD deviceType);
DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate);
DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait);
DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait, UCHAR deviceType);

DWORD FillCTDT(CTDT *ctdt, HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait, DWORD deviceType);