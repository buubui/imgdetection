#pragma once
bool detection(string path,string fname, string ext,float scaleStep,Size addStep,float minB,float radius,int minCsize, bool justMeanshift,bool mergeRect);
Rect resizeImg(Mat& img,float maxSz,float minSz,bool addBlank);