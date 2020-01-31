#include "stdafx.h"

#include "AlphaBlend.h"

DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, PIXEL color, INT *img, INT *mask, PIXEL *out)
{

	if(!img || !mask || !out)
		return FALSE;

	DWORD max = w*h;
	DWORD i = NULL;

	FLOAT trans = (1.0-alpha);

	for(i = 0; i < max; i++){
		
		if(mask[i]){
			out[i].b = alpha*img[i]+trans*color.b;
			out[i].g = alpha*img[i]+trans*color.g;
			out[i].r = alpha*img[i]+trans*color.r;

			if(out[i].b > 255)
				out[i].b = 255;
			if(out[i].g > 255)
				out[i].g = 255;
			if(out[i].r > 255)
				out[i].r = 255;
		}

	}

	return TRUE;
}

DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, DWORD flgColor, INT *img, UCHAR *mask, PIXEL *out)
{

	if(!img || !mask || !out)
		return FALSE;

	DWORD max = w*h;
	DWORD i = NULL;

	FLOAT trans = (1.0-alpha);

	memset(out, NULL, w*h*sizeof(PIXEL));

	UCHAR *ptr;

	ClampData(w*h, 0, 255, img, img);

	for(i = 0; i < max; i++){

		out[i].b = img[i];
		out[i].g = img[i];
		out[i].r = img[i];

		if(mask[i]){
			switch(flgColor){
				case AB_RED:
					out[i].r = alpha*img[i]+trans*mask[i];
					break;
				case AB_GREEN:
					out[i].g = alpha*img[i]+trans*mask[i];
					break;
				case AB_BLUE:
					out[i].b = alpha*img[i]+trans*mask[i];
					break;
				default:
					return FALSE;
			}

		}

	}

	return TRUE;
}

DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, DWORD flgColor, UCHAR *img, INT *mask, PIXEL *out)
{

	if(!img || !mask || !out)
		return FALSE;

	DWORD max = w*h;
	DWORD i = NULL;

	FLOAT trans = (1.0-alpha);

	memset(out, NULL, w*h*sizeof(PIXEL));

	UCHAR *ptr;

	//ClampData(w*h, 0, 255, img, img);

	for(i = 0; i < max; i++){

		out[i].b = img[i];
		out[i].g = img[i];
		out[i].r = img[i];

		if(mask[i]){
			switch(flgColor){
				case AB_RED:
					out[i].r = alpha*img[i]+trans*mask[i];
					break;
				case AB_GREEN:
					out[i].g = alpha*img[i]+trans*mask[i];
					break;
				case AB_BLUE:
					out[i].b = alpha*img[i]+trans*mask[i];
					break;
				default:
					return FALSE;
			}

		}

	}

	return TRUE;
}

DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, DWORD flgColor, UCHAR *img, UCHAR *mask, PIXEL *out)
{

	if(!img || !mask || !out)
		return FALSE;

	DWORD max = w*h;
	DWORD i = NULL;

	FLOAT trans = (1.0-alpha);

	memset(out, NULL, w*h*sizeof(PIXEL));

	UCHAR *ptr;

	for(i = 0; i < max; i++){

		out[i].b = img[i];
		out[i].g = img[i];
		out[i].r = img[i];

		if(mask[i]){
			switch(flgColor){
				case AB_RED:
					out[i].r = alpha*img[i]+trans*mask[i];
					break;
				case AB_GREEN:
					out[i].g = alpha*img[i]+trans*mask[i];
					break;
				case AB_BLUE:
					out[i].b = alpha*img[i]+trans*mask[i];
					break;
				default:
					return FALSE;
			}

		}

	}

	return TRUE;
}

//Alpha composits based on the value of the pixel values located at each pixel index in 'imgMask'

DWORD AlphaComposite(UINT w, UINT h, UCHAR *imgI, UCHAR *imgII, INT *imgMask, UCHAR *imgOut)
{
	if(!imgI || !imgII || !imgMask || !imgOut)
		return FALSE;

	DWORD max = w*h;
	DWORD i = NULL;

	FLOAT alpha = 0.0;
	FLOAT trans = 0.0;

	for(i = 0; i < max; i++){

		
			trans = ((float)abs(imgMask[i])-(float)255)/(float)255;
			//trans = ((float) 1/ (float) imgMask[i]);
			trans = abs(trans);
			alpha = 1-trans;
		if(imgMask[i]){
			imgOut[i] = alpha*imgI[i]+trans*imgII[i];
			//imgOut[i] = trans*imgI[i]+alpha*imgII[i];
		}else{
			imgOut[i] = trans*imgI[i]+alpha*imgII[i];
			//imgOut[i] = alpha*imgI[i]+trans*imgII[i];
		}


	}

	return TRUE;
}