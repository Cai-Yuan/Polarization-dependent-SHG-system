#include "stdafx.h"

#include "PDM.h"

DWORD AllocPDMBuffer(PDMBUFFER *pdmBuffer, UINT nFrame)
{
	if(!pdmBuffer)
		return FALSE;

	pdmBuffer->lpFrame = (PDMFRAME *)malloc(nFrame*sizeof(PDMFRAME));
	memset(pdmBuffer->lpFrame, NULL, nFrame*sizeof(PDMFRAME));

	return TRUE;
}