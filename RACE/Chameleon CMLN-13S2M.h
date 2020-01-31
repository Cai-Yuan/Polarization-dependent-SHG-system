#pragma once

#include "stdafx.h"
#include "C/FlyCapture2_C.h"

DWORD WINAPI ChameleonThread(LPVOID param);
fc2Error fc2GetResolution(fc2Context fcContext, int *w, int *h);