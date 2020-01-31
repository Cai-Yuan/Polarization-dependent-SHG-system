#include "stdafx.h"
#include "XBox.h"
#include "XBoxLinker.h"
#include "EnumerateDevice.h"

#define MAX_DEVICESTRING	65535


DWORD DiscoverXBox(HWND hParent)
{
	DWORD dw = NULL;
	static HWND hwnd;
	hwnd = hParent;
	HANDLE hThread;
	hThread = CreateThread(NULL, NULL, XBoxCaptureThread, (VOID *)&hwnd, NULL, NULL);

	return dw;
}

DWORD DiscoverDevices(HWND hFrame, HWND hManifest)
{
	DWORD i = NULL;
	DWORD dw = NULL;
	DWORD len = NULL;
	CHAR strDevices[MAX_DEVICESTRING];
	CHAR strDevice[MAX_STRING];

	//DiscoverXBox(hFrame);
	dw = QueryDosDevice(NULL, strDevices, MAX_DEVICESTRING);//The function can obtain the current mapping for a particular MS-DOS device name. The function can also obtain a list of all existing MS-DOS device names.
	                                          //the return value is the number of TCHARs stored into the buffer pointed to by lpTargetPath
	//Post Devices as Messages
	for(i = 0; i < dw; i++)
	{
		len = strlen((strDevices + i));//strlen所作的仅仅是一个计数器的工作，它从内存的某个位置（可以是字符串开头，中间某个位置，甚至是某个不确定的内存区域）开始扫描，直到碰到第一个字符串结束符'\0'为止，然后返回计数器值(长度不包含'\0')
		memset(strDevice, NULL, MAX_STRING*sizeof(CHAR));
		memcpy(strDevice, (strDevices + i), len*sizeof(CHAR));
		
		
		
		if(!strncmp("COM", strDevice, 3))
			ThrowMessage(hFrame, strDevice);
		else if(!strncmp("USB", strDevice, 3))
			ThrowMessage(hFrame, strDevice);
		i += len;
	}

	//DiscoverStage(hFrame)
	return dw;
}
