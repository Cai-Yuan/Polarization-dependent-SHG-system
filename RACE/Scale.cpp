#include "stdafx.h"

#include "Scale.h"

DWORD Scale(UINT n, FLOAT scale, UCHAR *data)
{

	if(!data)
		return FALSE;

	UINT i = NULL;

	for(i = 0; i < n; i++){

		data[i] = data[i]*scale;

	}

	return i;

}