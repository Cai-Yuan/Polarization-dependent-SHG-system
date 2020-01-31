#include "stdafx.h"

#include "Subtract.h"


FLOAT Subtract(TIMESTAMP ts1, TIMESTAMP ts2)
{
	FLOAT del = NULL;
	INT t1 = (ts1.wSecond * 1000) + (ts1.wMilliseconds);
	INT t2 = (ts2.wSecond * 1000) + (ts2.wMilliseconds);
	//INT delDay = Magnitude(ts1.wDay-ts1.;
	//INT delHour;
	//INT delMinute;
	//INT delSecond = Magnitude(ts2.wSecond - ts1.wSecond);
	//INT delMilli = Magnitude(ts2.wMilliseconds - ts1.wMilliseconds);

	//del = (1000*delSecond) + (delMilli);

	return Magnitude(t1 - t2);
}
