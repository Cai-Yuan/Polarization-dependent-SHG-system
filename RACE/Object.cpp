


#include "stdafx.h"
#include "Object.h"
#include "Text.h"
#include "RACEGDI.h"
//#include "GDIPlus.h"
//#include "Gdiplusgraphics.h"
using namespace Gdiplus;
//#pragma comment(lib, "Gdiplus.lib")

DWORD DrawCircle(Graphics *g, Pen *pen, CIRCLE c);


DWORD SizeOf(DWORD obj)
{
	switch(obj)
	{
		case OBJ_POINT:
			return sizeof(POINT);
		case OBJ_LINE:
			return sizeof(LINE);
		case OBJ_CIRCLE:
			return sizeof(CIRCLE);
		case OBJ_VECTOR:
			return sizeof(VECTOR);
		case OBJ_TEXT:
			return sizeof(TEXTR);
		default:
			return NULL;
	}

	return NULL;
}

DWORD Alloc(OBJECT *o, DWORD id)
{

	DWORD size = SizeOf(id);

	if(o->lpObject)
		return FALSE;

	if(!size)
		return FALSE;

	o->lpObject = (LPVOID *)malloc(size);

	if(!o->lpObject)
		return FALSE;

	memset(o->lpObject, NULL, size);
	o->id = id;
	o->size = size;

	return TRUE;
}

DWORD Free(OBJECT *o)
{
	if(!o)
		return FALSE;

	if(o->lpObject){
		free(o->lpObject);
		o->lpObject = NULL;
		o->size = NULL;
		o->id = NULL;
		o->alpha = NULL;
		memset(&o->color, NULL, sizeof(COLOR));
		return TRUE;
	}

	return FALSE;
}

DWORD Alloc(OBJECTSET *os, DWORD size)
{
	if(!os){
		return FALSE;
	}

	os->nObject = NULL;
	os->size = NULL;
	os->object = (OBJECT *)malloc(size*sizeof(OBJECT));

	if(!os->object)
		return FALSE;

	os->nObject = NULL;
	os->size = size;
	memset(os->object, NULL, size*sizeof(OBJECT));

	return TRUE;
}

DWORD Clean(OBJECTSET *os)
{
	if(!os)
		return FALSE;

	INT i = 0;
	
	for(i = 0; i < os->nObject; i++){
		Free(&os->object[i]);
	}

	os->nObject = NULL;

	return TRUE;
}

DWORD Append(OBJECTSET *os, LINE line, INT width, COLOR color, FLOAT alpha, DWORD style)
{

	if(!os)
		return FALSE;

	if(!os->size)
		return FALSE;

	DWORD index = os->nObject;

	if(index >= os->size)
		return FALSE;

	Alloc(&os->object[index], OBJ_LINE);
	memcpy(os->object[index].lpObject, &line, sizeof(LINE));
	memcpy(&os->object[index].color, &color, sizeof(COLOR));
	os->object[index].alpha = alpha;
	os->object[index].width = width;
	os->object[index].style = style;

	os->nObject = os->nObject + 1;

	return TRUE;
}

DWORD Append(OBJECTSET *os, CIRCLE circle, INT width, COLOR color, FLOAT alpha, DWORD style)
{
	if(!os)
		return FALSE;

	if(!os->size)
		return FALSE;

	DWORD index = os->nObject;

	if(index >= os->size)
		return FALSE;

	Alloc(&os->object[index], OBJ_CIRCLE);
	memcpy(os->object[index].lpObject, &circle, sizeof(CIRCLE));
	memcpy(&os->object[index].color, &color, sizeof(COLOR));
	os->object[index].alpha = alpha;
	os->object[index].width = width;
	os->object[index].style = style;

	os->nObject = os->nObject + 1;

	return TRUE;
}

DWORD Append(OBJECTSET *os, VECTOR vector, INT width, COLOR color, FLOAT alpha, DWORD style)
{
	if(!os)
		return FALSE;

	if(!os->size)
		return FALSE;

	DWORD index = os->nObject;

	if(index >= os->size)
		return FALSE;

	Alloc(&os->object[index], OBJ_VECTOR);
	memcpy(os->object[index].lpObject, &vector, sizeof(VECTOR));
	memcpy(&os->object[index].color, &color, sizeof(COLOR));
	os->object[index].alpha = alpha;
	os->object[index].width = width;
	os->object[index].style = style;

	os->nObject = os->nObject + 1;

	return TRUE;
}

DWORD Append(OBJECTSET *os, TEXTR text, INT width, COLOR color, FLOAT alpha, DWORD style)
{
	if(!os)
		return FALSE;

	if(!os->size)
		return FALSE;

	DWORD index = os->nObject;

	if(index >= os->size)
		return FALSE;

	Alloc(&os->object[index], OBJ_TEXT);
	memcpy(os->object[index].lpObject, &text, sizeof(TEXTR));
	memcpy(&os->object[index].color, &color, sizeof(COLOR));
	os->object[index].alpha = alpha;
	os->object[index].width = width;
	os->object[index].style = style;

	os->nObject = os->nObject + 1;

	return TRUE;
}

DWORD Copy(OBJECT *oDst, OBJECT *oSrc)
{
	if(!oDst || !oSrc)
		return FALSE;

	if(oDst->id != oSrc->id){
		Free(oDst);
		Alloc(oDst, oSrc->id);
	}

	memcpy(&oDst->color, &oSrc->color, sizeof(COLOR));
	memcpy(oDst->lpObject, oSrc->lpObject, oDst->size);
	oDst->alpha = oSrc->alpha;
	oDst->id = oSrc->id;
	oDst->size = oSrc->size;
	oDst->width = oSrc->width;
	oDst->style = oSrc->style;
	

	return TRUE;
}

DWORD Copy(OBJECTSET *osDest, OBJECTSET osSrc)
{

	INT i = 0;
	INT nCopy = osSrc.nObject;

	if(nCopy > osDest->size){
		nCopy = osDest->size;
	}

	osDest->nObject = 0;

	for(i = 0; i < nCopy; i++){
		if(Copy(&osDest->object[i], &osSrc.object[i])){
			osDest->nObject++;
		}
	}


	return TRUE;
}

DWORD Clear(OBJECTSET *os)
{
	if(!os)
		return FALSE;

	os->nObject = 0;

	return TRUE;
}


DWORD DrawObjectSet(DWORD w, DWORD h, PIXEL *img, OBJECTSET os)
{

	INT i = 0;
	CIRCLE circle;
	LINE line;
	VECTOR vector;
	COLORREF cr;


	for(i = 0; i < os.nObject; i++){
		cr = RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b);
		switch(os.object[i].id){
			case OBJ_LINE:
				break;
			case OBJ_CIRCLE:
				memcpy(&circle, os.object[i].lpObject, sizeof(CIRCLE));
				DrawCircle(w, h, circle, img, cr);
				break;
			case OBJ_VECTOR:
				memcpy(&vector, os.object[i].lpObject, sizeof(VECTOR));
				DrawVector(w, h, img, vector, cr);
				break;
			default:
				break;
		}
				
	}

	return TRUE;
}

DWORD DrawRect(HDC hdc, RECT rect/*, COLOR c*/)
{

	LINE line;
	ULONG_PTR token;
	GdiplusStartupInput gdiInput;
	GdiplusStartupOutput gdiOutput;

	GdiplusStartup(&token, &gdiInput, &gdiOutput);
	Graphics *g = new Graphics(hdc);
	Color *c = new Color(255, 0, 255, 255);
	//Color *c = new Color(c.r, c.g, c.b);
	Pen *pen = new Pen(*c, 2);

	c->SetValue(c->MakeARGB(255, 255, 0, 255));
	pen->SetColor(*c);
	pen->SetWidth(2);

	//DrawLeft
	g->DrawLine(pen, rect.left, rect.bottom, rect.left, rect.top);
	//DrawTop
	g->DrawLine(pen, rect.left, rect.top, rect.right, rect.top);
	//DrawRight
	g->DrawLine(pen, rect.right, rect.top, rect.right, rect.bottom);
	//DrawBottom
	g->DrawLine(pen, rect.right, rect.bottom, rect.left, rect.bottom);

	delete g;
	delete c;
	delete pen;

	GdiplusShutdown(token);

	return TRUE;
}

DWORD DrawObjectSet(HDC hdc, OBJECTSET os)
{
	INT i = 0;
	CIRCLE circle;
	LINE line;
	VECTOR vector;
	TEXTR text;
	COLORREF cr;
	HGDIOBJ hOld;
	HPEN hPen;
	HBRUSH hBrush;

	BYTE a;
	BYTE red, green, blue;

	ULONG_PTR token;
	GdiplusStartupInput gdiInput;
	GdiplusStartupOutput gdiOutput;

	GdiplusStartup(&token, &gdiInput, &gdiOutput);
	Graphics *g = new Graphics(hdc);
	Color *c = new Color(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b, 125);
	Pen *pen = new Pen(*c, 2);
	//SolidBrush *solidBrush = new SolidBrush();

	//Graphics g = new Graphics(hdc);

	for(i = 0; i < os.nObject; i++){
		
		
		//SetDCBrushColor(hdc, RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
		//cr = RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b);
		switch(os.object[i].id){
			case OBJ_LINE:
				break;
			case OBJ_CIRCLE:
				c->SetValue(c->MakeARGB(os.object[i].alpha*255, os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
				pen->SetColor(*c);
				pen->SetWidth(os.object[i].width);
				memcpy(&circle, os.object[i].lpObject, sizeof(CIRCLE));
				DrawCircle(g, pen, circle);
				//DrawCircle(hdc, circle, RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
				break;
			case OBJ_VECTOR:
				c->SetValue(c->MakeARGB(os.object[i].alpha*255, os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
				pen->SetColor(*c);
				a = c->GetA();
				red = c->GetR();
				green = c->GetG();
				blue = c->GetB();
				pen->SetWidth(os.object[i].width);

				memcpy(&vector, os.object[i].lpObject, sizeof(VECTOR));
				g->DrawLine(pen, vector.pHead.x, vector.pHead.y, vector.pTail.x, vector.pTail.y);
				//hPen = CreatePen(os.object[i].style, os.object[i].width, RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
				//hOld = SelectObject(hdc, hPen);
				//hOld = SelectObject(hdc, GetStockObject(DC_PEN));
				////SetDCPenColor(hdc, RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
				//memcpy(&vector, os.object[i].lpObject, sizeof(VECTOR));
				//MoveToEx(hdc, vector.pTail.x, vector.pTail.y, NULL);
				//LineTo(hdc, vector.pHead.x, vector.pHead.y);
				//SelectObject(hdc, hOld);
				//DeleteObject(hPen);
				//DrawVector(w, h, img, vector, cr);
				break;
			case OBJ_TEXT:
				//brush->
				memcpy(&text, os.object[i].lpObject, sizeof(TEXTR));
				DrawText(hdc, text, RGB(os.object[i].color.r, os.object[i].color.g, os.object[i].color.b));
				break;
			default:
				break;
		}
				
	}

	delete g;
	delete c;
	delete pen;

	GdiplusShutdown(token);

	return TRUE;
}

DWORD ScaleObjectSet(OBJECTSET *os, FLOAT scale)
{
	if(!os)
		return FALSE;

	DWORD i = 0;
	DWORD nObject = os->nObject;

	CIRCLE circle;
	VECTOR vector;
	TEXTR text;

	for(i = 0; i < nObject; i++){
	
		switch(os->object[i].id){
			case OBJ_CIRCLE:
				memcpy(&circle, os->object[i].lpObject, sizeof(CIRCLE));
				circle.r = circle.r*scale;
				circle.x = circle.x*scale;
				circle.y = circle.y*scale;
				memcpy(os->object[i].lpObject, &circle, sizeof(CIRCLE));
				break;
			case OBJ_VECTOR:
				memcpy(&vector, os->object[i].lpObject, sizeof(VECTOR));
				vector.pHead.x = vector.pHead.x*scale;
				vector.pHead.y = vector.pHead.y*scale;
				vector.pTail.x = vector.pTail.x*scale;
				vector.pTail.y = vector.pTail.y*scale;
				memcpy(&os->object[i].lpObject, &vector, sizeof(VECTOR));
				break;
			case OBJ_TEXT:
				memcpy(&text, os->object[i].lpObject, sizeof(TEXTR));
				text.pt.x = text.pt.x*scale;
				text.pt.y = text.pt.y*scale;
				memcpy(os->object[i].lpObject, &text, sizeof(TEXTR));
			default:
				break;
		}
				
	
	}


	return TRUE;
}