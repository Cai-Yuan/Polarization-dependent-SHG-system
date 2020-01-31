#pragma once

#include "stdafx.h"

VOID GradientPlow(UINT w, UINT h, UCHAR *img, UCHAR *imgGradient);
VOID GradientPlow(UINT w, UINT h, INT threshold, UCHAR *img, UCHAR *imgGradient);

DWORD GradientSecondOrder(UINT w, UINT h, INT b, INT *img, INT *imgGradient);
DWORD GradientSecondOrder(UINT w, UINT h, INT b, UCHAR *img, UCHAR *imgGradient);
DWORD GradientSecondOrder(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgGradient);

DWORD GradientFirstOrder(UINT w, UINT h, INT b, INT *img, INT *grad);
DWORD GradientFirstOrder(UINT w, UINT h, INT b, UCHAR *img, UCHAR *grad);
DWORD GradientFirstOrder(UINT w, UINT h, INT b, FLOAT *img, FLOAT *grad);

VOID GradientPlowInverted(UINT w, UINT h, UCHAR *img, UCHAR *imgGradient);

DWORD GradientSobel(UINT w, UINT h, INT b, FLOAT *img);
DWORD GradientSobel(UINT w, UINT h, INT b, INT *img, INT *grad);
DWORD GradientSobel(UINT w, UINT h, INT b, FLOAT *img, FLOAT *grad);
DWORD GradientCentralDifference(UINT w, UINT h, INT b, FLOAT *img, FLOAT *grad);
DWORD GradientCentralDifference(UINT w, UINT h, INT b, INT *img, INT *grad);
DWORD GradientAngle(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgAngle);

DWORD Laplacian(UINT w, UINT h, INT b, FLOAT *img);
DWORD Laplacian(UINT w, UINT h, INT b, INT *img, INT *imgGradient);
DWORD Laplacian(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgGradient);
DWORD InverseLaplacian(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgGradient);
DWORD LaplacianAngle(UINT w, UINT h, INT b, FLOAT *img, FLOAT *imgAngle);

DWORD HessianDxx(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out);
DWORD HessianDyy(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out);
DWORD HessianDxy(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out);
DWORD HessianDyx(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out);

DWORD CrossTest(FLOAT l, FLOAT c, FLOAT r);
DWORD ZeroCrossing(UINT w, UINT h, INT b, FLOAT *in, FLOAT *out);