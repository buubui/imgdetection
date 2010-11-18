#include "stdafx.h"
#include "HOG.h"
#include <cxoperations.hpp>
extern Size cellSize,blockSize,wndSize,maxWndSz;
extern cv::Vec2i blockOverlap, regionOverlap;
extern float delPart;
//Gradient** calcHOG(const Mat&filx,const Mat&fily)
//{
//	int r = filx.rows;
//	int c = filx.cols;
//	Gradient** h= new Gradient*[r];
//	for(int i=0;i<r;i++) h[i] = new Gradient[c];
//	for (int i=0;i<r;i++)
//	{
//		for (int j=0;j<c;j++)
//		{
//			/*float x = filx.at<uchar>(i,j)/255.0;
//			float y = fily.at<uchar>(i,j)/255.0;*/
//			float x = filx.at<float>(i,j)/255.0;
//			float y = fily.at<float>(i,j)/255.0;
//			float w =sqrt( x*x+y*y);
//			float a;
//			a  =  atan(y/(x+0.00001))*180/M_PI;
//			/*if(x!=0) a  =  atan(y/x)*180/M_PI;
//			else a = 90;*/
//			/*if(a - floor(a)<=0.001) a=floor(a);
//			if(-a + ceil(a)<=0.001) a=ceil(a);*/
//			h[i][j]= Gradient(a,w);
//
//		}
//	}
//	/*for (int i =0;i<10;i++)
//	{
//		for(int j =0 ;j<10;j++)
//		{
//			printf("%f %f\n",h[i][j].angle,h[i][j].weight);
//		}
//	}*/
//	
//
//	return h;
//
//
//};
Mat calcGradientOfPixels(const Mat&filx,const Mat&fily)
{
	int r = filx.rows;
	int c = filx.cols;
	Mat G(r,c,DataType<Gradient>::type);
	//Gradient** h= new Gradient*[r];
	//for(int i=0;i<r;i++) h[i] = new Gradient[c];
	for (int i=0;i<r;i++)
	{
		for (int j=0;j<c;j++)
		{
			
			float x = filx.at<float>(i,j);
			float y = fily.at<float>(i,j);
			float w =sqrt( x*x+y*y);
			float a;
			a  =  atan(y/(x+0.00001))*180/M_PI;
			a+=a<0?180:0;
			G.at<Gradient>(i,j)[0] =a;
			G.at<Gradient>(i,j)[1] =w;

		}
	}
		return G;


};

Mat calcGradientOfPixelsMaxChannel(const Mat* imfils , int nchannels)
{
	int r = imfils[0].rows;
	int c = imfils[0].cols;
	Mat G(r,c,DataType<Gradient>::type);
	//Gradient** h= new Gradient*[r];
	//for(int i=0;i<r;i++) h[i] = new Gradient[c];
	
	for (int i=0;i<r;i++)
	{
		for (int j=0;j<c;j++)
		{
			int ichannelMax=0;
			float w=-1;
			float x=0;
			float y=0;
			for (int k=0;k<nchannels;k++)
			{
				float x_tmp = imfils[k*2].at<float>(i,j);
				float y_tmp = imfils[k*2+1].at<float>(i,j);
				float w_tmp =sqrt( x_tmp*x_tmp+y_tmp*y_tmp);
				if(w_tmp>w)
				{
					w=w_tmp;
					x=x_tmp;
					y=y_tmp;
					ichannelMax=k;
				}
			}
			
			float a;
			a  =  atan(y/(x+0.00001))*180/M_PI;
			a+=a<0?180:0;
			G.at<Gradient>(i,j)[0] =a;
			G.at<Gradient>(i,j)[1] =w;

		}
	}
	return G;


};

//hog_pixcels: weight& angle of pixels in sub-win
// Rect: toa do cell & size
//n_bins: so luong bin
HIS* calcHisOfCell(Mat hog_pixels, Rect r, int n_bins)
{
	/*Mat A;
	A=(Mat::zeros(1,n_bins,DataType<float>::type));;
	HIS* H= &A;*/
	HIS* H = new Mat();
	*H=Mat::zeros(1,n_bins,DataType<float>::type);
	float w=0;
	float a =180.0 /n_bins;
	for (int i=0;i<r.height;i++)
	{
		for (int j=0;j<r.width;j++)
		{
		//	printf("\n%f:%f\n",hog_pixels.at<Gradient>(i+r.x,j+r.y)[0],hog_pixels.at<Gradient>(i+r.x,j+r.y)[1]);
		//	int n_b = (int)( (hog_pixels.at<Gradient>(i+r.x,j+r.y)[0]+90)/a);
			float angle = hog_pixels.at<Gradient>(i+r.y,j+r.x)[0]; 
			int n_b = (int)( (angle)/a);
			if(H->at<float>(0,n_b)<-100000.)
				H->at<float>(0,n_b)=0.;
			H->at<float>(0,n_b)+=hog_pixels.at<Gradient>(i+r.y,j+r.x)[1];
			
		}
	}
	/*for (int i=0;i<9;i++)
	{
		printf("%f, ",H->vector_weight[i]);
	}*/
	return H;

};
void calcHisOfCellsInWnd(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins,Mat&H)
{
	int c = (int)(wnd.width/cellSize.width);
	int r = (int)(wnd.height/cellSize.height);
	
//	Mat H(r,c,DataType<HIS*>::type);
	H=Mat::zeros(r,c,DataType<HIS*>::type);
	int currX =wnd.x;
	int currY=wnd.y;
	for (int i=0;i<r;i++)
	{
		currX=0;
		for (int j=0;j<c;j++)
			
		{
		//	cout<< j<<"<"<<c<<"\n";
			try
		{
			H.at<HIS*>(i,j)= calcHisOfCell(hog_pixels,Rect(currX,currY,cellSize.width,cellSize.height),n_bins);
		}
		
		catch (...)
		{
			cerr<< "-----Exception:" <<i<<";"<<j;
			exit(0);
		}
			
			currX+=cellSize.width;
		}
		currY+=cellSize.height;
	}
	return;	


};
int bin2dec(int* arrI,int n)
{
	
	int v = 0;
	for (int i=n-1;i>=0;i--)
	{
		v+= arrI[i]*std::pow(2.,n-1-i);
	}
	return v;

}
void calcLBP(Mat& hog_pixels,float th)
{
	int cols = hog_pixels.cols;
	int rows = hog_pixels.rows;
	Mat lbb = Mat::zeros(hog_pixels.size(),DataType<int>::type);
	for (int r=0;r<rows;r++)
	{
		
		for (int c=0;c<cols;c++)
		{
			int b[8];
			for (int t=0;t<8;t++)
			{
				b[t]=0;
			}
			float centerA=hog_pixels.at<Gradient>(r,c)[0];
			float compareA=0;
			if(r-1>0){
				compareA = hog_pixels.at<Gradient>(r-1,c)[0];b[0]=centerA<compareA+th?1:0;
				if(c-1>0)
					compareA = hog_pixels.at<Gradient>(r-1,c-1)[0];b[1]=centerA<compareA+th?1:0;
				if(c+1<cols)
					compareA = hog_pixels.at<Gradient>(r-1,c+1)[0];b[7]=centerA<compareA+th?1:0;
			}
			compareA=0;
			if(c-1>0){
				compareA = hog_pixels.at<Gradient>(r,c-1)[0];b[2]=centerA<compareA+th?1:0;compareA=0;
				if(r+1<rows)
					compareA = hog_pixels.at<Gradient>(r+1,c-1)[0];b[3]=centerA<compareA+th?1:0;
			}
			compareA=0;
			if(r+1<rows){
				compareA = hog_pixels.at<Gradient>(r+1,c)[0];b[4]=centerA<compareA+th?1:0;compareA=0;
				if(c+1<cols)
					compareA = hog_pixels.at<Gradient>(r+1,c+1)[0];b[5]=centerA<compareA+th?1:0;
			}
			compareA=0;
			if(c+1<cols)
				compareA = hog_pixels.at<Gradient>(r,c+1)[0];b[6]=centerA<compareA+th?1:0;
			int d=bin2dec(b,8);
			lbb.at<int>(r,c) = d;

		}
	}
	for (int r=0;r<rows;r++)
	{
		for (int c=0;c<cols;c++)
		{
			hog_pixels.at<Gradient>(r,c)[0] = lbb.at<int>(r,c);
		}
	}
	lbb.release();


}

void calcHisOfCellsInWnd2(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins,Mat& H,float maxD)
{
	int c = (int)(wnd.width/cellSize.width);
	int r = (int)(wnd.height/cellSize.height);
	if(H.rows!=r || H.cols!=c)
//	if(H.rows==0)
	{
	//	H.release();
		H=Mat::zeros(r,c,DataType<HIS*>::type);
	}
	int currX =wnd.x;
	int currY=wnd.y;
//	Mat mask=Mat::zeros(r,c,DataType<bool>::type);
	//for (int i=0;i<r;i++)
	//{
	//	for (int j=0;j<c;j++)

	//	{
	//		if(H.at<HIS*>(i,j)==NULL)
	//			H.at<HIS*>(i,j) = new Mat();
	//		*(H.at<HIS*>(i,j))=Mat::zeros(1,n_bins,DataType<float>::type);
	//		
	//		/*Mat A;
	//		A=(Mat::zeros(1,n_bins,DataType<float>::type));;
	//		H.at<HIS*>(i,j)= &A;*/
	//		
	//	/*	for (int t=0;t<h->cols;t++)
	//		{
	//			h->at<float>(0,t)=0.;
	//		}
	//		printf("ZEROS %f \n",H.at<HIS*>(i,j)->at<float>(0,0));*/
	//	}
	//}
	float a =maxD/n_bins;
	Rect rect(currX,currY,cellSize.width,cellSize.height);
	HIS* H0=NULL,*H1=NULL,*H2=NULL,*H3=NULL;
	HIS* Hs[4];
	float R[4];
	float angle; 
	float weight ; 
	int n_b ;
	Point currPix;
	Vec2i tmp;
	float r0=0,r1=0,r2=0,r3=0;
	Point currCenter1,currCenter2,currCenter3,currCenter;
	for (int i=0;i<r;i++)
	{
		currX=0;
		for (int j=0;j<c;j++)

		{
			if(i==0&&j==0){
			if(H.at<HIS*>(i,j)==NULL){
				H.at<HIS*>(i,j) = new Mat();
			}
//			if(mask.at<bool>(i,j)==false){
				*(H.at<HIS*>(i,j))=Mat::zeros(1,n_bins,DataType<float>::type);
//				mask.at<bool>(i,j)=true;
//			}
		}
			
			if(i==0&&j+1<c){
				if(H.at<HIS*>(i,j+1)==NULL)
					H.at<HIS*>(i,j+1) = new Mat();
//				if(mask.at<bool>(i,j+1)==false){
					*(H.at<HIS*>(i,j+1))=Mat::zeros(1,n_bins,DataType<float>::type);
//					mask.at<bool>(i,j+1)=true;
//				}
			}
			
			if(j==0&&i+1<r){
				if(H.at<HIS*>(i+1,j)==NULL)
					H.at<HIS*>(i+1,j) = new Mat();
//				if(mask.at<bool>(i+1,j)==false){
				*(H.at<HIS*>(i+1,j))=Mat::zeros(1,n_bins,DataType<float>::type);
//				mask.at<bool>(i+1,j)=true;
//				}
			}
			if(i+1<r&&j+1<c){
				if(H.at<HIS*>(i+1,j+1)==NULL)
					H.at<HIS*>(i+1,j+1) = new Mat();
			//	if(mask.at<bool>(i+1,j+1)==false){
					*(H.at<HIS*>(i+1,j+1))=Mat::zeros(1,n_bins,DataType<float>::type);
			//		mask.at<bool>(i+1,j+1)=true;
			//	}
			}
			rect.x=currX;
			rect.y=currY;
			
			//if(H.at<HIS*>(i,j)==NULL){
			//	/*Mat A;
			//	A=(Mat::zeros(1,n_bins,DataType<float>::type));;
			//	H.at<HIS*>(i,j)= &A;*/
			////	H.at<HIS*>(i,j) = new Mat(1,n_bins,DataType<float>::type);
			//	H.at<HIS*>(i,j) = new Mat();
			//	*(H.at<HIS*>(i,j))=Mat::zeros(1,n_bins,DataType<float>::type);
			//}
			H0=H.at<HIS*>(i,j);
			currCenter.x=currX+cellSize.width/2 ; currCenter.y=currY+cellSize.height/2;
			for (int ii=0;ii<rect.height;ii++)
			{
				for (int jj=0;jj<rect.width;jj++)
				{
					//	printf("\n%f:%f\n",hog_pixels.at<Gradient>(i+r.x,j+r.y)[0],hog_pixels.at<Gradient>(i+r.x,j+r.y)[1]);
					//	int n_b = (int)( (hog_pixels.at<Gradient>(i+r.x,j+r.y)[0]+90)/a);
					angle = hog_pixels.at<Gradient>(ii+rect.y,jj+rect.x)[0]; 
					weight = hog_pixels.at<Gradient>(ii+rect.y,jj+rect.x)[1]; 
					n_b = (int)( (angle)/a);
					currPix.x=currX+jj ; currPix.y=currY+ii;
				//	Vec2i tmp;
					/*HIS* H1 = (i+tmp[0])<r-1?(H.at<HIS*>(i+tmp[0],j)):NULL;
					HIS* H2 = (j+tmp[1])<c-1?(H.at<HIS*>(i,j+tmp[1])):NULL;
					HIS* H3 = (i+tmp[0]<r)&&(j+tmp[1]<c)?(H.at<HIS*>(i+tmp[0],j+tmp[1])):NULL;*/
					/*HIS* H1 = NULL;
					HIS* H2 = NULL;
					HIS* H3 = NULL;*/
					if (currPix.x>currCenter.x)
					{
						if (currPix.y>currCenter.y)
						{
							tmp[0]=1; //r
							tmp[1]=1; //c

							H2 = (i+tmp[0])<r-1?(H.at<HIS*>(i+tmp[0],j)):NULL;
							H1 = (j+tmp[1])<c-1?(H.at<HIS*>(i,j+tmp[1])):NULL;
							H3 = (i+tmp[0]<r)&&(j+tmp[1]<c)?(H.at<HIS*>(i+tmp[0],j+tmp[1])):NULL;
							
						}
						else{
							tmp[0]=-1; //r
							tmp[1]=1; //c
							H2 = (i+tmp[0])>0?(H.at<HIS*>(i+tmp[0],j)):NULL;
							H1 = (j+tmp[1])<c-1?(H.at<HIS*>(i,j+tmp[1])):NULL;
							H3 = (i+tmp[0]>0)&&(j+tmp[1]<c-1)?(H.at<HIS*>(i+tmp[0],j+tmp[1])):NULL;

						}
					}
					else{
						if (currPix.y>currCenter.y)
						{
							tmp[0]=+1; //r
							tmp[1]=-1; //c
							H2 = (i+tmp[0])<r-1?(H.at<HIS*>(i+tmp[0],j)):NULL;
							H1 = (j+tmp[1])>0?(H.at<HIS*>(i,j+tmp[1])):NULL;
							H3 = (i+tmp[0]<r-1)&&(j+tmp[1]>0)?(H.at<HIS*>(i+tmp[0],j+tmp[1])):NULL;
								

						}
						else{
							tmp[0]=-1;
							tmp[1]=-1;
							H1 = (i+tmp[0])>0?(H.at<HIS*>(i+tmp[0],j)):NULL;
							H2 = (j+tmp[1])>0?(H.at<HIS*>(i,j+tmp[1])):NULL;
							H3 = (i+tmp[0]>0)&&(j+tmp[1]>0)?(H.at<HIS*>(i+tmp[0],j+tmp[1])):NULL;

						}
					}

					
					r0=r1=r2=r3=0;
					if (H1!=NULL)
					{
						currCenter1.x=currCenter.x +tmp[1]*cellSize.width ; currCenter1.y=currCenter.y;
						int d_01 = currPix.y -currCenter.y;
						if(d_01<0) d_01=-d_01;
						int d_02 = currPix.x -currCenter.x;
						if(d_02<0) d_02=-d_02;
						
						if(H2!=NULL)
						{
							currCenter2.x=currCenter.x ;currCenter2.y=currCenter.y+tmp[0]*cellSize.height ;
							
							currCenter3.x=currCenter.x+tmp[1]*cellSize.width ; currCenter3.y=currCenter.y+tmp[0]*cellSize.height ;
							int d_13 = cellSize.width - d_02;
							if(d_13<0) d_13=-d_13;
							int d_23 = cellSize.height - d_01;
							if(d_23<0) d_23=-d_23;
							
							r0= (1. - 1.*d_01/cellSize.height) * (1. - 1.*d_02/cellSize.width);
							r1 = (1. - 1.*d_01/cellSize.height) * (1. - 1.*d_13/cellSize.width);
							r2 = (1. - 1.*d_23/cellSize.height) * (1. - 1.*d_02/cellSize.width);
							r3 = (1. - 1.*d_23/cellSize.height) * (1. - 1.*d_13/cellSize.width);

							/*H0->vector_weight[n_b]+=weight*r0;
							H1->vector_weight[n_b]+=weight*r1;
							H2->vector_weight[n_b]+=weight*r2;
							H3->vector_weight[n_b]+=weight*r3;*/

							/*setHisOfCell(Gradient(angle,weight*r0),H0,cellSize);
							setHisOfCell(Gradient(angle,weight*r1),H1,cellSize);
							setHisOfCell(Gradient(angle,weight*r2),H2,cellSize);
							setHisOfCell(Gradient(angle,weight*r3),H3,cellSize);*/

							Hs[0]=H0;Hs[1]=H1;Hs[2]=H2;Hs[3]=H3;
							R[0]=r0;R[1]=r1;R[2]=r2;R[3]=r3;
							setHisOfCells(Gradient(angle,weight),Hs,R,4,cellSize,maxD);
							
						}
						else{
							r0 =  (1. - 1.*d_02/cellSize.width);
							r1 =  (1.*d_02/cellSize.width);
							//r2 = (1. - 1.*d_23/cellSize.height) * (1. - 1.*d_02/cellSize.width);
							//r3 = (1. - 1.*d_23/cellSize.height) * (1. - 1.*d_13/cellSize.width);

						//	H0->vector_weight[n_b]+=weight*r0;
						//	H1->vector_weight[n_b]+=weight*r1;
							//H2->vector_weight[n_b]+=weight*r2;
							//H3->vector_weight[n_b]+=weight*r3;

							/*setHisOfCell(Gradient(angle,weight*r0),H0,cellSize);
							setHisOfCell(Gradient(angle,weight*r1),H1,cellSize);*/

							Hs[0]=H0;Hs[1]=H1;
							R[0]=r0;R[1]=r1;
							setHisOfCells(Gradient(angle,weight),Hs,R,2,cellSize,maxD);
						}
					}
					else if(H2!=NULL){
						int d_01 = currPix.y -currCenter.y;
						if(d_01<0) d_01=-d_01;
						int d_02 = currPix.x -currCenter.x;
						if(d_02<0) d_02=-d_02;
						
						Point currCenter2(currCenter.x ,currCenter.y+tmp[1]*cellSize.height );

						r0= (1. - 1.*d_01/cellSize.height);// * (1. - 1.*d_02/cellSize.width);
						//r1 = (1. - 1.*d_01/cellSize.height) * (1. - 1.*d_13/cellSize.width);
						r2 = (1.*d_01/cellSize.height); //* (1. - 1.*d_02/cellSize.width);
						//r3 = (1. - 1.*d_23/cellSize.height) * (1. - 1.*d_13/cellSize.width);

						//H0->vector_weight[n_b]+=weight*r0;
						//H1->vector_weight[n_b]+=weight*r1;
						//H2->vector_weight[n_b]+=weight*r2;
						//H3->vector_weight[n_b]+=weight*r3;

						/*setHisOfCell(Gradient(angle,weight*r0),H0,cellSize);
						setHisOfCell(Gradient(angle,weight*r2),H2,cellSize);*/

						Hs[0]=H0;Hs[1]=H2;
						R[0]=r0;R[1]=r2;
						setHisOfCells(Gradient(angle,weight),Hs,R,2,cellSize,maxD);
					}
					else{
						//H0->vector_weight[n_b]+=weight;
					//	setHisOfCell(Gradient(angle,weight),H0,cellSize);
						Hs[0]=H0;
						R[0]=1.;
						setHisOfCells(Gradient(angle,weight),Hs,R,1,cellSize,maxD);
					}
					
					//H0->vector_weight[n_b]+=hog_pixels.at<Gradient>(ii+r.y,jj+r.x)[1];

				}
			}
			//H.at<HIS*>(i,j)= calcHisOfCell(hog_pixels,Rect(currX,currY,cellSize.width,cellSize.height),n_bins);
			

			currX+=cellSize.width;
		}
		currY+=cellSize.height;
	}
//	mask.release();
	return ;	


};
HIS calcHistOfWndNew(const Mat& hog_pixels,Size cellSize, int n_bins,Mat*&h_w)
{
	Rect delRect = Rect(hog_pixels.cols/2  ,hog_pixels.rows/2,hog_pixels.cols*delPart,hog_pixels.rows*delPart);
	delRect.x -= delRect.width/2;
	delRect.y -= delRect.height/2;
//	printf("%d %d\n",hog_pixels.cols,hog_pixels.rows);
//	printf("%d %d %d %d\n",delRect.x,delRect.y,delRect.width,delRect.height);
//	Rect r=Rect(0,0,(hog_pixels.cols-delRect.width)/2,(hog_pixels.rows-delRect.height)/2+delRect.height);
//	printf("%d %d %d %d\n",r.x,r.y,r.width,r.height);
	Mat *hog_mats=new Mat[4];
//	Mat m=hog_pixels(Rect(0,0,203,222));
	hog_mats[0]=hog_pixels(Rect(0,0,(hog_pixels.cols-delRect.width)/2,(hog_pixels.rows-delRect.height)/2+delRect.height));
	hog_mats[1]=hog_pixels(Rect(hog_mats[0].cols,0,hog_pixels.cols-hog_mats[0].cols,(hog_pixels.rows-delRect.height)/2));
	hog_mats[2]=hog_pixels(Rect(hog_mats[0].cols+delRect.width,hog_mats[1].rows,hog_mats[0].cols,hog_pixels.rows-hog_mats[1].rows));
	hog_mats[3]=hog_pixels(Rect(0,hog_mats[0].rows, hog_pixels.cols-hog_mats[2].cols, hog_mats[1].rows));

	hog_mats[0]=rotateMat(rotateMat(hog_mats[0],1),1);
	hog_mats[1]=rotateMat(hog_mats[1],1);
	hog_mats[3]=rotateMat(hog_mats[3],0);
	/*imshow("ori",hog_pixels);
	imshow("0",Hs[0]);
	imshow("1",Hs[1]);
	imshow("2",Hs[2]);
	imshow("3",Hs[3]);
	imshow("del",hog_pixels(delRect));*/
	HIS* his_wins= new HIS[4];
	if(h_w==NULL)
		h_w=new Mat[4];
	int n_size=0;
	for (int i=0;i<4;i++)
//	while(true)
	{
		calcHisOfCellsInWnd2(hog_mats[i],Rect(Point(0,0),hog_mats[i].size()),cellSize,n_bins,h_w[i],180.);
		calcHistOfWnd(h_w[i],blockSize,blockOverlap,2,his_wins[i]);
		n_size += his_wins[i].cols;
	//	h_w.release();
	}
	
	HIS finalHis = appendHis(his_wins,4,n_size);
	for (int i=0;i<4;i++)
	{
	//	printf(" vung %d(%d %d): %d\n",i,Hs[i].rows,Hs[i].cols,His[i].cols);
		hog_mats[i].release();
		his_wins[i].release();
		/*H0.release();
		H1.release();
		H2.release();
		H3.release();*/
		
	}
//	h_w.release();
	delete[] hog_mats;
	delete[] his_wins;
	return finalHis;
};
HIS appendHis(HIS* Hs,int n, int n_size)
{
//	int n_size = 0;
//	for (int i=0;i<n;i++)
//	{
//		n_size+= Hs[i].cols;
//	}
//	cout<<"n_size="<<n_size<<endl;
	HIS A = Mat::zeros(1,n_size,Hs[0].type());
	int currIndex=0;
	for (int i=0;i<n;i++)
	{
		Hs[i].copyTo(A(Rect(currIndex,0,Hs[i].cols,1)));
		currIndex+=Hs[i].cols;
	}
	return A;
}
void setHisOfCell(const Gradient& hog_pixcell, HIS* Hcell,Size cellSize)
{
	float a = hog_pixcell[0];
	float w = hog_pixcell[1];
	float step = 180./Hcell->cols;
	int n_b = (int)( (a)/step);
		if(a<n_b*(step+0.5))
		{
			if(n_b>0)
			{
				float r1=  (n_b*(step+0.5)-a)/step ;
				float r0 = 1-r1;
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
				Hcell->at<float>(0,n_b)+=r0*w;

			//	if(Hcell->at<float>(0,n_b-1)<-100000.)
			//		Hcell->at<float>(0,n_b-1)=0.;
				Hcell->at<float>(0,n_b-1)+=r1*w;
			}else{
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
				Hcell->at<float>(0,n_b)+=w;
			}
		}else{
			if(n_b < Hcell->cols-1)
			{
				float r1= (a- n_b*(step+0.5))/step ;
				float r0 = 1-r1;
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
				Hcell->at<float>(0,n_b)+=r0*w;

			//	if(Hcell->at<float>(0,n_b+1)<-100000.)
			//		Hcell->at<float>(0,n_b+1)=0.;
				Hcell->at<float>(0,n_b+1)+=r1*w;
			}else{
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
				Hcell->at<float>(0,n_b)+=w;
			}
		}
	//Hcell->vector_weight[n_b]+= 

};

inline
void setHisOfCells(const Gradient& hog_pixcell, HIS** Hcells,float* R,int n_cells,Size cellSize,float maxD)
{
//	cout<<Hcells[0]->cols<<endl;
	
	float a = hog_pixcell[0];
	float w = hog_pixcell[1];
//	cout<<a<<endl;
	float step = maxD/Hcells[0]->cols;
	int n_b = (int)( (a)/step);
	for(int i=0;i<n_cells;i++)
	if(a<n_b*(step+0.5))
	{
		if(n_b>0)
		{
			float r1=  (n_b*(step+0.5)-a)/step ;
			float r0 = 1-r1;
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
			Hcells[i]->at<float>(0,n_b)+=r0*w*R[i];

			//	if(Hcell->at<float>(0,n_b-1)<-100000.)
			//		Hcell->at<float>(0,n_b-1)=0.;
			Hcells[i]->at<float>(0,n_b-1)+=r1*w*R[i];
		}else{
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
			Hcells[i]->at<float>(0,n_b)+=w*R[i];
		}
	}else{
		if(n_b < Hcells[0]->cols-1)
		{
			float r1= (a- n_b*(step+0.5))/step ;
			float r0 = 1-r1;
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
			Hcells[i]->at<float>(0,n_b)+=r0*w*R[i];

			//	if(Hcell->at<float>(0,n_b+1)<-100000.)
			//		Hcell->at<float>(0,n_b+1)=0.;
			Hcells[i]->at<float>(0,n_b+1)+=r1*w*R[i];
		}else{
			//	if(Hcell->at<float>(0,n_b)<-100000.)
			//		Hcell->at<float>(0,n_b)=0.;
			Hcells[i]->at<float>(0,n_b)+=w*R[i];
		}
	}
	//Hcell->vector_weight[n_b]+= 

};


void NormalizeBlock(Mat& m, int c)
{
	Mat mat;
//	mat.zeros(m.size(),m.type());
	float n =0.;
	switch(c)
	{
	case 0: // L2 norm
		n =norm(m,NORM_L2);	mat = m/sqrt( n*n + M_e*M_e); break;
	case 1: // L1 Norm
		mat = m/(norm(m,NORM_L1) + M_e); break;
	case 2: //L1-sqrt
		sqrt( m/(norm(m,NORM_L1) + M_e),mat); break;
	case 3://L2-HYS
		float maxval=0.2;
		float epsilon2=0.01;
		for (int i= 0; i< m.cols; ++i) 
			if (m.at<float>(0,i) > maxval)
				m.at<float>(0,i) = maxval;
		Mat tmp;
		cv::pow(m,2,tmp);
		float norm = std::sqrt(cv::sum(tmp)[0]) + epsilon2;
	//	cout<<norm<<endl;
	//	Mat tmp=m*m.t();
	//	float norm = std::sqrt(tmp.at<float>(0,0)) + epsilon2;
	//	tmp.release();
	//	m /= norm;
		mat=m/norm;
	//	n =norm(m,NORM_L2);
	//	mat = m/sqrt( n*n + M_e*M_e);
		break;
	
	}
	m.release();
	m=mat;	
//	return mat;
}

//void NormalizeBlock(HIS* h, int c)
//{
//
////	Mat m(1,h->n_bins,DataType<float>::typeC1);
//	//for (int i = 0 ; i<h->n_bins;i++)
//	//{
//	//	m.at<float>(0,i) = h->vector_weight[i];
//	//}
////	HIS* r = new HIS(h->n_bins);
//	Mat mat = NormalizeBlock(*h,c);
////	h->release();
//	
//	h=&mat;
//	//for (int i = 0 ; i<h->n_bins;i++)
//	//{
//	//	h->vector_weight[i] = mat.at<float>(0,i) ;
//	//}
//	//mat.release();
//	//m.release();
//	return ;
//}

Mat im2float(const Mat& m)
{
	int r = m.rows;
	int c = m.cols;
	Mat mat(r,c,DataType<float>::type);

	for (int i = 0 ; i<r ; i++)
		for (int j = 0 ; j< c ; j++)
		{
			mat.at<float>(i,j) = m.at<uchar>(i,j) / 255.0;
		}
	
	return mat;
	


}
void calcHistOfBlockInWnd(const Mat& mat, Rect p,HIS& hist)
{
	int n = mat.at<HIS*>(0,0)->cols;
	int w = p.width;
	int h = p.height;
	int x = p.x;
	int y = p.y;
	/*Mat A;
	A=(Mat::zeros(1,w*h*n,DataType<float>::type));;
	HIS* hist = &A;*/
//	if(hist.rows==0)
//		hist = new Mat();
	if(hist.rows==0)
		hist = Mat::zeros(1,w*h*n,DataType<float>::type);
	int s =0;
	for (int i=0;i<h;i++)
	{
		for (int j = 0 ; j< w;j++)
		{
		//	for (int e =0; e<n;e++)
		//	{
		//		hist->at<float>(0,s) = mat.at<HIS*>(x+i,j+y)->at<float>(0,e);
			//	HIS* h=mat.at<HIS*>(x+i,j+y);
				HIS* h=mat.at<HIS*>(y+i,j+x);
				h->copyTo((hist)(Rect(s,0,h->cols,1)));
				s+=h->cols;
		//	}
			/*for (int e =0; e<n;e++)
			{
				hist->at<float>(0,s) = mat.at<HIS*>(x+i,j+y)->at<float>(0,e);
				s++;
			}*/
			
			
		}
	}
//	printf("\n BLOCK: %d\n",s);
	/*for (int i =0 ; i<n*w*h;i++)
	{
		printf("%f \n ",hist->vector_weight[i]);
	}*/

	return ;
}
Mat GaussianBlurBlock(const Mat& mat,Vec2i overlap)
{
	Mat retM=mat.clone();
	/*Mat w(mat.size(),DataType<float>::type);
	Mat a(mat.size(),DataType<float>::type);
	for (int r=0;r<mat.rows;r++)
	{
		for (int c=0;c<mat.cols;c++)
		{
			a.at<float>(r,c)=mat.at<Gradient>(r,c)[0];
			w.at<float>(r,c)=mat.at<Gradient>(r,c)[1];
		}
	}*/
	double sigma = 0.5 * cellSize.height*blockSize.height;
	
	int n_block_w = floor( 1.*(mat.cols/cellSize.width - overlap[0])/(blockSize.width - overlap[0]));
	int n_block_h = floor( 1.*(mat.rows/cellSize.height - overlap[1])/(blockSize.height - overlap[1]));
	
	int x=0,y=0;
//	int s=0;
	for (int i=0; i < n_block_h;i++)
	{
		x=0;
		for (int j = 0 ; j<n_block_w;j++)
		{
			Rect blk(x,y,blockSize.width*cellSize.width,blockSize.height*cellSize.height);
			GaussianBlur(mat(blk),retM(blk),Size(3,3),sigma);
			/*GaussianBlur(a(blk),a(blk),Size(3,3),0.5*cellSize.width*blockSize.width);
			GaussianBlur(w(blk),w(blk),Size(3,3),0.5*cellSize.width*blockSize.width);
			for (int ii=0;ii<blk.height;ii++)
			{
				for (int jj=0;jj<blk.width;jj++)
				{
					retM(blk).at<Gradient>(ii,jj)=Gradient(a(blk).at<float>(ii,jj),w(blk).at<float>(ii,jj));
				}
			}*/
		//	calcHistOfBlockInWnd(mat,Rect(x,y,blockSize.width,blockSize.height),h_b);
	//		Mat temp= h_b.clone();
			
	//		h_b.copyTo((H)(Rect(s,0,h_b.cols,1)));
	//		s+=h_b.cols;
			
			x+= (blockSize.width -overlap[0])*cellSize.width;			
		}
		y+=(blockSize.height - overlap[1])*cellSize.height;

	}
	return retM;


//	GaussianBlur(h,h,Size(3,3),sigma);
}
void calcHistOfWnd(const Mat& mat, const Size& blockSize, Vec2i overlap, int norm_c,HIS& H)
{ 
	int n = mat.at<HIS*>(0,0)->cols;
	int n_block_w = floor( 1.*(mat.cols - overlap[0])/(blockSize.width - overlap[0]));
	int n_block_h = floor( 1.*(mat.rows - overlap[1])/(blockSize.height - overlap[1]));
	int n_block = n_block_h*n_block_w;
	/*Mat A;
	A=(Mat::zeros(1,n_block*blockSize.width*blockSize.height*n,DataType<float>::type));;
	HIS* H= &A;*/
//	HIS* H = new Mat();
	if(H.rows==0)
		H=Mat::zeros(1,n_block*blockSize.width*blockSize.height*n,DataType<float>::type);

	int x=0,y=0;
	int s=0;
	HIS h_b;
	/*ofstream outHisBlock;
	stringstream name;
	name <<"output/hisBlock_Gauss.txt";
	outHisBlock.open(name.str().c_str());*/
	for (int i=0; i < n_block_h;i++)
	{
		x=0;
		for (int j = 0 ; j<n_block_w;j++)
		{
			calcHistOfBlockInWnd(mat,Rect(x,y,blockSize.width,blockSize.height),h_b);
			Mat temp= h_b.clone();
//			GaussianBlur(temp,h_b,Size(0,0),0.5*cellSize.height*blockSize.height);
			//GaussianBlurBlock(h_b);
			//
			/*Mat temp;
			temp = h_b.clone();
			float sigma = 0.5*6*3;
			GaussianBlur(h_b,temp,Size(3,3),sigma);
			int r = temp.rows;
			int c = temp.cols;
			for(int rr =0; rr<r;rr++)
				for(int cc=0;cc<c;cc++)
				{
					float val = temp.at<float>(rr,cc);
					outHisBlock<<val<<"\t";
				}
			outHisBlock<<"\n";*/
			//
			NormalizeBlock(h_b,norm_c);


		//	for (int e=0;e<n*blockSize.width*blockSize.height;e++)
		//	{
		//		H->at<float>(0,s)=h_b->at<float>(0,e);
				h_b.copyTo((H)(Rect(s,0,h_b.cols,1)));
				s+=h_b.cols;
		//	}
			x+= blockSize.width -overlap[0];
		//	h_b->release();
			



			/*HIS* h_b = calcHistOfBlockInWnd(mat,Rect(x,y,blockSize.width,blockSize.height));
			NormalizeBlock(*h_b,norm_c);
			for (int e=0;e<n*blockSize.width*blockSize.height;e++)
			{
				H->at<float>(0,s)=h_b->at<float>(0,e);
				s++;
			}
			x+= blockSize.width -overlap[0];
			delete h_b;*/
		}
		y+=blockSize.height - overlap[1];
			
	}
	 h_b.release();
//	 outHisBlock.close();
	return ;


}