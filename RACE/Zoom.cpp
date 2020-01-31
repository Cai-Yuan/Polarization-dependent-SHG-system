#include "stdafx.h"
#include "Zoom.h"


DWORD ZoomPIXEL(UINT w, UINT h, UINT *wZoom, UINT *hZoom, PIXEL *img, PIXEL *zoom)
{

	DWORD dw = NULL;

	INT i = 0;
	INT iZ = 0;

	INT x = 0;
	INT y = 0;
	INT xZ = 0;
	INT yZ = 0;

	if(!img || !zoom)
		return FALSE;

	*wZoom = 2*w;
	*hZoom = 2*h;

	yZ = 0;
	for(y = 0; y < h; y++, yZ+=2){
		i = y*w + x;
		xZ = 0;
		iZ = (yZ**wZoom) + (xZ);
		for(x = 0; x < w; x++, xZ+=2, iZ+=2){

			zoom[iZ].r = img[i].r;
			zoom[iZ].g = img[i].g;
			zoom[iZ].b = img[i].b;

			zoom[iZ+1].r = img[i].r;
			zoom[iZ+1].g = img[i].g;
			zoom[iZ+1].b = img[i].b;

			zoom[iZ+(*wZoom)].r = img[i].r;
			zoom[iZ+(*wZoom)].g = img[i].g;
			zoom[iZ+(*wZoom)].b = img[i].b;

			zoom[iZ+(*wZoom)+1].r = img[i].r;
			zoom[iZ+(*wZoom)+1].g = img[i].g;
			zoom[iZ+(*wZoom)+1].b = img[i].b;

			//memcpy(&zoom[iZ], &img[i], sizeof(PIXEL));
			//memcpy(&zoom[iZ+1], &img[i], sizeof(PIXEL));
			//memcpy(&zoom[iZ+(*wZoom)], &img[i], sizeof(PIXEL));
			//memcpy(&zoom[iZ+(*wZoom)+1], &img[i], sizeof(PIXEL));

			i++;
		}
	}

	return TRUE;
}

DWORD ZoomPIXEL(UINT w, UINT h, UINT mag, UINT *wZoom, UINT *hZoom, PIXEL *img, PIXEL *zoom)
{

	DWORD dw = NULL;

	INT i = 0;
	INT iZ = 0;

	INT x = 0;
	INT y = 0;
	INT xZ = 0;
	INT yZ = 0;
	INT xM = 0;
	INT yM = 0;

	INT wZ = mag*w;
	INT hZ = mag*h;

	if(!img || !zoom)
		return FALSE;

	*wZoom = mag*w;
	*hZoom = mag*h;

	INT wMax = w-1;
	INT hMax = h-1;

	yZ = 0;
	for(y = 0; y < hMax; y++, yZ+=mag){
		i = y*w + x;
		xZ = 0;
		iZ = (yZ*wZ) + (xZ);
		for(x = 0; x < wMax; x++, xZ+=mag, iZ+=mag){


			for(yM = 0; yM < mag; yM++){
				for(xM = 0; xM < mag; xM++){
					zoom[iZ + (wZ*yM) + xM].r = img[i].r;
					zoom[iZ + (wZ*yM) + xM].g = img[i].g;
					zoom[iZ + (wZ*yM) + xM].b = img[i].b;

				}
			}


			i++;
		}
	}

	return TRUE;
}

DWORD ZoomUCHAR(UINT w, UINT h, UINT *wZoom, UINT *hZoom, UCHAR *img, UCHAR *zoom)
{

	DWORD dw = NULL;

	INT i = 0;
	INT iZ = 0;

	INT x = 0;
	INT y = 0;
	INT xZ = 0;
	INT yZ = 0;

	if(!img || !zoom)
		return FALSE;

	*wZoom = 2*w;
	*hZoom = 2*h;

	yZ = 0;
	for(y = 0; y < h; y++, yZ+=2){
		xZ = 0;
		i = y*w + x;
		iZ = (yZ**wZoom) + (xZ);
		for(x = 0; x < w; x++, /*xZ+=2,*/ iZ+=2){

			//iZ = (yZ**wZoom) + (xZ);
			zoom[iZ] = img[i];
			zoom[iZ+1] = img[i];
			zoom[iZ+(*wZoom)] = img[i];
			zoom[iZ+(*wZoom)+1] = img[i];

			i++;
		}
	}

	return TRUE;
}