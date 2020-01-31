#include "stdafx.h"

VOID fitocImage(UINT width, UINT height, INT *imgIn, unsigned char *imgOut);
VOID titocImage(UINT width, UINT height, UINT maxRange, INT *imgIn, unsigned char *imgOut, unsigned char *midPlane);
VOID CHARtoPIXEL(UINT width, UINT height, CHAR *imgIn, PIXEL *imgOut);
VOID UCHARtoPIXEL(UINT width, UINT height, unsigned char *imgIn, PIXEL *imgOut);
VOID UCHARtoPIXEL(UINT width, UINT height, UCHAR *imgR, UCHAR *imgG, UCHAR *imgB, PIXEL *imgOut);
VOID Color256toPIXEL(UINT width, UINT height, UCHAR *imgIn, PIXEL *imgOut);
VOID UCHARtoINT(UINT width, UINT height, unsigned char *imgIn, INT *imgOut);
DWORD UCHARtoFLOAT(UINT width, UINT height, unsigned char *imgIn, FLOAT *imgOut);
VOID DerivativeToBMP(UINT width, UINT height, INT *imgIn, PIXEL *imgOut);
VOID DerivativeToBMPColor(UINT width, UINT height, INT *imgIn, PIXEL *imgOut);

DWORD FLOATtoPIXEL(DWORD width, DWORD height, FLOAT *inR, FLOAT *inG, FLOAT *inB, PIXEL *pix);
DWORD FLOATtoPIXEL(DWORD width, DWORD height, FLOAT *gray, FLOAT *mask, PIXEL *pix);
DWORD FLOATtoPIXEL(DWORD width, DWORD height, FLOAT *inGray, FLOAT *inR, FLOAT *inG, FLOAT *inB, PIXEL *pix);

VOID INTtoUCHAR(UINT width, UINT height, INT *imgIn, UCHAR *imgOut);

VOID UINTtoUCHAR(UINT width, UINT height, UINT *imgIn, UCHAR *imgOut);

DWORD PIXELtoUCHAR(UINT width, UINT height, PIXEL *imgIn, UCHAR *imgOut, DWORD flag);
DWORD PIXELtoFLOAT(UINT width, UINT height, PIXEL *imgIn, FLOAT *imgOut, DWORD flag);
VOID INTtoPIXEL(UINT width, UINT height, INT *imgIn, PIXEL *imgOut);
VOID UINTtoPIXEL(UINT width, UINT height, UINT *imgIn, PIXEL *imgOut);
VOID FLOATtoPIXEL(UINT width, UINT height, FLOAT *imgIn, PIXEL *imgOut);
VOID colorINTtoPIXEL(UINT width, UINT height, INT *imgIn, PIXEL *imgOut);
VOID colorFLOATtoPIXEL(UINT width, UINT height, FLOAT *imgIn, PIXEL *imgOut);

DWORD ZoomCrop(INT w, INT h, INT wZ, INT hZ, UCHAR *imgZoom, UCHAR *img);


VOID Conversion12bTo8b(UINT w, UINT h, SHORT *img12, UCHAR *img8);

VOID Compress(UINT w, UINT h, UCHAR *imgDest, UCHAR *imgSrc);

VOID WindowFLOAT(UINT width, UINT height, UINT minWindow, UINT maxWindow, FLOAT *img);
VOID FLOATtoUCHAR(UINT width, UINT height, FLOAT *imgIn, UCHAR *imgOut);

VOID WindowINT(UINT width, UINT height, UINT minWindow, UINT maxWindow, INT *img);


DWORD MaximumIntensityProjection(DWORD width, DWORD height, FLOAT *in, FLOAT *inR, FLOAT *inG, FLOAT *inB, PIXEL *pix);