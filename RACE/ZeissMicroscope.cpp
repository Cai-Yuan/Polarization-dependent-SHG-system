#include "stdafx.h"

#include "ZeissMicroscope.h"


HANDLE GetZeissMicroscope(HWND hOwner, UINT port)
{
	HANDLE hCom = NULL;
	CHAR strPort[MAX_STRING];

	sprintf(strPort, "COM%d", port);

	SetLastError(NULL);
	hCom = CreateFile(strPort, 
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);

	
	if(hCom == INVALID_HANDLE_VALUE){
		ThrowError(hOwner, "ZAxis--INVALID_HANDLE_VALUE");
		ThrowLastError(hOwner);
		CloseHandle(hCom);
		return NULL;
	}

	ZAxisComConfigure(hOwner, hCom);

	return hCom;
}

HANDLE GetZeissMicroscopeStage(HWND hOwner, UINT port)
{
	HANDLE hCom = NULL;
	CHAR strPort[MAX_STRING];

	sprintf(strPort, "COM%d", port);

	SetLastError(NULL);
	hCom = CreateFile(strPort, 
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);

	
	if(hCom == INVALID_HANDLE_VALUE){
		ThrowError(hOwner, "ZAxis--INVALID_HANDLE_VALUE");
		ThrowLastError(hOwner);
		CloseHandle(hCom);
		return NULL;
	}

	ZAxisComConfigure(hOwner, hCom);

	return hCom;
}


HANDLE ZAxisComConnect(HWND hOwner, UINT port)
{
	HANDLE hCom = NULL;
	CHAR strPort[MAX_STRING];

	sprintf(strPort, "COM%d", port);

	SetLastError(NULL);
	hCom = CreateFile(strPort, 
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);

	
	if(hCom == INVALID_HANDLE_VALUE){
		ThrowError(hOwner, "ZAxis--INVALID_HANDLE_VALUE");
		ThrowLastError(hOwner);
		CloseHandle(hCom);
		return NULL;
	}

	return hCom;
}

HANDLE ZAxisComConfigure(HWND hOwner, HANDLE hCom)
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
		ThrowError(hOwner, "ZAxis--SetCommState Failed");
		ThrowLastError(hOwner);
	}

	comTimeOut.ReadIntervalTimeout = 50;
	comTimeOut.ReadTotalTimeoutConstant = 100;
	comTimeOut.ReadTotalTimeoutMultiplier = 10;
	comTimeOut.WriteTotalTimeoutConstant = 50;
	comTimeOut.WriteTotalTimeoutMultiplier = 10;

	SetLastError(NULL);
	if(!SetCommTimeouts(hCom, &comTimeOut)){
		ThrowError(hOwner, "ZAxis--SetCommTimeouts Faield");
		ThrowLastError(hOwner);
	}

	hConfigured = hCom;

	return hConfigured;
}

DWORD ZeissZAxisTest(HWND hOwner)
{

	DWORD comPort = 8;
	HANDLE hZAxis = NULL;
	CHAR strAnswer[MAX_STRING];

	ThrowError(hOwner, "");
	ThrowError(hOwner, "Connecting to ZAxis..");

	hZAxis = ZAxisComConnect(hOwner, comPort);
	if(!hZAxis)
		return FALSE;

	ZAxisComConfigure(hOwner, hZAxis);

	CHAR strInit[MAX_STRING];
	ConcatCommands(ZEISS_STAGE_GETVER, ZEISS_STAGE_GETVER, strInit);


	ZAxisSendCommand(hZAxis, (CHAR *)ZEISS_STAGE_GETVER, strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "1");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr38,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "2");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr38,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "3");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr2,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "4");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr2,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "5");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr36,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "6");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr36,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "7");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr1,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "8");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr1,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "9");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCk1,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "10");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCk1,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "11");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr39,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "12");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr39,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "13");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPTv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "14");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPTv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "15");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPTv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "16");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "17");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPTy2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "18");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPTv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "19");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPTv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "20");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPTy1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "21");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPTy2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "22");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPTv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "23");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPTy1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "24");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr2,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "25");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZp", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "26");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "27");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZa", strAnswer);
	ThrowError(hOwner, strAnswer);

	//Z-Axis Initialization??
	ThrowMessage(hOwner, "28");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZu", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "29");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZl", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "30");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZFc", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "31");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZFc", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "32");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZw", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "33");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZo", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "34");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZp", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "35");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZo", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "36");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "37");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNO", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "38");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNO", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "39");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "40");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPZS", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "41");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "42");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr2,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "43");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "44");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCt12", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "45");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCt12", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "46");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "47");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "48");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCt7", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCt8", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"NULL", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr38,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb9", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr38,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr36,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb9", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCk1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCk1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr39,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPSb9", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"HPCr39,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "NULL");
	ZAxisSendCommand(hZAxis, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	//ThrowMessage(hOwner, "NULL");
	//ZAxisSendCommand(hZAxis, (CHAR *)"NULL", strAnswer);
	//ThrowError(hOwner, strAnswer);

	//ThrowMessage(hOwner, "NULL");
	//ZAxisSendCommand(hZAxis, (CHAR *)"NULL", strAnswer);
	//ThrowError(hOwner, strAnswer);

	CloseHandle(hZAxis);
	ThrowError(hOwner, "Done.");
	return TRUE;
}

DWORD ZeissMicroscopeBoot(HWND hOwner, HANDLE hMicroscope)
{
	CHAR strAnswer[MAX_STRING];

	ThrowError(hOwner, "");
	ThrowError(hOwner, "Begining Microscope Boot Procedure..");

	if(!hMicroscope){
		ThrowMessage(hOwner, "Com Port Not Connected!");
		return FALSE;
	}

	ZAxisSendCommand(hMicroscope, (CHAR *)ZEISS_STAGE_GETVER, strAnswer);
	if(strncmp(strAnswer, "PHAV", 4)){
		ThrowMessage(hOwner, "Could Not Connect to ZeissMicroscope");
		ThrowMessage(hOwner, "Ensure Power is ON and AxioVision is closed");
		return FALSE;
	}
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "1");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr38,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "2");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr38,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "3");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr2,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "4");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr2,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "5");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr36,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "6");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr36,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "7");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr1,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "8");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr1,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "9");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCk1,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "10");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCk1,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "11");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr39,0", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "12");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr39,2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "13");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPTv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "14");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPTv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "15");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPTv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "16");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "17");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPTy2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "18");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPTv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "19");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPTv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "20");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPTy1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "21");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPTy2", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "22");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPTv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "23");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPTy1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "24");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr2,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "25");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZp", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "26");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZv", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "27");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZa", strAnswer);
	ThrowError(hOwner, strAnswer);

	//Z-Axis Initialization??
	ThrowMessage(hOwner, "28");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZu", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "29");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZl", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "30");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZFc", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "31");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZFc", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "32");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZw", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "33");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZo", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "34");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZp", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "35");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZo", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "36");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "37");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNO", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "38");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNO", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "39");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "40");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPZS", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "41");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "42");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr2,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "43");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "44");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCt12", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "45");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCt12", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "46");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCv1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "47");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "48");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCt7", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "49");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "50");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCt8", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "51");
	ZAxisSendCommand(hMicroscope, (CHAR *)"NULL", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "52");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "53");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "54");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr38,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "55");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "56");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "57");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb9", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "58");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr38,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "59");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "60");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "61");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr36,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "62");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb9", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "63");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCk1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "64");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "65");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCk1,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "66");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr39,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "67");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb6", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "68");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "69");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPSb9", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "70");
	ZAxisSendCommand(hMicroscope, (CHAR *)"HPCr39,1", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowMessage(hOwner, "71");
	ZAxisSendCommand(hMicroscope, (CHAR *)"FPNr", strAnswer);
	ThrowError(hOwner, strAnswer);

	ThrowError(hOwner, "Done.");
	return TRUE;
}

DWORD ZAxisSendCommand(HANDLE hDevice, CHAR *strMessage, CHAR *strRecieved)
{
	DWORD nBytesRead = NULL;
	DWORD nBytesWritten = NULL;
	DWORD strLen = strlen((CHAR *)strMessage);
	CHAR strCommand[MAX_STRING];
	CHAR strAnswer[MAX_STRING];
	
	//strlen所作的仅仅是一个计数器的工作，它从内存的某个位置（可以是字符串开头，中间某个位置，甚至是某个不确定的内存区域）开始扫描，直到碰到第一个字符串结束符'\0'为止，然后返回计数器值。
	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(strMessage[strLen] != 0x0D){
		memcpy(strCommand, strMessage, MAX_STRING*sizeof(CHAR));
		strCommand[strLen+0] = 0x0D;
		strCommand[strLen+1] = NULL;
	}
	else
		sprintf(strCommand, "%s", strMessage);

	strLen = strlen(strCommand);

	WriteFile(hDevice, strCommand, strLen, &nBytesWritten, NULL);

	if(strRecieved){
		ReadFile(hDevice, strAnswer, 50, &nBytesRead, NULL);
		sprintf(strRecieved, "%s", strAnswer);
	}

	return TRUE;
}

DWORD SendComCommand(HANDLE hDevice, CHAR *strMessage, CHAR *strRecieved)
{
	DWORD nBytesRead = NULL;
	DWORD nBytesWritten = NULL;
	DWORD strLen = strlen((CHAR *)strMessage);
	CHAR strCommand[MAX_STRING];
	CHAR strAnswer[MAX_STRING];

	memset(strAnswer, NULL, MAX_STRING*sizeof(CHAR));

	if(strMessage[strLen] != 0x0D){
		memcpy(strCommand, strMessage, MAX_STRING*sizeof(CHAR));
		strCommand[strLen+0] = 0x0D;
		strCommand[strLen+1] = NULL;
	}
	else
		sprintf(strCommand, "%s", strMessage);

	strLen = strlen(strCommand);

	WriteFile(hDevice, strCommand, strLen, &nBytesWritten, NULL);
	ReadFile(hDevice, strAnswer, 50, &nBytesRead, NULL);

	if(strRecieved)
		sprintf(strRecieved, "%s", strAnswer);

	return TRUE;
}

DWORD ConcatCommands(CHAR *str1, CHAR *str2, CHAR *strConcat)
{
	DWORD strLen1 = strlen(str1);
	DWORD strLen2 = strlen(str2);

	CHAR s1[MAX_STRING];
	CHAR s2[MAX_STRING];

	memcpy(s1, str1, strlen(str1)*sizeof(CHAR));
	memcpy(s2, str2, strlen(str2)*sizeof(CHAR));

	if(str1[strLen1] != 0x0D){
		s1[strLen1+0] = 0x0D;
		s1[strLen1+1] = 0x00;
	}

	if(str2[strLen2] != 0x0D){
		s2[strLen2+0] = 0x0D;
		s2[strLen2+1] = 0x00;
	}

	sprintf(strConcat, "%s%s", s1, s2);

	return strlen(strConcat);
}