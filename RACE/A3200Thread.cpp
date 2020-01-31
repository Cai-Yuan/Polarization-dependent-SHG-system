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

#include "A3200.h"
#include "A3200Stage.h"
#include "A3200ErrorCode.h"

DWORD UpdateInflightVelocity(WNDUNO32 *lpWndUNO32);

DWORD WINAPI A3200Thread(LPVOID param)
{

	WNDUNO32 *lpWndUNO32 = (WNDUNO32 *)param;

	if(!lpWndUNO32)
		return FALSE;

	CHAR str[MAX_STRING];
	FLOAT prd = ((FLOAT) 1000 / (FLOAT)lpWndUNO32->sampleRate);
	DWORD tUpdate = clock()+prd;
	clock_t cTime = NULL;
	ErrorData errorData;

	while(lpWndUNO32->flgThread){

		if(!lpWndUNO32->hAero){

			A3200Connect(&lpWndUNO32->hAero);

			if(lpWndUNO32->hAero){
				A3200MotionDisable(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
				A3200MotionFaultAck(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
				if(!A3200MotionEnable(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02)){
					MessageBox(NULL, "Drivers Not Enabled!", "Drivers Not Enabled!", MB_OK);
				}
			}
		}

		errorData = A3200GetLastError();
		if(errorData.Code == ErrorCode_CommunicationLost){
			A3200Disconnect(&lpWndUNO32->hAero);
			lpWndUNO32->hAero = NULL;
		}


		prd = ((FLOAT) 1000 / (FLOAT)lpWndUNO32->sampleRate);
		cTime = clock();
		if(cTime >= tUpdate){
			tUpdate = tUpdate+prd;

			GetA3200Position(lpWndUNO32->hAero, &lpWndUNO32->dptCurrent);
			UpdateUNO32PositionDisplay(lpWndUNO32->dptCurrent, lpWndUNO32);

			//if(lpWndUNO32->flgMoveTo && !lpWndUNO32->flgZDelay){
			if(lpWndUNO32->flgMoveTo){
				if(!lpWndUNO32->flgZDelay){
				//Remove the MoveToFlag
					if(Compare(lpWndUNO32->dptCurrent, lpWndUNO32->dptMoveTo)){
						lpWndUNO32->xVel = 0;
						lpWndUNO32->yVel = 0;
						lpWndUNO32->flgMoveTo = FALSE;
					}
				}

				if(lpWndUNO32->flgZDelay){
					if(!lpWndUNO32->flgMoveToZ){	
						if(CompareXY(lpWndUNO32->dptCurrent, lpWndUNO32->dptMoveTo)){
							A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->dptMoveTo.z, abs(lpWndUNO32->zVelMax*0.001));
							lpWndUNO32->xVel = 0;
							lpWndUNO32->yVel = 0;
							lpWndUNO32->flgMoveToZ = TRUE;
							lpWndUNO32->zVel = lpWndUNO32->zVelMax;
						}
					}
				}

				if(lpWndUNO32->flgMoveToZ){
					if(CompareZ(lpWndUNO32->dptCurrent, lpWndUNO32->dptMoveTo)){
						lpWndUNO32->flgMoveTo = FALSE;
						lpWndUNO32->flgMoveToZ = FALSE;
						lpWndUNO32->zVel = 0;
						UpdateUNO32Velocity(lpWndUNO32);
					}
				}


			}	

			CreateUNO32Objects(lpWndUNO32);
 
			SendMessage(lpWndUNO32->hDataView, WM_SETOBJECT, NULL, (LPARAM)&lpWndUNO32->os);
		}

		if(lpWndUNO32->flgInitMoveTo){

			lpWndUNO32->flgMoveTo = TRUE;

			VECTOR v;
			GetVector(lpWndUNO32->dptMoveTo, lpWndUNO32->dptCurrent, &v);
			//lpWndUNO32->xVel = lpWndUNO32->xVelMax*cos((v.dir*PI)/180.0);
			//lpWndUNO32->yVel = lpWndUNO32->yVelMax*sin((v.dir*PI)/180.0);
			lpWndUNO32->xVel = lpWndUNO32->xVelMax*cos((v.dir*PI)/180.0)*lpWndUNO32->xPolStage;
			lpWndUNO32->yVel = lpWndUNO32->yVelMax*sin((v.dir*PI)/180.0)*lpWndUNO32->yPolStage;
				
				if(lpWndUNO32->xVelMax != lpWndUNO32->xVelCoarse){
					A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, lpWndUNO32->dptMoveTo.y, abs(lpWndUNO32->yVel*0.001));
					A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, lpWndUNO32->dptMoveTo.x, abs(lpWndUNO32->xVel*0.001));

				}else{
					lpWndUNO32->zVel = lpWndUNO32->zVelMax;
					
					A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, lpWndUNO32->dptMoveTo.y, abs(lpWndUNO32->yVel*0.001));
					A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, lpWndUNO32->dptMoveTo.x, abs(lpWndUNO32->xVel*0.001));
					A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->dptMoveTo.z, abs(lpWndUNO32->zVel*0.001));
				}

			lpWndUNO32->flgInitMoveTo = FALSE;

		}


		if(!lpWndUNO32->flgMoveTo){
			if(lpWndUNO32->flgXInput || lpWndUNO32->flgYInput){

				FLOAT xVel;
				FLOAT yVel;
				FLOAT xStick = lpWndUNO32->xStick;
				FLOAT yStick = lpWndUNO32->yStick;
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

				//lpWndUNO32->xVel = -1.0*(abs(v.i)+abs(v.j))*lpWndUNO32->xVelMax*cos(anglRad);
				//lpWndUNO32->yVel = (abs(v.i)+abs(v.j))*lpWndUNO32->yVelMax*sin(anglRad);
				lpWndUNO32->xVel = (abs(v.i)+abs(v.j))*lpWndUNO32->xVelMax*cos(anglRad);
				lpWndUNO32->yVel = (abs(v.i)+abs(v.j))*lpWndUNO32->yVelMax*sin(anglRad);
				xVel = (abs(v.i)+abs(v.j))*lpWndUNO32->xVelMax*cos(anglRad)*lpWndUNO32->xPolStage;
				yVel = (abs(v.i)+abs(v.j))*lpWndUNO32->yVelMax*sin(anglRad)*lpWndUNO32->yPolStage;

				A3200MotionFreeRun(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, xVel*0.001);
				A3200MotionFreeRun(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, yVel*0.001);

				Edit_SetFloat(lpWndUNO32->hEditXVel, lpWndUNO32->xVel/**lpWndUNO32->xScale*/);
				Edit_SetFloat(lpWndUNO32->hEditYVel, lpWndUNO32->yVel/**lpWndUNO32->xScale*/);
				
				lpWndUNO32->flgXInput = FALSE;
				lpWndUNO32->flgYInput = FALSE;
			}

			if(lpWndUNO32->flgZInput){

				//lpWndUNO32->zVel = lpWndUNO32->zStick*lpWndUNO32->zVelMax;
				lpWndUNO32->zVel = lpWndUNO32->zStick*lpWndUNO32->zVelMax*lpWndUNO32->zPolStage;
				Edit_SetFloat(lpWndUNO32->hEditZVel, lpWndUNO32->zVel/**lpWndUNO32->xScale*/);
				A3200MotionFreeRun(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->zVel*0.001);
				lpWndUNO32->flgZInput = FALSE;
			}

		}

		if(lpWndUNO32->flgAButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				//MessageBox(lpWndUNO32->hDataView, "Override Saved Position?", "Position Manager", MB_ICONEXCLAMATION|MB_YESNO);
				GetA3200Position(lpWndUNO32->hAero, &lpWndUNO32->dptA);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->dptMoveTo, &lpWndUNO32->dptA, sizeof(DOUBLE_3DPOINT));
				lpWndUNO32->flgInitMoveTo = TRUE;
				lpWndUNO32->flgLeftShoulder = NULL;
			}
			lpWndUNO32->flgAButton = NULL;
			lpWndUNO32->flgAPosition = TRUE;
		}

		if(lpWndUNO32->flgBButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpWndUNO32->hAero, &lpWndUNO32->dptB);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->dptMoveTo, &lpWndUNO32->dptB, sizeof(DOUBLE_3DPOINT));
				lpWndUNO32->flgInitMoveTo = TRUE;
				lpWndUNO32->flgLeftShoulder = NULL;
			}
			lpWndUNO32->flgBButton = NULL;
			lpWndUNO32->flgBPosition = TRUE;
		}
		if(lpWndUNO32->flgXButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpWndUNO32->hAero, &lpWndUNO32->dptX);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->dptMoveTo, &lpWndUNO32->dptX, sizeof(DOUBLE_3DPOINT));
				lpWndUNO32->flgInitMoveTo = TRUE;
				lpWndUNO32->flgLeftShoulder = NULL;
			}
			lpWndUNO32->flgXButton = NULL;
			lpWndUNO32->flgXPosition = TRUE;
		}

		if(lpWndUNO32->flgYButton == XB_BUTTON_PRESSED){
			if(lpWndUNO32->flgRightShoulder == XB_BUTTON_DOWN){
				GetA3200Position(lpWndUNO32->hAero, &lpWndUNO32->dptY);
				lpWndUNO32->flgRightShoulder = NULL;
			}
			if(lpWndUNO32->flgLeftShoulder == XB_BUTTON_DOWN){
				memcpy(&lpWndUNO32->dptMoveTo, &lpWndUNO32->dptY, sizeof(DOUBLE_3DPOINT));
				lpWndUNO32->flgInitMoveTo = TRUE;
			}
			lpWndUNO32->flgYButton = NULL;
			lpWndUNO32->flgYPosition = TRUE;
		}

		if(lpWndUNO32->flgBackButton || lpWndUNO32->flgLeftTrigger){
			if(lpWndUNO32->flgMoveTo){
				memcpy(&lpWndUNO32->dptMoveTo, &lpWndUNO32->dptCurrent, sizeof(UNO32_3DPOINT));
				A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, lpWndUNO32->dptMoveTo.y, abs(lpWndUNO32->yVelMax*0.001));
				A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, lpWndUNO32->dptMoveTo.x, abs(lpWndUNO32->xVelMax*0.001));
				A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->dptMoveTo.z, abs(lpWndUNO32->zVelMax*0.001));
				lpWndUNO32->flgBackButton = NULL;
			}
			lpWndUNO32->flgLeftTrigger = NULL;
		}

		if(lpWndUNO32->flgStartButton){
			A3200Connect(&lpWndUNO32->hAero);
			A3200MotionFaultAck(lpWndUNO32->hAero, TASKID_Library, AXISMASK_00|AXISMASK_01|AXISMASK_02);
			lpWndUNO32->flgStartButton = NULL;
		}

		if(lpWndUNO32->flgRightTrigger){
			DataView_SnapFrame(lpWndUNO32->hDataView);
			ShowWindow(lpWndUNO32->hDataView, SW_SHOW);
		}

		UpdateInflightVelocity(lpWndUNO32);

		if(lpWndUNO32->flgZero){
			sprintf(str, "!SETPOSA%d", 0);
			SerialSend(lpWndUNO32->hDevice, str, NULL);
			sprintf(str, "!SETPOSB%d", 0);
			SerialSend(lpWndUNO32->hDevice, str, NULL);
			sprintf(str, "!SETPOSC%d", 0);
			SerialSend(lpWndUNO32->hDevice, str, NULL);
			lpWndUNO32->flgZero = NULL;
		}

		if(lpWndUNO32->flgZStack){
			long long int zRef = lpWndUNO32->ptZRef.z;
			long long int zGoTo;
			INT slice = lpWndUNO32->slice;
			FLOAT stepSize = lpWndUNO32->stepSize;
			INT w = lpWndUNO32->pdmIn.pdmfh.width;
			INT h = lpWndUNO32->pdmIn.pdmfh.height;
			CHAR str[MAX_STRING];
			PDMFRAME pdmf;
			AllocPDMFrame(&pdmf, w, h, DT_PIXEL);

			sprintf(str, "%s\Slice%d.bmp", lpWndUNO32->strFolder, lpWndUNO32->slice);

			if(lpWndUNO32->slice == 0){
				GetUNO32DevicePosition(lpWndUNO32->hDevice, &lpWndUNO32->ptZRef);
				//memcpy(&lpWndUNO32->ptZRef, &lpWndUNO32->ptCurrent, sizeof(UNO32_3DPOINT));
				SetUNO32DevicePosition(lpWndUNO32->hDevice, 
										(lpWndUNO32->ptCurrent.x),
										(lpWndUNO32->ptCurrent.y),
										(lpWndUNO32->ptCurrent.z));
				lpWndUNO32->driveC.dir = !lpWndUNO32->driveC.dir;
				UpdateDriveDirection(lpWndUNO32->hDevice, -lpWndUNO32->zVel, &lpWndUNO32->driveC);
				SetUNO32DeviceVelocity(lpWndUNO32->hDevice, 
										(10/lpWndUNO32->xScale),
										(10/lpWndUNO32->yScale),
										(10/lpWndUNO32->zScale));
				Sleep(100);
				lpWndUNO32->flgFrameIn = FALSE;
				while(!lpWndUNO32->flgFrameIn){
					Sleep(1);
				}
				CopyPDMFrameUCHARtoPIXEL(&pdmf, &lpWndUNO32->pdmIn);
				SaveBMP(w, h, str, (PIXEL *)pdmf.lpvoid);
			}

			zGoTo = (zRef*lpWndUNO32->zScale) - (slice*stepSize);
			SetUNO32DevicePositionZ(lpWndUNO32->hDevice, (zGoTo/lpWndUNO32->zScale));
			SetUNO32DeviceVelocityZ(lpWndUNO32->hDevice, (100/lpWndUNO32->zScale));
			Sleep(100);
			lpWndUNO32->flgFrameIn = FALSE;
			while(!lpWndUNO32->flgFrameIn){
				Sleep(1);
			}
			CopyPDMFrameUCHARtoPIXEL(&pdmf, &lpWndUNO32->pdmIn);
			SaveBMP(w, h, str, (PIXEL *)pdmf.lpvoid);
			lpWndUNO32->slice++;

			if(lpWndUNO32->slice == lpWndUNO32->nSlice){
				SetUNO32DevicePosition(lpWndUNO32->hDevice, lpWndUNO32->ptZRef);
				SetUNO32DeviceVelocity(lpWndUNO32->hDevice, 
					(300/lpWndUNO32->xScale),
					(300/lpWndUNO32->yScale),
					(300/lpWndUNO32->zScale));
				lpWndUNO32->slice = 0;
				lpWndUNO32->flgZStack = FALSE;
				Sleep(200);
			}

		}


	}

	A3200Disconnect(lpWndUNO32->hAero);


	return FALSE;
}

DWORD UpdateInflightVelocity(WNDUNO32 *lpWndUNO32)
{
	//Ensure the microscope is in auto-pilot
	if(!lpWndUNO32->flgMoveTo){
		return TRUE;
	}

	VECTOR v;
	GetVector(lpWndUNO32->dptMoveTo, lpWndUNO32->dptCurrent, &v);
	lpWndUNO32->xVel = lpWndUNO32->xVelMax*cos((v.dir*PI)/180.0);
	lpWndUNO32->yVel = lpWndUNO32->yVelMax*sin((v.dir*PI)/180.0);

	if(lpWndUNO32->flgDPadLeft){
		A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, lpWndUNO32->dptMoveTo.y, abs(lpWndUNO32->yVel*0.001));
		A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, lpWndUNO32->dptMoveTo.x, abs(lpWndUNO32->xVel*0.001));
		lpWndUNO32->flgDPadLeft = FALSE;
	}

	if(lpWndUNO32->flgDPadRight){
		A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_00, lpWndUNO32->dptMoveTo.y, abs(lpWndUNO32->yVel*0.001));
		A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_01, lpWndUNO32->dptMoveTo.x, abs(lpWndUNO32->xVel*0.001));
		lpWndUNO32->flgDPadRight = FALSE;
	}

	if(lpWndUNO32->flgMoveToZ){
		if(lpWndUNO32->flgDPadUp){
			A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->dptMoveTo.z, abs(lpWndUNO32->zVelMax*0.001));
			lpWndUNO32->flgDPadUp = FALSE;
		}	

		if(lpWndUNO32->flgDPadDown){
			A3200MotionMoveAbs(lpWndUNO32->hAero, TASKID_Library, AXISINDEX_02, lpWndUNO32->dptMoveTo.z, abs(lpWndUNO32->zVelMax*0.001));
			lpWndUNO32->flgDPadDown = FALSE;
		}
	}



	return TRUE;
}