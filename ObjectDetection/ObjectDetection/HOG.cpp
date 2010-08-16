#include "stdafx.h"
#include "HOG.h"
#include <cxoperations.hpp>

//Gradient** calcHOG(const Mat&filx,const Mat&fily)
//{
//	int r = filx.rows;
//	int c = filx.cols;
//	Gradient** h= new Gradient*[r];
//	for(int i=0;i<r;i++) h[i] = new Gradient[c];
//	for (int i=0;i<r;i++)
//	{
//		for (int j=0;j<c;j++)
//		{
//			/*double x = filx.at<uchar>(i,j)/255.0;
//			double y = fily.at<uchar>(i,j)/255.0;*/
//			double x = filx.at<double>(i,j)/255.0;
//			double y = fily.at<double>(i,j)/255.0;
//			double w =sqrt( x*x+y*y);
//			double a;
//			a  =  atan(y/(x+0.00001))*180/M_PI;
//			/*if(x!=0) a  =  atan(y/x)*180/M_PI;
//			else a = 90;*/
//			/*if(a - floor(a)<=0.001) a=floor(a);
//			if(-a + ceil(a)<=0.001) a=ceil(a);*/
//			h[i][j]= Gradient(a,w);
//
//		}
//	}
//	/*for (int i =0;i<10;i++)
//	{
//		for(int j =0 ;j<10;j++)
//		{
//			printf("%f %f\n",h[i][j].angle,h[i][j].weight);
//		}
//	}*/
//	
//
//	return h;
//
//
//};
Mat calcGradientOfPixels(const Mat&filx,const Mat&fily)
{
	int r = filx.rows;
	int c = filx.cols;
	Mat G(r,c,DataType<Gradient>::type);
	//Gradient** h= new Gradient*[r];
	//for(int i=0;i<r;i++) h[i] = new Gradient[c];
	for (int i=0;i<r;i++)
	{
		for (int j=0;j<c;j++)
		{
			
			double x = filx.at<double>(i,j);
			double y = fily.at<double>(i,j);
			double w =sqrt( x*x+y*y);
			double a;
			a  =  atan(y/(x+0.00001))*180/M_PI;
			G.at<Gradient>(i,j)[0] =a;
			G.at<Gradient>(i,j)[1] =w;

		}
	}
		return G;


};


//hog_pixcels: weight& angle of pixels in sub-win
// Rect: toa do cell & size
//n_bins: so luong bin
HIS* calcHisOfCell(Mat hog_pixels, Rect r, int n_bins)
{
	HIS* H=new HIS(n_bins);
	double w=0;
	double a =180.0 /n_bins;
	for (int i=0;i<r.height;i++)
	{
		for (int j=0;j<r.width;j++)
		{
		//	printf("\n%f:%f\n",hog_pixels.at<Gradient>(i+r.x,j+r.y)[0],hog_pixels.at<Gradient>(i+r.x,j+r.y)[1]);
		//	int n_b = (int)( (hog_pixels.at<Gradient>(i+r.x,j+r.y)[0]+90)/a);
			float angle = hog_pixels.at<Gradient>(i+r.y,j+r.x)[0]; 
			angle += angle<0?180:0;
			int n_b = (int)( (angle)/a);
			H->vector_weight[n_b]+=hog_pixels.at<Gradient>(i+r.y,j+r.x)[1];
			
		}
	}
	/*for (int i=0;i<9;i++)
	{
		printf("%f, ",H->vector_weight[i]);
	}*/
	return H;

};
Mat calcHisOfCellsInWnd(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins)
{
	int c = (int)(wnd.width/cellSize.width);
	int r = (int)(wnd.height/cellSize.height);
	
	Mat H(r,c,DataType<HIS*>::type);
	int currX =wnd.x;
	int currY=wnd.y;
	for (int i=0;i<r;i++)
	{
		currX=0;
		for (int j=0;j<c;j++)
			
		{
		//	cout<< j<<"<"<<c<<"\n";
			try
		{
			H.at<HIS*>(i,j)= calcHisOfCell(hog_pixels,Rect(currX,currY,cellSize.width,cellSize.height),n_bins);
		}
		
		catch (...)
		{
			cerr<< "-----Exception:" <<i<<";"<<j;
			exit(0);
		}
			
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
	case 2: //L1-sqrt
		sqrt( m/(norm(m,NORM_L1) + M_e),mat); break;
	}
	
	return mat;
}

HIS* NormalizeBlock(HIS* h, int c)
{

	Mat m(1,h->n_bins,CV_64FC1);
	for (int i = 0 ; i<h->n_bins;i++)
	{
		m.at<double>(0,i) = h->vector_weight[i];
	}
	Mat mat = NormalizeBlock(m,c);
	HIS* r = new HIS(h->n_bins);
	for (int i = 0 ; i<h->n_bins;i++)
	{
		r->vector_weight[i] = mat.at<double>(0,i) ;
	}

	return r;
}

Mat im2double(const Mat& m)
{
	int r = m.rows;
	int c = m.cols;
	Mat mat(r,c,CV_64FC1);

	for (int i = 0 ; i<r ; i++)
		for (int j = 0 ; j< c ; j++)
		{
			mat.at<double>(i,j) = m.at<uchar>(i,j) / 255.0;
		}
	
	return mat;
	


}
HIS* calcHistOfBlockInWnd(Mat mat, Rect p)
{
	int n = mat.at<HIS*>(0,0)->n_bins;
	int w = p.width;
	int h = p.height;
	int x = p.x;
	int y = p.y;
	HIS* hist = new HIS(w*h*n);
	int s =0;
	for (int i=0;i<h;i++)
	{
		for (int j = 0 ; j< w;j++)
		{
			for (int e =0; e<n;e++)
			{
				hist->vector_weight[s] = mat.at<HIS*>(x+i,j+y)->vector_weight[e];
				s++;
			}
			
			
		}
	}
	printf("\n BLOCK: %d\n",s);
	for (int i =0 ; i<n*w*h;i++)
	{
		printf("%f \n ",hist->vector_weight[i]);
	}

	return hist;
}

HIS* calcHistOfWnd(Mat mat, Size blockSize, Vec2i overlap, int norm_c)
{
	int n = mat.at<HIS*>(0,0)->n_bins;
	int n_block_w = floor( 1.*(mat.cols - overlap[0])/(blockSize.width - overlap[0]));
	int n_block_h = floor( 1.*(mat.rows - overlap[1])/(blockSize.height - overlap[1]));
	int n_block = n_block_h*n_block_w;
	HIS* H = new HIS(n_block*blockSize.width*blockSize.height*n);

	int x=0,y=0;
	int s=0;
	for (int i=0; i < n_block_w;i++)
	{
		x=0;
		for (int j = 0 ; j<n_block_h;j++)
		{
			HIS* h_b = calcHistOfBlockInWnd(mat,Rect(x,y,blockSize.width,blockSize.height));
			h_b= NormalizeBlock(h_b,norm_c);
			for (int e=0;e<n*blockSize.width*blockSize.height;e++)
			{
				H->vector_weight[s]=h_b->vector_weight[e];
				s++;
			}
			x+= blockSize.width -overlap[0];

		}
		y+=blockSize.height - overlap[1];
		
		
	}
	
	return H;


}