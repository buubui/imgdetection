#pragma once
bool detection(string path,string fname, string ext,float scaleStep,Size addStep,float minB,double radius,int minCsize, bool justMeanshift,bool mergeRect);
Rect resizeImg(Mat& img,double maxSz,double minSz,bool addBlank);