#include "stdafx.h"

#include "DataPreview.h"

FLOAT GetPreviewRatio(UINT w, UINT h, UINT wPreview, UINT *hPreview)
{
	FLOAT ratio;

	ratio = (FLOAT) ( (FLOAT)wPreview / (FLOAT) w);
	*hPreview = (h*ratio);
	

	return ratio;

}