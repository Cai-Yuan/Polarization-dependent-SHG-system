#include "stdafx.h"

#include "Difference.h"

FLOAT GetDifference(FLOAT d1, FLOAT d2)
{
	return abs(d1 - d2);
}

DOUBLE GetDifference(DOUBLE d1, DOUBLE d2)
{
	return abs(d1 - d2);
}

INT GetDifference(INT d1, INT d2)
{
	return abs((FLOAT)d1 - (FLOAT)d2);
}

UINT GetDifference(UINT d1, UINT d2)
{
	return abs((FLOAT)d1 - (FLOAT)d2);
}

CHAR GetDifference(CHAR d1, CHAR d2)
{
	return abs((FLOAT)d1 - (FLOAT)d2);
}

UCHAR GetDifference(UCHAR d1, UCHAR d2)
{
	return abs((FLOAT)d1 - (FLOAT)d2);
}
