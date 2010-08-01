#include "stdafx.h"
#ifndef __HOG_H__
#define __HOG_H__
typedef Vec2f Gradient;



//struct gradient{
//	float angle;
//	float weight;
//	gradient(){}
//	gradient(float a, float w){
//		angle =a;
//		weight=w;
//
//	}
//};

struct HIS{
	double* vector_weight;
	int n_bins;
	HIS(int n=9 ){
		n_bins =n;
		vector_weight = new double[n];
		for (int i=0;i<n_bins;i++)
		{
			vector_weight[i]=0.0;
		}
	}
	~HIS(){}
};

//calculate HOG of each pixels;
//Gradient** calcHOG(const Mat&filx,const Mat&fily);
Mat calcGradientOfPixels(const Mat&filx,const Mat&fily);
//calculate HIstoram of an cell
HIS* calcHisOfCell(Mat hog_pixels, Rect r, int n_bins);
//HIS*** calcHisOfCellsInWnd(Gradient** hog_pixels,Size wndSize, Size cellSize, int n_bins);
Mat calcHisOfCellsInWnd(Mat hog_pixels,Size wndSize, Size cellSize, int n_bins);

Mat NormalizeBlock(Mat m, int c);
Mat im2double(const Mat& m);
HIS* calcHistOfBlockInWnd(Mat mat, Rect p);

HIS* calcHistOfWnd(Mat mat, Size blockSize, Vec2i overlap);



#endif
