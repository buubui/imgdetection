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
	
	Mat img = imread("E:\\3.png");
	imshow("asdasd",img);
	Mat* imFils = imFilter(img);
	/*Mat img_gray;
	cvtColor(img,img_gray,CV_BGR2GRAY);
	equalizeHist(img_gray,img_gray);
	*/
	
	//imshow("filter x",imFils[0]);
	//imshow("filter y",imFils[1]);
	
	Mat G = calcGradientOfPixels(imFils[0],imFils[1]);
	
	printf("\nGRADIENT:\n");
//	for(int i=0;i<30;i++)
//		for(int j=0;j<20;j++)
//			printf("%f: %f \n",G.at<Gradient>(i,j)[0],G.at<Gradient>(i,j)[1]);

	/*HIS* his = calcHisOfCell(G,Rect(50,50,10,10),9);
	for (int i=0;i<9;i++)
	{
		printf("%f, ",his->vector_weight[i]);
	}*/
//	Mat his_wnd = calcHisOfCellsInWnd(G,Rect(0,0,64,128),Size(8,8),9);
	Rect R(0,0,img.cols,img.rows);
	cout <<"RECT:"<< R.width << ";"<<img.cols<<";"<<G.cols;
	Mat his_wnd = calcHisOfCellsInWnd2(G,Rect(0,0,img.cols,img.rows),Size(8,8),9);
	ofstream myfile;
	myfile.open ("hisCell.txt");
	printf("\ncalcHisOfCellsInWnd\n");
	myfile <<"[";
	for(int ii =0; ii<his_wnd.rows;ii++)
	{
		for(int jj =0; jj<his_wnd.cols;jj++)
		{

			
			for (int i=0;i<9;i++)
			{
				printf("%2.2f, ",his_wnd.at<HIS*>(ii,jj)->vector_weight[i]);
				myfile << his_wnd.at<HIS*>(ii,jj)->vector_weight[i] <<" ";
			}
			myfile << "\n ;";
		
		printf("\n");
		}
		

	}		
			
			myfile.close();
			printf("\n %d %d",his_wnd.rows,his_wnd.cols);

//	calcHistOfBlockInWnd(his_wnd,Rect(2,2,3,3));
	/*HIS* h_n = NormalizeBlock(his,2);
	for (int i=0;i<h_n->n_bins;i++)
	{
	printf("%f, ",h_n->vector_weight[i]);
	}*/
	/*HIS* h_w = calcHistOfWnd(his_wnd,Size(3,3),Vec2i(2,2),2);


	printf("\ncalcHistOfWnd\n");
	for (int i=0;i<h_w->n_bins;i++)
	{
		printf("%f ; ",h_w->vector_weight[i]);
	}
	printf("\n n_BIN %d\n",h_w->n_bins);*/

	
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
	//G.release();
	
		cvWaitKey();	
    return 0;
}
