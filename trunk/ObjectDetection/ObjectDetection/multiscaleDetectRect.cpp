#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include "meanshift.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;
extern cv::Vec2i blockOverlap;
extern cv::Vec2f regionOverlap;
extern float delPart;
void multiscale(Mat img,float step )
{
	Mat result = img.clone();
	Point startP(0,0);
	float scale =0.;
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
	float b;
	getWeight("w.txt",weight,b);
	Mat* imFils = imFilter(img,true);
	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
	Rect MaxWnd(0,0,wndSize.width,wndSize.height);
	float max=0;
	Mat img_slideWnd, his_wnd;
	HIS h_w;
	
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


			//	his_wnd = calcHisOfCellsInWnd(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd,180.);
			//	HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);
			//	if(!his){
					/*Mat A;
					A=(Mat::zeros(1,h_w->cols,DataType<float>::type));;
					his= &A;*/
			//		his=new Mat(1,h_w->cols,DataType<float>::type);
			/*		his = new Mat();
					*his=Mat::zeros(1,h_w.cols,DataType<float>::type);
				}*/
				//printf("%d",h_w->n_bins);
				/*for (int i=0;i<h_w->cols;i++)
				{
					his->at<float>(0,i)=h_w->at<float>(0,i);
				}*/
			
				Mat R = (h_w)* (*weight ) - b;
				float v = R.at<float>(0,0);
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
				


				//		imFils[0].release();
				//		imFils[1].release();
				//for (int ii=0;ii<his_wnd.rows;ii++)
				//{
				//	for (int jj=0;jj<his_wnd.cols;jj++)
				//	{
				//		HIS* hh=his_wnd.at<HIS*>(ii,jj);
				//	//	hh->release();
				//		delete hh;
				//	}
				//}
				//his_wnd.release();
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
	h_w.release();
	G.release();

	//	imshow("max",img(MaxWnd));
	imshow("result",result);
}

//Mat multiscaleExp(string filepath,float step,Size addStep,bool useMaxChannel)
//{
//	Mat img = imread(filepath);
////	Mat img;
//	
//	float maxSz=maxWndSz.width*maxWndSz.height;
//	float minSz = wndSize.width*wndSize.height;
//	Rect realRect=resizeImg(img,maxSz,minSz,true);
//	/*
//	//float t=(float)imgOrg.rows*imgOrg.cols;
//	float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
//	resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
//	resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
//	imgOrg.release();*/
//	std::vector<std::string> strs;
//	char* s = (char*)(filepath.c_str());
//	boost::split(strs,s , boost::is_any_of("/\\"));
//
//	string fname = strs[strs.size()-1];
//	int dot=fname.rfind('.',fname.size()-1);
//
//	fname=fname.substr(0,dot);
//	cout<<"FILENAME:"<<fname<<endl;
//	string filename ="output/"+ fname+"_multiscale.txt";
//	
//	stringstream outstr;
//	
//
//	Mat result = img.clone();
//	Point startP(0,0);
//	float scale =1.;
//	Size cellSz,wndSz; 
//	Size tmp;
//
//	tmp.width = wndSize.width / cellSize.width;
//	tmp.height = wndSize.height /cellSize.height;
//	float baseWidth=cellSize.width*tmp.width;
//	cellSz.width = cellSize.width;
//	cellSz.height = cellSize.height;
//	wndSz.width = wndSize.width;
//	wndSz.height = wndSize.height;
//	Rect slideWnd(0,0,wndSize.width,wndSize.height);
//	int i =0;
//	Mat * weight;
//	float b;
//	getWeight("w.txt",weight,b);
//	int n_channels=1;
//	Mat* imFils;
//	Mat G;
//	if(useMaxChannel==true)
//	{
//		n_channels=img.channels();
//		imFils = imFilterChannels(img,true);
//		G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
//	}else{
//		imFils = imFilter(img,true);
//		G = calcGradientOfPixels(imFils[0],imFils[1]);
//	}
//	Rect MaxWnd(0,0,wndSize.width,wndSize.height);
//	float max=0;
//	Mat img_slideWnd, his_wnd;
//	HIS h_w;
////	Mat img2=img.clone();
//	Mat img_gray;
//	cvtColor(img,img_gray,CV_BGR2GRAY);
////	equalizeHist(img_gray,img_gray);
//	Mat img2;
//	img_gray.convertTo(img2,DataType<float>::type,1./255);
//	img_gray.release();
////	Size addStep;//=Size(2,2);
////	addStep.width=cellSize.width*1.;
////	addStep.height=cellSize.height*1.;
//	float divStep =sqrt(((maxSz)/(img.rows*img.cols)));
//	divStep=divStep<1?1:divStep;
////	divStep=divStep<1.?1:divStep;
//	addStep.width =round(addStep.width /divStep);
//	addStep.height =round(addStep.height /divStep);
//	if(addStep.width<1) addStep.width=1;
//	if(addStep.height<1) addStep.height=1;
//
////	addStep=Size(1,1);
//
//	printf("addstep %d divstep %f\n",addStep.width,divStep);
//	ofstream outtmp;
//	//for (int h=tmp.height*cellSize.height/2;h<img.rows-wndSize.height/2;h+=addStep.height)
//	int colFound1=0,colFound2=0;
//	int addH=addStep.height;
//	int addW=addStep.width;
////	float sizeLim=std::max((float)wndSize.width*wndSize.height,(float)realRect.height/3*realRect.width/3);
//	float sizeLim=(float)wndSize.width*wndSize.height*4;
//	Mat* imFils2=imFilter(img,false);
//	Mat wEdges=Mat::zeros(realRect.height-2,2,DataType<int>::type);
///************************************************************************/
///*                    Fill into wEdges                                 */
///************************************************************************/
//	for (int hh=realRect.y+2;hh<realRect.y+realRect.height-2;hh+=addH){
//		int ih=hh-realRect.y-2;
//		wEdges.at<int>(ih,0)=realRect.x;
//		wEdges.at<int>(ih,1)=realRect.x+realRect.width;
//		for (int ww=realRect.x+2;ww<realRect.x+realRect.width-2;ww++)
//		{
//			int t= ww-realRect.x-2;
//			if(wEdges.at<int>(ih,0)==realRect.x){
//			//		Gradient g1 =G.at<Gradient>(Point(ww,hh));
//			//		float v=g1[1];
//				float x = imFils2[0].at<float>(hh,ww);
//				float y = imFils2[1].at<float>(hh,ww);
//				float v=sqrt(x*x+y*y);
//				if(v>=0.08 ){
//					//	if(g1[1]>=0.2){
//					if(wEdges.at<int>(ih,0)<=wEdges.at<int>(ih,1))
//						wEdges.at<int>(ih,0)=ww;
//				}else
//					img2.at<float>(hh,ww)=0.;
//
//			}
//			if(wEdges.at<int>(ih,1)==realRect.x+realRect.width){
//		//			Gradient g2 =G.at<Gradient>(Point(ww,realRect.y+realRect.height-2-t));
//		//			float v = g2[1];
//				//		printf("%d %f %d\n",realRect.y+realRect.height-t,g2[1],t);
//				float x = imFils2[0].at<float>(hh,realRect.x+realRect.width-2-t);
//				float y = imFils2[1].at<float>(hh,realRect.x+realRect.width-2-t);
//				float v=sqrt(x*x+y*y);
//				if(v>=0.08 ){
//					//if(g2[1]>=0.2){
//
//					if(realRect.x+realRect.width-2-t>=wEdges.at<int>(ih,0))
//						wEdges.at<int>(ih,1)=realRect.x+realRect.width-2-t;
//				}else
//					img2.at<float>(hh,realRect.x+realRect.width-1-t)=0.;
//			}
//			if( wEdges.at<int>(ih,0)!=realRect.x&&
//				wEdges.at<int>(ih,1)!=realRect.x+realRect.width)
//			{
//				//		printf("DONE %d   %d\n",firstEdge,lastEdge);
//				break;
//			}
//		}
//		if( wEdges.at<int>(ih,0)==realRect.x&&
//			wEdges.at<int>(ih,1)==realRect.x+realRect.width)
//		{
//			wEdges.at<int>(ih,0)= wEdges.at<int>(ih,1)=realRect.x+2;
//		}
//					printf("%d: %d -> %d, %d : %d %d\n",hh,wEdges.at<int>(ih,0),wEdges.at<int>(ih,1),realRect.x,realRect.x+realRect.width,ih);
//	}
////////////////////////////////////////////////////////////////////////////
//	for (int w=realRect.x+2;w<realRect.x+realRect.width-2;w+=addW)
//	{
//		int lastfound=0;
//		int firstEdge=realRect.y;
//		int lastEdge=realRect.y+realRect.height;
//		//////////////////////////////////////////////////////////////////////////
//		for (int hh=realRect.y+2;hh<realRect.y+realRect.height-2;hh+=1)
//		{
//			int t= hh-realRect.y-2;
//			if(firstEdge==realRect.y){
//				//	Gradient g1 =G.at<Gradient>(Point(w,hh));
//				//	float v = g1[1];
//				float x = imFils2[0].at<float>(hh,w);
//				float y = imFils2[1].at<float>(hh,w);
//				float v=sqrt(x*x+y*y);
//				if(v>=0.08){
//			//	if(g1[1]>=0.2){
//					if(firstEdge<=lastEdge)
//						firstEdge=hh;
//				}else 
//					img2.at<float>(hh,w)=0.;
//
//			}
//			if(lastEdge==realRect.y+realRect.height){
//				//	Gradient g2 =G.at<Gradient>(Point(w,realRect.y+realRect.height-2-t));
//				//	float v = g2[1];
//				//		printf("%d %f %d\n",realRect.y+realRect.height-t,g2[1],t);
//				float x = imFils2[0].at<float>(realRect.y+realRect.height-2-t,w);
//				float y = imFils2[1].at<float>(realRect.y+realRect.height-2-t,w);
//				float v=sqrt(x*x+y*y);
//				if(v>=0.08){
//				//if(g2[1]>=0.2){
//
//					if(realRect.y+realRect.height-t>=firstEdge)
//						lastEdge=realRect.y+realRect.height-2-t;
//				}else 
//					img2.at<float>(realRect.y+realRect.height-2-t,w)=0.;
//			}
//			if(firstEdge!=realRect.y&&lastEdge!=realRect.y+realRect.height)
//			{
//				//		printf("DONE %d   %d\n",firstEdge,lastEdge);
//				break;
//			}
//		}
//		if(firstEdge==realRect.y&&lastEdge==realRect.y+realRect.height)
//		{
//			firstEdge=lastEdge=realRect.y+2;
//		}
//		//////////////////////////////////////////////////////////////////////////
//		int oldH=addH;
//		addH=addStep.height;
//		addW=addStep.width;
//		startP.x= w;
//		if(!colFound2){
//				float t=std::abs(lastEdge-firstEdge);
//				t= (t/wndSize.height);
//				sizeLim=min((float)wndSize.width*wndSize.height*4,t*t*wndSize.width*wndSize.height*2*2);
//		
//			}
//	//	printf("%d: %d %d: %d %d\n",startP.x,addW,addH,colFound1,colFound2);
//		colFound1=colFound2;
//		colFound2=0;
//	//	sizeLim=(float)wndSize.width*wndSize.height*4;
//	//	for (int w=tmp.width*cellSize.width/2;w<img.cols-wndSize.width/2;w+=addStep.width)
//		int firstH=realRect.y+2;
//
//		//////////////////////////////////////////////////////////////////////////
//		for (int h=realRect.y+2;h<=realRect.y+realRect.height-2;h+=addH)
//		
//	//	for (int h=firstEdge;h<=lastEdge;h+=addH)
//		{
//			int ih=h-realRect.y-2;
//			if(firstEdge-h>addH){
//				int ht=(firstEdge-h)/addH;
//				h+=ht*addH;
//				firstH=h;
//			}
//			if(h>lastEdge)
//				break;
//			
//			if(w<wEdges.at<int>(ih,0)||w>wEdges.at<int>(ih,1)){
//				continue;
//			}
//			startP.y = h;	
//	
//	//		Gradient g =G.at<Gradient>(Point(startP.x,startP.y));
//
//		//	if(colFound2==0 &&g[1]<=0.001){
//			//	printf("skip %d %d\n",startP.x,startP.y);
//			//	img2.at<float>(startP.y,startP.x)=0.;
//	//			continue;
//		//	}
//			scale =1.;
//			slideWnd= getRect(startP.x,startP.y,scale);
//			cellSz.width = cellSize.width;
//			cellSz.height = cellSize.height;
//			wndSz.width =slideWnd.width;
//			wndSz.height =slideWnd.height;
//			while( slideWnd.x>=0 && slideWnd.y>=0 && slideWnd.x+slideWnd.width<=img.cols && 
//				slideWnd.y+slideWnd.height<=img.rows && slideWnd.width* slideWnd.height<=sizeLim)
//			{
//				img_slideWnd=img(slideWnd);
//				
//				calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd,180.);
//			//	calcHisOfCellsInWnd(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
//			//	if(his_wnd.rows==0)
//				calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);
//
//				Mat R = (h_w)* (*weight ) - b;
//				float v = R.at<float>(0,0);
//				
//				
//				R.release();
//			//	v>0.068
//			//	float v=1;
//				if(v>0.){
//					printf("%d, %d, %f, %f, (%d, %d), (%d, %d), %d\n",startP.x,startP.y,wndSz.width/baseWidth,v,firstEdge,lastEdge,wEdges.at<int>(ih,0),wEdges.at<int>(ih,1),ih);
//					outstr<<startP.x<<", "<<startP.y<<", "<<log(wndSz.width/baseWidth)<<", "<<v<<endl;
//					lastfound=h;
//						if(!colFound2)
//							colFound1=colFound2;
//						colFound2++;
//						float t=std::max(lastEdge,firstEdge)-std::min(lastEdge,firstEdge);
//						t= (t/wndSize.height);
//						float largeSzLim=t*t*wndSize.width*wndSize.height*1.5*1.5;
//						if(sizeLim<largeSzLim){
//							sizeLim=largeSzLim;
//							if(h>realRect.y){
//								if(firstEdge>realRect.y+2){
//									h-=min(3*addH,h-firstH+addH);
//								}
//								break;
//							}
//						}
//						addH=addStep.height;
//				}
//				i++;
//				scale = scale * step;
//				float w=scale* cellSize.width;
//				float h=scale* cellSize.height;
//				if(round(w)==cellSz.width && round(h)==cellSz.height)
//				{
//					cellSz.width = ceil( w);
//					cellSz.height = ceil( h);
//					scale = (float)cellSz.width/cellSize.width;
//				}else{
//					cellSz.width = round( w);
//					cellSz.height = round( h);
//				}
//				slideWnd= getRect(startP.x,startP.y,scale);
//				wndSz.width =slideWnd.width;
//				wndSz.height =slideWnd.height;
//			}
//
//
//		}
//	}
////	imFils[0].release();
////	imFils[1].release();
//	for (int i=0;i<n_channels;i++)
//	{
//		imFils[i].release();
//		imFils[i+1].release();
//	}
//	delete[] imFils;
//	imFils2[0].release();
//	imFils2[1].release();
//	for (int ii=0;ii<his_wnd.rows;ii++)
//	{
//		for (int jj=0;jj<his_wnd.cols;jj++)
//		{
//			HIS* hh=his_wnd.at<HIS*>(ii,jj);
//			delete hh;
//		}
//	}
//	//	delete his_wnd;
//	his_wnd.release();
//	h_w.release();
//	G.release();
//	ofstream out;
//	out.open(filename.c_str());
//	if(out.is_open()){
//		out<<outstr.str();
//		out.close();
//	}
//	
//	//	imshow("max",img(MaxWnd));
////	imshow("result",result);
//	imshow("dasd",img2);
//	img2.release();
//	return result;
//}
//
//
//
extern int n_x2,n_y2,n_x3,n_y3,n_x4, n_y4, n_x5, n_y5;
extern int* x_corr2, *x_corr3,*x_corr4,*x_corr5,*y_corr2,*y_corr3,*y_corr4,*y_corr5;
extern Mat M_cellsInBlock1,M_cellsInBlock2,M_cellsInBlock3,M_cellsInBlock4, M_cellsInBlock5 ;
//Mat multiscaleExp(string filepath,float step,Size addStep,bool useMaxChannel)
Mat multiscaleExp(string filepath,float step,Size addStep, int normType, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech,string weightFile)
{
	Mat img = imread(filepath);
	//	Mat img;
	step=1.2;
	float maxSz=maxWndSz.width*maxWndSz.height;
	float minSz = wndSize.width*wndSize.height;
//	imshow("o1",img);
	Rect realRect=resizeImg(img,maxSz,minSz,true);
	
//	imshow("o2",img);
	/*
	//float t=(float)imgOrg.rows*imgOrg.cols;
	float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
	resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
	resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
	imgOrg.release();*/
	std::vector<std::string> strs;
	char* s = (char*)(filepath.c_str());
	boost::split(strs,s , boost::is_any_of("/\\"));

	string fname = strs[strs.size()-1];
	int dot=fname.rfind('.',fname.size()-1);

	fname=fname.substr(0,dot);
	cout<<"FILENAME:"<<fname<<endl;
	string filename ="output/"+ fname+"_multiscale.txt";

	stringstream outstr;


	Mat result = img.clone();
	Point startP(0,0);
	float scale =1.;
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
	float b;
	//get detection vector
	getWeight(weightFile,weight,b);
	int n_channels=1;
	Mat* imFils;
	Mat G;
	Mat his_cells_wnd ;
	HIS his_wind;
	HIS* h_ws=NULL;
	int* x_corr=NULL;int* y_corr=NULL; int n_x=0, n_y=0;
	/*if(useMaxChannel==true)
	{
		n_channels=img.channels();
		imFils = imFilterChannels(img,true);
		G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
	}else{
		imFils = imFilter(img,true);
		G = calcGradientOfPixels(imFils[0],imFils[1]);
	}*/
	Rect MaxWnd(0,0,wndSize.width,wndSize.height);
	float max=0;
	Mat img_slideWnd, his_wnd;
	HIS h_w;
	//	Mat img2=img.clone();
	Mat img_gray;

//	cvtColor(img,img_gray,CV_BGR2GRAY);
	n_channels=img.channels();
	Mat * imgs = new Mat[n_channels];
	cv::split(img,imgs);
	img_gray=imgs[0];
	//	equalizeHist(img_gray,img_gray);
	Mat img2;
	img_gray.convertTo(img2,DataType<float>::type,1./255);
	img_gray.release();
	//	Size addStep;//=Size(2,2);
	//	addStep.width=cellSize.width*1.;
	//	addStep.height=cellSize.height*1.;
	//float divStep =sqrt(((maxSz)/(img.rows*img.cols)));
	//divStep=divStep<1?1:divStep;
	////	divStep=divStep<1.?1:divStep;
	//addStep.width =round(addStep.width /divStep);
	//addStep.height =round(addStep.height /divStep);
	//if(addStep.width<1) addStep.width=1;
	//if(addStep.height<1) addStep.height=1;

	//	addStep=Size(1,1);

//	printf("addstep %d divstep %f\n",addStep.width,divStep);
	ofstream outtmp;
	//for (int h=tmp.height*cellSize.height/2;h<img.rows-wndSize.height/2;h+=addStep.height)
//	int colFound1=0,colFound2=0;
	int addH=addStep.height;
	int addW=addStep.width;
	//	float sizeLim=std::max((float)wndSize.width*wndSize.height,(float)realRect.height/3*realRect.width/3);
	float sizeLim=(float)wndSize.width*wndSize.height*4;
	Mat* imFils2=imFilter(img,false);
	Mat wEdges=Mat::zeros(realRect.height-2,2,DataType<int>::type);
	/************************************************************************/
	/*                    Fill into wEdges                                 */
	/************************************************************************/
	float del_lim=0.08;
//	float del_lim=-10;
	for (int hh=realRect.y+2;hh<realRect.y+realRect.height-2;hh+=addH){
		int ih=hh-realRect.y-2;
		wEdges.at<int>(ih,0)=realRect.x;
		wEdges.at<int>(ih,1)=realRect.x+realRect.width;
		for (int ww=realRect.x+2;ww<realRect.x+realRect.width-2;ww++)
		{
			int t= ww-realRect.x-2;
			if(wEdges.at<int>(ih,0)==realRect.x){
				//		Gradient g1 =G.at<Gradient>(Point(ww,hh));
				//		float v=g1[1];
				float x = imFils2[0].at<float>(hh,ww);
				float y = imFils2[1].at<float>(hh,ww);
				float v=sqrt(x*x+y*y);
				if(v>=del_lim ){
					//	if(g1[1]>=0.2){
					if(wEdges.at<int>(ih,0)<=wEdges.at<int>(ih,1))
						wEdges.at<int>(ih,0)=ww;
				}else
					img2.at<float>(hh,ww)=0.;

			}
			if(wEdges.at<int>(ih,1)==realRect.x+realRect.width){
				//			Gradient g2 =G.at<Gradient>(Point(ww,realRect.y+realRect.height-2-t));
				//			float v = g2[1];
				//		printf("%d %f %d\n",realRect.y+realRect.height-t,g2[1],t);
				float x = imFils2[0].at<float>(hh,realRect.x+realRect.width-2-t);
				float y = imFils2[1].at<float>(hh,realRect.x+realRect.width-2-t);
				float v=sqrt(x*x+y*y);
				if(v>=del_lim ){
					//if(g2[1]>=0.2){

					if(realRect.x+realRect.width-2-t>=wEdges.at<int>(ih,0))
						wEdges.at<int>(ih,1)=realRect.x+realRect.width-2-t;
				}else
					img2.at<float>(hh,realRect.x+realRect.width-1-t)=0.;
			}
			if( wEdges.at<int>(ih,0)!=realRect.x&&
				wEdges.at<int>(ih,1)!=realRect.x+realRect.width)
			{
				//		printf("DONE %d   %d\n",firstEdge,lastEdge);
				break;
			}
		}
		if( wEdges.at<int>(ih,0)==realRect.x&&
			wEdges.at<int>(ih,1)==realRect.x+realRect.width)
		{
			wEdges.at<int>(ih,0)= wEdges.at<int>(ih,1)=realRect.x+2;
		}
		printf("%d: %d -> %d, %d : %d %d\n",hh,wEdges.at<int>(ih,0),wEdges.at<int>(ih,1),realRect.x,realRect.x+realRect.width,ih);
	}
	//////////////////////////////////////////////////////////////////////////
	for (int w=realRect.x+2;w<realRect.x+realRect.width-2;w+=addW)
	{
	//	int lastfound=0;
		int firstEdge=realRect.y;
		int lastEdge=realRect.y+realRect.height;
		//////////////////////////////////////////////////////////////////////////
		for (int hh=realRect.y+2;hh<realRect.y+realRect.height-2;hh+=1)
		{
			int t= hh-realRect.y-2;
			if(firstEdge==realRect.y){
				//	Gradient g1 =G.at<Gradient>(Point(w,hh));
				//	float v = g1[1];
				float x = imFils2[0].at<float>(hh,w);
				float y = imFils2[1].at<float>(hh,w);
				float v=sqrt(x*x+y*y);
				if(v>=del_lim){
					//	if(g1[1]>=0.2){
					if(firstEdge<=lastEdge)
						firstEdge=hh;
				}else 
					img2.at<float>(hh,w)=0.;

			}
			if(lastEdge==realRect.y+realRect.height){
				//	Gradient g2 =G.at<Gradient>(Point(w,realRect.y+realRect.height-2-t));
				//	float v = g2[1];
				//		printf("%d %f %d\n",realRect.y+realRect.height-t,g2[1],t);
				float x = imFils2[0].at<float>(realRect.y+realRect.height-2-t,w);
				float y = imFils2[1].at<float>(realRect.y+realRect.height-2-t,w);
				float v=sqrt(x*x+y*y);
				if(v>=del_lim){
					//if(g2[1]>=0.2){

					if(realRect.y+realRect.height-t>=firstEdge)
						lastEdge=realRect.y+realRect.height-2-t;
				}else 
					img2.at<float>(realRect.y+realRect.height-2-t,w)=0.;
			}
			if(firstEdge!=realRect.y&&lastEdge!=realRect.y+realRect.height)
			{
				//		printf("DONE %d   %d\n",firstEdge,lastEdge);
				break;
			}
		}
		if(firstEdge==realRect.y&&lastEdge==realRect.y+realRect.height)
		{
			firstEdge=lastEdge=realRect.y+2;
		}
		//////////////////////////////////////////////////////////////////////////
	//	int oldH=addH;
		addH=addStep.height;
		addW=addStep.width;
		startP.x= w;
		/*if(!colFound2){
			float t=std::abs(lastEdge-firstEdge);
			t= (t/wndSize.height);
			sizeLim=min((float)wndSize.width*wndSize.height*4,t*t*wndSize.width*wndSize.height*2*2);

		}*/
		//	printf("%d: %d %d: %d %d\n",startP.x,addW,addH,colFound1,colFound2);
	//	colFound1=colFound2;
	//	colFound2=0;
		//	sizeLim=(float)wndSize.width*wndSize.height*4;
		//	for (int w=tmp.width*cellSize.width/2;w<img.cols-wndSize.width/2;w+=addStep.width)
		int firstH=realRect.y+2;

		//////////////////////////////////////////////////////////////////////////
		for (int h=realRect.y+2;h<=realRect.y+realRect.height-2;h+=addH)

			//	for (int h=firstEdge;h<=lastEdge;h+=addH)
		{
			int ih=h-realRect.y-2;
			if(firstEdge-h>addH){
				int ht=(firstEdge-h)/addH;
				h+=ht*addH;
				firstH=h;
			}
			if(h>lastEdge)
				break;

			if(w<wEdges.at<int>(ih,0)||w>wEdges.at<int>(ih,1)){
				continue;
			}
			startP.y = h;	

			//		Gradient g =G.at<Gradient>(Point(startP.x,startP.y));

			//	if(colFound2==0 &&g[1]<=0.001){
			//	printf("skip %d %d\n",startP.x,startP.y);
			//	img2.at<float>(startP.y,startP.x)=0.;
			//			continue;
			//	}
			scale =1.;
			slideWnd= getRect(startP.x,startP.y,scale);
			cellSz.width = cellSize.width;
			cellSz.height = cellSize.height;
			wndSz.width =slideWnd.width;
			wndSz.height =slideWnd.height;
			Mat img_small;
			Rect slideWnd_small;
			Mat G_small;
			Mat* imFils_small;
	//		imshow("ori",img);
			while( slideWnd.x>=0 && slideWnd.y>=0 && slideWnd.x+slideWnd.width<=img.cols && 
				slideWnd.y+slideWnd.height<=img.rows )// && slideWnd.width* slideWnd.height<=sizeLim)
			{
				img_slideWnd=img(slideWnd);
		//		printf("slidewd: x=%d,y=%d,w=%d,h=%d; img:%dx%d\n",slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height,img.cols,img.rows);
				if (useNewTech>=3&&( cellSz.width*blockSize.width*4>slideWnd.width || cellSz.height*blockSize.height*4>slideWnd.height))
				{
					break;
				}
				Mat img_small=img(slideWnd).clone();
				Rect slideWnd_small=resizeImg(img_small,minSz,minSz,false);
	//			imshow("aaa",img_small);
	//			return result;
				/*if (slideWnd.width>wndSize.width*1.2)
				{
					printf("%d %d \n",img_small.rows,img_small.cols);
				imshow("small",img_small);
				imshow("small_ori",img(slideWnd));
				return result;
				}*/
				

			//	Mat G_small;
			//	Mat* imFils_small;
					if(useMaxChannel==true)
					{
						n_channels=img_small.channels();
						imFils_small = imFilterChannels(img_small,true);
						G_small = calcGradientOfPixelsMaxChannel(imFils_small,n_channels);
					}else{
						imFils_small = imFilter(img_small,true);
						G_small = calcGradientOfPixels(imFils_small[0],imFils_small[1]);
					}
					
					float scale_small=1.;
			//		printf("%d %d %d %d || %d %d \n",slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height,wndSz.width,wndSz.height);
					svmClassify(img_small,G_small,slideWnd_small,cellSize,scale_small,n_x,n_y,x_corr,y_corr,his_cells_wnd,his_wind,h_ws,normType,useMaxChannel,useSmooth,useLBP,useNewTech);
					
					
					G_small.release();
					img_small.release();
					for (int i=0;i<n_channels;i++)
					{
						imFils_small[i].release();
						imFils_small[i+1].release();
					}
					delete[] imFils_small;
			//	svmClassify(img,G,slideWnd,cellSz,scale,n_x,n_y,x_corr,y_corr,his_cells_wnd,his_wind,h_ws,normType,useMaxChannel,useSmooth,useLBP,useNewTech);
				//	svmClassify(img(slideWnd),G(slideWnd),Rect(0,0,slideWnd.width,slideWnd.height),cellSz,scale,n_x,n_y,x_corr,y_corr,his_cells_wnd,his_wind,h_ws,normType,useMaxChannel,useSmooth,useLBP,useNewTech);

				
				/////////////////////////////////////////////////////////
		//		calcHisOfCellsInWnd2(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins,Mat& H,float maxD)
		//		calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd,180.);
				//	calcHisOfCellsInWnd(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
				//	if(his_wnd.rows==0)
		//		calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);
		//		printf("h=%d x %d",his_wind.rows,his_wind.cols);
		//		printf("w=%d x %d",weight->rows,weight->cols);
					h_w = his_wind;
					if(weight->rows<his_wind.cols)
					{
						//	Mat tmp=(*weight);		//

						//	*weight=Mat::zeros(his_wind.cols,1,weight->type());
						//	Mat m=(*weight)(Rect(0,0,1,tmp.rows));
						//	tmp.copyTo(m);

						h_w=his_wind(Rect(0,0,weight->rows,1));
					}
				//	R = (his_wind)* (*weight ) - b;
				Mat R = (h_w)* (*weight ) - b;
				float v = R.at<float>(0,0);
			//		float v=0;
		//		float v=0;

				R.release();
				//	v>0.068
				//	float v=1;
				
			//	printf("%d, %d, %f, %f, (%d, %d), (%d, %d), %d\n",startP.x,startP.y,wndSz.width/baseWidth,v,firstEdge,lastEdge,wEdges.at<int>(ih,0),wEdges.at<int>(ih,1),ih);
				if(v>0.){
					printf("%d, %d, %f, %f, (%d, %d), (%d, %d), %d\n",startP.x,startP.y,wndSz.width/baseWidth,v,firstEdge,lastEdge,wEdges.at<int>(ih,0),wEdges.at<int>(ih,1),ih);
					outstr<<startP.x<<", "<<startP.y<<", "<<log(wndSz.width/baseWidth)<<", "<<v<<endl;
				/*	lastfound=h;
					if(!colFound2)
						colFound1=colFound2;
					colFound2++;*/
				/*	float t=std::max(lastEdge,firstEdge)-std::min(lastEdge,firstEdge);
					t= (t/wndSize.height);
					float largeSzLim=t*t*wndSize.width*wndSize.height*1.5*1.5;
					if(sizeLim<largeSzLim){
						sizeLim=largeSzLim;
						if(h>realRect.y){
							if(firstEdge>realRect.y+2){
								h-=min(3*addH,h-firstH+addH);
							}
							break;
						}
					}*/
					addH=addStep.height;
				}
				i++;
				scale = scale * step;
				float w=scale* cellSize.width;
				float h=scale* cellSize.height;
				if(round(w)==cellSz.width && round(h)==cellSz.height)
				{
					cellSz.width = ceil( w);
					cellSz.height = ceil( h);
					scale = (float)cellSz.width/cellSize.width;
				}else{
					cellSz.width = round( w);
					cellSz.height = round( h);
				}
				scale=1.0*cellSz.width/cellSize.width;
				slideWnd= getRect(startP.x,startP.y,scale);
				wndSz.width =slideWnd.width;
				wndSz.height =slideWnd.height;

				
				

			}


		}
	}
	//	imFils[0].release();
	//	imFils[1].release();
	if (imFils!=NULL)
		for (int i=0;i<n_channels;i++)
		{
			imFils[i].release();
			imFils[i+1].release();
		}
	delete[] imFils;
	imFils2[0].release();
	imFils2[1].release();
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
//	h_w.release();
	G.release();
	ofstream out;
	out.open(filename.c_str());
	if(out.is_open()){
		out<<outstr.str();
		out.close();
	}

	//	imshow("max",img(MaxWnd));
	//	imshow("result",result);
//	imshow("dasd",img2);
	//imshow("w",wEdges);
	img2.release();
	if(h_ws!=NULL){
		for (int i=0;i<4;i++)
		{
			h_ws[i].release();
		}
		delete[]h_ws;
	}
	for (int ii=0;ii<his_cells_wnd.rows;ii++)
	{
		for (int jj=0;jj<his_cells_wnd.cols;jj++)
		{
			HIS* hh=his_cells_wnd.at<HIS*>(ii,jj);
			hh->release();
			delete hh;
		}
	}
	his_cells_wnd.release();
	his_wind.release();
	M_cellsInBlock1.release();
	M_cellsInBlock2.release();
	M_cellsInBlock3.release();
	M_cellsInBlock4.release();
	M_cellsInBlock5.release();
	if(x_corr!=NULL)
		delete[] x_corr;
	if(y_corr!=NULL)
		delete[] y_corr;
	if(x_corr2!=NULL)
		delete[] x_corr2;
	if(y_corr2!=NULL)
		delete[] y_corr2;
	if(x_corr3!=NULL)
		delete[] x_corr3;
	if(y_corr3!=NULL)
		delete[] y_corr3;
	if(x_corr4!=NULL)
		delete[] x_corr4;
	if(y_corr4!=NULL)
		delete[] y_corr4;
	if(x_corr5!=NULL)
		delete[] x_corr5;
	if(y_corr5!=NULL)
		delete[] y_corr5;
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
		if(rects[i].width==0)
			continue;
		for (int j=i+1;j<n_rect;j++)
		{
			if(rects[j].width==0)
				continue;
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
			float r1 =(float)cutR.area()/minR->area();
//			float r2 =(float)cutR.area()/rects[i].area();
			if(r1>0.8)
			{
				rects[j]=Rect(0,0,0,0);
				rects[i]=Rect(Point(minX1,minY1),Point(maxX2,maxY2));
				j=i;
				continue;
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
				//	printf("ATAN %d %d %f\n",dx,dy,a);
				}
			}

		}
	}
	for (int i=0;i<n_rect;i++)
	{
		rectangle(img,rects[i],Scalar(0,256,0),2);
	
	}
}
