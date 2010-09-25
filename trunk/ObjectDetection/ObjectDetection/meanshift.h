#pragma once
int			meanVec( double *x, double *data, int p, int n, double radius2,
					double *mean );
double		dist( double *A, double *B, int n );
void		meanShift( double data[], int p, int n, double radius, double rate,
					  int maxIter, bool blur, double labels[], double* means ) ;
void	meanShift2( double data[], int p, int n, double radius, double* &means,int& n_mean ) ;
bool meanshiftFromFile(string fname,double minValue,double radius,int minCsize,double* &means,int& n_mean ,int& p_mean);
double round(double d);
struct M_Point{
	int x;
	int y;
	double scale[100];
	int n;
	M_Point(){
		x=0;y=0;n=0;
	}
	M_Point(int _x,int _y,double _s){
		x=_x;
		y=_y;
		scale[0]=_s;
		n=1;
	}
	void add(double _scale){
		scale[n++]=_scale;
		
	}
};
//M_Point* getPoint(int i, ArrayList arr);
//void addPoint(M_Point* p,ArrayList arr);
Mat powDiagMat(Mat m ,double p);
double tw(double w, double c);
double wi(int i,Mat y,Mat* data,int n,Mat* Hs,double c);
Mat HhInv(Mat y, Mat* data,Mat*Hs,int n, double c );
void	newMeanShift( Mat* data, int p, int n,int sigma_x,int sigma_y,double sigma_s,
					 int maxIter ,double c) ;
void	newMeanShift2( Mat* data,int  p, int n, Mat*& means, int& n_mean,double c ) ;
bool newMeanshiftFromFile(string fname,double c ,int minCsize,Mat* &means,int& n_mean ,int& p_mean);
double mahalanobis(Mat y, Mat yi,Mat H);