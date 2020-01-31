#include "stdafx.h"

#include "DataTypes.h"
#include "ParticleTrack.h"
#include "ImageSubtract.h"
#include "LogOperator.h"
#include "Range.h"
#include "TheoPavlidis.h"

VOID GradientPlowImageAbsThresh(UINT w, UINT h, UINT thresh, UCHAR *img, INT *dxdyAbsThresh);
VOID FiniteFilter(UINT w, UINT h, INT *imgINT, INT *imgFilteredINT);

DWORD ParticleTrack(HWND hOwner, DATAMANAGER *dataManager, WPARAM wParam, LPARAM lParam)
{
	DWORD dw = NULL;

	VOID *lpvoid = NULL;

	UCHAR *imgI = NULL;
	UCHAR *imgII = NULL;
	UCHAR *imgIII = NULL;
	UCHAR *imgIV = NULL;
	UCHAR *imgSubI = NULL;
	UCHAR *imgSubII = NULL;
	UCHAR *imgSubIII = NULL;

	INT w = dataManager->wStack;
	INT h = dataManager->hStack;

	imgI = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgII = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgIII = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgIV = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	imgSubI = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgSubII = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgSubIII = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	CHAR typeData;
	CHAR typeMetaData;
	PDMFRAME *pdmFrame;
	CHAR strArchive[MAX_STRING];

	pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
	memset(&strArchive, NULL, sizeof(strArchive));

	//Get imgI
	GenerateArchivePathPDM(dataManager->idWnd, 
							((UINT) wParam) - 1, 
							(CHAR *)dataManager->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataManager->hOwner, "Error Reading imgI");
		ThrowLastError(dataManager->hOwner);
		return NULL;
	}
	else{
		memcpy(imgI, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	//Get imgII
	GenerateArchivePathPDM(dataManager->idWnd, 
							((UINT) wParam), 
							(CHAR *)dataManager->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataManager->hOwner, "Error Reading imgII");
		ThrowLastError(dataManager->hOwner);
		return NULL;
	}
	else{
		memcpy(imgII, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	//Get imgIII
	GenerateArchivePathPDM(dataManager->idWnd, 
							((UINT) wParam)+1, 
							(CHAR *)dataManager->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataManager->hOwner, "Error Reading imgIII");
		ThrowLastError(dataManager->hOwner);
		return NULL;
	}
	else{
		memcpy(imgIII, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	ImageSubtract(w, h, imgI, imgII, imgSubI);
	ImageSubtract(w, h, imgII, imgIII, imgSubII);
	ImageSubtract(w, h, imgIII, imgIV, imgSubIII);





	if(imgI)free(imgI);
	if(imgII)free(imgII);
	if(imgIII)free(imgIII);
	if(imgIV)free(imgIV);
	if(imgSubI)free(imgSubI);
	if(imgSubII)free(imgSubII);
	if(imgSubIII)free(imgSubIII);

	return dw;

}

DWORD ParticleTrack(HWND hOwner, DATAVIEW *dataView, WPARAM wParam, LPARAM lParam)
{
	DWORD dw = NULL;

	VOID *lpvoid = NULL;

	
	UCHAR *imgI = NULL;
	UCHAR *imgII = NULL;
	UCHAR *imgIII = NULL;
	UCHAR *imgIV = NULL;
	UCHAR *imgSubI = NULL;
	UCHAR *imgSubII = NULL;
	UCHAR *imgSubIII = NULL;
	UCHAR *imgGradI = NULL;
	UCHAR *imgGradII = NULL;
	UCHAR *imgGradIII = NULL;

	INT i = NULL;
	INT w = dataView->wSource;
	INT h = dataView->hSource;
	INT *tempINT = NULL;

	imgI = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgII = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgIII = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgIV = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	imgSubI = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgSubII = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgSubIII = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	imgGradI = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgGradII = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgGradIII = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	tempINT = (INT *) calloc(w*h, sizeof(INT));

	CHAR typeData;
	CHAR typeMetaData;
	PDMFRAME *pdmFrame;
	CHAR strArchive[MAX_STRING];

	pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
	memset(&strArchive, NULL, sizeof(strArchive));

	//Get imgI
	GenerateArchivePathPDM(dataView->idDataManager, 
							((UINT) dataView->fStack) - 1, 
							(CHAR *)dataView->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataView->hOwner, "Error Reading imgI");
		ThrowLastError(dataView->hOwner);
		return NULL;
	}
	else{
		memcpy(imgI, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	//Get imgII
	//GenerateArchivePathPDM(dataView->idDataManager, 
	//						((UINT) dataView->fStack), 
	//						(CHAR *)dataView->strArchivePath, 
	//						(CHAR *)FS_RAW, 
	//						strArchive);

	//if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
	//	ThrowError(dataView->hOwner, "Error Reading imgII");
	//	ThrowLastError(dataView->hOwner);
	//	return NULL;
	//}
	//else{
		memcpy(imgII, (VOID *)lParam/*lpvoid*/, w*h*sizeof(UCHAR));
	//	if(lpvoid) free(lpvoid);
	//}

	//Get imgIII
	GenerateArchivePathPDM(dataView->idDataManager, 
							((UINT) dataView->fStack)+1, 
							(CHAR *)dataView->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataView->hOwner, "Error Reading imgIII");
		ThrowLastError(dataView->hOwner);
		return NULL;
	}
	else{
		memcpy(imgIII, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}


	//Initialize Contour Set
	CONTOURSET contourSet;
	memset(&contourSet, NULL, sizeof(CONTOURSET));
	contourSet.nContour = 0;
	contourSet.contour = (CONTOUR *)calloc(w+h, sizeof(CONTOUR));
	for(i = 0; i < w+h; i++){
		contourSet.contour[i].point = (POINT *)calloc(w+h, sizeof(POINT));
	}

	//Initialize Ellipse Set
	//ELLIPSESET *ellipseSet;
	//ellipseSet = (ELLIPSESET *) calloc(3, sizeof(ELLIPSESET));



	//Initialize Ellipse Array
	FELLIPSE *ellipse = NULL;
	FELLIPSE *ellipseTemp = NULL;
	ellipse = (FELLIPSE *) calloc(w*h, sizeof(FELLIPSE));
	ellipseTemp = (FELLIPSE *) calloc(w*h, sizeof(FELLIPSE));

	//Test 1
	//GradientPlow(w, h, imgII, imgGradII);
	//GradientPlow(w, h, imgIII, imgGradIII);
	//ImageSubtract(w, h, imgII, imgIII, imgSubI);
	//Threshold(w, h, 20, imgSubI);


	ImageSubtract(w, h, imgI, imgII, imgSubI);
	ImageSubtract(w, h, imgII, imgIII, imgSubII);
	ImageSubtract(w, h, imgIII, imgIV, imgSubIII);
	////GradientPlow(w, h, 25, imgSubI, imgGradI);
	UCHARtoINT(w, h, imgSubI/*imgGradI*/, tempINT);	
	Threshold(w, h, 15, tempINT);
	TheoPavlidisContour(w, h, tempINT, &contourSet);



	INT nEllipse = 0;
	for(i = 0; i < contourSet.nContour; i++){
		if(contourSet.contour[i].nPoint > 15){
			FELLIPSE fellipse;
			GetEllipseFromPoints(contourSet.contour[i].nPoint, contourSet.contour[i].point, &fellipse);
			if( (fellipse.a / fellipse.b) < 2){
				memcpy(&ellipse[nEllipse], &fellipse, sizeof(FELLIPSE));
				nEllipse++;
			}
		}
	}

	nEllipse = RemoveInnerEllipse(nEllipse, 0, ellipse, ellipseTemp);

	//if(dataView->ellipseSet.fellipse) free(dataView->ellipseSet.fellipse);
	//dataView->ellipseSet.nEllipse = nEllipse;
	//dataView->ellipseSet.fellipse = (FELLIPSE *) calloc(nEllipse, sizeof(FELLIPSE));

	//for(i = 0; i < nEllipse; i++){
	//	memcpy(&dataView->ellipseSet.fellipse[i], &ellipse[i], sizeof(FELLIPSE));
	//}

	//DrawEllipses(w, h, nEllipse, dataView->pixSrc, ellipseTemp, RGB(255, 255, 0)); 

	//memcpy((VOID *)lParam, (VOID *) imgGradI, w*h*sizeof(UCHAR));
	//memcpy((VOID *)lParam, (VOID *)imgSubI, w*h*sizeof(UCHAR));
	//memcpy((VOID *)lParam, (VOID *) imgII, w*h*sizeof(UCHAR));
	//INTtoUCHAR(w, h, tempINT, (UCHAR *)lParam);

	if(imgI)free(imgI);
	if(imgII)free(imgII);
	if(imgIII)free(imgIII);
	if(imgIV)free(imgIV);
	if(imgSubI)free(imgSubI);
	if(imgSubII)free(imgSubII);
	if(imgSubIII)free(imgSubIII);

	return dw;

}

DWORD ParticleTrackGradient(HWND hOwner, DATAVIEW *dataView, WPARAM wParam, LPARAM lParam)
{
	DWORD dw = NULL;

	VOID *lpvoid = NULL;

	UCHAR *imgI = NULL;
	UCHAR *imgII = NULL;
	UCHAR *imgIII = NULL;
	UCHAR *imgIV = NULL;
	UCHAR *imgSubI = NULL;
	UCHAR *imgSubII = NULL;
	UCHAR *imgSubIII = NULL;
	UCHAR *imgIGrad = NULL;
	UCHAR *imgIIGrad = NULL;
	UCHAR *imgIIIGrad = NULL;

	INT w = dataView->wStack;
	INT h = dataView->hStack;

	imgI = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgII = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgIII = (UCHAR *) calloc(w*h,sizeof(UCHAR));
	imgIV = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	imgSubI = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgSubII = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgSubIII = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	imgIGrad = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgIIGrad = (UCHAR *) calloc(w*h, sizeof(UCHAR));
	imgIIIGrad = (UCHAR *) calloc(w*h, sizeof(UCHAR));

	CHAR typeData;
	CHAR typeMetaData;
	PDMFRAME *pdmFrame;
	CHAR strArchive[MAX_STRING];

	pdmFrame = (PDMFRAME *) calloc(1, sizeof(PDMFRAME));
	memset(&strArchive, NULL, sizeof(strArchive));

	//Get imgI
	GenerateArchivePathPDM(dataView->idDataManager, 
							((UINT) dataView->fStack) - 1, 
							(CHAR *)dataView->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataView->hOwner, "Error Reading imgI");
		ThrowLastError(dataView->hOwner);
		return NULL;
	}
	else{
		memcpy(imgI, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	//Get imgII
	GenerateArchivePathPDM(dataView->idDataManager, 
							((UINT) dataView->fStack), 
							(CHAR *)dataView->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataView->hOwner, "Error Reading imgII");
		ThrowLastError(dataView->hOwner);
		return NULL;
	}
	else{
		memcpy(imgII, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	//Get imgIII
	GenerateArchivePathPDM(dataView->idDataManager, 
							((UINT) dataView->fStack)+1, 
							(CHAR *)dataView->strArchivePath, 
							(CHAR *)FS_RAW, 
							strArchive);

	if(!ReadPDMFrame(&typeData, &typeMetaData, pdmFrame, (CHAR *) strArchive, &lpvoid)){
		ThrowError(dataView->hOwner, "Error Reading imgIII");
		ThrowLastError(dataView->hOwner);
		return NULL;
	}
	else{
		memcpy(imgIII, lpvoid, w*h*sizeof(UCHAR));
		if(lpvoid) free(lpvoid);
	}

	GradientPlow(w, h, imgII, imgIIGrad);
	GradientPlow(w, h, imgIII, imgIIIGrad);
	ImageSubtract(w, h, imgIIGrad, imgIIIGrad, imgSubI);

	Threshold(w, h, 20, imgSubI);
	//ImageSubtract(w, h, imgI, imgII, imgSubI);
	//ImageSubtract(w, h, imgII, imgIII, imgSubII);
	//ImageSubtract(w, h, imgIII, imgIV, imgSubIII);

	memcpy((VOID *)lParam, (VOID *)imgSubI, w*h*sizeof(UCHAR));


	if(imgI)free(imgI);
	if(imgII)free(imgII);
	if(imgIII)free(imgIII);
	if(imgIV)free(imgIV);
	if(imgSubI)free(imgSubI);
	if(imgSubII)free(imgSubII);
	if(imgSubIII)free(imgSubIII);
	if(imgIGrad)free(imgIGrad);
	if(imgIIGrad)free(imgIIGrad);
	if(imgIIIGrad)free(imgIIIGrad);

	return dw;

}

VOID GradientPlowImageAbsThresh(UINT w, UINT h, UINT thresh, UCHAR *img, INT *dxdyAbsThresh)
{

	INT x, y, i;
	INT sum;
	INT xMax = (w-1);
	INT yMax = (h-1);
	INT dxdySum;

	memset(dxdyAbsThresh, 0x00, w*h*sizeof(INT));

	for(y = 0; y < yMax; y++){
		for(x = 0; x < xMax; x++){
			i = (y*w) + x;
			//dxdySum = abs(img[i+1]+img[i+w]+img[i+1+w]+img[i-1+w] - (4*img[i]));
			dxdySum = abs(img[i+1]-img[i]);
			dxdySum += abs(img[i+w]-img[i]);
			dxdySum += abs(img[i+1+w]-img[i]);
			dxdySum += abs(img[i-1+w]-img[i]);
			if(dxdySum > thresh)
				dxdyAbsThresh[i] = 255;
		}
	}
	
}

VOID FiniteFilter(UINT w, UINT h, INT *imgINT, INT *imgFilteredINT)
{

	INT MIN_PIXEL_COUNT = 5;
	INT x, y, i;

	INT sKernel = 5;

	INT off = (FLOAT)(sKernel-1)/(FLOAT)2;
	INT yMax = h-off;
	INT xMax = w-off;
	INT  xOff = 0;
	INT yOff = 0;

	INT runValue;

	memcpy(imgFilteredINT, imgINT, w*h*sizeof(INT));


	for(y = off; y < yMax; y++){
		for(x = off; x < xMax; x++){
			i = y*w + x;
			if(imgINT[i]){
				runValue = 0;
				for(yOff = -off; yOff < off; yOff++){
					for(xOff = -off; xOff < off; xOff++){

						i = ((y+yOff)*w + (x+xOff));
						if(imgINT[i])
							runValue += imgINT[i];
					}
				}
				

				if(runValue < (MIN_PIXEL_COUNT*255)){
					i = (y*w) + x;
					imgFilteredINT[i] = 0;
				}
			}
		}
	}
					

	
}


//PARTICLE TRACKER
			//nEllipse = RemoveInnerEllipse(nEllipse, 0, elli, elliK);
			//for(i = 0; i < nEllipse; i++){
			//	//DrawEllipse(w, h, pixOut, elliK[i], R21GB(255, 255, 255));
			//	memcpy(&ellipseSet[f].fellipse[i], &elliK[i], sizeof(FELLIPSE));
			//	ellipseSet[f].nEllipse = nEllipse;
			//}

			////INT off = 3;
			////INT p = 0;
			////if(f > off && f < nFiles - off){
			////	for(p = (-off + 1); p < off; p++){
			////		DrawEllipses(w, h, ellipseSet[f+p].nEllipse, pixOut, ellipseSet[f+p].fellipse, RGB(255, 0, 0));
			////	}
			////}

			//if(f == 3){
			//	UINT mm;
			//	mm = 0;
			//	mm = 0;
			//}
			//if(f > 2){
			//	UINT nEllipse = 0; 
			//	//INT pairF0[256];
			//	//INT pairF1[256];
			//	//INT pairF2[256];
			//	INTGROUP pairF0;
			//	INTGROUP pairF1;
			//	INTGROUP pairF2;
			//	UINT nPF0 = 0;
			//	UINT nPF1 = 0;
			//	UINT nPF2 = 0;

			//	//Color All Ellipses found in frames F0, F1, and F2
			//	//Changing between TRUE/FALSE changes the color
			//	if(TRUE){
			//		DrawEllipses(w, h, ellipseSet[f-2].nEllipse, pixOut, ellipseSet[f-2].fellipse, RGB(0, 0, 255));
			//		DrawEllipses(w, h, ellipseSet[f-1].nEllipse, pixOut, ellipseSet[f-1].fellipse, RGB(0, 255, 0));
			//		DrawEllipses(w, h, ellipseSet[f-0].nEllipse, pixOut, ellipseSet[f-0].fellipse, RGB(255, 0, 0));
			//	}
			//	else{
			//		DrawEllipses(w, h, ellipseSet[f-2].nEllipse, pixOut, ellipseSet[f-2].fellipse, RGB(255, 255, 255));
			//		DrawEllipses(w, h, ellipseSet[f-1].nEllipse, pixOut, ellipseSet[f-1].fellipse, RGB(255, 255, 255));
			//		DrawEllipses(w, h, ellipseSet[f-0].nEllipse, pixOut, ellipseSet[f-0].fellipse, RGB(255, 255, 255));
			//	}
			//	
			//	
			//	nPF0 = ellipseSet[f-0].nEllipse;
			//	nPF1 = ellipseSet[f-1].nEllipse;
			//	nPF2 = ellipseSet[f-2].nEllipse;
			//	pF0.nPoints = nPF0;
			//	pF1.nPoints = nPF1;
			//	pF2.nPoints = nPF2;

			//	nEllipse = ellipseSet[f-1].nEllipse;
			//	if(nEllipse > 0){
			//			Edit_OutputEllipseSet(hOutput/*hEdit*/, ellipseSet[f-1]);
			//			GetEllipseCentroid(nPF0, ellipseSet[f-0].fellipse, pF0.fpoint);
			//			GetEllipseCentroid(nPF1, ellipseSet[f-1].fellipse, pF1.fpoint);
			//			GetEllipseCentroid(nPF2, ellipseSet[f-2].fellipse, pF2.fpoint);
			//			memcpy(pF0.point, pF0.fpoint, nPF0*sizeof(FPOINT));
			//			memcpy(pF1.point, pF1.fpoint, nPF1*sizeof(FPOINT));
			//			memcpy(pF2.point, pF2.fpoint, nPF2*sizeof(FPOINT));

			//			nVector = TFPT(f, pF0, pF1, pF2, vector);
			//			//CHAR str[MAX_STRING];
			//			//sprintf(str, "nVector = %d", nVector);
			//			//MessageBox(NULL, (LPCSTR)str, "!!!", MB_OK);

			//			UINT p = 0;
			//			for(p = 0; p < nVector; p++){
			//				DrawVector(w, h, pixOut, vector[p], RGB(0, 0, 255));
			//			}
			//		
			//	}
			//	

			//}