function M=calcMeanShift(filename,outfile,b,min)
X=csvread(filename);
[IDX,M]=MeanShift(X,b,.2,100,min,0);
csvwrite(outfile,M);
%quit;