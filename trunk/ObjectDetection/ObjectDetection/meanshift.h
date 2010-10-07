#pragma once
int			meanVec( float *x, float *data, int p, int n, float radius2,
					float *mean );
float		dist( float *A, float *B, int n );
void		meanShift( float data[], int p, int n, float radius, float rate,
					  int maxIter, bool blur, float labels[], float* means ) ;
void	meanShift2( float data[], int p, int n, float radius, float* &means,int& n_mean ) ;
bool meanshiftFromFile(string fname,float minValue,float radius,int minCsize,float* &means,int& n_mean ,int& p_mean);
float round(float d);
struct M_Point{
	int x;
	int y;
	float scale[100];
	int n;
	M_Point(){
		x=0;y=0;n=0;
	}
	M_Point(int _x,int _y,float _s){
		x=_x;
		y=_y;
		scale[0]=_s;
		n=1;
	}
	void add(float _scale){
		scale[n++]=_scale;
		
	}
};
//M_Point* getPoint(int i, ArrayList arr);
//void addPoint(M_Point* p,ArrayList arr);
Mat powDiagMat(Mat m ,float p);
float tw(float w, float c);
float wi(int i,Mat y,Mat* data,int n,Mat* Hs,float c);
Mat HhInv(Mat y, Mat* data,Mat*Hs,int n, float c );
void	newMeanShift( Mat* data, int p, int n,int sigma_x,int sigma_y,float sigma_s,
					 int maxIter ,float c) ;
void	newMeanShift2( Mat* data,int  p, int n, Mat*& means, int& n_mean,float c ) ;
bool newMeanshiftFromFile(string fname,float c ,int minCsize,Mat* &means,int& n_mean ,int& p_mean);
float mahalanobis(Mat y, Mat yi,Mat H);
float wi_tu(int i,Mat y,Mat* data,int n,Mat* Hs,float c);