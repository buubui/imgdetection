#include "stdafx.h"
#ifndef __SVMDATA_H__
#define __SVMDATA_H__
void svmGenerateData(string inputfilelist, int pos,int randTime);
void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg, int normType,float scale,float scaleStep, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech);
void VOCAnnRects(System::String^ XmlFileName,System::String^ objName,Rect* &rects,int& n_rect);
void VOCSvmGenerateData2(System::String^ AnnPath,std::string imgsPath,std::string fileExt,System::String^ objName,std::string filelist, int randTimePos,int randTimeNeg,double scale,double step,bool useMaxChannel,bool useSmooth, bool useLBP);
void svmClassify( Mat img,Mat G,Rect slideWnd, Size cellSz,float& scale, int& n_x, int& n_y,int*& x_corr,int*&y_corr, Mat& his_cells_wnd, HIS& his_wind,Mat*&h_ws, int normType, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech);
void svmGenHardList(string weightFile,string posfilelist, string negfilelist,string prefixName, int randTimePos,int randTimeNeg,int normType, float scale,float scaleStep, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech);
PCA computePCA(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg, int normType,float scale,float scaleStep, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech,int maxComponents);
#endif