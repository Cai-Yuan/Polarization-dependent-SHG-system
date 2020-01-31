#pragma once



#ifdef RACE_BITFLOW
//#pragma comment(lib, "Cid")
//#pragma comment(lib, "BFD")
//#pragma comment(lib, "Bid")
//#pragma comment(lib, "BFEr")
//#pragma comment(lib, "DispSurf")

#pragma comment(lib, "cid.lib")
#pragma comment(lib, "bfd.lib")
#pragma comment(lib, "bid.lib")
#pragma comment(lib, "bfer.lib")
#pragma comment(lib, "dispsurf.lib")
#include "conio.h"
#include "CIApi.h"
#include "BFApi.h"
#include "BFErApi.h"
#include "DSapi.h"
#include "BiApi.h"


typedef struct BITFLOW_{

	Bd		hBoard;
	BFU32	type;
	BFU32	num;
	BFU32	init;
	BFU32	serNum;
	BFU32	boardNumber;
	BFU32	options;
	BFU32	wFrame;
	BFU32	hFrame;
	BFU32	sizeFrame;
	BFU32	bitDepth;

	BFBOOL	NoPoCL;
	BFBOOL	PoCLUp;

	BFU8	*hostBuffer;

	CiENTRY ciEntry;

}BITFLOW;

typedef struct BITFLOWBI_{

	Bd		hBoard;
	BFU32	type;
	BFU32	num;
	BFU32	init;
	BFU32	serNum;
	BFU32	boardNumber;
	BFU32	options;
	BFU32	wFrame;
	BFU32	hFrame;
	BFU32	sizeFrame;
	BFU32	bitDepth;
	BFU32	boardType;
	
	//BFU8	*hostBuffer;
	BIBA	bufArray;


}BITFLOWBI;

DWORD InitBitFlowBoard(BITFLOW *bfBoard);
DWORD InitPhotonFocusCam(BITFLOW *bfBoard);
DWORD CloseBitFlowBoard(BITFLOW *bfBoard);

DWORD InitPhotonFocusCamBi(BITFLOWBI *bfBoard, INT nBuffer);


#endif