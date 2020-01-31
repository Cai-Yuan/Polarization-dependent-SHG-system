#include "stdafx.h"

//cid.lib 
//bfd.lib 
//bfer.lib 
//dispsurf.lib 
//bid.lib

//#ifdef __x86_64_
#ifdef RACE_BITFLOW
//#ifdef _WIN64
#include "BitFlow.h"
#include "conio.h"
#include "CiApi.h"
#include "CiDef.h"
#include "BFApi.h"
#include "BFErApi.h"
#include "DSapi.h"
#include "BiApi.h"
#include "BFErApi.h"
//#endif
#endif


//#ifdef _WIN64
//#include "BitFlow.h"
//#include "conio.h"
//#include "CiApi.h"
//#include "CiDef.h"
//#include "BFApi.h"
//#include "BFErApi.h"
//#include "DSapi.h"
//#endif

#ifdef RACE_BITFLOW
//Initialize the DAQ Board for the Camera
DWORD InitBitFlowBoard(BITFLOW *bfBoard)
{
	BFUPTR bfuPtr;
	bfBoard->boardNumber = 0xffff;
	bfuPtr = DoBrdOpenDialog(BOD_HIDEJUSTOPEN, FF_BITFLOW_MODERN, &bfBoard->type, &bfBoard->num, &bfBoard->init, &bfBoard->serNum);
	switch(bfuPtr){
		case BRD_DLG_OK:
			break;
		case BRD_DLG_NO_BOARDS:
			break;
		case BRD_DLG_USER_CANCEL:
			break;
		case BRD_DLG_MALLOC_ERROR:
			break;
		case BRD_DLG_DRIVER_ERROR:
			break;
		case BRD_DLG_OTHER_ERROR:
			break;
		default:
			break;
	}
	CiSysBrdFind(bfBoard->type, bfBoard->num, &bfBoard->ciEntry);

	return bfBoard->num;

}

//Initialize the Camera
DWORD InitPhotonFocusCam(BITFLOW *bfBoard)
{

	if(!bfBoard)
		return FALSE;

	memset(bfBoard, NULL, sizeof(BITFLOW));
	InitBitFlowBoard(bfBoard);

	bfBoard->options = CiSysInitialize;

	if (bfBoard->NoPoCL)
		bfBoard->options |= CISysNoPoCLChange;

	if (bfBoard->PoCLUp)
		bfBoard->options |= CISysPoCLUpOnly;

	if (CiBrdOpen(&bfBoard->ciEntry, &bfBoard->hBoard, bfBoard->options))
	{
	}

	//BFRC status;
	//PBFCNF pCam;
	//status = CiCamOpen(bfBoard->hBoard, "PhotonFocus-EL1-D1312-160-CL-W2-OneShot-SWTrig.r64", &pCam);

	//if(status != CI_OK)
	//{
	//	INT i;
	//	i = 0;
	//}

	CiBrdInquire(bfBoard->hBoard,CiCamInqXSize,&bfBoard->wFrame);
	CiBrdInquire(bfBoard->hBoard,CiCamInqYSize0,&bfBoard->hFrame);
	CiBrdInquire(bfBoard->hBoard,CiCamInqFrameSize0,&bfBoard->sizeFrame);
	CiBrdInquire(bfBoard->hBoard,CiCamInqPixBitDepth,&bfBoard->bitDepth);

	bfBoard->hostBuffer = (BFU8 *)malloc(bfBoard->sizeFrame);

	if (CiAqSetup(bfBoard->hBoard,  (PBFVOID)bfBoard->hostBuffer,  bfBoard->sizeFrame, 0, CiDMADataMem,
				  CiLutBypass, CiLut8Bit, CiQTabBank0, TRUE, CiQTabModeOneBank, 
				  AqEngJ))
	{
		free(bfBoard->hostBuffer);
		return FALSE;
	}

	return TRUE;
}

//Initilaze the Camera Using a circular buffer
DWORD InitPhotonFocusCamBi(BITFLOWBI *bfBoard, INT nBuffer)
{
	BFRC Error;

	if(DoBrdOpenDialog(BOD_HIDEJUSTOPEN, FF_BITFLOW_MODERN, &bfBoard->boardType, &bfBoard->boardNumber, &bfBoard->init, &bfBoard->serNum))
	{
		return FALSE;
	}

	Error = BiBrdOpen(BiTypeAny, bfBoard->boardNumber, &bfBoard->hBoard);
	if(Error != BI_OK)
	{
		BiErrorShow(bfBoard->hBoard, Error);
		return FALSE;
	}

	Error = BiBufferAllocCam(bfBoard->hBoard, &bfBoard->bufArray, nBuffer);
	if(Error != BI_OK)
	{
		BiErrorShow(bfBoard->hBoard, Error);
		BiBrdClose(bfBoard->hBoard);
		return FALSE;
	}

	Error = BiSeqAqSetup(bfBoard->hBoard, &bfBoard->bufArray, BiAqEngJ);
	if(Error != BI_OK)
	{
		BiErrorShow(bfBoard->hBoard, Error);
		BiBufferFree(bfBoard->hBoard, &bfBoard->bufArray);
		BiBrdClose(bfBoard->hBoard);
	}

	//Parameter 3 is in ms
	BiInternalTimeoutSet(bfBoard->hBoard, &bfBoard->bufArray, 1000);

	return TRUE;
}

DWORD CloseBitFlowBoard(BITFLOW *bfBoard)
{

	CiAqCleanUp(bfBoard->hBoard, AqEngJ);
	if(bfBoard->hostBuffer) free(bfBoard->hostBuffer);
	CiBrdClose(bfBoard->hBoard);
	return TRUE;
}
#endif
//CiAqCleanUp(g_PFCAM.hBoard, AqEngJ);
//free(g_PFCAM.hostBuffer);
//CiBrdClose(g_PFCAM.hBoard);