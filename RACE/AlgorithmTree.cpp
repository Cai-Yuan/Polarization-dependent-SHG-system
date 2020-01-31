#include "stdafx.h"

#include "AlgorithmTree.h"

//VERY UNTESTED!!!!!
//4.25.13

DWORD CreateNode(NODE *nodeParent,  NODE *node)
{
	node->nNode = NULL;
	node->node = NULL;
	//node->parent = (NODE *) calloc(1, sizeof(NODE));
	node->parent = nodeParent;

	memset(&node->str, NULL, MAX_STRING*sizeof(CHAR));

	return TRUE;
}

//DWROD CreateNodeChild(NODE *nodeParent, NODE *nodeChild)
//{
//	node->nNode = NULL;
//	node->node = NULL;
//	node->parent = nodeParent;
//	memset(&node->str, NULL, MAX_STRING*sizeof(CHAR));
//}

NODE* GetParent(NODE node){

	return node.parent;
}

DWORD NodeCreateTest(DWORD nTiers, DWORD nChildPerTier)
{

	DWORD i = NULL;
	DWORD p = NULL;

	NODE nodeRoot = {0};
	NODE *nodeChild = (NODE *) calloc(nChildPerTier, sizeof(NODE));

	CreateNode(NULL, &nodeRoot);

	for(p = 0; p < nTiers; p++){

		CreateNode(&nodeRoot, &nodeChild[p]);
	}

	if(nodeChild) free(nodeChild);

	return nChildPerTier;

}