//#include "svmData.h"
#include "stdafx.h"
using namespace System;
using namespace System::Xml;
extern Size cellSize,blockSize,wndSize,maxWndSz;
extern cv::Vec2i blockOverlap;
extern cv::Vec2f regionOverlap;
extern float delPart;

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




void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg,bool useMaxChannel,bool useSmooth,bool useLBP,bool useNewTech)
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
	float scale=1.,step=1.2;          // change step.
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
		Mat his_wnd ;
		HIS h_w;
		HIS* h_ws=NULL;
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

						scale = scale * step;
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
				Mat img_slideWnd=img(slideWnd);
				Mat G1=G(slideWnd);
				if(useSmooth)
					G1=GaussianBlurBlock(G(slideWnd),blockOverlap);
				
				
				//	imshow(filename,img);
				//		Mat* imFils = imFilter(img_slideWnd);
				//		Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
//				Mat his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				int maxD=180.; 
				int n_bins=9;
				if(useLBP)
				{
					calcLBP(G1,0);
					maxD=256.;
					n_bins=8;
				}
				if(!useNewTech)
				{
					calcHisOfCellsInWnd2(G1,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,n_bins,his_wnd,maxD);
			//		calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
			//		HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				//	calcHistOfWnd(his_wnd,blockSize,blockOverlap,2,h_w);
					calcHistOfWndNew2(his_wnd,blockSize,blockOverlap,2,h_w);
				}else{
					h_w=calcHistOfWndNew(G1,cellSz,n_bins,h_ws);
				}
				if(useSmooth)
					G1.release();
				
				myfile2 << pos<<"\t";
				for (int i=0;i<h_w.cols;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					float v =h_w.at<float>(0,i); 
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
		if(h_ws!=NULL){
			for (int i=0;i<4;i++)
			{
				h_ws[i].release();
			}
			delete[]h_ws;
		}
	//	myfile.close();
		myfile2.close();
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