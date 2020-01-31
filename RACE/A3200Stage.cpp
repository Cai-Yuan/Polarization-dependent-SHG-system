#include "stdafx.h"

#include "Time.h"
#include "A3200.h"
#include "A3200Stage.h"
#include "Microscope.h"
#include "Point.h"


//Needs to be removed
#include "UNO32.h"

DWORD GetA3200Position(A3200Handle handle, DOUBLE_3DPOINT *dpt)
{

	A3200StatusGetItem(handle, 1, STATUSITEM_PositionFeedback, 0, &dpt->x);
	A3200StatusGetItem(handle, 0, STATUSITEM_PositionFeedback, 0, &dpt->y);
	A3200StatusGetItem(handle, 2, STATUSITEM_PositionFeedback, 0, &dpt->z);

	return TRUE;
}

DWORD GetA3200Position(A3200Handle handle, P3D *dpt)
{

	A3200StatusGetItem(handle, 1, STATUSITEM_PositionFeedback, 0, &dpt->x);
	A3200StatusGetItem(handle, 0, STATUSITEM_PositionFeedback, 0, &dpt->y);
	A3200StatusGetItem(handle, 2, STATUSITEM_PositionFeedback, 0, &dpt->z);

	return TRUE;
}

DWORD GetA3200PositionZ(A3200Handle handle, DOUBLE_3DPOINT *dpt)
{
	dpt->x = 0;
	dpt->y = 0;
	//A3200StatusGetItem(handle, 1, STATUSITEM_PositionFeedback, 0, &dpt->x);
	//A3200StatusGetItem(handle, 0, STATUSITEM_PositionFeedback, 0, &dpt->y);
	A3200StatusGetItem(handle, 2, STATUSITEM_PositionFeedback, 0, &dpt->z);

	return TRUE;
}

DOUBLE GetA3200PositionZ(A3200Handle handle)
{
	DOUBLE zPos;
	A3200StatusGetItem(handle, 2, STATUSITEM_PositionFeedback, 0, &zPos);

	return zPos;
}

DWORD MoveToA3200Z(A3200Handle handle, DOUBLE z, DOUBLE rate)
{
	A3200MotionMoveAbs(handle, TASKID_Library, AXISINDEX_02, z, abs(rate)/*abs(lpStage->zVelMax*0.001)*/);
	return TRUE;
}

DWORD WaitForA3200Z(A3200Handle handle, DOUBLE posGoTo)
{
	DOUBLE z = 0;
	DOUBLE zPos = 0;

	FLOAT scale = 10000;

	int zCurrent;
	int zGoTo =(int)((double)posGoTo*(double)scale);

	zPos = GetA3200PositionZ(handle);
	zCurrent = (int)((double)zPos*(double)scale);
	while(zCurrent != zGoTo){
		zPos = GetA3200PositionZ(handle);
		zCurrent = (int)((double)zPos*(double)scale);
	}

	return TRUE;
}

DWORD WaitForA3200Z(A3200Handle handle, DOUBLE posGoTo, DWORD msWait)
{
	DOUBLE z = 0;
	DOUBLE zPos = 0;
	DOUBLE goTo = Truncate5(posGoTo);
	time_t t = clock();
	time_t del = 0;

	FLOAT scale = 1000;

	int zCurrent;
	int zGoTo =(int)((double)posGoTo*(double)scale);

	zPos = GetA3200PositionZ(handle);
	zCurrent = (int)((double)zPos*(double)scale);
	while(zCurrent != zGoTo){
		del = clock()-t;
		if(del >= msWait){
			return FALSE;
		}
		zPos = GetA3200PositionZ(handle);
		zCurrent = (int)((double)zPos*(double)scale);
	}

	return TRUE;
}
