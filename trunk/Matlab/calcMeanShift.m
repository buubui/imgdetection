function M=calcMeanShift(filename,outfile,b)
X=csvread(filename);
[IDX,M]=MeanShift(X,b,.2,100,3,0);
csvwrite(outfile,M);
%quit;