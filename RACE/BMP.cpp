#include "stdafx.h"

#include "BMP.h"

#ifdef RACE_OPENCV

#include <iostream>
#include <opencv2/core/core.hpp>
//#include <cxcore.h>
//#include <cv.h>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

#endif

VOID SaveBMP(UINT w, UINT h, LPCSTR lpcstrFileName, PIXEL *bmp)
{
	if(!bmp)
		return;

	HWND hTop = NULL;
	HWND hRoot = NULL;
	HANDLE hFile;
	DWORD nBytesWritten;

	HBITMAP				hbmp;
	BITMAPINFO			bmpInfo, bInfo;
	BITMAPINFOHEADER	bmpInfoHeader, bInfoHeader;
	BITMAPFILEHEADER	bmpFileHeader, bFileHeader;

	//Setup Bitmap
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biClrImportant = 0;
	bmpInfoHeader.biClrUsed = 0;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = -h;
	bmpInfoHeader.biWidth = w;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = w * h * 3;

	bmpInfo.bmiHeader = bmpInfoHeader;

	bmpFileHeader.bfType=0x4D42;
	bmpFileHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;

	hTop = GetTopWindow(NULL);
	hRoot = GetAncestor(hTop, GA_ROOT);
	ThrowMessage(hRoot, "Test");
	if((hFile = CreateFile((LPCSTR)lpcstrFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		WriteFile(hFile, (LPCVOID) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, (LPCVOID) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, (LPCVOID) bmp, bmpInfoHeader.biSizeImage, &nBytesWritten, NULL);
		CloseHandle(hFile);
	}
	else{
		HWND hWnd;
		CHAR strError[MAX_STRING];
		sprintf(strError, "Save Bitmap Failed: %s", lpcstrFileName);
		hWnd = FindWindow("RACE", NULL);
		ThrowMessage(hWnd, strError);
		CloseHandle(hFile);
	}

}

VOID SaveBMP(UINT w, UINT h, LPCSTR lpcstrFileName, UCHAR *img)
{
	if(!img)
		return;

	HANDLE hFile;
	DWORD nBytesWritten;

	HBITMAP				hbmp;
	BITMAPINFO			bmpInfo, bInfo;
	BITMAPINFOHEADER	bmpInfoHeader, bInfoHeader;
	BITMAPFILEHEADER	bmpFileHeader, bFileHeader;

	memset(&bmpInfoHeader, NULL, sizeof(BITMAPINFOHEADER));
	memset(&bmpFileHeader, NULL, sizeof(BITMAPFILEHEADER));

	//Setup Bitmap
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = -h;
	bmpInfoHeader.biWidth = w;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = w * h * BytesPerPixel(bmpInfoHeader.biBitCount);;

	bmpInfo.bmiHeader = bmpInfoHeader;

	bmpFileHeader.bfType=0x4D42;
	bmpFileHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;

	PIXEL *pix = (PIXEL *)malloc(w*h*sizeof(PIXEL));
	UCHARtoPIXEL(w, h, img, pix);

	if((hFile = CreateFile((LPCSTR)lpcstrFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		WriteFile(hFile, (LPCVOID) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, (LPCVOID) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, (LPCVOID) pix/*img*/, bmpInfoHeader.biSizeImage, &nBytesWritten, NULL);
		CloseHandle(hFile);
	}
	else{
		HWND hWnd;
		CHAR strError[MAX_STRING];
		sprintf(strError, "Save Bitmap Failed: %s", lpcstrFileName);
		hWnd = FindWindow("RACE", NULL);
		ThrowMessage(hWnd, strError);
		CloseHandle(hFile);
	}

	if(pix)free(pix);
}

VOID SaveBMP(UINT w, UINT h, LPCSTR lpcstrFileName, FLOAT *img)
{
	if(!img)
		return;

	HANDLE hFile;
	DWORD nBytesWritten;

	HBITMAP				hbmp;
	BITMAPINFO			bmpInfo, bInfo;
	BITMAPINFOHEADER	bmpInfoHeader, bInfoHeader;
	BITMAPFILEHEADER	bmpFileHeader, bFileHeader;

	memset(&bmpInfoHeader, NULL, sizeof(BITMAPINFOHEADER));
	memset(&bmpFileHeader, NULL, sizeof(BITMAPFILEHEADER));

	//Setup Bitmap
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biHeight = -h;
	bmpInfoHeader.biWidth = w;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biSizeImage = w * h * BytesPerPixel(bmpInfoHeader.biBitCount);;

	bmpInfo.bmiHeader = bmpInfoHeader;

	bmpFileHeader.bfType=0x4D42;
	bmpFileHeader.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;

	PIXEL *pix = (PIXEL *)malloc(w*h*sizeof(PIXEL));
	FLOATtoPIXEL(w, h, img, pix);

	if((hFile = CreateFile((LPCSTR)lpcstrFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		WriteFile(hFile, (LPCVOID) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, (LPCVOID) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesWritten, NULL);
		WriteFile(hFile, (LPCVOID) pix/*img*/, bmpInfoHeader.biSizeImage, &nBytesWritten, NULL);
		CloseHandle(hFile);
	}
	else{
		HWND hWnd;
		CHAR strError[MAX_STRING];
		sprintf(strError, "Save Bitmap Failed: %s", lpcstrFileName);
		hWnd = FindWindow("RACE", NULL);
		ThrowMessage(hWnd, strError);
		CloseHandle(hFile);
	}

}

//DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, VOID **lpvoid)
//{     
//	
//	HANDLE hFile = NULL;
//
//	DWORD nBytesRead = NULL;
//	DWORD nBytesImage = NULL;
//	DWORD nPixelsFile = NULL;
//
//	BITMAPFILEHEADER bmpFileHeader = {0};
//	BITMAPINFOHEADER bmpInfoHeader = {0};
//
//	if(*lpvoid)
//		return FALSE;
//	
//	if((hFile = CreateFile((LPCSTR)filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
//		ReadFile(hFile, (VOID *) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesRead, NULL);
//		ReadFile(hFile, (VOID *) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesRead, NULL);
//
//		//Compression Not Supported
//		if(bmpInfoHeader.biCompression){
//			CloseHandle(hFile);
//			return FALSE;
//		}
//
//		bmpInfoHeader.biWidth = Magnitude(bmpInfoHeader.biWidth);
//		bmpInfoHeader.biHeight = Magnitude(bmpInfoHeader.biHeight);
//
//		//Verify Three-Bytes Per Pixel
//		//if(bmpInfoHeader.biSizeImage / (bmpInfoHeader.biWidth*bmpInfoHeader.biHeight) == 3)
//		//{
//		*width = Magnitude(bmpInfoHeader.biWidth);
//		*height = Magnitude(bmpInfoHeader.biHeight);
//
//			//if(bmpInfoHeader.biWidth < 0)
//			//	*width = (bmpInfoHeader.biWidth*-1.0);
//			//else
//			//	*width = (bmpInfoHeader.biWidth);
//			//
//			//if(bmpInfoHeader.biHeight < 0)
//			//	*height = (bmpInfoHeader.biHeight*-1.0);
//			//else
//			//	*height = (bmpInfoHeader.biHeight);
//
//		nBytesImage = (*width)*(*height)*sizeof(PIXEL);
//
//		*lpvoid = (VOID *) calloc((*width)*(*height), sizeof(PIXEL));
//
//		ReadFile(hFile, (VOID *) *lpvoid, nBytesImage, &nBytesRead, NULL);
//		//}
//
//		//FlipHorzBMP(*width, *height, (PIXEL *) *lpvoid);
//
//		CloseHandle(hFile);
//	}
//
//	return TRUE;
//}

DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, VOID **lpvoid)
{     
	
	SHORT s = NULL;
	HANDLE hFile = NULL;

	DWORD nBytesRead = NULL;
	DWORD nBytesImage = NULL;
	DWORD nPixelsFile = NULL;

	BITMAPFILEHEADER bmpFileHeader = {0};
	BITMAPINFOHEADER bmpInfoHeader = {0};

	VOID *voidvoid = NULL;

	if(*lpvoid)
		return FALSE;
	
	if((hFile = CreateFile((LPCSTR)filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		ReadFile(hFile, (VOID *) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesRead, NULL);
		ReadFile(hFile, (VOID *) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesRead, NULL);

		//Compression Not Supported
		if(bmpInfoHeader.biCompression){
			*lpvoid = NULL;
			CloseHandle(hFile);
			return FALSE;
		}

		bmpInfoHeader.biWidth = Magnitude(bmpInfoHeader.biWidth);
		bmpInfoHeader.biHeight = Magnitude(bmpInfoHeader.biHeight);

		*width = Magnitude(bmpInfoHeader.biWidth);
		*height = Magnitude(bmpInfoHeader.biHeight);

		nBytesImage = (*width)*(*height)*BytesPerPixel(bmpInfoHeader.biBitCount);/*sizeof(PIXEL)*/;

		//voidvoid = (VOID *)malloc(nBytesImage);
		//voidvoid = (VOID *)calloc((*width)*(*height), BytesPerPixel(bmpInfoHeader.biBitCount));
		*lpvoid = (VOID *) calloc((*width)*(*height), BytesPerPixel(bmpInfoHeader.biBitCount));

		ReadFile(hFile, (VOID *) *lpvoid, nBytesImage, &nBytesRead, NULL);

		CloseHandle(hFile);
	}

	return BytesPerPixel(bmpInfoHeader.biBitCount);
}

DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, VOID *lpvoid)
{     
	
	SHORT s = NULL;
	HANDLE hFile = NULL;

	DWORD nBytesRead = NULL;
	DWORD nBytesImage = NULL;
	DWORD nPixelsFile = NULL;

	BITMAPFILEHEADER bmpFileHeader = {0};
	BITMAPINFOHEADER bmpInfoHeader = {0};

	VOID *voidvoid = NULL;

	if(!lpvoid)
		return FALSE;
	
	if((hFile = CreateFile((LPCSTR)filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		ReadFile(hFile, (VOID *) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesRead, NULL);
		ReadFile(hFile, (VOID *) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesRead, NULL);

		//Compression Not Supported
		if(bmpInfoHeader.biCompression){
			lpvoid = NULL;
			CloseHandle(hFile);
			return FALSE;
		}
		

		bmpInfoHeader.biWidth = Magnitude(bmpInfoHeader.biWidth);
		bmpInfoHeader.biHeight = Magnitude(bmpInfoHeader.biHeight);

		*width = Magnitude(bmpInfoHeader.biWidth);
		*height = Magnitude(bmpInfoHeader.biHeight);

		nBytesImage = (*width)*(*height)*BytesPerPixel(bmpInfoHeader.biBitCount);

		//Added 2.2.16
		//DWORD fPos;
		//fPos = SetFilePointer(hFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);

		ReadFile(hFile, (VOID *) lpvoid, nBytesImage, &nBytesRead, NULL);

		CloseHandle(hFile);
	}

	return BytesPerPixel(bmpInfoHeader.biBitCount);
}

DWORD ReadBMP(CHAR *filePath, UINT *width, UINT *height, UINT *nBytePerPixel, VOID **lpvoid)
{     
	
	SHORT s = NULL;
	HANDLE hFile = NULL;

	DWORD nBytesRead = NULL;
	DWORD nBytesImage = NULL;
	DWORD nPixelsFile = NULL;

	BITMAPFILEHEADER bmpFileHeader = {0};
	BITMAPINFOHEADER bmpInfoHeader = {0};

	if(*lpvoid)
		return FALSE;
	
	if((hFile = CreateFile((LPCSTR)filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		ReadFile(hFile, (VOID *) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesRead, NULL);
		ReadFile(hFile, (VOID *) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesRead, NULL);

		//Compression Not Supported
		if(bmpInfoHeader.biCompression){
			*lpvoid = NULL;
			CloseHandle(hFile);
			return FALSE;
		}

		bmpInfoHeader.biWidth = Magnitude(bmpInfoHeader.biWidth);
		bmpInfoHeader.biHeight = Magnitude(bmpInfoHeader.biHeight);

		*width = Magnitude(bmpInfoHeader.biWidth);
		*height = Magnitude(bmpInfoHeader.biHeight);

		nBytesImage = (*width)*(*height)*BytesPerPixel(bmpInfoHeader.biBitCount);

		*lpvoid = (VOID *) calloc((*width)*(*height), BytesPerPixel(bmpInfoHeader.biBitCount));

		ReadFile(hFile, (VOID *) *lpvoid, nBytesImage, &nBytesRead, NULL);

		*nBytePerPixel = BytesPerPixel(bmpInfoHeader.biBitCount);

		CloseHandle(hFile);
	}

	return (*width)*(*height)*BytesPerPixel(bmpInfoHeader.biBitCount);
}

DWORD ReadBMPOpenCV(CHAR *filePath, UINT *width, UINT *height, VOID *lpvoid)
{
#ifdef RACE_OPENCV
	IplImage *img = cvLoadImage(filePath, CV_LOAD_IMAGE_UNCHANGED);
	//Mat image;

	//image = imread(filePath, CV_LOAD_IMAGE_COLOR);

	//if(!image.data){
	//	return FALSE;
	//}

	if(img)free(img);

#endif

	return TRUE;
}

//Returns the number of 
DWORD GetSimilarChannelsBMP(UINT width, UINT height, PIXEL *bmp)
{

	INT i = NULL;
	INT w = width;
	INT h = height;
	INT max = w*h;

	DWORD cnt = NULL;

	for(i = 0; i < max; i++){

		if(bmp[i].r == bmp[i].g && bmp[i].g == bmp[i].b){
			cnt++;
		}
	}

	return cnt;
}

DWORD GetBMPSize(CHAR *filePath, UINT *width, UINT *height)
{

	HANDLE hFile = NULL;

	DWORD nBytesRead = NULL;
	
	BITMAPFILEHEADER	bmpFileHeader = {0};
	BITMAPINFOHEADER	bmpInfoHeader = {0};

	if((hFile = CreateFile((LPCSTR)filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
		ReadFile(hFile, (VOID *) &bmpFileHeader, sizeof(BITMAPFILEHEADER), &nBytesRead, NULL);
		ReadFile(hFile, (VOID *) &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &nBytesRead, NULL);
		CloseHandle(hFile);
	}

	*width = Magnitude(bmpInfoHeader.biWidth);
	*height = Magnitude(bmpInfoHeader.biHeight);

	return BytesPerPixel(bmpInfoHeader.biBitCount);
}

DWORD WINAPI SaveSequenceBMP(VOID *lpvoid)
{

	DATAMANAGER *dataManager = (DATAMANAGER *) lpvoid;

	DATAMANAGER *dm = (DATAMANAGER *) calloc(1, sizeof(DATAMANAGER));

	memcpy(dm, dataManager, sizeof(DATAMANAGER));

	DWORD dw = NULL;

	INT i;
	CHAR str[MAX_PATH];
	CHAR strArchive[MAX_PATH];
	CHAR strDest[MAX_PATH];
	CHAR typeData;
	CHAR typeMetaData;
	PDMFRAME *pdmFrame;
	VOID *lpVoid = NULL;
	PIXEL *pix = NULL;

	pix = (PIXEL *) calloc(dm->wStack*dm->hStack, sizeof(PIXEL));

	pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));

	sprintf(strDest, "%s\\", dm->strSrcPath);

	for(i = dm->fStart; i < dm->fStop; i++)
	//for(i = 0; i < dm->nFrame; i++)
	{
		GenerateArchivePathPDM(dm->idWnd, 
							(UINT) i, 
							(CHAR *)strDest,
							//(CHAR *)dm->strArchivePath, 
							//(CHAR *)dm->strSrcPath,
							(CHAR *)FS_RAW, 
							strArchive);

		if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpVoid)){
			ThrowError(dm->hOwner, "Error Reading Archive");
			ThrowLastError(dm->hOwner);
		}
		else{
			//sprintf(str, "%sBitmap%d.bmp", dm->strArchivePath, i);
			sprintf(str, "%s\\BMP\\", dm->strSrcPath);
			CreateDirectory(str, NULL);
			sprintf(str, "%s\\BMP\\Bitmap%d.bmp", dm->strSrcPath, i);
			UCHARtoPIXEL(dm->wStack, dm->hStack, (UCHAR *)lpVoid, pix);
			SaveBMP(dm->wStack, dm->hStack, str, pix);
			ThrowMessage(dm->hOwner, str);
		}

		if(lpVoid){
			free(lpVoid);
			lpVoid = NULL;
		}



	}
	
	if(dm)free(dm);
	if(pix)free(pix);

	return dw;

}

DWORD FlipHorzBMP(UINT w, UINT h, PIXEL *bmp)
{
	UINT i = NULL;
	UINT x = NULL;
	UINT y = NULL;
	UINT yMax = (w/2)-2;
	PIXEL *temp = NULL;
	temp = (PIXEL *) calloc(w*h, sizeof(PIXEL));

	if(!bmp | !temp)
		return FALSE;

	memcpy(temp, bmp, w*h*sizeof(PIXEL));

	for(y = 0; y < yMax; y++)
	{
		memcpy(&bmp[(h-y)*w], &temp[y*w], w*sizeof(PIXEL));
	}

	if(temp)free(temp);


	return TRUE;
}

DWORD BytesPerPixel(DWORD nBits)
{
	DWORD nBytes = (DWORD) (nBits/8);

	return nBytes;
}
