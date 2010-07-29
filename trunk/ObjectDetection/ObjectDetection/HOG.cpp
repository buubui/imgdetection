#include "stdafx.h"
#include "HOG.h"
#include <cxoperations.hpp>
HOG** calcHOG(const Mat&filx,const Mat&fily)
{
	int r = filx.rows;
	int c = filx.cols;
	HOG** h= new HOG*[r];
	for(int i=0;i<r;i++) h[i] = new HOG[c];
	for (int i=0;i<r;i++)
	{
		for (int j=0;j<c;j++)
		{
			double x = filx.at<uchar>(i,j)/255.0;
			double y = fily.at<uchar>(i,j)/255.0;
			double w =sqrt( x*x+y*y);
			double a;
			if(x!=0) a  =  atan(y/x)*180/M_PI;
			else a = 90;
			/*if(a - floor(a)<=0.001) a=floor(a);
			if(-a + ceil(a)<=0.001) a=ceil(a);*/
			h[i][j]= HOG(a,w);

		}
	}
	/*for (int i =0;i<10;i++)
	{
		for(int j =0 ;j<10;j++)
		{
			printf("%f %f\n",h[i][j].angle,h[i][j].weight);
		}
	}*/
	

	return h;


};
//hog_pixcels: weight& angle of pixels in sub-win
// Rect: toa do cell & size
//n_bins: so luong bin
HIS* calcHisOfCell(HOG** hog_pixels, Rect r,int n_bins)
{
	HIS* H=new HIS(9);
	double w=0;
	double a =180.0 /n_bins;
	for (int i=0;i<r.width;i++)
	{
		for (int j=0;j<r.height;j++)
		{
			printf("\n%f:%f\n",hog_pixels[i+r.x][j+r.y].angle,hog_pixels[i+r.x][j+r.y].weight);
			int n_b = (int)(hog_pixels[i+r.x][j+r.y].angle/a);
			H->vector_weight[n_b]+=hog_pixels[i+r.x][j+r.y].weight;
			
		}
	}
	/*for (int i=0;i<9;i++)
	{
		printf("%f, ",H->vector_weight[i]);
	}*/
	return H;

};
HIS*** calcHisOfCellsInWnd(HOG** hog_pixels,Size wndSize, Size cellSize, int n_bins)
{
	int c = (int)(wndSize.width/cellSize.width);
	int r = (int)(wndSize.height/cellSize.height);
	
	HIS*** H = new HIS**[r] ;
	for( int i=0; i<r;i++)
		H[i] = new HIS*[c];
	int currX =0;
	int currY=0;
	for (int i=0;i<r;i++)
	{
		currX=0;
		for (int j=0;j<c;j++)
		{
			H[i][j] = calcHisOfCell(hog_pixels,Rect(currX,currY,cellSize.width,cellSize.height),n_bins);
			currX+=cellSize.width;
		}
		currY+=cellSize.height;
	}
	return H;	


};

Mat NormalizeBlock(Mat m, int c)
{
	Mat mat;
	mat.zeros(m.size(),m.type());
	double n =0.;
	switch(c)
	{
	case 0: // L2 norm
		n =norm(m,NORM_L2);	mat = m/sqrt( n*n + M_e); break;
	case 1: // L1 Norm
		mat = m/(norm(m,NORM_L1) + M_e); break;
	}
	return mat;
}