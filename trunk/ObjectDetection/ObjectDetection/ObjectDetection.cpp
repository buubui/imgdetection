// ObjectDetection.cpp : main project file.

#include "stdafx.h"
#include <boost/algorithm/string.hpp>
using namespace System;

// TestOpenCv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "imFilter.h"
#include <math.h>
#include "HOG.h"
#include "ctime"
void getWeight(string filename,Mat* &weight, double& b);
//int _tmain(int argc, _TCHAR* argv[])
//{
//	clock_t start, stop;
//	start = clock();
//	IplImage* img = cvLoadImage("e:\\2009_003218.jpg");
//		int w = img->width -1;
//		int h = img->height -1;
//		for (int x=0;x<100;x++)
//		for (int i=0;i<w;i++)
//			for (int j=0;j<h;j++){
//				CvScalar a =cvGet2D(img, i, j);
//			//	printf("%f",a.val[1]);
//			}
//		stop = clock();
//		printf("%f", (double)(stop - start)/CLOCKS_PER_SEC);
//
//	return 0;
//}
//#include <windows.h>
//#include <tchar.h> 
//#include <stdio.h>
//#include <strsafe.h>
//
//void DisplayErrorBox(LPTSTR lpszFunction);
//
//int _tmain(int argc, TCHAR *argv[])
//{
//	WIN32_FIND_DATA ffd;
//	LARGE_INTEGER filesize;
//	TCHAR szDir[MAX_PATH];
//	size_t length_of_arg;
//	HANDLE hFind = INVALID_HANDLE_VALUE;
//	DWORD dwError=0;
//
//	// If the directory is not specified as a command-line argument,
//	// print usage.
//
//	if(argc != 2)
//	{
//		_tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
//		return (-1);
//	}
//
//	// Check that the input path plus 3 is not longer than MAX_PATH.
//	// Three characters are for the "\*" plus NULL appended below.
//
//	StringCchLength(argv[1], MAX_PATH, &length_of_arg);
//
//	if (length_of_arg > (MAX_PATH - 3))
//	{
//		_tprintf(TEXT("\nDirectory path is too long.\n"));
//		return (-1);
//	}
//
//	_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);
//
//	// Prepare string for use with FindFile functions.  First, copy the
//	// string to a buffer, then append '\*' to the directory name.
//
//	StringCchCopy(szDir, MAX_PATH, argv[1]);
//	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
//
//	// Find the first file in the directory.
//
//	hFind = FindFirstFile(szDir, &ffd);
//
//	if (INVALID_HANDLE_VALUE == hFind) 
//	{
//		DisplayErrorBox(TEXT("FindFirstFile"));
//		return dwError;
//	} 
//
//	// List all the files in the directory with some info about them.
//
//	do
//	{
//		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//		{
//			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
//		}
//		else
//		{
//			filesize.LowPart = ffd.nFileSizeLow;
//			filesize.HighPart = ffd.nFileSizeHigh;
//			_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
//		}
//	}
//	while (FindNextFile(hFind, &ffd) != 0);
//
//	dwError = GetLastError();
//	if (dwError != ERROR_NO_MORE_FILES) 
//	{
//		DisplayErrorBox(TEXT("FindFirstFile"));
//	}
//
//	FindClose(hFind);
//	return dwError;
//}
//
//
//void DisplayErrorBox(LPTSTR lpszFunction) 
//{ 
//	// Retrieve the system error message for the last-error code
//
//	LPVOID lpMsgBuf;
//	LPVOID lpDisplayBuf;
//	DWORD dw = GetLastError(); 
//
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//		FORMAT_MESSAGE_FROM_SYSTEM |
//		FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL,
//		dw,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR) &lpMsgBuf,
//		0, NULL );
//
//	// Display the error message and clean up
//
//	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
//		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
//	StringCchPrintf((LPTSTR)lpDisplayBuf, 
//		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
//		TEXT("%s failed with error %d: %s"), 
//		lpszFunction, dw, lpMsgBuf); 
//	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
//
//	LocalFree(lpMsgBuf);
//	LocalFree(lpDisplayBuf);
//}
//
//








Size cellSize,blockSize,wndSize;






//filelist_pos.txt
void generateData(string inputfilelist, int pos,int randTime){
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
				Mat* imFils = imFilter(img_slideWnd);
				Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
				Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSize,9);
				HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);


				myfile2 << pos<<"\t";
				for (int i=0;i<h_w->n_bins;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					double v =h_w->vector_weight[i]; 
					myfile << v<<"\t";
					if(v!=0)
						myfile2 << i+1<<":"<<v<<"\t";
				}
				myfile << pos<<"\n";
				myfile2 <<"\n";
				delete h_w;
				his_wnd.release();
				G.release();
				imFils[0].release();
				imFils[1].release();

			}
			img.release();
			
			
		}
		myfile.close();
		myfile2.close();
	}
	inputfile.close();


}


void multiscaleExp(Mat img,float step )
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
	for (int h=0;h<img.rows;h+=50)
	{
		//printf("%d\n",h);
		for (int w=0;w<img.cols;w+=50)
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
				
				Mat img_slideWnd=img(slideWnd);
				
				
				Mat his_wnd = calcHisOfCellsInWnd2(G(slideWnd),Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSz,9);
				HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
				Mat his(1,h_w->n_bins,CV_64F);
				//printf("%d",h_w->n_bins);
				for (int i=0;i<h_w->n_bins;i++)
				{
					his.at<double>(0,i)=h_w->vector_weight[i];
				}
				Mat R = his* (*weight ) - b;
				double v = R.at<double>(0,0);
				if(v>0){
//					printf(" (%d,%d) (%dx%d) %f %f\n",startP.x,startP.y, wndSz.width,wndSz.height,scale, v);
					printf("%d, %d, %f\n",startP.x+wndSz.width/2,startP.y+wndSz.height/2,wndSz.width/baseWidth);
					rectangle(result,slideWnd,Scalar(0,0,256));
					stringstream out;
					out<<v;
					putText(result,out.str(),Point(slideWnd.x,slideWnd.y-3),FONT_HERSHEY_COMPLEX_SMALL,0.5,Scalar(0,256,0));
					stringstream outputfile;
					outputfile <<"img "<<startP.x<<" "<<startP.y<<" "<<wndSz.width<<" "<<wndSz.height;
				//	imshow(outputfile.str(),img_slideWnd);
					if(v>max){
						max=v;
						MaxWnd = slideWnd;
					}
				}
				delete h_w;
		//		imFils[0].release();
		//		imFils[1].release();
				his_wnd.release();
		//		G.release();
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
		
	//	imshow("max",img(MaxWnd));
		imshow("result",result);
}
void loadConfig()
{
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


}
void generateData2(string posfilelist, string negfilelist,int randTimePos,int randTimeNeg){
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



	ifstream inputFile;
	printf("%s\n",posfilelist.c_str());
	inputFile.open (posfilelist.c_str());
	//inputNegFile.open (negfilelist.c_str());
	string filepath,filename;
	int pos=1,randTime=randTimePos;
	string posStr = pos==1?"Pos":"Neg";
	Rect slideWnd(0,0,wndSize.width,wndSize.height);
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
		for (int i=0;;i++)

			//		while (! inputfile.eof() )
		{
			getline (inputFile,filename);
			if (filename.length()<2)
			{
				if(pos!=1)
					break;
				inputFile.close();
				inputFile.open(negfilelist.c_str());
				if(!inputFile.is_open())
					printf("XXXXXX");
				pos=-1;
				randTime=randTimeNeg;
				inputFile.seekg(0, ios::beg);
				getline (inputFile,filepath);
				continue;
				//break;
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
				}else{
					if(pos>0&&(img.rows > slideWnd.height)){
						slideWnd.x = (img.rows - slideWnd.height)/2;
						slideWnd.y = (img.cols - slideWnd.width)/2;
					}
				}
				printf("%d. %s (%d,%d)\n",i+1,filename.c_str(),slideWnd.x,slideWnd.y);
				Mat img_slideWnd=img(slideWnd);
				//	imshow(filename,img);
				Mat* imFils = imFilter(img_slideWnd);
				Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
				Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img_slideWnd.cols,img_slideWnd.rows),cellSize,9);
				HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);


				myfile2 << pos<<"\t";
				for (int i=0;i<h_w->n_bins;i++)
				{
					//	printf("%f ; ",h_w->vector_weight[i]);
					double v =h_w->vector_weight[i]; 
					myfile << v<<"\t";
					if(v!=0)
						myfile2 << i+1<<":"<<v<<"\t";
				}
				myfile << pos<<"\n";
				myfile2 <<"\t #"<<filename<<"\t ("<<slideWnd.x<<", "<<slideWnd.y<<", "<<slideWnd.width<<", "<<slideWnd.height<<")\n";
				delete h_w;
				his_wnd.release();
				G.release();
				imFils[0].release();
				imFils[1].release();

			}
			img.release();


		}
		myfile.close();
		myfile2.close();
	}
	inputFile.close();







}

void getWeight(string filename,Mat* &weight, double& b){
	ifstream fi;
	fi.open (filename.c_str());
	string tmp;
	int n;
	if (fi.is_open())
	{
		getline (fi,tmp);
		n = atoi(tmp.c_str());
		weight = new Mat(n,1,CV_64F);
		for (int i=0;i<n;i++)
		{
			getline (fi,tmp);
			
			weight->at<double>(i,0)=atof(tmp.c_str());
		}
		getline (fi,tmp);

		b=atof(tmp.c_str());
	}

}



Rect getRect(int x,int y, float scale)
{
	
	Size cellSz; 
	Size tmp;
	Rect result;
	tmp.width = wndSize.width / cellSize.width;
	tmp.height = wndSize.height /cellSize.height;
	//cellSz = cellSize;
	cellSz.width = ( scale+cellSize.width);
	cellSz.height = ( scale+cellSize.height);

	result.width =cellSize.width*tmp.width*scale; 
	result.height= cellSize.height*tmp.height*scale;
	result.x = x - result.width/2;
	result.y = y - result.height/2;
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
		rectangle(img,r,Scalar(0,0,256));
		
		

		

				}
	}
	ffile.close();

}
int main(array<System::String ^> ^args)
{
//	generateData2("input/a.txt","input/b.txt",1,1);
	//Mat * w;
	//double b;
	//getWeight("w.txt",w,b);
	//printf("%f",b);
	//for (int i=0;i<w->rows;i++)
	//{
	//	printf("%f ;",w->at<double>(i,0));
	//}
	loadConfig();
	Mat img = imread("E:\\crop001704.png");
	//rectangle(img,Rect(10,10,100,200),Scalar(0,0,256));
//	imshow("asd",img);
//	multiscaleExp(img,0.01);
	//Rect x(112,202,11,11);
	//Rect y(110,194,11,11);
	//
	/*Rect x = getRect(583,268,2.5386);
	Rect y = getRect(284,335,2.853);
	Rect z = getRect(791,328,1.6288);
	Rect a =getRect(77,191,2.0473);
	Rect b = getRect(721,101,1.5835);
	
	rectangle(img,x,Scalar(0,0,256));
	rectangle(img,y,Scalar(0,0,256));
	rectangle(img,z,Scalar(0,0,256));
	rectangle(img,a,Scalar(0,256,0));
	rectangle(img,b,Scalar(0,256,0));*/
	drawRect2Img(img,"../Debug/result1704.txt");
	imwrite("result1704.png",img,vector<int>(CV_IMWRITE_PNG_COMPRESSION,4));
	imshow("asd",img);
	


//	generateData("input/filelist_pos.txt",1,1);
//	generateData("input/filelist_neg.txt",-1,2);
	//	ifstream inputfile;
	//	inputfile.open ("input/filelist.txt");
	//	string filepath,filename;
	//	if (inputfile.is_open())
	//	{
	////		while (! inputfile.eof() )
	////		{
	//			getline (inputfile,filepath);
	//			getline (inputfile,filename);
	//			
	////		}
	//	}
	//	inputfile.close();
	//
	//	ifstream conffile;
	//	conffile.open ("input/config.txt");
	//	string tmp;
	//	Size cellSize,blockSize;
	//	if (conffile.is_open())
	//	{
	//		//		while (! inputfile.eof() )
	//		//		{
	//		getline (conffile,tmp);//cell
	//		getline (conffile,tmp);
	//		cellSize.width = atoi(tmp.c_str());
	//		getline (conffile,tmp);
	//		cellSize.height = atoi(tmp.c_str());
	//		getline (conffile,tmp);//block
	//		getline (conffile,tmp);
	//		blockSize.width = atoi(tmp.c_str());
	//		getline (conffile,tmp);
	//		blockSize.height = atoi(tmp.c_str());
	//
	//		//		}
	//	}
	//	conffile.close();
	//	Mat img = imread(filepath+filename);
	//	imshow("asdasd",img);
	//	Mat* imFils = imFilter(img);
	//	/*Mat img_gray;
	//	cvtColor(img,img_gray,CV_BGR2GRAY);
	//	equalizeHist(img_gray,img_gray);
	//	*/
	//	
	//	//imshow("filter x",imFils[0]);
	//	//imshow("filter y",imFils[1]);
	//	
	//	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
	//	
	//	printf("\nGRADIENT:\n");
	////	for(int i=0;i<30;i++)
	////		for(int j=0;j<20;j++)
	////			printf("%f: %f \n",G.at<Gradient>(i,j)[0],G.at<Gradient>(i,j)[1]);
	//
	//	/*HIS* his = calcHisOfCell(G,Rect(50,50,10,10),9);
	//	for (int i=0;i<9;i++)
	//	{
	//		printf("%f, ",his->vector_weight[i]);
	//	}*/
	////	Mat his_wnd = calcHisOfCellsInWnd(G,Rect(0,0,64,128),Size(8,8),9);
	//	Rect R(0,0,img.cols,img.rows);
	//	cout <<"RECT:"<< R.width << ";"<<img.cols<<";"<<G.cols;
	//	Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img.cols,img.rows),cellSize,9);
	//	ofstream myfile;
	////	SYSTEMTIME st;
	////	GetSystemTime(&st);
	//	time_t curr;
	//	tm local;
	//	time(&curr); // get current time_t value
	//	local=*(localtime(&curr)); // dereference and assign
	//	stringstream outputfile; 
	//	outputfile<<"output/hisCell_"<<filename<<"_cell"<<cellSize.width<<"x"<<cellSize.height<<"_block"<<blockSize.width<<"x"<<blockSize.height<<"_"<<local.tm_year+1900<<"_"<<local.tm_mon<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
	//	myfile.open(outputfile.str().c_str());
	//	printf("\ncalcHisOfCellsInWnd\n");
	//	//myfile <<"[";
	//	myfile <<his_wnd.rows<<", "<<his_wnd.cols<<"\n";
	//	for(int ii =0; ii<his_wnd.rows;ii++)
	//	{
	//		for(int jj =0; jj<his_wnd.cols;jj++)
	//		{
	//
	//			
	//			for (int i=0;i<9;i++)
	//			{
	//				printf("%2.2f, ",his_wnd.at<HIS*>(ii,jj)->vector_weight[i]);
	//				myfile << his_wnd.at<HIS*>(ii,jj)->vector_weight[i] ;
	//				if(i == 8) continue;
	//				myfile<<", ";
	//			}
	//			//if(ii==his_wnd.rows-1 && jj == his_wnd.cols-1) continue;
	//			myfile << "\n";
	//		
	//		printf("\n");
	//		}
	//		
	//
	//	}		
	//			
	//			myfile.close();
	//			printf("\n %d %d",his_wnd.rows,his_wnd.cols);
	//			
	//
	////	calcHistOfBlockInWnd(his_wnd,Rect(2,2,3,3));
	//	/*HIS* h_n = NormalizeBlock(his,2);
	//	for (int i=0;i<h_n->n_bins;i++)
	//	{
	//	printf("%f, ",h_n->vector_weight[i]);
	//	}*/
	//	/*HIS* h_w = calcHistOfWnd(his_wnd,Size(3,3),Vec2i(2,2),2);
	//
	//
	//	printf("\ncalcHistOfWnd\n");
	//	for (int i=0;i<h_w->n_bins;i++)
	//	{
	//		printf("%f ; ",h_w->vector_weight[i]);
	//	}
	//	printf("\n n_BIN %d\n",h_w->n_bins);*/
	//
	//	
	//	/*Mat aa(2,5,DataType<Gradient>::type);
	//	for (int i=0;i<aa.rows;i++)
	//	{
	//		for (int j=0;j<aa.cols;j++)
	//		{
	//			aa.at<Gradient>(i,j) = Gradient(2,3);
	//		}
	//	}*/
	//
	//	/*Mat aa(20,7,DataType<Gradient>::type);
	//	for (int i=0;i<aa.rows;i++)
	//	{
	//		for (int j=0;j<aa.cols;j++)
	//		{
	//			aa.at<Gradient>(i,j) = Gradient(2,3);
	//		}
	//	}*/
	//	/*int n =9;
	//	Vec<int,5> a;
	//	double * b = new double[10];
	//	double c[10];
	//	printf("size %d",sizeof(c));*/
	//	//G.release();
	//	
	cvWaitKey();	
	return 0;
}
