#include "stdafx.h"
#include "Math.h"

//Addition
//Subtraction
//Multiplication
//Division


DWORD Add(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(	!in1 || 
		!in2 || 
		!out)
		return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = in1[i]+in2[i];

	}

	return i;
}

DWORD Subtract(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(	!in1 || 
		!in2 || 
		!out)
		return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = in1[i]-in2[i];

	}

	return i;
}

//Multiplication
DWORD Multiply(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(	!in1 || 
		!in2 || 
		!out)
		return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = in1[i]*in2[i];

	}

	return i;
}

DWORD Divide(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(	!in1 || 
		!in2 || 
		!out)
		return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = in1[i]/in2[i];

	}

	return i;
}

DWORD Max(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(	!in1 || 
		!in2 || 
		!out)
		return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){

		out[i] = max(in1[i], in2[1]);

	}

	return i;
}

DWORD AND(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(!in1||
		!in2||
		!out)
	return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){
		if(in1[i] == in2[i])
			out[i] = in1[i];
		else
			out[i] = 0;
	}

	return i;
}

DWORD XOR(DWORD n, FLOAT *in1, FLOAT *in2, FLOAT *out)
{
	if(!in1||
		!in2||
		!out)
	return FALSE;

	DWORD i = NULL;

	for(i = 0; i < n; i++){
		if(in1[i] && !in2[i])
			out[i] = in1[i];
		if(in2[i] && !in1[i])
			out[i] = in2[i];
	}

	return i;
}
//AKA: Dynamic Range Expansion
//AKA: Stretch()??
DWORD Normalize(DWORD w, DWORD h, FLOAT *in1, FLOAT *out)
{
	if(!in1 || !out)return FALSE;
	
	return Stretch(w, h, 0, 255, in1, out);

}