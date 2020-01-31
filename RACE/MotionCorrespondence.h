#pragma once

#include "Point.h"

typedef struct CORR_{
	FPOINT p1;
	FPOINT p2;
	FPOINT p3;
}CORR;

typedef struct CORRSET_{
	DWORD size;
	DWORD nCorr;
	CORR *corr;
}CORRSET;

typedef struct TRAJECTORY_{
	FPOINTSET ps;
}TRAJECTORY;

typedef struct TRAJECTORYSET_{
	DWORD size;
	DWORD nTrajectory;
	TRAJECTORY *trajectory;
}TRAJECTORYSET;


DWORD Free(CORRSET *cs);
DWORD Alloc(CORRSET *cs, DWORD size);
DWORD Push(CORRSET *cs, CORR corr);
DWORD Fetch(CORRSET *cs, DWORD i, CORR *cFetch);

DWORD Alloc(TRAJECTORY *t, DWORD size);
DWORD Free(TRAJECTORY *t);
DWORD Push(TRAJECTORY *t, FPOINT point);

DWORD MotionCorrespondance(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts);
DWORD MotionCorrespondanceCenterReference(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts);
DWORD MotionCorrespondanceCenterReference(FLOAT tAngle, FLOAT tDist, FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts);
//DWORD MotionCorrespondanceSmart(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts);
DWORD MotionCorrespondance(FLOAT maxAngle, FLOAT maxDelta, FLOAT maxDistance, FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts);

DWORD Alloc(TRAJECTORYSET *ts, DWORD size, DWORD lenTrajectory);
DWORD Free(TRAJECTORYSET *ts);
DWORD Push(TRAJECTORYSET *ts, TRAJECTORY trajectory);
DWORD Empty(TRAJECTORYSET *ts);
DWORD IsEmpty(TRAJECTORYSET ts);
DWORD Clear(TRAJECTORYSET *ts);
DWORD Transfer(TRAJECTORYSET *tsDest, TRAJECTORYSET *tsSrc);
DWORD Copy(TRAJECTORYSET *tsDest, TRAJECTORYSET *tsSrc);
DWORD GetSize(TRAJECTORYSET ts);
DWORD GetAt(TRAJECTORYSET ts, DWORD i, TRAJECTORY *t);
DWORD Zero(TRAJECTORYSET *ts, DWORD i);
DWORD Remove(TRAJECTORYSET *ts, DWORD i);

DWORD Alloc(TRAJECTORY *t, DWORD size);
DWORD Free(TRAJECTORY *t);
DWORD Push(TRAJECTORY *t, FPOINT point);
DWORD Copy(TRAJECTORY *tDest, TRAJECTORY *tSrc);
DWORD Transfer(TRAJECTORY *tDest, TRAJECTORY *tSrc);
DWORD Empty(TRAJECTORY *t);
DWORD GetSize(TRAJECTORY t);
DWORD GetAt(TRAJECTORY t, DWORD i, FPOINT *pt);
DWORD Zero(TRAJECTORY *t);

DWORD Reverse(TRAJECTORY *t);

DWORD Alloc(CORRSET *cs, DWORD size);
DWORD Free(CORRSET *cs);
DWORD Push(CORRSET *cs, CORR corr);
DWORD Fetch(CORRSET *cs, DWORD i, CORR *cFetch);


inline FLOAT SethiJainCost(FLOAT w1, FLOAT w2, FPOINT p1, FPOINT p2, FPOINT p3);
inline FLOAT Affinity(FLOAT sigma, FLOAT x, FLOAT y);