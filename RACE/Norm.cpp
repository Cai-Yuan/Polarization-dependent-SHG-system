#include "stdafx.h"

#include "Norm.h"


FLOAT Norm(FLOAT v1, FLOAT v2)
{
	return (FLOAT)sqrt((v1*v1)+(v2*v2));
}

DOUBLE Norm(DOUBLE v1, DOUBLE v2)
{
	return (DOUBLE)sqrt((v1*v1)+(v2*v2));
}