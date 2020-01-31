

#include "stdafx.h"

#include "Particle.h"

DWORD Alloc(XPARTICLESET *xps, DWORD nParticle, DWORD esLen, DWORD tLen)
{

	if(!xps){
		return FALSE;
	}

	xps->size = NULL;
	xps->n = NULL;
	xps->xp = (XPARTICLE *) malloc(nParticle*sizeof(XPARTICLE));

	if(!xps->xp){
		return FALSE;
	}

	Alloc(&xps->xp->es, esLen);
	Alloc(&xps->xp->t, tLen);

	return TRUE;

}