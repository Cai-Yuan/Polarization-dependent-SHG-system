#include "stdafx.h"

#include "Suppression.h"

//Direction Dependent
//Check x-Direction
//Check y-Direction
DWORD NonMaximumSuppression(UINT w, UINT h, FLOAT *in, FLOAT *out)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT max = w*h;

	INT w0 = w-1;
	INT h0 = h-1;

	INT fMax = NULL;	//Flag

	if(!in)return FALSE;

	memset(out, NULL, w*h*sizeof(FLOAT));

	for(y = 1; y < h0; y++){
		for(x = 1; x < w0; x++){

			i = w*y + x;

			fMax = NULL;

			//X-Direction
			if(in[i] > in[i-1] && in[i] > in[i+1]){
				out[i] = in[i];
				fMax = TRUE;
			}
			
			//Y-Direction
			if(!fMax){
				if(in[i] > in[i-w] && in[i] > in[i+w]){
					out[i] = in[i];
					fMax = TRUE;
				}
			}

			//SWtoNE-Direction
			if(!fMax){
				if(in[i] > in[i-w-1] && in[i] > in[i+w+1]){
					out[i] = in[i];
					fMax = TRUE;
				}
			}

			//NWtoSE-Direction
			if(!fMax){
				if(in[i] > in[i-w+1] && in[i] > in[i+w-1]){
					out[i] = in[i];
					fMax = TRUE;
				}
			}
		}
	}


	return TRUE;
}

DWORD NonMaximumSuppression(UINT w, UINT h, INT *in, INT *out)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT max = w*h;

	INT w0 = w-1;
	INT h0 = h-1;

	INT fMax = NULL;	//Flag

	if(!in)return FALSE;

	memset(out, NULL, w*h*sizeof(FLOAT));

	for(y = 1; y < h0; y++){
		for(x = 1; x < w0; x++){

			i = w*y + x;

			fMax = NULL;

			//X-Direction
			if(in[i] > in[i-1] && in[i] > in[i+1]){
				out[i] = in[i];
				fMax = TRUE;
			}
			
			//Y-Direction
			if(!fMax){
				if(in[i] > in[i-w] && in[i] > in[i+w]){
					out[i] = in[i];
					fMax = TRUE;
				}
			}

			//SWtoNE-Direction
			if(!fMax){
				if(in[i] > in[i-w-1] && in[i] > in[i+w+1]){
					out[i] = in[i];
					fMax = TRUE;
				}
			}

			//NWtoSE-Direction
			if(!fMax){
				if(in[i] > in[i-w+1] && in[i] > in[i+w-1]){
					out[i] = in[i];
					fMax = TRUE;
				}
			}
		}
	}


	return TRUE;
}


DWORD NonMaximumSuppressionExp(UINT w, UINT h, FLOAT *in, FLOAT *out)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;
	INT max = w*h;

	INT w0 = w-1;
	INT h0 = h-1;

	INT fMax = NULL;	//Flag

	if(!in)return FALSE;

	memset(out, NULL, w*h*sizeof(FLOAT));

	for(y = 1; y < h0; y++){
		for(x = 1; x < w0; x++){

			i = w*y + x;

			fMax = NULL;

			//X-Direction
			if(in[i] > in[i-1] && in[i] > in[i+1]){
				//out[i] = in[i];
				fMax++;
			}
			
			//Y-Direction
			if(in[i] > in[i-w] && in[i] > in[i+w]){
				//out[i] = in[i];
				fMax++;
			}

			//SWtoNE-Direction
			if(in[i] > in[i-w-1] && in[i] > in[i+w+1]){
				//out[i] = in[i];
				fMax++;
			}

			//NWtoSE-Direction
			if(in[i] > in[i-w+1] && in[i] > in[i+w-1]){
				//out[i] = in[i];
				fMax++;
			}

			//out[i] = 63*fMax;

			/*
			switch(fMax){
				case 1:
					break;
				case 2:
					break;
				case 3:
					out[i] = 100;
					break;
				case 4:
					out[i] = 255;
					break;
				default:
					break;
			}
			*/

			//if(fMax > 2){
				out[i] = fMax*in[i];
			//}

		}	
	}


	return TRUE;
}