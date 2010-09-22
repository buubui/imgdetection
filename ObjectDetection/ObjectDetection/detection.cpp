#include "stdafx.h"
#include "meanshift.h"
extern Size cellSize,blockSize,wndSize;
bool detection(string path,string fname, string ext,float scaleStep,float minB,double radius,int minCsize, bool justMeanshift)
{
	if(path[path.size()-1]!='\\')
		path=path+"\\";
		Mat imgOrg = imread(path+fname+ext);

		cout<<path<<fname<<ext<<endl;
		printf("%f %f %f %d\n",scaleStep,minB,radius,minCsize);
		Mat img;
		double maxSz=500*400.;
		double minSz = wndSize.width*wndSize.height;
		double t=(double)imgOrg.rows*imgOrg.cols;
		float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
		resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
		resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
		cout<<img.rows<<" "<<img.cols<<" "<<resizeScale<<endl;
		imgOrg.release();
		if(!justMeanshift){
			multiscaleExp(path+fname+ext,scaleStep);
		}
		Mat multiscale=img.clone();
		drawRect2Img(multiscale,"output/"+fname+"_multiscale.txt",minB);
		imwrite("output/"+fname+"_multiscale.png",multiscale,vector<int>(CV_IMWRITE_PNG_COMPRESSION,4));
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
		if(!imwrite("output/"+fname+"_meanshift.png",img,vector<int>(CV_IMWRITE_PNG_COMPRESSION,4)))
		{
			cout<<"KO GHI FILE DC\n";
		}
		
//		imshow(fname+"_meanshift",img);
		img.release();
	
	return true;
};