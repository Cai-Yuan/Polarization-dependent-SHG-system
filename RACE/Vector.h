#pragma once

#include "stdafx.h"
#include "Point.h"

#define MAX_VECTOR		256
#define VI_DEGREE		0
#define VI_RADIAN		1

typedef struct VECTOR_{
	FPOINT pTail;
	FPOINT pHead;
	FLOAT i;
	FLOAT j;
	FLOAT mag;
	FLOAT dir;
}VECTOR;

VOID GetVector(FPOINT ptHead, FPOINT ptTail, VECTOR *vector);
VOID GetVector(FPOINT ptTail, FLOAT dir, INT mag, VECTOR *v);
DWORD GetVector(P3D ptHead, P3D ptTail, VECTOR *v);
VOID GetVector(UINT nVector, FPOINT *ptHead, FPOINT *ptTail, VECTOR *vector);
VOID SetVectorTailFPOINT(FPOINT ptTail, VECTOR *vector);
VOID SetVectorHeadFPOINT(FPOINT ptHead, VECTOR *vector);
VOID SetVectorMagnitude(VECTOR *vector);
VOID SetVectorDirection(VECTOR *vector);
VOID SetVectorDirectionTrue(VECTOR *vector);
VOID ShiftVector(FPOINT ptShift, VECTOR *v);

VOID GetVectorPair(FPOINT pt0, FPOINT pt1, FPOINT pt2, VECTOR *v1, VECTOR *v2);
FLOAT GetIntersectionAngle(VECTOR v1, VECTOR v2, UINT uFlags);
FLOAT GetAngle(VECTOR v1, VECTOR v2, UINT uFlags);
FLOAT GetDotProduct(VECTOR v1, VECTOR v2);

FLOAT RadianToDegree(FLOAT radian);
FLOAT DegreeToRadian(FLOAT degree);