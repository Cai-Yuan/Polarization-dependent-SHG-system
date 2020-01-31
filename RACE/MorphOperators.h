#pragma once

DWORD Dilate(UINT w, UINT h, INT *imgINT, INT *imgDilate);
DWORD Erode(UINT w, UINT h, INT *imgINT, INT *imgErode);

DWORD Dilate(UINT w, UINT h, FLOAT *img, FLOAT *imgDilate);
DWORD Erode(UINT w, UINT h, FLOAT *img, FLOAT *imgErode);

DWORD Dilate(UINT w, UINT h, UINT times, FLOAT *img, FLOAT *dilate);
DWORD Erode(UINT w, UINT h, UINT times, FLOAT *img, FLOAT *erode);