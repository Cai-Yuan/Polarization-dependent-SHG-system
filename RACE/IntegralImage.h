#include "stdafx.h"

#define II_LOWDETAIL 0x100
#define II_HIGHDETAIL 0x101

VOID GetIntegralImage(UINT width, UINT height, UCHAR *imgIn, UINT *integralImg);
VOID GetIntegralImage(UINT width, UINT height, UCHAR *imgIn, FLOAT *integralImg);
VOID GetIntegralImage(UINT width, UINT height, UCHAR *imgIn, UINT64 *imgIntegral);
VOID GetIntegralImage(UINT width, UINT height, UINT *imgIn, FLOAT *imgIntegral);
//UINT GetIntegralImageSum(UINT width, UINT height, UINT left, UINT right, UINT top, UINT bottom, FLOAT *imgIntegral);
VOID GetImageFromIntegralImage(UINT width, UINT height, UINT n, FLOAT *imgIntegral, UCHAR *imgOut, UINT flags);
INT64 GetIntegralImageSum(UINT width, UINT height, UINT left, UINT right, UINT top, UINT bottom, FLOAT *imgIntegral);