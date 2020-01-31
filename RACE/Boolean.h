#pragma once

#include "stdafx.h"

DWORD BooleanAND(UINT w, UINT h, INT *inOne, INT *inTwo, INT *out);
DWORD BooleanAND(UINT w, UINT h, FLOAT *inOne, FLOAT *inTwo, FLOAT *out);
DWORD Boolean(UINT w, UINT h, INT set, INT *in, INT *out);
DWORD Boolean(UINT w, UINT h, FLOAT set, FLOAT *in,  FLOAT *out);