#pragma once
void svmGenerateData(string inputfilelist, int pos,int randTime);
void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg,bool useMaxChannel,bool useLBB);
void VOCAnnRects(System::String^ XmlFileName,System::String^ objName,Rect* &rects,int& n_rect);
void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg,bool useMaxChannel,bool useSmooth,bool useLBB);
