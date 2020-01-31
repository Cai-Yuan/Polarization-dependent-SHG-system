#pragma once
#include "stdafx.h"
#include "Point.h"
#include "Difference.h"
#include "Particle.h"

UINT GetF0ParticleGroups(UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF0);
UINT GetF2ParticleGroups(UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF2);
VOID GetF0ParticleMinimums(FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle);
VOID GetF2ParticleMinimums(FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle);
UINT TFPT(UINT frame, FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector);



UINT GetF0ParticleGroups(HWND hOutput, UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF0);
UINT GetF2ParticleGroups(HWND hOutput, UINT frame, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, PARTICLEGROUP *pgF2);
VOID GetF0ParticleMinimums(HWND hOutput, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle);
VOID GetF2ParticleMinimums(HWND hOutput, FPOINTSET pF0, FPOINTSET pF1, FPOINTSET pF2, FLOAT *minP0Dist, FLOAT *minP0Angle);
FLOAT GetParticleDistanceDifference(FPOINT pF0, FPOINT pF1, FPOINT pF2);
UINT TFPT(HWND hOutput, UINT frame, FPOINTSET F0, FPOINTSET F1, FPOINTSET F2, VECTOR *vector);
UINT RemoveDuplicateVector(UINT nData, VECTOR *dataIn, VECTOR *dataOut);
