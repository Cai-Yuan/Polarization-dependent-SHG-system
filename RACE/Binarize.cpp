#include "stdafx.h"

#include "Binarize.h"

DWORD Binarize(UINT w, UINT h, UCHAR *img)
{

	INT i = NULL;
	INT max = w*h;

	if(!img) return FALSE;

	for(i = 0; i < max; i++){

		if(img[i])
			img[i] = 1;
		else
			img[i] = 0;
	}

	
	return TRUE;
}
