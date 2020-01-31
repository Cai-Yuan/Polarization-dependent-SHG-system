#pragma once

#include "stdafx.h"

#ifdef RACE_OPENCV

#pragma comment(lib, "opencv_core245.lib")
#pragma comment(lib, "opencv_flann245.lib")
#pragma comment(lib, "opencv_highgui245.lib")
#pragma comment(lib, "opencv_features2d245.lib")
#pragma comment(lib, "opencv_core245d.lib")
#pragma comment(lib, "opencv_flann245d.lib")
#pragma comment(lib, "opencv_highgui245d.lib")
#pragma comment(lib, "opencv_features2d245d.lib")

#endif RACE_OPENCV


DWORD WINAPI MosaicThread(LPVOID param);