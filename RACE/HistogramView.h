#pragma once

#define WIDTH_HISTOGRAM 256
#define HEIGHT_HISTOGRAM 256

typedef struct HISTOGRAMVIEW_{
	HWND hOwner;

	PIXEL *pixHistogram;
}HISTOGRAMVIEW;

LRESULT CALLBACK HistogramViewProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);