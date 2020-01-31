#include "stdafx.h"

#include "Math.h"
#include "LogOperator.h"


VOID LogOperator(UINT w, UINT h, UCHAR *img, UCHAR *imgLog)
//VOID LogOperator(FLOAT darken, struct image *img_in, struct image *img_out)
{	
	if(!img || imgLog)
		return;

	FLOAT temp, m;
	FLOAT step;
	UINT log = 0;
	UINT row, col, i;
	UINT table[256];

	step = (10 - 1.0)/255.0;

	for(i = 0, m = 1.0; i < 255; i++, m+=step){
		table[i] = i*log10(m);
	}
	for(row = 0, i = 0; row < h; row++){
		for(col = 0; col < w; col++, i++){
			if(table[img[i]] > 255)
				imgLog[i] = 255;
			else if(table[img[i]] < 0)
				imgLog[i] = 0;
			else
				imgLog[i] = table[img[i]];
		}
	}
}

VOID LogOperator(UINT w, UINT h, FLOAT *img, FLOAT *imgLog)
{	
	if(!img || imgLog)
		return;

	FLOAT temp, m;
	FLOAT step;
	UINT log = 0;
	UINT row, col, i;
	UINT table[256];

	step = (9.8 - 1.0)/255.0;

	for(i = 0, m = 1.0; i < 256; i++, m+=step){
		table[i] = i*log10(m);
	}
	for(row = 0, i = 0; row < h; row++){
		for(col = 0; col < w; col++, i++){

			if(img[i] >= 0)
				imgLog[i] = table[((UINT)floor(img[i]))];
			else if(img[i] > 255)
				imgLog[i] = 255;
			else 
				imgLog[i] = 0;


		}
	}
}

VOID LogOperator(UINT w, UINT h, INT *img, INT *imgLog)
{
	if(!img || imgLog)
		return;

	FLOAT temp, m;
	FLOAT step;
	UINT log = 0;
	UINT row, col, i;
	UINT table[256];

	step = (9.8 - 1.0)/255.0;

	for(i = 0, m = 1.0; i < 256; i++, m+=step){
		table[i] = i*log10(m);
	}
	for(row = 0, i = 0; row < h; row++){
		for(col = 0; col < w; col++, i++){

			if(img[i] >= 0)
				imgLog[i] = table[((UINT)floor((FLOAT)img[i]))];
			else if(img[i] > 255)
				imgLog[i] = 255;
			else 
				imgLog[i] = 0;


		}
	}
}


VOID LogOperator(UINT w, UINT h, UCHAR *img)
//VOID LogOperator(FLOAT darken, struct image *img_in, struct image *img_out)
{	
	if(!img)
		return;

	FLOAT temp, m;
	FLOAT step;
	UINT log = 0;
	UINT row, col, i;
	UINT table[256];

	step = (10 - 1.0)/255.0;

	for(i = 0, m = 1.0; i < 255; i++, m+=step){
		table[i] = i*log10(m);
	}
	for(row = 0, i = 0; row < h; row++){
		for(col = 0; col < w; col++, i++){
			if(table[img[i]] > 255)
				img[i] = 255;
			else if(table[img[i]] < 0)
				img[i] = 0;
			else
				img[i] = table[img[i]];
		}
	}
}
//VOID nlogOperator(struct image *img_in, struct image *img_out)
//{
//	FLOAT temp, m;
//	UINT log = 0;
//	UINT row, col, i;
//	UINT table[256];
//	
//
//	for(i = 0, m = 1; i < 255; i++, m+=0.03922){
//		//table[i] = (UINT) 256*log(m);
//	}
//	for(row = 0, i = 0; row < img_in->height; row++){
//		for(col = 0; col < img_in->width; col++, i++){
//			//img_out->height[i] = table[img_in->pixel_data[i]];
//		}
//	}
//}