#include "stdafx.h"
#include "meanshift.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;
bool detection(string path,string fname, string ext,float scaleStep,Size addStep,float minB,float radius,int minCsize, bool justMeanshift,bool mergeRect)
{
	if(path[path.size()-1]!='\\')
		path=path+"\\";
		Mat img = imread(path+fname+ext);

		cout<<path<<fname<<ext<<endl;
		printf("%f %f %f %d\n",scaleStep,minB,radius,minCsize);
	//	Mat img;
		float maxSz=maxWndSz.width*maxWndSz.height;
		float minSz = wndSize.width*wndSize.height;
		Rect realRect=resizeImg(img,maxSz,minSz,true);
		/*
		float t=(float)imgOrg.rows*imgOrg.cols;
		float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
		resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
		resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
		cout<<img.rows<<" "<<img.cols<<" "<<resizeScale<<endl;
		imgOrg.release();*/
		
		if(!justMeanshift){
			multiscaleExp(path+fname+ext,scaleStep,addStep,3,true,false,false,3);
		//	multiscaleExp(path+fname+ext,scaleStep,addStep,true);
		}
		Mat multiscale=img.clone();
		drawRect2Img(multiscale,"output/"+fname+"_multiscale.txt",minB,realRect,false);
		imwrite("output/"+fname+"_multiscale.png",multiscale(realRect),vector<int>(CV_IMWRITE_PNG_COMPRESSION,4));
		multiscale.release();
		
		int n_mean,p_mean;
		float*means;
		meanshiftFromFile("output/"+fname+"_multiscale.txt",minB,radius,minCsize,means,n_mean,p_mean);
	//	Mat* means;
	//	newMeanshiftFromFile("output/"+fname+"_multiscale.txt",minB,minCsize,means,n_mean,p_mean);
		
	/*	for (int i=0;i<n_mean*p_mean;i++)
		{
			cout<<means[i]<<", ";
			if(i>0&&i%p_mean==p_mean-1)
				cout<<endl;
		}*/
		drawRect2Img(img,"output/"+fname+"_meanshift.txt",0.,realRect,mergeRect);
		if(!imwrite("output/"+fname+"_meanshift.png",img(realRect),vector<int>(CV_IMWRITE_PNG_COMPRESSION,4)))
		{
			cout<<"KO GHI FILE DC\n";
		}
		
//		imshow(fname+"_meanshift",img);
		img.release();
	
	return true;
};

Rect resizeImg(Mat& img,float maxSz,float minSz,bool addBlank)
{
	Mat img2;
//	float maxSz=500*400.;
//	float minSz = wndSize.width*wndSize.height;
	float t=(float)img.rows*img.cols;
	float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
	resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
	resize(img,img2,Size(img.cols*resizeScale,img.rows*resizeScale),resizeScale,resizeScale);
	if(!addBlank)
	{
		img.release();
		img=img2;
		return Rect(0,0,img.cols,img.rows);
	}
	Size retV = img2.size();
	cout<<img2.rows<<" "<<img2.cols<<" "<<resizeScale<<endl;
	img.release();
	Size addSz(0.1*img2.cols,0.1*img2.rows);
	img = Mat::zeros(img2.rows+addSz.width,img2.cols+addSz.height,img2.type());
	Rect realRect=Rect(addSz.width/2-1,addSz.height/2-1,retV.width,retV.height);
	img2.copyTo(img(realRect));
	img2.release();
//	img(Rect(wndSize.width,wndSize.height,retV.width,retV.height))=img2;
//	imshow("dasd",img);
	//img=img2;
	return realRect;
};