#ifndef _MYPCA_H_
#define _MYPCA_H_
PCA  compressPCA(const  Mat&  pcaset,  int  maxComponents,
				 const  Mat&  testset,  Mat&  compressed);
void backupPCA(PCA& p,std::string filename);
PCA restorePCA(std::string filename);
#endif