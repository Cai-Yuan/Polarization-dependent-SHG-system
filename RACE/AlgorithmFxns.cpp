#include "stdafx.h"

#include "StringFxns.h"
#include "AlgorithmFxns.h"
#include "TreeViewFxns.h"

DWORD GetAlgorithmInputs(CHAR *strIn, DWORD nChar, FLOAT *inputList)
{
	DWORD nInputs = NULL;
	CHAR strInputs[MAX_STRING];
	CHAR strWithin[MAX_STRING];
	STRINGSET ssParams = {0};

	if(!strIn)
		return FALSE;

	memset(strInputs, NULL, MAX_STRING*sizeof(CHAR));
	memset(strWithin, NULL, MAX_STRING*sizeof(CHAR));

	ReadStringWithin(strIn, strWithin, '(', ')');
	ReadCommaDelimitedStrings(strWithin, MAX_STRING, &ssParams);


	FreeStringSet(&ssParams);

	return nInputs;
}

DWORD GetAlgorithmParams(CHAR *strIn, CHAR *strRoot, FLOAT *paramList)
{
	FLOAT *inputList = NULL;
	DWORD nParams = NULL;
	CHAR strParams[MAX_STRING];

	if(!strIn || !strRoot)
		return FALSE;

	memset(strParams, NULL, MAX_STRING*sizeof(CHAR));
	//inputList = (FLOAT *) calloc(MAX_STRING, sizeof(FLOAT));

	ReadStringTo(strIn, strRoot, '('); 
	ReadStringWithin(strIn, strParams, '(', ')');
	nParams = ReadStringParameters(strParams, MAX_STRING, paramList);
	//GetAlgorithmInputs(strIn, MAX_STRING, inputList);

	//if(inputList)free(inputList);
	return nParams;
}

DWORD GetAlgorithmProcedures(HWND hwnd, HTREEITEM hTreeItem, DWORD *selProc, STRINGSET *ss)
{

	*selProc = NULL;

	DWORD i = NULL;
	DWORD nChild = NULL;

	HTREEITEM hRoot = NULL;
	HTREEITEM hParent = NULL;

	CHAR str[MAX_STRING];
	CHAR strIn[MAX_STRING];
	CHAR strSel[MAX_STRING];

	memset(str, NULL, MAX_STRING*sizeof(CHAR));
	memset(strIn, NULL, MAX_STRING*sizeof(CHAR));
	memset(strSel, NULL, MAX_STRING*sizeof(CHAR));

	hRoot = TreeView_GetRoot(hwnd);
	hParent = TreeView_GetParent(hwnd, hTreeItem);


	//Check to see if parent is the root
	if(hParent == NULL)
		hParent = hTreeItem;

	TreeView_GetItemText(hwnd, hParent, str);
	TreeView_GetItemText(hwnd, hTreeItem, strSel);

	//Check for Procedure Dependency
	if(!ReadStringWithin(str, strIn, '[', ']')){
		//Not dependent, so get all of procedures

		nChild = TreeView_GetChildCount(hwnd, hParent);
		CallocStringSet(nChild+1, MAX_STRING, ss);

		TreeView_GetParentChildStrings(hwnd, hParent, ss);
	}
	else{
		ThrowMessage(hwnd, "SubProcedures Not Supported");
	}

	for(i = 0; i < (nChild+1); i++){
		if(ss->string[i].str){
			if(!strcmp(strSel, ss->string[i].str)){
				*selProc = i;
				break;
			}
		}
	}

		
	return nChild;
}

DWORD SortAlgorithmProcedures(DWORD activeProc, STRINGSET *ss)
{

	STRINGSET ssTemp = {0};

	CallocStringSet(ss->nStr, ss->string[0].nChar, &ssTemp);
	MemcpyStringSet(&ssTemp, ss);
	FreeStringSet(&ssTemp);
	

	return TRUE;

}