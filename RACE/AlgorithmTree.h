#pragma once

#include "stdafx.h"

typedef struct NODE{
	DWORD nNode;
	NODE *parent;
	NODE *node;
	CHAR str[MAX_STRING];
}NODE;

DWORD CreateNode(NODE *nodeParent, NODE *node);
DWORD NodeCreateTest(DWORD nTiers, DWORD nChildPerTier);