/**************************************************************************
* Piotr's Image&Video Toolbox      Version 2.50
* Copyright 2010 Piotr Dollar.  [pdollar-at-caltech.edu]
* Please email me if you find bugs, or have suggestions or questions!
* Licensed under the Lesser GPL [see external/lgpl.txt]
*************************************************************************/
#include "stdafx.h"
#include "meanshift.h"
#include <math.h>
#include <string.h>


//#include "mex.h"

/**************************************************************************
* Calculates mean of all the points in data that lie on a sphere of
* radius^2==radius2 centered on [1xp] vector x. data is [nxp]. mean
* contains [1xp] result and return is number of points used for calc.
*************************************************************************/
int			meanVec( double *x, double *data, int p, int n, double radius2,
					double *mean ) {
						int i, j; double dist; int cnt=0, m=0;
						for( j=0; j<p; j++ ) mean[j]=0;
						for( i=0; i<n; i++ ) {
							dist = 0.0;
							for( j=0; j<p; j++ ) {
								dist += (x[j]-data[cnt])*(x[j]-data[cnt]); cnt++;
							}
							if( dist < radius2 ) {
								cnt-=p; m++;
								for( j=0; j<p; j++ ) mean[j]+=data[cnt++];
							}
						}
						if( m ) for( j=0; j<p; j++ ) mean[j]/=m;
						return m;
}

/* Squared euclidean distance between two vectors. */
double		dist( double *A, double *B, int n ) {
	double d=0.0; int i;
	for(i=0; i<n; i++) d+=(A[i]-B[i]) * (A[i]-B[i]);
	return d;
}

/**************************************************************************
* data				- p x n column matrix of data points
* p					- dimension of data points
* n					- number of data points
* radius			- radius of search windo
* rate				- gradient descent proportionality factor
* maxIter			- max allowed number of iterations
* blur				- specifies algorithm mode
* labels			- labels for each cluster
* means				- output (final clusters)
*************************************************************************/
void		meanShift( double data[], int p, int n, double radius, double rate,
					  int maxIter, bool blur, double labels[], double* means ) {
						  double radius2;		/* radius^2 */
						  int iter;			/* number of iterations */
						  double *mean;		/* mean vector */
						  int i, j, o, m;		/* looping and temporary variables */
						  int delta = 1;		/* indicator if change occurred between iterations */
						  int *deltas;		/* indicator if change occurred between iterations per point */
						  double *meansCur;	/* calculated means for current iter */
						  double *meansNxt;	/* calculated means for next iter */
						  double *data1;		/* If blur data1 points to meansCur else it points to data */
						  int *consolidated;	/* Needed in the assignment of cluster labels */
						  int nLabels = 1;	/* Needed in the assignment of cluster labels */

						  /* initialization */
						  meansCur = (double*) malloc( sizeof(double)*p*n );
						  meansNxt = (double*) malloc( sizeof(double)*p*n );
						  mean = (double*) malloc( sizeof(double)*p );
						  consolidated = (int*) malloc( sizeof(int)*n );
						  deltas = (int*) malloc( sizeof(int)*n );
						  for(i=0; i<n; i++) deltas[i] = 1;
						  radius2 = radius * radius;
						  meansCur = (double*) memcpy(meansCur, data, p*n*sizeof(double) );
						  if( blur ) data1=meansCur; else data1=data;

						  /* main loop */
			//			  mexPrintf("Progress: 0.000000"); mexEvalString("drawnow;");
						  for(iter=0; iter<maxIter; iter++) {
							  delta = 0;
							  for( i=0; i<n; i++ ) {
								  if( deltas[i] ) {
									  /* shift meansNxt in direction of mean (if m>0) */
									  o=i*p; m=meanVec( meansCur+o, data1, p, n, radius2, mean );
									  if( m ) {
										  for( j=0; j<p; j++ ) meansNxt[o+j] = (1-rate)*meansCur[o+j] + rate*mean[j];
										  if( dist(meansNxt+o, meansCur+o, p)>0.001) delta=1; else deltas[i]=0;
									  } else {
										  for( j=0; j<p; j++ ) meansNxt[o+j] = meansCur[o+j]; deltas[i]=0;
									  }
								  }
							  }
			//				  mexPrintf( "\b\b\b\b\b\b\b\b%f", (float)(iter+1)/maxIter ); mexEvalString("drawnow;");
							  memcpy( meansCur, meansNxt, p*n*sizeof(double) ); 
							  if(!delta) break;
						  }
			//			  mexPrintf( "\n" );

						  /* Consolidate: assign all points that are within radius2 to same cluster. */
						  for( i=0; i<n; i++ ) { consolidated[i]=0; labels[i]=0; }
						  for( i=0; i<n; i++ ) if( !consolidated[i]) {
							  for( j=0; j<n; j++ ) if( !consolidated[j]) {
								  if( dist(meansCur+i*p, meansCur+j*p, p) < radius2) {
									  labels[j]=nLabels; consolidated[j]=1;
								  }
							  }
							  nLabels++;
						  }
						  nLabels--; memcpy( means, meansCur, p*n*sizeof(double) );

						  /* free memory */
						  free(meansNxt); free(meansCur); free(mean); free(consolidated); free(deltas);
}
double round(double d)
{
	double t = d -floor(d);
	if(t<0.5)
		return floor(d);
	else
		return ceil(d);
};
void	meanShift2( double data[], int p, int n, double radius, double* &means,int& n_mean ) 
{
	double label[5];
	double m[4000];
	double tmp[5000];
	meanShift(data,p,n,radius,0.2,100,0,label,m);
//	double* currP=new double[p];
	n_mean=0;
//	for (int i=0;i<p;i++)
//	{
//		currP[i]=0.;
//	}
//	int currN=0;
	bool founded=false;
	for (int i=0;i<n;i++)
	{
		founded=false;
		for (int j=0;j<n_mean;j++)
		{
			
			if(-(radius-1)<=tmp[j*(p+1)+0]-round(m[i*p+0])&& tmp[j*(p+1)+0]-round(m[i*p+0])<=(radius-1) && -(radius-1)<= tmp[j*(p+1)+1]-round(m[i*p+1])&& tmp[j*(p+1)+1]-round(m[i*p+1])<=(radius-1))
			{
				for (int k=2;k<p;k++)
				{
					tmp[j*(p+1)+k] +=m[i*p+k];
				}
				tmp[j*(p+1)+p]++;
				founded=true;
				break;
			}
			
		}
		if(founded==false)
		{
			tmp[n_mean*(p+1)+0] =round(m[i*p+0]);
			tmp[n_mean*(p+1)+1] =round(m[i*p+1]);
			for (int k=2;k<p;k++)
			{
				tmp[n_mean*(p+1)+k] =m[i*p+k];
			}
			tmp[n_mean*(p+1)+p] =1;
			n_mean++;
		}
		
	}
		
	means=new double[n_mean*(p+1)];
	for (int i=0;i<n_mean;i++)
	{
		means[i*(p+1)+0]=tmp[i*(p+1)+0];
		means[i*(p+1)+1]=tmp[i*(p+1)+1];
		for (int j=2;j<p;j++)
		{
			means[i*(p+1)+j]=tmp[i*(p+1)+j]/tmp[i*(p+1)+p];
		}
		means[i*(p+1)+p]=tmp[i*(p+1)+p];
		
	}
	
};
bool meanshiftFromFile(string fname,double minValue,double radius,int minCsize,double* &means,int& n_mean ,int& p_mean)
{
	ifstream in;
	
	in.open(fname.c_str());
	if(!in.is_open())
		return false;
	string tmp;
	int p=0,n=0;
	double data[4000];
	while(1){
		getline (in,tmp);
		std::vector<std::string> strs;
		char* s = (char*)(tmp.c_str());
		boost::split(strs,s , boost::is_any_of(","));
		
		if(p==0){
			p=strs.size();
			p_mean=p+1;
		}
		if (strs.size()<3||strs.size()<p)
			break;
		if(atof(strs[p-1].c_str())<minValue)
			continue;
		for (int i=0;i<p;i++)
		{
			data[n*p+i] = atof(strs[i].c_str());
		}
		n++;

	}
	in.close();
	meanShift2(data,p,n,radius,means,n_mean);
	ofstream out;
//	std::vector<std::string> strs1;
//	char* s1 = (char*)(fname.c_str());
//	boost::split(strs1,s1 , boost::is_ is_any_of("multiscale.txt"));
	string outfname=fname;
	boost::replace_last(outfname, "multiscale", "meanshift");
//	string outfname=strs1[0]+"_meanshift.txt";
	cout<<outfname<<endl;
	out.open(outfname.c_str());
	if(out.is_open()){
		for (int i=0;i<n_mean;i++)
		{
			if(means[i*p_mean+p_mean-1]<minCsize)
				continue;
			for (int j=0;j<p_mean;j++)
			{
				out<<means[i*p_mean+j];
				if(j<p_mean-1)
					out<<", ";
				else
					out<<endl;
			}
		}
	}
	out.close();
	return true;
}
/* see meanShift.m for usage info */
//void		mexFunction( int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[] ) {
//	double radius, rate, *data, *labels, *means; int p, n, maxIter; bool blur;
//
//	/* Check inputs */
//	if(nrhs < 4) mexErrMsgTxt("At least four input arguments required.");
//	if(nlhs > 2) mexErrMsgTxt("Too many output arguments.");
//	if(nrhs==5) blur = mxGetScalar(prhs[4])!=0;
//
//	/* Get inputs */
//	data = mxGetPr(prhs[0]);
//	radius = mxGetScalar(prhs[1]);
//	rate = mxGetScalar(prhs[2]);
//	maxIter = (int) mxGetScalar(prhs[3]);
//	p=mxGetM(prhs[0]); n=mxGetN(prhs[0]);
//
//	/* Create outputs */
//	plhs[0] = mxCreateNumericMatrix(n, 1, mxDOUBLE_CLASS, mxREAL);
//	plhs[1] = mxCreateNumericMatrix(p, n, mxDOUBLE_CLASS, mxREAL);
//	labels=mxGetPr(plhs[0]); means=mxGetPr(plhs[1]);
//
//	/* Do the actual computations in a subroutine */
//	meanShift( data, p, n, radius, rate, maxIter, blur, labels, means );
//}
//M_Point* getPoint(int i, ArrayList arr){
//	M_Point* t = (M_Point*)(int)arr[i];
//	return t;
//};
//void addPoint(M_Point* p,ArrayList arr){
//	int t = (int)p;
//	arr.Add(t);
//};
