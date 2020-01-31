#include "stdafx.h"

#include "Stretch.h"

DWORD Stretch(UINT w, UINT h, UINT lower, UINT upper, UCHAR *img)
{

	if(!img)
		return FALSE;

	if(upper > 255)
		upper = 255;
	if(lower < 0)
		lower = 0;

	UINT i = NULL;
	UINT end = w*h;
	DWORD min = NULL;
	DWORD max = NULL;
	FLOAT ratio = NULL;
	DWORD temp = NULL;

	GetMaxMin(w*h, &max, &min, img);

	if(max-min)
		ratio = (FLOAT)(upper - lower) / (FLOAT)(max - min);
	else
		ratio = 0;

	for(i = 0; i < end; i++)
	{
		/*img[i]*/temp = (img[i]*ratio);
		img[i] = temp;
		if(temp > upper)
			img[i] = upper;
		if(temp < lower)
			img[i] = lower;
	}

	return i;
}

DWORD Stretch(UINT w, UINT h, FLOAT lower, FLOAT upper, FLOAT *in, FLOAT *out)
{

	if(!in || !out)
		return FALSE;

	UINT i = NULL;
	UINT end = w*h;
	FLOAT min = NULL;
	FLOAT max = NULL;
	FLOAT midData = NULL;
	FLOAT midRange = NULL;
	FLOAT ratio = NULL;
	//FLOAT shift = NULL;
	DWORD temp = NULL;

	GetMaxMin(w*h, &max, &min, in);

	//midData = (Magnitude(max) - Magnitude(min))/2.0;
	//midRange = (Magnitude(upper) - Magnitude(lower))/2.0;

	midData = (FLOAT)(max + min) / (FLOAT) 2.0;
	midRange = (FLOAT)(upper + lower) / (FLOAT) 2.0;

	if(max-min)
		ratio = (FLOAT)(midRange*2) / (FLOAT) (midData*2);
	else
		ratio = 0;


	for(i = 0; i < end; i++)
	{
		//out[i] = (in[i]*ratio) + shift;
		out[i] = ((in[i]-midData)*ratio) + midRange;
		if(out[i] > upper)
			out[i] = upper;
		if(out[i] < lower)
			out[i] = lower;
	}

	return i;
}

DWORD Stretch(UINT w, UINT h, INT lower, INT upper, INT *in, INT *out)
{

	if(!in || !out)
		return FALSE;

	UINT i = NULL;
	UINT end = w*h;
	INT min = NULL;
	INT max = NULL;
	FLOAT midData = NULL;
	FLOAT midRange = NULL;
	FLOAT ratio = NULL;
	DWORD temp = NULL;

	GetMaxMin(w*h, &max, &min, in);

	midData = (FLOAT)(max + min) / (FLOAT) 2.0;
	midRange = (FLOAT)(upper + lower) / (FLOAT) 2.0;

	if(max-min)
		ratio = (FLOAT)(midRange*2) / (FLOAT) (midData*2);
	else
		ratio = 0;


	for(i = 0; i < end; i++)
	{
		//out[i] = (in[i]*ratio) + shift;
		out[i] = ((in[i]-midData)*ratio) + midRange;
		if(out[i] > upper)
			out[i] = upper;
		if(out[i] < lower)
			out[i] = lower;
	}

	return i;
}

DWORD Stretch(UINT w, UINT h, INT lower, INT upper, INT *in, UCHAR *out)
{

	if(!in || !out)
		return FALSE;

	UINT i = NULL;
	UINT end = w*h;
	INT min = NULL;
	INT max = NULL;
	FLOAT midData = NULL;
	FLOAT midRange = NULL;
	FLOAT ratio = NULL;
	DWORD temp = NULL;

	GetMaxMin(w*h, &max, &min, in);

	midData = (FLOAT)(max + min) / (FLOAT) 2.0;
	midRange = (FLOAT)(upper + lower) / (FLOAT) 2.0;

	if(max-min)
		ratio = (FLOAT)(midRange*2) / (FLOAT) (midData*2);
	else
		ratio = 0;


	for(i = 0; i < end; i++)
	{
		//out[i] = (in[i]*ratio) + shift;
		out[i] = ((in[i]-midData)*ratio) + midRange;
		if(out[i] > upper)
			out[i] = upper;
		if(out[i] < lower)
			out[i] = lower;
	}

	return i;
}