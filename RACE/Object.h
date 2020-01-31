#pragma once

#include "stdafx.h"
#include "Text.h"
//Defines drawable objects

#define OBJ_POINT		100
#define OBJ_LINE		101
#define OBJ_LINESET		102
#define OBJ_CIRCLE		103
#define OBJ_TEXT		104
#define OBJ_VECTOR		105

typedef struct OBJECT_{
	DWORD id;
	DWORD size;
	INT width;		//Drawing Width
	DWORD style;		//Drawing Style
	COLOR color;	
	FLOAT alpha;
	LPVOID *lpObject;
}OBJECT;

typedef struct OBJECTSET_{
	DWORD size;
	DWORD nObject;
	OBJECT *object;
}OBJECTSET;


DWORD SizeOf(DWORD id);
DWORD Alloc(OBJECT *object, DWORD id);
DWORD Free(OBJECT *object);
DWORD Clear(OBJECTSET *os);
DWORD Append(OBJECTSET *os, CIRCLE line, INT width, COLOR color, FLOAT alpha, DWORD style);
DWORD Append(OBJECTSET *os, LINE line, INT width, COLOR color, FLOAT alpha, DWORD style);
DWORD Append(OBJECTSET *os, VECTOR vector, INT width, COLOR color, FLOAT alpha, DWORD style);
DWORD Append(OBJECTSET *os, TEXTR text, INT width, COLOR color, FLOAT alpha, DWORD style);
DWORD Copy(OBJECT *oDst, OBJECT *oSrc);


DWORD Alloc(OBJECTSET *os, DWORD size);
DWORD Clean(OBJECTSET *os);
DWORD Copy(OBJECTSET *osDest, OBJECTSET osSrc);


DWORD DrawObjectSet(DWORD w, DWORD h, PIXEL *img, OBJECTSET os);
DWORD DrawObjectSet(HDC hdc, OBJECTSET os);

DWORD ScaleObjectSet(OBJECTSET *os, FLOAT scale);

DWORD DrawRect(HDC hdc, RECT rect/*, COLOR c*/);