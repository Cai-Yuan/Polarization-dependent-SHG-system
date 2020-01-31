#pragma once

#include "stdafx.h"
#include "CommCtrl.h"

HWND CreateToolBar(HWND hwndOwner);
HWND CreateToolBar(HWND hwndOwner, DWORD idControl, DWORD dwStyle);

DWORD InitializeToolBar(HWND hToolBar);