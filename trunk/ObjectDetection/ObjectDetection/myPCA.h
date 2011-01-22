#ifndef _MYPCA_H_
#define _MYPCA_H_
PCA  compressPCA(const  Mat&  pcaset,  int  maxComponents,
				 const  Mat&  testset,  Mat&  compressed)
{
	PCA  pca(pcaset,  //  pass  the  data
		Mat(),  //  we  do  not  have  a  pre-computed  mean  vector,
		//  so  let  the  PCA  engine  to  compute  it
		CV_PCA_DATA_AS_ROW,  //  indicate  that  the  vectors
		//  are  stored  as  matrix  rows
		//  (use  CV_PCA_DATA_AS_COL  if  the  vectors  are
		//  the  matrix  columns)
		maxComponents  //  specify,  how  many  principal  components  to  retain
		);
	//  if  there  is  no  test  data,  just  return  the  computed  basis,  ready-to-use
	if(  !testset.data  )
		return  pca;
	CV_Assert(  testset.cols  ==  pcaset.cols  );
	compressed.create(testset.rows,  maxComponents,  testset.type());
	Mat  reconstructed;
	for(  int  i  =  0;  i  <  testset.rows;  i++  )
	{
		Mat  vec  =  testset.row(i),  coeffs  =  compressed.row(i);
		//  compress  the  vector,  the  result  will  be  stored
		//  in  the  i-th  row  of  the  output  matrix
		pca.project(vec,  coeffs);
		//  and  then  reconstruct  it
		pca.backProject(coeffs,  reconstructed);
		//  and  measure  the  error
		printf("%d.  diff  =  %g\n",  i,  norm(vec,  reconstructed,  NORM_L2));
	}
	return  pca;
};
void backupPCA(PCA& p,std::string filename)
{
	FileStorage fs(filename,FileStorage::WRITE);
	fs<<"eigenvalues"<<p.eigenvalues;
	fs<<"eigenvectors"<<p.eigenvectors;
	fs<<"mean"<<p.mean;
	fs.release();
}
PCA restorePCA(std::string filename)
{
	FileStorage fs(filename,FileStorage::READ);
	PCA p;
	fs["eigenvalues"]>>p.eigenvalues;
	fs["eigenvectors"]>>p.eigenvectors;
	fs["mean"]>>p.mean;
	fs.release();
	return p;
}
#endif