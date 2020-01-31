#include "stdafx.h"

#include "Region.h"
#include "Point.h"

DWORD Region(UINT w, UINT h, UCHAR *img, DWORD nPointSet, FPOINTSET **ptSet)
{
	DWORD i = 0;
	DWORD region = 0;
	DWORD max = w*h;

	for(i = 0; i < max; i++){
		if(img[i]){
			region = img[i];
			if(region < nPointSet){
				Push(ptSet[region], GetFPOINTFromIndex(w, h, i));
			}

		}
	}

	return region;
}