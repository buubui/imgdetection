#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include "meanshift.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;
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
				if(!his){
					/*Mat A;
					A=(Mat::zeros(1,h_w->cols,CV_64F));;
					his= &A;*/
			//		his=new Mat(1,h_w->cols,CV_64F);
					his = new Mat();
					*his=Mat::zeros(1,h_w->cols,CV_64F);
				}
				//printf("%d",h_w->n_bins);
				/*for (int i=0;i<h_w->cols;i++)
				{
					his->at<double>(0,i)=h_w->at<double>(0,i);
				}*/
				his = h_w;
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

			//	delete h_w;
			//	his->release();
				delete his;


				//		imFils[0].release();
				//		imFils[1].release();
				for (int ii=0;ii<his_wnd.rows;ii++)
				{
					for (int jj=0;jj<his_wnd.cols;jj++)
					{
						HIS* hh=his_wnd.at<HIS*>(ii,jj);
					//	hh->release();
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

Mat multiscaleExp(string filepath,float step )
{
	Mat img = imread(filepath);
//	Mat img;
	
	double maxSz=maxWndSz.width*maxWndSz.height;
	double minSz = wndSize.width*wndSize.height;
	Rect realRect=resizeImg(img,maxSz,minSz,true);
	/*
	//double t=(double)imgOrg.rows*imgOrg.cols;
	float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
	resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
	resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
	imgOrg.release();*/
	std::vector<std::string> strs;
	char* s = (char*)(filepath.c_str());
	boost::split(strs,s , boost::is_any_of(".\\"));
	string filename ="output/"+ strs[strs.size()-2]+"_multiscale.txt";
	
	stringstream outstr;
	

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
	Size addStep;//=Size(2,2);
	addStep.width=cellSize.width*1.;
	addStep.height=cellSize.height*1.;
	float divStep =sqrt(((maxSz)/(img.rows*img.cols)));
	divStep=divStep<1?1:divStep;
//	divStep=divStep<1.?1:divStep;
	addStep.width =round(addStep.width /divStep);
	addStep.height =round(addStep.height /divStep);
	if(addStep.width<1) addStep.width=1;
	if(addStep.height<1) addStep.height=1;
	printf("addstep %d divstep %f\n",addStep.width,divStep);
	ofstream outtmp;
	//for (int h=tmp.height*cellSize.height/2;h<img.rows-wndSize.height/2;h+=addStep.height)
	for (int h=realRect.y;h<=realRect.y+realRect.height;h+=addStep.height)
	{
		
	//	for (int w=tmp.width*cellSize.width/2;w<img.cols-wndSize.width/2;w+=addStep.width)
		for (int w=realRect.x;w<realRect.x+realRect.width;w+=addStep.width)
		{
			startP.x= w;
			startP.y = h;
			scale =1.;

			slideWnd= getRect(startP.x,startP.y,scale);
		//	cout<<"AAAAA "<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<endl;
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
//				if(!his)
//					his=new Mat(1,h_w->n_bins,CV_64F);
				his = h_w;
			//	cout<<his->cols<<endl;
				
			//	his = his_wnd.at<HIS*>(0,0);
				//printf("%d",h_w->n_bins);
				/*for (int i=0;i<h_w->n_bins;i++)
				{
					his->at<double>(0,i)=h_w->vector_weight[i];
				}*/
			//	for (int ttt=0;ttt<his->cols;ttt++)
			//	{
				//	printf("\n HIS %d %f, ",his->cols,his->at<double>(0,0));
			//	}
				Mat R = (*his)* (*weight ) - b;
				double v = R.at<double>(0,0);
			//	his->release();
				
				R.release();
			//	v>0.068
				if(v>0.){
					//					printf(" (%d,%d) (%dx%d) %f %f\n",startP.x,startP.y, wndSz.width,wndSz.height,scale, v);
					printf("%d, %d, %f, %f\n",startP.x,startP.y,wndSz.width/baseWidth,v);
					outstr<<startP.x<<", "<<startP.y<<", "<<log(wndSz.width/baseWidth)<<", "<<v<<endl;
				//	rectangle(result,slideWnd,Scalar(0,256,0),2);
				//	stringstream outStr;
				//	outStr<<v;
				//	putText(result,outStr.str(),Point(slideWnd.x,slideWnd.y-3),FONT_HERSHEY_COMPLEX_SMALL,0.5,Scalar(0,256,0));
					/*stringstream outputfile;
					outputfile <<"img "<<startP.x<<" "<<startP.y<<" "<<wndSz.width<<" "<<wndSz.height;*/
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
			//	delete his_wnd;
				his_wnd.release();
				//		G.release();
			//	R.release();

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
				

		//		cout<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<endl;
			}


		}
	}
	imFils[0].release();
	imFils[1].release();
	G.release();
	ofstream out;
	out.open(filename.c_str());
	if(out.is_open()){
		out<<outstr.str();
		out.close();
	}
	
	//	imshow("max",img(MaxWnd));
//	imshow("result",result);
	return result;
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
void drawRect2Img(Mat & img, string rectFile,float minValue,Rect realRect,bool isMeanshift)
{
	ifstream ffile;
	ffile.open (rectFile.c_str());
	string tmp;
	Rect rects[1000];
	int n_rect=0;
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
			if(atof(strs[3].c_str())<minValue)
				continue;
			Rect r=getRect((int)atof(strs[0].c_str()),(int)atof(strs[1].c_str()),exp(atof(strs[2].c_str())));
			if(r.x<realRect.x){
				int t = realRect.x-r.x;
				r.x=realRect.x;
				r.width -=t;
			}
			if(r.x+r.width>realRect.x+realRect.width)
			{
				if(r.x>realRect.x+realRect.width)
				{
					int tt=r.x-realRect.x+realRect.width;
					r.x=realRect.x+realRect.width;
					r.width-=tt;
				}
				int t =r.x+r.width- realRect.x-realRect.width;
			//	r.x=realRect.x+realRect.width;
				r.width -=t;
			}
			if(r.y<realRect.y)
			{
				int t = realRect.y-r.y;
				r.y=realRect.y;
				r.height -=t;
			}
			if(r.y+r.height>realRect.y+realRect.height)
			{
				if(r.y>realRect.y+realRect.height)
				{
					int tt=r.y-realRect.y+realRect.height;
					r.y=realRect.y+realRect.height;
					r.height-=tt;
				}
				int t =r.y+r.height- realRect.y-realRect.height;
				//	r.x=realRect.x+realRect.width;
				r.height -=t;
			}
			if(isMeanshift)
				rects[n_rect++]=r;
		//	printf("%d %d %d %d\n",r.x,r.y,r.width,r.height);
			if(!isMeanshift)
				rectangle(img,r,Scalar(0,256,0),2);

		}
		ffile.close();
	}else
		printf("Cannot open file %s\n",rectFile.c_str());
	if(!isMeanshift)
		return;
	for (int i=0;i<n_rect-1;i++)
	{
		for (int j=i+1;j<n_rect;j++)
		{
			int maxX1= rects[i].x>rects[j].x?rects[i].x:rects[j].x;
			int maxY1= rects[i].y>rects[j].y?rects[i].y:rects[j].y;

			int minX1= rects[i].x<rects[j].x?rects[i].x:rects[j].x;
			int minY1= rects[i].y<rects[j].y?rects[i].y:rects[j].y;
			Point p1=Point(maxX1,maxY1);
			if(!rects[i].contains(p1)|| !rects[j].contains(p1))
				continue;
			int minX2= rects[i].x+rects[i].width<rects[j].x+rects[j].width?rects[i].x+rects[i].width:rects[j].x+rects[j].width;
			int minY2= rects[i].y+rects[i].height<rects[j].y+rects[j].height?rects[i].y+rects[i].height:rects[j].y+rects[j].height;

			int maxX2= rects[i].x+rects[i].width>rects[j].x+rects[j].width?rects[i].x+rects[i].width:rects[j].x+rects[j].width;
			int maxY2= rects[i].y+rects[i].height>rects[j].y+rects[j].height?rects[i].y+rects[i].height:rects[j].y+rects[j].height;
			Point p2(minX2,minY2);

			Rect cutR(p1,p2);
			Rect* minR = rects[i].area()<rects[j].area()?&rects[i]:&rects[j];
			Rect* maxR;
			if(minR==&rects[i])
				maxR=&rects[j];
			else maxR=&rects[i];
			double r1 =(double)cutR.area()/minR->area();
//			double r2 =(double)cutR.area()/rects[i].area();
			if(r1>0.85)
			{
				*minR=Rect(0,0,0,0);
				*maxR=Rect(Point(minX1,minY1),Point(maxX2,maxY2));
			}/*else if(r2>0.85)
			{
				rects[i]=Rect(0,0,0,0);
				rects[j]=Rect(Point(minX1,minY1),Point(maxX2,maxY2));
				break;
			}*/
			else{
				Point m1(maxR->x+maxR->width/2,maxR->y+maxR->height/2);
				Point m2(minR->x+minR->width/2,minR->y+minR->height/2);
				int dx =abs( m1.x-m2.x);
				int dy=abs(m1.y=m2.y);
				
				float a =180.0* atan(1.*dx/dy)/M_PI;

				if(a<13&&r1>0.15){
					*minR=Rect(0,0,0,0);
					*maxR=Rect(Point(minX1,minY1),Point(maxX2,maxY2));
					printf("ATAN %d %d %f\n",dx,dy,a);
				}
			}

		}
	}
	for (int i=0;i<n_rect;i++)
	{
		rectangle(img,rects[i],Scalar(0,256,0),2);
	
	}
}
