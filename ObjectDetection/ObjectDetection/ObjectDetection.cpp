// ObjectDetection.cpp : main project file.

#include "stdafx.h"

using namespace System;

// TestOpenCv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "imFilter.h"
#include <math.h>
#include "HOG.h"


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













int main(array<System::String ^> ^args)
{
	/*Scalar* value = new Scalar();
	float aa[] = {1,2,3,4,5};
	value = aa;
	Mat* mat = new Mat(1,3,CV_32F,value);
	 double d =(double)( mat->at<double> (1,2));
	mat()*/

	//IplImage* img = cvLoadImage("D:\\Lectures\\Luan_van\\VOC\\VOCdevkit\\VOC2010\\JPEGImages\\2008_003591.jpg");
	///*float a[] = { -1, 2, -3, 4, -5, 6, -7, 8, -9 };
	//CvMat A = cvMat(3, 3, CV_32F, &a);*/
	//
	//cvSetImageROI(img, cvRect(10, 15, 200, 100));
	//IplImage* img2 = cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	//cvCopy(img,img2,NULL);
	//cvResetImageROI(img);

	//IplImage** arrImg =imFilter(img) ;
	//cvShowImage("filterx",arrImg[0]);
	//cvShowImage("filtery",arrImg[1]);
	
	/*Mat newmat = imread("D:\\Lectures\\Luan_van\\VOC\\VOCdevkit\\VOC2010\\JPEGImages\\2008_003591.jpg",0);
	imshow("ori",newmat);
	equalizeHist(newmat,newmat);
	printf("%d;%d;%d",newmat.rows,newmat.cols,newmat.channels());

	imshow("asdasd",newmat);
	float v[]={-1,0,1};
	Mat Dx (1,3,CV_32F,v);
	Mat dst,dst2;
	filter2D(newmat,dst,newmat.depth(),Dx);
	filter2D(newmat,dst2,newmat.depth(),Dx.t());
	imshow("filter",dst);
	imshow("filter-y",dst2);

	Mat mat2 = newmat(Range(3,10),Range(1,6));
	printf("%d,%d\n",mat2.rows,mat2.cols);*/

	/*for(int i=0;i<100;i++){
		CvScalar s = cvGet2D(arrImg[1],50,100+i);
		printf("%f:%f:%f:%f\n",s.val[0],s.val[1],s.val[2],s.val[3]);
	}*/
	//cvShowImage("ori",img);
	//cvShowImage("aaa",img2);


	
	Mat img = imread("e:\\001.jpg");
	Mat* imFils = imFilter(img);
	Mat img_gray;
	cvtColor(img,img_gray,CV_BGR2GRAY);
	equalizeHist(img_gray,img_gray);
	
	
//	imshow("filter x",imFils[0]);
//	imshow("filter y",imFils[1]);
	
	



	//img.locateROI(Size(20,35),Point(20,30));
	//imshow("roi",img);
//	cvWaitKey();
	double y = 1;
//	printf("AAAAAAA:  %f",atan(1.0)*180/M_PI);
    Console::WriteLine(L"Hello World");
	//printf("%d %d %d",imFils[0].rows,imFils[0].cols, imFils[0].at<uchar>(1,2));
	
	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
	/*printf("\nGRADIENT:\n");
	for(int i=0;i<300;i++)
		for(int j=0;j<200;j++)
			printf("%f: %f \n",G.at<Gradient>(i,j)[0],G.at<Gradient>(i,j)[1]);*/

	HIS* his = calcHisOfCell(G,Rect(50,50,10,10),9);
	/*for (int i=0;i<9;i++)
	{
		printf("%f, ",his->vector_weight[i]);
	}*/
	Mat his_wnd = calcHisOfCellsInWnd(G,Size(32,32),Size(4,4),9);
	printf("\n");

	calcHistOfBlockInWnd(his_wnd,Rect(2,2,3,3));


	//for(int ii =0; ii<32/4;ii++)
	//	for(int jj =0; jj<32/4;jj++)
	//	{

	//	
	//for (int i=0;i<9;i++)
	//{
	//	printf("%2.2f, ",his_wnd.at<HIS*>(ii,jj)->vector_weight[i]);
	//}
	//	printf("\n");
	//	}
	/*Mat aa(2,5,DataType<Gradient>::type);
	for (int i=0;i<aa.rows;i++)
	{
		for (int j=0;j<aa.cols;j++)
		{
			aa.at<Gradient>(i,j) = Gradient(2,3);
		}
	}*/

	/*Mat aa(20,7,DataType<Gradient>::type);
	for (int i=0;i<aa.rows;i++)
	{
		for (int j=0;j<aa.cols;j++)
		{
			aa.at<Gradient>(i,j) = Gradient(2,3);
		}
	}*/
	/*int n =9;
	Vec<int,5> a;
	double * b = new double[10];
	double c[10];
	printf("size %d",sizeof(c));*/
	Mat aa(200,700,DataType<HIS*>::type);
	for (int i=0;i<aa.rows;i++)
	{
		for (int j=0;j<aa.cols;j++)
		{
			aa.at<HIS*>(i,j) = new HIS(70);
		}
	}
	
	
	printf("HIS:  %d %f",aa.at<HIS*>(0,0)->n_bins,aa.at<HIS*>(0,0)->vector_weight[0]);
	
	//G.release();
	
	
    return 0;
}
