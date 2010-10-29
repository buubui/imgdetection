#pragma once
void svmGenerateData(string inputfilelist, int pos,int randTime);
void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg,bool useMaxChannel,bool useLBB);
void VOCAnnRects(System::String^ XmlFileName,System::String^ objName,Rect* &rects,int& n_rect);
void VOCSvmGenerateData2(System::String^ AnnPath,string imgsPath,string fileExt,System::String^ objName,string filelist, int randTimePos,int randTimeNeg,double scale,double step,bool useMaxChannel);
