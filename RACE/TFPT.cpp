#include "stdafx.h"
#include "TFPT.h"

//Search AGAINST all pF0 Points First
//Search AGAINST all pF1 Points Second
//Search AGAINST all pF2 Points Last



//***********************OUTPUT ENABLED**********************************************
//Returns the number of groups found

UINT GetF0ParticleGroups(UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF0)
{

	INT group[MAX_TRACK];
	UINT cntGroup = 0;

	FLOAT angle;
	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR vector;
	VECTOR v1;
	VECTOR v2;

	CHAR newLine[] = {13, 10, 0};
	CHAR output[256];

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;
	UINT gPF0, gPF1, gPF2;

	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;
	
	FLOAT threshold;
	FLOAT temp;
	FLOAT simF0 = 0;
	FLOAT simF1 = 0;

	FLOAT *minAngle = (FLOAT *)calloc(MAX_TRACK, sizeof(FLOAT));
	FLOAT *minDist = (FLOAT *)calloc(MAX_TRACK, sizeof(FLOAT));

	DOUBLE error = 0;
	DOUBLE errMin = 0;

	GetF0ParticleMinimums(pF0, pF1, pF2, minDist, minAngle);

	memset(group, -1, MAX_TRACK*sizeof(INT));

	for(iF0 = 0; iF0 < nPF0; iF0++){
		errMin = 1000;
		if(group[cntGroup] >= 0){
			cntGroup++;
		}
		for(iF1 = 0; iF1 < nPF1; iF1++){
			for(iF2 = 0; iF2 < nPF2; iF2++){

				
				DOUBLE distance[2];
				DOUBLE anglErrorNorm;
				DOUBLE distErrorNorm;
				
				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = (DOUBLE) GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);

				if(minAngle[iF0] == 0.0)
					minAngle[iF0] = 0.001;
				if(minDist[iF0] == 0.00)
					minDist[iF0] = 0.001;
				
				anglErrorNorm = ((FLOAT)angle / (FLOAT)minAngle[iF0]);
				distErrorNorm = ((FLOAT)distDiff / (FLOAT)minDist[iF0]);

				error = WT_ANGLE*anglErrorNorm + WT_DIST*distErrorNorm;

				if(error < errMin && angle < MAX_ANGLE && distance[0] < MAXDIST_TFPT && distance[1] < MAXDIST_TFPT ){
					errMin = error;
					
					if(group[cntGroup] != iF0){
						group[cntGroup] = iF0;
						
					}

					pgF0[cntGroup].frmNum = frame;

					pgF0[cntGroup].particleF0.frmID = iF0;
					pgF0[cntGroup].particleF0.frmNum = frame - 0;
					memcpy(&pgF0[cntGroup].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
					

					pgF0[cntGroup].particleF1.frmID = iF1;
					pgF0[cntGroup].particleF1.frmNum = frame - 1;
					memcpy(&pgF0[cntGroup].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));

					pgF0[cntGroup].particleF2.frmID = iF2;
					pgF0[cntGroup].particleF2.frmNum = frame - 2;
					memcpy(&pgF0[cntGroup].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));

				}
			}
		}


	}

	if(minAngle)free(minAngle);
	if(minDist)free(minDist);

	//Add one to zero based counter
	cntGroup++;
	return cntGroup;	
}

//Returns the number of groups found
UINT GetF2ParticleGroups(UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF2)
{
	INT group[MAX_TRACK];
	UINT cntGroup = 0;

	FLOAT angle;
	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR vector;
	VECTOR v1;
	VECTOR v2;

	CHAR newLine[] = {13, 10, 0};
	CHAR output[256];

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;
	UINT gPF0, gPF1, gPF2;

	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;
	
	FLOAT threshold;
	FLOAT temp;
	FLOAT simF0 = 0;
	FLOAT simF1 = 0;

	FLOAT *minAngle = (FLOAT *)malloc(100*sizeof(FLOAT));
	FLOAT *minDist = (FLOAT *)malloc(100*sizeof(FLOAT));

	DOUBLE errMin = 10000;


	GetF2ParticleMinimums(pF0, pF1, pF2, minDist, minAngle);

	memset(group, -1, MAX_TRACK*sizeof(INT));

	for(iF2 = 0; iF2 < nPF2; iF2++){
		errMin = 1000;
		if(group[cntGroup] >= 0){
			cntGroup++;
		}
		
			for(iF1 = 0; iF1 < nPF1; iF1++){
				for(iF0 = 0; iF0 < nPF0; iF0++){
				
				DOUBLE distance[2];
				DOUBLE anglErrorNorm;
				DOUBLE distErrorNorm;
				DOUBLE error;

				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);

				if(minAngle[iF2] == 0.0)
					minAngle[iF2] = 0.001;
				if(minDist[iF2] == 0.00) 
					minDist[iF2] = 0.001;
				
				anglErrorNorm = ((FLOAT)angle / (FLOAT)minAngle[iF2]);
				distErrorNorm = ((FLOAT)distDiff / (FLOAT)minDist[iF2]);

				error = WT_ANGLE*anglErrorNorm + WT_DIST*distErrorNorm;

				if(error < errMin && angle < MAX_ANGLE && distance[0] < MAXDIST_TFPT && distance[1] < MAXDIST_TFPT /*distDiff < MAXDIST_TFPTDIFF*/){
					errMin = error;
					
					if(group[cntGroup] != iF2){
						group[cntGroup] = iF2;
						//cntGroup++;
					}

					pgF2[cntGroup].frmNum = frame;

					pgF2[cntGroup].particleF0.frmID = iF0;
					pgF2[cntGroup].particleF0.frmNum = frame - 0;
					memcpy(&pgF2[cntGroup].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
					

					pgF2[cntGroup].particleF1.frmID = iF1;
					pgF2[cntGroup].particleF1.frmNum = frame - 1;
					memcpy(&pgF2[cntGroup].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));

					pgF2[cntGroup].particleF2.frmID = iF2;
					pgF2[cntGroup].particleF2.frmNum = frame - 2;
					memcpy(&pgF2[cntGroup].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));

				}

			}
		}
	}
	
	if(minAngle)free(minAngle);
	if(minDist)free(minDist);

	//Add one, it was zero-based counted
	cntGroup++;
	return cntGroup;
}



VOID GetF0ParticleMinimums(FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle)
{

	INT iPrevDist = -1;
	INT iPrevAngl = -1;
	UINT nMinDiff = 0;
	UINT nMinAngl = 0;
	
	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR v1;
	VECTOR v2;

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;

	FLOAT angle = 0;
	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;

	FLOAT minDiffAngle;
	FLOAT minDiffDist;


	//Test
	//memset(minP0Dist, -1, nPF0*sizeof(FLOAT));
	//memset(minP0Angle, -1, nPF0*sizeof(FLOAT));

	for(iF0 = 0; iF0 < nPF0; iF0++){
		minDiffAngle = 380;
		minDiffDist = 1000;
		iPrevDist = -1;
		iPrevAngl = -1;
		
			for(iF2 = 0; iF2 < nPF2; iF2++){
				for(iF1 = 0; iF1 < nPF1; iF1++){

				FLOAT distance[2];
				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);

				if(distDiff < minDiffDist){
					if(iPrevDist != iF0){
						iPrevDist = iF0;
						nMinDiff++;
					}
					minP0Dist[iF0] = distDiff;
					minDiffDist = distDiff;
				
				}

				if(angle < minDiffAngle){
					if(iPrevAngl != iF0){
						iPrevAngl = iF0;
						nMinAngl++;
					}
					minP0Angle[iF0] = angle;
					minDiffAngle = angle;
					
				}

			
			}
		}
	}

	nMinAngl=nMinAngl;
	nMinAngl=nMinAngl;

}

VOID GetF2ParticleMinimums(FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle)
{

	INT iPrevDist = -1;
	INT iPrevAngl = -1;
	UINT nMinDiff = 0;
	UINT nMinAngl = 0;

	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR v1;
	VECTOR v2;

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;

	FLOAT angle = 0;
	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;

	FLOAT minDiffAngle;
	FLOAT minDiffDist;

	for(iF2 = 0; iF2 < nPF2; iF2++){
		minDiffAngle = 380;
		minDiffDist = 1000;
		iPrevDist = -1;
		iPrevAngl = -1;

		for(iF1 = 0; iF1 < nPF1; iF1++){
			for(iF0 = 0; iF0 < nPF0; iF0++){

				FLOAT distance[2];
				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);


				if(distDiff < minDiffDist){
					if(iPrevDist != iF2){
						iPrevDist = iF2;
						nMinDiff++;
					}
					minP0Dist[iF2] = distDiff;
					minDiffDist = distDiff;
				
				}

				if(angle < minDiffAngle){
					if(iPrevAngl != iF2){
						iPrevAngl = iF2;
						nMinAngl++;
					}
					minP0Angle[iF2] = angle;
					minDiffAngle = angle;
					
				}

				//if(distDiff < minDiffDist){
				//	minP0Dist[iF0] = distDiff;
				//	minDiffDist = distDiff;
				//}

				//if(angle < minDiffAngle){
				//	minP0Angle[iF2] = angle;
				//	minDiffAngle = angle;
				//}
			
			}
		}
	}
}

////***********************OUTPUT ENABLED**********************************************/
//
//FLOAT GetParticleDistanceDifference(FPOINT pF0, FPOINT pF1, FPOINT pF2)
//{
//	FLOAT dF0F1 = GetDistance(pF0, pF1);
//	FLOAT dF2F1 = GetDistance(pF2, pF1);
//
//	return abs(dF0F1 - dF2F1);
//}


////Three Frame Particle Track
//UINT TFPT(UINT frame, FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector)
//{
//
//	/***************************B-INITIALIZE******************************/
//	UINT i;
//
//	UINT nPF0;
//	UINT nPF2;
//
//	UINT nV1;
//	UINT nV2;
//	UINT nVOut;
//	
//	FPOINT ptHead[MAX_TRACK];
//	FPOINT ptTail[MAX_TRACK];
//
//	VECTOR v2[MAX_TRACK];	
//	VECTOR vF0[MAX_TRACK];
//	VECTOR vF2[MAX_TRACK];
//
//	PARTICLEGROUP pgF0[MAX_TRACK];
//	PARTICLEGROUP pgF2[MAX_TRACK];
//
//	FPOINT *ptF0 = (FPOINT *)calloc(GetNumberPoint(F0), sizeof(FPOINT));
//	FPOINT *ptF1 = (FPOINT *)calloc(GetNumberPoint(F1), sizeof(FPOINT));
//	FPOINT *ptF2 = (FPOINT *)calloc(GetNumberPoint(F2), sizeof(FPOINT));
//	
//	memset(v2, NULL, sizeof(v2));
//	memset(vF0, NULL, sizeof(vF0));
//	memset(vF2, NULL, sizeof(vF2));
//	/****************************E-INITIALIZE*****************************/
//
//	/*******************************B-BODY********************************/
//	GetPointFromPointSet(F0, ptF0);
//	GetPointFromPointSet(F1, ptF1);
//	GetPointFromPointSet(F2, ptF2);
//
//	nPF0 = GetF0ParticleGroups(NULL, F0, F1, F2, pgF0);
//	nPF2 = GetF2ParticleGroups(NULL, F0, F1, F2, pgF2);
//
//	//Only Interested in the Vectors associated with F1
//	//So Only Interested in V2
//	//V2-Tail = pF1
//	//V2-Head = pF0
//	for(i = 0; i < nPF0; i++){
//		memcpy(&ptTail[i], &pgF0[i].particleF1.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF0[i].particleF0.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF0, ptHead, ptTail, vF0);
//
//	for(i = 0; i < nPF2; i++){
//		memcpy(&ptTail[i], &pgF2[i].particleF2.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF2[i].particleF1.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF2, ptHead, ptTail, vF2);
//
//
//
//	nV1 = nPF0;
//	nV2 = nPF2;
//	nVOut = 0;
//
//	for(i = 0; i < nV1; i++){
//		vF0[i].j = vF0[i].j;
//		vF0[i].j = vF0[i].j;
//		vF2[i].j = vF2[i].j;
//	}
//
//
//	memcpy(vector, vF0, nV1*sizeof(VECTOR));
//	memcpy((vector + nV1), vF2, nV2*sizeof(VECTOR));
//	nVOut = nV1+nV2;
//
//	//nVOut = CombineUniqueDataVECTOR(nV1, nV2, (VECTOR *) vF0, (VECTOR *) vF2, (VECTOR *) vector);
//
//	/*******************************E-BODY********************************/
//
//	/*******************************OUTPUT********************************/
//
//	return nVOut;
//
//}

//Three Frame Particle Track
UINT TFPT(UINT frame, FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector)
{

	/***************************B-INITIALIZE******************************/
	UINT i;

	UINT nPF0;
	UINT nPF2;

	UINT nV1;
	UINT nV2;
	UINT nVOut;
	
	FPOINT ptHead[MAX_TRACK];
	FPOINT ptTail[MAX_TRACK];

	VECTOR v2[MAX_TRACK];	
	VECTOR vF0[MAX_TRACK];
	VECTOR vF2[MAX_TRACK];

	PARTICLEGROUP pgF0[MAX_TRACK];
	PARTICLEGROUP pgF2[MAX_TRACK];

	FPOINT *ptF0 = (FPOINT *)calloc(GetNumberPoint(F0), sizeof(FPOINT));
	FPOINT *ptF1 = (FPOINT *)calloc(GetNumberPoint(F1), sizeof(FPOINT));
	FPOINT *ptF2 = (FPOINT *)calloc(GetNumberPoint(F2), sizeof(FPOINT));
	
	memset(v2, NULL, sizeof(v2));
	memset(vF0, NULL, sizeof(vF0));
	memset(vF2, NULL, sizeof(vF2));
	/****************************E-INITIALIZE*****************************/

	/*******************************B-BODY********************************/
	GetPointFromPointSet(F0, ptF0);
	GetPointFromPointSet(F1, ptF1);
	GetPointFromPointSet(F2, ptF2);

	nPF0 = GetF0ParticleGroups(frame, F0, F1, F2, pgF0);
	nPF2 = GetF2ParticleGroups(frame, F0, F1, F2, pgF2);

	//Only Interested in the Vectors associated with F1
	//So Only Interested in V2
	//V2-Tail = pF1
	//V2-Head = pF0
	for(i = 0; i < nPF0; i++){
		memcpy(&ptTail[i], &pgF0[i].particleF1.point, sizeof(FPOINT));
		memcpy(&ptHead[i], &pgF0[i].particleF0.point, sizeof(FPOINT));
	}

	GetVector(nPF0, ptHead, ptTail, vF0);

	for(i = 0; i < nPF2; i++){
		memcpy(&ptTail[i], &pgF2[i].particleF2.point, sizeof(FPOINT));
		memcpy(&ptHead[i], &pgF2[i].particleF1.point, sizeof(FPOINT));
	}

	GetVector(nPF2, ptHead, ptTail, vF2);



	nV1 = nPF0;
	nV2 = nPF2;
	nVOut = 0;

	for(i = 0; i < nV1; i++){
		vF0[i].j = vF0[i].j;
		vF0[i].j = vF0[i].j;
		vF2[i].j = vF2[i].j;
	}



	memcpy(vector, vF0, nV1*sizeof(VECTOR));
	memcpy((vector + nV1), vF2, nV2*sizeof(VECTOR));
	nVOut = nV1+nV2;

	if(ptF0)free(ptF0);
	if(ptF1)free(ptF1);
	if(ptF2)free(ptF2);
	

	//nVOut = CombineUniqueDataVECTOR(nV1, nV2, (VECTOR *) vF0, (VECTOR *) vF2, (VECTOR *) vector);

	return nVOut;

}







//***********************OUTPUT ENABLED**********************************************
//Returns the number of groups found
UINT GetF0ParticleGroups(HWND hOutput, UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF0)
{

	INT group[MAX_TRACK];
	UINT cntGroup = 0;

	FLOAT angle;
	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR vector;
	VECTOR v1;
	VECTOR v2;

	CHAR newLine[] = {13, 10, 0};
	CHAR output[256];

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;
	UINT gPF0, gPF1, gPF2;

	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;
	
	FLOAT threshold;
	FLOAT temp;
	FLOAT simF0 = 0;
	FLOAT simF1 = 0;

	FLOAT *minAngle = (FLOAT *)calloc(MAX_TRACK, sizeof(FLOAT));
	FLOAT *minDist = (FLOAT *)calloc(MAX_TRACK, sizeof(FLOAT));

	DOUBLE error = 0;
	DOUBLE errMin = 0;

	//sprintf(output, "%s*****P0*****%s", newLine, newLine);
	//Edit_AppendText(hOutput, output);

	//GetF0ParticleMinimums(hOutput, pF0, pF1, pF2, minDist, minAngle);

	//sprintf(output, "%sMinimum Distance%s", newLine, newLine);
	//Edit_AppendText(hOutput, output);
	//Edit_OutputFLOAT(hOutput, pF0.nPoint, minDist);

	//sprintf(output, "%sMinimum Angle%s", newLine, newLine);
	//Edit_AppendText(hOutput, output);
	//Edit_OutputFLOAT(hOutput, pF0.nPoint, minAngle);

	memset(group, -1, MAX_TRACK*sizeof(INT));

	for(iF0 = 0; iF0 < nPF0; iF0++){
		errMin = 1000;
		if(group[cntGroup] >= 0){
			cntGroup++;
		}
		for(iF1 = 0; iF1 < nPF1; iF1++){
			for(iF2 = 0; iF2 < nPF2; iF2++){

				
				DOUBLE distance[2];
				DOUBLE anglErrorNorm;
				DOUBLE distErrorNorm;
				
				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);

				if(minAngle[iF0] == 0.0)
					minAngle[iF0] = 0.001;
				if(minDist[iF0] == 0.00)
					minDist[iF0] = 0.001;
				
				anglErrorNorm = ((FLOAT)angle / (FLOAT)minAngle[iF0]);
				distErrorNorm = ((FLOAT)distDiff / (FLOAT)minDist[iF0]);

				error = WT_ANGLE*anglErrorNorm + WT_DIST*distErrorNorm;

				if(error < errMin && angle < MAX_ANGLE && distance[0] < MAXDIST_TFPT && distance[1] < MAXDIST_TFPT /*distDiff < MAXDIST_TFPTDIFF*/){
					errMin = error;
					
					if(group[cntGroup] != iF0){
						group[cntGroup] = iF0;
						
					}

					pgF0[cntGroup].frmNum = frame;

					pgF0[cntGroup].particleF0.frmID = iF0;
					pgF0[cntGroup].particleF0.frmNum = frame - 0;
					memcpy(&pgF0[cntGroup].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
					

					pgF0[cntGroup].particleF1.frmID = iF1;
					pgF0[cntGroup].particleF1.frmNum = frame - 1;
					memcpy(&pgF0[cntGroup].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));

					pgF0[cntGroup].particleF2.frmID = iF2;
					pgF0[cntGroup].particleF2.frmNum = frame - 2;
					memcpy(&pgF0[cntGroup].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));


					//pgF0[iF0].frmNum = frame;

					//pgF0[iF0].particleF0.frmID = iF0;
					//pgF0[iF0].particleF0.frmNum = frame - 0;
					//memcpy(&pgF0[iF0].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
					//

					//pgF0[iF0].particleF1.frmID = iF1;
					//pgF0[iF0].particleF1.frmNum = frame - 1;
					//memcpy(&pgF0[iF0].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));

					//pgF0[iF0].particleF2.frmID = iF2;
					//pgF0[iF0].particleF2.frmNum = frame - 2;
					//memcpy(&pgF0[iF0].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));
				}
			}
		}


	}

	//Add one, it was zero-based counted
	cntGroup++;
	return cntGroup;	
}

//Returns the number of groups found
UINT GetF2ParticleGroups(HWND hOutput, UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF2/*PARTICLEGROUP *pGroupDist, PARTICLEGROUP *pGroupAngle*/)
{
	INT group[MAX_TRACK];
	UINT cntGroup = 0;

	FLOAT angle;
	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR vector;
	VECTOR v1;
	VECTOR v2;

	CHAR newLine[] = {13, 10, 0};
	CHAR output[256];

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;
	UINT gPF0, gPF1, gPF2;

	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;
	
	FLOAT threshold;
	FLOAT temp;
	FLOAT simF0 = 0;
	FLOAT simF1 = 0;

	FLOAT *minAngle = (FLOAT *)malloc(100*sizeof(FLOAT));
	FLOAT *minDist = (FLOAT *)malloc(100*sizeof(FLOAT));

	DOUBLE errMin = 10000;

	//sprintf(output, "%s*****P2*****%s", newLine, newLine);
	//Edit_AppendText(hOutput, output);

	//GetF2ParticleMinimums(hOutput, pF0, pF1, pF2, minDist, minAngle);

	//sprintf(output, "%sMinimum Distance%s", newLine, newLine);
	//Edit_AppendText(hOutput, output);
	//Edit_OutputFLOAT(hOutput, pF2.nPoint, minDist);

	//sprintf(output, "%sMinimum Angle%s", newLine, newLine);
	//Edit_AppendText(hOutput, output);
	//Edit_OutputFLOAT(hOutput, pF2.nPoint, minAngle);

	memset(group, -1, MAX_TRACK*sizeof(INT));

	for(iF2 = 0; iF2 < nPF2; iF2++){
		errMin = 1000;
		if(group[cntGroup] >= 0){
			cntGroup++;
		}
		
			for(iF1 = 0; iF1 < nPF1; iF1++){
				for(iF0 = 0; iF0 < nPF0; iF0++){
				
				DOUBLE distance[2];
				DOUBLE anglErrorNorm;
				DOUBLE distErrorNorm;
				DOUBLE error;

				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);

				if(minAngle[iF2] == 0.0)
					minAngle[iF2] = 0.001;
				if(minDist[iF2] == 0.00) 
					minDist[iF2] = 0.001;
				
				anglErrorNorm = ((FLOAT)angle / (FLOAT)minAngle[iF2]);
				distErrorNorm = ((FLOAT)distDiff / (FLOAT)minDist[iF2]);

				error = WT_ANGLE*anglErrorNorm + WT_DIST*distErrorNorm;

				if(error < errMin && angle < MAX_ANGLE && distance[0] < MAXDIST_TFPT && distance[1] < MAXDIST_TFPT /*distDiff < MAXDIST_TFPTDIFF*/){
					errMin = error;
					
					if(group[cntGroup] != iF2){
						group[cntGroup] = iF2;
						//cntGroup++;
					}

					pgF2[cntGroup].frmNum = frame;

					pgF2[cntGroup].particleF0.frmID = iF0;
					pgF2[cntGroup].particleF0.frmNum = frame - 0;
					memcpy(&pgF2[cntGroup].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
					

					pgF2[cntGroup].particleF1.frmID = iF1;
					pgF2[cntGroup].particleF1.frmNum = frame - 1;
					memcpy(&pgF2[cntGroup].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));

					pgF2[cntGroup].particleF2.frmID = iF2;
					pgF2[cntGroup].particleF2.frmNum = frame - 2;
					memcpy(&pgF2[cntGroup].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));



					//pgF2[iF2].frmNum = frame;

					//pgF2[iF2].particleF0.frmID = iF0;
					//pgF2[iF2].particleF0.frmNum = frame - 0;
					//memcpy(&pgF2[iF2].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
					//

					//pgF2[iF2].particleF1.frmID = iF1;
					//pgF2[iF2].particleF1.frmNum = frame - 1;
					//memcpy(&pgF2[iF2].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));

					//pgF2[iF2].particleF2.frmID = iF2;
					//pgF2[iF2].particleF2.frmNum = frame - 2;
					//memcpy(&pgF2[iF2].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));
				}

			}
		}
	}
	
	//Add one, it was zero-based counted
	cntGroup++;
	return cntGroup;
}



VOID GetF0ParticleMinimums(HWND hOutput, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle)
{

	INT iPrevDist = -1;
	INT iPrevAngl = -1;
	UINT nMinDiff = 0;
	UINT nMinAngl = 0;
	
	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR v1;
	VECTOR v2;

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;

	FLOAT angle = 0;
	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;

	FLOAT minDiffAngle;
	FLOAT minDiffDist;


	//Test
	//memset(minP0Dist, -1, nPF0*sizeof(FLOAT));
	//memset(minP0Angle, -1, nPF0*sizeof(FLOAT));

	for(iF0 = 0; iF0 < nPF0; iF0++){
		minDiffAngle = 380;
		minDiffDist = 1000;
		iPrevDist = -1;
		iPrevAngl = -1;
		
			for(iF2 = 0; iF2 < nPF2; iF2++){
				for(iF1 = 0; iF1 < nPF1; iF1++){

				FLOAT distance[2];
				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);

				if(distDiff < minDiffDist){
					if(iPrevDist != iF0){
						iPrevDist = iF0;
						nMinDiff++;
					}
					minP0Dist[iF0] = distDiff;
					minDiffDist = distDiff;
				
				}

				if(angle < minDiffAngle){
					if(iPrevAngl != iF0){
						iPrevAngl = iF0;
						nMinAngl++;
					}
					minP0Angle[iF0] = angle;
					minDiffAngle = angle;
					
				}

			
			}
		}
	}

	nMinAngl=nMinAngl;
	nMinAngl=nMinAngl;

}

VOID GetF2ParticleMinimums(HWND hOutput, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle)
{

	INT iPrevDist = -1;
	INT iPrevAngl = -1;
	UINT nMinDiff = 0;
	UINT nMinAngl = 0;

	FPOINT ptTail;
	FPOINT ptHead;
	VECTOR v1;
	VECTOR v2;

	UINT i;
	UINT iF0;
	UINT iF1;
	UINT iF2;
	UINT nPF0 = pF0.nPoint;
	UINT nPF1 = pF1.nPoint;
	UINT nPF2 = pF2.nPoint;

	FLOAT angle = 0;
	FLOAT distDiff = 0;
	FLOAT dF0F1 = 0;
	FLOAT dF2F1 = 0;

	FLOAT minDiffAngle;
	FLOAT minDiffDist;

	for(iF2 = 0; iF2 < nPF2; iF2++){
		minDiffAngle = 380;
		minDiffDist = 1000;
		iPrevDist = -1;
		iPrevAngl = -1;

		for(iF1 = 0; iF1 < nPF1; iF1++){
			for(iF0 = 0; iF0 < nPF0; iF0++){

				FLOAT distance[2];
				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);

				distDiff = GetDifference(distance[0], distance[1]);

				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);

				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);


				if(distDiff < minDiffDist){
					if(iPrevDist != iF2){
						iPrevDist = iF2;
						nMinDiff++;
					}
					minP0Dist[iF2] = distDiff;
					minDiffDist = distDiff;
				
				}

				if(angle < minDiffAngle){
					if(iPrevAngl != iF2){
						iPrevAngl = iF2;
						nMinAngl++;
					}
					minP0Angle[iF2] = angle;
					minDiffAngle = angle;
					
				}

				//if(distDiff < minDiffDist){
				//	minP0Dist[iF0] = distDiff;
				//	minDiffDist = distDiff;
				//}

				//if(angle < minDiffAngle){
				//	minP0Angle[iF2] = angle;
				//	minDiffAngle = angle;
				//}
			
			}
		}
	}
}

//***********************OUTPUT ENABLED**********************************************/

FLOAT GetParticleDistanceDifference(FPOINT pF0, FPOINT pF1, FPOINT pF2)
{
	FLOAT dF0F1 = GetDistance(pF0, pF1);
	FLOAT dF2F1 = GetDistance(pF2, pF1);

	return abs(dF0F1 - dF2F1);
}


////Three Frame Particle Track
//UINT TFPT(UINT frame, FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector)
//{
//
//	/***************************B-INITIALIZE******************************/
//	UINT i;
//
//	UINT nPF0;
//	UINT nPF2;
//
//	UINT nV1;
//	UINT nV2;
//	UINT nVOut;
//	
//	FPOINT ptHead[MAX_TRACK];
//	FPOINT ptTail[MAX_TRACK];
//
//	VECTOR v2[MAX_TRACK];	
//	VECTOR vF0[MAX_TRACK];
//	VECTOR vF2[MAX_TRACK];
//
//	PARTICLEGROUP pgF0[MAX_TRACK];
//	PARTICLEGROUP pgF2[MAX_TRACK];
//
//	FPOINT *ptF0 = (FPOINT *)calloc(GetNumberPoint(F0), sizeof(FPOINT));
//	FPOINT *ptF1 = (FPOINT *)calloc(GetNumberPoint(F1), sizeof(FPOINT));
//	FPOINT *ptF2 = (FPOINT *)calloc(GetNumberPoint(F2), sizeof(FPOINT));
//	
//	memset(v2, NULL, sizeof(v2));
//	memset(vF0, NULL, sizeof(vF0));
//	memset(vF2, NULL, sizeof(vF2));
//	/****************************E-INITIALIZE*****************************/
//
//	/*******************************B-BODY********************************/
//	GetPointFromPointSet(F0, ptF0);
//	GetPointFromPointSet(F1, ptF1);
//	GetPointFromPointSet(F2, ptF2);
//
//	nPF0 = GetF0ParticleGroups(NULL, F0, F1, F2, pgF0);
//	nPF2 = GetF2ParticleGroups(NULL, F0, F1, F2, pgF2);
//
//	//Only Interested in the Vectors associated with F1
//	//So Only Interested in V2
//	//V2-Tail = pF1
//	//V2-Head = pF0
//	for(i = 0; i < nPF0; i++){
//		memcpy(&ptTail[i], &pgF0[i].particleF1.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF0[i].particleF0.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF0, ptHead, ptTail, vF0);
//
//	for(i = 0; i < nPF2; i++){
//		memcpy(&ptTail[i], &pgF2[i].particleF2.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF2[i].particleF1.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF2, ptHead, ptTail, vF2);
//
//
//
//	nV1 = nPF0;
//	nV2 = nPF2;
//	nVOut = 0;
//
//	for(i = 0; i < nV1; i++){
//		vF0[i].j = vF0[i].j;
//		vF0[i].j = vF0[i].j;
//		vF2[i].j = vF2[i].j;
//	}
//
//
//	memcpy(vector, vF0, nV1*sizeof(VECTOR));
//	memcpy((vector + nV1), vF2, nV2*sizeof(VECTOR));
//	nVOut = nV1+nV2;
//
//	//nVOut = CombineUniqueDataVECTOR(nV1, nV2, (VECTOR *) vF0, (VECTOR *) vF2, (VECTOR *) vector);
//
//	/*******************************E-BODY********************************/
//
//	/*******************************OUTPUT********************************/
//
//	return nVOut;
//
//}

//Three Frame Particle Track
UINT TFPT(HWND hOutput, UINT frame, FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector)
{

	CHAR newLine[] = {13, 10, 0};
	CHAR output[256];

	/***************************B-INITIALIZE******************************/
	UINT i;

	UINT nPF0;
	UINT nPF2;

	UINT nV1;
	UINT nV2;
	UINT nVOut;
	
	FPOINT ptHead[MAX_TRACK];
	FPOINT ptTail[MAX_TRACK];

	VECTOR v2[MAX_TRACK];	
	VECTOR vF0[MAX_TRACK];
	VECTOR vF2[MAX_TRACK];

	PARTICLEGROUP pgF0[MAX_TRACK];
	PARTICLEGROUP pgF2[MAX_TRACK];

	FPOINT *ptF0 = (FPOINT *)calloc(GetNumberPoint(F0), sizeof(FPOINT));
	FPOINT *ptF1 = (FPOINT *)calloc(GetNumberPoint(F1), sizeof(FPOINT));
	FPOINT *ptF2 = (FPOINT *)calloc(GetNumberPoint(F2), sizeof(FPOINT));
	
	memset(v2, NULL, sizeof(v2));
	memset(vF0, NULL, sizeof(vF0));
	memset(vF2, NULL, sizeof(vF2));
	/****************************E-INITIALIZE*****************************/

	/*******************************B-BODY********************************/
	GetPointFromPointSet(F0, ptF0);
	GetPointFromPointSet(F1, ptF1);
	GetPointFromPointSet(F2, ptF2);

	nPF0 = GetF0ParticleGroups(hOutput, frame, F0, F1, F2, pgF0);
	nPF2 = GetF2ParticleGroups(hOutput, frame, F0, F1, F2, pgF2);

	//Only Interested in the Vectors associated with F1
	//So Only Interested in V2
	//V2-Tail = pF1
	//V2-Head = pF0
	for(i = 0; i < nPF0; i++){
		memcpy(&ptTail[i], &pgF0[i].particleF1.point, sizeof(FPOINT));
		memcpy(&ptHead[i], &pgF0[i].particleF0.point, sizeof(FPOINT));
	}

	GetVector(nPF0, ptHead, ptTail, vF0);

	for(i = 0; i < nPF2; i++){
		memcpy(&ptTail[i], &pgF2[i].particleF2.point, sizeof(FPOINT));
		memcpy(&ptHead[i], &pgF2[i].particleF1.point, sizeof(FPOINT));
	}

	GetVector(nPF2, ptHead, ptTail, vF2);



	nV1 = nPF0;
	nV2 = nPF2;
	nVOut = 0;

	for(i = 0; i < nV1; i++){
		vF0[i].j = vF0[i].j;
		vF0[i].j = vF0[i].j;
		vF2[i].j = vF2[i].j;
	}

	

	sprintf(output, "%snVF0 = %d%s", newLine, nV1, newLine);
	Edit_AppendText(hOutput, output);
	sprintf(output, "%snVF2 = %d%s", newLine, nV2, newLine);
	Edit_AppendText(hOutput, output);


	memcpy(vector, vF0, nV1*sizeof(VECTOR));
	memcpy((vector + nV1), vF2, nV2*sizeof(VECTOR));
	nVOut = nV1+nV2;

	if(ptF0)free(ptF0);
	if(ptF1)free(ptF1);
	if(ptF2)free(ptF2);

	//Edit_OutputVector(hOutput, nVOut, vector);

	//nVOut = CombineUniqueDataVECTOR(nV1, nV2, (VECTOR *) vF0, (VECTOR *) vF2, (VECTOR *) vector);

	/*******************************E-BODY********************************/

	/*******************************OUTPUT********************************/

	return nVOut;

}

////Three Frame Particle Track
//UINT TFPT(FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector)
//{
//
//	UINT i;
//
//	UINT nPF0;
//	UINT nPF2;
//
//	UINT nV1;
//	UINT nV2;
//	UINT nVOut;
//	
//	FPOINT ptHead[MAX_TRACK];
//	FPOINT ptTail[MAX_TRACK];
//
//	VECTOR v2[MAX_TRACK];	
//	VECTOR v2F0[MAX_TRACK];
//	VECTOR v2F2[MAX_TRACK];
//
//	PARTICLEGROUP pgF0[MAX_TRACK];
//	PARTICLEGROUP pgF2[MAX_TRACK];
//
//	FPOINT *ptF0 = (FPOINT *)calloc(GetNumberPoint(F0), sizeof(FPOINT));
//	FPOINT *ptF1 = (FPOINT *)calloc(GetNumberPoint(F1), sizeof(FPOINT));
//	FPOINT *ptF2 = (FPOINT *)calloc(GetNumberPoint(F2), sizeof(FPOINT));
//	
//	memset(v2, NULL, sizeof(v2));
//	memset(v2F0, NULL, sizeof(v2F0));
//	memset(v2F2, NULL, sizeof(v2F2));
//
//	GetPointFromPointSet(F0, ptF0);
//	GetPointFromPointSet(F1, ptF1);
//	/*GetPointFromPointSet(F1, ptF1);*/
//	GetPointFromPointSet(F2, ptF2);
//
//	nPF0 = GetF0ParticleGroups(NULL, F0, F1, F2, pgF0);
//	nPF2 = GetF2ParticleGroups(NULL, F0, F1, F2, pgF2);
//
//	//Only Interested in the Vectors associated with F1
//	//So Only Interested in V2
//	//V2-Tail = pF1
//	//V2-Head = pF0
//	for(i = 0; i < nPF0; i++){
//		memcpy(&ptTail[i], &pgF0[i].particleF1.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF0[i].particleF0.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF0, ptHead, ptTail, v2F0);
//
//	for(i = 0; i < nPF2; i++){
//		memcpy(&ptTail[i], &pgF2[i].particleF2.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF2[i].particleF1.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF2, ptHead, ptTail, v2F2);
//
//	nV1 = nPF0;
//	nV2 = nPF2;
//	nVOut = 0;
//
//	nVOut = CombineUniqueDataVECTOR(nV1, nV2, (VECTOR *) v2F0, (VECTOR *) v2F2, (VECTOR *) vector);
//
//	return nVOut;
//
//}
	
//VOID ParticleTrackTest()
//{
//	UINT i;
//
//	UINT nPF0;
//	UINT nPF2;
//
//	FPOINT ptTail[MAX_TRACK];
//	FPOINT ptHead[MAX_TRACK];
//
//	FPOINTSET pF0/*[MAX_TRACK]*/;
//	FPOINTSET pF1/*[MAX_TRACK]*/;
//	FPOINTSET pF2/*[MAX_TRACK]*/;
//
//	PARTICLEGROUP pgF0[MAX_TRACK];
//	PARTICLEGROUP pgF2[MAX_TRACK];
//
//	VECTOR v2[MAX_TRACK];	
//	VECTOR v2F0[MAX_TRACK];
//	VECTOR v2F2[MAX_TRACK];
//	
//
//	pF0.point = (FPOINT *)malloc(MAX_TRACK * sizeof(FPOINT));
//	pF1.point = (FPOINT *)malloc(MAX_TRACK * sizeof(FPOINT));
//	pF2.point = (FPOINT *)malloc(MAX_TRACK * sizeof(FPOINT));
//
//	pF0.nPoint = 3;
//	pF1.nPoint = 2;
//	pF2.nPoint = 4;
//
//	memcpy(&pF0.point[0], &FPoint(5, 20), sizeof(FPOINT));
//	memcpy(&pF0.point[1], &FPoint(6, 17), sizeof(FPOINT));
//	memcpy(&pF0.point[2], &FPoint(6, 7), sizeof(FPOINT));
//
//	memcpy(&pF1.point[0], &FPoint(19, 20), sizeof(FPOINT));
//	memcpy(&pF1.point[1], &FPoint(19, 12), sizeof(FPOINT));
//
//	memcpy(&pF2.point[0], &FPoint(33, 21), sizeof(FPOINT));
//	memcpy(&pF2.point[1], &FPoint(29, 15), sizeof(FPOINT));
//	memcpy(&pF2.point[2], &FPoint(32, 6), sizeof(FPOINT));
//	memcpy(&pF2.point[3], &FPoint(32, 11), sizeof(FPOINT));
//
//
//	//They return the number of goups they each found
//	nPF0 = GetF0ParticleGroups(NULL, pF0, pF1, pF2, pgF0);
//	nPF2 = GetF2ParticleGroups(NULL, pF0, pF1, pF2, pgF2);
//
//	//Only Interested in the Vectors associated with F1
//	//So Only Interested in V2
//	//V2-Tail = pF1
//	//V2-Head = pF0
//	for(i = 0; i < nPF0; i++){
//		memcpy(&ptTail[i], &pgF0[i].particleF1.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF0[i].particleF0.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF0, ptHead, ptTail, v2F0);
//
//	for(i = 0; i < nPF2; i++){
//		memcpy(&ptTail[i], &pgF2[i].particleF2.point, sizeof(FPOINT));
//		memcpy(&ptHead[i], &pgF2[i].particleF1.point, sizeof(FPOINT));
//	}
//
//	GetVector(nPF2, ptHead, ptTail, v2F2);
//
//	for(i = 0; i < min(nPF0, nPF2); i++)
//	{
//		v2F0[i].mag = v2F0[i].mag;
//		v2F2[i].mag = v2F2[i].mag;
//		i = i;
//	}
//
//	UINT nV1 = nPF0;
//	UINT nV2 = nPF2;
//	UINT nV = 0;
//
//	memset(v2, 0x00, sizeof(v2));
//	nV = CombineUniqueDataVECTOR(nV1, nV2, (VECTOR *) v2F0, (VECTOR *) v2F2, (VECTOR *) v2);
//
//
//
//	//for(i = 0; i < min(nPF0, nPF2); i++)
//	//{
//	//	v2[i].mag = v2[i].mag;
//	//	v2[i].mag = v2[i].mag;
//	//}
//
//
//
//
//
//}

UINT CombineUniqueDataVECTOR(UINT nData1, UINT nData2, VECTOR *dataIn1, VECTOR *dataIn2, VECTOR *dataOut)
{

	BOOL bAdd;

	UINT iD1;
	UINT iD2;
	UINT iDO;
	UINT iIn;
	UINT iOut;

	INT tempINT;
	INT temINT;

	VECTOR tempVECTOR;
	VECTOR temVECTOR;
	
	UINT nDataOut = 0;

	VECTOR *vector = (VECTOR *)calloc(nData1, sizeof(VECTOR));

	nDataOut = RemoveDuplicateVector(nData1, dataIn1, vector);

	memcpy(dataOut, vector, nDataOut*sizeof(VECTOR));
	//memset(dataOut, 0x00, MAX_TRACK*sizeof(VECTOR));
	

	for(iIn = 0; iIn < nData2; iIn++){

		bAdd = TRUE;
		for(iOut = 0; iOut < nDataOut; iOut++){

			if(dataOut[iOut].i == dataIn2[iIn].i && 
				dataOut[iOut].j == dataIn2[iIn].j /*&& 
				!(memcmp(&dataIn2[iIn].pTail, &dataOut[iOut].pTail, sizeof(FPOINT)))*/){
				bAdd = FALSE;
				break;
			}

		}

		if(bAdd){
			memcpy(&dataOut[nDataOut], &dataIn2[iIn], sizeof(VECTOR));
			nDataOut++;
		}
	}

	

	free(vector);
	return nDataOut;

}

UINT RemoveDuplicateVector(UINT nData, VECTOR *dataIn, VECTOR *dataOut)
{

	BOOL bAdd = TRUE;
	UINT iIn = 0;
	UINT iOut = 0;
	UINT nDataOut = 0;

	memset(dataOut, 0x00, nData*sizeof(VECTOR));
	memcpy(&dataOut[0], &dataIn[0], sizeof(VECTOR));
	nDataOut++;

	for(iIn = 0; iIn < nData; iIn++){

		bAdd = TRUE;
		for(iOut = 0; iOut < nDataOut; iOut++){

			if(!memcmp(&dataIn[iIn], &dataOut[iOut], sizeof(VECTOR)))
			{
				bAdd = FALSE;
				break;
			}

		}

		if(bAdd){
			memcpy(&dataOut[nDataOut], &dataIn[iIn], sizeof(VECTOR));
			nDataOut++;
		}
	}

	return nDataOut;
}



////Returns the number of groups found
//UINT GetF0ParticleGroups(UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF0)
//{
//
//	INT group[MAX_TRACK];
//	UINT cntGroup = 0;
//
//	FLOAT angle;
//	FPOINT ptTail;
//	FPOINT ptHead;
//	VECTOR vector;
//	VECTOR v1;
//	VECTOR v2;
//
//	UINT i;
//	UINT iF0;
//	UINT iF1;
//	UINT iF2;
//	UINT nPF0 = pF0.nPoint;
//	UINT nPF1 = pF1.nPoint;
//	UINT nPF2 = pF2.nPoint;
//	UINT gPF0, gPF1, gPF2;
//
//	FLOAT distDiff = 0;
//	FLOAT dF0F1 = 0;
//	FLOAT dF2F1 = 0;
//	
//	FLOAT threshold;
//	FLOAT temp;
//	FLOAT simF0 = 0;
//	FLOAT simF1 = 0;
//
//	FLOAT *minAngle = (FLOAT *)calloc(MAX_TRACK, sizeof(FLOAT));
//	FLOAT *minDist = (FLOAT *)calloc(MAX_TRACK, sizeof(FLOAT));
//
//	DOUBLE error = 0;
//	DOUBLE errMin = 0;
//
//	GetF0ParticleMinimums(pF0, pF1, pF2, minDist, minAngle);
//
//	memset(group, -1, MAX_TRACK*sizeof(INT));
//
//	for(iF0 = 0; iF0 < nPF0; iF0++){
//		errMin = 1000;
//		if(group[cntGroup] >= 0){
//			cntGroup++;
//		}
//		for(iF1 = 0; iF1 < nPF1; iF1++){
//			for(iF2 = 0; iF2 < nPF2; iF2++){
//
//				
//				DOUBLE distance[2];
//				DOUBLE anglErrorNorm;
//				DOUBLE distErrorNorm;
//				
//				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
//				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);
//
//				distDiff = GetDifference(distance[0], distance[1]);
//
//				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);
//
//				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);
//
//				if(minAngle[iF0] == 0.0)
//					minAngle[iF0] = 0.001;
//				if(minDist[iF0] == 0.00)
//					minDist[iF0] = 0.001;
//				
//				anglErrorNorm = ((FLOAT)angle / (FLOAT)minAngle[iF0]);
//				distErrorNorm = ((FLOAT)distDiff / (FLOAT)minDist[iF0]);
//
//				error = WT_ANGLE*anglErrorNorm + WT_DIST*distErrorNorm;
//
//				if(error < errMin && angle < MAX_ANGLE && distance[0] < MAXDIST_TFPT && distance[1] < MAXDIST_TFPT/*distDiff < MAXDIST_TFPTDIFF*/){
//					errMin = error;
//					
//					if(group[cntGroup] != iF0){
//						group[cntGroup] = iF0;
//						
//					}
//
//					pgF0[iF0].frmNum = frame;
//
//					pgF0[iF0].particleF0.frmID = iF0;
//					pgF0[iF0].particleF0.frmNum = frame - 0;
//					memcpy(&pgF0[iF0].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
//					
//
//					pgF0[iF0].particleF1.frmID = iF1;
//					pgF0[iF0].particleF1.frmNum = frame - 1;
//					memcpy(&pgF0[iF0].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));
//
//					pgF0[iF0].particleF2.frmID = iF2;
//					pgF0[iF0].particleF2.frmNum = frame - 2;
//					memcpy(&pgF0[iF0].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));
//				}
//			}
//		}
//
//
//	}
//
//	//Add one, it was zero-based counted
//	cntGroup++;
//	return cntGroup;	
//}
//
////Returns the number of groups found
//UINT GetF2ParticleGroups(UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF2/*PARTICLEGROUP *pGroupDist, PARTICLEGROUP *pGroupAngle*/)
//{
//	INT group[MAX_TRACK];
//	UINT cntGroup = 0;
//
//	FLOAT angle;
//	FPOINT ptTail;
//	FPOINT ptHead;
//	VECTOR vector;
//	VECTOR v1;
//	VECTOR v2;
//
//	UINT i;
//	UINT iF0;
//	UINT iF1;
//	UINT iF2;
//	UINT nPF0 = pF0.nPoint;
//	UINT nPF1 = pF1.nPoint;
//	UINT nPF2 = pF2.nPoint;
//	UINT gPF0, gPF1, gPF2;
//
//	FLOAT distDiff = 0;
//	FLOAT dF0F1 = 0;
//	FLOAT dF2F1 = 0;
//	
//	FLOAT threshold;
//	FLOAT temp;
//	FLOAT simF0 = 0;
//	FLOAT simF1 = 0;
//
//	FLOAT *minAngle = (FLOAT *)malloc(100*sizeof(FLOAT));
//	FLOAT *minDist = (FLOAT *)malloc(100*sizeof(FLOAT));
//
//	DOUBLE errMin = 10000;
//
//	GetF2ParticleMinimums(pF0, pF1, pF2, minDist, minAngle);
//
//	memset(group, -1, MAX_TRACK*sizeof(INT));
//
//	for(iF2 = 0; iF2 < nPF2; iF2++){
//		errMin = 1000;
//		if(group[cntGroup] >= 0){
//			cntGroup++;
//		}
//		
//			for(iF0 = 0; iF0 < nPF0; iF0++){
//				for(iF1 = 0; iF1 < nPF1; iF1++){
//				
//				DOUBLE distance[2];
//				DOUBLE anglErrorNorm;
//				DOUBLE distErrorNorm;
//				DOUBLE error;
//
//				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
//				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);
//
//				distDiff = GetDifference(distance[0], distance[1]);
//
//				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);
//
//				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);
//
//				if(minAngle[iF2] == 0.0)
//					minAngle[iF2] = 0.001;
//				if(minDist[iF2] == 0.00) 
//					minDist[iF2] = 0.001;
//				
//				anglErrorNorm = ((FLOAT)angle / (FLOAT)minAngle[iF2]);
//				distErrorNorm = ((FLOAT)distDiff / (FLOAT)minDist[iF2]);
//
//				error = WT_ANGLE*anglErrorNorm + WT_DIST*distErrorNorm;
//
//				if(error < errMin && angle < MAX_ANGLE && distance[0] < MAXDIST_TFPT && distance[1] < MAXDIST_TFPT/*distDiff < MAXDIST_TFPTDIFF*/){
//					errMin = error;
//					
//					if(group[cntGroup] != iF2){
//						group[cntGroup] = iF2;
//						//cntGroup++;
//					}
//
//					pgF2[iF2].frmNum = frame;
//
//					pgF2[iF2].particleF0.frmID = iF0;
//					pgF2[iF2].particleF0.frmNum = frame - 0;
//					memcpy(&pgF2[iF2].particleF0.point, &pF0.point[iF0], sizeof(FPOINT));
//					
//
//					pgF2[iF2].particleF1.frmID = iF1;
//					pgF2[iF2].particleF1.frmNum = frame - 1;
//					memcpy(&pgF2[iF2].particleF1.point, &pF1.point[iF1], sizeof(FPOINT));
//
//					pgF2[iF2].particleF2.frmID = iF2;
//					pgF2[iF2].particleF2.frmNum = frame - 2;
//					memcpy(&pgF2[iF2].particleF2.point, &pF2.point[iF2], sizeof(FPOINT));
//				}
//
//			}
//		}
//	}
//	
//	//Add one, it was zero-based counted
//	cntGroup++;
//	return cntGroup;
//}
//
//
//
//VOID GetF0ParticleMinimums(FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle)
//{
//
//	INT iPrevDist = -1;
//	INT iPrevAngl = -1;
//	UINT nMinDiff = 0;
//	UINT nMinAngl = 0;
//	
//	FPOINT ptTail;
//	FPOINT ptHead;
//	VECTOR v1;
//	VECTOR v2;
//
//	UINT i;
//	UINT iF0;
//	UINT iF1;
//	UINT iF2;
//	UINT nPF0 = pF0.nPoint;
//	UINT nPF1 = pF1.nPoint;
//	UINT nPF2 = pF2.nPoint;
//
//	FLOAT angle = 0;
//	FLOAT distDiff = 0;
//	FLOAT dF0F1 = 0;
//	FLOAT dF2F1 = 0;
//
//	FLOAT minDiffAngle;
//	FLOAT minDiffDist;
//
//
//	//Test
//	//memset(minP0Dist, -1, nPF0*sizeof(FLOAT));
//	//memset(minP0Angle, -1, nPF0*sizeof(FLOAT));
//
//	for(iF0 = 0; iF0 < nPF0; iF0++){
//		minDiffAngle = 380;
//		minDiffDist = 1000;
//		
//			for(iF2 = 0; iF2 < nPF2; iF2++){
//				for(iF1 = 0; iF1 < nPF1; iF1++){
//
//				FLOAT distance[2];
//				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
//				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);
//
//				distDiff = GetDifference(distance[0], distance[1]);
//
//				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);
//
//				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);
//
//				if(distDiff < minDiffDist){
//					if(iPrevDist != iF0){
//						iPrevDist = iF0;
//						nMinDiff++;
//					}
//					minP0Dist[iF0] = distDiff;
//					minDiffDist = distDiff;
//					
//				
//				}
//
//				if(angle < minDiffAngle){
//					if(iPrevAngl != iF0){
//						iPrevAngl = iF0;
//						nMinAngl++;
//					}
//					minP0Angle[iF0] = angle;
//					minDiffAngle = angle;
//					
//				}
//
//			
//			}
//		}
//	}
//
//	nMinAngl=nMinAngl;
//	nMinAngl=nMinAngl;
//
//}
//
//VOID GetF2ParticleMinimums(FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle)
//{
//	
//	FPOINT ptTail;
//	FPOINT ptHead;
//	VECTOR v1;
//	VECTOR v2;
//
//	UINT i;
//	UINT iF0;
//	UINT iF1;
//	UINT iF2;
//	UINT nPF0 = pF0.nPoint;
//	UINT nPF1 = pF1.nPoint;
//	UINT nPF2 = pF2.nPoint;
//
//	FLOAT angle = 0;
//	FLOAT distDiff = 0;
//	FLOAT dF0F1 = 0;
//	FLOAT dF2F1 = 0;
//
//	FLOAT minDiffAngle;
//	FLOAT minDiffDist;
//
//	for(iF0 = 0; iF0 < nPF0; iF0++){
//		minDiffAngle = 380;
//		minDiffDist = 1000;
//		for(iF1 = 0; iF1 < nPF1; iF1++){
//			for(iF2 = 0; iF2 < nPF2; iF2++){
//
//				FLOAT distance[2];
//				distance[0] = GetDistance(pF0.point[iF0], pF1.point[iF1]);
//				distance[1] = GetDistance(pF2.point[iF2], pF1.point[iF1]);
//
//				distDiff = GetDifference(distance[0], distance[1]);
//
//				GetVectorPair(pF0.point[iF0], pF1.point[iF1], pF2.point[iF2], &v1, &v2);
//
//				angle = GetIntersectionAngle(v1, v2, VI_DEGREE);
//
//				if(distDiff < minDiffDist){
//					minP0Dist[iF0] = distDiff;
//					minDiffDist = distDiff;
//				}
//
//				if(angle < minDiffAngle){
//					minP0Angle[iF2] = angle;
//					minDiffAngle = angle;
//				}
//
//
//			
//			}
//		}
//	}
//}