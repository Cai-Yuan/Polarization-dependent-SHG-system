#pragma once
#include "stdafx.h"
#include "Point.h"
#include "Ellipse.h"
#include "Trajectory.h"

#define MAX_RELATIONSHIP 3
#define MAX_PARTICLE	500
#define MAX_TRACK		500

#define WT_ANGLE	0.4
#define WT_DIST		0.6

#define MAX_ANGLE 40
#define MAX_DISTDIFF 50
#define MAXDIST_TFPT 150
#define MAX_DIFFRATIO	5


typedef struct PARTICLEID_{
	UINT frmNum;
	UINT frmID;
}PARTICLEID;

typedef struct FRAMEPARTICLE_{
	UINT nParticle;
	PARTICLEID pID[MAX_PARTICLE];
}FRAMEPARTICLE;


typedef struct PARTICLE_{
	UINT frmNum;
	UINT frmID;
	FPOINT point;
}PARTICLE;

typedef struct PARTICLEGROUP_{
	PARTICLEID groupID;
	UINT frmNum;
	PARTICLE	particleF0;
	PARTICLE	particleF1;
	PARTICLE	particleF2;
}PARTICLEGROUP;


typedef struct PARTICLE0_{
	DWORD id;
	DWORD frame;
	TIMESTAMP ts;
	FELLIPSE fe;
	FLOAT velocity;
	//VECTOR v;
}PARTICLE0;

//'XPARTICLE'
//TODO: Change to a more meaningful name

//'id' represents the number given to the particle
//>>It should be a unique value
//'fStart' stores the first frame the particle was detected in
//'fEnd' stores the last frame the particle was detected in
//'es' is an ellipse set storing the ellipse model of the particle from 'fStart' to 'fEnd'
//'t' is trajectory storing the 'es' centroids from 'fStart' to 'fEnd'

//DESING NOTES:
//'t' is redundant, but is used to reduce computational time
//>>Removes the need to call 'TrajectoryFromEllipseSet()' functions during every itertion
typedef struct XPARTICLE_{
	DWORD id;
	DWORD fStart;
	DWORD fEnd;
	FELLIPSESET es;
	TRAJECTORY t;
}XPARTICLE;

typedef struct XPARTICLESET_{
	DWORD size;
	DWORD n;
	XPARTICLE *xp;
}XPARTICLESET;