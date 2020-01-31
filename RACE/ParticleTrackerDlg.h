#include "stdafx.h"

#include "ParticleTracker.h"
#include "Limit.h"

typedef struct PARTICLETRACKERDLG_{
	HWND hwnd;
	HWND hOwner;
	HWND hParticleTracker;
	HWND hListResources;
	HWND hEditUpper;
	HWND hEditLower;
	HWND hBtnCapture;

	HWND hEditMaxAngle;
	HWND hEditMaxDelta;
	HWND hEditMaxVelocity;

	FLOAT maxAngle;
	FLOAT maxDelta;
	FLOAT maxVelocity;

	LIMIT limDiameter;
	LIMIT limVelocity;

	PARTICLETRACKER *lpParticleTracker;

	DWORD flgResource;

}PARTICLETRACKERDLG;

INT_PTR CALLBACK ParticleTrackerDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);