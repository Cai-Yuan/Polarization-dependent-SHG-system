#include "stdafx.h"

#include "Iterations.h"

DWORD IterationsTest(HWND hwnd, DWORD nLoop1, DWORD nLoop2, DWORD nLoop3)
{
	DWORD nTotal = NULL;

	DWORD i, j, k;

	for(i = 0; i < nLoop1; i++){
		for(j = 0; j < nLoop2; j++){
			for(k = 0; k < nLoop3; k++){

				nTotal++;
			}
		}
	}

	CHAR str[MAX_STRING];
	sprintf(str, "nLoop1: %d\tnLoop2: %d\tnLoop3: %d\t nTotal: %d", nLoop1, nLoop2, nLoop3, nTotal);
	ThrowMessage(hwnd, str);

	return nTotal;
}

DWORD IterationsTest(HWND hwnd, DWORD nLoop1, DWORD nLoop2, DWORD nLoop3, DWORD nLoop4)
{
	DWORD nTotal = NULL;

	DWORD i, j, k, m;

	for(i = 0; i < nLoop1; i++){
		for(j = 0; j < nLoop2; j++){
			for(k = 0; k < nLoop3; k++){
				for(m = 0; m < nLoop4; m++){

					nTotal++;

				}
			}
		}
	}

	CHAR str[MAX_STRING];
	sprintf(str, "nLoop1: %d\tnLoop2: %d\tnLoop3: %d\tnLoop4: %d\t nTotal: %d", nLoop1, nLoop2, nLoop3, nLoop4, nTotal);
	ThrowMessage(hwnd, str);

	return nTotal;
}