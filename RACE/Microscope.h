#pragma once

#include "stdafx.h"
#include "Point.h"


//Double Rect
typedef struct DRECT_{
	double left;
	double top;
	double right;
	double bottom;
}DRECT;

//Field of View sruct
typedef struct FOV_{
	double xPos;
	double yPos;
	double wFOV;
	double hFOV;
	DRECT drcFO;
}FOV;

//typedef struct DOUBLE_3DPOINT{
//	double x;
//	double y;
//	double z;
//}DOUBLE_3DPOINT;