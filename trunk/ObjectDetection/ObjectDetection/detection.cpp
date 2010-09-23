#include "stdafx.h"
#include "meanshift.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;
bool detection(string path,string fname, string ext,float scaleStep,float minB,double radius,int minCsize, bool justMeanshift)
{
	if(path[path.size()-1]!='\\')
		path=path+"\\";
		Mat img = imread(path+fname+ext);

		cout<<path<<fname<<ext<<endl;
		printf("%f %f %f %d\n",scaleStep,minB,radius,minCsize);
	//	Mat img;
		double maxSz=maxWndSz.width*maxWndSz.height;
		double minSz = wndSize.width*wndSize.height;
		Rect realRect=resizeImg(img,maxSz,minSz);
		/*
		double t=(double)imgOrg.rows*imgOrg.cols;
		float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
		resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
		resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
		cout<<img.rows<<" "<<img.cols<<" "<<resizeScale<<endl;
		imgOrg.release();*/
		
		if(!justMeanshift){
			multiscaleExp(path+fname+ext,scaleStep);
		}
		Mat multiscale=img.clone();
		drawRect2Img(multiscale,"output/"+fname+"_multiscale.txt",minB);
		imwrite("output/"+fname+"_multiscale.png",multiscale(realRect),vector<int>(CV_IMWRITE_PNG_COMPRESSION,4));
		multiscale.release();
		
		int n_mean,p_mean;
		double*means;
		meanshiftFromFile("output/"+fname+"_multiscale.txt",minB,radius,minCsize,means,n_mean,p_mean);
		
		for (int i=0;i<n_mean*p_mean;i++)
		{
			cout<<means[i]<<", ";
			if(i>0&&i%p_mean==p_mean-1)
				cout<<endl;
		}
		drawRect2Img(img,"output/"+fname+"_meanshift.txt",0.);
		if(!imwrite("output/"+fname+"_meanshift.png",img(realRect),vector<int>(CV_IMWRITE_PNG_COMPRESSION,4)))
		{
			cout<<"KO GHI FILE DC\n";
		}
		
//		imshow(fname+"_meanshift",img);
		img.release();
	
	return true;
};

Rect resizeImg(Mat& img,double maxSz,double minSz)
{
	Mat img2;
//	double maxSz=500*400.;
//	double minSz = wndSize.width*wndSize.height;
	double t=(double)img.rows*img.cols;
	float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
	resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
	resize(img,img2,Size(img.cols*resizeScale,img.rows*resizeScale),resizeScale,resizeScale);
	Size retV = img2.size();
	cout<<img2.rows<<" "<<img2.cols<<" "<<resizeScale<<endl;
	img.release();
	img = Mat::zeros(img2.rows+wndSize.width*2,img2.cols+wndSize.width*2,img2.type());
	Rect realRect=Rect(wndSize.width,wndSize.width,retV.width,retV.height);
	img2.copyTo(img(realRect));
	img2.release();
//	img(Rect(wndSize.width,wndSize.height,retV.width,retV.height))=img2;
//	imshow("dasd",img);
	//img=img2;
	return realRect;
};