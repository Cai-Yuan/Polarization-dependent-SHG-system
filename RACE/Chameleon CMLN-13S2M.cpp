
#include "stdafx.h"
#include "StringFxns.h"
#include "C/FlyCapture2_C.h"
#include "Chameleon CMLN-13S2M.h"


fc2Error fc2GetResolution(fc2Context fcContext, int *w, int *h)
{
	CHAR str[MAX_STRING];
	CHAR strWidth[MAX_STRING];
	CHAR strHeight[MAX_STRING];
	fc2Error fcError;
	fc2PGRGuid guid;
	fc2Image fcImage;
	fc2CameraInfo fcCamInfo;

	if(!w || !h) return (fc2Error) FALSE;

	*w = 0;
	*h = 0;

	fcError = fc2GetCameraFromIndex(fcContext, 0, &guid);
	fcError = fc2Connect(fcContext, &guid);

	fcError = fc2GetCameraInfo(fcContext, &fcCamInfo);
	fcError = fc2StartCapture(fcContext);
	fcError = fc2CreateImage(&fcImage);
	fcError = fc2RetrieveBuffer(fcContext, &fcImage);
	
	*w = fcImage.cols;
	*h = fcImage.rows;

	fcError = fc2StopCapture(fcContext);
	fcError = fc2DestroyImage(&fcImage);

	if(!*w && !*h)
		return (fc2Error) FALSE;

	return (fc2Error) (TRUE);
}