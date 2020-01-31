#pragma once

//#include "CircularBuffer.h"
#include "WndLink.h"
#include "Contour.h"
#include "Limit.h"
#include "Trajectory.h"

#include <vector>

//ParticleTrack Show
#define PTS_STREAM			0
#define PTS_FILTER			2
#define PTS_REFERENCE		4
#define PTS_MASK_LAT		8
#define PTS_MASK_LON		16
#define PTS_TRACKING		32
#define PTS_MASK_CHN		64
#define PTS_MASK_FLOW		128

#define STR_STREAM			"Stream"
#define STR_FILTER			"Filter"
#define STR_REFFRAME		"Reference Frame"
#define STR_MASK_LATGUIDE	"Lateral Guidance Mask"
#define STR_MASK_LONGUIDE	"Longitudinal Guidance Mask"
#define STR_TRACKING		"Tracking"
#define STR_MASK_CHANNEL	"Channel Mask"
#define STR_MASK_FLOW		"Flow Mask"

#define VELOCITY	100
#define DIAMETER	101
#define ANGLE		102
#define DVELOCITY	103	//Delta Velocity

typedef struct TRACKPARAMETERS_{
	FLOAT maxAngle;
	FLOAT maxDelta;
	FLOAT maxVelocity;
}TRACKPARAMETERS;

typedef struct PARTICLETRACKER_{
	HWND hwnd;
	HWND hOwner;
	HWND hDataView;
	HWND hMDI;
	HWND hDlg;

	DWORD nElement;
	UCHAR **imgSet;
	DWORD nReference;		//Stores how many frames should be used for calculating the reference frame

	DWORD w;
	DWORD h;
	
	PDMFRAME pdmf;
	PDMFRAME pdmfIn;
	PDMFRAME pdmfRef;
	PDMFRAME pdmfDisp;
	PDMFRAME pdmfPrev;
	PDMFRAME pdmfINT;
	PDMFRAME pdmfUCHAR;
	PDMFRAME pdmfTemp;
	PDMFRAME pdmfMask;
	PDMFRAME pdmfPIXEL;

	PDMFRAME pdmfSubI;
	PDMFRAME pdmfSubII;
	PDMFRAME pdmfSubIII;
	PDMFRAME pdmfFlow;
	PDMFRAME pdmfFlowINT;

	PDMFRAME pdmfMaskLon;
	PDMFRAME pdmfMaskLat;
	PDMFRAME pdmfMaskChn;

	STRINGSET ssAlgorithm;
	DWORD selProc;

	DWORD iAvg;
	DWORD iSub;
	DWORD iRaw;
	DWORD iTrack;

	DWORD flgDisp;
	DWORD flgGetRef;
	DWORD flgTrack;
	DWORD flgShow;
	DWORD flgBlock;
	DWORD flgGetFlow;

	WNDLINKSET wls;

	CONTOURSET cs;

	FELLIPSESET es;
	FELLIPSESET es0;
	FELLIPSESET es1;
	FELLIPSESET es2;

	LIMIT limDiameter;
	LIMIT limVelocity;

	FPOINTSET fpsF0;
	FPOINTSET fpsF1;
	FPOINTSET fpsF2;

	TIMESTAMP tsF0;
	TIMESTAMP tsF1;
	TIMESTAMP tsF2;

	TRAJECTORYSET ts;


	//Coorespondance Parameters
	FLOAT maxAngle;
	FLOAT maxDelta;
	FLOAT maxVelocity;

	XPARTICLESET xps;

}PARTICLETRACKER;


LRESULT CALLBACK ParticleTrackerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD OnCreateParticleTracker(PARTICLETRACKER *lpParticleTracker, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSetFrameSizeParticleTracker(PARTICLETRACKER *pt, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


HWND CreateParticleTracker(HWND hOwner, CHAR *strTitle);
HWND CreateParticleTrackerSystem(HWND hOwner, CHAR *strTitle);

//ParticleTracker Functions
DWORD ParticleTracker_Track(PARTICLETRACKER *lpPT);
DWORD ParticleTracker_Track(PARTICLETRACKER *lpPT, PDMFRAME *pdmf);
DWORD ParticleTracker_SetFrameSize(HWND hwnd, UINT w, UINT h);
DWORD ParticleTracker_Enable(HWND hwnd, DWORD bEnable);
DWORD ParticleTracker_CalculateReference(PARTICLETRACKER *lpPT);
DWORD ParticleTracker_CalculateReference(PARTICLETRACKER *lpPT, DWORD nFrames);
DWORD ParticleTracker_CalculateFlowMask(PARTICLETRACKER *lpPT);

LRESULT OnTrackParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT OnDataInParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT OnDataDropParticleTracker(PARTICLETRACKER *lpPT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT OnInitAlgorithmParticleTrack(PARTICLETRACKER *lpParticleTrack, HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCreateLinkParticleTracker(PARTICLETRACKER *lpParticleTrack, HWND hwnd, WPARAM wParam, LPARAM lParam);

LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, INT *in, UCHAR *out);
LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, UCHAR *in, UCHAR *out);
LRESULT AlgorithmProcINT(UINT w, UINT h, DWORD selProc, STRINGSET ssAlgorithm, INT *in, INT *out);

DWORD BlendSubtract(DWORD w, DWORD h, UCHAR *imgI, UCHAR *imgII, UCHAR *imgOut);