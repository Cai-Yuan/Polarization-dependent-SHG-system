#pragma once

#include "stdafx.h"

typedef UCHAR GREYREF;
typedef UCHAR GRAYREF;

typedef struct BGR_{
	UCHAR b;
	UCHAR g;
	UCHAR r;
}BGR;

typedef struct IMGCHAR_{
	UINT w;
	UINT h;
	CHAR *img;
}IMGCHAR;

typedef struct IMGUCHAR_{
	UINT w;
	UINT h;
	UCHAR *img;
}IMGUCHAR;

typedef struct IMG_{
	UINT w;
	UINT h;
	UCHAR *img;
}IMG;

typedef struct IMGSET_{
	DWORD nElement;
	IMGUCHAR *element;
}IMGSET;

//Define Grayscale Values for Contour Functions
#define GV_OUTSIDE	0
#define GV_ZERO		0
#define GV_SEED		251
#define	GV_POINT	252
#define GV_INSIDE	253
#define	GV_CONTOUR	255
#define GV_EDGE		254



#define CV_OUTSIDE	0x0001
#define CV_INSIDE	0x0002
#define	CV_CONTOUR	0x0003
#define CV_EDGE		0x0004
#define	CV_POINT	0x0005

#define CR_OUTSIDE	0x0001
#define CR_INSIDE	0x0002
#define	CR_CONTOUR	0x0003
#define CR_EDGE		0x0004
#define	CR_POINT	0x0005


void InitializeIndex(UINT **, UINT, UINT);
void FreeIndexMem(UINT **, UINT, UINT);

