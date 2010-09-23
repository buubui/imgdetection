#include "stdafx.h"
#include "HOG.h"
#include <cxoperations.hpp>

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
//			/*double x = filx.at<uchar>(i,j)/255.0;
//			double y = fily.at<uchar>(i,j)/255.0;*/
//			double x = filx.at<double>(i,j)/255.0;
//			double y = fily.at<double>(i,j)/255.0;
//			double w =sqrt( x*x+y*y);
//			double a;
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
			
			double x = filx.at<double>(i,j);
			double y = fily.at<double>(i,j);
			double w =sqrt( x*x+y*y);
			double a;
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
	A=(Mat::zeros(1,n_bins,CV_64F));;
	HIS* H= &A;*/
	HIS* H = new Mat();
	*H=Mat::zeros(1,n_bins,CV_64F);
	double w=0;
	double a =180.0 /n_bins;
	for (int i=0;i<r.height;i++)
	{
		for (int j=0;j<r.width;j++)
		{
		//	printf("\n%f:%f\n",hog_pixels.at<Gradient>(i+r.x,j+r.y)[0],hog_pixels.at<Gradient>(i+r.x,j+r.y)[1]);
		//	int n_b = (int)( (hog_pixels.at<Gradient>(i+r.x,j+r.y)[0]+90)/a);
			float angle = hog_pixels.at<Gradient>(i+r.y,j+r.x)[0]; 
			int n_b = (int)( (angle)/a);
			if(H->at<double>(0,n_b)<-100000.)
				H->at<double>(0,n_b)=0.;
			H->at<double>(0,n_b)+=hog_pixels.at<Gradient>(i+r.y,j+r.x)[1];
			
		}
	}
	/*for (int i=0;i<9;i++)
	{
		printf("%f, ",H->vector_weight[i]);
	}*/
	return H;

};
Mat calcHisOfCellsInWnd(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins)
{
	int c = (int)(wnd.width/cellSize.width);
	int r = (int)(wnd.height/cellSize.height);
	
	Mat H(r,c,DataType<HIS*>::type);
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
	return H;	


};



Mat calcHisOfCellsInWnd2(Mat hog_pixels,Rect wnd, Size cellSize, int n_bins)
{
	int c = (int)(wnd.width/cellSize.width);
	int r = (int)(wnd.height/cellSize.height);

	Mat H(r,c,DataType<HIS*>::type);
	int currX =wnd.x;
	int currY=wnd.y;

	for (int i=0;i<r;i++)
	{
		for (int j=0;j<c;j++)

		{
			H.at<HIS*>(i,j) = new Mat();
			*(H.at<HIS*>(i,j))=Mat::zeros(1,n_bins,CV_64F);
			/*Mat A;
			A=(Mat::zeros(1,n_bins,CV_64F));;
			H.at<HIS*>(i,j)= &A;*/
			
		/*	for (int t=0;t<h->cols;t++)
			{
				h->at<double>(0,t)=0.;
			}
			printf("ZEROS %f \n",H.at<HIS*>(i,j)->at<double>(0,0));*/
		}
	}

	for (int i=0;i<r;i++)
	{
		currX=0;
		for (int j=0;j<c;j++)

		{
			Rect rect(currX,currY,cellSize.width,cellSize.height);
			double a =180./n_bins;
			if(H.at<HIS*>(i,j)==NULL){
				/*Mat A;
				A=(Mat::zeros(1,n_bins,CV_64F));;
				H.at<HIS*>(i,j)= &A;*/
			//	H.at<HIS*>(i,j) = new Mat(1,n_bins,CV_64F);
				H.at<HIS*>(i,j) = new Mat();
				*(H.at<HIS*>(i,j))=Mat::zeros(1,n_bins,CV_64F);
			}
			HIS* H0=H.at<HIS*>(i,j);
			Point currCenter(currX+cellSize.width/2,currY+cellSize.height/2);
			for (int ii=0;ii<rect.height;ii++)
			{
				for (int jj=0;jj<rect.width;jj++)
				{
					//	printf("\n%f:%f\n",hog_pixels.at<Gradient>(i+r.x,j+r.y)[0],hog_pixels.at<Gradient>(i+r.x,j+r.y)[1]);
					//	int n_b = (int)( (hog_pixels.at<Gradient>(i+r.x,j+r.y)[0]+90)/a);
					float angle = hog_pixels.at<Gradient>(ii+rect.y,jj+rect.x)[0]; 
					float weight = hog_pixels.at<Gradient>(ii+rect.y,jj+rect.x)[1]; 
					int n_b = (int)( (angle)/a);
					Point currPix(currX+jj,currY+ii);
					Vec2i tmp;
					/*HIS* H1 = (i+tmp[0])<r-1?(H.at<HIS*>(i+tmp[0],j)):NULL;
					HIS* H2 = (j+tmp[1])<c-1?(H.at<HIS*>(i,j+tmp[1])):NULL;
					HIS* H3 = (i+tmp[0]<r)&&(j+tmp[1]<c)?(H.at<HIS*>(i+tmp[0],j+tmp[1])):NULL;*/
					HIS* H1 = NULL;
					HIS* H2 = NULL;
					HIS* H3 = NULL;
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

					
					double r0=0,r1=0,r2=0,r3=0;
					if (H1!=NULL)
					{
						Point currCenter1(currCenter.x +tmp[1]*cellSize.width ,currCenter.y);
						int d_01 = currPix.y -currCenter.y;
						if(d_01<0) d_01=-d_01;
						int d_02 = currPix.x -currCenter.x;
						if(d_02<0) d_02=-d_02;
						
						if(H2!=NULL)
						{
							Point currCenter2(currCenter.x ,currCenter.y+tmp[0]*cellSize.height );
							
							Point currCenter3(currCenter.x+tmp[1]*cellSize.width ,currCenter.y+tmp[0]*cellSize.height );
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
							setHisOfCell(Gradient(angle,weight*r0),H0,cellSize);
							setHisOfCell(Gradient(angle,weight*r1),H1,cellSize);
							setHisOfCell(Gradient(angle,weight*r2),H2,cellSize);
							setHisOfCell(Gradient(angle,weight*r3),H3,cellSize);
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
							setHisOfCell(Gradient(angle,weight*r0),H0,cellSize);
							setHisOfCell(Gradient(angle,weight*r1),H1,cellSize);
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

						setHisOfCell(Gradient(angle,weight*r0),H0,cellSize);
						setHisOfCell(Gradient(angle,weight*r2),H2,cellSize);
					}
					else{
						//H0->vector_weight[n_b]+=weight;
						setHisOfCell(Gradient(angle,weight),H0,cellSize);
					}
					
					//H0->vector_weight[n_b]+=hog_pixels.at<Gradient>(ii+r.y,jj+r.x)[1];

				}
			}
			//H.at<HIS*>(i,j)= calcHisOfCell(hog_pixels,Rect(currX,currY,cellSize.width,cellSize.height),n_bins);
			

			currX+=cellSize.width;
		}
		currY+=cellSize.height;
	}
	return H;	


};
void setHisOfCell(Gradient hog_pixcell, HIS* Hcell,Size cellSize)
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
			//	if(Hcell->at<double>(0,n_b)<-100000.)
			//		Hcell->at<double>(0,n_b)=0.;
				Hcell->at<double>(0,n_b)+=r0*w;

			//	if(Hcell->at<double>(0,n_b-1)<-100000.)
			//		Hcell->at<double>(0,n_b-1)=0.;
				Hcell->at<double>(0,n_b-1)+=r1*w;
			}else{
			//	if(Hcell->at<double>(0,n_b)<-100000.)
			//		Hcell->at<double>(0,n_b)=0.;
				Hcell->at<double>(0,n_b)+=w;
			}
		}else{
			if(n_b < Hcell->cols-1)
			{
				float r1= (a- n_b*(step+0.5))/step ;
				float r0 = 1-r1;
			//	if(Hcell->at<double>(0,n_b)<-100000.)
			//		Hcell->at<double>(0,n_b)=0.;
				Hcell->at<double>(0,n_b)+=r0*w;

			//	if(Hcell->at<double>(0,n_b+1)<-100000.)
			//		Hcell->at<double>(0,n_b+1)=0.;
				Hcell->at<double>(0,n_b+1)+=r1*w;
			}else{
			//	if(Hcell->at<double>(0,n_b)<-100000.)
			//		Hcell->at<double>(0,n_b)=0.;
				Hcell->at<double>(0,n_b)+=w;
			}
		}
	//Hcell->vector_weight[n_b]+= 

};

void NormalizeBlock(Mat& m, int c)
{
	Mat mat;
//	mat.zeros(m.size(),m.type());
	double n =0.;
	switch(c)
	{
	case 0: // L2 norm
		n =norm(m,NORM_L2);	mat = m/sqrt( n*n + M_e); break;
	case 1: // L1 Norm
		mat = m/(norm(m,NORM_L1) + M_e); break;
	case 2: //L1-sqrt
		sqrt( m/(norm(m,NORM_L1) + M_e),mat); break;
	}
	m.release();
	m=mat;	
//	return mat;
}

//void NormalizeBlock(HIS* h, int c)
//{
//
////	Mat m(1,h->n_bins,CV_64FC1);
//	//for (int i = 0 ; i<h->n_bins;i++)
//	//{
//	//	m.at<double>(0,i) = h->vector_weight[i];
//	//}
////	HIS* r = new HIS(h->n_bins);
//	Mat mat = NormalizeBlock(*h,c);
////	h->release();
//	
//	h=&mat;
//	//for (int i = 0 ; i<h->n_bins;i++)
//	//{
//	//	h->vector_weight[i] = mat.at<double>(0,i) ;
//	//}
//	//mat.release();
//	//m.release();
//	return ;
//}

Mat im2double(const Mat& m)
{
	int r = m.rows;
	int c = m.cols;
	Mat mat(r,c,CV_64FC1);

	for (int i = 0 ; i<r ; i++)
		for (int j = 0 ; j< c ; j++)
		{
			mat.at<double>(i,j) = m.at<uchar>(i,j) / 255.0;
		}
	
	return mat;
	


}
HIS* calcHistOfBlockInWnd(const Mat& mat, Rect p)
{
	int n = mat.at<HIS*>(0,0)->cols;
	int w = p.width;
	int h = p.height;
	int x = p.x;
	int y = p.y;
	/*Mat A;
	A=(Mat::zeros(1,w*h*n,CV_64F));;
	HIS* hist = &A;*/
	HIS* hist = new Mat();
	*hist = Mat::zeros(1,w*h*n,CV_64F);
	int s =0;
	for (int i=0;i<h;i++)
	{
		for (int j = 0 ; j< w;j++)
		{
		//	for (int e =0; e<n;e++)
		//	{
		//		hist->at<double>(0,s) = mat.at<HIS*>(x+i,j+y)->at<double>(0,e);
				HIS* h=mat.at<HIS*>(x+i,j+y);
				h->copyTo((*hist)(Rect(s,0,h->cols,1)));
				s+=h->cols;
		//	}
			/*for (int e =0; e<n;e++)
			{
				hist->at<double>(0,s) = mat.at<HIS*>(x+i,j+y)->at<double>(0,e);
				s++;
			}*/
			
			
		}
	}
//	printf("\n BLOCK: %d\n",s);
	/*for (int i =0 ; i<n*w*h;i++)
	{
		printf("%f \n ",hist->vector_weight[i]);
	}*/

	return hist;
}

HIS* calcHistOfWnd(const Mat& mat, const Size& blockSize, Vec2i overlap, int norm_c)
{
	int n = mat.at<HIS*>(0,0)->cols;
	int n_block_w = floor( 1.*(mat.cols - overlap[0])/(blockSize.width - overlap[0]));
	int n_block_h = floor( 1.*(mat.rows - overlap[1])/(blockSize.height - overlap[1]));
	int n_block = n_block_h*n_block_w;
	/*Mat A;
	A=(Mat::zeros(1,n_block*blockSize.width*blockSize.height*n,CV_64F));;
	HIS* H= &A;*/
	HIS* H = new Mat();
	*H=Mat::zeros(1,n_block*blockSize.width*blockSize.height*n,CV_64F);

	int x=0,y=0;
	int s=0;
	for (int i=0; i < n_block_w;i++)
	{
		x=0;
		for (int j = 0 ; j<n_block_h;j++)
		{
			HIS* h_b = calcHistOfBlockInWnd(mat,Rect(x,y,blockSize.width,blockSize.height));
			NormalizeBlock(*h_b,norm_c);


		//	for (int e=0;e<n*blockSize.width*blockSize.height;e++)
		//	{
		//		H->at<double>(0,s)=h_b->at<double>(0,e);
				h_b->copyTo((*H)(Rect(s,0,h_b->cols,1)));
				s+=h_b->cols;
		//	}
			x+= blockSize.width -overlap[0];
		//	h_b->release();
			delete h_b;



			/*HIS* h_b = calcHistOfBlockInWnd(mat,Rect(x,y,blockSize.width,blockSize.height));
			NormalizeBlock(*h_b,norm_c);
			for (int e=0;e<n*blockSize.width*blockSize.height;e++)
			{
				H->at<double>(0,s)=h_b->at<double>(0,e);
				s++;
			}
			x+= blockSize.width -overlap[0];
			delete h_b;*/
		}
		y+=blockSize.height - overlap[1];
		
		
	}
	
	return H;


}