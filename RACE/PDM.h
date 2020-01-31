#pragma once

#include "stdafx.h"
#include "stdio.h"
#include "OpenFilePath.h"

#define BGR


/****************PORTABLE DATA MAP****************/
/*******************FILE FORMAT*******************/

//Define Meta Data Types
#define MD_FRAME		1
#define MD_WINDOW		2

//Define Data Types <--- Part of 'Magic Number'
#define DT_UCHAR		7

#define DT_CHAR			8
#define DT_UINT			9
#define DT_INT			10
#define	DT_PIXEL		11
#define DT_FLOAT		12
#define DT_DOUBLE		13
//#define DT_LONG			14
#define DT_SHORT		15
#define DT_CIRCLE		51
#define DT_LONG			56
#define DT_LONGPIXEL	55
#define DT_RENDERING	57
#define DT_RAW			58

#define DT_POINT		50
#define DT_ELLIPSE		52
#define DT_WINDOW		53

//Data Type Strings
//-->Used During Data Archive and Retrieval
#define STR_CHAR		"CHAR"
#define STR_UCHAR		"UCHAR"
#define STR_SHORT		"SHORT"
#define STR_INT			"INT"
#define STR_UINT		"UINT"
#define STR_FLOAT		"FLOAT"
#define STR_PIXEL		"PIXEL"
#define	STR_POINT		"POINT"
#define STR_FPOINT		"FPOINT"
#define STR_EDGE		"EDGE"
#define STR_FEDGE		"FEDGE"
#define STR_ELLIPSE		"ELLIPSE"
#define STR_FELLIPSE	"FELLIPSE"
#define STR_CIRCLE		"CIRCLE"
#define STR_FCIRCLE		"FCIRCLE"
#define STR_LINE		"LINE"
#define STR_FLINE		"FLINE"
#define STR_RENDERING	"RENDERING"
//#define STR_TIFF		"TIFF"
//etc..

//Filter Strings
#define FS_RAW		"RAW"
#define FS_DX		"DX"
#define FS_DY		"DY"
#define FS_DXDY		"DXDY"
#define FS_DYDX		"DYDX"
#define FS_SURF		"SURF"
//etc..

typedef double DOUBLE;

typedef struct PIXEL_{
#ifdef BGR
	BYTE b;
	BYTE g;
	BYTE r;
#else
	BYTE r;
	BYTE g;
	BYTE b;
#endif
}PIXEL;

typedef struct LONGPIXEL_{
#ifdef BGR
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
#else
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
#endif
}LONGPIXEL;

typedef struct COLOR_{
#ifdef BGR
	BYTE b;
	BYTE g;
	BYTE r;
#else
	BYTE r;
	BYTE g;
	BYTE b;
#endif
}COLOR;

typedef SYSTEMTIME	TIMESTAMP;

typedef struct PDMHEADER_{
	CHAR id[2];			//Magic Number -- Identifies What Type of Data
	LONG sizeMetaData;		//In Bytes
	//LONG typeMetaData;	<---Needs to be here
}PDMHEADER;

//11.14.2012
//>>Added LONG iFrame
typedef struct PDMFRAMEHEADER_{
	UCHAR typeMetaData;		//Magic Number -- Identifies What Type of Meta-Data
	UCHAR typeData;			//Magic Number -- Identifies What Type of Data
	UCHAR verMetaData;		//MetaData Version
	INT sizeData;			//Size of Data in Bytes
	LONG frame;				//Pos in frame stack
	LONG width;				//In Pixels
	LONG height;			//In Pixels
	TIMESTAMP timeStamp;	//In Win32 'SYSTEMTIME'
	UCHAR bitPrecision;		//An attribute of the camera or ADC	//Added 11.14.2012
	LONG countMaps;			//Number of Data Maps in the file
	UCHAR dataSource;		//Data Source (e.g. Camera, Software, etc..)
	//Added 8.15.12
	//UCHAR bitsPerPixel;		//An attribute of the camera
	//UCHAR sourceObjective;	//Predefined Indicators

	//Removed 11.14.2012
	//LONG timeStamp;		//In milliseconds
	
	//TOADD
	//Store 3D position and orientation of the field of view
	//Camera calibration metrics

}PDMFRAMEHEADER;

///NEW/////
typedef struct PDMHEADER8_{
	CHAR id[2];
	LONG sizeMetaData;
	UCHAR typeMetaData;
}PDMHEADER8;

typedef struct PDMDATAHEADER_{
	UCHAR typeData;
	DWORD nDataSamples;
	DWORD sizeDataType;
	TIMESTAMP timeStamp;
}PDMDATAHEADER;

typedef struct PDMDATA_{
	PDMDATAHEADER pdmDataHeader;
	LPVOID lpvoid;
}PDMDATA;
///////NEW////////

typedef struct PDMFRAME_{
	PDMFRAMEHEADER pdmfh;	//Meta-Data
	LPVOID lpvoid;					//Data
}PDMFRAME;

typedef struct PDMFFRAMESET_{
	DWORD size;
	DWORD nFrame;
	PDMFRAME *pdmf;
}PDMFRAMESET;

typedef struct PDMARCHIVEFRAME_{
	LONG	iWin;
	LONG	iFrame;
	CHAR	strPath[MAX_STRING];
	PDMFRAME pdmFrame;
}PDMARCHIVEFRAME;


//PDM - Window
typedef struct PDMWINDOWHEADER_{
	UCHAR typeMetaData;
	UCHAR typeData;
	UCHAR verMetaData;
	INT sizeData;
	INT nWindows;

}PDMWINDOWHEADER;

typedef struct PDMWINDOW_{
	PDMWINDOWHEADER pdmWindowHeader;
	VOID *lpvoid;

}PDMWINDOW;

typedef struct PDMWINDOWDATA_{
	INT idWindow;
	CHAR strText[MAX_STRING];
	CHAR strClass[MAX_STRING];
	RECT rcWindow;
}PDMWINDOWDATA;

//typedef struct PDMWINDOWDATA_{
//	INT idWindow;
//	RECT rcWindow;
//}PDMWINDOWDATA;

typedef struct PDMBUFFER_{
	UINT size;
	UINT start;
	UINT end;
	PDMFRAME *lpFrame;
}PDMBUFFER;

#define INIT_PDMFRAME(X) PDMFRAME X = {.pdmfh = {0}, .lpvoid = 0}
//In Code:
//PDMFRAME pdmf;
//INIT_PDMFRAME(pdmf);

COLOR ColorR(UCHAR r, UCHAR g, UCHAR b);

LONG GetDataTypeSize(UCHAR dataType);


LRESULT DropPDMFrame(HWND hwnd, PDMFRAME pdmf);
LRESULT DropPDMFrame(HWND hwnd, PDMFRAME pdmf, CHAR *strType);
DWORD FreeDrop(PDMFRAME *pdmfDrop);

DWORD ArchivePDM(CHAR typeData, CHAR typeMetaData, PDMFRAME pdmFrame, LPSTR lpsFilePath, LPVOID lpData);
WORD GetFilePathPDM(INT iWnd, INT iFrame, INT typeData, CHAR *strDirectory, CHAR *strPath);
BOOL VerifyStackDimensionsPDM(FILEPATHSET *fps);
DWORD GetPDMWidth(CHAR *strFilePath);
DWORD GetPDMHeight(CHAR *strFilePath);
DWORD GetPDMDimensions(CHAR *strFilePath, LONG *width, LONG *height);

DWORD Getpdmfh(CHAR *strFilePath, PDMFRAMEHEADER *pdmfh);

//LONG ReadPDMFrame(CHAR *typeData, CHAR *typeMetaData, PDMFRAME *pdmFrame, LPSTR lpsFilePath, VOID **lpvoid);


DWORD AllocPDMBuffer(PDMBUFFER *pdmBuffer, UINT size);
DWORD AllocPDMFrame(PDMFRAME *pdmFrame, UINT w, UINT h, DWORD flgDataType);
DWORD AllocPDMFrameSet(PDMFRAME *pdmFrame, UINT w, UINT h, DWORD flgDataType, DWORD nFrame);
DWORD FreePDMFrameData(PDMFRAME *pdmFrame);
DWORD FreePDM(PDMFRAME *pdmFrame);
DWORD FrameCopy(PDMFRAME pdmfDest, PDMFRAME pdmfSrc);
LRESULT SendPDMFrame(HWND hwnd, PDMFRAME *pdmf);

DWORD CopyPDMFrame(PDMFRAME *pdmfDest, PDMFRAME *pdmfSrc);
DWORD CopyPDMFrameUCHARtoPIXEL(PDMFRAME *pdmfDest, PDMFRAME *pdmfSrc);

DWORD SetPDMFrameUCHAR(PDMFRAME *pdmf, UCHAR value);
DWORD SetPDMFrameINT(PDMFRAME *pdmf, INT value);
DWORD SetPDMFramePIXEL(PDMFRAME *pdmf, PIXEL *img);

DWORD SameSize(PDMFRAMEHEADER pdmfh1, PDMFRAMEHEADER pdmfh2);

DWORD FlipVertical(DWORD w, DWORD h, PIXEL *img);
DWORD LONGtoPIXEL(DWORD nPixel, LONGPIXEL *imgIn, PIXEL *imgOut);

DWORD Alloc(PDMFRAMESET *pdmfs, DWORD nFrame, DWORD w, DWORD h, DWORD flgDataType);