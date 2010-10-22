#include "stdafx.h"
#include "HardSample.h"
#include <cxoperations.hpp>
#include <iostream>

void GenHardSample(string fileHOG, string file_list_index,string file_HardSample)
{
	ifstream readIndex, readline;
	ofstream writeHOG;
	int index, itmp=0;
	string tmp, lineHOG;
	readIndex.open(file_list_index.c_str()); 
	readline.open(file_HardSample.c_str());
	writeHOG.open(fileHOG.c_str(),ios::app);
//	writeHOG<<"\n";
	if(readIndex.is_open())
	{
		while(readIndex.eof()==false)
		{
			getline(readIndex,tmp);
			if(tmp=="")
				continue;
			index = atoi(tmp.c_str());
			int ind = index - itmp;
			for(int i=0;i<ind;i++)
				getline(readline,lineHOG);
			writeHOG<<lineHOG<<"\n";
			itmp = index;
			//fileHOG
		}
	}
	readIndex.close();
	readline.close();
	writeHOG.close();
}