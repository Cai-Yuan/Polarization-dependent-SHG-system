#include "stdafx.h"

#include "DerivativeOperator.h"

DWORD GradientFirstOrder(UINT w, UINT h, INT b, UCHAR *img, UCHAR *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			sum  = abs(img[i+b]-img[i]);
			sum += abs(img[i+(b*w)]-img[i]);
			sum += abs(img[i+(b*w)+b]-img[i]);
			sum += abs(img[i-(b*w)+b]-img[i]);

			if(sum < 0)
				grad[i] = 0;
			else if(sum > 255)
				grad[i] = 255;
			else
				grad[i] = sum;
		}
	}


	return i;
}

DWORD GradientFirstOrder(UINT w, UINT h, INT b, FLOAT *img, FLOAT *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			
			sum  = img[i+b]-img[i];
			sum += img[i+(b*w)]-img[i];
			sum += img[i+(b*w)+b]-img[i];
			sum += img[i-(b*w)+b]-img[i];

			//sum  = abs(img[i+b]-img[i]);
			//sum += abs(img[i+(b*w)]-img[i]);
			//sum += abs(img[i+(b*w)+b]-img[i]);
			//sum += abs(img[i-(b*w)+b]-img[i]);

			grad[i] = sum;
		}
	}


	return i;
}

DWORD GradientFirstOrder(UINT w, UINT h, INT b, INT *img, INT *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			sum  = abs(img[i+b]-img[i]);
			sum += abs(img[i+(b*w)]-img[i]);
			sum += abs(img[i+(b*w)+b]-img[i]);
			sum += abs(img[i-(b*w)+b]-img[i]);

			grad[i] = sum;
		}
	}


	return i;
}

DWORD GradientSobel(UINT w, UINT h, INT b, FLOAT *img)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT wb = w*b;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;
	FLOAT Gx = NULL;
	FLOAT Gy = NULL;

	if(!img)
		return FALSE;

	FLOAT *grad = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			Gx = -img[i - b - wb] 
				+ img[i + b - wb] 
				- 2*img[i-b] 
				+ 2*img[i+b]
				- img[i-b+wb]
				+ img[i+b+wb];

			Gy = img[i-b-wb]
				+ 2*img[i-wb]
				+ img[i+b-wb]
				- img[i-b+wb]
				- 2*img[i+wb]
				- img[i+b+wb];

			//grad[i] = Magnitude(Gx) + Magnitude(Gy);
			grad[i] = sqrt(Gx*Gx + Gy*Gy);

		}
	}

	if(grad){
		memcpy(img, grad, w*h*sizeof(FLOAT));
		free(grad);
	}


	return i;
}

DWORD GradientSobel(UINT w, UINT h, INT b, FLOAT *img, FLOAT *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT wb = w*b;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;
	FLOAT Gx = NULL;
	FLOAT Gy = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			Gx = -img[i - b - wb] 
				+ img[i + b - wb] 
				- 2*img[i-b] 
				+ 2*img[i+b]
				- img[i-b+wb]
				+ img[i+b+wb];

			Gy = img[i-b-wb]
				+ 2*img[i-wb]
				+ img[i+b-wb]
				- img[i-b+wb]
				- 2*img[i+wb]
				- img[i+b+wb];

			//grad[i] = Magnitude(Gx) + Magnitude(Gy);
			grad[i] = sqrt(Gx*Gx + Gy*Gy);

		}
	}


	return i;
}

DWORD GradientSobel(UINT w, UINT h, INT b, INT *img, INT *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT wb = w*b;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;
	FLOAT Gx = NULL;
	FLOAT Gy = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			Gx = -img[i - b - wb] 
				+ img[i + b - wb] 
				- 2*img[i-b] 
				+ 2*img[i+b]
				- img[i-b+wb]
				+ img[i+b+wb];

			Gy = img[i-b-wb]
				+ 2*img[i-wb]
				+ img[i+b-wb]
				- img[i-b+wb]
				- 2*img[i+wb]
				- img[i+b+wb];

			//grad[i] = Magnitude(Gx) + Magnitude(Gy);
			grad[i] = sqrt(Gx*Gx + Gy*Gy);

		}
	}


	return i;
}

DWORD GradientCentralDifference(UINT w, UINT h, INT b, FLOAT *img, FLOAT *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT wb = w*b;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;
	FLOAT Lx = NULL;
	FLOAT Ly = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			Lx =/* 0.5**/img[i+b] - /*0.5**/img[i-b];
			Ly = /*0.5**/img[i-wb] - /*0.5**/img[i+wb];

			grad[i] = sqrt(Lx*Lx + Ly*Ly);

		}
	}



	return i;
}

DWORD GradientCentralDifference(UINT w, UINT h, INT b, INT *img, INT *grad)
{

	INT i = NULL;
	INT x = NULL;
	INT y = NULL;

	INT wb = w*b;

	INT xMax = (w-b);
	INT yMax = (h-b);

	INT sum = NULL;
	FLOAT Lx = NULL;
	FLOAT Ly = NULL;

	if(!img || !grad)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){

			i = (y*w) + x;
			
			Lx =/* 0.5**/img[i+b] - /*0.5**/img[i-b];
			Ly = /*0.5**/img[i-wb] - /*0.5**/img[i+wb];

			grad[i] = sqrt(Lx*Lx + Ly*Ly);

		}
	}



	return i;
}

VOID GradientPlow(UINT w, UINT h, UCHAR *img, UCHAR *imgGradient)
{

	INT x, y, i;
	INT sum;
	INT xMax = (w-1);
	INT yMax = (h-1);
	INT dxdySum;

	memset(imgGradient, 0x00, w*h*sizeof(UCHAR));

	for(y = 0; y < yMax; y++){
		for(x = 0; x < xMax; x++){

			i = (y*w) + x;
			dxdySum = abs(img[i+1]-img[i]);
			dxdySum += abs(img[i+w]-img[i]);
			dxdySum += abs(img[i+1+w]-img[i]);
			dxdySum += abs(img[i-1+w]-img[i]);

			//dxdySum = (INT)((INT)dxdySum / (INT)4);

			if(dxdySum < 0)
				dxdySum = 0;

			if(dxdySum > 255)
				dxdySum = 255;

			imgGradient[i] = dxdySum;
		}
	}
	
}

VOID GradientPlow(UINT w, UINT h, INT threshold, UCHAR *img, UCHAR *imgGradient)
{

	INT x, y, i;
	INT sum;
	INT xMax = (w-1);
	INT yMax = (h-1);
	INT dxdySum;

	memset(imgGradient, 0x00, w*h*sizeof(UCHAR));

	for(y = 0; y < yMax; y++){
		for(x = 0; x < xMax; x++){

			i = (y*w) + x;
			dxdySum = abs(img[i+1]-img[i]);
			dxdySum += abs(img[i+w]-img[i]);
			dxdySum += abs(img[i+1+w]-img[i]);
			dxdySum += abs(img[i-1+w]-img[i]);

			if(dxdySum > threshold)
				imgGradient[i] = 255;
		}
	}
	
}

VOID GradientPlowInverted(UINT w, UINT h, UCHAR *img, UCHAR *imgGradient)
{

	INT x, y, i;
	INT sum;
	INT xMax = (w-1);
	INT yMax = (h-1);
	INT dxdySum;

	memset(imgGradient, 0x00, w*h*sizeof(UCHAR));

	for(y = 0; y < yMax; y++){
		for(x = 0; x < xMax; x++){

			i = (y*w) + x;
			dxdySum = abs(img[i+1]-img[i]);
			dxdySum += abs(img[i+w]-img[i]);
			dxdySum += abs(img[i+1+w]-img[i]);
			dxdySum += abs(img[i-1+w]-img[i]);

			//dxdySum = (INT)((INT)dxdySum / (INT)4);

			if(dxdySum < 0)
				dxdySum = 0;

			if(dxdySum > 255)
				dxdySum = 255;

			imgGradient[i] = (255 - dxdySum);
		}
	}
	
}

//fxx(x, y) = f(x+b, y) + f(x-b, y) - 2f(x, y)
//fyy(x, y) = f(x, y+b) + f(x, y-b) - 2f(x, y)

DWORD GradientSecondOrder(UINT w, UINT h, INT b, UCHAR *img, UCHAR *imgGradient)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!img || !imgGradient)
		return FALSE;

	memset(imgGradient, NULL, w*h*sizeof(UCHAR));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			//sum = Magnitude(fxx + fyy);
			sum = fxx + fyy + fxy + fyx;
			if(sum > 255)
				imgGradient[i] = 255;
			else if(sum < 0)
				imgGradient[i] = 0;
			else
				imgGradient[i] = sum;
		}
	}

	return i;
}

DWORD GradientSecondOrder(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgGradient)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!img || !imgGradient)
		return FALSE;

	memset(imgGradient, NULL, w*h*sizeof(UCHAR));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			sum = fxx + fyy + fxy + fyx;
			imgGradient[i] = sum;
		}
	}

	return i;
}

DWORD GradientSecondOrder(UINT w, UINT h, INT b, INT *img, INT *imgGradient)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!img || !imgGradient)
		return FALSE;

	memset(imgGradient, NULL, w*h*sizeof(UCHAR));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			sum = fxx + fyy + fxy + fyx;
			imgGradient[i] = sum;
		}
	}

	return i;
}


//Sum of Partial Second Derivatives in Cartesian Coordiantes
DWORD Laplacian(UINT w, UINT h, INT b, FLOAT *img)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!img)
		return FALSE;

	FLOAT *imgGradient = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			sum = fxx + fyy + fxy + fyx;
			imgGradient[i] = sum;
		}
	}

	if(imgGradient){
		memcpy(img, imgGradient, w*h*sizeof(FLOAT));
		free(imgGradient);
	}

	return i;
}

//Sum of Partial Second Derivatives in Cartesian Coordiantes
DWORD Laplacian(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgGradient)
{
	INT i = NULL;
	INT x, y;
	FLOAT sum;
	FLOAT xMax = (w-b);
	FLOAT yMax = (h-b);
	FLOAT fxx = NULL;
	FLOAT fyy = NULL;
	FLOAT fxy = NULL;
	FLOAT fyx = NULL;
	INT yOff = b*w;
	
	if(!img || !imgGradient)
		return FALSE;

	//memset(imgGradient, NULL, w*h*sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			sum = fxx + fyy + fxy + fyx;
			imgGradient[i] = sum;
		}
	}

	return i;
}

DWORD LaplacianAngle(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgAngle)
{
	INT i = NULL;
	INT x, y;
	FLOAT sum;
	FLOAT xMax = (w-b);
	FLOAT yMax = (h-b);
	FLOAT fxx = NULL;
	FLOAT fyy = NULL;
	FLOAT fxy = NULL;
	FLOAT fyx = NULL;
	INT yOff = b*w;
	FLOAT angle = 0;
	
	if(!img || !imgAngle)
		return FALSE;

	//memset(imgAngle, 0.0, w*h*sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			//fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			//fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			angle = atan2(fyy, fxx);
			imgAngle[i] = angle;
			//sum = fxx + fyy + fxy + fyx;
			//imgGradient[i] = sum;
		}
	}

	return i;
}

DWORD GradientAngle(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgAngle)
{
	INT i = NULL;
	INT x, y;
	FLOAT sum;
	FLOAT xMax = (w-b);
	FLOAT yMax = (h-b);
	FLOAT fxx = NULL;
	FLOAT fyy = NULL;
	FLOAT fxy = NULL;
	FLOAT fyx = NULL;
	INT yOff = b*w;
	FLOAT angle = 0;
	
	if(!img || !imgAngle)
		return FALSE;

	memset(imgAngle, 0.0, w*h*sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] - img[i - b];
			fyy = img[i + yOff] - img[i - yOff];

			//fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			//fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			angle = atan2(fyy, fxx);
			imgAngle[i] = angle;
			//sum = fxx + fyy + fxy + fyx;
			//imgGradient[i] = sum;
		}
	}

	return i;
}

//Sum of Partial Second Derivatives in Cartesian Coordiantes
DWORD InverseLaplacian(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgGradient)
{
	INT i = NULL;
	INT x, y;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!img || !imgGradient)
		return FALSE;

	memset(imgGradient, NULL, w*h*sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			sum = fxx + fyy + fxy + fyx;
			imgGradient[i] = -1.0*sum;
		}
	}

	return i;
}

DWORD Laplacian(UINT w, UINT h, INT b, INT *img, INT *imgGradient)
{
	INT i = NULL;
	INT x, y;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!img || !imgGradient)
		return FALSE;

	memset(imgGradient, NULL, w*h*sizeof(FLOAT));

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			fxx = img[i + b] + img[i - b] - (2*img[i]);
			fyy = img[i + yOff] + img[i - yOff] - (2*img[i]);

			fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);
			fyx = img[i + b - yOff] + img[i - b + yOff] - (2*img[i]);

			sum = fxx + fyy + fxy + fyx;
			imgGradient[i] = sum;
		}
	}

	return i;
}

DWORD HessianDxx(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!in || !out)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			out[i] = in[i+b] + in[i-b] - (2*in[i]);
			//fxx = in[i + b] + in[i - b] - (2*in[i]);
		}
	}

	return i;
}

DWORD HessianDyy(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!in || !out)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			out[i] = in[i+yOff] + in[i-yOff] - (2*in[i]);
			//fyy = in[i + yOff] + in[i - yOff] - (2*in[i]);

		}
	}

	return i;
}

DWORD HessianDxy(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out)
{
	INT x, y, i;
	INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT fxx = NULL;
	INT fyy = NULL;
	INT fxy = NULL;
	INT fyx = NULL;
	INT yOff = b*w;
	
	if(!in || !out)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			out[i] = in[i - b - yOff] + in[i + b + yOff] - (2*in[i]);
			//fxy = img[i - b - yOff] + img[i + b + yOff] - (2*img[i]);

		}
	}

	return i;
}

DWORD HessianDyx(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out)
{
	INT x, y, i;
	//INT sum;
	INT xMax = (w-b);
	INT yMax = (h-b);
	//INT fxx = NULL;
	//INT fyy = NULL;
	//INT fxy = NULL;
	//INT fyx = NULL;
	INT yOff = b*w;
	
	if(!in || !out)
		return FALSE;

	for(y = b; y < yMax; y++){
		for(x = b; x < xMax; x++){
			
			i = (y*w)+x;

			out[i] = in[i + b - yOff] + in[i - b + yOff] - (2*in[i]);
			//fyx = in[i + b - yOff] + in[i - b + yOff] - (2*in[i]);

		}
	}

	return i;
}

//'l' - Left value
//'c' - Central value
//'r' - Right value
DWORD CrossTest(FLOAT l, FLOAT c, FLOAT r)
{
	FLOAT cAbs = 0.0;

	if(Sign(l) == Sign(r))
		return 0;

	cAbs = fabs(c);
	if(cAbs < fabs(l) && cAbs < fabs(r))
		return 1;

	return 0;

}

DWORD ZeroCrossing(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out)
{
	INT x, y, i;
	INT xMax = (w-b);
	INT yMax = (h-b);
	INT yOff = b*w;

	if(!in || !out)
		return FALSE;

	for(y = b; y < yMax; y++){
		//i = (y*w);
		for(x = b; x < xMax; x++){

			//i +=x;
			i = (y*w)+x;

			//X-Direction Test
			if(CrossTest(in[i-b], in[i], in[i+b])){
				out[i] = 255.0;
				continue;
			}
			//Y-Direction Test
			else if(CrossTest(in[i-yOff], in[i], in[i+yOff])){
				out[i] = 255.0;
				continue;
			}
			//XY-Direction Test
			else if(CrossTest(in[i-b-yOff], in[i], in[i+b+yOff])){
				out[i] = 255.0;
				continue;
			}
			//YX-Direction Test
			else if(CrossTest(in[i-yOff+b], in[i], in[i+yOff-b])){
				out[i] = 255.0;
				continue;
			}
			else{
				out[i] = 0;
			}
		}
	}

	return i;
}