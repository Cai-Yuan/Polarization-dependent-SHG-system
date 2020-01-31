#include "stdafx.h"

#include "DisplaySurface.h"

HBITMAP CreateDisplaySurface(HWND hwnd, HDC *hdcMem, HDC *hdcDisp, UINT w, UINT h, PIXEL **buffer)
{
	HBITMAP				hbmp;
	BITMAP				bmp;
	BITMAPINFO			bmpInfo, bInfo;
	BITMAPINFOHEADER	bmpInfoHeader, bInfoHeader;
	BITMAPFILEHEADER	bmpFileHeader, bFileHeader;

	HGDIOBJ				hdcOld;

	//Setup Bitmap
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = -(h);
	bmpInfoHeader.biWidth = w;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = w * h * 3;

	bmpInfo.bmiHeader = bmpInfoHeader;

	bmpFileHeader.bfType=0x4D42;
	bmpFileHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;

	*hdcDisp = GetDC(hwnd);

	if((*hdcMem = ::CreateCompatibleDC(*hdcDisp))==NULL)
		::MessageBox(NULL, _T("Could Not Create Compatible DC"), _T("Error"), MB_OK);

	hbmp = CreateDIBSection(*hdcMem, &bmpInfo, DIB_RGB_COLORS, (void **)buffer, NULL, NULL);

	if(hbmp == (HBITMAP) ERROR_INVALID_PARAMETER || hbmp == NULL)
		::MessageBox(NULL, _T("Invalid Parameter DIB bits!"), _T("Error"), MB_OK);
	else if(hbmp == NULL)
		::MessageBox(NULL, _T("Could Not Create DIB bits!"), _T("Error"), MB_OK);

	if((hdcOld  = SelectObject(*hdcMem, hbmp))== NULL)
		::MessageBox(NULL, _T("Could Not Select Bitmap Into Memory!"), _T("Error"), MB_OK);

	GetObject(hbmp, sizeof(BITMAP), &bmp);

	return hbmp;
	
}
