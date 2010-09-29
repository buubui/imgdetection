#include "stdafx.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;
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

		outputfile<<"output/his"<<posStr<<"_winSvm_"<<local.tm_year+1900<<"_"<<local.tm_mon<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		outputfile2<<"output/his"<<posStr<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
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
				calcHisOfCellsInWnd2(G,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSize,9,his_wnd );
			//	HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2,h_w);

				myfile2 << pos<<"\t";
				for (int i=0;i<h_w.rows*h_w.cols;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					double v =h_w.at<double>(0,i); 
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




void svmGenerateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg)
{	
	ifstream inputFile;
	printf("%s\n",posfilelist.c_str());
	inputFile.open (posfilelist.c_str());
	//inputNegFile.open (negfilelist.c_str());
	string filepath,filename;
	int randTime=randTimePos;
	string pos ="+1";
	string posStr = pos.compare("+1")==0?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
	float scale=1.,step=1.2;
	if (inputFile.is_open())
	{
		getline (inputFile,filepath);
		ofstream myfile,myfile2;
		//	SYSTEMTIME st;
		//	GetSystemTime(&st);
		time_t curr;
		tm local;
		time(&curr); // get current time_t value
		local=*(localtime(&curr)); // dereference and assign
		stringstream outputfile,outputfile2; 

		outputfile<<"output/his"<<"_winSvm_"<<local.tm_year+1900<<"_"<<local.tm_mon<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		outputfile2<<"output/his"<<"_svmLight_"<<local.tm_year+1900<<"_"<<local.tm_mon<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
		myfile.open(outputfile.str().c_str());
		myfile2.open(outputfile2.str().c_str());
		Size cellSz,wndSz,tmp;
		tmp.width = wndSize.width / cellSize.width;
		tmp.height = wndSize.height / cellSize.height;
		Mat his_wnd ;
		HIS h_w;
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
			
			double maxSz=maxWndSz.width*maxWndSz.height;
			double minSz = wndSize.width*wndSize.height;
			Rect realRect=resizeImg(img,maxSz,minSz,false);
		//	Mat img = imread(filepath+filename);
			Mat* imFils = imFilter(img,true);
			Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
			bool continueScale = false;
			Point startP;
			for (int j =0;j<randTime;)
			{
				
				if(randTime>1){
				
					if (!continueScale)
					{
						int rnd;
						int sizeW = img.cols - wndSize.width;
						int sizeH = img.rows - wndSize.height;
						switch(j){
							case 0: startP.x=img.cols/2;startP.y=img.rows/2;break;
							case 1: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							case 2: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/4;break;
							case 3: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							case 4: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+3*sizeH/4;break;
							case 5: startP.x=wndSize.width/2+ sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							case 6: startP.x=wndSize.width/2+ 3*sizeW/4;startP.y=wndSize.height/2+sizeH/2;break;
							case 7: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+sizeH/4;break;
							case 8: startP.x=wndSize.width/2+ sizeW/2;startP.y=wndSize.height/2+3*sizeH/4;break;
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
					if(pos.compare("+1")==0){
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
				//	imshow(filename,img);
				//		Mat* imFils = imFilter(img_slideWnd);
				//		Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
//				Mat his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9,his_wnd);
		//		HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2,h_w);


				myfile2 << pos<<"\t";
				for (int i=0;i<h_w.cols;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					double v =h_w.at<double>(0,i); 
					myfile << v<<"\t";
					if(v!=0)
						myfile2 << i+1<<":"<<v<<"\t";
				}
				myfile << pos<<"\n";
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
			G.release();
			imFils[0].release();
			imFils[1].release();
			

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
		myfile.close();
		myfile2.close();
	}
	inputFile.close();







}




