#include "stdafx.h"
#include "TTRobot.h"

HANDLE TTRobotConnect(UINT port)
{
	HANDLE hCom = NULL;
	CHAR strPort[MAX_STRING];

	sprintf(strPort, "COM%d", port);

	hCom = CreateFile(strPort, 
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						0,
						NULL);


	return hCom;
}