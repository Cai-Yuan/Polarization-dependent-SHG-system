#pragma once

#include "stdafx.h"
#include "DataManager.h"

DWORD SubclassWnd(HWND hControl, WNDPROC wpSubclass);
DWORD SubclassWnd(HWND hParent, HWND hControl, WNDPROC wpSubclass);