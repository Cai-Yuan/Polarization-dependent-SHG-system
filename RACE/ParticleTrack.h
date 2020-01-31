#pragma once

#include "stdafx.h"

#include "Archive.h"
#include "ImageConversion.h"
#include "TextWindow.h"

//Temporary
#include "DataView.h"
#include "DataManager.h"

#include "DerivativeOperator.h"
#include "Threshold.h"
#include "Ellipse.h"
#include "Draw.h"

DWORD ParticleTrack(HWND hOwner, DATAMANAGER *dataManager, WPARAM wParam, LPARAM lParam);

//DWORD ParticleTrack(HWND hOwner, DATAVIEW *dataView, WPARAM wParam, LPARAM lParam);
