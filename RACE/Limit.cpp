#include "stdafx.h"

#include "Limit.h"

DWORD Range(FLOAT value, LIMIT limit)
{
	if(value > limit.lower && value < limit.upper){
		return TRUE;
	}else{
		return FALSE;
	}
}

DWORD Set(FLOAT lower, FLOAT upper, LIMIT *limit)
{
	if(!limit)
		return FALSE;

	limit->lower = lower;
	limit->upper = upper;

	return TRUE;
}

LRESULT SendLimit(HWND hwnd, DWORD flgLimit, LIMIT limit)
{
	return SendMessage(hwnd, WM_SETLIMITS, (WPARAM) flgLimit, (WPARAM) &limit);
}

