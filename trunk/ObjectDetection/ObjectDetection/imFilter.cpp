#include "stdafx.h"
#include "imFilter.h"
using namespace System;
using namespace System::Collections;
IplImage** imFilter(IplImage* img){
	//ArrayList imfils= gcnew ArrayList();
	IplImage** arrFils = new IplImage*[2];
	IplImage* img_gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U,1);
	cvCvtColor(img, img_gray, CV_BGR2GRAY);
	cvEqualizeHist(img_gray,img_gray);

	float v[]={-1,0,1};
	CvMat Dx =cvMat(1,3,CV_32F,&v);
	IplImage* img3 = cvCreateImage(cvGetSize(img_gray),img_gray->depth,img_gray->nChannels);
	cvFilter2D(img_gray,img3,&Dx);
	arrFils[0] = img3;
	IplImage* img4 = cvCreateImage(cvGetSize(img_gray),img_gray->depth,img_gray->nChannels);
	cvFilter2D(img_gray,img4,&Dx);
	arrFils[1] = img4;
	
	return arrFils;
	//cvShowImage("ori",img_gray);
	//cvShowImage("filter",img3);
	//for(int i=0;i<100;i++){
	//	CvScalar s = cvGet2D(img3,50,100+i);
	//	printf("%f:%f:%f:%f\n",s.val[0],s.val[1],s.val[2],s.val[3]);
	//}
	////cvShowImage("ori",img);
	////cvShowImage("aaa",img2);
	//cvWaitKey();
}


//IplImage** imFilter(IplImage* img){
//	//ArrayList imfils= gcnew ArrayList();
//	IplImage** arrFils = new IplImage*[2];
//	IplImage* img_gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U,1);
//	cvCvtColor(img, img_gray, CV_BGR2GRAY);
//	cvEqualizeHist(img_gray,img_gray);
//	
//	Scalar* v = new Scalar(-1,0,1);
//	Mat* Dx = new Mat(1,3,CV_32F,v);
//	IplImage* img3 = cvCreateImage(cvGetSize(img_gray),img_gray->depth,img_gray->nChannels);
//	//filter2D(img_gray,img3,IPL_DEPTH_8U,Dx);
//	//cvFilter2D(img_gray,img3,&Dx);
//	arrFils[0] = img3;
//	IplImage* img4 = cvCreateImage(cvGetSize(img_gray),img_gray->depth,img_gray->nChannels);
//	cvFilter2D(img_gray,img4,&Dx);
//	arrFils[1] = img4;
//
//	return arrFils;
//	//cvShowImage("ori",img_gray);
//	//cvShowImage("filter",img3);
//	//for(int i=0;i<100;i++){
//	//	CvScalar s = cvGet2D(img3,50,100+i);
//	//	printf("%f:%f:%f:%f\n",s.val[0],s.val[1],s.val[2],s.val[3]);
//	//}
//	////cvShowImage("ori",img);
//	////cvShowImage("aaa",img2);
//	//cvWaitKey();
//}