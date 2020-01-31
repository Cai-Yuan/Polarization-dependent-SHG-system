#include "stdafx.h"

#include "Chamfer.h"

//DWORD Chamfer(UINT width, UINT height, UCHAR *img, UCHAR *chamfer)
//{
//	UINT x, y, i;
//	UINT w = width;
//	UINT h = height;
//
//	UINT max = w * h + 1;
//	UINT dist = max;
//
//	memset(chamfer,0x00, w*h*sizeof(UCHAR));
//	//Forwards
//	i = 0;
//	for(y = 1; y < h; y++){
//		for(x = 1; x < w; x++){
//			i = (y * w) + x;
//			if(img[i])
//				chamfer[i] = 0;
//			else{
//				dist = max;
//				if(y > 0)
//					dist = ChamferMin(dist, chamfer[i - w] + 1);
//				if(x > 0)
//					dist = ChamferMin(dist, chamfer[i - 1] + 1);
//				chamfer[i] = dist;
//			}
//		}
//	}
//
//	//Backwards
//	//i = (w - 1)*(h-1) + 1;
//	for(y = (h - 1); y > 0; y--){
//		for(x = (w - 1); x > 0; x--, i--){
//			i = (y * w) + x;
//			if(img[i])
//				chamfer[i] = 0;
//			else{
//				dist = chamfer[i];
//				if(y < (h - 1))
//					dist = ChamferMin(dist, chamfer[i + w] + 1);
//				if(x < (w - 1))
//					dist = ChamferMin(dist, chamfer[i + 1] + 1);
//				chamfer[i] = dist;
//			}
//		}
//	}
//		
//	return TRUE;
//
//}


DWORD Chamfer(UINT width, UINT height, FLOAT *img, FLOAT *chamfer)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;

	UINT max = w * h + 1;
	UINT dist = max;

	//memset(chamfer, 0x00, w*h*sizeof(FLOAT));

	//Forwards
	i = 0;
	for(y = 1; y < h; y++){
		for(x = 1; x < w; x++){
			i = (y * w) + x;
			if(img[i])
				chamfer[i] = 0;
			else{
				dist = max;
				if(y > 0)
					dist = ChamferMin(dist, chamfer[i - w] + 1);
				if(x > 0)
					dist = ChamferMin(dist, chamfer[i - 1] + 1);
				chamfer[i] = dist;
			}
		}
	}

	//Backwards
	
	for(y = (h - 1); y > 0; y--){
		for(x = (w - 1); x > 0; x--, i--){
			//i = (w - 1)*(h-1) + 1;
			i = (y * w) + x;
			if(img[i])
				chamfer[i] = 0;
			else{
				dist = chamfer[i];
				if(y < (h - 1))
					dist = ChamferMin(dist, chamfer[i + w] + 1);
				if(x < (w - 1))
					dist = ChamferMin(dist, chamfer[i + 1] + 1);
				chamfer[i] = dist;
			}
		}
	}
				
	return TRUE;

}

//DWORD Chamfer(UINT width, UINT height, INT *img, INT *chamfer)
//{
//	UINT x, y, i;
//	UINT w = width;
//	UINT h = height;
//
//	UINT max = w * h + 1;
//	UINT dist = max;
//
//	//Forwards
//	i = 0;
//	for(y = 1; y < h; y++){
//		for(x = 1; x < w; x++){
//			i = (y * w) + x;
//			if(img[i])
//				chamfer[i] = 0;
//			else{
//				dist = max;
//				if(y > 0)
//					dist = ChamferMin(dist, chamfer[i - w] + 1);
//				if(x > 0)
//					dist = ChamferMin(dist, chamfer[i - 1] + 1);
//				chamfer[i] = dist;
//			}
//		}
//	}
//
//	//Backwards
//	
//	for(y = (h - 1); y > 0; y--){
//		for(x = (w - 1); x > 0; x--, i--){
//			//i = (w - 1)*(h-1) + 1;
//			i = (y * w) + x;
//			if(img[i])
//				chamfer[i] = 0;
//			else{
//				dist = chamfer[i];
//				if(y < (h - 1))
//					dist = ChamferMin(dist, chamfer[i + w] + 1);
//				if(x < (w - 1))
//					dist = ChamferMin(dist, chamfer[i + 1] + 1);
//				chamfer[i] = dist;
//			}
//		}
//	}
//				
//	return TRUE;
//
//}

DWORD Chamfer(UINT width, UINT height, FLOAT thresh, FLOAT *img, FLOAT *chamfer)
{
	UINT x, y, i;
	UINT w = width;
	UINT h = height;

	UINT max = w * h + 1;
	UINT dist = max;

	//memset(chamfer, 0x00, w*h*sizeof(FLOAT));

	//Forwards
	i = 0;
	for(y = 1; y < h; y++){
		for(x = 1; x < w; x++){
			i = (y * w) + x;
			if(img[i] < thresh)
				chamfer[i] = 0;
			else{
				dist = max;
				if(y > 0)
					dist = ChamferMin(dist, chamfer[i - w] + 1);
				if(x > 0)
					dist = ChamferMin(dist, chamfer[i - 1] + 1);
				chamfer[i] = dist;
			}
		}
	}

	//Backwards
	
	for(y = (h - 1); y > 0; y--){
		for(x = (w - 1); x > 0; x--, i--){
			//i = (w - 1)*(h-1) + 1;
			i = (y * w) + x;
			if(img[i] < thresh)
				chamfer[i] = 0;
			else{
				dist = chamfer[i];
				if(y < (h - 1))
					dist = ChamferMin(dist, chamfer[i + w] + 1);
				if(x < (w - 1))
					dist = ChamferMin(dist, chamfer[i + 1] + 1);
				chamfer[i] = dist;
			}
		}
	}
				
	return TRUE;

}

//INT ChamferMin(INT a, INT b)
//{
//	if(a < b)
//		return a;
//	else
//		return b;
//}

FLOAT ChamferMin(FLOAT a, FLOAT b)
{
	if(a < b)
		return a;
	else
		return b;
}