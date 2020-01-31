#pragma once

#include "MotionCorrespondence.h"

DWORD DrawTrajectory(UINT w, UINT h, PIXEL *img, TRAJECTORY trajectory, COLORREF c);
DWORD DrawTrajectorySet(UINT w, UINT h, PIXEL *img, TRAJECTORYSET ts, COLORREF c);
DWORD AppendMatchedTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend);
DWORD AppendRemoveTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend);

DWORD TrajecotryManagmentTest(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend);
DWORD AppendTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend, UCHAR *modMap);
DWORD RemoveTrajectories(TRAJECTORYSET *ts, DWORD sizeModMap, UCHAR *modMap);

DWORD Reverse(TRAJECTORYSET *ts);
INT AreSame(TRAJECTORY *t1, TRAJECTORY *t2);
DWORD DiscardMismatches(TRAJECTORYSET *ts1, TRAJECTORYSET *ts2, TRAJECTORYSET *tsOut);