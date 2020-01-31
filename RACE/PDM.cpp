#include "stdafx.h"

//#include "stdio.h"
#include "PDM.h"
#include "CUDAFunDll.h"

LRESULT DropPDMFrame(HWND hwnd, PDMFRAME pdmf)
{
	PDMFRAME *pdmfDrop = (PDMFRAME *)malloc(sizeof(PDMFRAME));
	memset(pdmfDrop, NULL, sizeof(PDMFRAME));
	AllocPDMFrame(pdmfDrop, pdmf.pdmfh.width, pdmf.pdmfh.height, DT_UCHAR);
	//AllocPDMFrame(pdmfDrop, pdmf.pdmfh.width, pdmf.pdmfh.height, pdmf.pdmfh.typeData);
	FrameCopy(*pdmfDrop, pdmf);
	PostMessage(hwnd, WM_DATADROP, (WPARAM)pdmfDrop, NULL);

	return TRUE;
}

LRESULT DropPDMFrame(HWND hwnd, PDMFRAME pdmf, CHAR *strType)
{
	PDMFRAME *pdmfDrop = (PDMFRAME *)malloc(sizeof(PDMFRAME));
	memset(pdmfDrop, NULL, sizeof(PDMFRAME));
	AllocPDMFrame(pdmfDrop, pdmf.pdmfh.width, pdmf.pdmfh.height, DT_UCHAR);
	//AllocPDMFrame(pdmfDrop, pdmf.pdmfh.width, pdmf.pdmfh.height, pdmf.pdmfh.typeData);
	FrameCopy(*pdmfDrop, pdmf);
	PostMessage(hwnd, WM_DATADROP, (WPARAM)pdmfDrop, (LPARAM)strType);

	return TRUE;
}

DWORD FreeDrop(PDMFRAME *pdmfDrop)
{
	if(!pdmfDrop)
		return FALSE;

	if(pdmfDrop->lpvoid)free(pdmfDrop->lpvoid);
	if(pdmfDrop)free(pdmfDrop);
	pdmfDrop = NULL;

	return TRUE;
}

DWORD ArchivePDM(CHAR typeData, CHAR typeMetaData, PDMFRAME pdmFrame, LPSTR lpsFilePath, LPVOID lpData)
{
	CHAR idPDM[]={'P', '7'};
	HANDLE hFile;
	DWORD nBytesFile = 0;
	DWORD nBytesWritten = 0;
	LONG w = pdmFrame.pdmfh.width;
	LONG h = pdmFrame.pdmfh.height;

	//Calculate Number of Bytes Needed For Data
	LONG nBytesData = w*h*GetDataTypeSize(typeData);

	//Calculate Number of Bytes Needed for Meta-Data
	LONG nBytesMetaData = sizeof(PDMFRAMEHEADER);

	if(nBytesData && lpData){

		if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

			//Append 'P7' to File (First Two Bytes of File)
			WriteFile(hFile, &idPDM, sizeof(idPDM), &nBytesWritten, NULL);
			nBytesFile += nBytesWritten;

			//Append Meta-Data Size to File
			WriteFile(hFile, &nBytesMetaData, sizeof(nBytesMetaData), &nBytesWritten, NULL);
			nBytesFile += nBytesWritten;

			//Append Meta-Data to File
			WriteFile(hFile, &pdmFrame.pdmfh, sizeof(PDMFRAMEHEADER), &nBytesWritten, NULL);
			nBytesFile += nBytesWritten;

			//Append Data to File
			WriteFile(hFile, pdmFrame.lpvoid, nBytesData, &nBytesWritten, NULL);
			nBytesFile += nBytesWritten;

			CloseHandle(hFile);
		}
	}

	return nBytesFile;
}



WORD GetFilePathPDM(INT iWnd, INT iFrame, INT typeData, CHAR *strDirectory, CHAR *strPath)
{

	if(!strDirectory || !strPath)
		return NULL;


	switch(typeData)
	{
	case DT_CHAR:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_CHAR);
		break;
	case DT_SHORT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_SHORT);
		break;
	case DT_UCHAR:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_UCHAR);
		break;
	case DT_INT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_INT);
		break;
	case DT_UINT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_UINT);
		break;
	case DT_FLOAT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_FLOAT);
		break;
	case DT_PIXEL:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_PIXEL);
		break;
	case DT_POINT:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_POINT);
		break;
	case DT_CIRCLE:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_CIRCLE);
		break;
	case DT_ELLIPSE:
		sprintf(strPath, "%s%d_%d_%s.pdm", strDirectory, iWnd, iFrame, STR_ELLIPSE);
		break;
	}


	return TRUE;

}

LONG GetDataTypeSize(UCHAR dataType)
{

	switch(dataType){
		case DT_UCHAR:
			return sizeof(UCHAR);
		case DT_CHAR:
			return sizeof(CHAR);
		case DT_UINT:
			return sizeof(UINT);
		case DT_INT:
			return sizeof(INT);
		case DT_PIXEL:
			return sizeof(PIXEL);
		case DT_FLOAT:
			return sizeof(FLOAT);
		case DT_DOUBLE:
			return sizeof(DOUBLE);
		case DT_LONG:
			return sizeof(LONG);
		case DT_LONGPIXEL:
			return sizeof(LONGPIXEL);
		default:
			return 0;
	}
}

BOOL VerifyStackDimensionsPDM(FILEPATHSET *fps)
{

	LONG w = 0;
	LONG h = 0;
	UINT nPaths = fps->nPaths;
	UINT i;

	GetPDMDimensions(fps->filePath[0].lpstr, &w, &h);
	//w = GetPDMWidth(fps->filePath[0].lpstr);
	//h = GetPDMHeight(fps->filePath[0].lpstr);

	for(i = 0+1; i < nPaths; i++){

		if(w != GetPDMWidth(fps->filePath[i].lpstr) || h != GetPDMHeight(fps->filePath[i].lpstr))
			return FALSE;

	}

	return TRUE;
}

BOOL VerifyStackDimensionsPDM(STRINGSET *ss)
{

	if(!ss)
		return FALSE;

	LONG w = 0;
	LONG h = 0;
	UINT nPaths = ss->nStr;
	UINT i;

	GetPDMDimensions(ss->string[0].str, &w, &h);
	//w = GetPDMWidth(ss->string[0].str);
	//h = GetPDMHeight(ss->string[0].str);

	for(i = 0+1; i < nPaths; i++){

		if(w != GetPDMWidth(ss->string[i].str) || h != GetPDMHeight(ss->string[i].str))
			return FALSE;

	}

	return TRUE;
}

DWORD GetPDMDimensions(CHAR *strFilePath, LONG *width, LONG *height)
{
	if(!strFilePath || !width || !height)
		return FALSE;

	DWORD nBytesRead = 0;
	CHAR id[2] = {0};
	HANDLE hFile = NULL;

	LONG sizeMetaData = 0;
	PDMFRAMEHEADER pdmfh = {0};

	if((hFile = CreateFile((LPSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

		ReadFile(hFile, id, 2*sizeof(CHAR), &nBytesRead, NULL);
		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);
		ReadFile(hFile, &pdmfh, sizeMetaData, &nBytesRead, NULL);
	}

	CloseHandle(hFile);
	*width = pdmfh.width;
	*height = pdmfh.height;

	return TRUE;
}

DWORD GetPDMWidth(CHAR *strFilePath)
{
	DWORD nBytesRead = 0;
	CHAR id[2] = {0};
	HANDLE hFile = NULL;

	LONG sizeMetaData = 0;
	PDMFRAMEHEADER pdmfh = {0};

	if((hFile = CreateFile((LPSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

		ReadFile(hFile, id, 2*sizeof(CHAR), &nBytesRead, NULL);
		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);
		ReadFile(hFile, &pdmfh, sizeMetaData, &nBytesRead, NULL);
	}

	CloseHandle(hFile);

	return pdmfh.width;
}

DWORD GetPDMHeight(CHAR *strFilePath)
{
	DWORD nBytesRead = 0;
	CHAR id[2] = {0};
	HANDLE hFile = NULL;

	LONG sizeMetaData = 0;
	PDMFRAMEHEADER pdmfh = {0};

	if((hFile = CreateFile((LPSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

		ReadFile(hFile, id, 2*sizeof(CHAR), &nBytesRead, NULL);
		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);
		ReadFile(hFile, &pdmfh, sizeMetaData, &nBytesRead, NULL);
	}

	CloseHandle(hFile);



	return pdmfh.height;
}

DWORD Getpdmfh(CHAR *strFilePath, PDMFRAMEHEADER *pdmfh)
{
	DWORD nBytesRead = 0;
	CHAR id[2] = {0};
	HANDLE hFile = NULL;

	LONG sizeMetaData = 0;

	if((hFile = CreateFile((LPSTR)strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){

		ReadFile(hFile, id, 2*sizeof(CHAR), &nBytesRead, NULL);
		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);
		ReadFile(hFile, pdmfh, sizeMetaData, &nBytesRead, NULL);
	}

	CloseHandle(hFile);



	return nBytesRead;
}

DWORD CopyPDMFrame(PDMFRAME *pdmfDest, PDMFRAME *pdmfSrc)
{
	if(!pdmfDest || !pdmfSrc){
		return FALSE;
	}

	//Added 8.26.14
	//Unverified
	if(!pdmfDest->lpvoid || !pdmfSrc->lpvoid)
		return FALSE;

	if(pdmfDest->pdmfh.typeData != pdmfSrc->pdmfh.typeData){
		return FALSE;
	}

	if(pdmfDest->pdmfh.width != pdmfSrc->pdmfh.width){
		return FALSE;
	}

	if(pdmfDest->pdmfh.height != pdmfSrc->pdmfh.height){
		return FALSE;
	}

	memcpy(pdmfDest->lpvoid, pdmfSrc->lpvoid, pdmfSrc->pdmfh.width*pdmfSrc->pdmfh.height*GetDataTypeSize(pdmfSrc->pdmfh.typeData));

	return TRUE;
}

DWORD CopyPDMFrameUCHARtoPIXEL(PDMFRAME *pdmfDest, PDMFRAME *pdmfSrc)
{
	if(!pdmfDest || !pdmfSrc){
		return FALSE;
	}

	if(!pdmfDest->lpvoid || !pdmfSrc->lpvoid)
		return FALSE;

	if(pdmfDest->pdmfh.typeData != DT_PIXEL)
		return FALSE;

	if(pdmfSrc->pdmfh.typeData != DT_UCHAR)
		return FALSE;

	if(pdmfDest->pdmfh.width != pdmfSrc->pdmfh.width){
		return FALSE;
	}

	if(pdmfDest->pdmfh.height != pdmfSrc->pdmfh.height){
		return FALSE;
	}

	UCHARtoPIXEL(pdmfDest->pdmfh.width, pdmfDest->pdmfh.height, (UCHAR *)pdmfSrc->lpvoid, (PIXEL *)pdmfDest->lpvoid);

	return TRUE;
}

DWORD AllocPDMFrame(PDMFRAME *pdmFrame, UINT w, UINT h, DWORD flgDataType)
{
	if(!pdmFrame)
		return FALSE;

	DWORD nByte = GetDataTypeSize(flgDataType);

	memset(pdmFrame, NULL, sizeof(PDMFRAME));

	pdmFrame->lpvoid = (VOID *)calloc(w*h, nByte);
	pdmFrame->pdmfh.width = w;
	pdmFrame->pdmfh.height = h;
	pdmFrame->pdmfh.typeData = flgDataType;
	pdmFrame->pdmfh.countMaps = 1;
	pdmFrame->pdmfh.sizeData = w*h*nByte;
	pdmFrame->pdmfh.typeMetaData = MD_FRAME;

	return TRUE;
}

DWORD cudaAllocPDMFrame(PDMFRAME *pdmFrame, UINT w, UINT h, DWORD flgDataType)
{
	if(!pdmFrame)
		return FALSE;

	memset(pdmFrame, NULL, sizeof(PDMFRAME));

	if(pdmFrame->lpvoid){
		free(pdmFrame->lpvoid);
	}

	//ccudaMallocHost(&pdmFrame->lpvoid, w*h*GetDataTypeSize(flgDataType));
	pdmFrame->pdmfh.width = w;
	pdmFrame->pdmfh.height = h;
	pdmFrame->pdmfh.typeData = flgDataType;
	pdmFrame->pdmfh.countMaps = 1;
	pdmFrame->pdmfh.sizeData = w*h*GetDataTypeSize(flgDataType);

	return TRUE;
}

DWORD AllocPDMFrameSet(PDMFRAME *pdmFrame, UINT w, UINT h, DWORD flgDataType, DWORD nFrame)
{

	INT i = NULL;

	for(i = 0; i < nFrame; i++)
	{

		AllocPDMFrame(&pdmFrame[i], w, h, flgDataType);

	}

	return TRUE;
}

DWORD SetPDMFrameUCHAR(PDMFRAME *pdmf, UCHAR value)
{
	if(!pdmf){
		return FALSE;
	}

	if(pdmf->pdmfh.typeData != DT_UCHAR){
		return FALSE;
	}

	if(!pdmf->lpvoid){
		return FALSE;
	}

	memset(pdmf->lpvoid, value, pdmf->pdmfh.width*pdmf->pdmfh.height*sizeof(UCHAR));
	

	return TRUE;
}

DWORD SetPDMFrameINT(PDMFRAME *pdmf, INT value)
{
	if(!pdmf){
		return FALSE;
	}

	if(pdmf->pdmfh.typeData != DT_INT){
		return FALSE;
	}

	if(!pdmf->lpvoid){
		return FALSE;
	}

	memset(pdmf->lpvoid, value, pdmf->pdmfh.width*pdmf->pdmfh.height*sizeof(INT));
	

	return TRUE;
}

DWORD SetPDMFramePIXEL(PDMFRAME *pdmf, PIXEL *img)
{
	if(!img || !pdmf->lpvoid)
		return FALSE;

	memcpy(pdmf->lpvoid, img, pdmf->pdmfh.width*pdmf->pdmfh.height*sizeof(PIXEL));

	return TRUE;
}

DWORD FreePDMFrameData(PDMFRAME *pdmFrame)
{

	if(!pdmFrame)
		return FALSE;

	if(!pdmFrame->lpvoid)
		return FALSE;

	free(pdmFrame->lpvoid);
	pdmFrame->lpvoid = NULL;

	return TRUE;
}

DWORD FreePDM(PDMFRAME *pdmFrame)
{

	if(!pdmFrame)
		return FALSE;

	if(!pdmFrame->lpvoid)
		return FALSE;

	free(pdmFrame->lpvoid);
	pdmFrame->lpvoid = NULL;

	return TRUE;
}

//Copies only the pixel bits
DWORD FrameCopy(PDMFRAME pdmfDest, PDMFRAME pdmfSrc)
{
	DWORD nByte = pdmfSrc.pdmfh.width*pdmfSrc.pdmfh.height;
	nByte = nByte*GetDataTypeSize(pdmfSrc.pdmfh.typeData);

	if(!pdmfDest.lpvoid)
		return FALSE;

	if(!pdmfSrc.lpvoid)
		return FALSE;

	memcpy(&pdmfDest.pdmfh, &pdmfSrc.pdmfh, sizeof(PDMFRAMEHEADER));
	memcpy(pdmfDest.lpvoid, pdmfSrc.lpvoid, nByte);


	return TRUE;
}

LRESULT SendPDMFrame(HWND hwnd, PDMFRAME *pdmf)
{
	return SendMessage(hwnd, WM_SHOWFRAME, (WPARAM) pdmf, (LPARAM) pdmf->lpvoid);
}

COLOR ColorR(UCHAR r, UCHAR g, UCHAR b)
{
	COLOR c = {0};
	c.r = r;
	c.g = g;
	c.b = b;

	return c;
}

//Returns true if they are the same size
//False otherwise
DWORD SameSize(PDMFRAMEHEADER pdmfh1, PDMFRAMEHEADER pdmfh2)
{
	if(pdmfh1.width != pdmfh2.width)
		return FALSE;

	if(pdmfh1.height != pdmfh2.height)
		return FALSE;

	if(pdmfh1.typeData != pdmfh2.typeData)
		return FALSE;

	return TRUE;
}


DWORD LONGtoPIXEL(DWORD nPixel, LONGPIXEL *imgIn, PIXEL *imgOut)
{
	DWORD i = 0;

	if(!imgIn||!imgOut)
		return FALSE;

	for(i = 0; i < nPixel; i++){
		imgOut[i].r = imgIn[i].r;
		imgOut[i].g = imgIn[i].g;
		imgOut[i].b = imgIn[i].b;
		//memcpy(&imgOut[i], &imgIn[/*nPixel-*/i], sizeof(PIXEL));
	}

	return FALSE;
}

DWORD FlipVertical(DWORD w, DWORD h, PIXEL *img)
{

	if(!img)
		return FALSE;

	DWORD i = 0;
	DWORD iFlip = 0;
	DWORD x = 0;
	DWORD y = 0;
	PDMFRAME pdmf;
	PIXEL *flip = NULL;
	AllocPDMFrame(&pdmf, w, h, DT_PIXEL);

	if(!pdmf.lpvoid)
		return FALSE;

	flip = (PIXEL *) pdmf.lpvoid;

	for(y = 0; y < h; y++)
	{
		//Row*Width+X
		i = y*w;
		iFlip = (h-y)*w;
		for(x = 0; x < 200; x++){
			memcpy(&flip[iFlip+x], &img[i+x], sizeof(PIXEL));
		}
			//memcpy(&flip[iFlip], &img[i], (200*sizeof(PIXEL)-1));
	}
		
	memcpy(img, flip, w*h*sizeof(PIXEL));
	
	FreePDM(&pdmf);


	return TRUE;
}

//2.20.16
//UNTESTED
DWORD Alloc(PDMFRAMESET *pdmfs, DWORD nFrame, DWORD w, DWORD h, DWORD flgDataType)
{
	if(!pdmfs)
		return FALSE;

	if(pdmfs->pdmf)
		return FALSE;

	pdmfs->pdmf = (PDMFRAME *)malloc(nFrame*sizeof(PDMFRAME));

	if(!pdmfs->pdmf)
		return FALSE;

	pdmfs->size = nFrame;
	pdmfs->nFrame = NULL;
	memset(pdmfs->pdmf, NULL, nFrame*sizeof(PDMFRAME));

	DWORD i = 0;
	for(i = 0; i < nFrame; i++){
		AllocPDMFrame(&pdmfs->pdmf[i], w, h, flgDataType);
	}

	return TRUE;
}

//LONG ReadPDMFrame(CHAR *typeData, CHAR *typeMetaData, PDMFRAME *pdmFrame, LPSTR lpsFilePath, VOID **lpvoid)
//{
//	CHAR id[2];
//	CHAR idPDM[]={'P', '7'};
//	HANDLE hFile;
//	LONG sizeData;
//	LONG sizeMetaData;
//
//	LONG sMetaData = sizeof(PDMFRAMEHEADER);
//	DWORD nBytesRead = 0;
//
//	LONG nBytesMetaData = sizeof(PDMFRAMEHEADER);
//
//	PDMFRAMEHEADER pdmfh;
//
//	//Ensure lpvoid is free
//	if(*lpvoid){
//		free(*lpvoid);
//	}
//
//
//	memset(id, 0x00, sizeof(id));
//	if((hFile = CreateFile((LPSTR)lpsFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE){
//		
//		//Get Magic Number 'P7'+'/0'
//		ReadFile(hFile, &id, 2*sizeof(CHAR), &nBytesRead, NULL);
//		
//		//Get Size of Meta-Data
//		ReadFile(hFile, &sizeMetaData, sizeof(sizeMetaData), &nBytesRead, NULL);
//
//		if(sMetaData == sizeMetaData){
//			//Read Meta-Data
//			ReadFile(hFile, &pdmfh, sizeMetaData, &nBytesRead, NULL);
//
//			//Check Meta-Data Type
//			//(MD_FRAME)
//			if(pdmfh.typeMetaData == MD_FRAME){
//
//				sizeData = GetDataTypeSize(pdmfh.typeData);
//
//				//TEMP
//				sizeData = pdmfh.width*pdmfh.height;
//				pdmFrame->pdmfh.sizeData = sizeData;
//				*lpvoid = (VOID *) calloc(sizeData, sizeof(UCHAR));
//				if(*lpvoid){
//					LONG nBytesData =  sizeData;
//					ReadFile(hFile, *lpvoid, nBytesData, &nBytesRead, NULL);
//				}
//				else
//					return FALSE;
//			}
//			else
//				return FALSE;
//		}
//		else
//			return FALSE;
//
//
//		CloseHandle(hFile);
//		return TRUE;
//	}
//	else
//		return FALSE;
//
//
//}

