#include "stdafx.h"
#include "Point.h"
#include "Vector.h"

#include "MotionCorrespondence.h"

//'Data Preprocessing'


//NOTES:

//Useful Techniques
//(% Difference) = abs(v1 - v2)*2 / v1 + v2
//Min Max Normilization = (v_i)-Vmin / (Vmax-Vmin)


//Feature Based Correspondence
//Uses Three Point Sets to determine 
//Notes discrepencies in the number of fps1 and fps3 particles
//Stores the energy for each trajectory found and then removes trajectories based on their 
////energy level and a function in the difference of fps1 and fps3 set sizes

//Trajectory matches should be analyzed after pairing has occured and removed based on global trajectory parameters.
//Parameter metrics to be evalulated are mean, median, standard deviation w/ outliars (Q1, Q2, Q3)

//'Particle Tracking Velocimetry in Three-Dimensional Flows'
//N.A. Malik (1993)
//Defines the PSDR ('Particle Spacing Displacement Ratio')
//Suggests a conservative approach where it's deemed better not to make a track link if there is
//some ambiguity involved in selecting the linkage points rather than producing an erronious link


//Feature Based Correspondence
//Uses Three Point Sets to determine kinematic matching constraints

//Feature Based Correspondence
//Uses Three Point Sets to determine 
DWORD MotionCorrespondance(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	DWORD iP1 = NULL;
	DWORD iP2 = NULL;
	DWORD iP3 = NULL;

	DWORD nP1 = fps1->nPoint;
	DWORD nP2 = fps2->nPoint;
	DWORD nP3 = fps3->nPoint;

	VECTOR v1 = {0};
	VECTOR v2 = {0};
	VECTOR v3 = {0};

	FPOINT p1 = {0};
	FPOINT p2 = {0};
	FPOINT p3 = {0};

	FLOAT angle = NULL;
	FLOAT aV1V3 = NULL;
	FLOAT aV1V2 = NULL;
	FLOAT aV2V3 = NULL;
	FLOAT degV1V2 = NULL;

	FLOAT d1 = NULL;
	FLOAT d2 = NULL;
	FLOAT dDif = NULL;
	FLOAT dAverage = NULL;
	FLOAT distance = NULL;
	FLOAT dMinMax = NULL;
	FLOAT dAmplitude = NULL;
	FLOAT dGeoMean = NULL;

	FLOAT dDifMin = NULL;
	FLOAT dNorm = NULL;

	FLOAT d1Min = GetMinimumDistance(*fps1, *fps2);
	FLOAT d2Min = GetMinimumDistance(*fps2, *fps3);

	FLOAT d1Max = GetMaximumDistance(*fps1, *fps2);
	FLOAT d2Max = GetMaximumDistance(*fps2, *fps3);

	FLOAT dAffinity = NULL;


	DWORD iMin = NULL;
	DOUBLE energy = NULL;
	DOUBLE minEnergy = 10000;

	INT iMin1 = -1;
	INT iMin2 = -1;
	INT iMin3 = -1;

	DWORD nTrajectory = NULL;
	TRAJECTORY t = {0};
	Alloc(&t, 3);

	for(iP3 = 0; iP3 < nP3; iP3++){
		dDifMin = 100000000;
		minEnergy =500000000000000;
		iMin1 = -1;
		iMin2 = -1;
		iMin3 = -1;

		memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));			//Load p3
		for(iP2 = 0; iP2 < nP2; iP2++){
			memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));		//Load p2
			for(iP1 = 0; iP1 < nP1; iP1++){
				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1

				GetVector(p2, p1, &v1);
				GetVector(p3, p2, &v2);
				GetVector(p3, p1, &v3);
				
				//Calculate Angles of the Vector Traingle
				//Three Vectors --> Three Angles
				angle = GetAngle(v1, v2, VI_RADIAN);

				aV1V2 = GetAngle(v1, v2, VI_RADIAN);
				aV1V3 = GetAngle(v1, v3, VI_RADIAN);
				aV2V3 = GetAngle(v2, v3, VI_RADIAN);

				degV1V2 = abs(aV1V2*57.295);

				//Calculate Velocity Coherence
				//d - 'Displacement'
				d1 = GetDistance(p1, p2);
				d2 = GetDistance(p2, p3);
				dAverage = (FLOAT)((FLOAT)(d1+d2)/(FLOAT)2.0);
				dDif = (FLOAT)abs(d1-d2) / (FLOAT)(d1+d2);
				//dDif = dDif*max(d1, d2);
				//dDif = (FLOAT)abs(d1-d2) / (FLOAT)dAverage;
				distance = v1.mag+v2.mag;
				dGeoMean = sqrt((d1*d1)+(d2*d2));
				//dNorm = (d1Min/d1) + (d2Min/d2);
				//dNorm = 2-dNorm;
				dNorm = (FLOAT)(d1Min+d2Min)/(FLOAT)(d1+d2);
				dNorm = 1-dNorm;
				//dNorm = (FLOAT)(d1+d2)/(FLOAT)(d1Min+d2Min);
				//dNorm = 1-dNorm;
				//dAffinity = Affinity(.1, d1, d1Max)+Affinity(.1, d2, d2Max);
				//dAffinity = Affinity(.5, d1, d2);

				energy = SethiJainCost(1, 2, p1, p2, p3)+dNorm;

				//energy = (FLOAT)dDif + ((FLOAT)degV1V2/(FLOAT)180) + dAffinity/*(1-dAffinity)*/;

				//energy = SethiJainCost(.1, .9, p1, p2, p3);

				//energy = (FLOAT)dDif + ((FLOAT)degV1V2/(FLOAT)180) + dNorm;
				

				//energy = dDif*(distance/v3.mag) + (abs(cos(aV1V2+(3.14159265/2)))*(distance/v3.mag)) + (1-(v3.mag / (distance)))*(distance/v3.mag);
				//energy = energy*distance;

				//energy = dDif + (degV1V2/180);

				//Dividing by 45 weights angle more
				//energy = (dDif / .5) + (degV1V2/20);
			//energy = (1-min(d1,d2)/max(d1,d2)) + (degV1V2/35);
				//energy = (dDif*dGeoMean + (degV1V2/180)*dGeoMean)/dGeoMean;

				//dAmplitude = ((FLOAT)max(d1, d2)/(FLOAT)min(d1,d2))/* * distance*/;
				//energy = dDif*dAmplitude + (degV1V2/180)*dAmplitude/* + (1-(v3.mag / (distance)))*/;

				//dMinMax = (1-(min(v1.mag, v2.mag)/max(v1.mag, v2.mag)));
				//energy = dMinMax + (degV1V2/180) + (1-(v3.mag / (distance)));

				//energy = dDif + (degV1V2/180) + (1-(v3.mag / (distance)));
				//energy = dDif*distance + ((FLOAT)degV1V2/(FLOAT)180)*distance + distance;

				//if(d1 < 300 && d2 < 300){
				//if(v3.mag > v1.mag && v3.mag > v2.mag){
				//if(degV1V2 < 45){
					//if(degV1V2+90 < 180){
					//if(distance < 500){					//Isolates particle mismatches due to uneven partile detection between frames
						if(energy < minEnergy){
							iMin1 = iP1;
							iMin2 = iP2;
							iMin3 = iP3;
							minEnergy = energy;
							dDifMin = dDif;
						}
					//}
					//}
				//}
				//}
				//}

			}

		}

		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){

				Push(&t, fps1->point[iMin1]);
				Push(&t, fps2->point[iMin2]);
				Push(&t, fps3->point[iMin3]);
			
				Push(ts, t);

		}

		Empty(&t);

	}

	Free(&t);

	return TRUE;
}

//Feature Based Correspondence
//Uses Three Point Sets to determine 
//'tAngle' - Angle Threshold
//'tDist' - Distance Threshold
DWORD MotionCorrespondanceCenterReference(FLOAT tAngle, FLOAT tDist, FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	DWORD iP1 = NULL;
	DWORD iP2 = NULL;
	DWORD iP3 = NULL;

	DWORD nP1 = fps1->nPoint;
	DWORD nP2 = fps2->nPoint;
	DWORD nP3 = fps3->nPoint;

	VECTOR v1 = {0};
	VECTOR v2 = {0};
	VECTOR v3 = {0};

	FPOINT p1 = {0};
	FPOINT p2 = {0};
	FPOINT p3 = {0};

	FLOAT angle = NULL;
	FLOAT aV1V3 = NULL;
	FLOAT aV1V2 = NULL;
	FLOAT aV2V3 = NULL;
	FLOAT degV1V2 = NULL;

	FLOAT d1 = NULL;
	FLOAT d2 = NULL;
	FLOAT dDif = NULL;
	FLOAT dAverage = NULL;
	FLOAT distance = NULL;
	FLOAT dMinMax = NULL;
	FLOAT dAmplitude = NULL;
	FLOAT dGeoMean = NULL;

	FLOAT dDifMin = NULL;
	FLOAT dNorm = NULL;

	FLOAT d1Min = GetMinimumDistance(*fps1, *fps2);
	FLOAT d2Min = GetMinimumDistance(*fps2, *fps3);

	FLOAT d1Max = GetMaximumDistance(*fps1, *fps2);
	FLOAT d2Max = GetMaximumDistance(*fps2, *fps3);

	FLOAT dAffinity = NULL;


	DWORD iMin = NULL;
	DOUBLE energy = NULL;
	DOUBLE minEnergy = 10000;

	INT iMin1 = -1;
	INT iMin2 = -1;
	INT iMin3 = -1;

	DWORD nTrajectory = NULL;
	TRAJECTORY t = {0};
	Alloc(&t, 3);

	for(iP2 = 0; iP2 < nP2; iP2++){
		dDifMin = 100000000;
		minEnergy =500000000000000;
		iMin1 = -1;
		iMin2 = -1;
		iMin3 = -1;

		memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));			//Load p2  //Search through all middle positions (green)
		for(iP3 = 0; iP3 < nP3; iP3++){
			memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));		//Load p3
			for(iP1 = 0; iP1 < nP1; iP1++){
				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1

				GetVector(p2, p1, &v1);
				GetVector(p3, p2, &v2);
				GetVector(p3, p1, &v3);
				
				//Calculate Angles of the Vector Traingle
				//Three Vectors --> Three Angles
				angle = GetAngle(v1, v2, VI_RADIAN);

				aV1V2 = GetAngle(v1, v2, VI_RADIAN);
				aV1V3 = GetAngle(v1, v3, VI_RADIAN);
				aV2V3 = GetAngle(v2, v3, VI_RADIAN);

				degV1V2 = abs(aV1V2*57.295);

				//Calculate Velocity Coherence
				//d - 'Displacement'
				d1 = GetDistance(p1, p2);
				d2 = GetDistance(p2, p3);
				dAverage = (FLOAT)((FLOAT)(d1+d2)/(FLOAT)2.0);
				dDif = (FLOAT)abs(d1-d2) / (FLOAT)(d1+d2);
				//dDif = dDif*max(d1, d2);
				//dDif = (FLOAT)abs(d1-d2) / (FLOAT)dAverage;
				distance = v1.mag+v2.mag;
				dGeoMean = sqrt((d1*d1)+(d2*d2));
				//dNorm = (d1Min/d1) + (d2Min/d2);
				//dNorm = 2-dNorm;
				dNorm = (FLOAT)(d1Min+d2Min)/(FLOAT)(d1+d2);
				dNorm = 1-dNorm;

				if(angle > DegreeToRadian(tAngle))
					break;
				//if(distance > tDist)
				//	break;

				energy = SethiJainCost(1, 2, p1, p2, p3)+dNorm;

				if(energy < minEnergy){
					iMin1 = iP1;
					iMin2 = iP2;
					iMin3 = iP3;
					minEnergy = energy;
					dDifMin = dDif;
				}
			}

		}

		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){

			//Push points onto trajectory
			Push(&t, fps1->point[iMin1]);
			Push(&t, fps2->point[iMin2]);
			Push(&t, fps3->point[iMin3]);
		
			//Push trajectory onto trajectory set
			Push(ts, t);

		}

		Empty(&t);

	}

	Free(&t);

	return TRUE;
}

//Feature Based Correspondence
//Uses Three Point Sets to determine 
DWORD MotionCorrespondanceCenterReference(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	DWORD iP1 = NULL;
	DWORD iP2 = NULL;
	DWORD iP3 = NULL;

	DWORD nP1 = fps1->nPoint;
	DWORD nP2 = fps2->nPoint;
	DWORD nP3 = fps3->nPoint;

	VECTOR v1 = {0};
	VECTOR v2 = {0};
	VECTOR v3 = {0};

	FPOINT p1 = {0};
	FPOINT p2 = {0};
	FPOINT p3 = {0};

	FLOAT angle = NULL;
	FLOAT aV1V3 = NULL;
	FLOAT aV1V2 = NULL;
	FLOAT aV2V3 = NULL;
	FLOAT degV1V2 = NULL;

	FLOAT d1 = NULL;
	FLOAT d2 = NULL;
	FLOAT dDif = NULL;
	FLOAT dAverage = NULL;
	FLOAT distance = NULL;
	FLOAT dMinMax = NULL;
	FLOAT dAmplitude = NULL;
	FLOAT dGeoMean = NULL;

	FLOAT dDifMin = NULL;
	FLOAT dNorm = NULL;

	FLOAT d1Min = GetMinimumDistance(*fps1, *fps2);
	FLOAT d2Min = GetMinimumDistance(*fps2, *fps3);

	FLOAT d1Max = GetMaximumDistance(*fps1, *fps2);
	FLOAT d2Max = GetMaximumDistance(*fps2, *fps3);

	FLOAT dAffinity = NULL;


	DWORD iMin = NULL;
	DOUBLE energy = NULL;
	DOUBLE minEnergy = 10000;

	INT iMin1 = -1;
	INT iMin2 = -1;
	INT iMin3 = -1;

	DWORD nTrajectory = NULL;
	TRAJECTORY t = {0};
	Alloc(&t, 3);

	for(iP2 = 0; iP2 < nP2; iP2++){
		dDifMin = 100000000;
		minEnergy =500000000000000;
		iMin1 = -1;
		iMin2 = -1;
		iMin3 = -1;

		memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));			//Load p2  //Search through all middle positions (green)
		for(iP3 = 0; iP3 < nP3; iP3++){
			memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));		//Load p3
			for(iP1 = 0; iP1 < nP1; iP1++){
				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1

				GetVector(p2, p1, &v1);
				GetVector(p3, p2, &v2);
				GetVector(p3, p1, &v3);
				
				//Calculate Angles of the Vector Traingle
				//Three Vectors --> Three Angles
				angle = GetAngle(v1, v2, VI_RADIAN);

				aV1V2 = GetAngle(v1, v2, VI_RADIAN);
				aV1V3 = GetAngle(v1, v3, VI_RADIAN);
				aV2V3 = GetAngle(v2, v3, VI_RADIAN);

				degV1V2 = abs(aV1V2*57.295);

				//Calculate Velocity Coherence
				//d - 'Displacement'
				d1 = GetDistance(p1, p2);
				d2 = GetDistance(p2, p3);
				dAverage = (FLOAT)((FLOAT)(d1+d2)/(FLOAT)2.0);
				dDif = (FLOAT)abs(d1-d2) / (FLOAT)(d1+d2);
				//dDif = dDif*max(d1, d2);
				//dDif = (FLOAT)abs(d1-d2) / (FLOAT)dAverage;
				distance = v1.mag+v2.mag;
				dGeoMean = sqrt((d1*d1)+(d2*d2));
				//dNorm = (d1Min/d1) + (d2Min/d2);
				//dNorm = 2-dNorm;
				dNorm = (FLOAT)(d1Min+d2Min)/(FLOAT)(d1+d2);
				dNorm = 1-dNorm;
				//dNorm = (FLOAT)(d1+d2)/(FLOAT)(d1Min+d2Min);
				//dNorm = 1-dNorm;
				//dAffinity = Affinity(.1, d1, d1Max)+Affinity(.1, d2, d2Max);
				//dAffinity = Affinity(.5, d1, d2);

				//energy = SethiJainCost(.1, .9, p1, p2, p3);
				energy = SethiJainCost(1, 2, p1, p2, p3)+dNorm;

				//energy = (FLOAT)dDif + ((FLOAT)degV1V2/(FLOAT)180) + dAffinity/*(1-dAffinity)*/;

				//energy = SethiJainCost(.1, .9, p1, p2, p3);

				//energy = (FLOAT)dDif + ((FLOAT)degV1V2/(FLOAT)180) + dNorm;
				

				//energy = dDif*(distance/v3.mag) + (abs(cos(aV1V2+(3.14159265/2)))*(distance/v3.mag)) + (1-(v3.mag / (distance)))*(distance/v3.mag);
				//energy = energy*distance;

				//energy = dDif + (degV1V2/180);

				//Dividing by 45 weights angle more
				//energy = (dDif / .5) + (degV1V2/20);
			//energy = (1-min(d1,d2)/max(d1,d2)) + (degV1V2/35);
				//energy = (dDif*dGeoMean + (degV1V2/180)*dGeoMean)/dGeoMean;

				//dAmplitude = ((FLOAT)max(d1, d2)/(FLOAT)min(d1,d2))/* * distance*/;
				//energy = dDif*dAmplitude + (degV1V2/180)*dAmplitude/* + (1-(v3.mag / (distance)))*/;

				//dMinMax = (1-(min(v1.mag, v2.mag)/max(v1.mag, v2.mag)));
				//energy = dMinMax + (degV1V2/180) + (1-(v3.mag / (distance)));

				//energy = dDif + (degV1V2/180) + (1-(v3.mag / (distance)));
				//energy = dDif*distance + ((FLOAT)degV1V2/(FLOAT)180)*distance + distance;

				//if(d1 < 300 && d2 < 300){
				//if(v3.mag > v1.mag && v3.mag > v2.mag){
				//if(degV1V2 < 45){
					//if(degV1V2+90 < 180){
					//if(distance < 500){					//Isolates particle mismatches due to uneven partile detection between frames
						if(energy < minEnergy){
							iMin1 = iP1;
							iMin2 = iP2;
							iMin3 = iP3;
							minEnergy = energy;
							dDifMin = dDif;
						}
					//}
					//}
				//}
				//}
				//}

			}

		}

		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){

			//Push points onto trajectory
			Push(&t, fps1->point[iMin1]);
			Push(&t, fps2->point[iMin2]);
			Push(&t, fps3->point[iMin3]);
		
			//Push trajectory onto trajectory set
			Push(ts, t);

		}

		Empty(&t);

	}

	Free(&t);

	return TRUE;
}

//p1 = p_k-1
//p2 = p_k
//p3 = p_k+1
inline FLOAT SethiJainCost(FLOAT w1, FLOAT w2, FPOINT p1, FPOINT p2, FPOINT p3)
{
	FLOAT cost = NULL;

	VECTOR v1 = {0};
	VECTOR v2 = {0};

	FLOAT m1 = NULL;
	FLOAT m2 = NULL;
	FLOAT magProduct = NULL;

	FLOAT dp = NULL;
	
	GetVector(p1, p2, &v1);
	GetVector(p2, p3, &v2);

	dp = GetDotProduct(v1, v2);
	magProduct = v1.mag*v2.mag;

	cost = w1*(1-(dp / (v1.mag*v2.mag))) + w2*(1-(sqrt(2*v1.mag*v2.mag))/(v1.mag + v2.mag));

	return cost;
}

inline FLOAT Affinity(FLOAT sigma, FLOAT x, FLOAT y)
{
	FLOAT affinity = NULL;
	FLOAT dif = x-y;
	FLOAT power = (-1.0*(1/(2*sigma*sigma))*dif*dif);
	affinity = exp(power);

	return affinity;

}


//DWORD MotionCorrespondance(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
//{
//
//	if(!ts){
//		return FALSE;
//	}
//
//	DWORD iP1 = NULL;
//	DWORD iP2 = NULL;
//	DWORD iP3 = NULL;
//
//	DWORD nP1 = fps1->nPoint;
//	DWORD nP2 = fps2->nPoint;
//	DWORD nP3 = fps3->nPoint;
//
//	VECTOR v1 = {0};
//	VECTOR v2 = {0};
//	VECTOR v3 = {0};
//
//	FPOINT p1 = {0};
//	FPOINT p2 = {0};
//	FPOINT p3 = {0};
//
//	FLOAT angle = NULL;
//	FLOAT aV1V3 = NULL;
//	FLOAT aV1V2 = NULL;
//	FLOAT aV2V3 = NULL;
//	FLOAT degV1V2 = NULL;
//
//	FLOAT d1 = NULL;
//	FLOAT d2 = NULL;
//	FLOAT dDif = NULL;
//	FLOAT dAverage = NULL;
//	FLOAT distance = NULL;
//
//	FLOAT dDifMin = NULL;
//
//	DWORD iMin = NULL;
//	DOUBLE energy = NULL;
//	DOUBLE minEnergy = 10000;
//
//	INT iMin1 = -1;
//	INT iMin2 = -1;
//	INT iMin3 = -1;
//
//	DWORD nTrajectory = NULL;
//	TRAJECTORY t = {0};
//	Alloc(&t, 3);
//
//	for(iP3 = 0; iP3 < nP3; iP3++){
//		dDifMin = 100000000;
//		minEnergy =500000000000000;
//		iMin1 = -1;
//		iMin2 = -1;
//		iMin3 = -1;
//
//		memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));			//Load p3
//		for(iP2 = 0; iP2 < nP2; iP2++){
//			memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));		//Load p2
//			for(iP1 = 0; iP1 < nP1; iP1++){
//				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1
//
//				GetVector(p2, p1, &v1);
//				GetVector(p3, p2, &v2);
//				GetVector(p3, p1, &v3);
//				
//				//Calculate Angles of the Vector Traingle
//				//Three Vectors --> Three Angles
//				angle = GetAngle(v1, v2, VI_RADIAN);
//
//				aV1V2 = GetAngle(v1, v2, VI_RADIAN);
//				aV1V3 = GetAngle(v1, v3, VI_RADIAN);
//				aV2V3 = GetAngle(v2, v3, VI_RADIAN);
//
//				degV1V2 = aV1V2*57.295;
//
//				//Calculate Velocity Coherence
//				//d - 'Displacement'
//				d1 = GetDistance(p1, p2);
//				d2 = GetDistance(p2, p3);
//				dAverage = (FLOAT)((FLOAT)(d1+d2)/(FLOAT)2.0);
//				dDif = (FLOAT)abs(d1-d2) / (FLOAT)d1+d2;
//				//dDif = (FLOAT)abs(d1-d2) / (FLOAT)dAverage;
//				distance = v1.mag+v2.mag;
//
//				//energy = dDif*(distance/v3.mag) + (abs(cos(aV1V2+(3.14159265/2)))*(distance/v3.mag)) + (1-(v3.mag / (distance)))*(distance/v3.mag);
//				//energy = energy*distance;
//				//energy = dDif*(distance) + (abs(cos(aV1V2+(PI/2)))*(distance) + (1-(v3.mag / (distance)))*distance;
//				//energy = dDif*distance + (abs(cos(aV1V2+(PI/2)))*distance) + distance;
//				//energy = dDif*distance + (abs(cos(aV1V2+(PI/2)))*distance) + distance;
//
//				//Works well for small displacements
//				
//				energy = dDif*distance + ((FLOAT)degV1V2/(FLOAT)180)*distance + distance;
//				//energy = (0.33)*dDif*distance + (0.33)*sin(aV1V2)*distance + (0.33)*distance;
//				//energy = 0.33*(dDif*v3.mag) + 0.33*((aV1V2/90)*v3.mag/* + (aV1V3/90)*v3.mag*/) + 0.33*(v3.mag);
//
//				//energy = 0.33*(dDif*v3.mag) + 0.33*((aV1V2/90)*v3.mag + (aV1V3/90)*v3.mag) + 0.33*(v3.mag);
//
//				//Allows strange matches
//				//energy = sin(dDif)*v3.mag + sin(abs(aV1V2-aV1V3))*v3.mag + v3.mag;
//
//				//Allows strange matches
//				//energy = sin(dDif)*v3.mag*v3.mag + sin(aV1V2)*v3.mag + /*sin(aV2V3)*v3.mag + */sin(aV1V3)*v3.mag + v3.mag;
//
//				//Does not perform excellent during ceratin low spatial resolution tracking where both particles are moving in parallel
//				//energy = sin(dDif)*v3.mag + sin(aV1V2)*v3.mag + sin(aV1V3)*v3.mag + v3.mag;
//
//
//				if(v3.mag > v1.mag && v3.mag > v2.mag){
//					if(degV1V2+90 < 180){
//					if(distance < 500){					//Isolates particle mismatches due to uneven partile detection between frames
//						if(energy < minEnergy){
//							iMin1 = iP1;
//							iMin2 = iP2;
//							iMin3 = iP3;
//							minEnergy = energy;
//							dDifMin = dDif;
//						}
//					}
//					}
//				}
//
//			}
//
//		}
//
//		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){
//
//				Push(&t, fps1->point[iMin1]);
//				Push(&t, fps2->point[iMin2]);
//				Push(&t, fps3->point[iMin3]);
//			
//				Push(ts, t);
//
//		}
//
//		Empty(&t);
//
//	}
//
//
//	return TRUE;
//}

//DWORD MotionCorrespondance(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
//{
//
//	if(!ts){
//		return FALSE;
//	}
//
//	DWORD iP1 = NULL;
//	DWORD iP2 = NULL;
//	DWORD iP3 = NULL;
//
//	DWORD nP1 = fps1->nPoint;
//	DWORD nP2 = fps2->nPoint;
//	DWORD nP3 = fps3->nPoint;
//
//	VECTOR v1 = {0};
//	VECTOR v2 = {0};
//	VECTOR v3 = {0};
//
//	FPOINT p1 = {0};
//	FPOINT p2 = {0};
//	FPOINT p3 = {0};
//
//	FLOAT angle = NULL;
//	FLOAT anglNorm = NULL;
//	FLOAT aDegree = NULL;
//	FLOAT aRadian = NULL;
//	FLOAT aDifference = NULL;
//
//	FLOAT aV1V3 = NULL;
//	FLOAT aV1V2 = NULL;
//	FLOAT aV2V3 = NULL;
//
//	FLOAT d = NULL;
//	FLOAT dNorm = NULL;
//
//	FLOAT dd = NULL;		//Represents the diference in distances
//	FLOAT dDif = NULL;
//	FLOAT dMax = NULL;
//	//FLOAT dMin = NULL;
//
//	FLOAT d1 = NULL;
//	FLOAT d2 = NULL;
//	FLOAT del = NULL;
//	FLOAT delNorm = NULL;
//
//	DWORD iMin = NULL;
//	DOUBLE energy = NULL;
//	DOUBLE minEnergy = 10000;
//
//	INT iMin1 = -1;
//	INT iMin2 = -1;
//	INT iMin3 = -1;
//
//	FLOAT angleMin = 100000;
//	FLOAT delMin = 100000;
//	FLOAT dMin = 100000;
//	FLOAT dAverage = NULL;
//
//	DWORD nTrajectory = NULL;
//	TRAJECTORY t = {0};
//	Alloc(&t, 3);
//
//	for(iP3 = 0; iP3 < nP3; iP3++){
//
//		minEnergy =500000000000000;
//		iMin1 = -1;
//		iMin2 = -1;
//		iMin3 = -1;
//
//		angleMin = 100000;
//		delMin = 100000;
//		dMin = 100000;
//
//		memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));			//Load p3
//		for(iP2 = 0; iP2 < nP2; iP2++){
//			memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));		//Load p2
//			for(iP1 = 0; iP1 < nP1; iP1++){
//				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1
//
//				GetVector(p2, p1, &v1);
//				GetVector(p3, p2, &v2);
//				GetVector(p3, p1, &v3);
//				
//				//Calculate Angle
//				angle = GetAngle(v1, v2, VI_RADIAN);
//				aDegree = angle*57.2957795;
//				aDifference = abs(v1.dir - v2.dir);
//
//				aV1V2 = GetAngle(v1, v2, VI_RADIAN);
//				aV1V3 = GetAngle(v1, v3, VI_RADIAN);
//				aV2V3 = GetAngle(v2, v3, VI_RADIAN);
//
//				//Get Angle from Hroz for both vectors
//
//				//Calculate Distance
//				d1 = GetDistance(p1, p2);
//				d2 = GetDistance(p2, p3);
//				d = d1+d2;
//				dAverage = (FLOAT)((FLOAT)(d1+d2)/(FLOAT)2.0);
//				dMax = max(d1, d2);
//
//				//Calculate Delta-Distance
//				del = abs(max(d1, d2)-min(d1,d2));
//
//				if(d1 && d2){
//					delNorm = (FLOAT)del / (FLOAT)(min(d1, d2));
//					anglNorm = (FLOAT)angle / (FLOAT)(min(d1, d2));
//					dNorm = (FLOAT) d / (FLOAT) (min(d1, d2));
//				}else{
//					delNorm = (FLOAT)del / (FLOAT)1000;
//					anglNorm = (FLOAT)angle / (FLOAT)1000;
//					dNorm = (FLOAT) d / (FLOAT) 1000;
//				}
//
//				dd = (FLOAT)abs(d1-d2) / (FLOAT)dAverage;
//				dDif = dd;
//				dd = dd*100;
//
//				energy = sin(dDif)*v3.mag + sin(angle)*v3.mag + sin(aV1V3)*v3.mag + v3.mag;
//
//				if(energy < 0){
//					MessageBox(NULL, "Negative Energy", "Negative Energy", MB_OK);
//				}
//
//				if(v3.mag > v1.mag && v3.mag > v2.mag){
//					if(v3.mag < 200){
//						if(energy < minEnergy){
//							iMin1 = iP1;
//							iMin2 = iP2;
//							iMin3 = iP3;
//							minEnergy = energy;
//
//							angleMin = angle;
//							delMin = del;
//							dMin = d;
//						}
//					}
//				}
//
//			}
//
//		}
//
//		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){
//			Push(&t, fps1->point[iMin1]);
//			Push(&t, fps2->point[iMin2]);
//			Push(&t, fps3->point[iMin3]);
//			
//			
//			Push(ts, t);
//		}
//
//		Empty(&t);
//
//	}
//
//
//	return TRUE;
//}

DWORD MotionCorrespondance(FLOAT maxAngle, FLOAT maxDelta, FLOAT maxDistance, FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	DWORD iP1 = NULL;
	DWORD iP2 = NULL;
	DWORD iP3 = NULL;

	DWORD nP1 = fps1->nPoint;
	DWORD nP2 = fps2->nPoint;
	DWORD nP3 = fps3->nPoint;

	VECTOR v1 = {0};
	VECTOR v2 = {0};

	FPOINT p1 = {0};
	FPOINT p2 = {0};
	FPOINT p3 = {0};

	FLOAT angle = NULL;
	FLOAT anglNorm = NULL;

	FLOAT d = NULL;
	FLOAT dNorm = NULL;

	FLOAT dd = NULL;		//Represents the diference in distances

	FLOAT d1 = NULL;
	FLOAT d2 = NULL;
	FLOAT del = NULL;
	FLOAT delNorm = NULL;

	DWORD iMin = NULL;
	DOUBLE energy = NULL;
	DOUBLE minEnergy = 10000;

	INT iMin1 = -1;
	INT iMin2 = -1;
	INT iMin3 = -1;

	FLOAT angleMin = 100000;
	FLOAT delMin = 100000;
	FLOAT dMin = 100000;

	DWORD nTrajectory = NULL;
	TRAJECTORY t = {0};
	Alloc(&t, 3);

	for(iP3 = 0; iP3 < nP3; iP3++){

		minEnergy =5000000000000;
		iMin1 = -1;
		iMin2 = -1;
		iMin3 = -1;

		angleMin = 100000;
		delMin = 100000;
		dMin = 100000;

		memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));			//Load p3
		for(iP2 = 0; iP2 < nP2; iP2++){
			memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));		//Load p2
			for(iP1 = 0; iP1 < nP1; iP1++){
				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1

				GetVector(p2, p1, &v1);
				GetVector(p3, p2, &v2);
				
				//Calculate Angle
				angle = GetAngle(v1, v2, VI_DEGREE);

				//Calculate Distance
				d1 = GetDistance(p1, p2);
				d2 = GetDistance(p2, p3);
				d = d1+d2;

				//Calculate Delta-Distance
				del = abs(d1-d2);

				if(d1 && d2){
					delNorm = (FLOAT)del / (FLOAT)(min(d1, d2));
					anglNorm = (FLOAT)angle / (FLOAT)(min(d1, d2));
					dNorm = (FLOAT) d / (FLOAT) (min(d1, d2));
				}else{
					delNorm = (FLOAT)del / (FLOAT)1000;
					anglNorm = (FLOAT)angle / (FLOAT)1000;
					dNorm = (FLOAT) d / (FLOAT) 1000;
				}

				dd = (FLOAT)(d1+d2);
				dd = (FLOAT)(d1+d2)/(FLOAT)2;
				dd = (FLOAT)abs(d1-d2) / (FLOAT)(dd);
				dd = dd*100;
			
				energy = (angle)*d1*d1*d2*d2*(del);

				//Works very well, still matches small angles more over close proximity
				//energy = angle*d1*d2*del;

				//Works well, but still does not converge to small displacements
				//energy = angle + (delNorm*dNorm);		

				//Does not work all that well
				//energy = delNorm + anglNorm + dNorm;

				if(angle < maxAngle && dd < maxDelta && (d1+d2) < maxDistance){
					if(energy < minEnergy){
						iMin1 = iP1;
						iMin2 = iP2;
						iMin3 = iP3;
						minEnergy = energy;

						angleMin = angle;
						delMin = del;
						dMin = d;
					}
				}

			}

		}

		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){
			Push(&t, fps3->point[iMin3]);
			Push(&t, fps2->point[iMin2]);
			Push(&t, fps1->point[iMin1]);
			Push(ts, t);
		}

		Empty(&t);

	}


	return TRUE;
}

//DWORD MotionCorrespondance(FLOAT maxAngle, FLOAT maxDelta, FLOAT maxDistance, FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
//{
//
//	if(!ts){
//		return FALSE;
//	}
//
//	DWORD iP1 = NULL;
//	DWORD iP2 = NULL;
//	DWORD iP3 = NULL;
//
//	DWORD nP1 = fps1->nPoint;
//	DWORD nP2 = fps2->nPoint;
//	DWORD nP3 = fps3->nPoint;
//
//	VECTOR v1 = {0};
//	VECTOR v2 = {0};
//
//	FPOINT p1 = {0};
//	FPOINT p2 = {0};
//	FPOINT p3 = {0};
//
//	FLOAT angle = NULL;
//	FLOAT anglNorm = NULL;
//
//	FLOAT d = NULL;
//	FLOAT dNorm = NULL;
//
//	FLOAT dd = NULL;		//Represents the diference in distances
//
//	FLOAT d1 = NULL;
//	FLOAT d2 = NULL;
//	FLOAT del = NULL;
//	FLOAT delNorm = NULL;
//
//	DWORD iMin = NULL;
//	DOUBLE energy = NULL;
//	DOUBLE minEnergy = 10000;
//
//	INT iMin1 = -1;
//	INT iMin2 = -1;
//	INT iMin3 = -1;
//
//	FLOAT angleMin = 100000;
//	FLOAT delMin = 100000;
//	FLOAT dMin = 100000;
//
//	DWORD nTrajectory = NULL;
//	TRAJECTORY t = {0};
//	Alloc(&t, 3);
//
//	for(iP3 = 0; iP3 < nP3; iP3++){
//
//		minEnergy =5000000000000;
//		iMin1 = -1;
//		iMin2 = -1;
//		iMin3 = -1;
//
//		angleMin = 100000;
//		delMin = 100000;
//		dMin = 100000;
//
//		memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));			//Load p3
//		for(iP2 = 0; iP2 < nP2; iP2++){
//			memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));		//Load p2
//			for(iP1 = 0; iP1 < nP1; iP1++){
//				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1
//
//				GetVector(p2, p1, &v1);
//				GetVector(p3, p2, &v2);
//				
//				//Calculate Angle
//				angle = GetAngle(v1, v2, VI_DEGREE);
//
//				//Calculate Distance
//				d1 = GetDistance(p1, p2);
//				d2 = GetDistance(p2, p3);
//				d = d1+d2;
//
//				//Calculate Delta-Distance
//				del = abs(d1-d2);
//
//				if(d1 && d2){
//					delNorm = (FLOAT)del / (FLOAT)(min(d1, d2));
//					anglNorm = (FLOAT)angle / (FLOAT)(min(d1, d2));
//					dNorm = (FLOAT) d / (FLOAT) (min(d1, d2));
//				}else{
//					delNorm = (FLOAT)del / (FLOAT)1000;
//					anglNorm = (FLOAT)angle / (FLOAT)1000;
//					dNorm = (FLOAT) d / (FLOAT) 1000;
//				}
//
//				dd = (FLOAT)(d1+d2);
//				dd = (FLOAT)(d1+d2)/(FLOAT)2;
//				dd = (FLOAT)abs(d1-d2) / (FLOAT)(dd);
//				dd = dd*100;
//			
//				energy = (angle+1)*d1*d1*d2*d2*(del+1);
//
//				//Works very well, still matches small angles more over close proximity
//				//energy = angle*d1*d2*del;
//
//				//Works well, but still does not converge to small displacements
//				//energy = angle + (delNorm*dNorm);		
//
//				//Does not work all that well
//				//energy = delNorm + anglNorm + dNorm;
//
//				if(angle < maxAngle && dd < maxDelta && (d1+d2) < maxDistance){
//					if(energy < minEnergy){
//						iMin1 = iP1;
//						iMin2 = iP2;
//						iMin3 = iP3;
//						minEnergy = energy;
//
//						angleMin = angle;
//						delMin = del;
//						dMin = d;
//					}
//				}
//
//			}
//
//		}
//
//		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){
//			Push(&t, fps3->point[iMin3]);
//			Push(&t, fps2->point[iMin2]);
//			Push(&t, fps1->point[iMin1]);
//			Push(ts, t);
//		}
//
//		Empty(&t);
//
//	}
//
//
//	return TRUE;
//}



//Trajectory Functions
DWORD Alloc(TRAJECTORY *t, DWORD size)
{
	if(!t){
		return FALSE;
	}
	
	return Alloc(&t->ps, size);

}

DWORD Free(TRAJECTORY *t)
{
	if(!t){
		return FALSE;
	}

	return Free(&t->ps);

}

DWORD Push(TRAJECTORY *t, FPOINT point)
{
	if(!t){
		return FALSE;
	}

	return Push(&t->ps, point);
}

DWORD Copy(TRAJECTORY *tDest, TRAJECTORY *tSrc)
{
	if(!tDest || !tSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = tSrc->ps.nPoint;

	if(n > tDest->ps.size){
		n = tDest->ps.size;
	}

	memcpy(tDest->ps.point, tSrc->ps.point, n*sizeof(FPOINT));
	tDest->ps.nPoint = n;
	
	return TRUE;

}

DWORD Transfer(TRAJECTORY *tDest, TRAJECTORY *tSrc)
{
	if(!tDest || !tSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = tSrc->ps.nPoint;

	if(n > tDest->ps.size){
		n = tDest->ps.size;
	}

	memcpy(tDest->ps.point, tSrc->ps.point, n*sizeof(FPOINT));
	tDest->ps.nPoint = n;

	memset(tSrc->ps.point, NULL, tSrc->ps.size*sizeof(FPOINT));
	
	return TRUE;

}

DWORD Empty(TRAJECTORY *t)
{

	if(!t){
		return FALSE;
	}

	return Empty(&t->ps);
}

DWORD GetLength(TRAJECTORY t)
{
	return t.ps.size;
}

DWORD GetSize(TRAJECTORY t)
{
	return t.ps.nPoint;
}

DWORD GetAt(TRAJECTORY t, DWORD i, FPOINT *pt)
{
	if(!pt){
		return FALSE;
	}

	if(i >= t.ps.nPoint){
		return FALSE;
	}

	memcpy(pt, &t.ps.point[i], sizeof(FPOINT));
	
	return TRUE;
}

DWORD Zero(TRAJECTORY *t)
{
	if(!t){
		return FALSE;
	}

	//Zero(&t->ps);
	memset(t->ps.point, NULL, t->ps.nPoint*sizeof(FPOINT));

	return TRUE;
}

DWORD Reverse(TRAJECTORY *t)
{
	if(!t){
		return FALSE;
	}

	FPOINTSET fps = {0};
	Alloc(&fps, t->ps.nPoint);
	Transfer(&fps, &t->ps);

	DWORD i = NULL;
	for(i = 0; i < fps.nPoint; i++){

		if(fps.nPoint-i-1 >= 0){
			Push(&t->ps, fps.point[fps.nPoint-i-1]);
		}else{
			i = i;
		}
		//t->ps.point[i] = fps.point[fps.nPoint-i];
	}

	Free(&fps);
	return TRUE;
}



//TrajectorySet Functions
DWORD IsEmpty(TRAJECTORYSET ts)
{
	return !(ts.nTrajectory);
}

DWORD Alloc(TRAJECTORYSET *ts, DWORD size, DWORD lenTrajectory)
{
	if(!ts){
		return FALSE;
	}

	if(ts->trajectory){
		Free(ts);
	}
	
	ts->trajectory = (TRAJECTORY *)malloc(size*sizeof(TRAJECTORY));

	if(ts->trajectory){
		memset(ts->trajectory, NULL, size*sizeof(TRAJECTORY));
		ts->size = size;
		ts->nTrajectory = NULL;

		INT i = NULL;
		for(i = 0; i < size; i++){
			Alloc(&ts->trajectory[i], lenTrajectory);
		}
	}else{
		ts->trajectory = NULL;
		ts->size = NULL;
		ts->nTrajectory = NULL;
	}
	
	
	return TRUE;
}

DWORD Free(TRAJECTORYSET *ts)
{
	if(!ts){
		return FALSE;
	}

	if(!ts->trajectory){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < ts->size; i++){
	//for(i = 0; i < ts->nTrajectory; i++){
		Free(&ts->trajectory[i]);
	}

	free(ts->trajectory);
	ts->trajectory = NULL;
	ts->size = NULL;
	ts->nTrajectory = NULL;

	return TRUE;

}

DWORD Push(TRAJECTORYSET *ts, TRAJECTORY trajectory)
{
	if(!ts){
		return FALSE;
	}

	if(ts->nTrajectory < ts->size){
		Copy(&ts->trajectory[ts->nTrajectory], &trajectory);
		ts->nTrajectory++;
	}

	return TRUE;
}

//Depreciated by 'Clear()'
DWORD Empty(TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < ts->nTrajectory; i++){
		Empty(&ts->trajectory[i]);
	}
	ts->nTrajectory = NULL;

	return TRUE;
}

DWORD Clear(TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	INT i = NULL;

	for(i = 0; i < ts->nTrajectory; i++){
		Empty(&ts->trajectory[i]);
	}
	ts->nTrajectory = NULL;

	return TRUE;
}

DWORD Transfer(TRAJECTORYSET *tsDest, TRAJECTORYSET *tsSrc)
{
	if(!tsDest || !tsSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = tsSrc->nTrajectory;

	if(n > tsDest->nTrajectory){
		n = tsDest->nTrajectory;
	}

	for(i = 0; i < n; i++){
		Transfer(&tsDest->trajectory[i], &tsSrc->trajectory[i]);
	}
	tsDest->nTrajectory = n;

	Empty(tsSrc);
	
	return TRUE;

}

DWORD Copy(TRAJECTORYSET *tsDest, TRAJECTORYSET *tsSrc)
{
	if(!tsDest || !tsSrc){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = tsSrc->nTrajectory;

	if(n > tsDest->size){
		n = tsDest->size;
	}

	for(i = 0; i < n; i++){
		Copy(&tsDest->trajectory[i], &tsSrc->trajectory[i]);
	}
	tsDest->nTrajectory = n;
	
	return TRUE;

}

DWORD GetSize(TRAJECTORYSET ts)
{
	return ts.nTrajectory;
}

DWORD GetAt(TRAJECTORYSET ts, DWORD i, TRAJECTORY *t)
{
	if(!t){
		return FALSE;
	}

	if(i >= ts.nTrajectory){
		return FALSE;
	}

	Copy(t, &ts.trajectory[i]);
	
	return TRUE;
}

DWORD Zero(TRAJECTORYSET *ts, DWORD i)
{
	if(!ts){
		return FALSE;
	}

	if(i < ts->size){
		Zero(&ts->trajectory[i]);
	}


	return TRUE;
}

DWORD Remove(TRAJECTORYSET *ts, DWORD i)
{
	if(!ts){
		return FALSE;
	}

	//if(i < ts->size){
	if(i < ts->nTrajectory){
		Copy(&ts->trajectory[i], &ts->trajectory[ts->nTrajectory-1]);
		//Free(&ts->trajectory[i]);

		//memcpy(&ts->trajectory[i], &ts->trajectory[i+1], (ts->nTrajectory-i)*sizeof(TRAJECTORY));
		//Alloc(&ts->trajectory[ts->nTrajectory-1], ts->trajectory[i].ps.size);

		if(ts->nTrajectory){
			ts->nTrajectory--;
		}
		//if(ts->nTrajectory < 0){
		//	ts->nTrajectory = 0;
		//}
	}

	return TRUE;
}

//DWORD Remove(TRAJECTORYSET *ts, DWORD i)
//{
//	if(!ts){
//		return FALSE;
//	}
//
//	//if(i < ts->size){
//	if(i < ts->nTrajectory){
//		Free(&ts->trajectory[i]);
//		memcpy(&ts->trajectory[i], &ts->trajectory[i+1], (ts->nTrajectory-i)*sizeof(TRAJECTORY));
//		Alloc(&ts->trajectory[ts->nTrajectory-1], ts->trajectory[i].ps.size);
//		ts->nTrajectory--;
//		if(ts->nTrajectory < 0){
//			ts->nTrajectory = 0;
//		}
//	}
//
//	return TRUE;
//}


//Correspondence Functions
DWORD Alloc(CORRSET *cs, DWORD size)
{
	if(!cs){
		return FALSE;
	}
	
	cs->corr = (CORR *)malloc(size*sizeof(CORR));

	if(cs->corr){
		cs->size = size;
		cs->nCorr = NULL;
	}else{
		cs->corr = NULL;
		cs->size = NULL;
		cs->nCorr = NULL;
	}


	return TRUE;
}

DWORD Free(CORRSET *cs)
{

	if(!cs){
		return FALSE;
	}

	if(cs->corr){
		free(cs->corr);
		cs->size = NULL;
		cs->nCorr = NULL;
	}

	return TRUE;
}

DWORD Push(CORRSET *cs, CORR corr)
{

	if(!cs){
		return FALSE;
	}

	if(cs->nCorr < cs->size){
		memcpy(&cs->corr[cs->nCorr], &corr, sizeof(CORR));
		cs->nCorr++;
	}else{
		return FALSE;
	}

	return TRUE;
}



DWORD Fetch(CORRSET *cs, DWORD i, CORR *cFetch)
{
	if(!cs || !cFetch){
		return FALSE;
	}

	if(i < cs->nCorr){
		memcpy(cFetch, &cs->corr[i], sizeof(CORR));
		return TRUE;
	}else{
		return FALSE;
	}

}









////Feature Based Correspondence
////Uses Three Point Sets to determine 
//DWORD MotionCorrespondance(FPOINTSET fps1, FPOINTSET fps2, FPOINTSET fps3, CORRSET *cs)
//{
//
//	if(!cs){
//		return FALSE;
//	}
//
//	DWORD iP1 = NULL;
//	DWORD iP2 = NULL;
//	DWORD iP3 = NULL;
//
//	DWORD nP1 = fps1.nPoint;
//	DWORD nP2 = fps2.nPoint;
//	DWORD nP3 = fps3.nPoint;
//
//	VECTOR v1 = {0};
//	VECTOR v2 = {0};
//
//	for(iP3 = 0; iP3 < nP3; iP3++){
//		for(iP2 = 0; iP2 < nP2; iP2++){
//			for(iP1 = 0; iP1 < nP1; iP1++){
//
//				
//
//			}
//
//		}
//
//	}
//
//
//	return TRUE;
//}


//Archived
//DWORD MotionCorrespondance(FPOINTSET *fps1, FPOINTSET *fps2, FPOINTSET *fps3, TRAJECTORYSET *ts)
//{
//
//	if(!ts){
//		return FALSE;
//	}
//
//	DWORD iP1 = NULL;
//	DWORD iP2 = NULL;
//	DWORD iP3 = NULL;
//
//	DWORD nP1 = fps1->nPoint;
//	DWORD nP2 = fps2->nPoint;
//	DWORD nP3 = fps3->nPoint;
//
//	VECTOR v1 = {0};
//	VECTOR v2 = {0};
//	VECTOR v3 = {0};
//
//	FPOINT p1 = {0};
//	FPOINT p2 = {0};
//	FPOINT p3 = {0};
//
//	FLOAT angle = NULL;
//	FLOAT anglNorm = NULL;
//	FLOAT aDegree = NULL;
//	FLOAT aRadian = NULL;
//	FLOAT aDifference = NULL;
//
//	FLOAT d = NULL;
//	FLOAT dNorm = NULL;
//
//	FLOAT dd = NULL;		//Represents the diference in distances
//	FLOAT dDif = NULL;
//	FLOAT dMax = NULL;
//	//FLOAT dMin = NULL;
//
//	FLOAT d1 = NULL;
//	FLOAT d2 = NULL;
//	FLOAT del = NULL;
//	FLOAT delNorm = NULL;
//
//	DWORD iMin = NULL;
//	DOUBLE energy = NULL;
//	DOUBLE minEnergy = 10000;
//
//	INT iMin1 = -1;
//	INT iMin2 = -1;
//	INT iMin3 = -1;
//
//	FLOAT angleMin = 100000;
//	FLOAT delMin = 100000;
//	FLOAT dMin = 100000;
//	FLOAT dAverage = NULL;
//
//	DWORD nTrajectory = NULL;
//	TRAJECTORY t = {0};
//	Alloc(&t, 3);
//
//	for(iP3 = 0; iP3 < nP3; iP3++){
//
//		minEnergy =500000000000000;
//		iMin1 = -1;
//		iMin2 = -1;
//		iMin3 = -1;
//
//		angleMin = 100000;
//		delMin = 100000;
//		dMin = 100000;
//
//		memcpy(&p3, &fps3->point[iP3], sizeof(FPOINT));			//Load p3
//		for(iP2 = 0; iP2 < nP2; iP2++){
//			memcpy(&p2, &fps2->point[iP2], sizeof(FPOINT));		//Load p2
//			for(iP1 = 0; iP1 < nP1; iP1++){
//				memcpy(&p1, &fps1->point[iP1], sizeof(FPOINT));	//Load p1
//
//				GetVector(p2, p1, &v1);
//				GetVector(p3, p2, &v2);
//				GetVector(p3, p1, &v3);
//				
//				//Calculate Angle
//				angle = GetAngle(v1, v2, VI_RADIAN);
//				aDegree = angle*57.2957795;
//				aDifference = abs(v1.dir - v2.dir);
//
//				//Get Angle from Hroz for both vectors
//
//				//Calculate Distance
//				d1 = GetDistance(p1, p2);
//				d2 = GetDistance(p2, p3);
//				d = d1+d2;
//				dAverage = (FLOAT)((FLOAT)(d1+d2)/(FLOAT)2.0);
//				dMax = max(d1, d2);
//
//				//Calculate Delta-Distance
//				del = abs(max(d1, d2)-min(d1,d2));
//
//				if(d1 && d2){
//					delNorm = (FLOAT)del / (FLOAT)(min(d1, d2));
//					anglNorm = (FLOAT)angle / (FLOAT)(min(d1, d2));
//					dNorm = (FLOAT) d / (FLOAT) (min(d1, d2));
//				}else{
//					delNorm = (FLOAT)del / (FLOAT)1000;
//					anglNorm = (FLOAT)angle / (FLOAT)1000;
//					dNorm = (FLOAT) d / (FLOAT) 1000;
//				}
//
//				dd = (FLOAT)abs(d1-d2) / (FLOAT)dAverage;
//				dDif = dd;
//				dd = dd*100;
//
//				energy = sin(dDif)*v3.mag + sin(angle)*v3.mag + v3.mag;
//
//				//energy = dAverage*dAverage + dDif*dAverage + ((FLOAT)aDegree / (FLOAT)360)*dAverage;
//
//				//Seems to work well for most cases, but has a few random miscoorespondances
//				//energy = dAverage*dAverage + dDif*dAverage + ((FLOAT)aDegree/(FLOAT)360)*dAverage;
//
//				//Permits large angles such as 150 degrees to be minimums
//				//energy = dAverage*dAverage + sin(dDif*.5*3.14159)*dAverage + sin(((FLOAT)aDegree/(FLOAT)360)*0.5*3.14159)*dAverage; 
//			
//				//energy = dAverage + del*dAverage + sin(angle)*dAverage;
//
//				//Best
//				//energy = dAverage + del*dAverage*dAverage + sin(angle)*dAverage*dAverage;
//
//				//energy = ((FLOAT)dAverage / (FLOAT)cos(angle)) + dAverage + dd;
//
//				//Difficult to
//				//energy = (angle+1)*d1*d1*d2*d2*(del+1);
//
//				//Works very well, still matches small angles more over close proximity
//				//energy = angle*d1*d2*del;
//
//				//Works well, but still does not converge to small displacements
//				//energy = angle + (delNorm*dNorm);		
//
//				//Does not work all that well
//				//energy = delNorm + anglNorm + dNorm;
//
//				if(energy < 0){
//					MessageBox(NULL, "Negative Energy", "Negative Energy", MB_OK);
//				}
//
//				if(v3.mag > v1.mag && v3.mag > v2.mag){
//					if(v3.mag < 200){
//				//if(aDegree < 35 && dd < 100 && dAverage < 200){
//						if(energy < minEnergy){
//							iMin1 = iP1;
//							iMin2 = iP2;
//							iMin3 = iP3;
//							minEnergy = energy;
//
//							angleMin = angle;
//							delMin = del;
//							dMin = d;
//						}
//					}
//				}
//
//			}
//
//		}
//
//		if(iMin1 >= 0 && iMin2 >= 0 && iMin3 >= 0){
//			Push(&t, fps1->point[iMin1]);
//			Push(&t, fps2->point[iMin2]);
//			Push(&t, fps3->point[iMin3]);
//			
//			
//			Push(ts, t);
//		}
//
//		Empty(&t);
//
//	}
//
//
//	return TRUE;
//}