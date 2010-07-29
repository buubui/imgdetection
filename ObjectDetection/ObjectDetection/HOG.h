#include "stdafx.h"
#ifndef __HOG_H__
#define __HOG_H__
struct HOG{
	double angle;
	double weight;
	HOG(){}
	HOG(double a, double w){
		angle =a;
		weight=w;

	}
};

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
HOG** calcHOG(const Mat&filx,const Mat&fily);
//calculate HIstoram of an cell
HIS* calcHisOfCell(HOG** hog_pixels, Rect r, int n_bins);
HIS*** calcHisOfCellsInWnd(HOG** hog_pixels,Size wndSize, Size cellSize, int n_bins);

Mat NormalizeBlock(Mat m, int c);




#endif
