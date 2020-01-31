#pragma once

#include "stdafx.h"

typedef struct STRING_{
	CHAR *str;
	DWORD nChar;
}STRING;


typedef struct STRINGSET_{
	//STRING *s;
	STRING *string;
	DWORD nStr;
	DWORD length;
}STRINGSET;

//typedef struct STRINGSET_{
//	STRING *string;
//	DWORD nStr;
//}STRINGSET;

DWORD GetRootPath(CHAR *str, CHAR *strPath, DWORD length);

DWORD CharReplace(CHAR *str, CHAR no, CHAR yes);

DWORD Alloc(STRINGSET *ss, DWORD strLen, DWORD nStrings);
DWORD Append(STRINGSET *ss, CHAR *str);
DWORD CallocString(DWORD iString, DWORD strLen, STRINGSET *strSet);
DWORD CallocStringSet(DWORD nStrings, DWORD strLen, STRINGSET *strSet);
DWORD MemcpyStringSet(STRINGSET *strSetDest, STRINGSET *strSetSrc);
DWORD FreeStringSet(STRINGSET *strSet);

DWORD ReadStringTo(CHAR *strIn, CHAR *strRead, CHAR c);
DWORD ReadStringWithin(CHAR *str, CHAR *strIn, CHAR cStart, CHAR cEnd);
DWORD ReadStringParameters(CHAR *str, DWORD nChar, FLOAT *param);
DWORD RemoveStringWithin(CHAR *str, DWORD nChar, CHAR from, CHAR to);

DWORD GetNumberObserved(CHAR *str, DWORD nChar, CHAR obs);
DWORD ReadCommaDelimitedStrings(CHAR *str, DWORD nChar, STRINGSET *strSet);

DWORD RemoveStringTermination(CHAR *str, CHAR *strOut);
DWORD RemoveStringTermination(CHAR *str, CHAR *strOut, CHAR cRemove);
