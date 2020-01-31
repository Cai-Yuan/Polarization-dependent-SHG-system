#include "stdafx.h"
#include "Text.h"


DWORD DrawText(HDC hdc, TEXTR text, COLORREF cr)
{
	INT strLen = strlen(text.str);
	RECT rc;
	rc.left = text.pt.x;
	rc.top = text.pt.y;
	rc.right = rc.left + 100;
	rc.bottom = rc.top + 100;

	//SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetTextColor(hdc, cr);
	DrawText(hdc, text.str, strLen, &rc, DT_LEFT);
	//DrawText(hdc, text.str, strLen, &rc, DT_CENTER|DT_CALCRECT);


	return TRUE;
}