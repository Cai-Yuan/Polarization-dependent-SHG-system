#include "stdafx.h"

#include "Sign.h"

INT Sign(FLOAT data)
{
	if(data >= 0.0)
		return 1;
	else
		return -1;
}

INT Sign(DOUBLE data)
{
	if(data >= 0.0)
		return 1;
	else
		return -1;
}

INT Sign(CHAR data)
{
	if(data >= 0)
		return 1;
	else
		return -1;
}

INT Sign(INT data)
{
	if(data >= 0)
		return 1;
	else
		return -1;
}