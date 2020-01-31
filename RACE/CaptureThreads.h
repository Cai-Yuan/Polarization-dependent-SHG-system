#pragma once

#include "stdafx.h"

HWND GetLinkedProc(HWND hwnd, BOOL *flag);
DWORD ReleaseLinkedData(HWND hLink, PDMFRAME *pdmFrame);