#include "stdafx.h"
#include "cutImage.h"



IplImage** cutImage(IplImage* img, CvSize* wndSize, CvSize* step)
{
	ArrayList arrImgs = gcnew ArrayList();
	
	//Vector v;
	//v.
	int W = img->width;
	int H = img->height;
	int currX =0;
	int currY=0;
	
	while(true)
	{
		cvSetImageROI(img, cvRect(currX, currY, wndSize->width, wndSize->height));
		IplImage* img2 = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
		cvCopy(img,img2,NULL);
		//arrImgs.Add(&img2);
	}
	
	cvResetImageROI(img);

	//return arrImgs;
	return NULL;

};