#include "stdafx.h"

#include "Trajectory.h"
#include "MotionCorrespondence.h"
#include "Vector.h"

DWORD DrawTrajectory(UINT w, UINT h, PIXEL *img, TRAJECTORY trajectory, COLORREF c)
{
	if(!img){
		return FALSE;
	}

	if(!trajectory.ps.nPoint){
		return FALSE;
	}

	VECTOR v = {0};
	
	INT i = NULL;

	for(i = 1; i < trajectory.ps.nPoint; i++){

		GetVector(trajectory.ps.point[i], trajectory.ps.point[i-1], &v);
		DrawVector(w, h, img, v, c);

	}

	//GetVector(trajectory.ps.point[0], trajectory.ps.point[1], &v);
	//DrawVector(w, h, img, v, c);

	return TRUE;


}

DWORD DrawTrajectorySet(UINT w, UINT h, PIXEL *img, TRAJECTORYSET ts, COLORREF c)
{

	if(!img){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD n = ts.nTrajectory;

	for(i = 0; i < n; i++){
		DrawTrajectory(w, h, img, ts.trajectory[i], c);
	}

	return TRUE;
}

//Cycles through each 'tsAppend' trajaectory and attempts to add to the existing partial path
//in 'ts'.  If a match is found the newest position is added
DWORD AppendMatchedTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend)
{
	if(!ts || !tsAppend){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD iT = NULL;		//Trajectory 'ts' index
	DWORD iA = NULL;		//Trajectory 'tsAppend' index
	DWORD n = ts->nTrajectory;
	DWORD nAppend = tsAppend->nTrajectory;

	FLOAT dX = NULL;
	FLOAT dY = NULL;

	FPOINT ptT = {0};
	FPOINT ptA = {0};

	DWORD flgAppend = TRUE;

	if(n){

		for(iA = 0; iA < nAppend; iA++){
			flgAppend = TRUE;
			for(iT = 0; iT < ts->nTrajectory; iT++){

				//Get the last point from each trajectory
				i = GetSize(ts->trajectory[iT]);
				GetAt(ts->trajectory[iT], i-1, &ptT);

				i = GetSize(tsAppend->trajectory[iA]);
				GetAt(tsAppend->trajectory[iA], i-2, &ptA);

				dX = abs(ptT.x - ptA.x);
				dY = abs(ptT.y - ptA.y);

				if(dX <= 0.1) dX = NULL;
				if(dY <= 0.1) dY = NULL;

				if(!dX && !dY){
					GetAt(tsAppend->trajectory[iA], i-1, &ptA);
					Push(&ts->trajectory[iT], ptA);
					flgAppend = FALSE;
				}

			}

			if(flgAppend){
				Push(ts, tsAppend->trajectory[iA]);
			}
			flgAppend = TRUE;
		}

	}else{
		Copy(ts, tsAppend);
	}



	return TRUE;
}

//Cycles through each 'tsAppend' trajaectory and attempts to add to the existing partial path
//in 'ts'.  If a match is found the newest position is added
DWORD AppendRemoveTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend)
{
	if(!ts || !tsAppend){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD iT = NULL;		//Trajectory 'ts' index
	DWORD iA = NULL;		//Trajectory 'tsAppend' index
	DWORD nT = ts->nTrajectory;
	DWORD nA = tsAppend->nTrajectory;

	FLOAT dX = NULL;
	FLOAT dY = NULL;

	FPOINT ptT = {0};
	FPOINT ptA = {0};

	DWORD flgAppend = TRUE;

	//-'removeMap' stores a high or low value indicating whether a trajectory
	//has been added to
	//-If a trajectory has not been added to, it is considered terminated (expired, abolished) and
	//should be removed
	//-Termination occurs due to particles falling out of detection (either from leaving
	//the field of view, or filter problems), or particle mismatch

	//INITIALIZE: Set all removeMap indexes to high (assuming all trajectories will be removed later)
	UCHAR *removeMap = (UCHAR *)malloc(ts->nTrajectory*sizeof(UCHAR));
	if(!removeMap){
		return FALSE;
	}
	memset(removeMap, TRUE, ts->nTrajectory*sizeof(UCHAR));


	if(nT){

		for(iA = 0; iA < nA; iA++){
			flgAppend = TRUE;
			for(iT = 0; iT < nT; iT++){

				//Get the last point from each trajectory
				i = GetSize(ts->trajectory[iT]);
				GetAt(ts->trajectory[iT], i-1, &ptT);

				i = GetSize(tsAppend->trajectory[iA]);
				GetAt(tsAppend->trajectory[iA], i-2, &ptA);

				dX = abs(ptT.x - ptA.x);
				dY = abs(ptT.y - ptA.y);

				i = GetSize(ts->trajectory[iT]);
				GetAt(ts->trajectory[iT], i-2, &ptT);

				i = GetSize(tsAppend->trajectory[iA]);
				GetAt(tsAppend->trajectory[iA], i-3, &ptA);

				dX = dX + abs(ptT.x - ptA.x);
				dY = dY + abs(ptT.y - ptA.y);

				if(dX <= 0.1) dX = NULL;
				if(dY <= 0.1) dY = NULL;

				if(!dX && !dY){
					GetAt(tsAppend->trajectory[iA], i-1, &ptA);
					Push(&ts->trajectory[iT], ptA);
					flgAppend = FALSE;
					removeMap[iT] = NULL;		//Set remove map bit to low so it's not removed at the end
				}

			}

			//'tsAppend->trajectory[iA]' is a new trajectory,
			//so add it to the trajectory set
			if(flgAppend){
				Push(ts, tsAppend->trajectory[iA]);
			}
		}

	}else{
		Copy(ts, tsAppend);
	}

	//Remove trajectories based on 'removeMap' flags
	for(i = 0; i < nT; i++){
		if(removeMap[i]){
			Remove(ts, i);
			//Empty(&ts->trajectory[i]);
		}

	}

	if(removeMap)free(removeMap);


	return TRUE;
}

//Untested: 12/3/13
//Cycles through each 'tsAppend' trajaectory and attempts to add to the existing partial path
//in 'ts'.  If a match is found the newest position is added
//'modMap' is an array used to hold the position and state of trajectory modification
////TRUE - if trajectory has been appended to
////FALSE - otherwise
DWORD AppendTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend, UCHAR *modMap)
{
	if(!ts || !tsAppend || !modMap){
		return FALSE;
	}

	DWORD i = NULL;
	DWORD iT = NULL;		//Trajectory 'ts' index
	DWORD iA = NULL;		//Trajectory 'tsAppend' index
	DWORD nT = ts->nTrajectory;
	DWORD nA = tsAppend->nTrajectory;

	FLOAT dX = NULL;
	FLOAT dY = NULL;

	FPOINT ptT = {0};
	FPOINT ptA = {0};

	DWORD flgAppend = TRUE;

	//-'modMap' stores a high or low value indicating whether a trajectory
	//has been added to
	//-If a trajectory has not been added to, it is considered terminated (expired, abolished) and
	//should be removed
	//-Termination occurs due to particles falling out of detection (either from leaving
	//the field of view, or removal by filtering), or particle mismatch

	//Removed 9.4.15
	//Looking for memory fault
	//memset(modMap, NULL, ts->nTrajectory*sizeof(UCHAR));

	if(nT){

		for(iA = 0; iA < nA; iA++){
			flgAppend = TRUE;
			for(iT = 0; iT < nT; iT++){

				//Get the last point from each trajectory
				i = GetSize(ts->trajectory[iT]);
				GetAt(ts->trajectory[iT], i-1, &ptT);

				i = GetSize(tsAppend->trajectory[iA]);
				GetAt(tsAppend->trajectory[iA], i-2, &ptA);

				dX = abs(ptT.x - ptA.x);
				dY = abs(ptT.y - ptA.y);

				i = GetSize(ts->trajectory[iT]);
				GetAt(ts->trajectory[iT], i-2, &ptT);

				i = GetSize(tsAppend->trajectory[iA]);
				GetAt(tsAppend->trajectory[iA], i-3, &ptA);

				dX = dX + abs(ptT.x - ptA.x);
				dY = dY + abs(ptT.y - ptA.y);

				if(dX <= 0.1) dX = NULL;
				if(dY <= 0.1) dY = NULL;

				if(!dX && !dY){
					GetAt(tsAppend->trajectory[iA], i-1, &ptA);
					Push(&ts->trajectory[iT], ptA);
					flgAppend = FALSE;
					modMap[iT] = TRUE;		//Set 'modMap' bit high indicating it has been modified
				}

			}

			//'tsAppend->trajectory[iA]' is a new trajectory,
			//so add it to the trajectory set
			if(flgAppend){
				Push(ts, tsAppend->trajectory[iA]);
			}
		}

	}else{
		Copy(ts, tsAppend);
	}

	return TRUE;
}

DWORD RemoveTrajectories(TRAJECTORYSET *ts, DWORD sizeModMap, UCHAR *modMap)
{
	if(!ts || !modMap){
		return FALSE;
	}

	INT n = sizeModMap;
	INT i = NULL;

	//Changed 9.6.15
	//for(i = n; i > 0; i--){
	for(i = n; i >= 0; i--){
		if(!modMap[i]){
			Remove(ts, i);
		}
	}

	//for(i = 0; i < n; i++){
	//	if(!modMap[i]){
	//		Remove(ts, i);
	//	}
	//}

	return TRUE;
}

DWORD TrajecotryManagmentTest(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend)
{

	if(!ts || !tsAppend){
		return FALSE;
	}
	
	DWORD n = ts->nTrajectory;
	
	UCHAR *modMap = (UCHAR *)malloc(n*sizeof(UCHAR));

	if(!modMap)
		return FALSE;

	memset(modMap, NULL, n*sizeof(UCHAR));

	AppendTrajectories(ts, tsAppend, modMap);
	RemoveTrajectories(ts, n, modMap);

	free(modMap);

	return TRUE;
}

//Capture *tsRemoved so that the results can be output as post processing
DWORD RemoveTrajectories(TRAJECTORYSET *ts, TRAJECTORYSET *tsRemoved, DWORD sizeModMap, UCHAR *modMap)
{
	if(!ts || !modMap){
		return FALSE;
	}

	INT n = sizeModMap;
	INT i = NULL;

	//Changed 9.6.15
	//for(i = n; i > 0; i--){
	for(i = n; i >= 0; i--){
		if(!modMap[i]){
			//Push(tsRemoved, ts[i]);
			Remove(ts, i);
		}
	}

	return TRUE;
}

DWORD TrajecotryManagmentTest(TRAJECTORYSET *ts, TRAJECTORYSET *tsAppend, TRAJECTORYSET *tsRemoved)
{

	if(!ts || !tsAppend){
		return FALSE;
	}
	
	DWORD n = ts->nTrajectory;
	
	UCHAR *modMap = (UCHAR *)malloc(n*sizeof(UCHAR));

	if(!modMap)
		return FALSE;

	memset(modMap, NULL, n*sizeof(UCHAR));

	AppendTrajectories(ts, tsAppend, modMap);
	RemoveTrajectories(ts, n, modMap);

	free(modMap);

	return TRUE;
}



//Remove trajectories in reverse order to reduce the number of trajectory shift operations in 'Remove'
DWORD Reverse(TRAJECTORYSET *ts)
{

	if(!ts){
		return FALSE;
	}

	DWORD n = ts->nTrajectory;
	DWORD i = NULL;

	for(i = 0; i < n; i++){
		Reverse(&ts->trajectory[i]);
	}


	return TRUE;
}

INT AreSame(TRAJECTORY *t1, TRAJECTORY *t2)
{
	if(!t1 || !t2){
		return (-1);
	}

	if(t1->ps.nPoint != t2->ps.nPoint){
		return FALSE;
	}


	DWORD n = t1->ps.nPoint;
	DWORD i = NULL;

	for(i = 0; i < n; i++){
		
		if(!AreSame(t1->ps.point[i], t2->ps.point[i])){
			return FALSE;
		}

	}


	return TRUE;

}

//Keeps only congruence between 'ts1' and 'ts2' appending the congruencies of 'ts1' to 'tsOut'
//Trajectories from 'ts2' are not added, it is only used as a reference
DWORD DiscardMismatches(TRAJECTORYSET *ts1, TRAJECTORYSET *ts2, TRAJECTORYSET *tsOut)
{
	if(!ts1 || !ts2 || !tsOut){
		return FALSE;
	}

	DWORD n1 = ts1->nTrajectory;
	DWORD n2 = ts2->nTrajectory;

	Clear(tsOut);

	DWORD i1 = NULL;
	DWORD i2 = NULL;

	for(i1 = NULL; i1 < n1; i1++){
		for(i2 = NULL; i2 < n2; i2++){

			if(AreSame(&ts1->trajectory[i1], &ts2->trajectory[i2])){
				Push(tsOut, ts1->trajectory[i1]);
			}

		}
	}


	return TRUE;
}

