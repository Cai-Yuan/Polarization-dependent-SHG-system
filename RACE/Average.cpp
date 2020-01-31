#include "stdafx.h"

#include "Average.h"
#include "Sum.h"

DWORD Average(DWORD nData, INT *imgI, INT *imgII, INT *imgOut)
{
	DWORD i = 0;


	if(!imgI||!imgII||!imgOut)
		return FALSE;

	for(i = 0; i < nData; i++){
		imgOut[i] = (INT)((imgI[i]+imgII[i])*0.5);
	}

	return TRUE;
}

DWORD Average(DWORD nData, UCHAR *imgI, UCHAR *imgII, UCHAR *imgOut)
{
	DWORD i = 0;


	if(!imgI||!imgII||!imgOut)
		return FALSE;

	for(i = 0; i < nData; i++){
		imgOut[i] = (UCHAR)((imgI[i]*0.5)+(imgII[i]*0.5));
	}

	return TRUE;
}

DWORD Blend(DWORD nData, FLOAT aI, FLOAT aII, UCHAR *imgI, UCHAR *imgII, UCHAR *imgOut)
{
	DWORD i = 0;

	if(!imgI||!imgII||!imgOut)
		return FALSE;

	for(i = 0; i < nData; i++){
		imgOut[i] = (UCHAR)((imgI[i]*aI)+(imgII[i]*aII));
	}

	return TRUE;
}