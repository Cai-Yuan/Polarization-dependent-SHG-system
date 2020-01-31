#include "stdafx.h"

#include "Stack.h"

FLOAT ** CallocImageStack(DWORD width, DWORD height, DWORD nSlice)
{
	INT i = NULL;
	DWORD w = width;
	DWORD h = height;
	FLOAT **stack = (FLOAT **)calloc(nSlice, sizeof(FLOAT *));

	for(i = 0; i < nSlice; i++){

		stack[i] = (FLOAT *)calloc(w*h, sizeof(FLOAT));

	}

	return stack;
}

VOID **CallocImageSet(DWORD width, DWORD height, DWORD nElement, DWORD sizePixel)
{
	INT i = NULL;
	DWORD w = width;
	DWORD h = height;
	VOID **set = (VOID **)calloc(nElement, sizePixel);

	for(i = 0; i < nElement; i++){

		set[i] = (VOID *)calloc(w*h, sizePixel);

	}

	return set;
}

DWORD FreeImageStack(DWORD nSlice, FLOAT **stack)
{

	INT i = NULL;

	for(i = 0; i < nSlice; i++){

		if(stack[i]){
			free(stack[i]);
			stack[i] = NULL;
		}
	}

	if(stack){
		free(stack);
		stack = NULL;
	}

	return TRUE;
}