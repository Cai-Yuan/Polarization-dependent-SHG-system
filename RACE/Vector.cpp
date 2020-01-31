#include "stdafx.h"

#include "Vector.h"


VOID GetVector(FPOINT ptHead, FPOINT ptTail, VECTOR *vector)
{
	SetVectorTailFPOINT(ptTail, vector);
	SetVectorHeadFPOINT(ptHead, vector);
	SetVectorMagnitude(vector);
	SetVectorDirection(vector);

}

VOID GetVector(FPOINT ptTail, FLOAT dir, INT mag, VECTOR *v)
{
	FPOINT ptHead;
	memset(v, 0, sizeof(VECTOR));
	SetVectorTailFPOINT(ptTail, v);

	ptHead.y = mag*sin(dir * ((float)PI/(float)180))+ptTail.y;
	ptHead.x = mag*cos(dir * ((float)PI/(float)180))+ptTail.x;

	SetVectorHeadFPOINT(ptHead, v);
	SetVectorMagnitude(v);
	SetVectorDirectionTrue(v);
}

DWORD GetVector(P3D ptHead, P3D ptTail, VECTOR *v)
{
	FPOINT fptHead;
	FPOINT fptTail;

	fptHead.x = ptHead.x;
	fptHead.y = ptHead.y;

	fptTail.x = ptTail.x;
	fptTail.y = ptTail.y;

	GetVector(fptHead, fptTail, v);

	return TRUE;
}

VOID ShiftVector(FPOINT ptShift, VECTOR *v)
{
	v->pTail.x = v->pTail.x + ptShift.x;
	v->pTail.y = v->pTail.y + ptShift.y;

	v->pHead.x = v->pHead.x + ptShift.x;
	v->pHead.y = v->pHead.y + ptShift.y;
	SetVectorMagnitude(v);
	SetVectorDirection(v);

}


VOID GetVector(UINT nVector, FPOINT *ptHead, FPOINT *ptTail, VECTOR *vector)
{
	UINT i;

	for(i = 0; i < nVector; i++){
		SetVectorTailFPOINT(ptTail[i], &vector[i]);
		SetVectorHeadFPOINT(ptHead[i], &vector[i]);
		SetVectorMagnitude(&vector[i]);
		SetVectorDirection(&vector[i]);
	}
}

VOID SetVectorTailFPOINT(FPOINT ptTail, VECTOR *vector)
{
	vector->pTail.x = ptTail.x;
	vector->pTail.y = ptTail.y;
}

VOID SetVectorHeadFPOINT(FPOINT ptHead, VECTOR *vector)
{
	vector->pHead.x = ptHead.x;
	vector->pHead.y = ptHead.y;
}

VOID SetVectorMagnitude(VECTOR *vector)
{
	FLOAT temp;
	FLOAT i;
	FLOAT j;
	FLOAT k;

	vector->i = vector->pHead.x - vector->pTail.x;
	vector->j = vector->pHead.y - vector->pTail.y;
	vector->mag = sqrt((FLOAT)(vector->i*vector->i) + (FLOAT)(vector->j*vector->j));

}

VOID SetVectorDirection(VECTOR *vector)
{
	FLOAT temp;
	FLOAT i = vector->i;
	FLOAT j = vector->j;
	FLOAT phi=0;
	FLOAT theta=0;

	phi = atan2(j, i);
	theta = phi*57.2957795;

	vector->dir = theta;
	
}

//VOID SetVectorDirection(VECTOR *vector)
//{
//	FLOAT temp;
//	FLOAT i = vector->i;
//	FLOAT j = vector->j;
//	FLOAT phi=0;
//	FLOAT theta=0;
//
//	phi = atan((j/i));
//	phi = phi*57.2957795;
//
//	if(i == 0 && j == 0){
//		vector->dir = 0;
//		return;
//	}
//
//	if(i == 0){
//
//		if(j > 0)
//			theta = 90;
//		else
//			theta = 270;
//
//		vector->dir = theta;
//		return;
//	}
//	if(j == 0){
//
//		if(i > 0)
//			theta = 0;
//		else
//			theta = 180;
//
//		vector->dir = theta;
//		return;
//	}
//
//	//Quadrants
//
//	//ONE
//	if(i >= 0 && j >= 0)
//		theta = phi;
//	//FOUR
//	else if(i > 0 && j < 0)
//		theta = 360-phi;
//	//TWO
//	else if(i < 0 && j > 0)
//		theta = 180 - phi;
//		/*theta = phi + 90;*/
//	//THREE
//	else if(i < 0 && j < 0)
//		theta = phi + 180;
//
//	vector->dir = theta;
//	
//}

VOID SetVectorDirectionTrue(VECTOR *vector)
{
	FLOAT temp;
	FLOAT i = vector->i;
	FLOAT j = vector->j;
	FLOAT phi=0;
	FLOAT theta=0;

	phi = atan2(j, i);
	//phi = atan((j/i));
	phi = phi*57.2957795;

	if(i == 0 && j == 0){
		vector->dir = 0;
		return;
	}

	if(i == 0){

		if(j > 0)
			theta = 90;
		else
			theta = 270;

		vector->dir = theta;
		return;
	}
	if(j == 0){

		if(i > 0)
			theta = 0;
		else
			theta = 180;

		vector->dir = theta;
		return;
	}

	theta = phi;

	vector->dir = theta;
	
}

VOID GetVectorPair(FPOINT pt0, FPOINT pt1, FPOINT pt2, VECTOR *v1, VECTOR *v2)
{
	FPOINT ptHead;
	FPOINT ptTail;

	memcpy(&ptHead, &FPoint(pt1.x, pt1.y), sizeof(FPOINT));
	memcpy(&ptTail, &FPoint(pt2.x, pt2.y), sizeof(FPOINT));

	GetVector(ptHead, ptTail, v1);

	memcpy(&ptHead, &FPoint(pt0.x, pt0.y), sizeof(FPOINT));
	memcpy(&ptTail, &FPoint(pt1.x, pt1.y), sizeof(FPOINT));

	GetVector(ptHead, ptTail, v2);

}

FLOAT GetIntersectionAngle(VECTOR v1, VECTOR v2, UINT uFlags)
{

	DOUBLE dotProduct = (v1.i * v2.i) + (v1.j * v2.j);
	DOUBLE magProduct = v1.mag*v2.mag;

	DOUBLE angle = acos( ((DOUBLE)dotProduct) / ((DOUBLE)magProduct));

	switch(uFlags)
	{
	case VI_DEGREE:
		angle = angle * 57.2957795;
		return (FLOAT)angle;
		break;
	case VI_RADIAN:
		return (FLOAT)angle;
		break;
	default:
		return (FLOAT)(angle * 57.2957795);
		break;
	}

}

FLOAT GetAngle(VECTOR v1, VECTOR v2, UINT uFlags)
{

	DOUBLE dotProduct = (v1.i * v2.i) + (v1.j * v2.j);
	DOUBLE magProduct = v1.mag*v2.mag;

	//dotProduct = abs(dotProduct);

	DOUBLE angle = acos( ((DOUBLE)dotProduct) / ((DOUBLE)magProduct));

	switch(uFlags)
	{
	case VI_DEGREE:
		angle = angle * 57.2957795;
		return angle;
		break;
	case VI_RADIAN:
		return angle;
		break;
	default:
		return angle * 57.2957795;
		break;
	}

}

FLOAT GetDotProduct(VECTOR v1, VECTOR v2)
{
	FLOAT dotProduct = (v1.i * v2.i) + (v1.j * v2.j);

	return dotProduct; 
}

FLOAT RadianToDegree(FLOAT radian)
{
	return radian*57.30;
}

FLOAT DegreeToRadian(FLOAT degree)
{
	return ((FLOAT)degree/(FLOAT)PI);
}