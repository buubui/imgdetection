#include "stdafx.h"
#include <boost/algorithm/string.hpp>
extern Size cellSize,blockSize,wndSize;
void multiscale(Mat img,float step )
{
	Mat result = img.clone();
	Point startP(0,0);
	double scale =0.;
	Size cellSz,wndSz; 
	Size tmp;

	tmp.width = wndSize.width / cellSize.width;
	tmp.height = wndSize.height /cellSize.height;
	float baseWidth=cellSize.width*tmp.width;
	cellSz.width = cellSize.width;
	cellSz.height = cellSize.height;
	wndSz.width = wndSize.width;
	wndSz.height = wndSize.height;
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
	int i =0;
	Mat * weight;
	double b;
	getWeight("w.txt",weight,b);
	Mat* imFils = imFilter(img);
	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
	Rect MaxWnd(0,0,wndSize.width,wndSize.height);
	double max=0;
	Mat img_slideWnd, his_wnd;
	Mat* his=NULL;
	for (int h=0;h<img.rows;h+=30)
	{
		//printf("%d\n",h);
		for (int w=0;w<img.cols;w+=80)
		{
			startP.x= w;
			startP.y = h;
			slideWnd.x = startP.x;
			slideWnd.y = startP.y;
			scale =0.;
			cellSz.width = cellSize.width;
			cellSz.height = cellSize.height;
			wndSz.width = wndSize.width;
			wndSz.height = wndSize.height;
			slideWnd.width =wndSz.width;
			slideWnd.height =wndSz.height;

			while( wndSz.width <=(img.cols - startP.x) && wndSz.height <=(img.rows - startP.y))
			{

				img_slideWnd=img(slideWnd);


				his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				if(!his)
					his=new Mat(1,h_w->n_bins,CV_64F);
				//printf("%d",h_w->n_bins);
				for (int i=0;i<h_w->n_bins;i++)
				{
					his->at<double>(0,i)=h_w->vector_weight[i];
				}
				Mat R = (*his)* (*weight ) - b;
				double v = R.at<double>(0,0);
				cout<<v<<endl;
				if(v>0){
					//					printf(" (%d,%d) (%dx%d) %f %f\n",startP.x,startP.y, wndSz.width,wndSz.height,scale, v);
					printf("%d, %d, %f\n",startP.x+wndSz.width/2,startP.y+wndSz.height/2,wndSz.width/baseWidth);
					rectangle(result,slideWnd,Scalar(0,256,0),2);
					stringstream out;
					out<<v;
					putText(result,out.str(),Point(slideWnd.x,slideWnd.y-3),FONT_HERSHEY_COMPLEX_SMALL,0.5,Scalar(0,256,0));
					stringstream outputfile;
					outputfile <<"img "<<startP.x<<" "<<startP.y<<" "<<wndSz.width<<" "<<wndSz.height;
					//	imshow(outputfile.str(),img_slideWnd);
					//	if(v>max){
					//		max=v;
					//		MaxWnd = slideWnd;
					//	}
				}

				delete h_w;


				//		imFils[0].release();
				//		imFils[1].release();
				for (int ii=0;ii<his_wnd.rows;ii++)
				{
					for (int jj=0;jj<his_wnd.cols;jj++)
					{
						HIS* hh=his_wnd.at<HIS*>(ii,jj);
						delete hh;
					}
				}
				his_wnd.release();
				//		G.release();
				R.release();

				i++;
				//scale =  pow(step,i);
				scale = scale + 2;
				cellSz.width = ( scale+cellSize.width);
				cellSz.height = ( scale+cellSize.height);

				wndSz.width = cellSz.width * tmp.width;
				wndSz.height = cellSz.height * tmp.height;

				slideWnd.width =wndSz.width;
				slideWnd.height =wndSz.height;

			}


		}
	}
	imFils[0].release();
	imFils[1].release();
	G.release();

	//	imshow("max",img(MaxWnd));
	imshow("result",result);
}

void multiscaleExp(Mat img,float step )
{
	Mat result = img.clone();
	Point startP(0,0);
	double scale =1.;
	Size cellSz,wndSz; 
	Size tmp;

	tmp.width = wndSize.width / cellSize.width;
	tmp.height = wndSize.height /cellSize.height;
	float baseWidth=cellSize.width*tmp.width;
	cellSz.width = cellSize.width;
	cellSz.height = cellSize.height;
	wndSz.width = wndSize.width;
	wndSz.height = wndSize.height;
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
	int i =0;
	Mat * weight;
	double b;
	getWeight("w.txt",weight,b);
	Mat* imFils = imFilter(img);
	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
	Rect MaxWnd(0,0,wndSize.width,wndSize.height);
	double max=0;
	Mat img_slideWnd, his_wnd;
	Mat* his=NULL;
	for (int h=wndSize.height;h<img.rows-wndSize.height;h+=80)
	{
		
		for (int w=wndSize.width;w<img.cols-wndSize.width;w+=10)
		{
			startP.x= w;
			startP.y = h;
			scale =1.;

			slideWnd= getRect(startP.x,startP.y,scale);
		//	slideWnd.x = startP.x;
		//	slideWnd.y = startP.y;
			
			cellSz.width = cellSize.width;
			cellSz.height = cellSize.height;

			wndSz.width =slideWnd.width;
			wndSz.height =slideWnd.height;
		//	wndSz.width = wndSize.width;
		//	wndSz.height = wndSize.height;
		//	slideWnd.width =wndSz.width;
		//	slideWnd.height =wndSz.height;

			while( slideWnd.x>=0 && slideWnd.y>=0 && slideWnd.x+slideWnd.width<=img.cols && slideWnd.y+slideWnd.height<=img.rows)
			{


				img_slideWnd=img(slideWnd);


				his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				if(!his)
					his=new Mat(1,h_w->n_bins,CV_64F);
				//printf("%d",h_w->n_bins);
				for (int i=0;i<h_w->n_bins;i++)
				{
					his->at<double>(0,i)=h_w->vector_weight[i];
				}
				Mat R = (*his)* (*weight ) - b;
				double v = R.at<double>(0,0);
				
				if(v>0){
					//					printf(" (%d,%d) (%dx%d) %f %f\n",startP.x,startP.y, wndSz.width,wndSz.height,scale, v);
					printf("%d, %d, %f\n",startP.x+wndSz.width/2,startP.y+wndSz.height/2,wndSz.width/baseWidth);
					rectangle(result,slideWnd,Scalar(0,256,0),2);
					stringstream out;
					out<<v;
					putText(result,out.str(),Point(slideWnd.x,slideWnd.y-3),FONT_HERSHEY_COMPLEX_SMALL,0.5,Scalar(0,256,0));
					stringstream outputfile;
					outputfile <<"img "<<startP.x<<" "<<startP.y<<" "<<wndSz.width<<" "<<wndSz.height;
					//	imshow(outputfile.str(),img_slideWnd);
					//	if(v>max){
					//		max=v;
					//		MaxWnd = slideWnd;
					//	}
				}

				delete h_w;


				//		imFils[0].release();
				//		imFils[1].release();
				for (int ii=0;ii<his_wnd.rows;ii++)
				{
					for (int jj=0;jj<his_wnd.cols;jj++)
					{
						HIS* hh=his_wnd.at<HIS*>(ii,jj);
						delete hh;
					}
				}
				his_wnd.release();
				//		G.release();
				R.release();

				i++;
				//scale =  pow(step,i);
				scale = scale * step;
				cellSz.width = ceil( scale* cellSize.width);
				cellSz.height = ceil( scale* cellSize.height);
				scale = (float)cellSz.width/cellSize.width;
				slideWnd= getRect(startP.x,startP.y,scale);
				wndSz.width =slideWnd.width;
				wndSz.height =slideWnd.height;
			//	wndSz.width = cellSz.width * tmp.width;
			//	wndSz.height = cellSz.height * tmp.height;
				
			//	slideWnd.width =wndSz.width;
			//	slideWnd.height =wndSz.height;
				

				cout<<slideWnd.x<<" "<<slideWnd.y<<" "<<slideWnd.width<<" "<<slideWnd.height<<endl;
			}


		}
	}
	imFils[0].release();
	imFils[1].release();
	G.release();

	//	imshow("max",img(MaxWnd));
	imshow("result",result);
}



Rect getRect(int x,int y, float scale)
{

//	Size cellSz; 
	Size wndSz;
	Size tmp;
	Rect result;
	tmp.width = wndSize.width / cellSize.width;
	tmp.height = wndSize.height /cellSize.height;
	wndSz.width=cellSize.width*tmp.width;
	wndSz.height=cellSize.height*tmp.height;
	//cellSz = cellSize;
//	cellSz.width = ( scale+cellSize.width);
//	cellSz.height = ( scale+cellSize.height);

//	result.width =cellSize.width*tmp.width*scale; 
//	result.height= cellSize.height*tmp.height*scale;
	result.width = wndSz.width*scale;
	result.height = wndSz.height*scale;
	result.x = x - result.width/2;
	result.y = y - result.height/2;
//	printf("%d %d %d %d\n",result.x,result.y,result.width,result.height);
	return result;

}
void drawRect2Img(Mat & img, string rectFile)
{
	ifstream ffile;
	ffile.open (rectFile.c_str());
	string tmp;


	if (ffile.is_open())
	{
		while (! ffile.eof() )
		{
			getline (ffile,tmp);
			std::vector<std::string> strs;
			char* s = (char*)(tmp.c_str());
			boost::split(strs,s , boost::is_any_of(","));
			if (strs.size()<3)
				break;
			//cellSize.width = atoi(tmp.c_str());
			Rect r=getRect((int)atof(strs[0].c_str()),(int)atof(strs[1].c_str()),atof(strs[2].c_str()));
			rectangle(img,r,Scalar(0,256,0),2);





		}
	}
	ffile.close();

}
