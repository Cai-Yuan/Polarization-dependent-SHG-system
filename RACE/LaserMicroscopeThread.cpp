

#include "stdafx.h"

#include "UNO32.h"
#include "XBox.h"
#include "EnumerateDevice.h"
//#include "CircularBuffer.h"
#include "Sum.h"
#include "SC10Shutter.h"
#include "ToolsWin32.h"
#include "Point.h"
#include "Object.h"
#include "Text.h"
#include "Magnitude.h"
#include "Norm.h"
#include "FilePath.h"
#include "GetFolderPath.h"
#include "Point.h"

#include "A3200.h"
#include "A3200Stage.h"
#include "A3200ErrorCode.h"

#include "LaserMicroscope.h"
#include "StageProc.h"
#include "SystemProc.h"
#include "DeviceProc.h"
#include "AutoFocusProc.h"

#include "Point.h"
//DWORD UpdateInflightVelocity(LASERMICROSCOPE *lpMicroscope);

DWORD WINAPI LaserMicroscopeThread(LPVOID param)
{

	LASERMICROSCOPE *lpMicroscope = (LASERMICROSCOPE *)param;

	if(!lpMicroscope)
		return FALSE;

	LM_SYSTEM *lpSystem = lpMicroscope->lpSystem;
	LM_STAGE *lpStage = lpMicroscope->lpStage;
	LM_DISPLAY *lpDisplay = lpMicroscope->lpDisplay;
	LM_DEVICE *lpDevice = lpMicroscope->lpDevice;
	LM_XBOX *lpXBox = lpDevice->lpXBox;
	LM_SC10 *lpSC10 = lpDevice->lpSC10;
	LM_AUTOFOCUS *lpAutoFocus = lpMicroscope->lpAutoFocus;

	INT iAutoFocus = 0;
	DOUBLE focalPlane = 0;
	DOUBLE maxMeasure = 0;
	DOUBLE_3DPOINT dptStart;
	DOUBLE_3DPOINT dptMax;
	DOUBLE goTo = 0;
	DWORD flgHoldPrev = TRUE;

	CHAR str[MAX_STRING];
	FLOAT prd = ((FLOAT) 1000 / (FLOAT)lpMicroscope->lpSystem->sampleRate);
	DWORD tUpdate = clock()+prd;
	clock_t cTime = NULL;
	ErrorData errorData;

	if(1){
		A3200Handle handle;
		A3200Reset(lpDevice->hAero);
		A3200Connect(&handle);
		A3200AcknowledgeAll(handle, TASKID_01);
		A3200Disconnect(handle);
	}


	while(lpMicroscope->flgThread){
		if(!lpDevice->hAero){
			A3200Reset(lpDevice->hAero);
			A3200Connect(&lpDevice->hAero);
			if(lpDevice->hAero){
				A3200MotionDisable(lpDevice->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
				A3200MotionFaultAck(lpDevice->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
				if(!A3200MotionEnable(lpDevice->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02)){
					MessageBox(NULL, "Drivers Not Enabled!", "Drivers Not Enabled!", MB_OK);
				}
				DOUBLE acc[3];
				acc[0] = 50.0;
				acc[1] = 50.0;
				acc[2] = 50.0;
				A3200MotionSetupRampTypeAxis(lpDevice->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02, RAMPTYPE_Linear);
				A3200MotionSetupRampRateAxis(lpDevice->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02, acc);
				GetA3200Position(lpDevice->hAero, &lpStage->dptHold);
				lpStage->flgHold = TRUE;
			}
		}

		errorData = A3200GetLastError();
		if(errorData.Code == ErrorCode_CommunicationLost){
			A3200Disconnect(&lpDevice->hAero);
			lpDevice->hAero = NULL;
		}
		prd = ((FLOAT) 1000 / (FLOAT)lpSystem->sampleRate);
		cTime = clock();
		if(cTime >= tUpdate){
			tUpdate = tUpdate+prd;

			GetA3200Position(lpDevice->hAero, &lpStage->dptCurrent);
			UpdateVelocity(lpMicroscope);
			DisplayLaserPosition(lpMicroscope, lpStage->dptCurrent);

			if(lpStage->flgMoveTo){
				if(!lpStage->flgZDelay){
				//Remove the MoveToFlag
					if(Compare(lpStage->dptCurrent, lpStage->dptMoveTo)){
						lpStage->xVel = 0;
						lpStage->yVel = 0;
						lpStage->flgMoveTo = FALSE;
					}
				}
				if(lpStage->flgZDelay){
					if(!lpStage->flgMoveToZ){	
						if(CompareXY(lpStage->dptCurrent, lpStage->dptMoveTo)){
							A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptMoveTo.z, abs(lpStage->zVelMax*0.001));
							lpStage->xVel = 0;
							lpStage->yVel = 0;
							lpStage->flgMoveToZ = TRUE;
							lpStage->zVel = lpStage->zVelMax;
						}
					}
				}
				if(lpStage->flgMoveToZ){
					if(CompareZ(lpStage->dptCurrent, lpStage->dptMoveTo)){
						lpStage->flgMoveTo = FALSE;
						lpStage->flgMoveToZ = FALSE;
						lpStage->zVel = 0;
						UpdateVelocity(lpMicroscope);
					}
				}


			}	
			CreateDisplayObjects(lpMicroscope);
			SendMessage(lpMicroscope->lpDisplay->hDataView, WM_SETOBJECT, NULL, (LPARAM)&lpDisplay->os);
		}

		if(lpStage->flgInitMoveTo){

			lpStage->flgMoveTo = TRUE;

			VECTOR v;
			GetVector(lpStage->dptMoveTo, lpStage->dptCurrent, &v);
			lpStage->xVel = lpStage->xVelMax*cos((v.dir*PI)/180.0);
			lpStage->yVel = lpStage->yVelMax*sin((v.dir*PI)/180.0);
				
			if(lpStage->xVelMax != lpStage->xVelCoarse){
				if(lpStage->flgZDelay){
					A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptMoveTo.y, abs(lpStage->yVel*0.001));
					A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptMoveTo.x, abs(lpStage->xVel*0.001));
				}else{
					lpStage->zVel = lpStage->zVelFine;
					A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptMoveTo.y, abs(lpStage->yVel*0.001));
					A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptMoveTo.x, abs(lpStage->xVel*0.001));
					A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptMoveTo.z, abs(lpStage->zVel*0.001));
				}
			}else{
				lpStage->zVel = lpStage->zVelMax;
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptMoveTo.y, abs(lpStage->yVel*0.001));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptMoveTo.x, abs(lpStage->xVel*0.001));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptMoveTo.z, abs(lpStage->zVel*0.001));
			}

			GetA3200Position(lpDevice->hAero, &lpStage->dptPrev);

			lpStage->flgInitMoveTo = FALSE;

		}


		if(!lpStage->flgMoveTo){
			if(!lpAutoFocus->flgAutoFocus){
				if(lpXBox->flgXInput || lpXBox->flgYInput){

					FLOAT xVel;
					FLOAT yVel;
					FLOAT xStick = lpXBox->xStick;
					FLOAT yStick = lpXBox->yStick;
					FLOAT norm = Norm(xStick, yStick);

					VECTOR v;
					FPOINT p1;
					FPOINT p2;
					p1.x = 0;
					p1.y = 0;
					p2.x = xStick;
					p2.y = yStick;
					GetVector(p2, p1, &v);
					FLOAT anglRad = (v.dir*PI)/180.0;

					lpStage->xVel = (abs(v.i)+abs(v.j))*lpStage->xVelMax*cos(anglRad);
					lpStage->yVel = (abs(v.i)+abs(v.j))*lpStage->yVelMax*sin(anglRad);
					xVel = (abs(v.i)+abs(v.j))*lpStage->xVelMax*cos(anglRad)*lpStage->xPolStage;
					yVel = (abs(v.i)+abs(v.j))*lpStage->yVelMax*sin(anglRad)*lpStage->yPolStage;

					
					if(0){//Keyboard Input
					if(GetKeyState(VK_A)&128){
						lpStage->xVel = 1.0*(lpStage->xVelMax/5.0);
						xVel = lpStage->xVel;
					}
					if(GetKeyState(VK_D)&128){
						lpStage->xVel = -1.0*(lpStage->xVelMax/5.0);
						xVel = lpStage->xVel;
					}
					if(GetKeyState(VK_W)&128){
						lpStage->yVel = -1.0*(lpStage->yVelMax/5.0);
						yVel = lpStage->yVel;
					}
					if(GetKeyState(VK_S)&128){
						lpStage->yVel = 1.0*(lpStage->yVelMax/5.0);
						yVel = lpStage->yVel;
					}
					}

					A3200MotionFreeRun(lpDevice->hAero, TASKID_Library, AXISINDEX_01, xVel*0.001);
					A3200MotionFreeRun(lpDevice->hAero, TASKID_Library, AXISINDEX_00, yVel*0.001);

					Edit_SetFloat(lpStage->hEditXVel, lpStage->xVel);
					Edit_SetFloat(lpStage->hEditYVel, lpStage->yVel);
					
					lpXBox->flgXInput = FALSE;
					lpXBox->flgYInput = FALSE;
				}

				if(lpXBox->flgZInput){

					lpStage->zVel = lpXBox->zStick*lpStage->zVelMax*lpStage->zPolStage;

					if(0){//Keyboard Input
					if(GetKeyState(VK_UP)&128){
						lpStage->zVel = (lpStage->zVelMax/5.0);
					}
					if(GetKeyState(VK_DOWN)&128){
						lpStage->zVel = -1.0*(lpStage->zVelMax/5.0);
					}
					}


					Edit_SetFloat(lpStage->hEditZVel, lpStage->zVel);
					A3200MotionFreeRun(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->zVel*0.001);
					lpXBox->flgZInput = FALSE;
				}
			}

		}

		//START_BUTTON
		if(lpXBox->flgStartButton){
			if(lpXBox->flgRightTrigger)
			{
				P3D p;
				GetA3200Position(lpDevice->hAero, &p);
				Append(lpStage->pSet, p);
			}
			else{
				A3200Connect(&lpDevice->hAero);
				A3200MotionFaultAck(lpDevice->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
			}

			lpXBox->flgStartButton = NULL;

		}

		//A-BUTTON
		if(lpXBox->flgAButton == XB_BUTTON_PRESSED){
			if(lpXBox->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpDevice->hAero, &lpStage->dptA);
				lpXBox->flgRightShoulder = NULL;
			}
			if(lpXBox->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpStage->dptMoveTo, &lpStage->dptA, sizeof(DOUBLE_3DPOINT));
				lpStage->flgInitMoveTo = TRUE;
				lpXBox->flgLeftShoulder = NULL;
			}
			lpXBox->flgAButton = NULL;
			CheckButton(lpDisplay->hCheckA, &lpDisplay->flgAPosition);
			//lpStage->flgAPosition = TRUE;
		}

		//B-BUTTON
		if(lpXBox->flgBButton == XB_BUTTON_PRESSED){
			if(lpXBox->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpDevice->hAero, &lpStage->dptB);
				lpXBox->flgRightShoulder = NULL;
			}
			if(lpXBox->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpStage->dptMoveTo, &lpStage->dptB, sizeof(DOUBLE_3DPOINT));
				lpStage->flgInitMoveTo = TRUE;
				lpXBox->flgLeftShoulder = NULL;
			}
			lpXBox->flgBButton = NULL;
			CheckButton(lpDisplay->hCheckB, &lpDisplay->flgBPosition);
			//lpStage->flgBPosition = TRUE;
		}

		//X-BUTTON
		if(lpXBox->flgXButton == XB_BUTTON_PRESSED){
			if(lpXBox->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpDevice->hAero, &lpStage->dptX);
				lpXBox->flgRightShoulder = NULL;
			}
			if(lpXBox->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpStage->dptMoveTo, &lpStage->dptX, sizeof(DOUBLE_3DPOINT));
				lpStage->flgInitMoveTo = TRUE;
				lpXBox->flgLeftShoulder = NULL;
			}
			lpXBox->flgXButton = NULL;
			CheckButton(lpDisplay->hCheckX, &lpDisplay->flgXPosition);
			//lpStage->flgXPosition = TRUE;
		}

		//Y-BUTTON
		if(lpXBox->flgYButton == XB_BUTTON_PRESSED){
			if(lpXBox->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpDevice->hAero, &lpStage->dptY);
				lpXBox->flgRightShoulder = NULL;
			}
			if(lpXBox->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpStage->dptMoveTo, &lpStage->dptY, sizeof(DOUBLE_3DPOINT));
				lpStage->flgInitMoveTo = TRUE;
			}
			lpXBox->flgYButton = NULL;
			CheckButton(lpDisplay->hCheckY, &lpDisplay->flgYPosition);
			//lpStage->flgYPosition = TRUE;
		}

		//RIGHT TRIGGER
		if(lpXBox->flgRightTrigger == XB_BUTTON_PRESSED){
			DataView_SnapFrame(lpMicroscope->lpDisplay->hDataView);
			ShowWindow(lpMicroscope->lpDisplay->hDataView, SW_SHOW);
			//SendMessage(lpMicroscope->hAutoFocus, WM_AUTOFOCUS, NULL, NULL);
		}

		//BACK_BUTTON
		if(lpXBox->flgBackButton || lpXBox->flgLeftTrigger){
			//lpAutoFocus->flgAutoFocus = FALSE;
			if(lpStage->flgMoveTo){
				memcpy(&lpStage->dptMoveTo, &lpStage->dptCurrent, sizeof(UNO32_3DPOINT));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptMoveTo.y, abs(lpStage->yVelMax*0.001));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptMoveTo.x, abs(lpStage->xVelMax*0.001));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptMoveTo.z, abs(lpStage->zVelMax*0.001));
				lpXBox->flgBackButton = NULL;
			}
			lpXBox->flgLeftTrigger = NULL;
		}

		

		UpdateInflightVelocity(lpMicroscope);

		lpAutoFocus->flgAutoFocus = 0;
		if(lpAutoFocus->flgAutoFocus){
			
			if(lpAutoFocus->flgInitAutoFocus){
				InitAutoFocus(lpMicroscope);
				iAutoFocus = 0;
				maxMeasure = 0;
				//focalPlane = lpStage->dptCurrent.z*0.001;
				memcpy(&dptMax, &lpStage->dptCurrent, sizeof(DOUBLE_3DPOINT));
				DOUBLE offset = (lpAutoFocus->af.nStep*(lpAutoFocus->af.stepDist*0.001)*0.5);
				goTo = dptStart.z+offset;
				focalPlane = goTo;
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, goTo, abs(5000*0.001));
			}
			lpStage->flgMoveTo = FALSE;
			lpStage->flgMoveToZ = FALSE;
			goTo = goTo - (lpAutoFocus->af.stepDist*0.001);
			//goTo = dptStart.z-(iAutoFocus*(lpAutoFocus->af.stepDist*0.001));
			A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, goTo, abs(5000*0.001));
			iAutoFocus++;

			if(WaitForA3200Z(lpDevice->hAero, goTo)){
				DOUBLE measure;
				measure = GetFocalMeasure(lpMicroscope);
				if(measure > maxMeasure){
					maxMeasure = measure;
					focalPlane = goTo;
				}
			}else{
				ThrowMessage(lpMicroscope->hOwner, "Focal Measure could not be calculated");
			}

			if(iAutoFocus > lpAutoFocus->af.nStep){
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, focalPlane, abs(5000*0.001));
				//A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, dptStart.z, abs(5000*0.001));
				iAutoFocus = 0;
				lpAutoFocus->flgAutoFocus = FALSE;
			}
		}else{
			iAutoFocus = 0;
			memcpy(&dptStart, &lpStage->dptCurrent, sizeof(DOUBLE_3DPOINT));
		}
		
		lpDisplay->flgFrameIn = FALSE;

		cTime = clock();
		if(tUpdate+prd > cTime){
			Sleep(tUpdate+prd-cTime-1);
		}

		if(1){
		if(lpMicroscope->flgZStack)
		{
			DOUBLE zRef;
			DOUBLE zGoTo;
			INT slice = lpMicroscope->slice;
			FLOAT stepSize = lpMicroscope->stepSize;
			INT w = lpMicroscope->pdmf.pdmfh.width;
			INT h = lpMicroscope->pdmf.pdmfh.height;
			CHAR str[MAX_STRING];
			PDMFRAME pdmf;
			AllocPDMFrame(&pdmf, w, h, DT_PIXEL);
			lpMicroscope->pdmf.pdmfh.typeMetaData = MD_FRAME;

			sprintf(str, "%s\Slice%d.pdm", lpMicroscope->strFolder, lpMicroscope->slice);

			if(lpMicroscope->slice == 0){
				memcpy(&lpMicroscope->ptZRef, &lpStage->dptCurrent, sizeof(DOUBLE_3DPOINT));
				zRef = lpMicroscope->ptZRef.z;
				
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptCurrent.z, abs(lpStage->zVelMax*0.001));
				Sleep(100);
				lpMicroscope->flgFrameIn = FALSE;
				while(!lpMicroscope->flgFrameIn){
					Sleep(1);
				}
				WritePDMFrame(lpMicroscope->pdmf.pdmfh.typeData, lpMicroscope->pdmf.pdmfh.typeMetaData, lpMicroscope->pdmf, str, lpMicroscope->pdmf.lpvoid);
			}

			zGoTo = (zRef/*lpMicroscope->zScale*/) + (slice*stepSize*0.001);
			A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, zGoTo, abs(lpStage->zVelMax*0.001));
			Sleep(100);
			lpMicroscope->flgFrameIn = FALSE;
			while(!lpMicroscope->flgFrameIn){
				Sleep(1);
			}
			lpMicroscope->pdmf.pdmfh.frame = lpMicroscope->slice;
			WritePDMFrame(lpMicroscope->pdmf.pdmfh.typeData, lpMicroscope->pdmf.pdmfh.typeMetaData, lpMicroscope->pdmf, str, lpMicroscope->pdmf.lpvoid);
			lpMicroscope->slice++;

			if(lpMicroscope->slice == lpMicroscope->nSlice){
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpMicroscope->ptZRef.z, abs(lpStage->zVelMax*0.001));
				lpMicroscope->slice = 0;
				lpMicroscope->flgZStack = FALSE;
				Sleep(200);
			}

		}
		}

		if(lpMicroscope->flgAutoFocus || 
			lpStage->flgInitMoveTo || 
			lpStage->flgMoveTo || 
			lpStage->flgMoveToZ || 
			lpStage->zVel || 
			lpStage->xVel ||
			lpStage->yVel){
			lpStage->flgHold = FALSE;
		}else{
			lpStage->flgHold = TRUE;
		}

		if(!flgHoldPrev && lpStage->flgHold){
				GetA3200Position(lpDevice->hAero, &lpStage->dptHold);
		}

		if(lpStage->flgHold){
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptHold.y, abs(lpStage->yVelMax*0.001));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptHold.x, abs(lpStage->xVelMax*0.001));
				A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptHold.z, abs(lpStage->zVelMax*0.001));
				A3200MotionFreeRunStop(lpDevice->hAero, TASKID_Library, AXISINDEX_00);
				A3200MotionFreeRunStop(lpDevice->hAero, TASKID_Library, AXISINDEX_01);
				A3200MotionFreeRunStop(lpDevice->hAero, TASKID_Library, AXISINDEX_02);
		}
		flgHoldPrev = lpStage->flgHold;

	}

	A3200Disconnect(lpDevice->hAero);


	return FALSE;
}

DWORD DisplayLaserPosition(LASERMICROSCOPE *lpMicroscope, DOUBLE_3DPOINT pt)
{
	Edit_SetFloat(lpMicroscope->lpStage->hEditXPos, pt.x*1000.0);
	Edit_SetFloat(lpMicroscope->lpStage->hEditYPos, pt.y*1000.0);
	Edit_SetFloat(lpMicroscope->lpStage->hEditZPos, pt.z*1000.0);

	return TRUE;
}

//DWORD UpdateVelocity(LASERMICROSCOPE *lpMicroscope)
//{
//	if(!lpMicroscope){
//		return FALSE;
//	}
//	
//	UpdateVelocity(lpMicroscope->lpStage);
//	//LM_STAGE *lpStage = lpMicroscope->lpStage;
//
//	//if(lpStage->flgShutter){
//	//	lpStage->xVelMax = lpStage->xVelFine;
//	//	lpStage->yVelMax = lpStage->yVelFine;
//	//	lpStage->zVelMax = lpStage->zVelFine;
//	//}else{
//	//	lpStage->xVelMax = lpStage->xVelCoarse;
//	//	lpStage->yVelMax = lpStage->yVelCoarse;
//	//	lpStage->zVelMax = lpStage->zVelCoarse;
//	//}
//
//	return TRUE;
//}

DWORD UpdateVelocity(LASERMICROSCOPE *lpMicroscope)
{
	if(!lpMicroscope){
		return FALSE;
	}

	LM_SC10 *lpSC10 = lpMicroscope->lpDevice->lpSC10;
	LM_STAGE *lpStage = lpMicroscope->lpStage;

	if(lpSC10->flgShutter){
		lpStage->xVelMax = lpStage->xVelFine;
		lpStage->yVelMax = lpStage->yVelFine;
		lpStage->zVelMax = lpStage->zVelFine;
	}else{
		lpStage->xVelMax = lpStage->xVelCoarse;
		lpStage->yVelMax = lpStage->yVelCoarse;
		lpStage->zVelMax = lpStage->zVelCoarse;
	}

	return TRUE;
}

DWORD CreateDisplayObjects(LASERMICROSCOPE *lpMicroscope)
{
	if(!lpMicroscope){
		return FALSE;
	}

	LM_XBOX *lpXBox = lpMicroscope->lpDevice->lpXBox;
	LM_STAGE *lpStage = lpMicroscope->lpStage;
	LM_SYSTEM *lpSystem = lpMicroscope->lpSystem;
	LM_DISPLAY *lpDisplay = lpMicroscope->lpDisplay;

	Clean(&lpDisplay->os);

	CIRCLE cTemp;
	CIRCLE c;
	c.r = 200;
	c.x = lpDisplay->wDisplay/2;
	c.y = lpDisplay->hDisplay/2; 

	
	Append(&lpDisplay->os, c, 2, ColorR(0, 0, 255), 0.25, NULL);

	c.r = lpDisplay->roiDiameter*lpDisplay->pixPerUm*0.5;
	c.x = lpDisplay->wDisplay/2;
	c.y = lpDisplay->hDisplay/2;

	if(lpMicroscope->lpDevice->lpSC10->flgShutter){
		Append(&lpDisplay->os, c, 4, ColorR(255, 0, 0), 0.25, NULL);
	}else{
		Append(&lpDisplay->os, c, 4, ColorR(0, 0, 255), 0.25, NULL);
	}

	FLOAT xPol = lpStage->xPolStage;
	//FLOAT yPol = -lpStage->yPolStage;
	FLOAT yPol = -lpStage->yPolStage;
	FLOAT zPol = -lpStage->zPolStage;

	FLOAT delZ;
	FLOAT delXY;
	FLOAT norm;
	VECTOR v;
	TEXTR t = {0};
	FPOINT ptCentroid;
	ptCentroid.x = c.x;
	ptCentroid.y = c.y;

	DOUBLE_3DPOINT dptCurrent;
	DOUBLE_3DPOINT dptX;
	DOUBLE_3DPOINT dptY;
	DOUBLE_3DPOINT dptA;
	DOUBLE_3DPOINT dptB;

	memcpy(&dptX, &lpStage->dptX, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptY, &lpStage->dptY, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptA, &lpStage->dptA, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptB, &lpStage->dptB, sizeof(DOUBLE_3DPOINT));
	memcpy(&dptCurrent, &lpStage->dptCurrent, sizeof(DOUBLE_3DPOINT));

	dptX.x = xPol*dptX.x;
	dptY.x = xPol*dptY.x;
	dptA.x = xPol*dptA.x;
	dptB.x = xPol*dptB.x;
	dptCurrent.x = xPol*dptCurrent.x;

	dptX.y = yPol*dptX.y;
	dptY.y = yPol*dptY.y;
	dptA.y = yPol*dptA.y;
	dptB.y = yPol*dptB.y;
	dptCurrent.y = yPol*dptCurrent.y;

	dptX.z = zPol*dptX.z;
	dptY.z = zPol*dptY.z;
	dptA.z = zPol*dptA.z;
	dptB.z = zPol*dptB.z;
	dptCurrent.z = zPol*dptCurrent.z;

	Scale(dptX, &dptX, 1000);
	Scale(dptY, &dptY, 1000);
	Scale(dptA, &dptA, 1000);
	Scale(dptB, &dptB, 1000);
	Scale(dptCurrent, &dptCurrent, 1000);

	//This helps filter the stage position a bit
	//It looks funny when displaying a high precision position on screen due to pixel rounding
	Truncate(dptX, &dptX);
	Truncate(dptY, &dptY);
	Truncate(dptA, &dptA);
	Truncate(dptB, &dptB);
	Truncate(dptCurrent, &dptCurrent);

	FLOAT xVel = -1.0*lpStage->xPolStage*lpStage->xVel;
	FLOAT yVel = -1.0*lpStage->yPolStage*lpStage->yVel;
	FLOAT zVel = -1.0*lpStage->zPolStage*lpStage->zVel;


	if(lpStage->flgMoveTo){
		xVel = -1.0*xVel;
		yVel = -1.0*yVel;
		zVel = lpStage->zVelMax;
	}

	if(lpDisplay->flgXPosition){
		GetUNO32NavigationVector(dptCurrent, dptX, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptX);
		delZ = GetZDisplacement(dptCurrent, dptX);

		sprintf(t.str, "X(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpDisplay->os, v, 2, ColorR(0, 0, 255), 0.5, NULL);
		Append(&lpDisplay->os, t, 2, ColorR(0, 0, 255), 0.5, NULL);
	}

	if(lpDisplay->flgYPosition){
		GetUNO32NavigationVector(dptCurrent, dptY, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptY);
		delZ = GetZDisplacement(dptCurrent, dptY);

		sprintf(t.str, "Y(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpDisplay->os, t, 2, ColorR(255, 255, 0), 1.0, NULL);
		Append(&lpDisplay->os, v, 2, ColorR(255, 255, 0), 0.5, NULL);
	}

	if(lpDisplay->flgAPosition){
		GetUNO32NavigationVector(dptCurrent, dptA, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptA);
		delZ = GetZDisplacement(dptCurrent, dptA);

		sprintf(t.str, "A(%d, %d)", (INT)delXY, (INT)delZ);
		Append(&lpDisplay->os, t, 2, ColorR(0, 255, 0), 1.0, NULL);
		Append(&lpDisplay->os, v, 2, ColorR(0, 255, 0), 0.5, NULL);
	}

	if(lpDisplay->flgBPosition){
		GetUNO32NavigationVector(dptCurrent, dptB, ptCentroid, &v);
		t.pt.x = v.pTail.x;
		t.pt.y = v.pTail.y;

		delXY = GetXYDisplacement(dptCurrent, dptB);
		delZ = GetZDisplacement(dptCurrent, dptB);

		sprintf(t.str, "B(%d,%d)", (INT)delXY, (INT)delZ);
		Append(&lpDisplay->os, v, 2, ColorR(255, 0, 0), 0.5, NULL);
		Append(&lpDisplay->os, t, 2, ColorR(255, 0, 0), 0.5, NULL);
	}

	if(GetVelocityVector(lpDisplay, xVel, -yVel, &v, &t)){
		ShiftVector(ptCentroid, &v);
		Append(&lpDisplay->os, v, 2, ColorR(255, 255, 255), 0.5, NULL);
		t.pt.x+=ptCentroid.x;
		t.pt.y+=ptCentroid.y;
		Append(&lpDisplay->os, t, 2, ColorR(255, 255, 255), 1.0, NULL);
	}

	if(zVel){
		sprintf(t.str, "%d", (INT)zVel); 
		t.pt.x=ptCentroid.x-5;
		t.pt.y=ptCentroid.y+30;
		Append(&lpDisplay->os, t, 2, ColorR(255, 0, 255), 1.0, NULL);
	}
	//INT i;
	//for(i = 0; i < lpStage->pSet->nPoint; i++){
	//	P3D p;
	//	Copy(&p, lpStage->dptCurrent);

	//	DOUBLE distUm = GetXYDisplacement(p, lpStage->pSet->p[i]);
	//	DOUBLE distPixel = 


	//}

	if(GetZAxisIndicator(lpStage, &c, &t)){
		Append(&lpDisplay->os, c, 2, ColorR(255, 255, 255), 0.5, NULL);
	}


	return TRUE;
}
DWORD UpdateInflightVelocity(LASERMICROSCOPE *lpMicroscope)
{
	if(!lpMicroscope)
		return FALSE;

	LM_DEVICE *lpDevice = lpMicroscope->lpDevice;
	LM_STAGE *lpStage = lpMicroscope->lpStage;
	LM_XBOX *lpXBox = lpMicroscope->lpDevice->lpXBox;

	//Ensure the microscope is in auto-pilot
	if(!lpStage->flgMoveTo){
		return TRUE;
	}

	VECTOR v;
	GetVector(lpStage->dptMoveTo, lpStage->dptCurrent, &v);
	lpStage->xVel = lpStage->xVelMax*cos((v.dir*PI)/180.0);
	lpStage->yVel = lpStage->yVelMax*sin((v.dir*PI)/180.0);

	if(lpXBox->flgDPadLeft){
		A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptMoveTo.y, abs(lpStage->yVel*0.001));
		A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptMoveTo.x, abs(lpStage->xVel*0.001));
		lpXBox->flgDPadLeft = FALSE;
	}

	if(lpXBox->flgDPadRight){
		A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_00, lpStage->dptMoveTo.y, abs(lpStage->yVel*0.001));
		A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_01, lpStage->dptMoveTo.x, abs(lpStage->xVel*0.001));
		lpXBox->flgDPadRight = FALSE;
	}

	if(lpStage->flgMoveToZ){
		if(lpXBox->flgDPadUp){
			A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptMoveTo.z, abs(lpStage->zVelMax*0.001));
			lpXBox->flgDPadUp = FALSE;
		}	

		if(lpXBox->flgDPadDown){
			A3200MotionMoveAbs(lpDevice->hAero, TASKID_Library, AXISINDEX_02, lpStage->dptMoveTo.z, abs(lpStage->zVelMax*0.001));
			lpXBox->flgDPadDown = FALSE;
		}
	}

	return TRUE;
}
