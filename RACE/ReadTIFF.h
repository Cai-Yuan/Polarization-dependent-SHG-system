#pragma once

#include "stdafx.h"

UINT32 ReadTIFFWidth(CHAR *fileName);
UINT32 ReadTIFFHeight(CHAR *fileName);

BOOL VerifyStackDimensionsTIFF(FILEPATHSET *fps);
VOID CompressTIFF(UINT w, UINT h, UCHAR *imgSrc, UCHAR *imgDest);
VOID CompressTIFF2(UINT w, UINT h, UCHAR *imgSrc, UCHAR *imgDest);
VOID CompressTIFF3(UINT w, UINT h, UCHAR *imgSrc, UCHAR *imgDest);
