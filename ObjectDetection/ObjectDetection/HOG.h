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
typedef Mat HIS;
//struct HIS{
//	float* vector_weight;
//	int n_bins;
//	HIS(int n=9 ){
//		n_bins =n;
//		vector_weight = new float[n];
//		for (int i=0;i<n_bins;i++)
//		{
//			vector_weight[i]=0.0;
//		}
//	}
//	~HIS(){
//		delete[] vector_weight;
//	}
//};

//calculate HOG of each pixels;
//Gradient** calcHOG(const Mat&filx,const Mat&fily);
Mat calcGradientOfPixels(const Mat&filx,const Mat&fily);
Mat calcGradientOfPixelsMaxChannel(const Mat* imfils , int nchannels);
//calculate HIstoram of an cell
HIS* calcHisOfCell(Mat hog_pixels, Rect r, int n_bins);
//HIS*** calcHisOfCellsInWnd(Gradient** hog_pixels,Size wndSize, Size cellSize, int n_bins);
void calcHisOfCellsInWnd(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins,Mat&H);

void NormalizeBlock(Mat& m, int c);
//void NormalizeBlock(HIS* h, int c);
Mat im2float(const Mat& m);
//HIS* calcHistOfBlockInWnd(Mat mat, Rect p);
void calcHistOfBlockInWnd(const Mat& mat, Rect p,HIS& hist);
//HIS* calcHistOfWnd(Mat mat, Size blockSize, Vec2i overlap, int norm_c);
void calcHistOfWnd(const Mat& mat, const Size& blockSize, Vec2i overlap, int norm_c,HIS& H);
void calcHisOfCellsInWnd2(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins,Mat& H);
void setHisOfCell(const Gradient& hog_pixcell, HIS* Hcell,Size cellSize);
inline void setHisOfCells(const Gradient& hog_pixcell, HIS** Hcells,float* R,int n_cells,Size cellSize);
void calcHisOfCellsInWnd2new(const Mat& hog_pixels,Rect wnd, Size cellSize, int n_bins,Mat& H);
void GaussianBlurBlock(Mat& h);
#endif
