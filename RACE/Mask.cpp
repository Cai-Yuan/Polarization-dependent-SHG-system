#include "stdafx.h"

#include "Mask.h"

DWORD MaskData(DWORD nData, INT *in, INT *mask, INT *out)
{
	if(!in || !mask || !out){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < nData; i++){
		
		if(mask[i]){
			out[i] = in[i];
		}

	}


	return TRUE;
}

DWORD MaskData(DWORD nData, UCHAR *in, INT *mask, UCHAR *out)
{
	if(!in || !mask || !out){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < nData; i++){
		
		if(mask[i]){
			out[i] = in[i];
		}

	}


	return TRUE;
}