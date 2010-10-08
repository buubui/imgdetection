function [val,list] = FalseNeg(M, numpos, thr)
val =0;
n = size(M,1);
temp = zeros(n,1);
j =1;
for i=1:numpos
    if M(i,1) < thr
        val = val + 1;
        temp(j,1) = i;
        j = j + 1;
    end
end
list = temp(1:val,:);
end

