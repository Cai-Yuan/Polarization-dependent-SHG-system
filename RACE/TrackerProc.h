#pragma once

#include "stdafx.h"
#include "Limit.h"
#include "ParticleTracker.h"

typedef struct LM_TRACKER_{
	HWND hOwner;
	HWND hDataView;
	HWND hParticleTracker;

	HWND hListResources;
	HWND hEditUpperDim;
	HWND hEditLowerDim;
	HWND hEditUpperVel;
	HWND hEditLowerVel;
	HWND hEditMaxAngle;
	HWND hEditMaxDelta;
	HWND hEditMaxVelocity;
	HWND hEditNumReference;

	HWND hBtnCapture;
	HWND hBtnReference;

	HWND hCheckEnable;

	FLOAT maxAngle;
	FLOAT maxDelta;
	FLOAT maxVelocity;

	LIMIT limDiameter;

	DWORD flgEnable;
	DWORD flgResource;

	PARTICLETRACKER *lpParticleTracker;

}LM_TRACKER;

//"TrackerProc.cpp"
INT_PTR CALLBACK TrackerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD Tracker_Enable(LM_TRACKER *lpTracker, BOOL bState);