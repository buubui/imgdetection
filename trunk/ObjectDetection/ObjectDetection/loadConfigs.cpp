#include "stdafx.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;

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
		getline (conffile,tmp);//window size
		getline (conffile,tmp);
		wndSize.width = atoi(tmp.c_str());
		getline (conffile,tmp);
		wndSize.height = atoi(tmp.c_str());
		getline (conffile,tmp);//Max Window Size
		getline (conffile,tmp);
		maxWndSz.width = atoi(tmp.c_str());
		getline (conffile,tmp);
		maxWndSz.height = atoi(tmp.c_str());

		//		}
	}
	conffile.close();


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
