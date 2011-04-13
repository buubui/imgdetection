#include "stdafx.h"
extern Size cellSize,blockSize,wndSize,maxWndSz;
extern cv::Vec2i blockOverlap;
extern cv::Vec2f regionOverlap;
extern float delPart;
extern int useTech;
extern string weightFile;
void loadConfig()
{
	srand(time(NULL));
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
		
		getline (conffile,tmp);//Block overlap
		getline (conffile,tmp);
		blockOverlap[0] = atoi(tmp.c_str());
		getline (conffile,tmp);
		blockOverlap[1] = atoi(tmp.c_str());

		getline (conffile,tmp);//Del part
		getline (conffile,tmp);
		delPart = atof(tmp.c_str());

		getline (conffile,tmp);//Region overlap
		getline (conffile,tmp);
		regionOverlap[0] = atof(tmp.c_str());
		getline (conffile,tmp);
		regionOverlap[1] = atof(tmp.c_str());
		getline (conffile,tmp); // use tech
		getline (conffile,tmp);
		useTech = atoi(tmp.c_str());
		getline (conffile,tmp);//weight file
		getline (conffile,tmp);
		weightFile = tmp;
		printf("tech:%d weight:%s",useTech,weightFile.c_str());
		//		}
	}
	conffile.close();


}


void getWeight(string filename,Mat* &weight, float& b){
	ifstream fi;
	fi.open (filename.c_str());
	string tmp;
	int n;
	if (fi.is_open())
	{
		getline (fi,tmp);
		n = atoi(tmp.c_str());
		weight = new Mat(n,1,DataType<float>::type);
		for (int i=0;i<n;i++)
		{
			getline (fi,tmp);

			weight->at<float>(i,0)=atof(tmp.c_str());
		}
		getline (fi,tmp);

		b=atof(tmp.c_str());
	}

}
