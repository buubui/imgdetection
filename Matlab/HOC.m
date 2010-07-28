function value = HOC(MatM, MatA, bins)
[r, c] = size(MatM);
value = zeros(1,bins);
Dtheta = 180/bins;
for i =1:bins
    for j =1:r
        for k =1:c
            if(MatA(j,k)< i*Dtheta && MatA(j,k)>=(i-1)*Dtheta)
                value(1,i) = value(1 , i) + MatM(j,k);
            end
        end
    end
end
end