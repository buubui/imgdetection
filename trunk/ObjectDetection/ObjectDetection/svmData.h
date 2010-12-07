#include "stdafx.h"
#ifndef __SVMDATA_H__
#define __SVMDATA_H__
void svmGenerateData(string inputfilelist, int pos,int randTime);
void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg,bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech);
void VOCAnnRects(System::String^ XmlFileName,System::String^ objName,Rect* &rects,int& n_rect);
void VOCSvmGenerateData2(System::String^ AnnPath,std::string imgsPath,std::string fileExt,System::String^ objName,std::string filelist, int randTimePos,int randTimeNeg,double scale,double step,bool useMaxChannel,bool useSmooth, bool useLBP);
#endif