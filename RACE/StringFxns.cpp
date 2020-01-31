#include "stdafx.h"

#include "StringFxns.h"
#include "stdlib.h"

DWORD CharReplace(CHAR *str, CHAR no, CHAR yes)
{
	INT i = NULL;
	INT n = NULL;

	if(!str)
		return FALSE;

	DWORD len = strlen(str);

	for(i = 0; i < len; i++){

		if(str[i] == no){
			str[i] = yes;
			n++;
		}

	}

	return n;
}

DWORD Append(STRINGSET *ss, CHAR *str)
{
	if(!ss || !str)
		return FALSE;

	strcpy(ss->string[ss->nStr].str, str);
	ss->nStr++;

	return TRUE;
}

DWORD Alloc(STRINGSET *ss, DWORD strLen, DWORD nStrings)
{
	return CallocStringSet(nStrings, strLen, ss);
}

DWORD CallocStringSet(DWORD nStrings, DWORD strLen, STRINGSET *strSet)
{

	INT i = NULL;
	if(!strSet)
		return FALSE;

	strSet->string = (STRING *) calloc(nStrings, sizeof(STRING));
	strSet->length = nStrings;
	//strSet->nStr = nStrings;
	strSet->nStr = 0;

	for(i = 0; i < nStrings; i++){

		strSet->string[i].str = (CHAR *) calloc(strLen, sizeof(CHAR));
		strSet->string[i].nChar = strLen;

	}
	

	return nStrings;
}

DWORD FreeStringSet(STRINGSET *strSet)
{
	INT i = NULL;

	if(!strSet){
		return FALSE;
	}

	for(i = 0; i < strSet->nStr; i++){

		if(strSet->string[i].str){
			free(strSet->string[i].str);
			strSet->string[i].str = NULL;
			strSet->string[i].nChar = NULL;
		}

	}

	strSet->nStr = NULL;

	return TRUE;
}

DWORD MemcpyStringSet(STRINGSET *strSetDest, STRINGSET *strSetSrc)
{
	INT i = NULL;

	if(!strSetDest || !strSetSrc)
		return FALSE;

	for(i = 0; i < strSetSrc->nStr; i++){

		memcpy(strSetDest->string[i].str, strSetSrc->string[i].str, strSetSrc->string[i].nChar*sizeof(CHAR));
		strSetDest->string[i].nChar = strSetSrc->string[i].nChar;
	}

	strSetDest->nStr = strSetSrc->nStr;

	return TRUE;

}

DWORD ReadStringTo(CHAR *strIn, CHAR *strRead, CHAR c)
{
	DWORD i = NULL;
	DWORD p = NULL;
	DWORD strLen = NULL;
	DWORD nRead = NULL;
	
	if(!strIn || !strRead)
		return FALSE;

	strLen = strlen(strIn);

	for(i = 0; i < strLen; i++){
		if(strIn[i] != c){
			strRead[p] = strIn[i];
			p++;
			
		}
		else
			break;
	}

	nRead = p;

	return nRead;
}

DWORD ReadStringWithin(CHAR *str, CHAR *strIn, CHAR cStart, CHAR cEnd)
{
	DWORD i = NULL;
	DWORD p = NULL;
	DWORD strLen = NULL;
	DWORD nRead = NULL;
	BOOL fRead = NULL;


	if(!str || !strIn)
		return FALSE;

	strLen = strlen(str);

	for(i = 0; i < strLen; i++){

		if(str[i] == cStart){
			i++;
			for(p = 0; i < strLen; i++, p++){
				if(str[i] == cEnd)
					return p;
				strIn[p] = str[i];	
			}
		}

	}

	nRead = p;
	return nRead;
}

DWORD ReadStringParameters(CHAR *str, DWORD nChar, FLOAT *param)
{
	INT i = NULL;
	INT p = NULL;
	DWORD nParams = NULL;
	DWORD size = sizeof(CHAR);

	if(!str || !param)
		return FALSE;

	CHAR strParam[MAX_STRING];
	memset(strParam, NULL, MAX_STRING*sizeof(CHAR));
	//CHAR *strParam = (CHAR *)calloc(nChar, sizeof(CHAR));


	for(i = 0; i < nChar; i++){

		if(!str[i])
			break;

		if(str[i] > 45 && str[i] < 58){
			if(str[i] != 47){

				for(i = i; i < nChar; i++){
					if(str[i] > 45 && str[i] < 58){
						if(str[i] != 47){
							strParam[p] = str[i];
							p++;
						}
					}
				else{
					sscanf(strParam, "%f", (param + nParams));
					memset(strParam, NULL, nChar*sizeof(CHAR));
					nParams++;
					p=0;
					break;
					}
				}
			}
		}

	}
		


	//if(strParam) free(strParam);
	return nParams;
}


DWORD RemoveStringWithin(CHAR *str, DWORD nChar, CHAR from, CHAR to)
{
	INT i = NULL;
	INT p = NULL;
	BOOL fRemove = NULL;
	//CHAR *strTemp = NULL;
	CHAR strTemp[MAX_STRING];

	if(!str)
		return FALSE;

	//strTemp = (CHAR *) malloc(nChar*sizeof(CHAR));
	memset(strTemp, NULL, nChar*sizeof(CHAR));
	//strTemp = (CHAR *) calloc(nChar, sizeof(CHAR));
	memcpy(strTemp, str, nChar*sizeof(CHAR));

	for(i = 0; i < nChar; i++){
		
		if(str[i] == from){
			fRemove = TRUE;
		}
		if(str[i] == to){
			fRemove = FALSE;
			i++;
		}

		if(!fRemove){
			strTemp[p] = str[i];
			p++;
		}
	}

	memset(str, NULL, nChar*sizeof(CHAR));
	memcpy(str, strTemp, p*sizeof(CHAR));

	//if(strTemp) free(strTemp);

	return p;
}

DWORD ReadCommaDelimitedStrings(CHAR *str, DWORD nChar, STRINGSET *strSet)
{
	DWORD i = NULL;
	DWORD p = NULL;
	DWORD iStr = NULL;
	DWORD nStr = NULL;
	DWORD nParam = NULL;
	CHAR strBracket[MAX_STRING];

	if(!str || !strSet)
		return FALSE;

	if(str[i]){
		nParam = GetNumberObserved(str, MAX_STRING, ',');
		nStr = nParam+1;

		CallocStringSet(nStr, MAX_STRING, strSet);

		
		for(i = 0; i < nChar; i++){

			if(!str[i])
				break;

			//Load String Up To ','
			for(p = 0; p < strSet->string[iStr].nChar; p++){

				if(str[i+p] == ',' || !str[i+p]){
					iStr++;
					i = i+p;
					break;
				}

				strSet->string[iStr].str[p] = str[i+p];				
			}

		}

		for(i = 0; i < nStr; i++){
			strSet->string[i].str[0] = strSet->string[i].str[0];
		}
	}
	
	return nStr;
}

DWORD GetNumberObserved(CHAR *str, DWORD nChar, CHAR obs)
{
	DWORD nObserved = NULL;
	DWORD i = NULL;

	for(i = 0; i < nChar; i++){
		if(str[i] == obs)
			nObserved++;
	}

	return nObserved;
}

//Removes the last character in a string
DWORD RemoveStringTermination(CHAR *str, CHAR *strOut)
{
	DWORD strLen = NULL;
	if(!str || !strOut){
		return FALSE;
	}

	strLen = strlen(str);

	memcpy(strOut, str, strLen*sizeof(CHAR));

	strOut[strLen] = NULL;


	return TRUE;
}

//Removes the last character in a string if equal to 'cRemove'
//Otherwhise string is not modified
DWORD RemoveStringTermination(CHAR *str, CHAR *strOut, CHAR cRemove)
{
	DWORD strLen = NULL;
	if(!str || !strOut){
		return FALSE;
	}

	strLen = strlen(str);

	memcpy(strOut, str, strLen*sizeof(CHAR));

	if(strOut[strLen-1] == cRemove){
		strOut[strLen-1] = NULL;
	}


	return TRUE;
}

DWORD GetRootPath(CHAR *str, CHAR *strPath, DWORD length)
{
	if(!str || !strPath)
		return FALSE;

	DWORD i;
	DWORD pos = 0;

	for(i = 0; i < length; i++){
		//if(str[i] == "\" )
		if(str[i] == 92)
			pos = i;
	}

	memcpy(strPath, str, pos*sizeof(CHAR));
	strPath[pos] = 0;

	return TRUE;
}
