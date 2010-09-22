#ifndef _MULTISCALE_DETECT_RECT_H_
#define _MULTISCALE_DETECT_RECT_H_

void multiscale(Mat img,float step );
void multiscaleExp(string filepath,float step,float minb );
Rect getRect(int x,int y, float scale);
void drawRect2Img(Mat & img, string rectFile);

#endif