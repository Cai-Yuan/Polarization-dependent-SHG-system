
#include "stdafx.h"
#include "Circle.h"
#include "Point.h"
#include "Holography.h"


//S - Arc Length
//C - Circle

//S = r*theta
//V = S/t

//C(x, y, r)
//v[pixel/s]
//t[s] - Time that the points are generated at

POINT GetRotationPosition(CIRCLE c, FLOAT v, FLOAT t, FLOAT fps)
{
	POINT p = {0};
	FLOAT dt = fps/1000.0; //dt[ms]

	FLOAT theta = ((FLOAT)v/(FLOAT)c.r)*t;
	p.x = c.r*cos(theta)+c.x;
	p.y = c.r*sin(theta)+c.y;

	return p;
}
DWORD GenerateCircumfrencialPoints(UINT w, UINT h, UCHAR *img, FLOAT v, FLOAT t, FLOAT fps)
{
	if(!img)
		return FALSE;

	INT i = 0;
	FLOAT dt = fps/1000.0;	//dt[ms]
	POINT p1;
	POINT p2;
	POINT p3;

	CIRCLE c1;
	CIRCLE c2;
	CIRCLE c3;

	c1 = Circle(50, 64, 128);
	c2 = Circle(50, 128, 128);
	c3 = Circle(50, 192, 128);

	memset(img, NULL, w*h*sizeof(UCHAR));

	for(i = 0; i < 10; i++)
	{
		p1 = GetRotationPosition(c1, v, t, fps); 
		p2 = GetRotationPosition(c2, -1*v, t, fps);
		p3 = GetRotationPosition(c3, v, t, fps);

		DrawPoint(w, h, &p1, img, 255);
		DrawPoint(w, h, &p2, img, 255);
		DrawPoint(w, h, &p3, img, 255);

	}
	

	return TRUE;
}

DWORD TestHologramPointGen(HWND hwnd, UINT w, UINT h)
{
	HWND hDataView;
	INT i = 0;
	UCHAR *img = (UCHAR*) malloc(w*h*sizeof(UCHAR));

	CHAR str[MAX_STRING];

	PDMFRAME pdmf = {0};
	AllocPDMFrame(&pdmf, w, h, DT_UCHAR);

	hDataView = CreateMDIDataView(hwnd, "Hologram Point Generation");
	DataView_SetDimensions(hDataView, w, h);
	ShowWindow(hDataView, SW_SHOW);
	
	FLOAT v = 10*3.14*3;
	FLOAT fps = 25;
	FLOAT dt = fps/1000.0;
	FLOAT t = 0.0;

	for(t = 0; t < 3.3; t+=dt, i++)
	{
		GenerateCircumfrencialPoints(w, h, img, v, t, fps);
		memcpy(pdmf.lpvoid, img, w*h*sizeof(UCHAR));

		DataView_ShowFrame(hDataView, &pdmf);
		sprintf(str, "C:\\Archive\\Hologram Point Generator\\P%d.bmp", i);
		SaveBMP(w, h, str, img);
		Sleep(5);
	}


	if(img)free(img);
	FreePDM(&pdmf);



	return TRUE;
}