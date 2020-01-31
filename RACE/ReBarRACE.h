#pragma once

#include "stdafx.h"

HWND CreateRebar(HWND hOwner);
HWND BuildRebar(HWND hOwner, HWND hToolBar);

DWORD BuildCameraRebarBand(HWND hOwner, HWND hRebar);
DWORD BuildFileRebarBand(HWND hOwner, HWND hRebar);
