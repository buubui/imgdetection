#pragma once
bool detection(string path,string fname, string ext,float scaleStep,float minB,double radius,int minCsize, bool justMeanshift);
Rect resizeImg(Mat& img,double maxSz,double minSz);