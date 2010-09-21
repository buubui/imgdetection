#pragma once
int			meanVec( double *x, double *data, int p, int n, double radius2,
					double *mean );
double		dist( double *A, double *B, int n );
void		meanShift( double data[], int p, int n, double radius, double rate,
					  int maxIter, bool blur, double labels[], double* means ) ;
void	meanShift2( double data[], int p, int n, double radius, double* &means,int& n_mean ) ;
bool meanshiftFromFile(string fname,double radius,int minCsize,double* &means,int& n_mean ,int& p_mean);
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
