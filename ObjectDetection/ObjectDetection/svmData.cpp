//#include "svmData.h"
#include "stdafx.h"
using namespace System;
using namespace System::Xml;
extern Size cellSize,blockSize,wndSize,maxWndSz;
extern cv::Vec2i blockOverlap;
extern cv::Vec2f regionOverlap;
extern float delPart;

int n_x2 = 0;
int n_y2 = 0;
int n_x3 = 0;
int n_y3 = 0;
int n_x4=0, n_y4=0;
int n_x5=0, n_y5=0;
int* x_corr2=NULL;
int* x_corr3=NULL;
int* x_corr4=NULL;
int* x_corr5=NULL;
int* y_corr2=NULL;
int* y_corr3=NULL;
int* y_corr4=NULL;
int* y_corr5=NULL;
Mat M_cellsInBlock1,M_cellsInBlock2,M_cellsInBlock3,M_cellsInBlock4, M_cellsInBlock5 ;

void svmGenerateData(string inputfilelist, int pos,int randTime){
	ifstream conffile;
	conffile.open ("input/config.txt");
	string tmp;

	if (conffile.is_open())
	{
		//		while (! inputfile.eof() )
		//		{
		getline (conffile,tmp);//cell
		getline (conffile,tmp);
		cellSize.width = atoi(tmp.c_str());
		getline (conffile,tmp);
		cellSize.height = atoi(tmp.c_str());
		getline (conffile,tmp);//block
		getline (conffile,tmp);
		blockSize.width = atoi(tmp.c_str());
		getline (conffile,tmp);
		blockSize.height = atoi(tmp.c_str());
		getline (conffile,tmp);//window
		getline (conffile,tmp);
		wndSize.width = atoi(tmp.c_str());
		getline (conffile,tmp);
		wndSize.height = atoi(tmp.c_str());

		//		}
	}
	conffile.close();



	ifstream inputfile;
	printf("%s\n",inputfilelist.c_str());
	inputfile.open (inputfilelist.c_str());
	string filepath,filename;
	string posStr = pos==1?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
	if (inputfile.is_open())
	{
		getline (inputfile,filepath);
		ofstream myfile,myfile2;
		//	SYSTEMTIME st;
		//	GetSystemTime(&st);
		time_t curr;
		tm local;
		time(&curr); // get current time_t value
		local=*(localtime(&curr)); // dereference and assign
		stringstream outputfile,outputfile2; 

		outputfile<<"output/his"<<posStr<<"_winSvm_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		outputfile2<<"output/his"<<posStr<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		myfile.open(outputfile.str().c_str());
		myfile2.open(outputfile2.str().c_str());
		Mat his_wnd ;
		HIS h_w; 
		for (int i=0;inputfile.eof()==false;i++)

			//		while (! inputfile.eof() )
		{
			getline (inputfile,filename);
			if (filename.length()<2)
			{
				break;
			}

			Mat img = imread(filepath+filename);

			for (int j =0;j<randTime;j++)
			{
				if(randTime>1){
					int rnd;
					int sizeW = img.cols - wndSize.width;
					int sizeH = img.rows - wndSize.height;
					rnd = rand()%sizeW;
					slideWnd.x =rnd;
					rnd = rand()%sizeH;
					slideWnd.y = rnd;
				}
				printf("%d. %s (%d,%d)\n",i+1,filename.c_str(),slideWnd.x,slideWnd.y);
				Mat img_slideWnd=img(slideWnd);
				//	imshow(filename,img);
				Mat* imFils = imFilter(img_slideWnd,true);
				Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
			//	Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSize,9);
				calcHisOfCellsInWnd2(G,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSize,9,his_wnd ,180.);
			//	HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);

				myfile2 << pos<<"\t";
				for (int i=0;i<h_w.rows*h_w.cols;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					float v =h_w.at<float>(0,i); 
					myfile << v<<"\t";
					if(v!=0)
						myfile2 << i+1<<":"<<v<<"\t";
				}
				myfile << pos<<"\n";
				myfile2 <<"\n";
//				delete h_w;
			//	h_w->release();
			//	his_wnd.release();
				G.release();
				imFils[0].release();
				imFils[1].release();

			}
			img.release();


		}
		myfile.close();
		myfile2.close();
		his_wnd.release() ;
		h_w.release(); 
	}
	
	
	inputfile.close();


}

void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg, int normType,float scale,float scaleStep, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech)
{	
	ifstream inputFile;
	printf("%s\n",posfilelist.c_str());
	inputFile.open (posfilelist.c_str());
	//inputNegFile.open (negfilelist.c_str());
	string filepath,filename;
	int randTime = randTimePos;
	string pos ="+1";
	string posStr = pos.compare("+1")==0?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
//	float scaleStep=1.2;          // change step.
	if (inputFile.is_open())
	{
		getline (inputFile,filepath);
		ofstream myfile2;

		//ofstream myfile;
		//	SYSTEMTIME st;
		//	GetSystemTime(&st);
		time_t curr;
		tm local;
		time(&curr); // get current time_t value
		local=*(localtime(&curr)); // dereference and assign
		//	stringstream outputfile;
		stringstream outputfile2; 

		//	outputfile<<"output/his_"<<"test"<<"_winSvm_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		outputfile2<<"output/his_test"<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		//	myfile.open(outputfile.str().c_str());
		myfile2.open(outputfile2.str().c_str());
		myfile2.precision(4);
		Size cellSz,wndSz,tmp;
		cellSz.width = cellSize.width;
		cellSz.height = cellSize.height;
		tmp.width = wndSize.width / cellSize.width;
		tmp.height = wndSize.height / cellSize.height;
		Mat his_cells_wnd ;
		HIS his_wind;
		HIS* h_ws=NULL;
		int* x_corr=NULL;int* y_corr=NULL; int n_x=0, n_y=0;
		int rnd=0;
		int sizeW = 0;
		int sizeH = 0;
		for (int i=0;;i++)

			//		while (! inputfile.eof() )
		{
			/*cellSz = cellSize;
			wndSz = wndSize;
			slideWnd.width = wndSize.width;
			slideWnd.height =wndSize.height;*/

			getline (inputFile,filename);
			if (filename.length()<2)
			{
				if(pos.compare("+1")!=0)
					break;
				inputFile.close();
				inputFile.open(negfilelist.c_str());
				if(!inputFile.is_open())
					printf("XXXXXX");
				pos="-1";
				randTime=randTimeNeg;
				inputFile.seekg(0, ios::beg);
				getline (inputFile,filepath);
				continue;
				//break;
			}
			Mat img = imread(filepath+filename);

			float maxSz=maxWndSz.width*maxWndSz.height;
			float minSz = wndSize.width*wndSize.height;
			Rect realRect=resizeImg(img,maxSz,minSz,false);
			//	Mat img = imread(filepath+filename);
			int n_channels=1;
			Mat* imFils;
			Mat G;
			if(useMaxChannel==true)
			{
				n_channels=img.channels();
				imFils = imFilterChannels(img,true);
				G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
			}else{
				imFils = imFilter(img,true);
				G = calcGradientOfPixels(imFils[0],imFils[1]);
			}

			bool continueScale = false;
			Point startP;
			for (int j =0;j<randTime;)
			{

				if(randTime>1)
				{

					if (!continueScale)
					{
					//	int rnd;
						sizeW = img.cols - wndSize.width;
						sizeH = img.rows - wndSize.height;
						switch(j)
						{
							//	case 0: startP.x=img.cols/2;startP.y=img.rows/2;break;
							//	case 1: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 2: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 3: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//	case 4: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//	case 5: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//	case 6: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//	case 7: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 8: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+3*sizeH/4;break;
						default:

							rnd = rand()%sizeW;
							startP.x =wndSize.width/2+rnd;
							rnd = rand()%sizeH;
							startP.y = wndSize.height/2+rnd;
							break;


						}

						//	slideWnd.width = wndSize.width;
						//	slideWnd.height=wndSize.height;
						scale=1.;
						slideWnd=getRect(startP.x,startP.y,scale);
						wndSz.width=slideWnd.width;
						wndSz.height=slideWnd.height;
					}
					else
					{
						//	cellSz.width+=2;
						//	cellSz.height+=2;

						scale = scale * scaleStep;
						float tt = scale* cellSize.width;
						if( (int)tt==cellSz.width)
						{
							cellSz.width = ceil( tt);
							cellSz.height = ceil( scale* cellSize.height);
						}else{
							cellSz.width = (int)tt;
							cellSz.height = (int)( scale* cellSize.height);
						}
						scale = (float)cellSz.width/cellSize.width;
						slideWnd= getRect(startP.x,startP.y,scale);
						wndSz.width =slideWnd.width;
						wndSz.height =slideWnd.height;

					}

					//	for (int t=0;;t++)
					//	{
					if( slideWnd.x<0 || slideWnd.y<0 || slideWnd.x+slideWnd.width>img.cols || 
						slideWnd.y+slideWnd.height>img.rows || max(slideWnd.width,slideWnd.height)> 0.75*max(img.cols,img.rows))
						//		if( (cellSz.width*tmp.width>img.cols - slideWnd.x) || (cellSz.height*tmp.height>img.rows - slideWnd.y) 
						//		||	max(cellSz.width*tmp.width,cellSz.height*tmp.height)>0.75*max(img.cols,img.rows ) 
						//			)
					{

						//	printf("SAI %d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);
						cellSz = cellSize;
						wndSz = wndSize;
						slideWnd.width = wndSize.width;
						slideWnd.height =wndSize.height;
						j++;
						continueScale = false;
						continue;
					}
					continueScale = true;
					//	}
					//	wndSz.width = cellSz.width*tmp.width;
					//	wndSz.height = cellSz.height*tmp.height;
					//	slideWnd.width = wndSz.width;
					//	slideWnd.height = wndSz.height;



				}else{
					scale=1.;
					if(pos.compare("+1")==0)
					{
						if(img.rows > slideWnd.height)
						{
							slideWnd.x = (img.rows - slideWnd.height)/2;
							slideWnd.y = (img.cols - slideWnd.width)/2;
						}else{
							slideWnd.x=0;
							slideWnd.y=0;
						}
						
					}
					cellSz = cellSize;
					wndSz = wndSize;
					slideWnd.width = wndSize.width;
					slideWnd.height =wndSize.height;
					j++;
					continueScale = false;
				}
				if((i+1)%300==0)
					printf("%d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);

					svmClassify(img,G,slideWnd,cellSz,scale,n_x,n_y,x_corr,y_corr,his_cells_wnd,his_wind,h_ws,normType,useMaxChannel,useSmooth,useLBP,useNewTech);

				//Mat img_slideWnd=img(slideWnd);
				//Mat G1=G(slideWnd);
				//if(useSmooth)
				//	G1=GaussianBlurBlock(G(slideWnd),blockOverlap);


				////	imshow(filename,img);
				////		Mat* imFils = imFilter(img_slideWnd);
				////		Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
				////				Mat his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				//int maxD=180.; 
				//int n_bins=9;
				//if(useLBP)
				//{
				//	calcLBP(G1,0);
				//	maxD=256.;
				//	n_bins=8;
				//}
				//if(useNewTech==-1)
				//{
				//	calcHisOfCellsInWnd2(G1,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,n_bins,his_cells_wnd,maxD);
				//	//		calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
				//	//		HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				//	calcHistOfWnd(his_cells_wnd,blockSize,blockOverlap,2,his_wind);
				//	//	calcHistOfWndNew2(his_wnd,blockSize,blockOverlap,2,h_w);

				//}

				//else if(useNewTech==0)
				//{
				//	calcHisOfCellsInWnd2(G1,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,n_bins,his_cells_wnd,maxD);
				//	//		calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
				//	//		HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				//	//	calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);
				//	calcHistOfWndNew2(his_cells_wnd,blockSize,blockOverlap,2,his_wind);

				//}else if(useNewTech==1){
				//	his_wind=calcHistOfWndNew(G1,cellSz,n_bins,h_ws);
				//}
				//else if(useNewTech==2)
				//{
				//	if(n_x==0)
				//		calcGrid(G1,blockSize,cellSz, Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr, n_x, n_y);
				//	calcHisOfGrid(G1,blockSize,cellSz,Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr,n_x,n_y,scale, n_bins,his_wind);

				//}
				//if(useSmooth)
				//	G1.release();

				myfile2 << pos<<"\t";
				for (int i=0;i<his_wind.cols;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					float v =his_wind.at<float>(0,i); 
					//		myfile << v<<"\t";
					if(v!=0)
						myfile2 << i+1<<":"<<v<<"\t";
				}
				//	myfile << pos<<"\n";
				myfile2 <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
				//	delete[] h_w->vector_weight;
				// h_w->release();
				// delete h_w;
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


			}
			img.release();
			for (int i=0;i<n_channels;i++)
			{
				imFils[i].release();
				imFils[i+1].release();
			}
			delete[] imFils;
			G.release();




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
		if(h_ws!=NULL){
			for (int i=0;i<4;i++)
			{
				h_ws[i].release();
			}
			delete[]h_ws;
		}
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
		//	myfile.close();
		myfile2.close();
	}
	inputFile.close();







}

PCA computePCA(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg, int normType,float scale,float scaleStep, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech,int maxComponents)
{	
	ifstream inputFile;
	printf("%s\n",posfilelist.c_str());
	inputFile.open (posfilelist.c_str());
	//inputNegFile.open (negfilelist.c_str());
	string filepath,filename;
	int randTime = randTimePos;
	string pos ="+1";
	string posStr = pos.compare("+1")==0?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
	int n_rows=0,n_cols=0,n_pos=0;
	Mat* data=new Mat();
	//	float scaleStep=1.2;          // change step.
	if (inputFile.is_open())
	{
		getline (inputFile,filepath);
		ofstream myfile2;

		//ofstream myfile;
		//	SYSTEMTIME st;
		//	GetSystemTime(&st);
		time_t curr;
		tm local;
		time(&curr); // get current time_t value
		local=*(localtime(&curr)); // dereference and assign
		//	stringstream outputfile;
		stringstream outputfile2; 

		//	outputfile<<"output/his_"<<"test"<<"_winSvm_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		outputfile2<<"output/his_test"<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		//	myfile.open(outputfile.str().c_str());
		//myfile2.open(outputfile2.str().c_str());
		//myfile2.precision(4);
		Size cellSz,wndSz,tmp;
		cellSz.width = cellSize.width;
		cellSz.height = cellSize.height;
		tmp.width = wndSize.width / cellSize.width;
		tmp.height = wndSize.height / cellSize.height;
		Mat his_cells_wnd ;
		HIS his_wind;
		HIS* h_ws=NULL;
		int* x_corr=NULL;int* y_corr=NULL; int n_x=0, n_y=0;
		int rnd=0;
		int sizeW = 0;
		int sizeH = 0;
		for (int i=0;;i++)

			//		while (! inputfile.eof() )
		{
			/*cellSz = cellSize;
			wndSz = wndSize;
			slideWnd.width = wndSize.width;
			slideWnd.height =wndSize.height;*/

			getline (inputFile,filename);
			if (filename.length()<2)
			{
				if(pos.compare("+1")!=0)
					break;
				inputFile.close();
				n_pos=n_rows;
				inputFile.open(negfilelist.c_str());
				if(!inputFile.is_open())
					printf("XXXXXX");
				pos="-1";
				randTime=randTimeNeg;
				inputFile.seekg(0, ios::beg);
				getline (inputFile,filepath);
				continue;
				//break;
			}
			Mat img = imread(filepath+filename);

			float maxSz=maxWndSz.width*maxWndSz.height;
			float minSz = wndSize.width*wndSize.height;
			Rect realRect=resizeImg(img,maxSz,minSz,false);
			//	Mat img = imread(filepath+filename);
			int n_channels=1;
			Mat* imFils;
			Mat G;
			if(useMaxChannel==true)
			{
				n_channels=img.channels();
				imFils = imFilterChannels(img,true);
				G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
			}else{
				imFils = imFilter(img,true);
				G = calcGradientOfPixels(imFils[0],imFils[1]);
			}

			bool continueScale = false;
			Point startP;
			for (int j =0;j<randTime;)
			{

				if(randTime>1)
				{

					if (!continueScale)
					{
						//	int rnd;
						sizeW = img.cols - wndSize.width;
						sizeH = img.rows - wndSize.height;
						switch(j)
						{
							//	case 0: startP.x=img.cols/2;startP.y=img.rows/2;break;
							//	case 1: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 2: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 3: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//	case 4: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//	case 5: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//	case 6: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//	case 7: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 8: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+3*sizeH/4;break;
						default:

							rnd = rand()%sizeW;
							startP.x =wndSize.width/2+rnd;
							rnd = rand()%sizeH;
							startP.y = wndSize.height/2+rnd;
							break;


						}

						//	slideWnd.width = wndSize.width;
						//	slideWnd.height=wndSize.height;
						scale=1.;
						slideWnd=getRect(startP.x,startP.y,scale);
						wndSz.width=slideWnd.width;
						wndSz.height=slideWnd.height;
					}
					else
					{
						//	cellSz.width+=2;
						//	cellSz.height+=2;

						scale = scale * scaleStep;
						float tt = scale* cellSize.width;
						if( (int)tt==cellSz.width)
						{
							cellSz.width = ceil( tt);
							cellSz.height = ceil( scale* cellSize.height);
						}else{
							cellSz.width = (int)tt;
							cellSz.height = (int)( scale* cellSize.height);
						}
						scale = (float)cellSz.width/cellSize.width;
						slideWnd= getRect(startP.x,startP.y,scale);
						wndSz.width =slideWnd.width;
						wndSz.height =slideWnd.height;

					}

					//	for (int t=0;;t++)
					//	{
					if( slideWnd.x<0 || slideWnd.y<0 || slideWnd.x+slideWnd.width>img.cols || 
						slideWnd.y+slideWnd.height>img.rows || max(slideWnd.width,slideWnd.height)> 0.75*max(img.cols,img.rows))
						//		if( (cellSz.width*tmp.width>img.cols - slideWnd.x) || (cellSz.height*tmp.height>img.rows - slideWnd.y) 
						//		||	max(cellSz.width*tmp.width,cellSz.height*tmp.height)>0.75*max(img.cols,img.rows ) 
						//			)
					{

						//	printf("SAI %d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);
						cellSz = cellSize;
						wndSz = wndSize;
						slideWnd.width = wndSize.width;
						slideWnd.height =wndSize.height;
						j++;
						continueScale = false;
						continue;
					}
					continueScale = true;

				}else{
					scale=1.;
					if(pos.compare("+1")==0)
					{
						if(img.rows > slideWnd.height)
						{
							slideWnd.x = (img.rows - slideWnd.height)/2;
							slideWnd.y = (img.cols - slideWnd.width)/2;
						}else{
							slideWnd.x=0;
							slideWnd.y=0;
						}

					}
					cellSz = cellSize;
					wndSz = wndSize;
					slideWnd.width = wndSize.width;
					slideWnd.height =wndSize.height;
					j++;
					continueScale = false;
				}
				if((i+1)%300==0)
					printf("%d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);

				svmClassify(img,G,slideWnd,cellSz,scale,n_x,n_y,x_corr,y_corr,his_cells_wnd,his_wind,h_ws,normType,useMaxChannel,useSmooth,useLBP,useNewTech);
				if(n_cols==0)
				{
					n_cols=his_wind.cols;
					*data=Mat::zeros(20000,n_cols,his_wind.type());
					his_wind.copyTo((*data)(Rect(0,n_rows,n_cols,1)));
				}
				n_rows++;
				his_wind=((*data)(Rect(0,n_rows,n_cols,1)));
				
				//myfile2 << pos<<"\t";
				//for (int i=0;i<his_wind.cols;i++)
				//{
				//	//	printf("%f ; ",h_w->vector_weight[i]);
				//	float v =his_wind.at<float>(0,i); 
				//	//		myfile << v<<"\t";
				//	if(v!=0)
				//		myfile2 << i+1<<":"<<v<<"\t";
				//}
				//	myfile << pos<<"\n";
				//myfile2 <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
				//	delete[] h_w->vector_weight;
				// h_w->release();
				// delete h_w;
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


			}
			img.release();
			for (int i=0;i<n_channels;i++)
			{
				imFils[i].release();
				imFils[i+1].release();
			}
			delete[] imFils;
			G.release();
		



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
		if(h_ws!=NULL){
			for (int i=0;i<4;i++)
			{
				h_ws[i].release();
			}
			delete[]h_ws;
		}
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
		//	myfile.close();
		myfile2.close();
	}
	inputFile.close();

	 PCA myPCA((*data)(Rect(0,0,n_cols,n_rows)),Mat(),CV_PCA_DATA_AS_ROW,maxComponents);
	 ofstream myfile;
	 
	 myfile.open("output/pca_vecs.txt");
	 myfile.precision(4);
	 int ispos=1;
	
	 
	 for(int r=0;r<n_rows;r++)
	 {
		 
		 if(r>=n_pos)
			 ispos=-1;
		 //myfile<< ispos<<"\t";
		 myfile<< ispos<<"\t";
		 Mat his_wind=myPCA.project(data->row(r));
		 for (int i=0;i<his_wind.cols;i++)
		 {
			 //	printf("%f ; ",h_w->vector_weight[i]);
			 float v =his_wind.at<float>(0,i); 
			 //		myfile << v<<"\t";
			 if(v!=0)
				 //myfile << i+1<<":"<<v<<"\t";
				myfile << i+1<<":"<<v<<"\t";
		 }
	 //	myfile << pos<<"\n";
		//myfile <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
		myfile <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
	 }
	 
	 myfile.close();
	return myPCA;


}




void svmClassify( Mat img,Mat G,Rect slideWnd, Size cellSz,float& scale, int& n_x, int& n_y,int*& x_corr,int*&y_corr, Mat& his_cells_wnd, HIS& his_wind,Mat*&h_ws, int normType, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech)
{	
	Mat img_slideWnd=img(slideWnd);
	Mat G1=G(slideWnd);
	/*int n_x2 = (n_x+1)/2 -1;
	int n_y2 = (n_y+1)/2 -1;
	int n_x3 = (n_x2+1)/2 -1;
	int n_y3 = (n_y2+1)/2 -1;
	int* x_corr2=NULL;
	int* x_corr3=NULL;
	int* y_corr2=NULL;
	int* y_corr3=NULL;*/
	if(useSmooth)
		G1=GaussianBlurBlock(G(slideWnd),blockOverlap);
	int maxD=180.; 
	int n_bins=9;
	if(useLBP)
	{
		calcLBP(G1,0);
		maxD=256.;
		n_bins=8;
	}
	if(useNewTech==-1) // cach cu: tinh cell roi tinh block
	{
		calcHisOfCellsInWnd2(G1,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,n_bins,his_cells_wnd,maxD);
		calcHistOfWnd(his_cells_wnd,blockSize,blockOverlap,normType,his_wind);
	}
	else if(useNewTech==0) // chay theo kim dong ho
	{
		calcHisOfCellsInWnd2(G1,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,n_bins,his_cells_wnd,maxD);
		calcHistOfWndNew2(his_cells_wnd,blockSize,blockOverlap,normType,his_wind);

	}else if(useNewTech==1){ // 4 parts
		his_wind=calcHistOfWndNew(G1,cellSz,n_bins,h_ws);
	}
	else if(useNewTech==2) //grid
	{
		if(n_x==0)
			calcGrid(G1,blockSize,cellSz, Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr, n_x, n_y);
		calcHisOfGrid(G1,blockSize,cellSz,Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr,n_x,n_y,scale, n_bins,his_wind,M_cellsInBlock1);

	}
	else if(useNewTech==3)  // multiscale
	{
		if(n_x==0)
		{
			calcGrid(G1,blockSize,cellSz, Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr, n_x, n_y);
			calcGrid(G1,blockSize,cellSz*2, Size(blockSize.width*cellSz.width*2/2,blockSize.height*cellSz.height*2/2),x_corr2,y_corr2, n_x2, n_y2);
			calcGrid(G1,blockSize,cellSz*4, Size(blockSize.width*cellSz.width*4/2,blockSize.height*cellSz.height*4/2),x_corr3,y_corr3, n_x3, n_y3);
			/*calcGrid(G1,blockSize,cellSz*sqrt(2), Size(blockSize.width*cellSz.width*sqrt(2)/2,blockSize.height*cellSz.height*sqrt(2)/2),x_corr2,y_corr2, n_x2, n_y2);
			calcGrid(G1,blockSize,cellSz*2, Size(blockSize.width*cellSz.width*2/2,blockSize.height*cellSz.height*2/2),x_corr3,y_corr3, n_x3, n_y3);
			calcGrid(G1,blockSize,cellSz*2*sqrt(2), Size(blockSize.width*cellSz.width*2*sqrt(2)/2,blockSize.height*cellSz.height*2*sqrt(2)/2),x_corr4,y_corr4, n_x4, n_y4);
			calcGrid(G1,blockSize,cellSz*4, Size(blockSize.width*cellSz.width*4/2,blockSize.height*cellSz.height*4/2),x_corr5,y_corr5, n_x5, n_y5);*/
		}
		int s1 = n_x*n_y*blockSize.height*blockSize.width*n_bins;
		int s2 = n_x2*n_y2*blockSize.height*blockSize.width*n_bins;
		int s3 = n_x3*n_y3*blockSize.height*blockSize.width*n_bins;
		/*int s4 = n_x4*n_y4*blockSize.height*blockSize.width*n_bins;
		int s5 = n_x5*n_y5*blockSize.height*blockSize.width*n_bins;*/
		int ss = s1 + s2 + s3;
		if(his_wind.cols!=ss)
			his_wind = Mat::zeros(1,ss,DataType<float>::type);
		HIS hw1=his_wind(Rect(0,0,s1,1));
		HIS hw2=his_wind(Rect(s1,0,s2,1));
		HIS hw3=his_wind(Rect(s1+s2,0,s3,1));
		/*HIS hw4=his_wind(Rect(s1+s2+s3,0,s4,1));
		HIS hw5=his_wind(Rect(s1+s2+s3+s4,0,s5,1));*/
		calcHisOfGrid(G1,blockSize,cellSz,Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr,n_x,n_y,scale, n_bins,hw1,M_cellsInBlock1);
		calcHisOfGrid(G1,blockSize,cellSz*2,Size(blockSize.width*cellSz.width*2/2,blockSize.height*cellSz.height*2/2),x_corr2,y_corr2,n_x2,n_y2,scale, n_bins,hw2,M_cellsInBlock2);
		calcHisOfGrid(G1,blockSize,cellSz*4,Size(blockSize.width*cellSz.width*4/2,blockSize.height*cellSz.height*4/2),x_corr3,y_corr3,n_x3,n_y3,scale, n_bins,hw3,M_cellsInBlock3);
	//	hw1.copyTo((his_wind)(Rect(0,0,s1,1)));
	//	hw2.copyTo((his_wind)(Rect(s1,0,s2,1)));
	//	hw3.copyTo((his_wind)(Rect(s1+s2,0,s3,1)));
	//	hw1.release();
	//	hw2.release();
	//	hw3.release(); 
	}
	else if(useNewTech==4)  // multiscale
	{
		float f2 = Math::Sqrt(2);
		if(n_x==0)
		{
			calcGrid(G1,blockSize,cellSz, Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr, n_x, n_y);
			//	calcGrid(G1,blockSize,cellSz*2, Size(blockSize.width*cellSz.width*2/2,blockSize.height*cellSz.height*2/2),x_corr2,y_corr2, n_x2, n_y2);
			//	calcGrid(G1,blockSize,cellSz*4, Size(blockSize.width*cellSz.width*4/2,blockSize.height*cellSz.height*4/2),x_corr3,y_corr3, n_x3, n_y3);
			calcGrid(G1,blockSize,Size((int)(cellSz.width*f2),(int)(cellSz.height*f2)), Size((int)(blockSize.width*cellSz.width*f2/2),(int)(blockSize.height*cellSz.height*f2/2)),x_corr2,y_corr2, n_x2, n_y2);
			calcGrid(G1,blockSize,cellSz*2, Size(blockSize.width*cellSz.width*2/2,blockSize.height*cellSz.height*2/2),x_corr3,y_corr3, n_x3, n_y3);
			calcGrid(G1,blockSize,Size((int)(cellSz.width*2*f2),(int)(cellSz.height*2*f2)), Size((int)(blockSize.width*cellSz.width*2*f2/2),(int)(blockSize.height*cellSz.height*2*f2/2)),x_corr4,y_corr4, n_x4, n_y4);
			calcGrid(G1,blockSize,cellSz*4, Size(blockSize.width*cellSz.width*4/2,blockSize.height*cellSz.height*4/2),x_corr5,y_corr5, n_x5, n_y5);
		}
		
		int s1 = n_x*n_y*blockSize.height*blockSize.width*n_bins;
		int s2 = n_x2*n_y2*blockSize.height*blockSize.width*n_bins;
		int s3 = n_x3*n_y3*blockSize.height*blockSize.width*n_bins;
		int s4 = n_x4*n_y4*blockSize.height*blockSize.width*n_bins;
		int s5 = n_x5*n_y5*blockSize.height*blockSize.width*n_bins;
		int ss = s1 + s2 + s3 + s4 + s5;
		if(his_wind.cols!=ss)
			his_wind = Mat::zeros(1,ss,DataType<float>::type);
		HIS hw1=his_wind(Rect(0,0,s1,1));
		HIS hw2=his_wind(Rect(s1,0,s2,1));
		HIS hw3=his_wind(Rect(s1+s2,0,s3,1));
		HIS hw4=his_wind(Rect(s1+s2+s3,0,s4,1));
		HIS hw5=his_wind(Rect(s1+s2+s3+s4,0,s5,1));
		calcHisOfGrid(G1,blockSize,cellSz,Size(blockSize.width*cellSz.width/2,blockSize.height*cellSz.height/2),x_corr,y_corr,n_x,n_y,scale, n_bins,hw1,M_cellsInBlock1);
		calcHisOfGrid(G1,blockSize,Size((int)(cellSz.width*f2),(int)(cellSz.height*f2)),Size((int)(blockSize.width*cellSz.width*f2/2),(int)(blockSize.height*cellSz.height*f2/2)),x_corr2,y_corr2,n_x2,n_y2,scale, n_bins,hw2,M_cellsInBlock2);
		calcHisOfGrid(G1,blockSize,cellSz*2,Size(blockSize.width*cellSz.width*2/2,blockSize.height*cellSz.height*2/2),x_corr3,y_corr3,n_x3,n_y3,scale, n_bins,hw3,M_cellsInBlock3);
		calcHisOfGrid(G1,blockSize,Size((int)(cellSz.width*f2),(int)(cellSz.height*f2)),Size((int)(blockSize.width*cellSz.width*2*f2/2),(int)(blockSize.height*cellSz.height*2*f2/2)),x_corr4,y_corr4,n_x4,n_y4,scale, n_bins,hw4,M_cellsInBlock4);
		calcHisOfGrid(G1,blockSize,cellSz*4,Size(blockSize.width*cellSz.width*4/2,blockSize.height*cellSz.height*4/2),x_corr5,y_corr5,n_x5,n_y5,scale, n_bins,hw5,M_cellsInBlock5);
		//	hw1.copyTo((his_wind)(Rect(0,0,s1,1)));
		//	hw2.copyTo((his_wind)(Rect(s1,0,s2,1)));
		//	hw3.copyTo((his_wind)(Rect(s1+s2,0,s3,1)));
		//	hw1.release();
		//	hw2.release();
		//	hw3.release(); 
	}
	if(useSmooth)
		G1.release();			
}



void svmGenHardList(string weightFile,string posfilelist, string negfilelist,string prefixName, int randTimePos,int randTimeNeg,int normType, float scale,float scaleStep, bool useMaxChannel,bool useSmooth,bool useLBP,int useNewTech)
{	
	Mat * weight;
	float b;
	getWeight(weightFile,weight,b);
	ifstream inputFile;
	printf("%s\n",posfilelist.c_str());
	inputFile.open (posfilelist.c_str());
	//inputNegFile.open (negfilelist.c_str());
	string filepath,filename;
	int randTime = randTimePos;
	string pos ="+1";
	string posStr = pos.compare("+1")==0?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
//	float scale=1.,scaleStep=1.2;          // change step.
	if (inputFile.is_open())
	{
		getline (inputFile,filepath);
		ofstream hardPosFile,harNegFile,scoreFilePos,scoreFileNeg;
		
		string str="output/"+prefixName+"_hardPos.txt";
		hardPosFile.open(str.c_str());
		str="output/"+prefixName+"_hardNeg.txt";
		harNegFile.open(str.c_str());
		
		str="output/"+prefixName+"_scorePos.txt";
		scoreFilePos.open(str.c_str());
		str="output/"+prefixName+"_scoreNeg.txt";
		scoreFileNeg.open(str.c_str());
		hardPosFile.precision(4);
		harNegFile.precision(4);
		
		scoreFilePos.precision(4);
		scoreFileNeg.precision(4);
		int truePos=0,trueNeg=0,falsePos=0,falseNeg=0;
		long totalWnd=0;
		float precision=0,recall=0;
		Size cellSz,wndSz,tmp;
		cellSz.width = cellSize.width;
		cellSz.height = cellSize.height;
		tmp.width = wndSize.width / cellSize.width;
		tmp.height = wndSize.height / cellSize.height;
		Mat his_cells_wnd ;
		HIS his_wind;
		HIS* h_ws=NULL;
		int* x_corr=NULL;int* y_corr=NULL; int n_x=0, n_y=0;
		Mat R;
		Mat h_w;
		for (int i=0;;i++)

			//		while (! inputfile.eof() )
		{
			/*cellSz = cellSize;
			wndSz = wndSize;
			slideWnd.width = wndSize.width;
			slideWnd.height =wndSize.height;*/

			getline (inputFile,filename);
			if (filename.length()<2)
			{
				if(pos.compare("+1")!=0)
					break;
				inputFile.close();
				inputFile.open(negfilelist.c_str());
				if(!inputFile.is_open())
					printf("XXXXXX");
				pos="-1";
				randTime=randTimeNeg;
				inputFile.seekg(0, ios::beg);
				getline (inputFile,filepath);
				continue;
				//break;
			}
			Mat img = imread(filepath+filename);   //

			float maxSz=maxWndSz.width*maxWndSz.height;
			float minSz = wndSize.width*wndSize.height;
			Rect realRect=resizeImg(img,maxSz,minSz,false);
			//	Mat img = imread(filepath+filename);
			int n_channels=1;
			Mat* imFils;                           //
			Mat G;									//
			
			if(useMaxChannel==true)
			{
				n_channels=img.channels();
				imFils = imFilterChannels(img,true);
				G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
			}else{
				imFils = imFilter(img,true);
				G = calcGradientOfPixels(imFils[0],imFils[1]);
			}

			bool continueScale = false;
			Point startP;
			
			for (int j =0;j<randTime;)
			{

				if(randTime>1)
				{

					if (!continueScale)
					{
						int rnd;
						int sizeW = img.cols - wndSize.width;
						int sizeH = img.rows - wndSize.height;
						switch(j)
						{
							//	case 0: startP.x=img.cols/2;startP.y=img.rows/2;break;
							//	case 1: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 2: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 3: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//	case 4: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//	case 5: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//	case 6: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//	case 7: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+sizeH/4;break;
							//	case 8: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+3*sizeH/4;break;
						default:

							rnd = rand()%sizeW;
							startP.x =wndSize.width/2+rnd;
							rnd = rand()%sizeH;
							startP.y = wndSize.height/2+rnd;
							break;


						}

						//	slideWnd.width = wndSize.width;
						//	slideWnd.height=wndSize.height;
						scale=1.;
						slideWnd=getRect(startP.x,startP.y,scale);
						wndSz.width=slideWnd.width;
						wndSz.height=slideWnd.height;
					}
					else
					{
						//	cellSz.width+=2;
						//	cellSz.height+=2;

						scale = scale * scaleStep;
						float tt = scale* cellSize.width;
						if( (int)tt==cellSz.width)
						{
							cellSz.width = ceil( tt);
							cellSz.height = ceil( scale* cellSize.height);
						}else{
							cellSz.width = (int)tt;
							cellSz.height = (int)( scale* cellSize.height);
						}
						scale = (float)cellSz.width/cellSize.width;
						slideWnd= getRect(startP.x,startP.y,scale);
						wndSz.width =slideWnd.width;
						wndSz.height =slideWnd.height;

					}

					//	for (int t=0;;t++)
					//	{
					if( slideWnd.x<0 || slideWnd.y<0 || slideWnd.x+slideWnd.width>img.cols || 
						slideWnd.y+slideWnd.height>img.rows || max(slideWnd.width,slideWnd.height)> 0.75*max(img.cols,img.rows))
						//		if( (cellSz.width*tmp.width>img.cols - slideWnd.x) || (cellSz.height*tmp.height>img.rows - slideWnd.y) 
						//		||	max(cellSz.width*tmp.width,cellSz.height*tmp.height)>0.75*max(img.cols,img.rows ) 
						//			)
					{

						//	printf("SAI %d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);
						cellSz = cellSize;
						wndSz = wndSize;
						slideWnd.width = wndSize.width;
						slideWnd.height =wndSize.height;
						j++;
						continueScale = false;
						continue;
					}
					continueScale = true;
					//	}
					//	wndSz.width = cellSz.width*tmp.width;
					//	wndSz.height = cellSz.height*tmp.height;
					//	slideWnd.width = wndSz.width;
					//	slideWnd.height = wndSz.height;



				}else{
					scale=1.;
					if(pos.compare("+1")==0)
					{
						if(img.rows > slideWnd.height)
						{
							slideWnd.x = (img.rows - slideWnd.height)/2;
							slideWnd.y = (img.cols - slideWnd.width)/2;
						}else{
							slideWnd.x=0;
							slideWnd.y=0;
						}
						float r=rand();
					}
					cellSz = cellSize;
					wndSz = wndSize;
					slideWnd.width = wndSize.width;
					slideWnd.height =wndSize.height;
					j++;
					continueScale = false;
				}
				if((i+1)%300==0)
					printf("%d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);

				svmClassify(img,G,slideWnd,cellSz,scale,n_x,n_y,x_corr,y_corr,his_cells_wnd,his_wind,h_ws,normType, useMaxChannel,useSmooth,useLBP,useNewTech);
				h_w = his_wind;
				if(weight->rows<his_wind.cols)
				{
				//	Mat tmp=(*weight);		//
					
				//	*weight=Mat::zeros(his_wind.cols,1,weight->type());
				//	Mat m=(*weight)(Rect(0,0,1,tmp.rows));
				//	tmp.copyTo(m);
					
					h_w=his_wind(Rect(0,0,weight->rows,1));
				}
				R = (his_wind)* (*weight ) - b;
				float score = R.at<float>(0,0);
				R.release();
				totalWnd++;
				
				//write hard example

				if(pos.compare("+1")==0)
				{
					scoreFilePos<<score<<endl;
					if( score<0.)
					{
						falseNeg++;
					//	cout<<"Found hard ex:"<<pos<<endl;
						hardPosFile << pos<<"\t";
						for (int i=0;i<his_wind.cols;i++)
						{
							float v =his_wind.at<float>(0,i); 
							if(v!=0)
								hardPosFile << i+1<<":"<<v<<"\t";
						}
						hardPosFile <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
					}else
					{
						truePos++;
					}

				}
				else if(pos.compare("-1")==0 )
				{
					scoreFileNeg<<score<<endl;
					
					if( score>=0.)
					{
						falsePos++;
					//	cout<<"Found hard ex:"<<pos<<endl;
						harNegFile << pos<<"\t";
						for (int i=0;i<his_wind.cols;i++)
						{
							float v =his_wind.at<float>(0,i); 
							if(v!=0)
								harNegFile << i+1<<":"<<v<<"\t";
						}
						harNegFile <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
					}
					else
					{
						trueNeg++;
					}
				}
			}
			
			img.release();
			for (int i=0;i<n_channels;i++)
			{
				imFils[i].release();
				imFils[i+1].release();
			}
			delete[] imFils;
			G.release();
			
		}
		
		for (int ii=0;ii<his_cells_wnd.rows;ii++)
		{
			for (int jj=0;jj<his_cells_wnd.cols;jj++)
			{
				HIS* hh=his_cells_wnd.at<HIS*>(ii,jj);
				//	hh->release();
				delete hh;
			}
		}
		his_cells_wnd.release();
		his_wind.release();
		if(h_ws!=NULL){
			for (int i=0;i<4;i++)
			{
				h_ws[i].release();
			}
			delete[]h_ws;
		}
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
		//	myfile.close();
		ofstream infofile;
		str="output/"+prefixName+"_info.txt";
		infofile.open(str.c_str());
		infofile.precision(4);
		infofile<<"Total windows: "<<totalWnd<<endl;
		infofile<<"True Pos: "<<truePos<<endl;
		infofile<<"True Neg: "<<trueNeg<<endl;
		infofile<<"False Pos: "<<falsePos<<endl;
		infofile<<"False Neg: "<<falseNeg<<endl;
		if(truePos+falsePos!=0)
		{
			precision=1.*truePos/(truePos+falsePos);
			infofile<<"Precision : "<<100. * precision<<"(%)"<<endl;
		}
		else
		{
			precision=-1;
			infofile<<"Precision : !#!"<<"(%)"<<endl;
		}
		if(truePos+falseNeg!=0)
		{
			recall=1.*truePos/(truePos+falseNeg);
			infofile<<"Recall : "<<100. * recall<<"(%)"<<endl;
		}
		else
		{
			recall=-1;
			infofile<<"Recall : !#!"<<"(%)"<<endl;
		}
		if (totalWnd>0)
		{
			infofile<<"FPPW : "<<1. * falsePos/totalWnd<<endl;
		}
		if(recall!=-1)
			infofile<<"Missrate : "<<1. - recall<<endl;
		
		harNegFile.close();
		hardPosFile.close();
		infofile.close();
		scoreFilePos.close();
		scoreFileNeg.close();
	}
	inputFile.close();







}



void VOCAnnRects(System::String^ XmlFileName,System::String^ objName,Rect* &rects,int& n_rect)
{
	XmlDocument ^ doc = gcnew XmlDocument;

	doc->Load(XmlFileName);
	//	XmlTextReader^ reader= gcnew XmlTextReader("input/test.xml");
	//	cout<<msclr::interop::marshal_as<std::string>(reader->);
	XmlNodeList^ nl= doc->GetElementsByTagName("object");
	
//	n_rect=nl->Count;
	 rects= new Rect[nl->Count];
	n_rect=0;
	for (int i=0;i<nl->Count;i++)
	{
		
		if (nl->Item(i)->ChildNodes->Item(0)->InnerText != objName)
		{
			continue;
		}
		XmlNodeList^ objNl=nl->Item(i)->ChildNodes;
		for (int j=0;j<objNl->Count;j++)
		{
			if(objNl->Item(j)->Name==gcnew System::String("bndbox"))
			{
				XmlNodeList^ boundNl=objNl->Item(j)->ChildNodes;
				//	Console::WriteLine(nl->Item(i)->ChildNodes->Item(4)->InnerText);
				int xmin,xmax,ymin,ymax;
				for (int t=0;t<boundNl->Count;t++)
				{
					if(boundNl->Item(t)->Name=="xmin")
						 xmin =atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(t)->InnerText).c_str());
					else if(boundNl->Item(t)->Name=="xmax")
						xmax=atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(t)->InnerText).c_str());
					else if(boundNl->Item(t)->Name=="ymin")
						ymin=atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(t)->InnerText).c_str());
					else if(boundNl->Item(t)->Name=="ymax")
						ymax=atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(t)->InnerText).c_str());
				
					//	xmin=atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(t)->InnerText).c_str());
				}
			
				rects[n_rect].x= xmin;
				rects[n_rect].y=ymin;
				rects[n_rect].width = xmax-xmin;
				rects[n_rect].height = ymax-ymin;
			//	cout<<rects[n_rect].x<<","<<rects[n_rect].y<<","<<rects[n_rect].width<<","<<rects[n_rect].height<<endl;
			//	rects[n_rect].width=atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(2)->InnerText).c_str())-rects[n_rect].x;
			//	rects[n_rect].height=atoi(msclr::interop::marshal_as<std::string>(boundNl->Item(3)->InnerText).c_str())-rects[n_rect].y;

				n_rect++;
				break;
			}
			
		}
		
	}
	delete doc;
	return ;
};
//void VOCSvmGenerateData2(System::String^ AnnPath,string imgsPath,string fileExt,System::String^ objName,string filelist, int randTimePos,int randTimeNeg)
//{
//	ifstream inputFile;
//	printf("%s\n",filelist.c_str());
//	inputFile.open (filelist.c_str());
//	//inputNegFile.open (negfilelist.c_str());
//	string filepath=imgsPath,filename;
//	int randTime=randTimePos;
//	
//
//	
//
//
//
//
//
//	string pos ="+1";
//	string posStr = pos.compare("+1")==0?"Pos":"Neg";
//	Rect slideWnd(0,0,wndSize.width,wndSize.height);
//	float scale=1.,step=2.;
//	if (inputFile.is_open())
//	{
//	//	getline (inputFile,filepath);
//		ofstream myfile,myfile2;
//		//	SYSTEMTIME st;
//		//	GetSystemTime(&st);
//		time_t curr;
//		tm local;
//		time(&curr); // get current time_t value
//		local=*(localtime(&curr)); // dereference and assign
//		stringstream outputfile,outputfile2,outfilelist; 
//
//		outputfile<<"output/hisfilelist"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
//		outputfile2<<"output/his"<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
//		myfile.open(outputfile.str().c_str());
//		myfile2.open(outputfile2.str().c_str());
//		Size cellSz=cellSize,wndSz=wndSize,tmp;
//		tmp.width = wndSize.width / cellSize.width;
//		tmp.height = wndSize.height / cellSize.height;
//		Mat his_wnd ;
//		HIS h_w;
//		Rect* rects;
//		int n_rect;
//		string strtmp;
//		for (int i=0;;i++)
//
//			//		while (! inputfile.eof() )
//		{
//			/*cellSz = cellSize;
//			wndSz = wndSize;
//			slideWnd.width = wndSize.width;
//			slideWnd.height =wndSize.height;*/
//
//		//	getline (inputFile,filename);
//
//
//			getline (inputFile,strtmp);
//			
//			std::vector<std::string> strs;
//			char* s = (char*)(strtmp.c_str());
//			boost::split(strs,s , boost::is_any_of(" "));
//			filename=strs[0];
//			if (filename.length()<2||strs.size()<2){
//				break;
//			}
//					VOCAnnRects(AnnPath+gcnew System::String(strs[0].c_str())+gcnew System::String(".xml"),objName,rects,n_rect);
//
//				//	printf("%s: %d \n",strs[0].c_str(),n_rect);
//					if(n_rect){
//						pos="+1";
//						randTime=randTimePos;
//					}else{
//						pos="-1";
//						randTime=randTimeNeg;
//					}
//				
//			Mat imgOrg = imread(filepath+filename+fileExt);
//			Mat img = imgOrg.clone();
//
//			float maxSz=maxWndSz.width*maxWndSz.height;
//			float minSz = wndSize.width*wndSize.height;
//		//	Rect realRect=resizeImg(img,maxSz,minSz,false);
//			//	Mat img = imread(filepath+filename);
//			Mat* imFils = imFilter(img,true);
//			Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
//			bool continueScale = false;
//			Point startP;
//			int scanTime=n_rect==0?randTime:n_rect;
//			for (int j =0;j<scanTime;)
//			{
//
//			//	if(randTime>1){
//				if(pos=="-1"){
//					if (!continueScale)
//					{
//						int rnd;
//						int sizeW = img.cols - wndSize.width;
//						int sizeH = img.rows - wndSize.height;
//						switch(j){
//							case 0: startP.x=img.cols/2;startP.y=img.rows/2;break;
//							case 1: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
//							case 2: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
//							case 3: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
//							case 4: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
//				//			case 5: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
//				//			case 6: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
//				//			case 7: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+sizeH/4;break;
//				//			case 8: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+3*sizeH/4;break;
//							default:
//								rnd = rand()%sizeW;
//								startP.x =wndSize.width/2+rnd;
//								rnd = rand()%sizeH;
//								startP.y = wndSize.height/2+rnd;
//								break;
//
//
//						}
//
//						//	slideWnd.width = wndSize.width;
//						//	slideWnd.height=wndSize.height;
//						scale=1.;
//						slideWnd=getRect(startP.x,startP.y,scale);
//						wndSz.width=slideWnd.width;
//						wndSz.height=slideWnd.height;
//					}else{
//						//	cellSz.width+=2;
//						//	cellSz.height+=2;
//
//						scale = scale * step;
//						float tt = scale* cellSize.width;
//						if( (int)tt==cellSz.width)
//						{
//							cellSz.width = ceil( tt);
//							cellSz.height = ceil( scale* cellSize.height);
//						}else{
//							cellSz.width = (int)tt;
//							cellSz.height = (int)( scale* cellSize.height);
//						}
//						scale = (float)cellSz.width/cellSize.width;
//						slideWnd= getRect(startP.x,startP.y,scale);
//						wndSz.width =slideWnd.width;
//						wndSz.height =slideWnd.height;
//
//					}
//
//					//	for (int t=0;;t++)
//					//	{
//					if( slideWnd.x<0 || slideWnd.y<0 || slideWnd.x+slideWnd.width>img.cols || 
//						slideWnd.y+slideWnd.height>img.rows || max(slideWnd.width,slideWnd.height)> 0.75*max(img.cols,img.rows))
//						//		if( (cellSz.width*tmp.width>img.cols - slideWnd.x) || (cellSz.height*tmp.height>img.rows - slideWnd.y) 
//						//		||	max(cellSz.width*tmp.width,cellSz.height*tmp.height)>0.75*max(img.cols,img.rows ) 
//						//			)
//					{
//
//						//	printf("SAI %d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);
//						cellSz = cellSize;
//						wndSz = wndSize;
//						slideWnd.width = wndSize.width;
//						slideWnd.height =wndSize.height;
//						j++;
//						continueScale = false;
//						continue;
//					}
//					continueScale = true;
//					//	}
//					//	wndSz.width = cellSz.width*tmp.width;
//					//	wndSz.height = cellSz.height*tmp.height;
//					//	slideWnd.width = wndSz.width;
//					//	slideWnd.height = wndSz.height;
//
//
//
//				}else{
//					scale=1.;
//					if(pos.compare("+1")==0){
//						/*if(img.rows > slideWnd.height)
//						{
//							slideWnd.x = (img.rows - slideWnd.height)/2;
//							slideWnd.y = (img.cols - slideWnd.width)/2;
//						}else{
//							slideWnd.x=0;
//							slideWnd.y=0;
//						}*/
//						slideWnd=rects[j];
//						Point center(slideWnd.x+slideWnd.width/2,slideWnd.y+slideWnd.height/2);
//						/*float newW= slideWnd.width*1.2<img.cols-slideWnd.x?slideWnd.width*1.2:img.cols-slideWnd.x;
//						float newH= slideWnd.height*1.4<img.rows-slideWnd.y?slideWnd.height*1.4:img.rows-slideWnd.y;
//						float _s=min(newW/slideWnd.width,newH/slideWnd.height);*/
//						float newW= slideWnd.width<img.cols-slideWnd.x?slideWnd.width:img.cols-slideWnd.x;
//						float newH= slideWnd.height<img.rows-slideWnd.y?slideWnd.height:img.rows-slideWnd.y;
//						float _s=min(newW/slideWnd.width,newH/slideWnd.height);
//						float _smax=min((float)(img.cols-slideWnd.width)/slideWnd.width,(float)(img.rows-slideWnd.height)/slideWnd.height);
//							slideWnd = getRect(center.x,center.y,min(_s,_smax));
//							if(slideWnd.x<0) slideWnd.x=0;
//							if(slideWnd.y<0) slideWnd.y=0;
//
//							cellSz.width=round((float)slideWnd.width / tmp.width);
//							cellSz.height = round((float)slideWnd.height /tmp.height);
//							slideWnd.width = cellSz.width * tmp.width;
//							slideWnd.height = cellSz.height * tmp.height;
//						
//						if(slideWnd.x+slideWnd.width>img.cols)
//							slideWnd.x -=slideWnd.x+slideWnd.width-img.cols;
//							if(slideWnd.y+slideWnd.height>img.rows)
//								slideWnd.y -=slideWnd.y+slideWnd.height-img.rows;
//						if(slideWnd.width<wndSize.width||slideWnd.height<wndSize.height||slideWnd.width>img.cols||slideWnd.height>img.rows){
//							cellSz = cellSize;
//							wndSz = wndSize;
//							j++;
//							continue;
//						}
//						
//						
//					}else{
//					cellSz = cellSize;
//					wndSz = wndSize;
//					slideWnd.width = wndSize.width;
//					slideWnd.height =wndSize.height;
//					}
//					j++;
//					continueScale = false;
//				}
//				if((i+1)%300==0)
//					printf("%d %d. %s (%d,%d, %d, %d) %s\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height,pos.c_str());
//				Mat img_slideWnd=img(slideWnd);
//				//	imshow(filename,img);
//				//		Mat* imFils = imFilter(img_slideWnd);
//				//		Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
//				//				Mat his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
//				calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
//				//		HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
//				calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),3,h_w);
//
//
//				myfile2 << pos<<"\t";
//				for (int i=0;i<h_w.cols;i++)
//				{
//					//	printf("%f ; ",h_w->vector_weight[i]);
//					float v =h_w.at<float>(0,i); 
//				//	myfile << v<<"\t";
//					if(v!=0)
//						myfile2 << i+1<<":"<<v<<"\t";
//				}
//			//	myfile << pos<<"\n";
//				outfilelist<<pos<<", "<<filename<<", ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
//				myfile2 <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
//				//	delete[] h_w->vector_weight;
//				// h_w->release();
//				// delete h_w;
//				//for (int ii=0;ii<his_wnd.rows;ii++)
//				//{
//				//	for (int jj=0;jj<his_wnd.cols;jj++)
//				//	{
//				//		HIS* hh=his_wnd.at<HIS*>(ii,jj);
//				//	//	hh->release();
//				//		delete hh;
//				//	}
//				//}
//				//his_wnd.release();
//
//
//			}
//			img.release();
//			imgOrg.release();
//			G.release();
//			imFils[0].release();
//			imFils[1].release();
//
//
//		}
//		for (int ii=0;ii<his_wnd.rows;ii++)
//		{
//			for (int jj=0;jj<his_wnd.cols;jj++)
//			{
//				HIS* hh=his_wnd.at<HIS*>(ii,jj);
//				//	hh->release();
//				delete hh;
//			}
//		}
//		his_wnd.release();
//		h_w.release();
//		myfile<<outfilelist.str();
//		myfile.close();
//		myfile2.close();
//	}
//	inputFile.close();
//
//
//
//
//
//
//
//};


void VOCSvmGenerateData2(System::String^ AnnPath,std::string imgsPath,std::string fileExt,System::String^ objName,std::string filelist, int randTimePos,int randTimeNeg,double scale,double step,bool useMaxChannel,bool useSmooth, bool useLBP)
{
	ifstream inputFile;
	printf("%s\n",filelist.c_str());
	inputFile.open (filelist.c_str());
	//inputNegFile.open (negfilelist.c_str());
	string filepath=imgsPath,filename;
	int randTime=randTimePos;


	string pos ="+1";
	string posStr = pos.compare("+1")==0?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
//	double scale=1.;
	if (inputFile.is_open())
	{
		//	getline (inputFile,filepath);
		ofstream myfile,myfile2;
		//	SYSTEMTIME st;
		//	GetSystemTime(&st);
		time_t curr;
		tm local;
		time(&curr); // get current time_t value
		local=*(localtime(&curr)); // dereference and assign
		stringstream outputfile,outputfile2,outfilelist; 

		outputfile<<"output/hisfilelist"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		outputfile2<<"output/his"<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		myfile.open(outputfile.str().c_str());
		myfile2.open(outputfile2.str().c_str());
		Size cellSz=cellSize,wndSz=wndSize,tmp;
		tmp.width = wndSize.width / cellSize.width;
		tmp.height = wndSize.height / cellSize.height;
		Mat his_wnd ;
		HIS h_w;
		Rect* rects;
		int n_rect;
		string strtmp;
		for (int i=0;;i++)

			//		while (! inputfile.eof() )
		{
			/*cellSz = cellSize;
			wndSz = wndSize;
			slideWnd.width = wndSize.width;
			slideWnd.height =wndSize.height;*/

			//	getline (inputFile,filename);


			getline (inputFile,strtmp);

			std::vector<std::string> strs;
			char* s = (char*)(strtmp.c_str());
			boost::split(strs,s , boost::is_any_of(" "));
			filename=strs[0];
			if (filename.length()<2||strs.size()<2){
				break;
			}
			VOCAnnRects(AnnPath+gcnew System::String(strs[0].c_str())+gcnew System::String(".xml"),objName,rects,n_rect);

			//	printf("%s: %d \n",strs[0].c_str(),n_rect);
			if(n_rect){
				pos="+1";
				randTime=randTimePos;
			}else{
				pos="-1";
				randTime=randTimeNeg;
			}

			Mat imgOrg = imread(filepath+filename+fileExt);
			Mat img = imgOrg.clone();

			double maxSz=maxWndSz.width*maxWndSz.height;
			double minSz = wndSize.width*wndSize.height;
			//	Rect realRect=resizeImg(img,maxSz,minSz,false);
			//	Mat img = imread(filepath+filename);
			int n_channels=1;
			Mat* imFils;
			Mat G;
			if(useMaxChannel==true)
			{
				n_channels=img.channels();
				imFils = imFilterChannels(img,true);
				G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
			}else{
				imFils = imFilter(img,true);
				G = calcGradientOfPixels(imFils[0],imFils[1]);
			}
			bool continueScale = false;
			Point startP;
			int scanTime=n_rect==0?randTime:n_rect;
			for (int j =0;j<scanTime;)
			{

				//	if(randTime>1){
				if(pos=="-1"){
					if (!continueScale)
					{
						int rnd;
						int sizeW = img.cols - wndSize.width;
						int sizeH = img.rows - wndSize.height;
						switch(j){
							//			case 0: startP.x=img.cols/2;startP.y=img.rows/2;break;
							//			case 1: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//			case 2: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							//			case 3: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//			case 4: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							//			case 5: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//			case 6: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							//			case 7: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+sizeH/4;break;
							//			case 8: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+3*sizeH/4;break;
							default:
								
								rnd = rand()%sizeW;
								startP.x =wndSize.width/2+rnd;
								rnd = rand()%sizeH;
								startP.y = wndSize.height/2+rnd;
								break;


						}

						//	slideWnd.width = wndSize.width;
						//	slideWnd.height=wndSize.height;
						scale=1.;
						slideWnd=getRect(startP.x,startP.y,scale);
						wndSz.width=slideWnd.width;
						wndSz.height=slideWnd.height;
					}else{
						//	cellSz.width+=2;
						//	cellSz.height+=2;

						scale = scale * step;
						double tt = scale* cellSize.width;
						if( (int)tt==cellSz.width)
						{
							cellSz.width = ceil( tt);
							cellSz.height = ceil( scale* cellSize.height);
						}else{
							cellSz.width = (int)tt;
							cellSz.height = (int)( scale* cellSize.height);
						}
						scale = (double)cellSz.width/cellSize.width;
						slideWnd= getRect(startP.x,startP.y,scale);
						wndSz.width =slideWnd.width;
						wndSz.height =slideWnd.height;

					}

					//	for (int t=0;;t++)
					//	{
					if( slideWnd.x<0 || slideWnd.y<0 || slideWnd.x+slideWnd.width>img.cols || 
						slideWnd.y+slideWnd.height>img.rows || max(slideWnd.width,slideWnd.height)> 0.75*max(img.cols,img.rows))
						//		if( (cellSz.width*tmp.width>img.cols - slideWnd.x) || (cellSz.height*tmp.height>img.rows - slideWnd.y) 
						//		||	max(cellSz.width*tmp.width,cellSz.height*tmp.height)>0.75*max(img.cols,img.rows ) 
						//			)
					{

						//	printf("SAI %d %d. %s (%d,%d, %d, %d)\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height);
						cellSz = cellSize;
						wndSz = wndSize;
						slideWnd.width = wndSize.width;
						slideWnd.height =wndSize.height;
						j++;
						continueScale = false;
						continue;
					}
					continueScale = true;
					//	}
					//	wndSz.width = cellSz.width*tmp.width;
					//	wndSz.height = cellSz.height*tmp.height;
					//	slideWnd.width = wndSz.width;
					//	slideWnd.height = wndSz.height;



				}else{
					scale=1.;
					if(pos.compare("+1")==0){
						/*if(img.rows > slideWnd.height)
						{
						slideWnd.x = (img.rows - slideWnd.height)/2;
						slideWnd.y = (img.cols - slideWnd.width)/2;
						}else{
						slideWnd.x=0;
						slideWnd.y=0;
						}*/
						slideWnd=rects[j];
						Point center(slideWnd.x+slideWnd.width/2,slideWnd.y+slideWnd.height/2);
						/*double newW= slideWnd.width*1.2<img.cols-slideWnd.x?slideWnd.width*1.2:img.cols-slideWnd.x;
						double newH= slideWnd.height*1.4<img.rows-slideWnd.y?slideWnd.height*1.4:img.rows-slideWnd.y;
						double _s=min(newW/slideWnd.width,newH/slideWnd.height);*/
						double newW= 1.1*slideWnd.width<img.cols-slideWnd.x?1.1*slideWnd.width:img.cols-slideWnd.x;
						double newH= 1.1*slideWnd.height<img.rows-slideWnd.y?1.1*slideWnd.height:img.rows-slideWnd.y;
						//	double _s=min(newW/slideWnd.width,newH/slideWnd.height);
						double _s=min(newW/wndSize.width,newH/wndSize.height);
						//	double _smax=min((double)(img.cols-slideWnd.width)/slideWnd.width,(double)(img.rows-slideWnd.height)/slideWnd.height);
						//	slideWnd = getRect(center.x,center.y,min(_s,_smax));
						slideWnd = getRect(center.x,center.y,_s);
						if(slideWnd.x<0) slideWnd.x=0;
						if(slideWnd.y<0) slideWnd.y=0;

						cellSz.width=round((double)slideWnd.width / tmp.width);
						cellSz.height = round((double)slideWnd.height /tmp.height);
						slideWnd.width = cellSz.width * tmp.width;
						slideWnd.height = cellSz.height * tmp.height;

						if(slideWnd.x+slideWnd.width>img.cols)
							slideWnd.x -=slideWnd.x+slideWnd.width-img.cols;
						if(slideWnd.y+slideWnd.height>img.rows)
							slideWnd.y -=slideWnd.y+slideWnd.height-img.rows;
						if(slideWnd.width<wndSize.width||slideWnd.height<wndSize.height||slideWnd.width>img.cols||slideWnd.height>img.rows){
							cellSz = cellSize;
							wndSz = wndSize;
							j++;
							continue;
						}


					}else{
						cellSz = cellSize;
						wndSz = wndSize;
						slideWnd.width = wndSize.width;
						slideWnd.height =wndSize.height;
					}
					j++;
					continueScale = false;
				}
				if((i+1)%300==0)
					printf("%d %d. %s (%d,%d, %d, %d) %s\n",j,i+1,filename.c_str(),slideWnd.x,slideWnd.y,slideWnd.width,slideWnd.height,pos.c_str());
				
				Mat img_slideWnd=img(slideWnd);
				Mat G1=G(slideWnd);
				if(useSmooth)
					G1=GaussianBlurBlock(G(slideWnd),blockOverlap);
				int maxD=180.; 
				int n_bins=9;
				if(useLBP)
				{
					calcLBP(G1,0);
					maxD=256.;
					n_bins=8;
				}
				calcHisOfCellsInWnd2(G1,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,n_bins,his_wnd,maxD);
				//		calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
				//		HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);
				if(useSmooth)
					G1.release();

				myfile2 << pos<<"\t";
				for (int i=0;i<h_w.cols;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					double v =h_w.at<float>(0,i); 
					//	myfile << v<<"\t";
					if(v!=0)
						myfile2 << i+1<<":"<<v<<"\t";
				}
				//	myfile << pos<<"\n";
				outfilelist<<pos<<", "<<filename<<", ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
				myfile2 <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
				//	delete[] h_w->vector_weight;
				// h_w->release();
				// delete h_w;
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


			}
			img.release();
			imgOrg.release();
			for (int i=0;i<n_channels;i++)
			{
				imFils[i].release();
				imFils[i+1].release();
			}
			delete[] imFils;
			G.release();
		}
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
		myfile<<outfilelist.str();
		myfile.close();
		myfile2.close();
	}
	inputFile.close();







};