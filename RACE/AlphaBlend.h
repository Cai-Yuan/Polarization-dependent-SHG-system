#pragma once

#define AB_RED		1
#define AB_GREEN	2
#define AB_BLUE		3

DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, DWORD flgColor, UCHAR *img, UCHAR *mask, PIXEL *out);
DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, DWORD flgColor, INT *img, UCHAR *mask, PIXEL *out);
DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, PIXEL color, INT *img, INT *mask, PIXEL *out);
DWORD AlphaComposite(UINT w, UINT h, FLOAT alpha, DWORD flgColor, UCHAR *img, INT *mask, PIXEL *out);

DWORD AlphaComposite(UINT w, UINT h, UCHAR *imgI, UCHAR *imgII, INT *imgMask, UCHAR *imgOut);