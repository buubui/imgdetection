// ObjectDetection.cpp : main project file.

#include "stdafx.h"
#include "MainWindow.h"
//using namespace System;
using namespace ObjectDetection;
// TestOpenCv.cpp : Defines the entry point for the console application.
//
//#include "imFilter.h"
//#include <math.h>
//#include "HOG.h"
#include "ctime"
#include "glbVars.h"
//#include "meanshift.h"
//#include "HardSample.h"

void takefalseImg(string ffile)
{
	ifstream in1,in2;
	in1.open(ffile.c_str());
	in2.open("input/testPos.txt");
	if(!in1.is_open())
		return;
	string tmp1,tmp2;
	bool b=true;
	string path;
	getline(in2,path);
	for(int i=1;;i++)
	{
		if(b)
		{
			getline(in1,tmp1);
			if(tmp1.size()<1)
				break;
			b=false;
		}
		getline(in2,tmp2);
		if(i==atoi(tmp1.c_str()))
		{
			//
			Mat img = imread(path+tmp2);
			imwrite("output/false/"+tmp2,img,vector<int>(CV_IMWRITE_PNG_COMPRESSION,4));
			imshow("img_"+tmp2,img);
			b=true;
		}
	}
}
void takeHardList(string fileresult,string fileData)
{
	ifstream  inResult,inData;
	stringstream strstreams;
	//	inList.open(filelist.c_str());
	inResult.open(fileresult.c_str());
	inData.open(fileData.c_str());
	int i=0;
	string lineResult,lineData;
	while(!inResult.eof()){
		//	cout<<i++<<endl;
		getline(inResult,lineResult);
		//	getline(inList,lineList);
		if(lineResult.length()<1)
			break;
		getline(inData,lineData);
		if(lineData[0]=='+'){
			double d;
			d=atof(lineResult.c_str());
			if(d<0)
				strstreams<<lineData<<endl;
		}
		else if(lineData[0]=='-'){
			double d;
			d=atof(lineResult.c_str());
			if(d>0)
				strstreams<<lineData<<endl;
		}
	}
	inData.close();
	inResult.close();
	ofstream outHard;
	outHard.open("output/hard.txt");
	outHard<<strstreams.str();
	outHard.close();

}
void takeHardEx(string filelist, string filedata){
	ifstream inList, inData;

	stringstream strstreams;
	inList.open(filelist.c_str());
	inData.open(filedata.c_str());
	int i=0;
	string lineList,lineData;
	while(!inList.eof()){
		getline(inList,lineList);
		if(lineList.length()<1)
			break;
		int line = atoi(lineList.c_str());
		for (;i<=line;i++)
		{
			getline(inData,lineData);
			if(i==line)
				strstreams<<lineData<<endl;
		}
	}
	inData.close();
	inList.close();
	ofstream outHard;
	outHard.open("output/hard.txt");
	outHard<<strstreams.str();
	outHard.close();

}

void takeType(string fnamein,string fnameout,char type)
{
	ifstream  inData;
	ofstream outData;
	inData.open(fnamein.c_str());
	outData.open(fnameout.c_str());
	string lineData;
	while(!inData.eof()){
		getline(inData,lineData);
		if(lineData.length()<1)
			break;
		if(lineData[0]!=type)
			continue;
		outData <<lineData<<endl;

	}
	inData.close();
	outData.close();
}
void splitList(string fname, int n_part)
{
	ifstream inData;
	inData.open(fname.c_str());
	ofstream* outData = new ofstream[n_part];
	
	for (int i=0;i<n_part;i++)
	{
		stringstream strName;
		strName<<"output/list_"<<i<<".txt";
		outData[i].open(strName.str().c_str());
	}
	string lineData;
	getline(inData,lineData);
	for (int i=0;i<n_part;i++)
	{
		outData[i]<<lineData<<endl;
	}
	int iLine=0;
	while(!inData.eof()){
		getline(inData,lineData);
		if(lineData.length()<1)
			break;
	//	for (int i=0;i<n_part;i++)
	//	{
			outData[iLine%n_part]<<lineData<<endl;
	//	}
		iLine++;

	}
	inData.close();
	for (int i=0;i<n_part;i++)
	{
		outData[i].close();
	}
}
[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	clock_t t1,t2;
	t1 = clock();	
	loadConfig();

	/*Mat img=imread("d:\\Lectures\\Luan_van\\DATASET\\INRIAPerson\\train_64x128_H96\\pos\\person_and_bike_209a.png");
	Mat* imFils;
	Mat G;
	int n_channels=img.channels();
	imFils = imFilterChannels(img,true);
	G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
	int* x_corr=NULL;int* y_corr=NULL; int n_x=0, n_y=0;
	Mat his_cells_wnd ;
	HIS his_wind;
	HIS* h_ws=NULL;
	cv::Rect slideWnd(0,0,64,128);
	slideWnd.x = (img.rows - slideWnd.height)/2;
	slideWnd.y = (img.cols - slideWnd.width)/2;
	svmClassify( img, G,slideWnd, cellSize, 1.,  n_x,  n_y, x_corr,y_corr,  his_cells_wnd,  his_wind,h_ws, 3, true,false,false,4);
	compressPCA(his_wind,0)*/

	/*cout.precision(4);
	cout << 1.23456789<<endl;
	cout << 1.23456789<<endl;*/
//	takeHardList("D:\\Lectures\\Luan_van\\tool\\svm_light_windows\\classify_result1.txt","D:\\Lectures\\Luan_van\\tool\\svm_light_windows\\val_1.txt");
//	splitList("input/trainNeg.txt",2);
//	int a[4]={0,1,0,1};
//	cout << bin2dec(a,4);

//	Mat img = imread("D:\\Lectures\\Luan_van\\DATASET\\INRIAPerson\\70X134H96\\Test\\pos\\crop_000002b.png");
//	Mat img2;
//	cvtColor(img,img,CV_BGR2HSV);
////	cout<<img2.channels()<<img2.type()<<endl;
//	imshow("hsv",img);
//	//Vec3b v;
//	//v= img2.at<Vec3b>(5,5);
//	//cout<<(int)v[0]<<" "<<(int)v[1]<<" "<<(int)v[2]<<endl;
//	waitKey(0);
	//img=img(Rect(0,0,64,128));
	//int n_channels=1;
	//Mat* imFils;
	//Mat G;
	//
	//n_channels=img.channels();
	//imFils = imFilterChannels(img,true);
	//G = calcGradientOfPixelsMaxChannel(imFils,n_channels);
	//Mat* h_w=NULL;
	//int*x_corr=NULL;int*y_corr=NULL;
	//int n_x=0, n_y=0;
	//float scale=1.;
	//int n_bins=9;
	//HIS his_wind;
	//if(n_x==0)
	//	calcGrid(G,blockSize,cellSize, cv::Size(blockSize.width*cellSize.width/2,blockSize.height*cellSize.height/2),x_corr,y_corr, n_x, n_y);
	//Mat m_tmp;
	//while(true)
	//{
	////	HIS* h_w=NULL;
	////	HIS H=calcHistOfWndNew(img,cellSize,9,h_w);
	//	
	//	calcHisOfGrid(G,blockSize,cellSize,cv::Size(blockSize.width*cellSize.width/2,blockSize.height*cellSize.height/2),x_corr,y_corr,n_x,n_y,scale, n_bins,his_wind,m_tmp);
	//	
	////	H.release();
	//}

	/*printf("%d %d\n",H.rows,H.cols);
	for (int i=0;i <200;i++)
	{
		cout<<H.at<float>(0,i)<<"; ";
	}
	waitKey(0);*/
	
	/*	Mat* imgfils=imFilterChannels(im,true);
		int n_channels=im.channels();
		Mat G = calcGradientOfPixelsMaxChannel(imgfils,n_channels);
		Mat T;
		GaussianBlur(G,T,cv::Size(3,3),0.5*8*3);
		for (int i=100;i<300;i++)
		{
			printf("%f %f %f %f\n",G.at<Gradient>(i,i)[0],G.at<Gradient>(i,i)[1],T.at<Gradient>(i,i)[0],T.at<Gradient>(i,i)[1]);
		}
		for (int j=0;j<im.channels();j++)
		{
			imgfils[j].release();
			imgfils[j+1].release();
		}
		G.release();
		delete[] imgfils;*/

//	}
	
	/*imshow("0",imgfils[0]);
	imshow("1",imgfils[1]);
	imshow("2",imgfils[2]);
	imshow("3",imgfils[3]);
	imshow("4",imgfils[4]);
	imshow("5",imgfils[5]);
	waitKey(0);*/
	/*srand(time(NULL));
	for (int i=0;i<20;i++)
	{
		cout<<rand()%100<<endl;
	}*/
	
//	GenHardSample("input/thu/train.txt","input/thu/index.txt","input/thu/NegHardSample.txt");
 /************************************************************************/
 /*                  Generating data for SVM                             */
 /************************************************************************/
//	Rect r(100,100,33,33);
//for (int i=0;i<100000000000;i++)
//{
//	r= getRect(0,0,2);
//}
	
//	svmGenerateData2("input/Train_Pos.txt","input/Train_Neg_P1.txt",1,8);
//	svmGenerateData2("input/Test_Pos.txt","input/Test_Neg.txt",1,8);
//	svmGenerateData2("input/a.txt","input/b.txt",1,5000000,3, 1., 1.05, true,false,false,2);

//	svmGenerateData2("input/trainPos.txt","input/trainNeg.txt",1,2,3,1.,1.1,true,false,false,3);
//	svmGenerateData2("input/NULL.txt","input/trainNeg_0.txt",1,1,true,false,false);
//	svmGenerateData2("input/NULL.txt","input/trainNeg_1.txt",1,1,true,false,false);
//	svmGenerateData2("input/NULL.txt","input/trainNeg.txt",1,6,true,false,false,-1);
//	svmGenerateData2("input/NULL.txt","input/trainNeg.txt",1,6,true,false,false,-1);
/*	svmGenerateData2("input/NULL.txt","input/trainNeg.txt",1,6,true,false,false,-1);
	svmGenerateData2("input/NULL.txt","input/trainNeg.txt",1,6,true,false,false,-1);
	svmGenerateData2("input/NULL.txt","input/trainNeg.txt",1,6,true,false,false,-1);*/

//	svmGenerateData2("input/testPosF.txt","input/testNegF.txt",1,10,true,false,false,-1);
	//computePCA("input/trainPos.txt","input/trainNeg.txt",1,1,3,1.,1.5,true,false,false,3);

	/*PCA p=computePCA("input/a.txt","input/b.txt",1,5,3,1.,1.1,true,false,false,4,700);
	backupPCA(p,"output/pca.yml");
	PCA p1=restorePCA("output/pca.yml");
	cout <<p1.eigenvalues.rows<<" "<<p1.eigenvalues.cols<<endl;
	cout <<p1.eigenvectors.rows<<" "<<p1.eigenvectors.cols<<endl;
	cout <<p1.mean.rows<<" "<<p1.mean.cols<<endl;*/

//	svmGenHardList("input/weight.txt","input/a.txt","input/b.txt","temp",1,4,1.,100,true,false,false,-1);
	//svmGenHardList("input/weightj3.txt","input/trainPos.txt","input/trainNeg.txt","train",1,400,3,1.,1.05,true,false,false,3);
//	svmGenHardList("input/weight.txt","input/testPosF.txt","input/testNegF.txt","test",1,350,1.,1.05,true,false,false,-1);
	/************************************************************************/
	/*                      Test new mean shift                             */
	/************************************************************************/
	//Mat* means;
	//int n_mean;
	//int p_mean;
	//newMeanshiftFromFile("output/crop001501b_multiscale_2.txt",0.,1,means,n_mean,p_mean);

	/************************************************************************/
	/*                Test XML                       */
	/************************************************************************/
//	XmlDocument ^ doc = gcnew XmlDocument;
//
//	doc->Load("input/file.xml");
////	XmlTextReader^ reader= gcnew XmlTextReader("input/test.xml");
////	cout<<msclr::interop::marshal_as<std::string>(reader->);
//	XmlNodeList^ nl= doc->GetElementsByTagName("object");
//	XmlNodeList^ att=nl->Item(0)->ChildNodes->Item(4)->ChildNodes;
//	for (int i=0;i<att->Count;i++)
//	{
//		Console::WriteLine(att->Item(i)->InnerText);
//	}
	//////////////////////////////////VOC//////////////////////////////////////////////////////
	System::String^ annpath="D:\\Lectures\\Luan_van\\DATASET\\VOC\\VOCdevkit\\VOC2010\\Annotations\\";
	Rect* rects;
	int n_rect;
	//	VOCAnnRects("D:\\My Documents\\VOC2009\\Annotations\\2009_005311.xml","person",rects,n_rect);
	//	cout<<n_rect<<endl;
	std::string filelist="D:\\Lectures\\Luan_van\\DATASET\\VOC\\VOCdevkit\\VOC2010\\ImageSets\\Main\\person_val.txt";
	//	VOCSvmGenerateData2(annpath,"D:\\Lectures\\Luan_van\\DATASET\\VOC\\VOCdevkit\\VOC2010\\JPEGImages\\",".jpg","person",filelist,1,5,2.,5.,true,false,false);
	//	takeType("output/train_person.txt","output/train_person_pos.txt",'+');
	//	takeType("output/train_person.txt","output/train_person_neg.txt",'-');
	///////////////////////////////////////////////////////////////////////////////////////////////
	//ifstream inputFile;
	//printf("%s\n",posfilelist.c_str());
	//inputFile.open (posfilelist.c_str());
	////inputNegFile.open (negfilelist.c_str());
	//string filepath,filename;
	//Rect slideWnd(0,0,wndSize.width,wndSize.height);
	//float scale=1.,step=1.2;
	//string tmp;
	//std::vector<std::string> strs;
	//while (!inputFile.eof())
	//{
	//	getline (inputFile,tmp);
	//	
	//	char* s = (char*)(tmp.c_str());
	//	boost::split(strs,s , boost::is_any_of(" "));
	//	if (strs.size()>2){
	//		int _i=atoi(strs[strs.size()-1].c_str());
	//		if(_i!=-1)
	//		{
	//			VOCAnnRects(annpath+gcnew System::String(strs[0].c_str())+gcnew System::String(".xml"),"person",rects,n_rect);

	//			printf("%s: %d \n",strs[0].c_str(),n_rect);
	//		}
	//	}

	//}


/************************************************************************/
/*                         Load  GUI                                    */
/************************************************************************/

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew MainWindow());


	t2 = clock();
	printf("Running time: %f (mins)\n",(float)(t2-t1)/(60*CLOCKS_PER_SEC));
	return 0;
}

//int main(array<System::String ^> ^args)
//{
//	loadConfig();
//	
////	svmGenerateData2("input/trainPos.txt","input/trainNeg2.txt",1,10);
//
////	string path="D:\\Lectures\\Luan_van\\DATASET\\INRIAPerson\\Test\\pos\\";
//	string path="D:\\Lectures\\Luan_van\\DATASET\\INRIAPerson\\Train\\pos\\";
//	string fname="person_117";
//	string ext=".png";
//	Mat imgOrg = imread(path+fname+ext);
//	Mat img;
//	float maxSz=500*400.;
//	float minSz = wndSize.width*wndSize.height;
//	float t=(float)imgOrg.rows*imgOrg.cols;
//	float resizeScale=t>maxSz?sqrt(maxSz/t):1.;
//	resizeScale=t<minSz?sqrt(minSz/t):resizeScale;
//	resize(imgOrg,img,Size(imgOrg.cols*resizeScale,imgOrg.rows*resizeScale),resizeScale,resizeScale);
//	cout<<img.rows<<" "<<img.cols<<" "<<resizeScale<<endl;
//	imgOrg.release();
////	multiscaleExp(path+fname+ext,1.2,0.);
//
//
//	int n_mean,p_mean;
//	float*means;
//	meanshiftFromFile("output/"+fname+"_multiscale.txt",100,8,means,n_mean,p_mean);
//	
//	for (int i=0;i<n_mean*p_mean;i++)
//	{
//		cout<<means[i]<<", ";
//		if(i>0&&i%p_mean==p_mean-1)
//			cout<<endl;
//	}
//	drawRect2Img(img,"output/"+fname+"_meanshift.txt");
//	imwrite("output/"+fname+"_meanshift.png",img,vector<int>(CV_IMWRITE_PNG_COMPRESSION,4));
//	imshow(fname+"_meanshift",img);
//
//	//	takefalseImg("input/false_pos.txt");
//	//CHECK MEMORY LEAKING
////	while(1){
////	Mat img = imread("E:\crop_000027.png");
////	Mat* imFils = imFilter(img);
////	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
////	Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img.cols,img.rows),cellSize,9);
////	
////		
////	HIS* h_w = calcHistOfWnd(his_wnd,blockSize,Vec2i(1,1),2);
//////	float* vv = &(h_w->vector_weight[0]);
//////	cout<<*vv<<endl;
//////	delete[] h_w->vector_weight;
//////	cout<<*vv<<endl;
////	delete h_w;
////	
////	for (int i=0;i<his_wnd.rows;i++)
////	{
////		for (int j=0;j<his_wnd.cols;j++)
////		{
////			HIS* hh=his_wnd.at<HIS*>(i,j);
////		//	delete[] hh->vector_weight;
////			delete hh;
////		}
////	}
////	his_wnd.release();
////	img.release();
////	imFils[0].release();
////	imFils[1].release();
////	G.release();
////	}
//	
//
//
////	generateData("input/filelist_pos.txt",1,1);
////	generateData("input/filelist_neg.txt",-1,2);
//	//	ifstream inputfile;
//	//	inputfile.open ("input/filelist.txt");
//	//	string filepath,filename;
//	//	if (inputfile.is_open())
//	//	{
//	////		while (! inputfile.eof() )
//	////		{
//	//			getline (inputfile,filepath);
//	//			getline (inputfile,filename);
//	//			
//	////		}
//	//	}
//	//	inputfile.close();
//	//
//	//	ifstream conffile;
//	//	conffile.open ("input/config.txt");
//	//	string tmp;
//	//	Size cellSize,blockSize;
//	//	if (conffile.is_open())
//	//	{
//	//		//		while (! inputfile.eof() )
//	//		//		{
//	//		getline (conffile,tmp);//cell
//	//		getline (conffile,tmp);
//	//		cellSize.width = atoi(tmp.c_str());
//	//		getline (conffile,tmp);
//	//		cellSize.height = atoi(tmp.c_str());
//	//		getline (conffile,tmp);//block
//	//		getline (conffile,tmp);
//	//		blockSize.width = atoi(tmp.c_str());
//	//		getline (conffile,tmp);
//	//		blockSize.height = atoi(tmp.c_str());
//	//
//	//		//		}
//	//	}
//	//	conffile.close();
//	//	Mat img = imread(filepath+filename);
//	//	imshow("asdasd",img);
//	//	Mat* imFils = imFilter(img);
//	//	/*Mat img_gray;
//	//	cvtColor(img,img_gray,CV_BGR2GRAY);
//	//	equalizeHist(img_gray,img_gray);
//	//	*/
//	//	
//	//	//imshow("filter x",imFils[0]);
//	//	//imshow("filter y",imFils[1]);
//	//	
//	//	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
//	//	
//	//	printf("\nGRADIENT:\n");
//	////	for(int i=0;i<30;i++)
//	////		for(int j=0;j<20;j++)
//	////			printf("%f: %f \n",G.at<Gradient>(i,j)[0],G.at<Gradient>(i,j)[1]);
//	//
//	//	/*HIS* his = calcHisOfCell(G,Rect(50,50,10,10),9);
//	//	for (int i=0;i<9;i++)
//	//	{
//	//		printf("%f, ",his->vector_weight[i]);
//	//	}*/
//	////	Mat his_wnd = calcHisOfCellsInWnd(G,Rect(0,0,64,128),Size(8,8),9);
//	//	Rect R(0,0,img.cols,img.rows);
//	//	cout <<"RECT:"<< R.width << ";"<<img.cols<<";"<<G.cols;
//	//	Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img.cols,img.rows),cellSize,9);
//	//	ofstream myfile;
//	////	SYSTEMTIME st;
//	////	GetSystemTime(&st);
//	//	time_t curr;
//	//	tm local;
//	//	time(&curr); // get current time_t value
//	//	local=*(localtime(&curr)); // dereference and assign
//	//	stringstream outputfile; 
//	//	outputfile<<"output/hisCell_"<<filename<<"_cell"<<cellSize.width<<"x"<<cellSize.height<<"_block"<<blockSize.width<<"x"<<blockSize.height<<"_"<<local.tm_year+1900<<"_"<<local.tm_mon+1<<"_"<<local.tm_mday<<"_"<<local.tm_hour<<"_"<<local.tm_min<<".txt" ;
//	//	myfile.open(outputfile.str().c_str());
//	//	printf("\ncalcHisOfCellsInWnd\n");
//	//	//myfile <<"[";
//	//	myfile <<his_wnd.rows<<", "<<his_wnd.cols<<"\n";
//	//	for(int ii =0; ii<his_wnd.rows;ii++)
//	//	{
//	//		for(int jj =0; jj<his_wnd.cols;jj++)
//	//		{
//	//
//	//			
//	//			for (int i=0;i<9;i++)
//	//			{
//	//				printf("%2.2f, ",his_wnd.at<HIS*>(ii,jj)->vector_weight[i]);
//	//				myfile << his_wnd.at<HIS*>(ii,jj)->vector_weight[i] ;
//	//				if(i == 8) continue;
//	//				myfile<<", ";
//	//			}
//	//			//if(ii==his_wnd.rows-1 && jj == his_wnd.cols-1) continue;
//	//			myfile << "\n";
//	//		
//	//		printf("\n");
//	//		}
//	//		
//	//
//	//	}		
//	//			
//	//			myfile.close();
//	//			printf("\n %d %d",his_wnd.rows,his_wnd.cols);
//	//			
//	//
//	////	calcHistOfBlockInWnd(his_wnd,Rect(2,2,3,3));
//	//	/*HIS* h_n = NormalizeBlock(his,2);
//	//	for (int i=0;i<h_n->n_bins;i++)
//	//	{
//	//	printf("%f, ",h_n->vector_weight[i]);
//	//	}*/
//	//	/*HIS* h_w = calcHistOfWnd(his_wnd,Size(3,3),Vec2i(2,2),2);
//	//
//	//
//	//	printf("\ncalcHistOfWnd\n");
//	//	for (int i=0;i<h_w->n_bins;i++)
//	//	{
//	//		printf("%f ; ",h_w->vector_weight[i]);
//	//	}
//	//	printf("\n n_BIN %d\n",h_w->n_bins);*/
//	//
//	//	
//	//	/*Mat aa(2,5,DataType<Gradient>::type);
//	//	for (int i=0;i<aa.rows;i++)
//	//	{
//	//		for (int j=0;j<aa.cols;j++)
//	//		{
//	//			aa.at<Gradient>(i,j) = Gradient(2,3);
//	//		}
//	//	}*/
//	//
//	//	/*Mat aa(20,7,DataType<Gradient>::type);
//	//	for (int i=0;i<aa.rows;i++)
//	//	{
//	//		for (int j=0;j<aa.cols;j++)
//	//		{
//	//			aa.at<Gradient>(i,j) = Gradient(2,3);
//	//		}
//	//	}*/
//	//	/*int n =9;
//	//	Vec<int,5> a;
//	//	float * b = new float[10];
//	//	float c[10];
//	//	printf("size %d",sizeof(c));*/
//	//	//G.release();
//	//	
//	cvWaitKey();	
//	return 0;
//}
//
//
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
//		printf("%f", (float)(stop - start)/CLOCKS_PER_SEC);
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


