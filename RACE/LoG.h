#pragma once

#include "stdafx.h"

VOID BlobDetect(UINT width, UINT height, FLOAT s, UCHAR *imgIn, UCHAR *imgOut);
VOID BlobDetect(UINT width, UINT height, FLOAT s, UCHAR *imgIn, INT *imgOut);
VOID GetBlobDetectKernel(FLOAT s, INT *kernel);