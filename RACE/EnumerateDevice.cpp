#include "stdafx.h"
#include "EnumerateDevice.h"

DWORD FillCTDT(CTDT *ctdt, HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait, DWORD deviceType)
{
	if(!ctdt){
		return FALSE;
	}
	ctdt->hwnd = hwnd;
	sprintf(ctdt->strID, "%s", strID);
	sprintf(ctdt->strID, "%s", strID);
	sprintf(ctdt->strRequest, "%s", strRequest);
	ctdt->baudRate = baudRate;
	ctdt->msWait = msWait;
	ctdt->deviceType = deviceType;

	return TRUE;
}

//在OSVERSIONINFO数据结构中包含操作系统版本信息。这些信息包括主要和次要版本号，一个内部版本号，一个平台标识符，以及操作系统的说明性文本。GetVersionEx函数将使用这个结构体。
ULONG GetOSVersion() 
{
	OSVERSIONINFO osvi;
	memset( &osvi, 0, sizeof(OSVERSIONINFO) );
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	// get running os
	GetVersionEx( &osvi );

	return osvi.dwPlatformId;
}

UINT EnumPorts(CHAR *portList)
{
	//portlist.clear();
	UINT com = 0;
	CHAR strCom[MAX_STRING];
	UINT nPort = 0;

	// use querydosdevice on NT/2000/xp (no need to open ports...)
	if (GetOSVersion() == VER_PLATFORM_WIN32_NT)
	{
		TCHAR buf[65535];
		memset(buf, NULL, 65535*sizeof(TCHAR));
		LONG len = QueryDosDevice( 0, buf, 65535 );

		for (LONG n=0; n<len; n++)
		{
			// if found "COM", then add number to list
			if (_tcsnicmp( &buf[n], _T("COM"), 3 ) == 0){
				portList[nPort] = buf[n+3];
				sscanf(&buf[n+3], "%d", &com);
				portList[nPort] = com;
				nPort++;
			}

			// find next null pointer
			while (buf[n] != _T('\0'))
				n++;
		}
	}
	// else, open port 1-255
	else
	{
		for (INT i=1; i<255; i++)
		{
			CHAR str[256];
			//CString str;
			sprintf(str, "\\\\.\\COM%d", i);
			//str.Format( _T("\\\\.\\COM%d"), i );

			// try to open port
			bool result = false;
			
			HANDLE handle = ::CreateFile( str, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0 );
			
			if (handle == INVALID_HANDLE_VALUE)
			continue;

			CloseHandle( handle );
  		
			portList[nPort] = i;
			nPort++;
			//portlist.push_back( i );
		}
	}

	nPort = nPort;
	nPort = nPort;

	return nPort;
}

HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID)
{
	INT i = NULL;
	HANDLE hCom = NULL;
	DWORD nPort = NULL;
	//DWORD strLen = strlen(strID);
	CHAR portList[MAX_STRING];
	CHAR strPort[MAX_STRING];
	CHAR strRecieved[MAX_STRING];
	
	nPort = EnumPorts(portList)+1;

	DWORD error;

	for(i = 0; i < nPort; i++){

		sprintf(strPort, "\\\\.\\COM%d", portList[i]);

		SetLastError(NULL);
		hCom = CreateFile(strPort, 
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		error = GetLastError();

		if(hCom == INVALID_HANDLE_VALUE){
			CloseHandle(hCom);
			hCom = NULL;
		}
		else{
			SerialConfigure(hCom);
			SerialSend(hCom, strRequest, strRecieved, 100);
			if(!strncmp(strID, strRecieved, strlen(strID))){
				*port = portList[i];
				return hCom;
			}
			else{
				CloseHandle(hCom);
				hCom = NULL;
			}
		}
	}

	return hCom;
}

HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID, DWORD baudRate)
{
	INT i = NULL;
	HANDLE hCom = NULL;
	DWORD nPort = NULL;
	//DWORD strLen = strlen(strID);
	CHAR portList[MAX_STRING];

	CHAR strPort[MAX_STRING];
	CHAR strRecieved[MAX_STRING];
	
	nPort = EnumPorts(portList)+1;

	DWORD error;

	//CHAR strIDTemp[MAX_STRING];
	//sprintf(strIDTemp, "%s", strID);


	for(i = 0; i < nPort; i++){

		sprintf(strPort, "\\\\.\\COM%d", portList[i]);

		SetLastError(NULL);
		hCom = CreateFile(strPort, 
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		error = GetLastError();

		if(hCom == INVALID_HANDLE_VALUE){
			CloseHandle(hCom);
			hCom = NULL;
		}
		else{
			SerialConfigure(hCom, baudRate);
			//Sleep(2500);
			SerialSend(hCom, strRequest, strRecieved, 100);
			if(!strncmp(strID, strRecieved, strlen(strID))){
				*port = portList[i];
				return hCom;
			}
			else{
				CloseHandle(hCom);
				hCom = NULL;
			}
		}
	}

	return hCom;
}

HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait)
{
	INT i = NULL;
	HANDLE hCom = NULL;
	DWORD nPort = NULL;
	//DWORD strLen = strlen(strID);
	CHAR portList[MAX_STRING];

	CHAR strPort[MAX_STRING];
	CHAR strRecieved[MAX_STRING];
	
	nPort = EnumPorts(portList)+1;

	DWORD error;


	for(i = 0; i < nPort; i++){

		sprintf(strPort, "\\\\.\\COM%d", portList[i]);

		SetLastError(NULL);
		hCom = CreateFile(strPort, 
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		error = GetLastError();

		if(hCom == INVALID_HANDLE_VALUE){
			CloseHandle(hCom);
			hCom = NULL;
		}
		else{
			SerialConfigure(hCom, baudRate);
			Sleep(msWait);
			SerialSend(hCom, strRequest, strRecieved, 100);
			if(!strncmp(strID, strRecieved, strlen(strID))){
				*port = portList[i];
				return hCom;
			}
			else{
				CloseHandle(hCom);
				hCom = NULL;
			}
		}
	}

	return hCom;
}

DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, UCHAR deviceType)
{

	ConnectToDeviceThreaded(hwnd, strRequest, strID, baudRate, (DWORD)0);

	return FALSE;
}

DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate)
{

	ConnectToDeviceThreaded(hwnd, strRequest, strID, baudRate, (DWORD)0);

	return FALSE;
}

////Sends the connected handle to 'hwnd'
DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait)
{
	if(!strRequest && !strID){
		return FALSE;
	}


	CTDT *ctdt = (CTDT *)malloc(sizeof(CTDT));

	FillCTDT(ctdt, hwnd, strRequest, strID, baudRate, msWait, /*deviceType*/NULL);
	//ctdt->hwnd = hwnd;
	//sprintf(ctdt->strID, "%s", strID);
	//sprintf(ctdt->strRequest, "%s", strRequest);
	//ctdt->baudRate = baudRate;
	//ctdt->msWait = msWait;

	CreateThread(NULL, NULL, ConnectToDeviceThreadedProc, ctdt, NULL, NULL);

	return FALSE;
}



//Sends the connected handle to 'hwnd'
DWORD ConnectToDeviceThreaded(HWND hwnd, CHAR *strRequest, CHAR *strID, DWORD baudRate, DWORD msWait, DWORD deviceType)
{
	if(!strRequest || !strID){
	//if(!strRequest && !strID){
		return FALSE;
	}

	CTDT *ctdt = (CTDT *)malloc(sizeof(CTDT));
	FillCTDT(ctdt, hwnd, strRequest, strID, baudRate, msWait, deviceType);

	CreateThread(NULL, NULL, ConnectToDeviceThreadedProc, ctdt, NULL, NULL);

	return FALSE;
}



DWORD WINAPI ConnectToDeviceThreadedProc(LPVOID param)
{
	if(!param){
		return FALSE;
	}

	CTDT *ptrctdt = (CTDT *)param;


	HANDLE handle;
	CHAR port;
	CTDT ctdt = {0};
	memcpy(&ctdt, ptrctdt, sizeof(CTDT));

	if(ptrctdt){
		free(ptrctdt);
	}

	if(ctdt.msWait){
		handle = ConnectToDevice(&port, ctdt.strRequest, ctdt.strID, ctdt.baudRate, ctdt.msWait);
	}else{
		handle = ConnectToDevice(&port, ctdt.strRequest, ctdt.strID, ctdt.baudRate);
	}
	if(handle){
		SendMessage(ctdt.hwnd, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM) handle);
		//SendMessage(ctdt.hwnd, WM_SETHANDLE, HANDLE_DEVICE, (LPARAM) handle);
		return TRUE;
	}

	return TRUE;
}


/*
//Connects to a device and provides a delay between opening the port and requesting an ID
HANDLE ConnectToDevice(CHAR *port, CHAR *strRequest, CHAR *strID, INT msDelay)
{
	INT i = NULL;
	HANDLE hCom = NULL;
	DWORD nPort = NULL;
	CHAR portList[MAX_STRING];

	CHAR strPort[MAX_STRING];
	CHAR strRecieved[MAX_STRING];
	
	nPort = EnumPorts(portList)+1;

	DWORD error;

	*port = NULL;

	for(i = 0; i < nPort; i++){

		sprintf(strPort, "\\\\.\\COM%d", portList[i]);

		SetLastError(NULL);
		hCom = CreateFile(strPort, 
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
		error = GetLastError();

		if(hCom == INVALID_HANDLE_VALUE){
			CloseHandle(hCom);
			hCom = NULL;
		}
		else{
			SerialConfigure(hCom);
			Sleep(msDelay);
			SerialSend(hCom, strRequest, strRecieved);
			if(!strncmp(strID, strRecieved, strlen(strID))){
				*port = portList[i];
				return hCom;
			}
			else{
				CloseHandle(hCom);
				hCom = NULL;
			}
		}
	}

	return hCom;
}*/

HANDLE SerialConfigure(HANDLE hCom)
{
	HANDLE hConfigured = NULL;

	COMMCONFIG comConfig = {0};
	COMMTIMEOUTS comTimeOut = {0};

	GetCommState(hCom, &comConfig.dcb);

	comConfig.dcb.BaudRate = CBR_9600;
	comConfig.dcb.ByteSize = 8;
	comConfig.dcb.StopBits = ONESTOPBIT;
	comConfig.dcb.Parity = NOPARITY;

	SetLastError(NULL);
	if(!SetCommState(hCom, &comConfig.dcb)){
		//ThrowError(hOwner, "ZAxis--SetCommState Failed");
		//ThrowLastError(hOwner);
	}

	comTimeOut.ReadIntervalTimeout = 50;
	comTimeOut.ReadTotalTimeoutConstant = 50;
	comTimeOut.ReadTotalTimeoutMultiplier = 10;
	comTimeOut.WriteTotalTimeoutConstant = 50;
	comTimeOut.WriteTotalTimeoutMultiplier = 10;

	SetLastError(NULL);
	if(!SetCommTimeouts(hCom, &comTimeOut)){
		//ThrowError(hOwner, "ZAxis--SetCommTimeouts Faield");
		//ThrowLastError(hOwner);
	}

	hConfigured = hCom;

	return hConfigured;
}

HANDLE SerialConfigure(HANDLE hCom, DWORD baudRate)
{
	HANDLE hConfigured = NULL;

	COMMCONFIG comConfig = {0};
	COMMTIMEOUTS comTimeOut = {0};

	GetCommState(hCom, &comConfig.dcb);
	comConfig.dcb.BaudRate = baudRate;
	comConfig.dcb.ByteSize = 8;
	comConfig.dcb.StopBits = ONESTOPBIT;
	comConfig.dcb.Parity = NOPARITY;

	SetLastError(NULL);
	if(!SetCommState(hCom, &comConfig.dcb)){
		//ThrowError(hOwner, "ZAxis--SetCommState Failed");
		//ThrowLastError(hOwner);
	}

	comTimeOut.ReadIntervalTimeout = 50;
	comTimeOut.ReadTotalTimeoutConstant = 50;
	comTimeOut.ReadTotalTimeoutMultiplier = 10;
	comTimeOut.WriteTotalTimeoutConstant = 50;
	comTimeOut.WriteTotalTimeoutMultiplier = 10;

	SetLastError(NULL);
	if(!SetCommTimeouts(hCom, &comTimeOut)){
		//ThrowError(hOwner, "ZAxis--SetCommTimeouts Faield");
		//ThrowLastError(hOwner);
	}

	hConfigured = hCom;

	return hConfigured;
}

DWORD SerialSend(HANDLE hDevice, CHAR *strSend, CHAR *strRecieved)
{	
	
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	DWORD strLen = NULL;
	DWORD nBytesRead = NULL;
	DWORD nBytesWritten = NULL;

	if(!hDevice)
		return FALSE;
	
	strLen = strlen((CHAR *)strSend);

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(strSend[strLen] != 0x0D){
		memcpy(str, strSend, MAX_STRING*sizeof(CHAR));
		str[strLen+0] = 0x0D;
		str[strLen+1] = NULL;
	}
	else
		sprintf(str, "%s", strSend);

	strLen = strlen(str);

	WriteFile(hDevice, str, strLen, &nBytesWritten, NULL);

	if(strRecieved){
		Sleep(5);
		ReadFile(hDevice, strAnswer, 50, &nBytesRead, NULL);
		
		//ReadStringTo(strAnswer, strRecieved, 13);
		sprintf(strRecieved, "%s", strAnswer);
	}

	return TRUE;
}

DWORD SerialSendEx(HANDLE hDevice, DWORD lenAnswer, CHAR *strSend, CHAR *strRecieved)
{	
	
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	DWORD strLen = NULL;
	DWORD nBytesRead = NULL;
	DWORD nBytesWritten = NULL;

	if(!hDevice)
		return FALSE;
	
	strLen = strlen((CHAR *)strSend);

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(strSend[strLen-1] != 0x0D){
		memcpy(str, strSend, MAX_STRING*sizeof(CHAR));
		str[strLen+0] = 0x0D;
		str[strLen+1] = NULL;
	}
	else
		sprintf(str, "%s", strSend);

	strLen = strlen(str);

	WriteFile(hDevice, str, strLen, &nBytesWritten, NULL);

	if(strRecieved){
		Sleep(5);
		ReadFile(hDevice, strAnswer, MAX_STRING, &nBytesRead, NULL);
		//ReadStringTo(strAnswer, strRecieved, 13);
		//sprintf(strRecieved, "%s", strAnswer);
		memcpy(strRecieved, strAnswer, lenAnswer);
	}

	return TRUE;
}

DWORD SerialSend(HANDLE hDevice, CHAR *strSend, CHAR *strRecieved, DWORD tSleep)
{	
	
	CHAR str[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	DWORD strLen = NULL;
	DWORD nBytesRead = NULL;
	DWORD nBytesWritten = NULL;

	if(!hDevice)
		return FALSE;
	
	strLen = strlen((CHAR *)strSend);

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(strSend[strLen-1] != 0x0D){
		memcpy(str, strSend, MAX_STRING*sizeof(CHAR));
		str[strLen+0] = 0x0D;
		str[strLen+1] = NULL;
	}
	else
		sprintf(str, "%s", strSend);

	strLen = strlen(str);

	WriteFile(hDevice, str, strLen, &nBytesWritten, NULL);

	if(strRecieved){
		Sleep(tSleep);
		ReadFile(hDevice, strAnswer, MAX_STRING, &nBytesRead, NULL);
		sprintf(strRecieved, "%s", strAnswer);
	}

	return TRUE;
}
