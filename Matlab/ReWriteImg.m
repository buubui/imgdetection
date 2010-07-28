function Mat = ReWriteImg(img, cellr, cellc, Dx, Dy, bins)
[i,j] = size(img);
r = floor(i/cellr);
c = floor(j/cellc);
Mat = zeros(1,bins,r,c);
[MatM, MatA] = ComputeWeightGradient(img,Dx,Dy);

for i=1:r
    for j=1:c
        e1=1;
        e2 =1;
        v = HOC(MatM(e1:e1+cellr-1, e2:e2+cellc-1),MatA(e1:e1+cellr-1, e2:e2+cellc),bins);
        e1 = e1+cellr;
        e2 = e2 + cellc;
        Mat(:, :, i,j) = v;
    end
end
end