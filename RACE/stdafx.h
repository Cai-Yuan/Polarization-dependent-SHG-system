// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//Disable compiler warnings
#if defined(_WIN32)||defined(_WIN64)
#define _CRT_SECURE_NO_WARNINGS
#endif

//#ifndef _WIN64
#ifndef RACE_BITFLOW
#define RACE_BITFLOW
#endif
//#endif

#ifndef RACE_CHAMELEON
#define RACE_CHAMELEON
#endif

#define DIR_F "F:\\Archive\\"
#define DIR_C "C:\\Archive\\"
#define DIR_E "E:\\Archive\\"

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commctrl.h>
#include <windowsx.h>
#include "Commctrl.h"


// TODO: reference additional headers your program requires here


#include "FilePath.h"

#include "Point.h"

#include "CaptureToolBar.h"
#include "PDM.h"
#include "DisplaySurface.h"
#include "DataView.h"
#include "OpenFilePath.h"
#include "DataManager.h"
#include "RACE.h"
#include "MDI.h"
#include "ToolBar.h"
#include "Menu.h"
#include "TextEditor.h"


#include "ToolsWIN32.h"
