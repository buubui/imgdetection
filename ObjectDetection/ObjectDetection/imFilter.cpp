#include "stdafx.h"
#include "imFilter.h"
#include "HOG.h"
using namespace System;
using namespace System::Collections;
//IplImage** imFilter(IplImage* img){
//	//ArrayList imfils= gcnew ArrayList();
//	IplImage** arrFils = new IplImage*[2];
//	IplImage* img_gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U,1);
//	cvCvtColor(img, img_gray, CV_BGR2GRAY);
//	//cvShowImage("ori",img_gray);
//
//	cvEqualizeHist(img_gray,img_gray);
//	//cvShowImage("normalize",img_gray);
//	float v[]={-1,0,1};
//	CvMat Dx =cvMat(1,3,CV_32F,&v);
//	IplImage* img3 = cvCreateImage(cvGetSize(img_gray),img_gray->depth,img_gray->nChannels);
//	cvFilter2D(img_gray,img3,&Dx);
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
//
//

Mat* imFilter(const Mat& img,bool equalize){
	Mat* arrFils = new Mat[2];
	Mat img_gray;
	cvtColor(img,img_gray,CV_BGR2GRAY);
	if(equalize)
		equalizeHist(img_gray,img_gray);
	//convert image from uchar(0->255) to float (0->1)
	//Mat imfloat= im2float(img_gray);
	Mat imfloat;
	img_gray.convertTo(imfloat,DataType<float>::type,1./255);
	img_gray.release();

	
	float v[]={-1,0,1};
	Mat Dx (1,3,CV_32F,v);
	Mat dst,dst2;
	filter2D(imfloat,dst,imfloat.depth(),Dx);
	//filter2D(img_gray,dst,img_gray.depth(),Dx);
	arrFils[0] = dst;
	filter2D(imfloat,dst2,imfloat.depth(),Dx.t());
	//filter2D(img_gray,dst2,img_gray.depth(),Dx.t());
	arrFils[1] = dst2;
	
	/*for (int i=0;i<30;i++)
	{
		for (int j=0;j<30;j++)
		{
			printf("%f ; ",arrFils[0].at<float>(i,j));
		}
	}*/


	return arrFils;
}

Mat* imFilterChannels(const Mat& img1,bool equalize){
	Mat img=img1;
	//buubui - try this: BGR -> sqrtBGR -> HSV
//	if(equalize)
//	{
	/*	for (int x=0;x<img.cols;x++)
		{
			for (int y=0;y<img.rows;y++)
			{
				Vec3b v = img.at<Vec3b>(y,x);
				v[0]=std::sqrt(v[0]/255.)*255;
				v[1]=std::sqrt(v[1]/255.)*255;
				v[2]=std::sqrt(v[2]/255.)*255;
			}
		}*/

	//	cvtColor(img,img,CV_BGR2HSV);
//	}
	cvtColor(img,img,CV_BGR2Lab);

//	Mat img;
//	cvtColor(img1,img,CV_BGR2HSV);
	int n_channels =  img.channels();
	Mat* arrFils = new Mat[2*n_channels];
	Mat * imgs = new Mat[n_channels];
	cv::split(img,imgs);
//	Mat img_gray;
//	cvtColor(img,img_gray,CV_BGR2GRAY);
	/*if(equalize)
	{
		for (int i=0;i<n_channels;i++)
		{
			equalizeHist(imgs[i],imgs[i]);	
		}
	}*/
	float v[]={-1,0,1};
	Mat Dx (1,3,CV_32F,v);
	for (int i=0;i<n_channels;i++)
	{
		imgs[i].convertTo(imgs[i],DataType<float>::type,1./255);
		if(equalize)
			cv::sqrt(imgs[i],imgs[i]);
	//	Mat dst,dst2;
		filter2D(imgs[i],arrFils[i*2],imgs[i].depth(),Dx);
		//filter2D(img_gray,dst,img_gray.depth(),Dx);
	//	arrFils[i*2] = dst;
		filter2D(imgs[i],arrFils[i*2+1],imgs[i].depth(),Dx.t());
		//filter2D(img_gray,dst2,img_gray.depth(),Dx.t());
	//	arrFils[1] = dst2;

	}
	//convert image from uchar(0->255) to float (0->1)
	//Mat imfloat= im2float(img_gray);
//	Mat imfloat;
	
//	img_gray.release();


	
	
	
	/*for (int i=0;i<30;i++)
	{
	for (int j=0;j<30;j++)
	{
	printf("%f ; ",arrFils[0].at<float>(i,j));
	}
	}*/
	for (int i=0;i<n_channels;i++)
	{
		imgs[i].release();
	}
	delete[] imgs;
	img.release();
	return arrFils;
}

Mat rotateMat(Mat src,int rotateType)//0:left, 1:right
{
		Mat r=Mat::zeros(src.size(),src.type());
	//	Mat d=Mat::zeros(a.size(),a.type());
		if(rotateType==0)
			cv::flip(src,r,1);
		else
			cv::flip(src,r,0);
	return r.t();

}